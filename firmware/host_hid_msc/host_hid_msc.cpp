////////////////////////////////////////////////////////////////////////
//
// SONY MSX/MSX2 HB-701/HB-F500/HB-F900 USB Keyboard Converter
// Copyright 2026 @V9938
//	
//	26/09/04 V1.0		1st version
//
////////////////////////////////////////////////////////////////////////

// コンパイル時の注意：
// PICO-SDK 2.30に含まれているTINY-USB0.18は、RP2350にはStackするIssueがあります。
// このプログラムは、TINY-USB0.21.0を下記に展開してコンパイルを実施してください。
// ${USERHOME}/.pico-sdk/sdk/2.3.0/lib/tinyusb021"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>

#include "jxglib/Serial.h"
#include "jxglib/LFS/Flash.h"
#include "jxglib/USBHost/HID.h"
#include "jxglib/FAT/USBMSC.h"
#include "jxglib/FS.h"
#include "jxglib/JSON.h"

#include "hardware/uart.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "pico/flash.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/sync.h"
#include "hardware/structs/ioqspi.h"
#include "hardware/structs/sio.h"
#include "hardware/regs/sio.h"
#include "tusb.h"
#include "keymap1.h"
#include "keymap2.h"
#include "keymap3.h"
#include "keymap4.h"
#include "gpadmap0.h"
#include "gpadmap1.h"
#include "gpadmap2.h"
#include "gpadmap3.h"
#include "gpadmap4.h"
#include "ws2812.pio.h"
#include "hbf500_dat_bus.pio.h"

using namespace jxglib;

//RP2354 Flash Setting
//#define PICO_FLASH_SIZE_BYTES (2 * 1024 * 1024)

// 各種設定値
#ifndef PICO_DEFAULT_LED_PIN
#define PICO_DEFAULT_LED_PIN 25
#endif

#define LED_BACKEND_GPIO_RGB 0
#define LED_BACKEND_WS2812   1
#ifndef LED_BACKEND
#define LED_BACKEND LED_BACKEND_WS2812
#endif

#define USER_LED_GREEN 16
#define USER_LED_RED   17
#define USER_LED_BLUE  PICO_DEFAULT_LED_PIN

#ifdef PICO_DEFAULT_WS2812_PIN
#define WS2812_PIN PICO_DEFAULT_WS2812_PIN
#else
#define WS2812_PIN 2
#endif
#define PIO_SM_HBF500  0
#define PIO_SM_LED     3
#define WS2812_SM      PIO_SM_LED
#define WS2812_IS_RGBW false
#define WS2812_ENABLE_POWER_GPIO 0
#define WS2812_POWER_PIN 11
#define WS2812_UPDATE_INTERVAL_MS 20
#define WS2812_POWER_STABILIZE_MS 10

#define SETTING_SEC 5000
#define STR_SEC 60
#define BOOT_LONGPRESS_MS 5000
#define BOOT_POLL_INTERVAL_MS 500
#define BOOT_DEBOUNCE_MS 10

#define CFG_NONE   0   // 通常動作
#define CFG_PRE    1   // 選択キー押下中（SETTING_SEC 待ち）
#define CFG_ACTIVE 2   // 設定確定・キー待ち

// HB-F500 キーマトリクス I/O
// GPIO1-8 = DAT0-7, GPIO9 = CS(入力), GPIO10 = CAPS(入力), GPIO26 = KANA(入力)
// DAT0-3: 常時 HiZ-Low 出力。DAT4-7: CS=LOW 中は入力、CS=HIGH 中は HiZ-Low 出力。
// 出力値は s_keyMatrix[y] (bit=0→LOW、bit=1→HiZ)。
// CAPS/KANA は LOW で LED 点灯 → s_keybordleds の CAPSLOCK/NUMLOCK ビットを更新。
#define HBF500_ENABLE         1

#define HBF500_DAT0_GPIO      3    // DAT0=GPIO3 .. DAT7=GPIO10
#define HBF500_CS_GPIO        11    // CS  (入力、Low active)
#define HBF500_CAPS_GPIO      12   // CAPS LED 信号 (入力、Low=点灯)
#define HBF500_KANA_GPIO      13   // KANA LED 信号 (入力、Low=点灯)

// 32bit GPIO マスク (コンパイル時定数)
#define HBF500_DAT_FULL_MASK  (0xFFu  << HBF500_DAT0_GPIO)          // GPIO3-10
#define HBF500_DAT03_MASK     (0x0Fu  << HBF500_DAT0_GPIO)          // GPIO3-6
#define HBF500_DAT47_MASK     (0x0Fu  << (HBF500_DAT0_GPIO + 4))    // GPIO7-10

// ファームウェアバージョン文字列
#define SW_VERSION "VER0100"

// フラッシュ領域を drive B として公開する USB MSC ボリューム。
static FAT::USBMSC s_usbFat("B");
// 直前のマウント状態を保持して遷移判定に使う。
static bool s_usbMscPrevMounted = false;
// 現在のマウントが解除されるまで、繰り返し処理を抑止する。
static bool s_usbMscIgnoreUntilUnmount = false;

// Inter-core event flags: written by Core1, consumed by Core0
// USB MSC 側から立てられるマウントイベントフラグ。
static volatile bool s_usbMscMountEvent   = false;
// USB MSC 側から立てられるアンマウントイベントフラグ。
static volatile bool s_usbMscUnmountEvent = false;

// デフォルトイメージと実行時 LFS の間で同期する 1 ファイルを表す。
struct SyncFile {
  const char* name;
  const char* defaultContent;
};

// JSON から抽出したキーマップ/ゲームパッド用の設定項目。
struct Setting {
  char id[24];
  uint8_t y[2];
  uint8_t mask[2];
  float value;
};

// 2 スロット分のマトリクスマッピング項目。
struct MatrixTable {
  uint8_t y[2];
  uint8_t mask[2];
};

// 修飾キー、Usage、Shift、ASCII 用の参照テーブル。
static MatrixTable modifierTable[4][8];
static MatrixTable usageTable[4][256];
static MatrixTable shiftTable[4][256];
static MatrixTable asciiTable[4][128];

// ボタン、Hat、軸しきい値を保持するゲームパッドプロファイル表。
struct GamepadMapTable {
  MatrixTable button[13];
  MatrixTable hat[4];      // 0:U 1:D 2:L 3:R
  MatrixTable axisMin[9];  // AxisN <= value
  MatrixTable axisMax[9];  // AxisN >= value
  float axisMinValue[9];
  float axisMaxValue[9];
};

// 現在のゲームパッドマッピング状態とキャッシュ済みプロファイル。
static GamepadMapTable s_gamepadMap = {};
static bool s_gamepadMapLoaded = false;
static bool s_gamePadMounted = false;
static bool s_prevButtonPressed[13] = {};
static bool s_prevHatPressed[4] = {};
static bool s_prevAxisMinPressed[9] = {};
static bool s_prevAxisMaxPressed[9] = {};

// 起動時や同期時に LFS へ反映する対象ファイル。
static const SyncFile kFilesToSync[] = {
  {"KEYMAP0.JSN", JSON_DEF0},
  {"KEYMAP1.JSN", JSON_DEF1},
  {"KEYMAP2.JSN", JSON_DEF2},
  {"KEYMAP3.JSN", JSON_DEF3},
  {"GPADMAP0.JSN", JSON_GPAD_DEF0},
  {"GPADMAP1.JSN", JSON_GPAD_DEF1},
  {"GPADMAP2.JSN", JSON_GPAD_DEF2},
  {"GPADMAP3.JSN", JSON_GPAD_DEF3},
  {"GPADMAP4.JSN", JSON_GPAD_DEF4},
  {"VIDCFG.INI",  "# VID(hex),PID(hex),ProfileFileName\n"},  // VID/PID → プロファイル対応表
};

// USB MSC 同期処理のフェーズ。
enum class UsbMscPhase {
  Idle,
  BeginMount,
  ListOpen,
  ListNext,
  PrepareFileOps,
  ResetFormat,
  ResetRemount,
  ResetCreateFiles,
  CopyFiles,
};

// USB MSC 同期処理の状態。
struct UsbMscTaskState {
  UsbMscPhase phase = UsbMscPhase::Idle;
  bool fatMounted = false;
  bool aMounted = false;
  bool resetRequested = false;
  bool selfTestRequested = false;
  bool lfsDataUpdated = false;
  int fileIdx = 0;
  FS::Dir* pDir = nullptr;
};

// Keyboard HID report snapshot: modifier + 6 keycodes (1 frame from USBHost::Keyboard)
// USBHost::Keyboard から取得した 1 フレーム分の入力状態
struct KeyboardReport {
  uint8_t modifier;
  uint8_t keycode[6];
};

// Board LED ブリンクパターン（未接続/接続/サスペンド）
enum {
    BLINK_NOT_MOUNTED = 250,
    BLINK_MOUNTED     = 1000,
    BLINK_SUSPENDED   = 2500,
    BLINK_FAST        = 100,
};

// VID/PID → プロファイルファイル名テーブルエントリ（VIDCFG.INI 対応）
// VID/PID とプロファイルファイル名の対応 1 件を保持する。
struct keyconfigTable {
    uint16_t vid;
    uint16_t pid;
    char     profileFileName[32]; // 例: KEYMAP0.JSN / GPADMAP0.JSN
};

// USB MSC タスク状態機械のグローバル実体。
static UsbMscTaskState s_usbMscTask;

// 1-hot のマスク値をビット番号に変換する。
static uint8_t mask2bitNum(uint8_t maskByte)
{
  uint8_t num = 0;
  uint8_t mask = maskByte;
  for (num = 0; num < 8; num++) {
    mask = static_cast<uint8_t>(mask >> 1);
    if (mask == 0) break;
  }
  return num;
}

// 1 プロファイル分の修飾キー設定を初期化する。
static void modifierTableInit(int x)
{
  for (int table = 0; table < 8; table++) {
    modifierTable[x][table].mask[0] = 0;
    modifierTable[x][table].mask[1] = 0;
    modifierTable[x][table].y[0] = 0;
    modifierTable[x][table].y[1] = 0;
  }
}

// 1 プロファイル分の Usage/Shift 設定を初期化する。
static void usageTableInit(int x)
{
  for (int table = 0; table < 256; table++) {
    usageTable[x][table].mask[0] = 0;
    shiftTable[x][table].mask[0] = 0;
    usageTable[x][table].mask[1] = 0;
    shiftTable[x][table].mask[1] = 0;
    usageTable[x][table].y[0] = 0;
    shiftTable[x][table].y[0] = 0;
    usageTable[x][table].y[1] = 0;
    shiftTable[x][table].y[1] = 0;
  }
}

// 1 プロファイル分の ASCII 設定を初期化する。
static void asciiTableInit(int x)
{
  for (int table = 0; table < 128; table++) {
    asciiTable[x][table].mask[0] = 0;
    asciiTable[x][table].mask[1] = 0;
    asciiTable[x][table].y[0] = 0;
    asciiTable[x][table].y[1] = 0;
  }
}

// "Keymap" オブジェクトから指定セクションを抽出する JSON パーサ。
// SAX-style JSON parser that extracts one named section (e.g. "Modifier", "Usage")
// from the "Keymap" object and fills a Setting array.
//
// Depth tracking uses iGroupStack_ from the JSON base class, which is incremented
// before On*Start callbacks fire and decremented before On*End callbacks fire.
// A saved depth value > 0 means "currently inside that scope".
//   keymapDepth_     : iGroupStack_ when "Keymap" object started (0 = not inside)
//   targetArrayDepth_: iGroupStack_ when the target section array started (0 = not inside)
//   settingDepth_    : iGroupStack_ when a per-entry object started (0 = not inside)
class KeymapSectionParser : public JSON {
private:
  enum class YMask { None, Y, Mask };
private:
  const char* rootName_;
  Setting* settings_;
  size_t maxSettings_;
  size_t count_;
  bool parseError_;
  bool foundTargetArray_;
  int keymapDepth_;
  int targetArrayDepth_;
  int settingDepth_;
  YMask ymask_;
  int ymaskIndex_;
  Setting current_;
public:
  KeymapSectionParser(const char* rootName, Setting* settings, size_t maxSettings) :
    rootName_(rootName), settings_(settings), maxSettings_(maxSettings),
    count_(0), parseError_(false), foundTargetArray_(false),
    keymapDepth_(0), targetArrayDepth_(0), settingDepth_(0),
    ymask_(YMask::None), ymaskIndex_(0), current_{}
  {}
public:
  size_t GetCount() const { return count_; }
  bool HasParseError() const { return parseError_; }
  bool FoundTargetArray() const { return foundTargetArray_; }
private:
  static uint8_t to_uint8(double num)
  {
    if (num <= 0.0) return 0;
    if (num >= 255.0) return 255;
    return static_cast<uint8_t>(num);
  }
  void beginSetting()
  {
    settingDepth_ = iGroupStack_;   // iGroupStack_ already at new depth
    current_ = Setting{};
    ymask_ = YMask::None;
    ymaskIndex_ = 0;
  }
  void endSetting()
  {
    if (count_ < maxSettings_) settings_[count_++] = current_;
    else parseError_ = true;
    settingDepth_ = 0;
    ymask_ = YMask::None;
    ymaskIndex_ = 0;
  }
public:
  virtual void OnString(const char* /*str*/) override {}
  virtual void OnStringNamed(const char* objectName, const char* str) override
  {
    if (settingDepth_ == 0) return;
    if (::strcmp(objectName, "Id") == 0)
      ::snprintf(current_.id, sizeof(current_.id), "%s", str);
    else if (::strcmp(objectName, "Value") == 0) {
      char* endptr = nullptr;
      current_.value = static_cast<float>(::strtol(str, &endptr, 0));
    }
  }
  virtual void OnNumber(double num) override
  {
    if (settingDepth_ == 0) return;
    if (ymask_ == YMask::Y && ymaskIndex_ < 2)
      current_.y[ymaskIndex_++] = to_uint8(num);
    else if (ymask_ == YMask::Mask && ymaskIndex_ < 2)
      current_.mask[ymaskIndex_++] = to_uint8(num);
  }
  virtual void OnNumberNamed(const char* objectName, double num) override
  {
    if (settingDepth_ == 0) return;
    // scalar Y or Mask (non-array form)
    if (::strcmp(objectName, "Y") == 0)
      { current_.y[0] = to_uint8(num); current_.y[1] = 0; }
    else if (::strcmp(objectName, "Mask") == 0)
      { current_.mask[0] = to_uint8(num); current_.mask[1] = 0; }
    else if (::strcmp(objectName, "Value") == 0)
      { current_.value = static_cast<float>(num); }
  }
  virtual void OnSymbol(const char* /*symbol*/) override {}
  virtual void OnSymbolNamed(const char* /*objectName*/, const char* /*symbol*/) override {}
  virtual void OnObjectStart() override
  {
    // Unnamed object inside target array ?Enew setting entry
    if (targetArrayDepth_ > 0 && settingDepth_ == 0)
      beginSetting();
  }
  virtual void OnObjectStartNamed(const char* objectName) override
  {
    if (keymapDepth_ == 0 && ::strcmp(objectName, "Keymap") == 0)
      keymapDepth_ = iGroupStack_;
    else if (targetArrayDepth_ > 0 && settingDepth_ == 0)
      beginSetting();
  }
  virtual void OnObjectEnd() override
  {
    // iGroupStack_ is already decremented; compare against saved depth - 1
    if (settingDepth_ > 0 && iGroupStack_ == settingDepth_ - 1)
      endSetting();
    if (keymapDepth_ > 0 && iGroupStack_ == keymapDepth_ - 1)
      keymapDepth_ = 0;
  }
  virtual void OnArrayStart() override {}
  virtual void OnArrayStartNamed(const char* objectName) override
  {
    if (keymapDepth_ > 0 && targetArrayDepth_ == 0 && ::strcmp(objectName, rootName_) == 0) {
      targetArrayDepth_ = iGroupStack_;
      foundTargetArray_ = true;
    } else if (settingDepth_ > 0) {
      if (::strcmp(objectName, "Y") == 0)
        { ymask_ = YMask::Y;    ymaskIndex_ = 0; }
      else if (::strcmp(objectName, "Mask") == 0)
        { ymask_ = YMask::Mask; ymaskIndex_ = 0; }
    }
  }
  virtual void OnArrayEnd() override
  {
    if (targetArrayDepth_ > 0 && iGroupStack_ == targetArrayDepth_ - 1)
      targetArrayDepth_ = 0;
    ymask_ = YMask::None;
    ymaskIndex_ = 0;
  }
};

// Parses a named JSON section into a Setting array.
static int parse_settings(const char* json_string, const char* root_string, Setting* settings, size_t max_settings)
{
  KeymapSectionParser parser(root_string, settings, max_settings);
  if (!parser.Parse(json_string)) {
    ::printf("[LFS] %s parse failed: %s\n", root_string, parser.GetErrorMsg());
    return -1;
  }
  if (parser.HasParseError()) {
    ::printf("[LFS] %s parse failed: too many settings\n", root_string);
    return -1;
  }
  if (!parser.FoundTargetArray()) {
    ::printf("[LFS] %s parse failed: section not found\n", root_string);
    return -1;
  }
  return static_cast<int>(parser.GetCount());
}

// ファイル全体をヒープ確保した NUL 終端バッファへ読み込む。
static std::unique_ptr<char[]> read_file_text(const char* pathName)
{
  std::unique_ptr<FS::FileInfo> pFileInfo(FS::GetFileInfo(pathName));
  if (!pFileInfo || pFileInfo->IsDirectory()) return nullptr;
  const size_t fileSize = static_cast<size_t>(pFileInfo->GetSize());
  std::unique_ptr<char[]> buff(new char[fileSize + 1]);
  std::unique_ptr<FS::File> pFile(FS::OpenFile(pathName, "r"));
  if (!pFile) return nullptr;
  size_t total = 0;
  while (total < fileSize) {
    int n = pFile->Read(buff.get() + total, static_cast<int>(fileSize - total));
    if (n < 0) return nullptr;
    if (n == 0) break;
    total += static_cast<size_t>(n);
  }
  buff[total] = '\0';
  return buff;
}

// 1 つのキーマップファイルを読み込み、プロファイル用の参照テーブルを埋める。
static int readAndSetKeymap(const char* pathName, int settingTableNumber)
{
  if (settingTableNumber < 0 || settingTableNumber >= 4) return -1;
  std::unique_ptr<char[]> fileReadBuffer(read_file_text(pathName));
  if (!fileReadBuffer) {
    ::printf("[LFS] %s: open/read failed\n", pathName);
    return -1;
  }

  Setting settings_a[8];
  Setting settings_u[256];
  char* endptr = nullptr;
  int table = 0;

  modifierTableInit(settingTableNumber);
  int num_settings = parse_settings(fileReadBuffer.get(), "Modifier", settings_a, count_of(settings_a));
  if (num_settings < 0) return -1;

  for (table = 0; table < num_settings; table++) {
    uint8_t decimal_value = static_cast<uint8_t>(::strtol(settings_a[table].id, &endptr, 0));
    if (decimal_value == 0 || (decimal_value & (decimal_value - 1)) != 0) continue;
    uint8_t modifierIndex = mask2bitNum(decimal_value);
    if (modifierIndex >= 8) continue;
    modifierTable[settingTableNumber][modifierIndex].mask[0] = mask2bitNum(settings_a[table].mask[0]);
    modifierTable[settingTableNumber][modifierIndex].mask[1] = mask2bitNum(settings_a[table].mask[1]);
    modifierTable[settingTableNumber][modifierIndex].y[0] = settings_a[table].y[0];
    modifierTable[settingTableNumber][modifierIndex].y[1] = settings_a[table].y[1];
  }

  usageTableInit(settingTableNumber);
  num_settings = parse_settings(fileReadBuffer.get(), "Usage", settings_u, count_of(settings_u));
  if (num_settings < 0) return -1;
  for (table = 0; table < num_settings; table++) {
    uint8_t decimal_value = static_cast<uint8_t>(::strtol(settings_u[table].id, &endptr, 0));
    usageTable[settingTableNumber][decimal_value].mask[0] = mask2bitNum(settings_u[table].mask[0]);
    usageTable[settingTableNumber][decimal_value].mask[1] = mask2bitNum(settings_u[table].mask[1]);
    usageTable[settingTableNumber][decimal_value].y[0] = settings_u[table].y[0];
    usageTable[settingTableNumber][decimal_value].y[1] = settings_u[table].y[1];

    if (settings_u[table].y[1] == 0) {
      if (modifierTable[settingTableNumber][1].y[0] != 0) {
        shiftTable[settingTableNumber][decimal_value].mask[0] = modifierTable[settingTableNumber][1].mask[0];
        shiftTable[settingTableNumber][decimal_value].y[0] = modifierTable[settingTableNumber][1].y[0];
        shiftTable[settingTableNumber][decimal_value].mask[1] = mask2bitNum(settings_u[table].mask[0]);
        shiftTable[settingTableNumber][decimal_value].y[1] = settings_u[table].y[0];
      } else {
        shiftTable[settingTableNumber][decimal_value].mask[0] = mask2bitNum(settings_u[table].mask[0]);
        shiftTable[settingTableNumber][decimal_value].y[0] = settings_u[table].y[0];
        shiftTable[settingTableNumber][decimal_value].mask[1] = mask2bitNum(settings_u[table].mask[1]);
        shiftTable[settingTableNumber][decimal_value].y[1] = settings_u[table].y[1];
      }
    } else {
      shiftTable[settingTableNumber][decimal_value].mask[0] = mask2bitNum(settings_u[table].mask[0]);
      shiftTable[settingTableNumber][decimal_value].y[0] = settings_u[table].y[0];
      shiftTable[settingTableNumber][decimal_value].mask[1] = mask2bitNum(settings_u[table].mask[1]);
      shiftTable[settingTableNumber][decimal_value].y[1] = settings_u[table].y[1];
    }
  }

  num_settings = parse_settings(fileReadBuffer.get(), "Shift", settings_u, count_of(settings_u));
  if (num_settings > 0) {
    for (table = 0; table < num_settings; table++) {
      uint8_t decimal_value = static_cast<uint8_t>(::strtol(settings_u[table].id, &endptr, 0));
      shiftTable[settingTableNumber][decimal_value].mask[0] = mask2bitNum(settings_u[table].mask[0]);
      shiftTable[settingTableNumber][decimal_value].mask[1] = mask2bitNum(settings_u[table].mask[1]);
      shiftTable[settingTableNumber][decimal_value].y[0] = settings_u[table].y[0];
      shiftTable[settingTableNumber][decimal_value].y[1] = settings_u[table].y[1];
    }
  }

  asciiTableInit(settingTableNumber);
  num_settings = parse_settings(fileReadBuffer.get(), "ASCII", settings_u, count_of(settings_u));
  if (num_settings < 0) return -1;
  for (table = 0; table < num_settings; table++) {
    uint8_t decimal_value = static_cast<uint8_t>(::strtol(settings_u[table].id, &endptr, 0));
    if (decimal_value >= 128) continue;
    asciiTable[settingTableNumber][decimal_value].mask[0] = mask2bitNum(settings_u[table].mask[0]);
    asciiTable[settingTableNumber][decimal_value].mask[1] = mask2bitNum(settings_u[table].mask[1]);
    asciiTable[settingTableNumber][decimal_value].y[0] = settings_u[table].y[0];
    asciiTable[settingTableNumber][decimal_value].y[1] = settings_u[table].y[1];
  }
  ::printf("[LFS] done: readAndSetKeymap(%s, %d)\n", pathName, settingTableNumber);
  return 0;
}

// プロファイルごとのゲームパッドのエッジ検出状態を初期化する。
static void reset_gamepad_runtime_state()
{
  ::memset(s_prevButtonPressed, 0x00, sizeof(s_prevButtonPressed));
  ::memset(s_prevHatPressed, 0x00, sizeof(s_prevHatPressed));
  ::memset(s_prevAxisMinPressed, 0x00, sizeof(s_prevAxisMinPressed));
  ::memset(s_prevAxisMaxPressed, 0x00, sizeof(s_prevAxisMaxPressed));
}

// ゲームパッドマッピング領域とデフォルトしきい値を初期化する。
static void init_gamepad_map()
{
  ::memset(&s_gamepadMap, 0x00, sizeof(s_gamepadMap));
  for (int i = 0; i < 9; i++) {
    s_gamepadMap.axisMinValue[i] = -1.0f;
    s_gamepadMap.axisMaxValue[i] = 1.0f;
  }
  s_gamepadMapLoaded = false;
  reset_gamepad_runtime_state();
}

// 解析済み設定をマトリクスマッピング項目へコピーする。
static void set_matrix_from_setting(MatrixTable& dst, const Setting& s)
{
  dst.y[0] = s.y[0];
  dst.y[1] = s.y[1];
  dst.mask[0] = mask2bitNum(s.mask[0]);
  dst.mask[1] = mask2bitNum(s.mask[1]);
}

// 1 つのゲームパッドプロファイルを読み込み、実行時参照テーブルへ反映する。
static int readAndSetGamepadMap(const char* pathName)
{
  std::unique_ptr<char[]> fileReadBuffer(read_file_text(pathName));
  if (!fileReadBuffer) {
    ::printf("[LFS] %s: open/read failed\n", pathName);
    s_gamepadMapLoaded = false;
    return -1;
  }

  Setting settings_g[128];
  init_gamepad_map();
  int num_settings = parse_settings(fileReadBuffer.get(), "GamePad", settings_g, count_of(settings_g));
  // 既存 GPADMAP0.JSN 互換: Keymap.GamePad が無い場合は Keymap.Usage を使う
  if (num_settings < 0) {
    num_settings = parse_settings(fileReadBuffer.get(), "Usage", settings_g, count_of(settings_g));
    if (num_settings < 0) return -1;
  }

  for (int i = 0; i < num_settings; i++) {
    const char* id = settings_g[i].id;
    int index = -1;

    if (::sscanf(id, "Button%d", &index) == 1 && index >= 0 && index < 13) {
      set_matrix_from_setting(s_gamepadMap.button[index], settings_g[i]);
      continue;
    }
    if (::strcmp(id, "HatSwitch_U") == 0) { set_matrix_from_setting(s_gamepadMap.hat[0], settings_g[i]); continue; }
    if (::strcmp(id, "HatSwitch_D") == 0) { set_matrix_from_setting(s_gamepadMap.hat[1], settings_g[i]); continue; }
    if (::strcmp(id, "HatSwitch_L") == 0) { set_matrix_from_setting(s_gamepadMap.hat[2], settings_g[i]); continue; }
    if (::strcmp(id, "HatSwitch_R") == 0) { set_matrix_from_setting(s_gamepadMap.hat[3], settings_g[i]); continue; }

    if (::sscanf(id, "Axis%d", &index) == 1 && index >= 0 && index < 9) {
      const float v = settings_g[i].value;
      if (v >= 0.0f) {
        // 正の閾値: axis >= v でON (axisMax スロット)
        set_matrix_from_setting(s_gamepadMap.axisMax[index], settings_g[i]);
        s_gamepadMap.axisMaxValue[index] = v;
      } else {
        // 負の閾値: axis <= v でON (axisMin スロット)
        set_matrix_from_setting(s_gamepadMap.axisMin[index], settings_g[i]);
        s_gamepadMap.axisMinValue[index] = v;
      }
      continue;
    }
  }

  s_gamepadMapLoaded = true;
  ::printf("[LFS] done: readAndSetGamepadMap(%s)\n", pathName);
  return 0;
}

// マトリクス項目に少なくとも 1 つの有効スロットがあれば true。
static bool has_matrix_value(const MatrixTable& matrixTable)
{
  return matrixTable.mask[0] != 0 || matrixTable.mask[1] != 0 || matrixTable.y[0] != 0 || matrixTable.y[1] != 0;
}

// Dumps the currently loaded keymap tables for a profile.
static void print_profile_tables(int profileNo)
{
  ::printf("=== KEYMAP PROFILE %d ===\n", profileNo);
//  ::printf("[Modifier]\n");
//  for (int i = 0; i < 8; i++) {
//    const MatrixTable& m = modifierTable[profileNo][i];
//    ::printf("  mod[%d] mask=[%u,%u] y=[%u,%u]\n", i, m.mask[0], m.mask[1], m.y[0], m.y[1]);
//  }

  int usageCount = 0;
//  ::printf("[Usage]\n");
  for (int i = 0; i < 256; i++) {
    const MatrixTable& m = usageTable[profileNo][i];
    if (!has_matrix_value(m)) continue;
    usageCount++;
//    ::printf("  usage[%d] mask=[%u,%u] y=[%u,%u]\n", i, m.mask[0], m.mask[1], m.y[0], m.y[1]);
  }

  int shiftCount = 0;
//  ::printf("[Shift]\n");
  for (int i = 0; i < 256; i++) {
    const MatrixTable& m = shiftTable[profileNo][i];
    if (!has_matrix_value(m)) continue;
    shiftCount++;
//    ::printf("  shift[%d] mask=[%u,%u] y=[%u,%u]\n", i, m.mask[0], m.mask[1], m.y[0], m.y[1]);
  }

  int asciiCount = 0;
//  ::printf("[ASCII]\n");
  for (int i = 0; i < 128; i++) {
    const MatrixTable& m = asciiTable[profileNo][i];
    if (!has_matrix_value(m)) continue;
    asciiCount++;
//    ::printf("  ascii[%d] mask=[%u,%u] y=[%u,%u]\n", i, m.mask[0], m.mask[1], m.y[0], m.y[1]);
  }
  // Summary のみ表示
  ::printf("[Summary] usage=%d shift=%d ascii=%d\n", usageCount, shiftCount, asciiCount);
}

// 前方宣言
static int updateConfigNumberCSV(const char* filename, uint16_t vidNum, uint16_t pidNum, const char* profileFileName);
static void reset_modifier_transition_state();
static void reload_lfs_runtime_data(const char* logPrefix);

// Dynamic VID/PID table loaded from VIDCFG.INI.
static keyconfigTable* s_pidTable     = nullptr; // VID/PID テーブル（動的確保）
// Number of valid entries currently stored in the table.
static int             s_pidMaxNumber  = 0;         // テーブル有効件数
// Number of entries allocated for the table.
static int             s_pidCapacity   = 0;         // 確保済みエントリ数
// Connected keyboard VID.
static uint16_t       s_vid            = 0;  // 接続中キーボードの VID
// Connected keyboard PID.
static uint16_t       s_pid            = 0;  // 接続中キーボードの PID
// Keyboard mount state.
static bool           s_keyboardMounted = false;  // キーボード接続状態
// Connected gamepad VID.
static uint16_t       s_gvid           = 0;  // 接続中 GamePad の VID
// Connected gamepad PID.
static uint16_t       s_gpid           = 0;  // 接続中 GamePad の PID
// 現在のゲームパッドプロファイル番号。
static int            s_gpadConfigSettingNumber = 0; // 使用中 GamePad プロファイル番号

// キーボード LED ビットフィールド（CAPSLOCK/NUMLOCK/SCROLLLOCK）
// Keyboard LED bitfield mirrored from the host report.
static uint8_t        s_keybordleds     = 0;
// Board LED 点滅間隔 [ms]（BLINK_* 定数で更新）
// ボード LED の点滅間隔。
static uint32_t       s_blink_interval_ms = BLINK_NOT_MOUNTED;
// True while file sync or reload activity is in progress.
static bool           s_fileAccessActive = false;
// True when a file error or parse error has been detected.
static bool           s_fileErrorActive = false;

#if LED_BACKEND == LED_BACKEND_WS2812
// PIO instance used by the WS2812 backend.
static PIO            s_ws2812Pio = pio0;
// Offset of the loaded WS2812 PIO program.
static uint           s_ws2812Offset = 0;
// Timestamp of the last WS2812 update.
static uint32_t       s_ws2812LastUpdateMs = 0;
// Last pixel value written to the WS2812 chain.
static uint32_t       s_ws2812LastPixel = 0xFFFFFFFFu;
#endif

// 参照するキーマッププロファイル番号
// Active keyboard profile number.
static int s_configSettingNumber = 0;

// configMode 関連
// 現在の設定モード状態。
static uint8_t  s_configMode      = CFG_NONE; // 現在の設定モード
// Selected profile number during config mode.
static uint8_t  s_numProfile      = 0;        // 選択中のプロファイル番号
// Enables key-ID dump output.
static bool     s_keyboardIDMode  = false;    // キーID ダンプ出力フラグ

// keyboardStrTask 関連: プロファイル確定/VERSION 確定後に文字列をマトリクス出力する
// String queued for matrix emission.
static std::unique_ptr<char[]> s_keyboardStr;       // 送出する ASCII 文字列（動的確保）
// True while keyboardStrTask is actively sending.
static bool                    s_keyboardStrMode = false; // true 中は keyboardStrTask() が送出中
// Holds until all keys are released after a string transfer.
static bool                    s_keyboardStrWaitRelease = false; // 文字列送出後、全キー離上まで入力無効
// Marks that pre-send clearing has not been performed yet.
static bool                    s_keyboardStrClearPending = false; // 送出開始前クリア未実行フラグ
// Tracks whether the latest report still has any pressed key.
static bool                    s_keyboardAnyKeyPressed = false; // 最新レポートでキー押下中か
// Previous keyboard report used for edge detection.
static KeyboardReport          s_prevKeyboardReport = {0, {0, 0, 0, 0, 0, 0}}; // 入力遷移用の前回レポート
// BOOT button press start timestamp.
static uint32_t                s_bootButtonPressStartMs = 0; // BOOT long-press start time
// Guards the BOOT long-press handling from repeating.
static bool                    s_bootLongPressHandled = false; // prevent repeated BOOT long-press reset
// BOOTSEL 読み取り失敗の通知状態。
static bool                    s_bootselReadFaultActive = false;
// BOOTSEL の生値が最後に変化した時刻。
static uint32_t                s_bootselLastChangeMs = 0;
// BOOTSEL の前回生サンプル値。
static bool                    s_bootselRawPressed = false;
// デバウンス後の BOOTSEL 押下状態。
static bool                    s_bootselDebouncedPressed = false;
// BOOTSEL ポーリングの前回実行時刻。
static uint32_t                s_bootselLastPollMs = 0;
// 一度離されるまで長押し判定を有効化しないためのアーム状態。
static bool                    s_bootselLongPressArmed = false;

// USB メモリ上の selftest.do により有効化される HB-F500 Y 入力セルフテスト状態。
static volatile bool           s_hbf500SelfTestActive = false;
static volatile uint16_t       s_hbf500SelfTestYCount[11] = {0};
static bool                    s_hbf500SelfTestPassed = false;
static bool                    s_hbf500SelfTestPassPending = false;

// modifier 遷移検出の内部状態（keyboardStrTask 前後でリセット可能にする）
// Previous modifier state for the valid-B slot.
static uint8_t s_modOldValidB  = 0;
// Previous shift state for the A slot.
static uint8_t s_modOldShiftA  = 0;
// Previous modifier state for the A slot.
static uint8_t s_modOldA       = 0;
// Previous shift state for the B slot.
static uint8_t s_modOldShiftB  = 0;
// Previous modifier state for the B slot.
static uint8_t s_modOldB       = 0;

// MSX キーマトリクスの押下状態 (Y0..Y11, X0..X7 をビット保持)
// Pressed-state bitmap for the MSX matrix.
static uint8_t s_msxMatrixPressed[12] = {0};

// HB-F500 向けキーマトリクス出力バッファ
// 初期値 0xFF = 全キー離し。bit=0 でキー押下、bit=1 でキー離し (HiZ-Low 出力用)
// Output buffer for HB-F500 key matrix lines.
static volatile uint8_t s_keyMatrix[12];
static_assert(count_of(s_keyMatrix) == count_of(s_msxMatrixPressed), "array size mismatch");

// MSX キーマトリクス表 (内部は x=0..7 / y=0..11)
// 呼び出し側の y は 1 始まりなので、利用時に (y - 1) へ変換する。
// Human-readable labels for matrix positions.
static const char* kMsxKeyNameTable[12][8] = {
  // Y0
  {"0", "1", "2", "3", "4", "5", "6", "7"},
  // Y1
  {"8", "9", "-", "^", "\\", "@", "[", "+"},
  // Y2
  {"*", "]", "<", ">", "/", "_", "A", "B"},
  // Y3
  {"C", "D", "E", "F", "G", "H", "I", "J"},
  // Y4
  {"K", "L", "M", "N", "O", "P", "Q", "R"},
  // Y5
  {"S", "T", "U", "V", "W", "X", "Y", "Z"},
  // Y6
  {"SHIFT", "CTRL", "GRAPH", "CAPS", "KANA", "F1", "F2", "F3"},
  // Y7
  {"F4", "F5", "ESC", "TAB", "STOP", "BS", "SELECT", "RETURN"},
  // Y8
  {"SPACE", "CLS", "INS", "DEL", "LEFT", "UP", "DOWN", "RIGHT"},
  // Y9 (テンキー)
  {"*", "+", "/", "0", "1", "2", "3", "4"},
  // Y10 (テンキー)
  {"5", "6", "7", "8", "9", "-", ",", "."},
  // Y11 (一部機種のみ)
  {nullptr, "JIKKOU", nullptr, "TORIKESHI", nullptr, nullptr,nullptr ,nullptr },
};

// Prints the currently selected MSX matrix keys.
static void print_msx_selected_keys()
{
  bool hasAny = false;
  for (int y = 0; y < static_cast<int>(count_of(s_msxMatrixPressed)); ++y) {
    if (s_msxMatrixPressed[y] != 0) {
      hasAny = true;
      break;
    }
  }
  if (!hasAny) {
    ::printf("[MSXKEY] none\n");
    return;
  }

  ::printf("[MSXKEY]");
  for (int y = 0; y < static_cast<int>(count_of(s_msxMatrixPressed)); ++y) {
    for (int x = 7; x >= 0; --x) {
      if (s_msxMatrixPressed[y] & (1u << x)) {
        const char* keyName = kMsxKeyNameTable[y][x];
        if (keyName != nullptr) {
          ::printf(" [%s]", keyName);
        }
      }
    }
  }
  ::printf("\n");
}

// MSX マトリクス 1 位置の押下ビットマップを更新する。
static void update_msx_selected_keys(uint8_t y, uint8_t x, bool pressed)
{
  if (y == 0) return;
  uint8_t yIndex = static_cast<uint8_t>(y - 1);
  if (yIndex >= static_cast<uint8_t>(count_of(s_msxMatrixPressed))) return;
  if (x >= 8) return;
  uint8_t bit = static_cast<uint8_t>(1u << x);
  if (pressed) {
    s_msxMatrixPressed[yIndex] = static_cast<uint8_t>(s_msxMatrixPressed[yIndex] | bit);
  } else {
    s_msxMatrixPressed[yIndex] = static_cast<uint8_t>(s_msxMatrixPressed[yIndex] & static_cast<uint8_t>(~bit));
  }
  print_msx_selected_keys();
}

// 押下ビットマップと HB-F500 の出力バッファをクリアする。
static void clear_msx_selected_keys()
{
  ::memset(s_msxMatrixPressed, 0x00, sizeof(s_msxMatrixPressed));
  ::memset(const_cast<uint8_t*>(s_keyMatrix), 0xFF, sizeof(s_keyMatrix));
}

// MSX マトリクスのいずれかのキーが押下中なら true。
static bool has_any_msx_selected_key()
{
  for (int y = 0; y < static_cast<int>(count_of(s_msxMatrixPressed)); ++y) {
    if (s_msxMatrixPressed[y] != 0) return true;
  }
  return false;
}

// 三角波のように変化するブレス輝度を計算する。
static uint8_t calc_breath_brightness(uint32_t nowMs, uint32_t halfCycleMs)
{
  if (halfCycleMs == 0) return 255;
  const uint32_t fullCycleMs = halfCycleMs * 2u;
  const uint32_t phaseMs = nowMs % fullCycleMs;
  if (phaseMs < halfCycleMs) {
    return static_cast<uint8_t>((255u * phaseMs) / halfCycleMs);
  }
  return static_cast<uint8_t>((255u * (fullCycleMs - phaseMs)) / halfCycleMs);
}

// RGB 成分を WS2812 向けの GRB 形式へ詰める。
static inline uint32_t led_rgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
  return (static_cast<uint32_t>(g) << 16) |
         (static_cast<uint32_t>(r) << 8) |
         static_cast<uint32_t>(b);
}

#if LED_BACKEND == LED_BACKEND_WS2812
// WS2812 チェーンへ 1 ピクセル送信する。
static inline void put_pixel(uint32_t pixelGrb)
{
  pio_sm_put_blocking(s_ws2812Pio, WS2812_SM, pixelGrb << 8u);
}
#endif

// 現在の LED バックエンドを初期化する。
static void led_backend_init()
{
#if LED_BACKEND == LED_BACKEND_WS2812
  #if WS2812_ENABLE_POWER_GPIO
  gpio_init(WS2812_POWER_PIN);
  gpio_set_dir(WS2812_POWER_PIN, GPIO_OUT);
  gpio_put(WS2812_POWER_PIN, 1);
  sleep_ms(WS2812_POWER_STABILIZE_MS);
  #endif
  s_ws2812Offset = pio_add_program(s_ws2812Pio, &ws2812_program);
  ws2812_program_init(s_ws2812Pio, WS2812_SM, s_ws2812Offset, WS2812_PIN, 800000, WS2812_IS_RGBW);
  s_ws2812LastUpdateMs = to_ms_since_boot(get_absolute_time());
  s_ws2812LastPixel = 0xFFFFFFFFu;
  put_pixel(led_rgb_u32(0, 0, 0));
#else
  const uint ledPins[] = {USER_LED_BLUE, USER_LED_GREEN, USER_LED_RED};
  for (int i = 0; i < static_cast<int>(count_of(ledPins)); ++i) {
    gpio_set_function(ledPins[i], GPIO_FUNC_PWM);
    uint sliceNum = pwm_gpio_to_slice_num(ledPins[i]);
    pwm_set_wrap(sliceNum, 255);
    pwm_set_enabled(sliceNum, true);
    pwm_set_gpio_level(ledPins[i], 255);
  }
#endif
}

// 現在の LED バックエンドへ 1 つの RGB 色を適用する。
static void led_backend_set_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
#if LED_BACKEND == LED_BACKEND_WS2812
  const uint32_t nowMs = to_ms_since_boot(get_absolute_time());
  const uint32_t pixel = led_rgb_u32(red, green, blue);
  if ((nowMs - s_ws2812LastUpdateMs) < WS2812_UPDATE_INTERVAL_MS && pixel == s_ws2812LastPixel) {
    return;
  }
  if ((nowMs - s_ws2812LastUpdateMs) < WS2812_UPDATE_INTERVAL_MS) {
    return;
  }
  s_ws2812LastUpdateMs = nowMs;
  s_ws2812LastPixel = pixel;
  put_pixel(pixel);
#else
  pwm_set_gpio_level(USER_LED_BLUE, static_cast<uint16_t>(255 - blue));
  pwm_set_gpio_level(USER_LED_GREEN, static_cast<uint16_t>(255 - green));
  pwm_set_gpio_level(USER_LED_RED, static_cast<uint16_t>(255 - red));
#endif
}

#if HBF500_ENABLE
static volatile uint8_t s_hbf500CurrentY = 0;
static volatile bool s_hbf500Core1Ready = false;
static PIO s_hbf500Pio = nullptr;
static int s_hbf500Sm = -1;
static uint s_hbf500PioOffset = 0;
static inline void hbf500_set_hiZ_low(uint8_t val, uint32_t mask, int shiftBase);
static void hbf500_core1_entry();  // Core 1 ポーリングループ前方宣言

static int y_table[16] = {
  // 入力 nibble は bit0=DAT4, bit1=DAT5, bit2=DAT6, bit3=DAT7。
  // Y の bit 配列は DAT7..DAT4 の逆順になるため、この表で反転する。
  0x0, 0x8, 0x4, 0xC, 0x2, 0xA, 0x6, 0xE,
  0x1, 0x9, 0x5, 0xD, 0x3, 0xB, 0x7, 0xF,
};

// PIO initialization for HB-F500 DAT bus control
static void hbf500_pio_init()
{
  PIO pio = pio0;
  const int sm = PIO_SM_HBF500;

  if (!pio_can_add_program(pio, &hbf500_dat_bus_ctrl_program)) {
    panic("HB-F500 PIO init failed (program memory full)");
  }

  pio_sm_claim(pio, static_cast<uint>(sm));

  const uint offset = pio_add_program(pio, &hbf500_dat_bus_ctrl_program);
  pio_sm_config c = hbf500_dat_bus_ctrl_program_get_default_config(offset);

  // set pindirs, 0 の対象はDAT4..DAT7
  sm_config_set_out_pins(&c, HBF500_DAT0_GPIO, 8);
  //PIO内のset pindirs, 0は、CS=L時にDAT4..DAT7をHi-Zにするために使う。
  sm_config_set_set_pins(&c, HBF500_DAT0_GPIO + 4, 4);
  //CSピンのJMP設定
  sm_config_set_jmp_pin(&c, HBF500_CS_GPIO);
  //ARMから渡す値のbit配置:bit0=DAT0... bit7=DAT7,LSB設定
  sm_config_set_out_shift(&c, true, false, 32);
  sm_config_set_clkdiv(&c, 1.0f);

  // PIO pin initialization (DAT0-7 のみ。CSはGPIO割り込みのためSIO機能を維持)
  for (int pin = 0; pin < 8; ++pin) {
    pio_gpio_init(pio, static_cast<uint>(HBF500_DAT0_GPIO + pin));
  }
  // HBF500_CS_GPIO は jmp_pin としてのみ使用するため pio_gpio_init() を呼ばない。
  // pio_gpio_init() は GPIO_FUNC_PIO0 に切り替えるため、
  // 呼ぶとIO_BANK0の割り込みが届かなくなる。

  // Initial pin state: DAT0-7 all Hi-Z
  pio_sm_set_pins_with_mask(pio, sm, 0u, HBF500_DAT_FULL_MASK);
  pio_sm_set_pindirs_with_mask(pio, sm, 0u, HBF500_DAT_FULL_MASK);

  pio_sm_init(pio, sm, offset, &c);

  // Y register not needed (no edge detection in new design)
  pio_sm_set_enabled(pio, sm, true);

  s_hbf500Pio = pio;
  s_hbf500Sm = sm;
  s_hbf500PioOffset = offset;
}

// HB-F500 の GPIO 方向、プル設定、CS エッジ処理を初期化する。
static void hbf500_init()
{
  // DAT0-7 初期化
  for (int i = 0; i < 8; i++) {
    const uint pin = static_cast<uint>(HBF500_DAT0_GPIO + i);
    gpio_init(pin);
    gpio_disable_pulls(pin);
    gpio_put(pin, 0);
  }

  // DAT0-3 は常時 OUTPUT
  gpio_set_dir_masked(HBF500_DAT03_MASK, HBF500_DAT03_MASK);

  // DAT4-7 は初期 INPUT
  gpio_set_dir_in_masked(HBF500_DAT47_MASK);

  // CS (GPIO9): 入力、プルアップ
  gpio_init(HBF500_CS_GPIO);
  gpio_set_dir(HBF500_CS_GPIO, GPIO_IN);
  gpio_pull_up(HBF500_CS_GPIO);
  // CAPS LED 信号 (GPIO10): 入力、プルアップ
  gpio_init(HBF500_CAPS_GPIO);
  gpio_set_dir(HBF500_CAPS_GPIO, GPIO_IN);
  gpio_pull_up(HBF500_CAPS_GPIO);
  // KANA LED 信号 (GPIO26): 入力、プルアップ
  gpio_init(HBF500_KANA_GPIO);
  gpio_set_dir(HBF500_KANA_GPIO, GPIO_IN);
  gpio_pull_up(HBF500_KANA_GPIO);

  // s_keyMatrix 初期化 (0xFF = 全キー離し)
  ::memset(const_cast<uint8_t*>(s_keyMatrix), 0xFF, sizeof(s_keyMatrix));

  // 初期方向: DAT4-7 入力、DAT0-3 出力
  gpio_set_dir_masked(HBF500_DAT47_MASK, 0u);
  hbf500_set_hiZ_low(s_keyMatrix[0], HBF500_DAT03_MASK, HBF500_DAT0_GPIO);

  // PIO initialization for DAT bus control
  hbf500_pio_init();

  // Core 1 でポーリングループを起動して CS エッジ処理を行う
  multicore_launch_core1(hbf500_core1_entry);
  const absolute_time_t readyDeadline = make_timeout_time_ms(1000);
  while (!s_hbf500Core1Ready) {
    if (time_reached(readyDeadline)) {
      panic("HB-F500 core1 startup timeout");
    }
    tight_loop_contents();
  }
}

// CS立ち下がり割り込みハンドラー (Core1のIO_IRQ_BANK0 raw handler)。
// CS=LOW検出時にDAT4-7を読み、CS=HIGH になるまでPIOへ値を送り続ける。
// PIOはCS=LOW中もループしてFIFOを消費し続けるため、
// 継続的に送信しないとY変化に追従できない。
static void hbf500_cs_irq_handler()
{
  // CSピンの立ち下がりイベントか確認してからクリア
  if (!(gpio_get_irq_event_mask(HBF500_CS_GPIO) & GPIO_IRQ_EDGE_FALL)) return;
  gpio_acknowledge_irq(HBF500_CS_GPIO, GPIO_IRQ_EDGE_FALL);

  constexpr uint32_t kDatShift = (HBF500_DAT0_GPIO + 4);

  // CS=LOW の間、DAT4-7 を読みながら PIO へ値を送り続ける
//  while (!(gpio_get_all() & (1u << HBF500_CS_GPIO))) {
    const uint32_t gpioAll = gpio_get_all();
    const uint8_t y = static_cast<uint8_t>(y_table[(gpioAll >> kDatShift) & 0x0Fu]);
    s_hbf500CurrentY = y;
    const uint8_t val = (y < static_cast<uint8_t>(count_of(s_keyMatrix))) ? s_keyMatrix[y] : 0xFFu;
    if (!pio_sm_is_tx_fifo_full(s_hbf500Pio, static_cast<uint>(s_hbf500Sm))) {
      pio_sm_put(s_hbf500Pio, static_cast<uint>(s_hbf500Sm), static_cast<uint32_t>(val));
      if (s_hbf500SelfTestActive && y < static_cast<uint8_t>(count_of(s_hbf500SelfTestYCount)) &&
          s_hbf500SelfTestYCount[y] != UINT16_MAX) {
        ++s_hbf500SelfTestYCount[y];
      }
    }
//  }
}

// Core 1 エントリ: Core1のNVICにCS割り込みを登録後、待機。
// gpio_set_irq_enabled_with_callback() はCore0専用のため、
// Core1ではirq_add_shared_handler() + irq_set_enabled() を使う。
// RP2350はSecure/Non-Secureの2系統があり、通常動作はNS側を使う。
static void hbf500_core1_entry()
{
  if (!flash_safe_execute_core_init()) {
    panic("flash_safe_execute_core_init failed");
  }
#if defined(IO_IRQ_BANK0_NS)
  // RP2350: 通常のアプリはNon-Secureモードで動作するためNS側IRQを使う
  static constexpr uint kGpioIrq = IO_IRQ_BANK0_NS;
#else
  static constexpr uint kGpioIrq = IO_IRQ_BANK0;
#endif
  irq_add_shared_handler(kGpioIrq, hbf500_cs_irq_handler, PICO_SHARED_IRQ_HANDLER_DEFAULT_ORDER_PRIORITY);
  gpio_set_irq_enabled(HBF500_CS_GPIO, GPIO_IRQ_EDGE_FALL, true);
  irq_set_priority(kGpioIrq, 0);
  irq_set_enabled(kGpioIrq, true);
  s_hbf500Core1Ready = true;

  for (;;) {
    tight_loop_contents();
  }
}


// DAT ピン群の出力方向を KeyMatrix 値に従い更新する
// val の各ビット: 0 → OUTPUT LOW (キー押下)、1 → INPUT HiZ (キー離し)
// mask: 対象 GPIO マスク (HBF500_DAT_FULL_MASK 等)
// マトリクス値に応じて HiZ/LOW の出力方向を設定する。
static inline void hbf500_set_hiZ_low(uint8_t val, uint32_t mask, int shiftBase)
{
  // direction: val bit=0 → 1(OUTPUT)、val bit=1 → 0(INPUT)
  const uint32_t valShifted = static_cast<uint32_t>(val) << shiftBase;
  gpio_set_dir_masked(mask, (~valShifted) & mask);
}

// 入力側をサンプリングしながら HB-F500 のマトリクス出力を維持する。
static void hbf500_matrix_task()
{
  const uint32_t gpioAll = gpio_get_all();

  // DAT0-7 の出力と CS エッジ処理は Core 1 (hbf500_core1_entry) が担当するため、
  // ここでは CAPS/KANA LED 状態の更新のみ行う。

  // CAPS (GPIO10) LOW → CAPSLOCK(bit1)、KANA (GPIO26) LOW → NUMLOCK(bit0)
  const bool caps = !static_cast<bool>((gpioAll >> HBF500_CAPS_GPIO) & 1u);
  const bool kana = !static_cast<bool>((gpioAll >> HBF500_KANA_GPIO) & 1u);
  uint8_t leds = s_keybordleds;
  leds = caps ? static_cast<uint8_t>(leds | 0x02u) : static_cast<uint8_t>(leds & ~0x02u);
  leds = kana ? static_cast<uint8_t>(leds | 0x01u) : static_cast<uint8_t>(leds & ~0x01u);
  s_keybordleds = leds;
}
#endif  // HBF500_ENABLE

// 元ソースの CH446 書き込み相当。ここでは動作確認しやすいようにログで可視化する。
// 1 つのマトリクスマッピング項目を現在の出力状態へ反映する。
static void matrix_action_emit(const char* tableName, const char* slotName, uint8_t usage, const MatrixTable& matrix, bool pressed)
{
  if (matrix.y[0] != 0) {
    ::printf("[MATRIX] %-8s %-2s usage=0x%02x y=%u mask=%u %s\n",
      tableName, slotName, usage, matrix.y[0], matrix.mask[0], pressed ? "DOWN" : "UP");
    update_msx_selected_keys(matrix.y[0], matrix.mask[0], pressed);
    // KeyMatrix 更新 (bit=0=押下, bit=1=離し, HiZ-Low 出力用)
    const uint8_t yIdx0 = static_cast<uint8_t>(matrix.y[0] - 1u);
    if (yIdx0 < static_cast<uint8_t>(count_of(s_keyMatrix))) {
      const uint8_t bit0 = static_cast<uint8_t>(1u << matrix.mask[0]);
      if (pressed) s_keyMatrix[yIdx0] &= static_cast<uint8_t>(~bit0);
      else         s_keyMatrix[yIdx0] |= bit0;
    }
  }
  // スロット B (y[1]/mask[1]) の KeyMatrix 更新
  if (matrix.y[1] != 0) {
    const uint8_t yIdx1 = static_cast<uint8_t>(matrix.y[1] - 1u);
    if (yIdx1 < static_cast<uint8_t>(count_of(s_keyMatrix))) {
      const uint8_t bit1 = static_cast<uint8_t>(1u << matrix.mask[1]);
      if (pressed) s_keyMatrix[yIdx1] &= static_cast<uint8_t>(~bit1);
      else         s_keyMatrix[yIdx1] |= bit1;
    }
  }
}

// asciiTable を使って ASCII コードをマトリクスキー A スロットへ出力する
// ASCII コード 1 文字をマトリクスの A スロットへ出力する。
static void ascii2MatrixKeyA(uint8_t asciiCode, bool pressed)
{
  if (asciiCode >= 128) return;
  const MatrixTable& m = asciiTable[s_configSettingNumber][asciiCode];
  if (m.y[0] == 0) return;
  matrix_action_emit("ASCII", "A", asciiCode, m, pressed);
}

// asciiTable を使って ASCII コードをマトリクスキー B スロットへ出力する
// ASCII コード 1 文字をマトリクスの B スロットへ出力する。
static void ascii2MatrixKeyB(uint8_t asciiCode, bool pressed)
{
  if (asciiCode >= 128) return;
  const MatrixTable& m = asciiTable[s_configSettingNumber][asciiCode];
  if (m.y[1] == 0) return;
  MatrixTable matrixSlot {{m.y[1], 0}, {m.mask[1], 0}};
  matrix_action_emit("ASCII", "B", asciiCode, matrixSlot, pressed);
}

// マトリクス送出用の ASCII 文字列をキューに積む。
static void queue_keyboard_string(const char* text)
{
  if (!text) return;
  size_t len = ::strlen(text);
  s_keyboardStr = std::make_unique<char[]>(len + 1);
  ::memcpy(s_keyboardStr.get(), text, len + 1);
  s_keyboardStrMode = true;
  s_keyboardStrWaitRelease = false;
  s_keyboardStrClearPending = true;
}

// s_keyboardStr の文字を STR_SEC ms 間隔でマトリクス出力するタスク。
// 1文字につき 4 フェーズ:
//   phase%4==0: A スロット DOWN
//   phase%4==1: B スロット DOWN
//   phase%4==2: B スロット UP
//   phase%4==3: A スロット UP
// Drives the queued string through the key matrix at a fixed interval.
static void keyboardStrTask()
{
  static uint32_t nextMs  = 0;
  static uint32_t strPhase = 0; // 現在処理中のフェーズ (文字インデックス = strPhase/4)

  if (!s_keyboardStrMode || !s_keyboardStr) return;

  // 文字送出開始時に現在の押下状態を無効化する
  if (s_keyboardStrClearPending) {
    clear_msx_selected_keys();
    s_keyboardStrClearPending = false;
    s_prevKeyboardReport = {0, {0, 0, 0, 0, 0, 0}};
    reset_modifier_transition_state();
  }

  uint32_t nowMs = to_ms_since_boot(get_absolute_time());

  if (nextMs == 0) {
    // 最初のフェーズを即時実行
    uint8_t ch = static_cast<uint8_t>(s_keyboardStr.get()[strPhase / 4]);
    if (ch != 0) {
      ascii2MatrixKeyA(ch, true);
      nextMs = nowMs + STR_SEC;
      strPhase++;
    } else {
      // 文字列終端: 送出完了
      nextMs    = 0;
      strPhase  = 0;
      s_keyboardStrMode = false;
      s_keyboardStr.reset();
      clear_msx_selected_keys();
      s_keyboardStrWaitRelease = s_keyboardAnyKeyPressed;
    }
    return;
  }

  if (nowMs <= nextMs) return;

  uint8_t ch = static_cast<uint8_t>(s_keyboardStr.get()[strPhase / 4]);
  if (ch != 0) {
    uint32_t phase = strPhase % 4;
    if (phase == 0) ascii2MatrixKeyA(ch, true);
    if (phase == 1) ascii2MatrixKeyB(ch, true);
    if (phase == 2) ascii2MatrixKeyB(ch, false);
    if (phase == 3) ascii2MatrixKeyA(ch, false);
    nextMs = nowMs + STR_SEC;
    strPhase++;
  } else {
    // 文字列終端: 送出完了
    nextMs    = 0;
    strPhase  = 0;
    s_keyboardStrMode = false;
    s_keyboardStr.reset();
    clear_msx_selected_keys();
    s_keyboardStrWaitRelease = s_keyboardAnyKeyPressed;
  }
}

// Usage テーブルの B スロットが有効なら true。
static bool vaildUsageTableB(uint8_t usbKeycode) { return usageTable[s_configSettingNumber][usbKeycode].y[1] != 0; }
// Shift テーブルの B スロットが有効なら true。
static bool vaildShiftTableB(uint8_t usbKeycode) { return shiftTable[s_configSettingNumber][usbKeycode].y[1] != 0; }

// Shift の 1st スロット(A)が実質 Shift 修飾と同一かどうか判定
// Shift の A スロットが修飾キーと一致するなら true。
static bool vaildisShiftTableA(uint8_t usbKeycode)
{
  return shiftTable[s_configSettingNumber][usbKeycode].y[0] == modifierTable[s_configSettingNumber][1].y[0] &&
         shiftTable[s_configSettingNumber][usbKeycode].mask[0] == modifierTable[s_configSettingNumber][1].mask[0];
}

static void start_hbf500_selftest()
{
  s_hbf500SelfTestActive = false;
  ::memset(const_cast<uint16_t*>(s_hbf500SelfTestYCount), 0, sizeof(s_hbf500SelfTestYCount));
  s_hbf500SelfTestPassed = false;
  s_hbf500SelfTestPassPending = false;
  s_hbf500SelfTestActive = true;
  ::printf("[SELFTEST] selftest.do found: waiting for Y0-Y10\n");
}

static void stop_hbf500_selftest()
{
  s_hbf500SelfTestActive = false;
  s_hbf500SelfTestPassed = false;
  s_hbf500SelfTestPassPending = false;
}

static void hbf500_selftest_task()
{
  if (s_hbf500SelfTestActive) {
    for (int y = 0; y < static_cast<int>(count_of(s_hbf500SelfTestYCount)); ++y) {
      if (s_hbf500SelfTestYCount[y] < 10) return;
    }
    s_hbf500SelfTestActive = false;
    s_hbf500SelfTestPassed = true;
    s_hbf500SelfTestPassPending = true;
    ::printf("[SELFTEST] Y0-Y10 pass\n");
  }

  if (s_hbf500SelfTestPassPending && !s_keyboardStrMode && !s_keyboardStrWaitRelease) {
    s_hbf500SelfTestPassPending = false;
    queue_keyboard_string("0123456789 SeLf TeSt PaSs[OK]");
  }
}

// 現在の USB キーコードの ASCII ダンプ処理を開始する。
static void start_keyid_str_task(uint8_t usbKeycode)
{
  if (!s_keyboardIDMode || s_keyboardStrMode) return;
  char tmp[16];
  ::snprintf(tmp, sizeof(tmp), "KEYID: 0x%02x", usbKeycode);
  queue_keyboard_string(tmp);
}

// Usage マッピングを A スロットへ出力する。
static void usage2MatrixKeyA(uint8_t usbKeycode, bool pressed)
{
  if (pressed) start_keyid_str_task(usbKeycode);
  const MatrixTable& m = usageTable[s_configSettingNumber][usbKeycode];
  if (m.y[0] == 0) return;
  matrix_action_emit("Usage", "A", usbKeycode, m, pressed);
}

// Usage マッピングを B スロットへ出力する。
static void usage2MatrixKeyB(uint8_t usbKeycode, bool pressed)
{
  const MatrixTable& m = usageTable[s_configSettingNumber][usbKeycode];
  if (m.y[1] == 0) return;
  MatrixTable matrixSlot {{m.y[1], 0}, {m.mask[1], 0}};
  matrix_action_emit("Usage", "B", usbKeycode, matrixSlot, pressed);
}

// Shift マッピングを A スロットへ出力する。
static void shift2MatrixKeyA(uint8_t usbKeycode, bool pressed)
{
  if (pressed) start_keyid_str_task(usbKeycode);
  const MatrixTable& m = shiftTable[s_configSettingNumber][usbKeycode];
  if (m.y[0] == 0) return;
  matrix_action_emit("Shift", "A", usbKeycode, m, pressed);
}

// Shift マッピングを B スロットへ出力する。
static void shift2MatrixKeyB(uint8_t usbKeycode, bool pressed)
{
  const MatrixTable& m = shiftTable[s_configSettingNumber][usbKeycode];
  if (m.y[1] == 0) return;
  MatrixTable matrixSlot {{m.y[1], 0}, {m.mask[1], 0}};
  matrix_action_emit("Shift", "B", usbKeycode, matrixSlot, pressed);
}

// 修飾キーの遷移追跡状態をクリアする。
static void reset_modifier_transition_state()
{
  s_modOldValidB = 0;
  s_modOldShiftA = 0;
  s_modOldA = 0;
  s_modOldShiftB = 0;
  s_modOldB = 0;
}

// 2 つのプロファイル番号が同一なら true。
static bool sameTable(uint8_t numA, uint8_t numB)
{
  return modifierTable[s_configSettingNumber][numA].y[0] == modifierTable[s_configSettingNumber][numB].y[0] &&
         modifierTable[s_configSettingNumber][numA].y[1] == modifierTable[s_configSettingNumber][numB].y[1] &&
         modifierTable[s_configSettingNumber][numA].mask[0] == modifierTable[s_configSettingNumber][numB].mask[0] &&
         modifierTable[s_configSettingNumber][numA].mask[1] == modifierTable[s_configSettingNumber][numB].mask[1];
}

// 修飾キー 1 スロット分の遷移を出力する。
static void emit_modifier_slot(const char* phase, uint8_t idx, bool slotB, bool pressed)
{
  const MatrixTable& m = modifierTable[s_configSettingNumber][idx];
  uint8_t y = slotB ? m.y[1] : m.y[0];
  uint8_t mask = slotB ? m.mask[1] : m.mask[0];
  if (y == 0) return;
  MatrixTable matrixSlot {{y, 0}, {mask, 0}};
  matrix_action_emit(phase, slotB ? "B" : "A", idx, matrixSlot, pressed);
}

// 修飾キーの B スロットが設定済みなら true。
static bool vaildmodifierTableB(uint8_t modifierKeycode)
{
  uint8_t modKeycode = modifierKeycode ^ s_modOldValidB;
  s_modOldValidB = modifierKeycode;

  if (sameTable(1, 5)) {
    if ((modKeycode & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT)) && modifierTable[s_configSettingNumber][1].y[1] != 0) return true;
  } else {
    if ((modKeycode & KEYBOARD_MODIFIER_LEFTSHIFT) && modifierTable[s_configSettingNumber][1].y[1] != 0) return true;
    if ((modKeycode & KEYBOARD_MODIFIER_RIGHTSHIFT) && modifierTable[s_configSettingNumber][5].y[1] != 0) return true;
  }
  if (sameTable(0, 4)) {
    if ((modKeycode & (KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_RIGHTCTRL)) && modifierTable[s_configSettingNumber][0].y[1] != 0) return true;
  } else {
    if ((modKeycode & KEYBOARD_MODIFIER_LEFTCTRL) && modifierTable[s_configSettingNumber][0].y[1] != 0) return true;
    if ((modKeycode & KEYBOARD_MODIFIER_RIGHTCTRL) && modifierTable[s_configSettingNumber][4].y[1] != 0) return true;
  }
  if (sameTable(2, 6)) {
    if ((modKeycode & (KEYBOARD_MODIFIER_LEFTALT | KEYBOARD_MODIFIER_RIGHTALT)) && modifierTable[s_configSettingNumber][2].y[1] != 0) return true;
  } else {
    if ((modKeycode & KEYBOARD_MODIFIER_LEFTALT) && modifierTable[s_configSettingNumber][2].y[1] != 0) return true;
    if ((modKeycode & KEYBOARD_MODIFIER_RIGHTALT) && modifierTable[s_configSettingNumber][6].y[1] != 0) return true;
  }
  if ((modKeycode & KEYBOARD_MODIFIER_LEFTGUI) && modifierTable[s_configSettingNumber][3].y[1] != 0) return true;
  if ((modKeycode & KEYBOARD_MODIFIER_RIGHTGUI) && modifierTable[s_configSettingNumber][7].y[1] != 0) return true;
  return false;
}

// A 側の Shift 遷移状態を更新する。
static void setShiftKeyA(uint8_t modKeycode, bool is_push)
{
  if (sameTable(1, 5)) {
    if (modKeycode & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT)) emit_modifier_slot("ModShift", 1, false, is_push);
  } else {
    if (modKeycode & KEYBOARD_MODIFIER_LEFTSHIFT) emit_modifier_slot("ModShift", 1, false, is_push);
    if (modKeycode & KEYBOARD_MODIFIER_RIGHTSHIFT) emit_modifier_slot("ModShift", 5, false, is_push);
  }
}

// B 側の Shift 遷移状態を更新する。
static void setShiftKeyB(uint8_t modKeycode, bool is_push)
{
  if (sameTable(1, 5)) {
    if (modKeycode & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT)) emit_modifier_slot("ModShift", 1, true, is_push);
  } else {
    if (modKeycode & KEYBOARD_MODIFIER_LEFTSHIFT) emit_modifier_slot("ModShift", 1, true, is_push);
    if (modKeycode & KEYBOARD_MODIFIER_RIGHTSHIFT) emit_modifier_slot("ModShift", 5, true, is_push);
  }
}

// 修飾キーから Shift へのマッピングを A スロットへ出力する。
static void modifierShift2MatrixKeyA(uint8_t modifierKeycode)
{
  uint8_t modKeycode = modifierKeycode ^ s_modOldShiftA;
  bool is_push = (modKeycode & modifierKeycode) != 0;
  s_modOldShiftA = modifierKeycode;
  setShiftKeyA(modKeycode, is_push);
}

// 修飾キーのマッピングを A スロットへ出力する。
static void modifier2MatrixKeyA(uint8_t modifierKeycode)
{
  uint8_t modKeycode = modifierKeycode ^ s_modOldA;
  bool is_push = (modKeycode & modifierKeycode) != 0;
  s_modOldA = modifierKeycode;

  if (sameTable(0, 4)) {
    if (modKeycode & (KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_RIGHTCTRL)) emit_modifier_slot("Modifier", 0, false, is_push);
  } else {
    if (modKeycode & KEYBOARD_MODIFIER_LEFTCTRL) emit_modifier_slot("Modifier", 0, false, is_push);
    if (modKeycode & KEYBOARD_MODIFIER_RIGHTCTRL) emit_modifier_slot("Modifier", 4, false, is_push);
  }
  if (sameTable(2, 6)) {
    if (modKeycode & (KEYBOARD_MODIFIER_LEFTALT | KEYBOARD_MODIFIER_RIGHTALT)) emit_modifier_slot("Modifier", 2, false, is_push);
  } else {
    if (modKeycode & KEYBOARD_MODIFIER_LEFTALT) emit_modifier_slot("Modifier", 2, false, is_push);
    if (modKeycode & KEYBOARD_MODIFIER_RIGHTALT) emit_modifier_slot("Modifier", 6, false, is_push);
  }
  if (modKeycode & KEYBOARD_MODIFIER_LEFTGUI) emit_modifier_slot("Modifier", 3, false, is_push);
  if (modKeycode & KEYBOARD_MODIFIER_RIGHTGUI) emit_modifier_slot("Modifier", 7, false, is_push);
}

// 修飾キーから Shift へのマッピングを B スロットへ出力する。
static void modifierShift2MatrixKeyB(uint8_t modifierKeycode)
{
  uint8_t modKeycode = modifierKeycode ^ s_modOldShiftB;
  bool is_push = (modKeycode & modifierKeycode) != 0;
  s_modOldShiftB = modifierKeycode;

  if (sameTable(1, 5)) {
    if (modKeycode & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT)) emit_modifier_slot("ModShift", 1, true, is_push);
  } else {
    if (modKeycode & KEYBOARD_MODIFIER_LEFTSHIFT) emit_modifier_slot("ModShift", 1, true, is_push);
    if (modKeycode & KEYBOARD_MODIFIER_RIGHTSHIFT) emit_modifier_slot("ModShift", 5, true, is_push);
  }
}

// 修飾キーのマッピングを B スロットへ出力する。
static void modifier2MatrixKeyB(uint8_t modifierKeycode)
{
  uint8_t modKeycode = modifierKeycode ^ s_modOldB;
  bool is_push = (modKeycode & modifierKeycode) != 0;
  s_modOldB = modifierKeycode;

  if (sameTable(0, 4)) {
    if (modKeycode & (KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_RIGHTCTRL)) emit_modifier_slot("Modifier", 0, true, is_push);
  } else {
    if (modKeycode & KEYBOARD_MODIFIER_LEFTCTRL) emit_modifier_slot("Modifier", 0, true, is_push);
    if (modKeycode & KEYBOARD_MODIFIER_RIGHTCTRL) emit_modifier_slot("Modifier", 4, true, is_push);
  }
  if (sameTable(1, 5)) {
    if (modKeycode & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT)) emit_modifier_slot("Modifier", 1, true, is_push);
  } else {
    if (modKeycode & KEYBOARD_MODIFIER_LEFTSHIFT) emit_modifier_slot("Modifier", 1, true, is_push);
    if (modKeycode & KEYBOARD_MODIFIER_RIGHTSHIFT) emit_modifier_slot("Modifier", 5, true, is_push);
  }
  if (sameTable(2, 6)) {
    if (modKeycode & (KEYBOARD_MODIFIER_LEFTALT | KEYBOARD_MODIFIER_RIGHTALT)) emit_modifier_slot("Modifier", 2, true, is_push);
  } else {
    if (modKeycode & KEYBOARD_MODIFIER_LEFTALT) emit_modifier_slot("Modifier", 2, true, is_push);
    if (modKeycode & KEYBOARD_MODIFIER_RIGHTALT) emit_modifier_slot("Modifier", 6, true, is_push);
  }
  if (modKeycode & KEYBOARD_MODIFIER_LEFTGUI) emit_modifier_slot("Modifier", 3, true, is_push);
  if (modKeycode & KEYBOARD_MODIFIER_RIGHTGUI) emit_modifier_slot("Modifier", 7, true, is_push);
}

// 指定キーがレポート内に存在するかを判定
// キーボードレポート内に指定キーコードが含まれていれば true。
static inline bool find_key_in_report(const KeyboardReport* report, uint8_t keycode)
{
  for (uint8_t i = 0; i < 6; i++) {
    if (report->keycode[i] == keycode) return true;
  }
  return false;
}

// LSHIFT+LCTRL+LALT+Q/W/E/R でプロファイル選択モードへ移行するチェック。
// SETTING_SEC ms 保持で確定。CFG_ACTIVE 中にキーが来たら通常モードへ戻る。
// 現在のキーボードレポートから設定モード状態機械を更新する。
static void configModeCheck(const KeyboardReport* report)
{
  static uint32_t end_ms = 0;

  // anyKey: modifier または keycode が押されているかどうか
  bool anyKey = (report->modifier != 0x00);
  for (uint8_t i = 0; i < 6; i++) {
    if (report->keycode[i] != 0x00) { anyKey = true; break; }
  }

  // CFG_ACTIVE: 設定確定済み → キー入力があれば通常モードへ
  if (s_configMode == CFG_ACTIVE) {
    if (anyKey) return;
    ::printf("[CFG] profile mode exit\n");
    s_configMode = CFG_NONE;
    s_blink_interval_ms = BLINK_MOUNTED;
    return;
  }

  // CFG_PRE: 選択キー押下中 → SETTING_SEC 経過で確定
  if (s_configMode == CFG_PRE) {
    if (!anyKey) {
      // キーが外れたらキャンセル
      ::printf("[CFG] profile mode cancel\n");
      s_configMode = CFG_NONE;
      s_blink_interval_ms = BLINK_MOUNTED;
      end_ms = 0;
      return;
    }
    // タイマー開始
    if (end_ms == 0) {
      end_ms = to_ms_since_boot(get_absolute_time()) + SETTING_SEC;
    }
    // SETTING_SEC 経過で確定
    if (to_ms_since_boot(get_absolute_time()) > end_ms) {
      s_configMode = CFG_ACTIVE;
      end_ms = 0;
      s_blink_interval_ms = BLINK_NOT_MOUNTED;
      ::printf("[CFG] profile %d confirmed\n", s_numProfile);
      if (s_numProfile < 10) {
        if (s_configSettingNumber != (int)s_numProfile) {
          s_configSettingNumber = (int)s_numProfile;
          ::printf("[CFG] switching to KEYMAP%d.JSN\n", s_configSettingNumber);
          {
            char keymapFile[32];
            ::snprintf(keymapFile, sizeof(keymapFile), "KEYMAP%d.JSN", s_configSettingNumber);
            updateConfigNumberCSV("A:/VIDCFG.INI", s_vid, s_pid, keymapFile);
          }
          // プロファイル切替をマトリクス経由で文字表示する
          {
            char tmp[64];
            ::snprintf(tmp, sizeof(tmp), " KEYMAP%d.JSN Select.", s_configSettingNumber);
            queue_keyboard_string(tmp);
          }
        }
      }
      // numProfile=99: FW バージョン表示
      if (s_numProfile == 99) {
        ::printf("[CFG] FW: %s\n", SW_VERSION);
        {
          char tmp[64];
          ::snprintf(tmp, sizeof(tmp), "FW: %s", SW_VERSION);
          queue_keyboard_string(tmp);
        }
      }
      // numProfile=98: キー ID ダンプ ON/OFF トグル
      if (s_numProfile == 98) {
        s_keyboardIDMode = !s_keyboardIDMode;
        ::printf("[CFG] KeyID dump %s\n", s_keyboardIDMode ? "ON" : "OFF");
      }
    }
    return;
  }

  // CFG_NONE: LSHIFT+LCTRL+LALT + Q/W/E/R/V/I でモード遷移
  if ((report->modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_LEFTALT))
      == (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_LEFTALT)) {
    bool profileKey = find_key_in_report(report, HID_KEY_Q)
                   || find_key_in_report(report, HID_KEY_W)
                   || find_key_in_report(report, HID_KEY_E)
                   || find_key_in_report(report, HID_KEY_R)
                   || find_key_in_report(report, HID_KEY_V)
                   || find_key_in_report(report, HID_KEY_I);
    if (profileKey) {
      s_configMode = CFG_PRE;
      end_ms = 0;
      ::printf("[CFG] profile mode start\n");
      if (find_key_in_report(report, HID_KEY_Q)) s_numProfile = 0;
      else if (find_key_in_report(report, HID_KEY_W)) s_numProfile = 1;
      else if (find_key_in_report(report, HID_KEY_E)) s_numProfile = 2;
      else if (find_key_in_report(report, HID_KEY_R)) s_numProfile = 3;
      else if (find_key_in_report(report, HID_KEY_V)) s_numProfile = 99; // VERSION 表示
      else if (find_key_in_report(report, HID_KEY_I)) s_numProfile = 98; // KeyID ダンプ ON/OFF
      ::printf("[CFG] target profile=%d\n", s_numProfile);
    }
  }
}

// pico-littlefs の process_kbd_report() をベースにした入力遷移処理
// - 新規押下: Aスロットを出力
// - 離上: A/B の解除を出力
// - 必要時: 1ms 後に Bスロットを追従出力
// Processes one keyboard report and emits matrix actions.
static void process_kbd_report(const KeyboardReport* report)
{
  bool usage2ndkeyEnable = false;
  bool modifire2ndKeyEnable = false;

  // configMode チェック（プロファイル切替 / 通常モード復帰）
  configModeCheck(report);

  // keyboardStrTask 送出中と送出後の離上待ち中は物理キー入力を無効化する
  if (s_keyboardStrMode || s_keyboardStrWaitRelease) {
    if (s_keyboardStrWaitRelease && !s_keyboardAnyKeyPressed) {
      s_keyboardStrWaitRelease = false;
      s_prevKeyboardReport = {0, {0, 0, 0, 0, 0, 0}};
      reset_modifier_transition_state();
      return;
    }
    s_prevKeyboardReport = *report;
    return;
  }

  // CFG_ACTIVE / CFG_PRE 中はキーマトリクス出力を行わない
  if (s_configMode != CFG_NONE) {
    s_prevKeyboardReport = *report;
    return;
  }

  // 新規押下キーを処理
  for (uint8_t i = 0; i < 6; i++) {
    uint8_t keycode = report->keycode[i];
    if (keycode == 0) continue;
    if (find_key_in_report(&s_prevKeyboardReport, keycode)) continue;

    if (report->modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT)) {
      if (vaildisShiftTableA(keycode)) {
        shift2MatrixKeyB(keycode, true);
      } else {
        setShiftKeyA(s_prevKeyboardReport.modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT), false);
        setShiftKeyB(s_prevKeyboardReport.modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT), false);
        shift2MatrixKeyA(keycode, true);
        if (!usage2ndkeyEnable) usage2ndkeyEnable = vaildShiftTableB(keycode);
      }
    } else {
      usage2MatrixKeyA(keycode, true);
      if (!usage2ndkeyEnable) usage2ndkeyEnable = vaildUsageTableB(keycode);
    }
  }

  // 離上キーを処理
  for (uint8_t i = 0; i < 6; i++) {
    uint8_t keycode = s_prevKeyboardReport.keycode[i];
    if (keycode == 0) continue;
    if (find_key_in_report(report, keycode)) continue;

    if (s_prevKeyboardReport.modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT)) {
      shift2MatrixKeyB(keycode, false);
      if (!vaildisShiftTableA(keycode)) {
        shift2MatrixKeyA(keycode, false);
        setShiftKeyA(s_prevKeyboardReport.modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT), true);
        setShiftKeyB(s_prevKeyboardReport.modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT), true);
      }
    } else {
      usage2MatrixKeyB(keycode, false);
      usage2MatrixKeyA(keycode, false);
    }
  }

  if (s_prevKeyboardReport.modifier != report->modifier) {
    modifier2MatrixKeyA(report->modifier);
    modifierShift2MatrixKeyA(report->modifier);
    modifire2ndKeyEnable = vaildmodifierTableB(report->modifier);
  }

  // 2nd スロットが必要なケースのみ遅延追従で出力
  if (usage2ndkeyEnable || modifire2ndKeyEnable) {
    sleep_ms(1);
    for (uint8_t i = 0; i < 6; i++) {
      uint8_t keycode = report->keycode[i];
      if (keycode == 0) continue;
      if (find_key_in_report(&s_prevKeyboardReport, keycode)) continue;
      if (report->modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT)) {
        shift2MatrixKeyB(keycode, true);
      } else {
        usage2MatrixKeyB(keycode, true);
      }
    }
    if (s_prevKeyboardReport.modifier != report->modifier) {
      modifier2MatrixKeyB(report->modifier);
      modifierShift2MatrixKeyB(report->modifier);
    }
  }

  s_prevKeyboardReport = *report;
}

// 現在の USB MSC フェーズを終了し、一時状態をクリアする。
static void finish_usb_msc_task() {
  ::printf("[USBMSC] finish_usb_msc_task()\n");

  if (s_usbMscTask.pDir) {
    delete s_usbMscTask.pDir;
    s_usbMscTask.pDir = nullptr;
  }
  if (s_usbMscTask.fatMounted) {
    FS::Unmount("B:");
  }
  s_usbMscTask.phase = UsbMscPhase::Idle;
  s_usbMscTask.fatMounted = false;
  s_usbMscTask.aMounted = false;
  s_usbMscTask.resetRequested = false;
  s_usbMscTask.selfTestRequested = false;
  s_usbMscTask.lfsDataUpdated = false;
  s_usbMscTask.fileIdx = 0;
}

// USB MSC 処理を完了し、フェーズ機械を進める。
static void complete_usb_msc_task()
{
    const bool needReload = s_usbMscTask.lfsDataUpdated;
    finish_usb_msc_task();
    if (needReload) {
        reload_lfs_runtime_data("[USBMSC]");
    }
    s_usbMscIgnoreUntilUnmount = s_usbFat.GetMSC().IsMounted();
}

// 2 つのファイルの内容が完全一致するとき true。
static bool are_files_equal(const char* pathName1, const char* pathName2)
{
    std::unique_ptr<FS::FileInfo> pFileInfo1(FS::GetFileInfo(pathName1));
    std::unique_ptr<FS::FileInfo> pFileInfo2(FS::GetFileInfo(pathName2));
    if (!pFileInfo1 || !pFileInfo2 || pFileInfo1->IsDirectory() || pFileInfo2->IsDirectory()) {
        return false;
    }
    if (pFileInfo1->GetSize() != pFileInfo2->GetSize()) {
        return false;
    }

    std::unique_ptr<FS::File> pFile1(FS::OpenFile(pathName1, "r"));
    std::unique_ptr<FS::File> pFile2(FS::OpenFile(pathName2, "r"));
    if (!pFile1 || !pFile2) {
        return false;
    }

    char buff1[64];
    char buff2[64];
    while (true) {
        int bytes1 = pFile1->Read(buff1, static_cast<int>(sizeof(buff1)));
        int bytes2 = pFile2->Read(buff2, static_cast<int>(sizeof(buff2)));
        if (bytes1 != bytes2) {
            return false;
        }
        if (bytes1 <= 0) {
            return true;
        }
        if (::memcmp(buff1, buff2, static_cast<size_t>(bytes1)) != 0) {
            return false;
        }
    }
}

// Core1 から毎回呼ばれる。
// マウント/アンマウントの遷移だけを検出し、フラグで Core0 に通知する。
// USB MSC のマウント/アンマウントイベントを処理する。
static void handle_usb_msc_events()
{
    bool mounted = s_usbFat.GetMSC().IsMounted();
    if (s_usbMscIgnoreUntilUnmount) {
        if (!mounted && s_usbMscPrevMounted) {
            s_usbMscUnmountEvent = true;
            s_usbMscIgnoreUntilUnmount = false;
        }
        s_usbMscPrevMounted = mounted;
        return;
    }
    if (mounted && !s_usbMscPrevMounted) {
        s_usbMscMountEvent = true;
    }
    if (!mounted && s_usbMscPrevMounted) {
        s_usbMscUnmountEvent = true;
    }
    s_usbMscPrevMounted = mounted;
}

// Core0 のメインループから呼ばれる。
// Core1 のイベントを契機に USB MSC のファイル操作と FAT アンマウントを処理する。
// ファイル操作は挿入 1 回につき 1 回だけ実行し、CDC ログの急増を避けるためループに分散する。
// USB MSC のファイル同期ワークフローを進める。
static void process_usb_msc_events()
{
    if (s_usbMscMountEvent) {
        s_usbMscMountEvent = false;
        finish_usb_msc_task();
        s_usbMscTask.phase = UsbMscPhase::BeginMount;
    }
    if (s_usbMscUnmountEvent) {
        s_usbMscUnmountEvent = false;
        finish_usb_msc_task();
        stop_hbf500_selftest();
        ::printf("[USBMSC] USB drive unmounted\n");
    }

    switch (s_usbMscTask.phase) {
    case UsbMscPhase::Idle:
        break;

    case UsbMscPhase::BeginMount:
        ::printf("[USBMSC] USB drive mounted\n");
        if (!FS::Mount("B:")) {
            ::printf("[USBMSC] FAT mount failed (not FAT?), no file operations\n");
            s_fileErrorActive = true;
            complete_usb_msc_task();
        } else {
            s_usbMscTask.fatMounted = true;
            s_usbMscTask.phase = UsbMscPhase::ListOpen;
        }
        break;

    case UsbMscPhase::ListOpen:
        ::printf("[USBMSC] File list on B:/\n");
        s_usbMscTask.pDir = FS::OpenDir("B:/");
        if (!s_usbMscTask.pDir) {
            ::printf("[USBMSC] failed to open B:/\n");
            s_fileErrorActive = true;
            s_usbMscTask.phase = UsbMscPhase::PrepareFileOps;
        } else {
            s_usbMscTask.phase = UsbMscPhase::ListNext;
        }
        break;

    case UsbMscPhase::ListNext:
        if (!s_usbMscTask.pDir) {
            s_usbMscTask.phase = UsbMscPhase::PrepareFileOps;
            break;
        }
        {
            std::unique_ptr<FS::FileInfo> pFileInfo(s_usbMscTask.pDir->Read());
            if (!pFileInfo) {
                delete s_usbMscTask.pDir;
                s_usbMscTask.pDir = nullptr;
                s_usbMscTask.phase = UsbMscPhase::PrepareFileOps;
            } else if (pFileInfo->IsDirectory()) {
                ::printf("  [DIR]  %s/\n", pFileInfo->GetName());
            } else {
                ::printf("  [FILE] %s\n", pFileInfo->GetName());
            }
        }
        break;

    case UsbMscPhase::PrepareFileOps:
        if (!FS::Mount("A:")) {
            ::printf("[USBMSC] A: not mounted, skip file operations\n");
            s_fileErrorActive = true;
            complete_usb_msc_task();
            break;
        }
        s_usbMscTask.aMounted = true;
        s_usbMscTask.selfTestRequested = FS::DoesExist("B:/selftest.do");
        if (s_usbMscTask.selfTestRequested) {
            start_hbf500_selftest();
        } else {
            stop_hbf500_selftest();
        }
        s_usbMscTask.resetRequested = FS::DoesExist("B:/resetdef.txt");
        s_usbMscTask.fileIdx = 0;
        if (s_usbMscTask.resetRequested) {
            ::printf("[USBMSC] resetdef.txt found: formatting A: and restoring defaults ...\n");
            FS::Unmount("A:");
            s_usbMscTask.aMounted = false;
            s_usbMscTask.phase = UsbMscPhase::ResetFormat;
        } else if (s_usbMscTask.selfTestRequested) {
            ::printf("[SELFTEST] skipping LFS-to-USB file sync\n");
            complete_usb_msc_task();
        } else {
            s_usbMscTask.phase = UsbMscPhase::CopyFiles;
        }
        break;

    case UsbMscPhase::ResetFormat:
        if (!FS::Format("A:")) {
            ::printf("[USBMSC] A: format failed\n");
            s_fileErrorActive = true;
            complete_usb_msc_task();
        } else {
            s_usbMscTask.phase = UsbMscPhase::ResetRemount;
        }
        break;

    case UsbMscPhase::ResetRemount:
        if (!FS::Mount("A:")) {
            ::printf("[USBMSC] A: remount after format failed\n");
            s_fileErrorActive = true;
            complete_usb_msc_task();
        } else {
            s_usbMscTask.aMounted = true;
            s_usbMscTask.fileIdx = 0;
            ::printf("[USBMSC] A: formatted OK\n");
            s_usbMscTask.phase = UsbMscPhase::ResetCreateFiles;
        }
        break;

    case UsbMscPhase::ResetCreateFiles:
        if (s_usbMscTask.fileIdx >= static_cast<int>(count_of(kFilesToSync))) {
            complete_usb_msc_task();
            break;
        }
        {
            const SyncFile& sf = kFilesToSync[s_usbMscTask.fileIdx++];
            char dstPath[64];
            ::snprintf(dstPath, sizeof(dstPath), "A:/%s", sf.name);
            std::unique_ptr<FS::File> pF(FS::OpenFile(dstPath, "w"));
            if (pF) {
                pF->Write(sf.defaultContent, static_cast<int>(::strlen(sf.defaultContent)));
                ::printf("[USBMSC] %s: reset to default\n", sf.name);
                s_usbMscTask.lfsDataUpdated = true;
            } else {
                ::printf("[USBMSC] %s: create failed\n", sf.name);
                s_fileErrorActive = true;
            }
        }
        break;

    case UsbMscPhase::CopyFiles:
        while (s_usbMscTask.fileIdx < static_cast<int>(count_of(kFilesToSync))) {
            const SyncFile& sf = kFilesToSync[s_usbMscTask.fileIdx++];
            char srcPath[64], dstPath[64];
            ::snprintf(srcPath, sizeof(srcPath), "A:/%s", sf.name);
            ::snprintf(dstPath, sizeof(dstPath), "B:/%s", sf.name);
            if (FS::DoesExist(srcPath) && FS::DoesExist(dstPath)) {
                if (!are_files_equal(srcPath, dstPath)) {
                    if (FS::CopyFile(dstPath, srcPath, true)) {
                        ::printf("[USBMSC] %s: copied B:->A: OK\n", sf.name);
                        s_usbMscTask.lfsDataUpdated = true;
                    } else {
                        ::printf("[USBMSC] %s: copy B:->A: failed\n", sf.name);
                        s_fileErrorActive = true;
                    }
                    break;
                }else{
                    ::printf("[USBMSC] %s: same file...not copy\n", sf.name);

                }
                continue;
            }
            if (!FS::DoesExist(srcPath) || FS::DoesExist(dstPath)) continue;
            if (FS::CopyFile(srcPath, dstPath, false)) {
                ::printf("[USBMSC] %s: copied A:->B: OK\n", sf.name);
            } else {
                ::printf("[USBMSC] %s: copy A:->B: failed\n", sf.name);
                s_fileErrorActive = true;
            }
            break;
        }
        if (s_usbMscTask.fileIdx >= static_cast<int>(count_of(kFilesToSync))) {
            complete_usb_msc_task();
        }
        break;
    }
    s_fileAccessActive = (s_usbMscTask.phase != UsbMscPhase::Idle);
}

// --------------------------------------------------------------------------
// VIDCFG.INI (CSV: vid,pid,profileFileName) 読み書き関数
// --------------------------------------------------------------------------

// テーブルが required エントリ以上を保持できるよう realloc で拡張する
// VID/PID テーブルが要求件数以上を保持できるよう拡張する。
static bool ensurePidTableCapacity(int required)
{
    if (required <= s_pidCapacity) return true;
    int newCap = (s_pidCapacity > 0) ? s_pidCapacity : 16;
    while (newCap < required) newCap *= 2;
    void* p = ::realloc(s_pidTable, static_cast<size_t>(newCap) * sizeof(keyconfigTable));
    if (!p) {
        ::printf("[CSV] ensurePidTableCapacity: realloc failed (required=%d)\n", required);
        return false;
    }
    s_pidTable   = static_cast<keyconfigTable*>(p);
    s_pidCapacity = newCap;
    return true;
}
// VIDCFG.INI をメモリテーブル(s_pidTable)に読み込む
// 成功: テーブルのエントリ数を返す / ファイル未存在時: -1
// Appends one VID/PID/profile entry to the in-memory CSV table.
static bool appendConfigFileEntry(uint16_t vidNum, uint16_t pidNum, const char* profileFileName)
{
    if (!profileFileName || profileFileName[0] == '\0') return false;
    if (!ensurePidTableCapacity(s_pidMaxNumber + 1)) return false;
    s_pidTable[s_pidMaxNumber].vid = vidNum;
    s_pidTable[s_pidMaxNumber].pid = pidNum;
    ::snprintf(s_pidTable[s_pidMaxNumber].profileFileName,
               sizeof(s_pidTable[s_pidMaxNumber].profileFileName),
               "%s", profileFileName);
    s_pidMaxNumber++;
    return true;
}

// VIDCFG.INI をメモリ上のテーブルへ読み込む。
static int readConfigNumberCSV(const char* filename)
{
    std::unique_ptr<char[]> fileReadBuffer(read_file_text(filename));
    if (!fileReadBuffer) {
        ::printf("[CSV] %s: not found (first run?)\n", filename);
        s_pidMaxNumber = 0;
        s_fileErrorActive = true;
        return -1;
    }

    s_pidMaxNumber = 0;
    char* line = ::strtok(fileReadBuffer.get(), "\n");
    while (line != NULL) {
        while (*line == ' ' || *line == '\t') line++;
        if (*line == '\0' || *line == '#' || *line == ';') {
            line = ::strtok(NULL, "\n");
            continue;
        }

        unsigned int csv_vid_u32 = 0;
        unsigned int csv_pid_u32 = 0;
        char csv_profile_file[32];
        csv_profile_file[0] = '\0';
        // 新形式のみ: vid,pid,profileFileName
        if (::sscanf(line, "%x,%x,%31[^,\r\n]", &csv_vid_u32, &csv_pid_u32, csv_profile_file) == 3 &&
            csv_vid_u32 <= 0xFFFFu && csv_pid_u32 <= 0xFFFFu) {
            char* profile = csv_profile_file;
            while (*profile == ' ' || *profile == '\t') profile++;
            if (!appendConfigFileEntry(static_cast<uint16_t>(csv_vid_u32),
                                       static_cast<uint16_t>(csv_pid_u32),
                                       profile)) return -1;
        } else {
            ::printf("[CSV] %s: invalid line: %s\n", filename, line);
            s_fileErrorActive = true;
            return -1;
        }
        line = ::strtok(NULL, "\n");
    }
    ::printf("[CSV] %s: loaded %d entries\n", filename, s_pidMaxNumber);
    return s_pidMaxNumber;
}

// メモリテーブルから VID/PID に対応するキーマップ番号を検索する
// 見つかれば番号を、見つからなければ -1 を返す
// Finds the keymap profile index for a keyboard VID/PID pair.
static int findConfigNumberCSV(uint16_t vidNum, uint16_t pidNum)
{
    if (!s_pidTable) return -1;
    for (int i = 0; i < s_pidMaxNumber; i++) {
        if (s_pidTable[i].vid != vidNum || s_pidTable[i].pid != pidNum) continue;
        int keymapIndex = -1;
        if (::sscanf(s_pidTable[i].profileFileName, "KEYMAP%d.JSN", &keymapIndex) == 1)
            return keymapIndex;
    }
    return -1;  // 未登録
}

// メモリテーブルから VID/PID に対応する GamePad マップ番号を検索する
// Finds the gamepad profile index for a gamepad VID/PID pair.
static int findGpadConfigNumberCSV(uint16_t vidNum, uint16_t pidNum)
{
    if (!s_pidTable) return -1;
    for (int i = 0; i < s_pidMaxNumber; i++) {
        if (s_pidTable[i].vid != vidNum || s_pidTable[i].pid != pidNum) continue;
        int gpadmapIndex = -1;
        if (::sscanf(s_pidTable[i].profileFileName, "GPADMAP%d.JSN", &gpadmapIndex) == 1)
            return gpadmapIndex;
    }
    return -1;  // 未登録
}

// メモリテーブルを更新して VIDCFG.INI ファイルに書き戻す
// VIDCFG.INI に 1 件の VID/PID/プロファイル対応を書き戻す。
static int updateConfigNumberCSV(const char* filename, uint16_t vidNum, uint16_t pidNum, const char* profileFileName)
{
    if (!profileFileName || profileFileName[0] == '\0') {
        ::printf("[CSV] invalid profile file name\n");
        s_fileErrorActive = true;
        return -1;
    }
    bool isKeymap = ::strncmp(profileFileName, "KEYMAP", 6) == 0;
    bool isGpadmap = ::strncmp(profileFileName, "GPADMAP", 7) == 0;
    if (!isKeymap && !isGpadmap) {
        ::printf("[CSV] unsupported profile file: %s\n", profileFileName);
        s_fileErrorActive = true;
        return -1;
    }

    bool found = false;
    for (int i = 0; i < s_pidMaxNumber; i++) {
        if (s_pidTable[i].vid != vidNum || s_pidTable[i].pid != pidNum) continue;
        const bool slotIsKeymap = ::strncmp(s_pidTable[i].profileFileName, "KEYMAP", 6) == 0;
        const bool slotIsGpadmap = ::strncmp(s_pidTable[i].profileFileName, "GPADMAP", 7) == 0;
        if ((isKeymap && slotIsKeymap) || (isGpadmap && slotIsGpadmap)) {
            ::snprintf(s_pidTable[i].profileFileName, sizeof(s_pidTable[i].profileFileName), "%s", profileFileName);
            found = true;
            break;
        }
    }
    if (!found) {
        if (!appendConfigFileEntry(vidNum, pidNum, profileFileName)) return -1;
    }

    // 全エントリを CSV 文字列に組み立てる (vid,pid,profileFileName)
    const int kBufSize = ((s_pidMaxNumber > 0 ? s_pidMaxNumber : 1) * 64) + 64;
    std::unique_ptr<char[]> buf(new char[kBufSize]);
    ::snprintf(buf.get(), static_cast<size_t>(kBufSize), "# VID(hex),PID(hex),ProfileFileName\n");
    for (int i = 0; i < s_pidMaxNumber; i++) {
        char line_buf[64];
        ::snprintf(line_buf, sizeof(line_buf), "0x%04X,0x%04X,%s\n",
            s_pidTable[i].vid, s_pidTable[i].pid,
            s_pidTable[i].profileFileName);
        const size_t remaining = static_cast<size_t>(kBufSize) - ::strlen(buf.get()) - 1;
        ::strncat(buf.get(), line_buf, remaining);
    }

    // ファイルに書き戻す（上書き）
    std::unique_ptr<FS::File> pF(FS::OpenFile(filename, "w"));
    if (!pF) {
        ::printf("[CSV] %s: write failed\n", filename);
        s_fileErrorActive = true;
        return -1;
    }
    pF->Write(buf.get(), static_cast<int>(::strlen(buf.get())));
    ::printf("[CSV] %s: saved %d entries (VID=%04x PID=%04x profile=%s)\n",
        filename, s_pidMaxNumber, vidNum, pidNum, profileFileName);
    return 0;
}

// --------------------------------------------------------------------------
// Board LED 点滅 + キーボード LED 送信タスク
// --------------------------------------------------------------------------
// メインループから毎回呼ぶ。
//   - s_keybordleds が変化したらキーボードに LED レポートを送信する
//   - s_blink_interval_ms 周期で Board LED を点滅させる
// ボード LED を更新し、キーボード LED 状態をホスト側へ転送する。
static void led_blinking_task(USBHost::Keyboard& keyboard)
{
    static uint8_t  oldleds   = 0xFF;  // 初回強制送信のため 0xFF で初期化

    // キーボード LED の変化をキーボードへ送信する
    if (s_keybordleds != oldleds) {
        if (keyboard.IsMounted() && keyboard.GetHID().IsSendReady()) {
            keyboard.GetHID().SendReport(0, &s_keybordleds, sizeof(s_keybordleds));
            oldleds = s_keybordleds;
        }
    }

    const uint32_t now_ms = to_ms_since_boot(get_absolute_time());
    const uint8_t boardBlue = calc_breath_brightness(now_ms, s_blink_interval_ms);
    const uint8_t fastPulse = calc_breath_brightness(now_ms, BLINK_FAST);
    const bool keyPressed = has_any_msx_selected_key();

    uint8_t blueLevel = boardBlue;
    uint8_t greenLevel = boardBlue;
    uint8_t redLevel = boardBlue;

    if (s_hbf500SelfTestActive) {
        redLevel = boardBlue;
        greenLevel = 0;
        blueLevel = 0;
    } else if (s_hbf500SelfTestPassed) {
        redLevel = 0;
        greenLevel = boardBlue;
        blueLevel = 0;
    } else if (s_fileAccessActive) {
        greenLevel = fastPulse;
    } else if (keyPressed) {
        greenLevel = 0;
    }
    if (s_fileErrorActive) {
        redLevel = fastPulse;
    }

    led_backend_set_rgb(redLevel, greenLevel, blueLevel);
}

// LFS 更新後にキーマップ、ゲームパッドマップ、VIDCFG.INI を再読込する。
static void reload_lfs_runtime_data(const char* logPrefix)
{
  s_fileAccessActive = true;
  bool hadError = false;
  static const char* kKeymapPathTbl[] = {
    "A:/KEYMAP0.JSN",
    "A:/KEYMAP1.JSN",
    "A:/KEYMAP2.JSN",
    "A:/KEYMAP3.JSN",
  };
  for (int i = 0; i < static_cast<int>(count_of(kKeymapPathTbl)); i++) {
    if (readAndSetKeymap(kKeymapPathTbl[i], i) != 0) {
      ::printf("%s keymap load failed: %s\n", logPrefix, kKeymapPathTbl[i]);
      hadError = true;
      continue;
    }
    print_profile_tables(i);
  }

  if (readConfigNumberCSV("A:/VIDCFG.INI") < 0) {
    hadError = true;
  }

  if (s_keyboardMounted) {
    const int cfg = findConfigNumberCSV(s_vid, s_pid);
    if (cfg >= 0) s_configSettingNumber = cfg;
  }

  int gpadCfg = 0;
  if (s_gamePadMounted) {
    const int mapped = findGpadConfigNumberCSV(s_gvid, s_gpid);
    if (mapped >= 0) gpadCfg = mapped;
  }
  s_gpadConfigSettingNumber = gpadCfg;
  char gpadPath[32];
  ::snprintf(gpadPath, sizeof(gpadPath), "A:/GPADMAP%d.JSN", s_gpadConfigSettingNumber);
  if (readAndSetGamepadMap(gpadPath) != 0) {
    ::printf("%s gamepad map load failed: %s\n", logPrefix, gpadPath);
    hadError = true;
    if (readAndSetGamepadMap("A:/GPADMAP0.JSN") != 0) {
      ::printf("%s gamepad map load failed: A:/GPADMAP0.JSN\n", logPrefix);
      hadError = true;
    }
    s_gpadConfigSettingNumber = 0;
  }
  s_fileErrorActive = hadError;
  s_fileAccessActive = false;
}

// 起動時に A: をマウントし、LFS 実行時データを準備する。
static void handle_lfs_startup() {
  // A: をマウント。失敗時はフォーマットを試す（元コード同等）
  if (!FS::Mount("A:")) {
    ::printf("[LFS] A: not formatted, formatting ...\n");
    if (!FS::Format("A:")) {
      ::printf("[LFS] A: format failed\n");
      s_fileErrorActive = true;
      return;
    } else {
      ::printf("[LFS] A: formatted and mounted\n");
      // LFS::Drive::Format() completes by mounting the newly formatted drive.
    }
  } else {
    ::printf("[LFS] A: mounted\n");
  }

  // すべての対象ファイルを1回で処理
  for (int fileIdx = 0; fileIdx < static_cast<int>(count_of(kFilesToSync)); ++fileIdx) {
    const SyncFile& sf = kFilesToSync[fileIdx];
    char path[64];
    ::snprintf(path, sizeof(path), "A:/%s", sf.name);

    // 無ければ作成
    if (!FS::DoesExist(path)) {
      ::printf("[LFS] %s: not found, creating ...\n", sf.name);
      std::unique_ptr<FS::File> pF(FS::OpenFile(path, "w"));
      if (pF) {
        pF->Write(sf.defaultContent, static_cast<int>(::strlen(sf.defaultContent)));
        ::printf("[LFS] %s: created\n", sf.name);
      } else {
        ::printf("[LFS] %s: create failed\n", sf.name);
        s_fileErrorActive = true;
      }
      continue;
    }

// 既存内容の表示は停止
//    ::printf("=== A:/%s ===\n", sf.name);
//    std::unique_ptr<FS::File> pFileHandle(FS::OpenFile(path, "r"));
//    if (!pFileHandle) {
//      ::printf("[LFS] %s: open failed\n", sf.name);
//      continue;
//    }
//
//    while (true) {
//      char buf[48];
//      int n = pFileHandle->Read(buf, static_cast<int>(sizeof(buf)) - 1);
//      if (n > 0) {
//        buf[n] = '\0';
//        ::printf("%s", buf);
//      } else {
//        ::printf("\n");
//        break;
//      }
//    }
  }

  reload_lfs_runtime_data("[LFS]");
}
// flash_safe_execute のコールバックで使う BOOTSEL 読み取り結果。
struct BootselReadContext {
  bool pressed;
};

// BOOT button long-press reset: format A:, restore defaults, reload maps
// フラッシュ実行を避けて BOOTSEL ボタン状態を読む。
static void __no_inline_not_in_flash_func(read_bootsel_button_pressed_impl)(void* param)
{
  BootselReadContext* context = static_cast<BootselReadContext*>(param);
  const uint CS_PIN_INDEX = 1;
  const uint32_t ctrl = ioqspi_hw->io[CS_PIN_INDEX].ctrl;
  uint32_t flags = save_and_disable_interrupts();

  hw_write_masked(&ioqspi_hw->io[CS_PIN_INDEX].ctrl,
                  GPIO_OVERRIDE_LOW << IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_LSB,
                  IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_BITS);

  for (volatile int i = 0; i < 1000; ++i) {
  }

  context->pressed = (sio_hw->gpio_hi_in & SIO_GPIO_HI_IN_QSPI_CSN_BITS) == 0;

  ioqspi_hw->io[CS_PIN_INDEX].ctrl = ctrl;

  restore_interrupts(flags);
}

// flash_safe_execute で Core 1 を停止させたうえで BOOTSEL 状態を取得する。
static bool read_bootsel_button_pressed()
{
  BootselReadContext context = {false};
  const int rc = flash_safe_execute(read_bootsel_button_pressed_impl, &context, 1000);
  if (rc != PICO_OK) {
    if (!s_bootselReadFaultActive) {
      ::printf("[BOOT] BOOTSEL read failed: rc=%d\n", rc);
      s_bootselReadFaultActive = true;
    }
    return false;
  }

  if (s_bootselReadFaultActive) {
    ::printf("[BOOT] BOOTSEL read recovered\n");
    s_bootselReadFaultActive = false;
  }

  return context.pressed;
}

// 通常動作開始時の BOOTSEL 状態を基準として記録する。
// 起動時に押されていた場合は、いったん離されるまで長押しを受け付けない。
static void initialize_bootsel_long_press()
{
  const uint32_t nowMs = to_ms_since_boot(get_absolute_time());
  const bool pressed = read_bootsel_button_pressed();

  s_bootselRawPressed = pressed;
  s_bootselDebouncedPressed = pressed;
  s_bootselLastChangeMs = nowMs;
  s_bootselLastPollMs = nowMs;
  s_bootselLongPressArmed = !pressed;
  s_bootButtonPressStartMs = 0;
  s_bootLongPressHandled = false;

}


// BOOTSEL 長押し後にデフォルトのファイルセットへ戻す。
static bool reset_lfs_files_to_default_from_boot()
{
  ::printf("[BOOT] 5s hold detected: resetting A: to default files ...\n");

  finish_usb_msc_task();
  s_usbMscMountEvent = false;
  s_usbMscUnmountEvent = false;

  FS::Unmount("A:");
  if (!FS::Format("A:")) {
    ::printf("[BOOT] A: format failed\n");
    s_fileErrorActive = true;
    return false;
  }
  // LFS::Drive::Format() completes by mounting the newly formatted drive.

  for (int i = 0; i < static_cast<int>(count_of(kFilesToSync)); ++i) {
    const SyncFile& sf = kFilesToSync[i];
    char path[64];
    ::snprintf(path, sizeof(path), "A:/%s", sf.name);
    std::unique_ptr<FS::File> pF(FS::OpenFile(path, "w"));
    if (pF) {
      pF->Write(sf.defaultContent, static_cast<int>(::strlen(sf.defaultContent)));
      ::printf("[BOOT] %s: reset to default\n", sf.name);
    } else {
      ::printf("[BOOT] %s: create failed\n", sf.name);
      s_fileErrorActive = true;
    }
  }

  s_gpadConfigSettingNumber = 0;
  reload_lfs_runtime_data("[BOOT]");

  s_configMode = CFG_NONE;
  s_keyboardStrMode = false;
  s_keyboardStrWaitRelease = false;
  s_keyboardStrClearPending = false;
  s_keyboardAnyKeyPressed = false;
  s_keyboardStr.reset();
  s_prevKeyboardReport = {0, {0, 0, 0, 0, 0, 0}};
  reset_modifier_transition_state();
  reset_gamepad_runtime_state();
  clear_msx_selected_keys();

  ::printf("[BOOT] LFS reset completed\n");
  return true;
}

// BOOTSEL の長押しによるリセット要求を監視する。
static void boot_button_long_press_task()
{
  const uint32_t nowMs = to_ms_since_boot(get_absolute_time());
  if ((nowMs - s_bootselLastPollMs) < BOOT_POLL_INTERVAL_MS) return;
  s_bootselLastPollMs = nowMs;

  const bool pressed = read_bootsel_button_pressed();

  if (pressed != s_bootselRawPressed) {
    s_bootselRawPressed = pressed;
    s_bootselLastChangeMs = nowMs;
    ::printf("[BOOT] BOOTSEL raw=%s at %lums\n", pressed ? "pressed" : "released",
             static_cast<unsigned long>(nowMs));
  }

  if ((nowMs - s_bootselLastChangeMs) < BOOT_DEBOUNCE_MS) return;
  s_bootselDebouncedPressed = s_bootselRawPressed;

  if (!s_bootselDebouncedPressed) {
    if (!s_bootselLongPressArmed) {
      ::printf("[BOOT] BOOTSEL armed\n");
    }
    s_bootselLongPressArmed = true;
    s_bootButtonPressStartMs = 0;
    s_bootLongPressHandled = false;
    return;
  }

  if (!s_bootselLongPressArmed) return;

  if (s_bootButtonPressStartMs == 0) {
    s_bootButtonPressStartMs = nowMs;
    ::printf("[BOOT] BOOTSEL hold started\n");
    return;
  }

  if (s_bootLongPressHandled) return;
  if ((nowMs - s_bootButtonPressStartMs) < BOOT_LONGPRESS_MS) return;

  s_bootLongPressHandled = true;
  (void)reset_lfs_files_to_default_from_boot();
}

// USB HID report (modifier + reserved + 6 keycodes)
// 現在のキーボードレポートとそこから導かれるマトリクス動作を表示する。
// Dumps the current keyboard report and derived matrix actions.
static void print_keyboard_keycodes(USBHost::Keyboard& keyboard) {
  KeyboardReport report = {0, {0, 0, 0, 0, 0, 0}};
  if (!keyboard.IsMounted()) {
    s_keyboardAnyKeyPressed = false;
    s_prevKeyboardReport = {0, {0, 0, 0, 0, 0, 0}};
    reset_modifier_transition_state();
    return;
  }
  const USBHost::HID::Report& hidReport = keyboard.GetReport();
  if (hidReport.buff == nullptr || hidReport.len < sizeof(hid_keyboard_report_t)) {
    return;
  }
  const hid_keyboard_report_t& usbReport = *reinterpret_cast<const hid_keyboard_report_t*>(hidReport.buff);
  report.modifier = usbReport.modifier;
  for (int i = 0; i < static_cast<int>(count_of(report.keycode)); ++i) {
    report.keycode[i] = usbReport.keycode[i];
  }
  s_keyboardAnyKeyPressed = (report.modifier != 0);
  if (!s_keyboardAnyKeyPressed) {
    for (int i = 0; i < static_cast<int>(count_of(report.keycode)); ++i) {
      if (report.keycode[i] != 0) { s_keyboardAnyKeyPressed = true; break; }
    }
  }
  process_kbd_report(&report);
}

// ゲームパッド 1 軸の正規化値を返す。
// ゲームパッド 1 軸の正規化値を返す。
static float get_gamepad_axis_value(const USBHost::GamePad& gamePad, int axisIndex)
{
  switch (axisIndex) {
    case 0: return gamePad.Get_Axis0();
    case 1: return gamePad.Get_Axis1();
    case 2: return gamePad.Get_Axis2();
    case 3: return gamePad.Get_Axis3();
    case 4: return gamePad.Get_Axis4();
    case 5: return gamePad.Get_Axis5();
    case 6: return gamePad.Get_Axis6();
    case 7: return gamePad.Get_Axis7();
    case 8: return gamePad.Get_Axis8();
    default: return 0.0f;
  }
}

// ゲームパッド 1 軸の生値を返す。
// ゲームパッド 1 軸の生値を返す。
static uint32_t get_gamepad_axis_raw_value(const USBHost::GamePad& gamePad, int axisIndex)
{
  switch (axisIndex) {
    case 0: return gamePad.GetRaw_Axis0();
    case 1: return gamePad.GetRaw_Axis1();
    case 2: return gamePad.GetRaw_Axis2();
    case 3: return gamePad.GetRaw_Axis3();
    case 4: return gamePad.GetRaw_Axis4();
    case 5: return gamePad.GetRaw_Axis5();
    case 6: return gamePad.GetRaw_Axis6();
    case 7: return gamePad.GetRaw_Axis7();
    case 8: return gamePad.GetRaw_Axis8();
    default: return 0;
  }
}

// ゲームパッドのマッピング遷移 1 件を出力する。
// 押下状態が変化したときにゲームパッドのマトリクス遷移を出力する。
static void emit_gamepad_matrix_changed(const char* slotName, uint8_t usageCode,
                                        const MatrixTable& matrix, bool pressedNow, bool& pressedPrev)
{
  if (pressedNow == pressedPrev) return;
  pressedPrev = pressedNow;
  matrix_action_emit("GamePad", slotName, usageCode, matrix, pressedNow);
}

// ゲームパッドを監視し、ボタン・Hat・軸のイベントを処理する。
// ゲームパッドを監視してボタン、Hat、軸のマッピングを処理する。
static void gamepad_monitor_task(USBHost::GamePad& gamePad)
{
  if (!gamePad.HasReportChanged()) return;
  if (!gamePad.IsSwitchProReady()) return;

//  ::printf("[GPAD] %s%s%s%s%s%s%s%s%s%s%s%s%s %X % 1.2f % 1.2f % 1.2f % 1.2f % 1.2f % 1.2f % 1.2f % 1.2f % 1.2f\n",
//    gamePad.Get_Button0()?  "0" : ".",
//    gamePad.Get_Button1()?  "1" : ".",
//    gamePad.Get_Button2()?  "2" : ".",
//    gamePad.Get_Button3()?  "3" : ".",
//    gamePad.Get_Button4()?  "4" : ".",
//    gamePad.Get_Button5()?  "5" : ".",
//    gamePad.Get_Button6()?  "6" : ".",
//    gamePad.Get_Button7()?  "7" : ".",
//    gamePad.Get_Button8()?  "8" : ".",
//    gamePad.Get_Button9()?  "9" : ".",
//    gamePad.Get_Button10()? "A" : ".",
//    gamePad.Get_Button11()? "B" : ".",
//    gamePad.Get_Button12()? "C" : ".",
//    gamePad.Get_HatSwitch(),
//    gamePad.Get_Axis0(),
//    gamePad.Get_Axis1(),
//    gamePad.Get_Axis2(),
//    gamePad.Get_Axis3(),
//    gamePad.Get_Axis4(),
//    gamePad.Get_Axis5(),
//    gamePad.Get_Axis6(),
 //   gamePad.Get_Axis7(),
//    gamePad.Get_Axis8());

  if (!s_gamepadMapLoaded) return;

  bool buttons[13] = {
    gamePad.Get_Button0(),  gamePad.Get_Button1(),  gamePad.Get_Button2(),
    gamePad.Get_Button3(),  gamePad.Get_Button4(),  gamePad.Get_Button5(),
    gamePad.Get_Button6(),  gamePad.Get_Button7(),  gamePad.Get_Button8(),
    gamePad.Get_Button9(),  gamePad.Get_Button10(), gamePad.Get_Button11(),
    gamePad.Get_Button12()
  };
  for (int i = 0; i < 13; i++) {
    emit_gamepad_matrix_changed("A", static_cast<uint8_t>(i), s_gamepadMap.button[i], buttons[i], s_prevButtonPressed[i]);
  }

  const uint32_t hat = gamePad.Get_HatSwitch();
  // Get_HatSwitch() 返り値: 0=未押下, 1=N, 2=NE, 3=E, 4=SE, 5=S, 6=SW, 7=W, 8=NW
  const bool hatU = (hat == 1 || hat == 2 || hat == 8);  // N, NE, NW
  const bool hatR = (hat == 2 || hat == 3 || hat == 4);  // NE, E, SE
  const bool hatD = (hat == 4 || hat == 5 || hat == 6);  // SE, S, SW
  const bool hatL = (hat == 6 || hat == 7 || hat == 8);  // SW, W, NW
  emit_gamepad_matrix_changed("A", 0x20, s_gamepadMap.hat[0], hatU, s_prevHatPressed[0]);
  emit_gamepad_matrix_changed("A", 0x21, s_gamepadMap.hat[1], hatD, s_prevHatPressed[1]);
  emit_gamepad_matrix_changed("A", 0x22, s_gamepadMap.hat[2], hatL, s_prevHatPressed[2]);
  emit_gamepad_matrix_changed("A", 0x23, s_gamepadMap.hat[3], hatR, s_prevHatPressed[3]);

  for (int i = 0; i < 9; i++) {
    // Cooked 値 [-1,1] で判定: 負の閾値(例 -0.6) なら axis<=閾値でON、正の閾値(例 0.6) なら axis>=閾値でON
    const float axisCooked = get_gamepad_axis_value(gamePad, i);
    const bool minOn = axisCooked <= s_gamepadMap.axisMinValue[i];
    const bool maxOn = axisCooked >= s_gamepadMap.axisMaxValue[i];
    emit_gamepad_matrix_changed("A", static_cast<uint8_t>(0x40 + i * 2), s_gamepadMap.axisMin[i], minOn, s_prevAxisMinPressed[i]);
    emit_gamepad_matrix_changed("A", static_cast<uint8_t>(0x41 + i * 2), s_gamepadMap.axisMax[i], maxOn, s_prevAxisMaxPressed[i]);
  }
}

// プログラムのエントリポイント。
int main() {
 
  set_sys_clock_khz(240000, true);
  ::stdio_init_all();
  sleep_ms(10);
  led_backend_init();
#if HBF500_ENABLE
  hbf500_init();
#endif
  initialize_bootsel_long_press();
  ::printf("ILF SONY External Keyboard Unit for SONY HB-F500/900\n");
  ::printf("Copyright @v9938 "); // 起動画面
  ::printf("Release Data: %s\n\n",__DATE__);

  LFS::Flash driveA("A:", 0x10100000, 0x00040000);
  (void)driveA;

  // Complete any initial format and configuration writes before USB enumeration.
  handle_lfs_startup();

  USBHost::Initialize(0);
  USBHost::Keyboard keyboard;
  USBHost::GamePad gamePad;

  while (true) {
    handle_usb_msc_events();
    process_usb_msc_events();
    boot_button_long_press_task();

    // キーボードの接続/切断を検出して VID/PID → キーマップ番号を設定する
    bool currentMounted = keyboard.IsMounted();
    if (currentMounted && !s_keyboardMounted) {
      // 接続イベント: VID/PID を取得してキーマップ番号を決定する
      s_vid = keyboard.GetHID().GetVID();
      s_pid = keyboard.GetHID().GetPID();
      int cfg = findConfigNumberCSV(s_vid, s_pid);
      if (cfg < 0) {
        // 未登録のデバイス: プロファイル 0 をデフォルトとして登録する
        cfg = 0;
        updateConfigNumberCSV("A:/VIDCFG.INI", s_vid, s_pid, "KEYMAP0.JSN");
      }
      s_configSettingNumber = cfg;
      ::printf("[KBD] using profile %d\n", s_configSettingNumber);
      s_blink_interval_ms = BLINK_MOUNTED;
      s_keyboardMounted = true;
      clear_msx_selected_keys();
    } else if (!currentMounted && s_keyboardMounted) {
      // 切断イベント: 状態をリセットする
      ::printf("[KBD] unmounted\n");
      s_keyboardMounted = false;
      s_vid = 0;
      s_pid = 0;
      s_blink_interval_ms = BLINK_NOT_MOUNTED;
      clear_msx_selected_keys();
    }

    // GamePad の接続/切断を検出する
    bool currentGamePadMounted = gamePad.IsMounted();
    if (currentGamePadMounted && !s_gamePadMounted) {
      s_gvid = gamePad.GetVID();
      s_gpid = gamePad.GetPID();

      // VID/PID → GamePad プロファイル番号を検索
      int gcfg = findGpadConfigNumberCSV(s_gvid, s_gpid);
      if (gcfg < 0) {
        // 未登録: プロファイル 0 をデフォルトとして登録する
        gcfg = 0;
        updateConfigNumberCSV("A:/VIDCFG.INI", s_gvid, s_gpid, "GPADMAP0.JSN");
      }
      s_gpadConfigSettingNumber = gcfg;

      // 対応する GPADMAP?.JSN をロード
      char gpadPath[32];
      ::snprintf(gpadPath, sizeof(gpadPath), "A:/GPADMAP%d.JSN", s_gpadConfigSettingNumber);
      if (readAndSetGamepadMap(gpadPath) != 0) {
        ::printf("[GPAD] map load failed: %s, using GPADMAP0.JSN\n", gpadPath);
        readAndSetGamepadMap("A:/GPADMAP0.JSN");
        s_gpadConfigSettingNumber = 0;
      }
      ::printf("[GPAD] mounted VID=%04x PID=%04x (map=GPADMAP%d.JSN)\n",
               s_gvid, s_gpid, s_gpadConfigSettingNumber);

      s_gamePadMounted = true;
      reset_gamepad_runtime_state();
      clear_msx_selected_keys();
    } else if (!currentGamePadMounted && s_gamePadMounted) {
      ::printf("[GPAD] unmounted\n");
      s_gamePadMounted = false;
      s_gvid = 0;
      s_gpid = 0;
      reset_gamepad_runtime_state();
      clear_msx_selected_keys();
    }

    // 未マウント時の不要処理を抑止する
    if (s_keyboardMounted) {
      print_keyboard_keycodes(keyboard);
    }
    if (s_gamePadMounted) {
      gamepad_monitor_task(gamePad);
    }
    led_blinking_task(keyboard);
    keyboardStrTask();
#if HBF500_ENABLE
    hbf500_matrix_task();
    hbf500_selftest_task();
#endif
    Tickable::Tick();
  }
}
