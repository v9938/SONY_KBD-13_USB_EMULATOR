#define JSON_DEF2 \
"{ \n" \
"\t\"#comment\" : [ \n" \
"\t\t\"ファイルサイズは32KByte以下にしてください,\", \n" \
"\t\t\"ROOT名  [Modifier],[Usage],[ASCII]のいずれか\", \n" \
"\t\t\"  書式  [Id]   USBのキーIDコード(HEX)\", \n" \
"\t\t\"        [Y]    Matrixの番号0-16 17=PAUSE,18=RESET [0x00は割り当て無し]\", \n" \
"\t\t\"        [Mask] 当該Matrixのマスク値、当該BITを1にするとON\", \n" \
"\t\t\"        [Name] キー名（未使用)\", \n" \
"\t\t\"(YとMASKは一つのキーに2つまで持てる)\" \n" \
"\t], \n" \
"\t\"Keymap\" : { \n" \
"\t\t\"Modifier\" : [ \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x01\", \n" \
"\t\t\t\t\"Name\" : \"L Ctrl\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x10\", \n" \
"\t\t\t\t\"Name\" : \"R Ctrl\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x02\", \n" \
"\t\t\t\t\"Name\" : \"L Shift\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x20\", \n" \
"\t\t\t\t\"Name\" : \"R Shift\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x04\", \n" \
"\t\t\t\t\"Name\" : \"L Alt\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x40\", \n" \
"\t\t\t\t\"Name\" : \"R Alt\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x08\", \n" \
"\t\t\t\t\"Name\" : \"R GUI\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x80\", \n" \
"\t\t\t\t\"Name\" : \"L GUI\" \n" \
"\t\t\t} \n" \
"\t\t], \n" \
"\t\t\"Usage\" : [ \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 3, 0], \n" \
"\t\t\t\t\"Mask\" : [ 64, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x04\", \n" \
"\t\t\t\t\"Name\" : \"Key A\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 3, 0], \n" \
"\t\t\t\t\"Mask\" : [ 128, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x05\", \n" \
"\t\t\t\t\"Name\" : \"Key B\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 4, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x06\", \n" \
"\t\t\t\t\"Name\" : \"Key C\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 4, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x07\", \n" \
"\t\t\t\t\"Name\" : \"Key D\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 4, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x08\", \n" \
"\t\t\t\t\"Name\" : \"Key E\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 4, 0], \n" \
"\t\t\t\t\"Mask\" : [ 8, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x09\", \n" \
"\t\t\t\t\"Name\" : \"Key F\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 4, 0], \n" \
"\t\t\t\t\"Mask\" : [ 16, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x0a\", \n" \
"\t\t\t\t\"Name\" : \"Key G\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 4, 0], \n" \
"\t\t\t\t\"Mask\" : [ 32, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x0b\", \n" \
"\t\t\t\t\"Name\" : \"Key H\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 4, 0], \n" \
"\t\t\t\t\"Mask\" : [ 64, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x0c\", \n" \
"\t\t\t\t\"Name\" : \"Key I\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 4, 0], \n" \
"\t\t\t\t\"Mask\" : [ 128, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x0d\", \n" \
"\t\t\t\t\"Name\" : \"Key J\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 5, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x0e\", \n" \
"\t\t\t\t\"Name\" : \"Key K\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 5, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x0f\", \n" \
"\t\t\t\t\"Name\" : \"Key L\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 5, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x10\", \n" \
"\t\t\t\t\"Name\" : \"Key M\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 5, 0], \n" \
"\t\t\t\t\"Mask\" : [ 8, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x11\", \n" \
"\t\t\t\t\"Name\" : \"Key N\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 5, 0], \n" \
"\t\t\t\t\"Mask\" : [ 16, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x12\", \n" \
"\t\t\t\t\"Name\" : \"Key O\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 5, 0], \n" \
"\t\t\t\t\"Mask\" : [ 32, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x13\", \n" \
"\t\t\t\t\"Name\" : \"Key P\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 5, 0], \n" \
"\t\t\t\t\"Mask\" : [ 64, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x14\", \n" \
"\t\t\t\t\"Name\" : \"Key Q\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 5, 0], \n" \
"\t\t\t\t\"Mask\" : [ 128, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x15\", \n" \
"\t\t\t\t\"Name\" : \"Key R\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 6, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x16\", \n" \
"\t\t\t\t\"Name\" : \"Key S\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 6, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x17\", \n" \
"\t\t\t\t\"Name\" : \"Key T\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 6, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x18\", \n" \
"\t\t\t\t\"Name\" : \"Key U\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 6, 0], \n" \
"\t\t\t\t\"Mask\" : [ 8, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x19\", \n" \
"\t\t\t\t\"Name\" : \"Key V\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 6, 0], \n" \
"\t\t\t\t\"Mask\" : [ 16, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x1a\", \n" \
"\t\t\t\t\"Name\" : \"Key W\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 6, 0], \n" \
"\t\t\t\t\"Mask\" : [ 32, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x1b\", \n" \
"\t\t\t\t\"Name\" : \"Key X\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 6, 0], \n" \
"\t\t\t\t\"Mask\" : [ 64, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x1c\", \n" \
"\t\t\t\t\"Name\" : \"Key Y\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 6, 0], \n" \
"\t\t\t\t\"Mask\" : [ 128, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x1d\", \n" \
"\t\t\t\t\"Name\" : \"Key Z\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 1, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x1e\", \n" \
"\t\t\t\t\"Name\" : \"Key 1 and !\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 1, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x1f\", \n" \
"\t\t\t\t\"Name\" : \"Key 2 and @\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 1, 0], \n" \
"\t\t\t\t\"Mask\" : [ 8, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x20\", \n" \
"\t\t\t\t\"Name\" : \"Key 3 and #\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 1, 0], \n" \
"\t\t\t\t\"Mask\" : [ 16, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x21\", \n" \
"\t\t\t\t\"Name\" : \"Key 4 and $\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 1, 0], \n" \
"\t\t\t\t\"Mask\" : [ 32, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x22\", \n" \
"\t\t\t\t\"Name\" : \"Key 5 and %\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 1, 0], \n" \
"\t\t\t\t\"Mask\" : [ 64, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x23\", \n" \
"\t\t\t\t\"Name\" : \"Key 6 and ^\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 1, 0], \n" \
"\t\t\t\t\"Mask\" : [ 128, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x24\", \n" \
"\t\t\t\t\"Name\" : \"Key 7 and [and]\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 2, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x25\", \n" \
"\t\t\t\t\"Name\" : \"Key 8 and *\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 2, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x26\", \n" \
"\t\t\t\t\"Name\" : \"Key 9 and (\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 1, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x27\", \n" \
"\t\t\t\t\"Name\" : \"Key 0 and )\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 8, 0], \n" \
"\t\t\t\t\"Mask\" : [ 128, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x28\", \n" \
"\t\t\t\t\"Name\" : \"Key Return (ENTER)\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 8, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x29\", \n" \
"\t\t\t\t\"Name\" : \"Key ESCAPE\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 8, 0], \n" \
"\t\t\t\t\"Mask\" : [ 32, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x2a\", \n" \
"\t\t\t\t\"Name\" : \"Key DELETE (Backspace)\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 8, 0], \n" \
"\t\t\t\t\"Mask\" : [ 8, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x2b\", \n" \
"\t\t\t\t\"Name\" : \"Key Tab\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 9, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x2c\", \n" \
"\t\t\t\t\"Name\" : \"Key Spacebar\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 2, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x2d\", \n" \
"\t\t\t\t\"Name\" : \"Key - and _\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 2], \n" \
"\t\t\t\t\"Mask\" : [ 1, 4], \n" \
"\t\t\t\t\"Id\"   : \"0x2e\", \n" \
"\t\t\t\t\"Name\" : \"Key = and +\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 2, 0], \n" \
"\t\t\t\t\"Mask\" : [ 64, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x2f\", \n" \
"\t\t\t\t\"Name\" : \"Key [ and {\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 3, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x30\", \n" \
"\t\t\t\t\"Name\" : \"Key ] and }\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 2, 0], \n" \
"\t\t\t\t\"Mask\" : [ 16, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x31\", \n" \
"\t\t\t\t\"Name\" : \"Key ￥ and |\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 3, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x32\", \n" \
"\t\t\t\t\"Name\" : \"Key Non-US # and ~\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 2, 0], \n" \
"\t\t\t\t\"Mask\" : [ 128, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x33\", \n" \
"\t\t\t\t\"Name\" : \"Key ; and :\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 1], \n" \
"\t\t\t\t\"Mask\" : [ 1, 128], \n" \
"\t\t\t\t\"Id\"   : \"0x34\", \n" \
"\t\t\t\t\"Name\" : \"Key ’ and ”\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 2], \n" \
"\t\t\t\t\"Mask\" : [ 1, 32], \n" \
"\t\t\t\t\"Id\"   : \"0x35\", \n" \
"\t\t\t\t\"Name\" : \"Key ZENKAKU\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 3, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x36\", \n" \
"\t\t\t\t\"Name\" : \"Key ，and ＜\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 3, 0], \n" \
"\t\t\t\t\"Mask\" : [ 8, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x37\", \n" \
"\t\t\t\t\"Name\" : \"Key .  and ＞ \" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 3, 0], \n" \
"\t\t\t\t\"Mask\" : [ 16, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x38\", \n" \
"\t\t\t\t\"Name\" : \"Key / and ?\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 0], \n" \
"\t\t\t\t\"Mask\" : [ 8, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x39\", \n" \
"\t\t\t\t\"Name\" : \"Key Caps Lock\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 0], \n" \
"\t\t\t\t\"Mask\" : [ 32, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x3a\", \n" \
"\t\t\t\t\"Name\" : \"Key F1\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 0], \n" \
"\t\t\t\t\"Mask\" : [ 64, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x3b\", \n" \
"\t\t\t\t\"Name\" : \"Key F2\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 0], \n" \
"\t\t\t\t\"Mask\" : [ 128, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x3c\", \n" \
"\t\t\t\t\"Name\" : \"Key F3\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 8, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x3d\", \n" \
"\t\t\t\t\"Name\" : \"Key F4\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 8, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x3e\", \n" \
"\t\t\t\t\"Name\" : \"Key F5\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 7], \n" \
"\t\t\t\t\"Mask\" : [ 1, 32], \n" \
"\t\t\t\t\"Id\"   : \"0x3f\", \n" \
"\t\t\t\t\"Name\" : \"Key F6\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 7], \n" \
"\t\t\t\t\"Mask\" : [ 1, 64], \n" \
"\t\t\t\t\"Id\"   : \"0x40\", \n" \
"\t\t\t\t\"Name\" : \"Key F7\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 7], \n" \
"\t\t\t\t\"Mask\" : [ 1, 128], \n" \
"\t\t\t\t\"Id\"   : \"0x41\", \n" \
"\t\t\t\t\"Name\" : \"Key F8\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 8], \n" \
"\t\t\t\t\"Mask\" : [ 1, 1], \n" \
"\t\t\t\t\"Id\"   : \"0x42\", \n" \
"\t\t\t\t\"Name\" : \"Key F9\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 8], \n" \
"\t\t\t\t\"Mask\" : [ 1, 2], \n" \
"\t\t\t\t\"Id\"   : \"0x43\", \n" \
"\t\t\t\t\"Name\" : \"Key F10\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 17, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x44\", \n" \
"\t\t\t\t\"Name\" : \"Key F11\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 12, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x45\", \n" \
"\t\t\t\t\"Name\" : \"Key F12\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 0], \n" \
"\t\t\t\t\"Mask\" : [ 16, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x46\", \n" \
"\t\t\t\t\"Name\" : \"Key Print Screen\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 8, 0], \n" \
"\t\t\t\t\"Mask\" : [ 64, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x47\", \n" \
"\t\t\t\t\"Name\" : \"Key Scroll Lock\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 8, 0], \n" \
"\t\t\t\t\"Mask\" : [ 16, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x48\", \n" \
"\t\t\t\t\"Name\" : \"Key Pause\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 9, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x49\", \n" \
"\t\t\t\t\"Name\" : \"Key Insert\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 9, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x4a\", \n" \
"\t\t\t\t\"Name\" : \"Key Home\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 9, 0], \n" \
"\t\t\t\t\"Mask\" : [ 32, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x4b\", \n" \
"\t\t\t\t\"Name\" : \"Key Page Up\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 9, 0], \n" \
"\t\t\t\t\"Mask\" : [ 8, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x4c\", \n" \
"\t\t\t\t\"Name\" : \"Key Delete Forward\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x4d\", \n" \
"\t\t\t\t\"Name\" : \"Key End\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 9, 0], \n" \
"\t\t\t\t\"Mask\" : [ 64, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x4e\", \n" \
"\t\t\t\t\"Name\" : \"Key Page Down\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 9, 0], \n" \
"\t\t\t\t\"Mask\" : [ 128, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x4f\", \n" \
"\t\t\t\t\"Name\" : \"Key Right Arrow\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 9, 0], \n" \
"\t\t\t\t\"Mask\" : [ 16, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x50\", \n" \
"\t\t\t\t\"Name\" : \"Key Left Arrow\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 9, 0], \n" \
"\t\t\t\t\"Mask\" : [ 64, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x51\", \n" \
"\t\t\t\t\"Name\" : \"Key Down Arrow\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 9, 0], \n" \
"\t\t\t\t\"Mask\" : [ 32, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x52\", \n" \
"\t\t\t\t\"Name\" : \"Key Up Arrow\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x53\", \n" \
"\t\t\t\t\"Name\" : \"Key Num Lock and Clear\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 10, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x54\", \n" \
"\t\t\t\t\"Name\" : \"Keypad /\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 10, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x55\", \n" \
"\t\t\t\t\"Name\" : \"Keypad *\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 11, 0], \n" \
"\t\t\t\t\"Mask\" : [ 32, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x56\", \n" \
"\t\t\t\t\"Name\" : \"Keypad -\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 10, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x57\", \n" \
"\t\t\t\t\"Name\" : \"Keypad +\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 8, 0], \n" \
"\t\t\t\t\"Mask\" : [ 128, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x58\", \n" \
"\t\t\t\t\"Name\" : \"Keypad ENTER\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 10, 0], \n" \
"\t\t\t\t\"Mask\" : [ 16, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x59\", \n" \
"\t\t\t\t\"Name\" : \"Keypad 1 and End\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 10, 0], \n" \
"\t\t\t\t\"Mask\" : [ 32, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x5a\", \n" \
"\t\t\t\t\"Name\" : \"Keypad 2 and Down Arrow\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 10, 0], \n" \
"\t\t\t\t\"Mask\" : [ 64, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x5b\", \n" \
"\t\t\t\t\"Name\" : \"Keypad 3 and PageDn\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 10, 0], \n" \
"\t\t\t\t\"Mask\" : [ 128, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x5c\", \n" \
"\t\t\t\t\"Name\" : \"Keypad 4 and Left Arrow\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 11, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x5d\", \n" \
"\t\t\t\t\"Name\" : \"Keypad 5\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 11, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x5e\", \n" \
"\t\t\t\t\"Name\" : \"Keypad 6 and Right Arrow\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 11, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x5f\", \n" \
"\t\t\t\t\"Name\" : \"Keypad 7 and Home\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 11, 0], \n" \
"\t\t\t\t\"Mask\" : [ 8, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x60\", \n" \
"\t\t\t\t\"Name\" : \"Keypad 8 and Up Arrow\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 11, 0], \n" \
"\t\t\t\t\"Mask\" : [ 16, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x61\", \n" \
"\t\t\t\t\"Name\" : \"Keypad 9 and Page Up\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 10, 0], \n" \
"\t\t\t\t\"Mask\" : [ 8, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x62\", \n" \
"\t\t\t\t\"Name\" : \"Keypad 0 and Insert\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 11, 0], \n" \
"\t\t\t\t\"Mask\" : [ 128, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x63\", \n" \
"\t\t\t\t\"Name\" : \"Keypad . and Delete\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x64\", \n" \
"\t\t\t\t\"Name\" : \"Key Non-US ￥ and |\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x65\", \n" \
"\t\t\t\t\"Name\" : \"Key Application\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 17, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x66\", \n" \
"\t\t\t\t\"Name\" : \"Key Power\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 2], \n" \
"\t\t\t\t\"Mask\" : [ 1, 4], \n" \
"\t\t\t\t\"Id\"   : \"0x67\", \n" \
"\t\t\t\t\"Name\" : \"Keypad =\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x74\", \n" \
"\t\t\t\t\"Name\" : \"Key Execute\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x75\", \n" \
"\t\t\t\t\"Name\" : \"Key Help\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x76\", \n" \
"\t\t\t\t\"Name\" : \"Key Menu\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x77\", \n" \
"\t\t\t\t\"Name\" : \"Key Select\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x78\", \n" \
"\t\t\t\t\"Name\" : \"Key Stop\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x79\", \n" \
"\t\t\t\t\"Name\" : \"Key Again\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x7a\", \n" \
"\t\t\t\t\"Name\" : \"Key Undo\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x7b\", \n" \
"\t\t\t\t\"Name\" : \"Key Cut\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x7c\", \n" \
"\t\t\t\t\"Name\" : \"Key Copy\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x7d\", \n" \
"\t\t\t\t\"Name\" : \"Key Paste\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x7e\", \n" \
"\t\t\t\t\"Name\" : \"Key Find\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x7f\", \n" \
"\t\t\t\t\"Name\" : \"Key Mute\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x80\", \n" \
"\t\t\t\t\"Name\" : \"Key Volume Up\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x81\", \n" \
"\t\t\t\t\"Name\" : \"Key Volume Down\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x82\", \n" \
"\t\t\t\t\"Name\" : \"Key Locking Caps Lock\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x83\", \n" \
"\t\t\t\t\"Name\" : \"Key Locking Num Lock\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x84\", \n" \
"\t\t\t\t\"Name\" : \"Key Locking Scroll Lock\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 11, 0], \n" \
"\t\t\t\t\"Mask\" : [ 64, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x85\", \n" \
"\t\t\t\t\"Name\" : \"Keypad Comma\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x86\", \n" \
"\t\t\t\t\"Name\" : \"Keypad Equal Sign\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 3, 0], \n" \
"\t\t\t\t\"Mask\" : [ 32, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x87\", \n" \
"\t\t\t\t\"Name\" : \"Key International1\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 0], \n" \
"\t\t\t\t\"Mask\" : [ 16, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x88\", \n" \
"\t\t\t\t\"Name\" : \"Key International2(KANA)\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 2, 0], \n" \
"\t\t\t\t\"Mask\" : [ 16, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x89\", \n" \
"\t\t\t\t\"Name\" : \"Key International3\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 12, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x8a\", \n" \
"\t\t\t\t\"Name\" : \"Key International4\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 12, 0], \n" \
"\t\t\t\t\"Mask\" : [ 8, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x8b\", \n" \
"\t\t\t\t\"Name\" : \"Key International5\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 11, 0], \n" \
"\t\t\t\t\"Mask\" : [ 64, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x8c\", \n" \
"\t\t\t\t\"Name\" : \"Key International6 (JP108[，])\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x90\", \n" \
"\t\t\t\t\"Name\" : \"Key LANG1\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x91\", \n" \
"\t\t\t\t\"Name\" : \"Key LANG2\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x92\", \n" \
"\t\t\t\t\"Name\" : \"Key LANG3 (KATAKANA)\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x93\", \n" \
"\t\t\t\t\"Name\" : \"Key LANG4 (HIRAGANA)\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 8, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x94\", \n" \
"\t\t\t\t\"Name\" : \"Key LANG5(ZENKAKU/HANKAKU)\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 2], \n" \
"\t\t\t\t\"Mask\" : [ 1, 1], \n" \
"\t\t\t\t\"Id\"   : \"0xb6\", \n" \
"\t\t\t\t\"Name\" : \"Keypad (\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 2], \n" \
"\t\t\t\t\"Mask\" : [ 1, 2], \n" \
"\t\t\t\t\"Id\"   : \"0xb7\", \n" \
"\t\t\t\t\"Name\" : \"Keypad )\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0xe0\", \n" \
"\t\t\t\t\"Name\" : \"Key Left Control\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0xe1\", \n" \
"\t\t\t\t\"Name\" : \"Key Left Shift\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0xe2\", \n" \
"\t\t\t\t\"Name\" : \"Key Left Alt\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0xe3\", \n" \
"\t\t\t\t\"Name\" : \"Key Left GUI\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0xe4\", \n" \
"\t\t\t\t\"Name\" : \"Key Right Control\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0xe5\", \n" \
"\t\t\t\t\"Name\" : \"Key Right Shift\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0xe6\", \n" \
"\t\t\t\t\"Name\" : \"Key Right Alt\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0xe7\", \n" \
"\t\t\t\t\"Name\" : \"Key Right GUI\" \n" \
"\t\t\t} \n" \
"\t\t], \n" \
"\t\t\"ASCII\" : [ \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x00\", \n" \
"\t\t\t\t\"Name\" : \"NUL（null文字)\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x01\", \n" \
"\t\t\t\t\"Name\" : \"SOH（ヘッダ開始）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x02\", \n" \
"\t\t\t\t\"Name\" : \"STX（テキスト開始）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x03\", \n" \
"\t\t\t\t\"Name\" : \"ETX（テキスト終了）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x04\", \n" \
"\t\t\t\t\"Name\" : \"EOT（転送終了）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x05\", \n" \
"\t\t\t\t\"Name\" : \"ENQ（照会）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x06\", \n" \
"\t\t\t\t\"Name\" : \"ACK（受信確認）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x07\", \n" \
"\t\t\t\t\"Name\" : \"BEL（警告）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 8, 0], \n" \
"\t\t\t\t\"Mask\" : [ 32, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x08\", \n" \
"\t\t\t\t\"Name\" : \"BS（後退）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 8, 0], \n" \
"\t\t\t\t\"Mask\" : [ 8, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x09\", \n" \
"\t\t\t\t\"Name\" : \"HT（水平タブ）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 9, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x0a\", \n" \
"\t\t\t\t\"Name\" : \"LF（改行）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x0b\", \n" \
"\t\t\t\t\"Name\" : \"VT（垂直タブ）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x0c\", \n" \
"\t\t\t\t\"Name\" : \"FF（改頁）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 8, 0], \n" \
"\t\t\t\t\"Mask\" : [ 128, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x0d\", \n" \
"\t\t\t\t\"Name\" : \"CR（復帰）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x0e\", \n" \
"\t\t\t\t\"Name\" : \"SO（シフトアウト）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x0f\", \n" \
"\t\t\t\t\"Name\" : \"SI（シフトイン）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x10\", \n" \
"\t\t\t\t\"Name\" : \"DLE（データリンクエスケー プ）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x11\", \n" \
"\t\t\t\t\"Name\" : \"DC1（装置制御１）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x12\", \n" \
"\t\t\t\t\"Name\" : \"DC2（装置制御２）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x13\", \n" \
"\t\t\t\t\"Name\" : \"DC3（装置制御３）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x14\", \n" \
"\t\t\t\t\"Name\" : \"DC4（装置制御４）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x15\", \n" \
"\t\t\t\t\"Name\" : \"NAK（受信失敗）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x16\", \n" \
"\t\t\t\t\"Name\" : \"SYN（同期）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 8, 0], \n" \
"\t\t\t\t\"Mask\" : [ 16, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x17\", \n" \
"\t\t\t\t\"Name\" : \"ETB（転送ブロック終了）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x18\", \n" \
"\t\t\t\t\"Name\" : \"CAN（キャンセル）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x19\", \n" \
"\t\t\t\t\"Name\" : \"EM（メディア終了）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x1a\", \n" \
"\t\t\t\t\"Name\" : \"SUB（置換）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 8, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x1b\", \n" \
"\t\t\t\t\"Name\" : \"ESC（エスケープ）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x1c\", \n" \
"\t\t\t\t\"Name\" : \"FS（フォーム区切り）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x1d\", \n" \
"\t\t\t\t\"Name\" : \"GS（グループ区切り）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x1e\", \n" \
"\t\t\t\t\"Name\" : \"RS（レコード区切り）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 0, 0], \n" \
"\t\t\t\t\"Mask\" : [ 0, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x1f\", \n" \
"\t\t\t\t\"Name\" : \"US（ユニット区切り）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 9, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x20\", \n" \
"\t\t\t\t\"Name\" : \"SPC（空白文字）\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 1], \n" \
"\t\t\t\t\"Mask\" : [ 1, 2], \n" \
"\t\t\t\t\"Id\"   : \"0x21\", \n" \
"\t\t\t\t\"Name\" : \"!\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 1], \n" \
"\t\t\t\t\"Mask\" : [ 1, 4], \n" \
"\t\t\t\t\"Id\"   : \"0x22\", \n" \
"\t\t\t\t\"Name\" : \"”\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 1], \n" \
"\t\t\t\t\"Mask\" : [ 1, 8], \n" \
"\t\t\t\t\"Id\"   : \"0x23\", \n" \
"\t\t\t\t\"Name\" : \"#\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 1], \n" \
"\t\t\t\t\"Mask\" : [ 1, 16], \n" \
"\t\t\t\t\"Id\"   : \"0x24\", \n" \
"\t\t\t\t\"Name\" : \"$\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 1], \n" \
"\t\t\t\t\"Mask\" : [ 1, 32], \n" \
"\t\t\t\t\"Id\"   : \"0x25\", \n" \
"\t\t\t\t\"Name\" : \"%\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 1], \n" \
"\t\t\t\t\"Mask\" : [ 1, 64], \n" \
"\t\t\t\t\"Id\"   : \"0x26\", \n" \
"\t\t\t\t\"Name\" : \"&\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 1], \n" \
"\t\t\t\t\"Mask\" : [ 1, 128], \n" \
"\t\t\t\t\"Id\"   : \"0x27\", \n" \
"\t\t\t\t\"Name\" : \"'\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 2], \n" \
"\t\t\t\t\"Mask\" : [ 1, 1], \n" \
"\t\t\t\t\"Id\"   : \"0x28\", \n" \
"\t\t\t\t\"Name\" : \"(\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 2], \n" \
"\t\t\t\t\"Mask\" : [ 1, 2], \n" \
"\t\t\t\t\"Id\"   : \"0x29\", \n" \
"\t\t\t\t\"Name\" : \")\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 3], \n" \
"\t\t\t\t\"Mask\" : [ 1, 1], \n" \
"\t\t\t\t\"Id\"   : \"0x2a\", \n" \
"\t\t\t\t\"Name\" : \"*\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 2], \n" \
"\t\t\t\t\"Mask\" : [ 1, 128], \n" \
"\t\t\t\t\"Id\"   : \"0x2b\", \n" \
"\t\t\t\t\"Name\" : \"+\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 3, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x2c\", \n" \
"\t\t\t\t\"Name\" : \"，\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 2, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x2d\", \n" \
"\t\t\t\t\"Name\" : \"-\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 3, 0], \n" \
"\t\t\t\t\"Mask\" : [ 8, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x2e\", \n" \
"\t\t\t\t\"Name\" : \".\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 3, 0], \n" \
"\t\t\t\t\"Mask\" : [ 16, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x2f\", \n" \
"\t\t\t\t\"Name\" : \"/\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 1, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x30\", \n" \
"\t\t\t\t\"Name\" : \"0\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 1, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x31\", \n" \
"\t\t\t\t\"Name\" : \"1\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 1, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x32\", \n" \
"\t\t\t\t\"Name\" : \"2\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 1, 0], \n" \
"\t\t\t\t\"Mask\" : [ 8, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x33\", \n" \
"\t\t\t\t\"Name\" : \"3\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 1, 0], \n" \
"\t\t\t\t\"Mask\" : [ 16, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x34\", \n" \
"\t\t\t\t\"Name\" : \"4\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 1, 0], \n" \
"\t\t\t\t\"Mask\" : [ 32, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x35\", \n" \
"\t\t\t\t\"Name\" : \"5\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 1, 0], \n" \
"\t\t\t\t\"Mask\" : [ 64, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x36\", \n" \
"\t\t\t\t\"Name\" : \"6\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 1, 0], \n" \
"\t\t\t\t\"Mask\" : [ 128, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x37\", \n" \
"\t\t\t\t\"Name\" : \"7\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 2, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x38\", \n" \
"\t\t\t\t\"Name\" : \"8\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 2, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x39\", \n" \
"\t\t\t\t\"Name\" : \"9\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 3, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x3a\", \n" \
"\t\t\t\t\"Name\" : \":\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 2, 0], \n" \
"\t\t\t\t\"Mask\" : [ 128, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x3b\", \n" \
"\t\t\t\t\"Name\" : \";\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 3], \n" \
"\t\t\t\t\"Mask\" : [ 1, 4], \n" \
"\t\t\t\t\"Id\"   : \"0x3c\", \n" \
"\t\t\t\t\"Name\" : \"<\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 2], \n" \
"\t\t\t\t\"Mask\" : [ 1, 4], \n" \
"\t\t\t\t\"Id\"   : \"0x3d\", \n" \
"\t\t\t\t\"Name\" : \"=\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 3], \n" \
"\t\t\t\t\"Mask\" : [ 1, 8], \n" \
"\t\t\t\t\"Id\"   : \"0x3e\", \n" \
"\t\t\t\t\"Name\" : \">\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 3], \n" \
"\t\t\t\t\"Mask\" : [ 1, 16], \n" \
"\t\t\t\t\"Id\"   : \"0x3f\", \n" \
"\t\t\t\t\"Name\" : \"?\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 2, 0], \n" \
"\t\t\t\t\"Mask\" : [ 32, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x40\", \n" \
"\t\t\t\t\"Name\" : \"@\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 3], \n" \
"\t\t\t\t\"Mask\" : [ 1, 64], \n" \
"\t\t\t\t\"Id\"   : \"0x41\", \n" \
"\t\t\t\t\"Name\" : \"A\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 3], \n" \
"\t\t\t\t\"Mask\" : [ 1, 128], \n" \
"\t\t\t\t\"Id\"   : \"0x42\", \n" \
"\t\t\t\t\"Name\" : \"B\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 4], \n" \
"\t\t\t\t\"Mask\" : [ 1, 1], \n" \
"\t\t\t\t\"Id\"   : \"0x43\", \n" \
"\t\t\t\t\"Name\" : \"C\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 4], \n" \
"\t\t\t\t\"Mask\" : [ 1, 2], \n" \
"\t\t\t\t\"Id\"   : \"0x44\", \n" \
"\t\t\t\t\"Name\" : \"D\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 4], \n" \
"\t\t\t\t\"Mask\" : [ 1, 4], \n" \
"\t\t\t\t\"Id\"   : \"0x45\", \n" \
"\t\t\t\t\"Name\" : \"E\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 4], \n" \
"\t\t\t\t\"Mask\" : [ 1, 8], \n" \
"\t\t\t\t\"Id\"   : \"0x46\", \n" \
"\t\t\t\t\"Name\" : \"F\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 4], \n" \
"\t\t\t\t\"Mask\" : [ 1, 16], \n" \
"\t\t\t\t\"Id\"   : \"0x47\", \n" \
"\t\t\t\t\"Name\" : \"G\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 4], \n" \
"\t\t\t\t\"Mask\" : [ 1, 32], \n" \
"\t\t\t\t\"Id\"   : \"0x48\", \n" \
"\t\t\t\t\"Name\" : \"H\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 4], \n" \
"\t\t\t\t\"Mask\" : [ 1, 64], \n" \
"\t\t\t\t\"Id\"   : \"0x49\", \n" \
"\t\t\t\t\"Name\" : \"I\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 4], \n" \
"\t\t\t\t\"Mask\" : [ 1, 128], \n" \
"\t\t\t\t\"Id\"   : \"0x4a\", \n" \
"\t\t\t\t\"Name\" : \"J\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 5], \n" \
"\t\t\t\t\"Mask\" : [ 1, 1], \n" \
"\t\t\t\t\"Id\"   : \"0x4b\", \n" \
"\t\t\t\t\"Name\" : \"K\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 5], \n" \
"\t\t\t\t\"Mask\" : [ 1, 2], \n" \
"\t\t\t\t\"Id\"   : \"0x4c\", \n" \
"\t\t\t\t\"Name\" : \"L\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 5], \n" \
"\t\t\t\t\"Mask\" : [ 1, 4], \n" \
"\t\t\t\t\"Id\"   : \"0x4d\", \n" \
"\t\t\t\t\"Name\" : \"M\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 5], \n" \
"\t\t\t\t\"Mask\" : [ 1, 8], \n" \
"\t\t\t\t\"Id\"   : \"0x4e\", \n" \
"\t\t\t\t\"Name\" : \"N\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 5], \n" \
"\t\t\t\t\"Mask\" : [ 1, 16], \n" \
"\t\t\t\t\"Id\"   : \"0x4f\", \n" \
"\t\t\t\t\"Name\" : \"O\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 5], \n" \
"\t\t\t\t\"Mask\" : [ 1, 32], \n" \
"\t\t\t\t\"Id\"   : \"0x50\", \n" \
"\t\t\t\t\"Name\" : \"P\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 5], \n" \
"\t\t\t\t\"Mask\" : [ 1, 64], \n" \
"\t\t\t\t\"Id\"   : \"0x51\", \n" \
"\t\t\t\t\"Name\" : \"Q\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 5], \n" \
"\t\t\t\t\"Mask\" : [ 1, 128], \n" \
"\t\t\t\t\"Id\"   : \"0x52\", \n" \
"\t\t\t\t\"Name\" : \"R\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 6], \n" \
"\t\t\t\t\"Mask\" : [ 1, 1], \n" \
"\t\t\t\t\"Id\"   : \"0x53\", \n" \
"\t\t\t\t\"Name\" : \"S\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 6], \n" \
"\t\t\t\t\"Mask\" : [ 1, 2], \n" \
"\t\t\t\t\"Id\"   : \"0x54\", \n" \
"\t\t\t\t\"Name\" : \"T\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 6], \n" \
"\t\t\t\t\"Mask\" : [ 1, 4], \n" \
"\t\t\t\t\"Id\"   : \"0x55\", \n" \
"\t\t\t\t\"Name\" : \"U\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 6], \n" \
"\t\t\t\t\"Mask\" : [ 1, 8], \n" \
"\t\t\t\t\"Id\"   : \"0x56\", \n" \
"\t\t\t\t\"Name\" : \"V\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 6], \n" \
"\t\t\t\t\"Mask\" : [ 1, 16], \n" \
"\t\t\t\t\"Id\"   : \"0x57\", \n" \
"\t\t\t\t\"Name\" : \"W\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 6], \n" \
"\t\t\t\t\"Mask\" : [ 1, 32], \n" \
"\t\t\t\t\"Id\"   : \"0x58\", \n" \
"\t\t\t\t\"Name\" : \"X\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 6], \n" \
"\t\t\t\t\"Mask\" : [ 1, 64], \n" \
"\t\t\t\t\"Id\"   : \"0x59\", \n" \
"\t\t\t\t\"Name\" : \"Y\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 6], \n" \
"\t\t\t\t\"Mask\" : [ 1, 128], \n" \
"\t\t\t\t\"Id\"   : \"0x5a\", \n" \
"\t\t\t\t\"Name\" : \"Z\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 2, 0], \n" \
"\t\t\t\t\"Mask\" : [ 64, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x5b\", \n" \
"\t\t\t\t\"Name\" : \"[\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 2, 0], \n" \
"\t\t\t\t\"Mask\" : [ 16, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x5c\", \n" \
"\t\t\t\t\"Name\" : \"￥\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 3, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x5d\", \n" \
"\t\t\t\t\"Name\" : \"]\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 2, 0], \n" \
"\t\t\t\t\"Mask\" : [ 8, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x5e\", \n" \
"\t\t\t\t\"Name\" : \"^\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 3], \n" \
"\t\t\t\t\"Mask\" : [ 1, 32], \n" \
"\t\t\t\t\"Id\"   : \"0x5f\", \n" \
"\t\t\t\t\"Name\" : \"_\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 2], \n" \
"\t\t\t\t\"Mask\" : [ 1, 32], \n" \
"\t\t\t\t\"Id\"   : \"0x60\", \n" \
"\t\t\t\t\"Name\" : \"`\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 3, 0], \n" \
"\t\t\t\t\"Mask\" : [ 64, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x61\", \n" \
"\t\t\t\t\"Name\" : \"a\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 3, 0], \n" \
"\t\t\t\t\"Mask\" : [ 128, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x62\", \n" \
"\t\t\t\t\"Name\" : \"b\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 4, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x63\", \n" \
"\t\t\t\t\"Name\" : \"c\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 4, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x64\", \n" \
"\t\t\t\t\"Name\" : \"d\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 4, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x65\", \n" \
"\t\t\t\t\"Name\" : \"e\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 4, 0], \n" \
"\t\t\t\t\"Mask\" : [ 8, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x66\", \n" \
"\t\t\t\t\"Name\" : \"f\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 4, 0], \n" \
"\t\t\t\t\"Mask\" : [ 16, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x67\", \n" \
"\t\t\t\t\"Name\" : \"g\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 4, 0], \n" \
"\t\t\t\t\"Mask\" : [ 32, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x68\", \n" \
"\t\t\t\t\"Name\" : \"h\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 4, 0], \n" \
"\t\t\t\t\"Mask\" : [ 64, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x69\", \n" \
"\t\t\t\t\"Name\" : \"i\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 4, 0], \n" \
"\t\t\t\t\"Mask\" : [ 128, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x6a\", \n" \
"\t\t\t\t\"Name\" : \"j\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 5, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x6b\", \n" \
"\t\t\t\t\"Name\" : \"k\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 5, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x6c\", \n" \
"\t\t\t\t\"Name\" : \"l\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 5, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x6d\", \n" \
"\t\t\t\t\"Name\" : \"m\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 5, 0], \n" \
"\t\t\t\t\"Mask\" : [ 8, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x6e\", \n" \
"\t\t\t\t\"Name\" : \"n\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 5, 0], \n" \
"\t\t\t\t\"Mask\" : [ 16, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x6f\", \n" \
"\t\t\t\t\"Name\" : \"o\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 5, 0], \n" \
"\t\t\t\t\"Mask\" : [ 32, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x70\", \n" \
"\t\t\t\t\"Name\" : \"p\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 5, 0], \n" \
"\t\t\t\t\"Mask\" : [ 64, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x71\", \n" \
"\t\t\t\t\"Name\" : \"q\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 5, 0], \n" \
"\t\t\t\t\"Mask\" : [ 128, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x72\", \n" \
"\t\t\t\t\"Name\" : \"r\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 6, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x73\", \n" \
"\t\t\t\t\"Name\" : \"s\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 6, 0], \n" \
"\t\t\t\t\"Mask\" : [ 2, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x74\", \n" \
"\t\t\t\t\"Name\" : \"t\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 6, 0], \n" \
"\t\t\t\t\"Mask\" : [ 4, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x75\", \n" \
"\t\t\t\t\"Name\" : \"u\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 6, 0], \n" \
"\t\t\t\t\"Mask\" : [ 8, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x76\", \n" \
"\t\t\t\t\"Name\" : \"v\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 6, 0], \n" \
"\t\t\t\t\"Mask\" : [ 16, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x77\", \n" \
"\t\t\t\t\"Name\" : \"w\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 6, 0], \n" \
"\t\t\t\t\"Mask\" : [ 32, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x78\", \n" \
"\t\t\t\t\"Name\" : \"x\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 6, 0], \n" \
"\t\t\t\t\"Mask\" : [ 64, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x79\", \n" \
"\t\t\t\t\"Name\" : \"y\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 6, 0], \n" \
"\t\t\t\t\"Mask\" : [ 128, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x7a\", \n" \
"\t\t\t\t\"Name\" : \"z\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 2], \n" \
"\t\t\t\t\"Mask\" : [ 1, 64], \n" \
"\t\t\t\t\"Id\"   : \"0x7b\", \n" \
"\t\t\t\t\"Name\" : \"{\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 2], \n" \
"\t\t\t\t\"Mask\" : [ 1, 16], \n" \
"\t\t\t\t\"Id\"   : \"0x7c\", \n" \
"\t\t\t\t\"Name\" : \"|\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 3], \n" \
"\t\t\t\t\"Mask\" : [ 1, 2], \n" \
"\t\t\t\t\"Id\"   : \"0x7d\", \n" \
"\t\t\t\t\"Name\" : \"}\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 2], \n" \
"\t\t\t\t\"Mask\" : [ 1, 8], \n" \
"\t\t\t\t\"Id\"   : \"0x7e\", \n" \
"\t\t\t\t\"Name\" : \"~\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 9, 0], \n" \
"\t\t\t\t\"Mask\" : [ 8, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x7f\", \n" \
"\t\t\t\t\"Name\" : \"DEL（削除）\" \n" \
"\t\t\t} \n" \
"\t\t], \n" \
"\t\t\"Shift\" : [ \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 2, 0], \n" \
"\t\t\t\t\"Mask\" : [ 32, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x1f\", \n" \
"\t\t\t\t\"Name\" : \"Key 2 and @\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 2, 0], \n" \
"\t\t\t\t\"Mask\" : [ 8, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x23\", \n" \
"\t\t\t\t\"Name\" : \"Key 6 and ^\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 1], \n" \
"\t\t\t\t\"Mask\" : [ 1, 64], \n" \
"\t\t\t\t\"Id\"   : \"0x24\", \n" \
"\t\t\t\t\"Name\" : \"Key 7 and [and]\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 3], \n" \
"\t\t\t\t\"Mask\" : [ 1, 1], \n" \
"\t\t\t\t\"Id\"   : \"0x25\", \n" \
"\t\t\t\t\"Name\" : \"Key 8 and *\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 2], \n" \
"\t\t\t\t\"Mask\" : [ 1, 1], \n" \
"\t\t\t\t\"Id\"   : \"0x26\", \n" \
"\t\t\t\t\"Name\" : \"Key 9 and (\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 2], \n" \
"\t\t\t\t\"Mask\" : [ 1, 2], \n" \
"\t\t\t\t\"Id\"   : \"0x27\", \n" \
"\t\t\t\t\"Name\" : \"Key 0 and )\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 3], \n" \
"\t\t\t\t\"Mask\" : [ 1, 32], \n" \
"\t\t\t\t\"Id\"   : \"0x2d\", \n" \
"\t\t\t\t\"Name\" : \"Key - and _\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 2], \n" \
"\t\t\t\t\"Mask\" : [ 1, 128], \n" \
"\t\t\t\t\"Id\"   : \"0x2e\", \n" \
"\t\t\t\t\"Name\" : \"Key = and +\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 3, 0], \n" \
"\t\t\t\t\"Mask\" : [ 1, 0], \n" \
"\t\t\t\t\"Id\"   : \"0x33\", \n" \
"\t\t\t\t\"Name\" : \"Key ; and :\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 1], \n" \
"\t\t\t\t\"Mask\" : [ 1, 4], \n" \
"\t\t\t\t\"Id\"   : \"0x34\", \n" \
"\t\t\t\t\"Name\" : \"Key ’ and ”\" \n" \
"\t\t\t} , \n" \
"\t\t\t{ \n" \
"\t\t\t\t\"Y\"    : [ 7, 2], \n" \
"\t\t\t\t\"Mask\" : [ 1, 8], \n" \
"\t\t\t\t\"Id\"   : \"0x35\", \n" \
"\t\t\t\t\"Name\" : \"Key ZENKAKU\" \n" \
"\t\t\t} \n" \
"\t\t] \n" \
"\t} \n" \
"}\n" \
