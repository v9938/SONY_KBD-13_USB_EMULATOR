#define JSON_GPAD_DEF4 \
"{ \n" \
"	\"#comment\" : [ \n" \
"\t\t\"ファイルサイズは32KByte以下にしてください,\", \n" \
"\t\t\"ROOT名  [Modifier],[Usage],[ASCII]のいずれか\", \n" \
"\t\t\"  書式  [Id]   GamePADのIDコード\", \n" \
"\t\t\"        [Value]KEYONになる閾値(-1〜1)\", \n" \
"\t\t\"        [Y]    Matrixの番号0-16 17=PAUSE,18=RESET [0x00は割り当て無し]\", \n" \
"\t\t\"        [Mask] 当該Matrixのマスク値、当該BITを1にするとON\", \n" \
"\t\t\"        [Name] キー名（未使用)\", \n" \
"\t\t\"(YとMASKは一つのキーに2つまで持てる)\" \n" \
"	], \n" \
"	\"Keymap\" : { \n" \
"		\"GamePad\" : [ \n" \
"			{ \n" \
"				\"Y\"    : [ 9, 0], \n" \
"				\"Mask\" : [ 1, 0], \n" \
"				\"Id\"   : \"Button0\", \n" \
"				\"Name\" : \"Key SPACE\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 7, 0], \n" \
"				\"Mask\" : [ 4, 0], \n" \
"				\"Id\"   : \"Button1\", \n" \
"				\"Name\" : \"Key GRAPH\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 1, 0], \n" \
"				\"Mask\" : [ 4, 0], \n" \
"				\"Id\"   : \"Button2\", \n" \
"				\"Name\" : \"Key 2 and @\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 1, 0], \n" \
"				\"Mask\" : [ 8, 0], \n" \
"				\"Id\"   : \"Button3\", \n" \
"				\"Name\" : \"Key 3 and #\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 1, 0], \n" \
"				\"Mask\" : [ 16, 0], \n" \
"				\"Id\"   : \"Button4\", \n" \
"				\"Name\" : \"Key 4 and $\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 1, 0], \n" \
"				\"Mask\" : [ 32, 0], \n" \
"				\"Id\"   : \"Button5\", \n" \
"				\"Name\" : \"Key 5 and %\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 1, 0], \n" \
"				\"Mask\" : [ 64, 0], \n" \
"				\"Id\"   : \"Button6\", \n" \
"				\"Name\" : \"Key 6 and ^\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 1, 0], \n" \
"				\"Mask\" : [ 128, 0], \n" \
"				\"Id\"   : \"Button7\", \n" \
"				\"Name\" : \"Key 7 and [and]\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 2, 0], \n" \
"				\"Mask\" : [ 1, 0], \n" \
"				\"Id\"   : \"Button8\", \n" \
"				\"Name\" : \"Key 8 and *\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 2, 0], \n" \
"				\"Mask\" : [ 2, 0], \n" \
"				\"Id\"   : \"Button9\", \n" \
"				\"Name\" : \"Key 9 and (\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 1, 0], \n" \
"				\"Mask\" : [ 1, 0], \n" \
"				\"Id\"   : \"Button10\", \n" \
"				\"Name\" : \"Key 0 and )\" \n" \
"			} , \n" \
"						{ \n" \
"				\"Y\"    : [ 1, 0], \n" \
"				\"Mask\" : [ 2, 0], \n" \
"				\"Id\"   : \"Button11\", \n" \
"				\"Name\" : \"Key 1 and !\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 1, 0], \n" \
"				\"Mask\" : [ 4, 0], \n" \
"				\"Id\"   : \"Button12\", \n" \
"				\"Name\" : \"Key 2 and @\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 9, 0], \n" \
"				\"Mask\" : [ 128, 0], \n" \
"				\"Id\"   : \"HatSwitch_R\", \n" \
"				\"Name\" : \"Key Right Arrow\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 9, 0], \n" \
"				\"Mask\" : [ 16, 0], \n" \
"				\"Id\"   : \"HatSwitch_L\", \n" \
"				\"Name\" : \"Key Left Arrow\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 9, 0], \n" \
"				\"Mask\" : [ 64, 0], \n" \
"				\"Id\"   : \"HatSwitch_D\", \n" \
"				\"Name\" : \"Key Down Arrow\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 9, 0], \n" \
"				\"Mask\" : [ 32, 0], \n" \
"				\"Id\"   : \"HatSwitch_U\", \n" \
"				\"Name\" : \"Key Up Arrow\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 11, 0], \n" \
"				\"Mask\" : [ 2, 0], \n" \
"				\"Id\"   : \"Axis0\", \n" \
"				\"Value\": 0.6, \n" \
"				\"Name\" : \"Keypad 6 and Right Arrow\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 10, 0], \n" \
"				\"Mask\" : [ 128, 0], \n" \
"				\"Id\"   : \"Axis0\", \n" \
"				\"Value\": -0.6, \n" \
"				\"Name\" : \"Keypad 4 and Left Arrow\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 10, 0], \n" \
"				\"Mask\" : [ 32, 0], \n" \
"				\"Id\"   : \"Axis1\", \n" \
"				\"Value\": 0.6, \n" \
"				\"Name\" : \"Keypad 2 and Down Arrow\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 11, 0], \n" \
"				\"Mask\" : [ 8, 0], \n" \
"				\"Id\"   : \"Axis1\", \n" \
"				\"Value\": -0.6, \n" \
"				\"Name\" : \"Keypad 8 and Up Arrow\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 9, 0], \n" \
"				\"Mask\" : [ 128, 0], \n" \
"				\"Id\"   : \"Axis2\", \n" \
"				\"Value\": 0.6, \n" \
"				\"Name\" : \"Key Right Arrow\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 9, 0], \n" \
"				\"Mask\" : [ 16, 0], \n" \
"				\"Id\"   : \"Axis2\", \n" \
"				\"Value\": -0.6, \n" \
"				\"Name\" : \"Key Left Arrow\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 9, 0], \n" \
"				\"Mask\" : [ 32, 0], \n" \
"				\"Id\"   : \"Axis3\", \n" \
"				\"Value\": -0.6, \n" \
"				\"Name\" : \"Key Up Arrow\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 9, 0], \n" \
"				\"Mask\" : [ 64, 0], \n" \
"				\"Id\"   : \"Axis3\", \n" \
"				\"Value\": 0.6, \n" \
"				\"Name\" : \"Key Down Arrow\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 6, 0], \n" \
"				\"Mask\" : [ 128, 0], \n" \
"				\"Id\"   : \"Axis4\", \n" \
"				\"Value\": 0.6, \n" \
"				\"Name\" : \"Key Z\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 6, 0], \n" \
"				\"Mask\" : [ 32, 0], \n" \
"				\"Id\"   : \"Axis4\", \n" \
"				\"Value\": -0.6, \n" \
"				\"Name\" : \"Key X\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 4, 0], \n" \
"				\"Mask\" : [ 1, 0], \n" \
"				\"Id\"   : \"Axis5\", \n" \
"				\"Value\": 0.6, \n" \
"				\"Name\" : \"Key C\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 6, 0], \n" \
"				\"Mask\" : [ 8, 0], \n" \
"				\"Id\"   : \"Axis5\", \n" \
"				\"Value\": -0.6, \n" \
"				\"Name\" : \"Key V\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 0, 0], \n" \
"				\"Mask\" : [ 0, 0], \n" \
"				\"Id\"   : \"Axis6\", \n" \
"				\"Value\": 0.6, \n" \
"				\"Name\" : \"None\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 0, 0], \n" \
"				\"Mask\" : [ 0, 0], \n" \
"				\"Id\"   : \"Axis6\", \n" \
"				\"Value\": -0.6, \n" \
"				\"Name\" : \"None\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 0, 0], \n" \
"				\"Mask\" : [ 0, 0], \n" \
"				\"Id\"   : \"Axis7\", \n" \
"				\"Value\": 0.6, \n" \
"				\"Name\" : \"None\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 0, 0], \n" \
"				\"Mask\" : [ 0, 0], \n" \
"				\"Id\"   : \"Axis7\", \n" \
"				\"Value\": -0.6, \n" \
"				\"Name\" : \"None\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 0, 0], \n" \
"				\"Mask\" : [ 0, 0], \n" \
"				\"Id\"   : \"Axis8\", \n" \
"				\"Value\": 0.6, \n" \
"				\"Name\" : \"None\" \n" \
"			} , \n" \
"			{ \n" \
"				\"Y\"    : [ 0, 0], \n" \
"				\"Mask\" : [ 0, 0], \n" \
"				\"Id\"   : \"Axis8\", \n" \
"				\"Value\": -0.6, \n" \
"				\"Name\" : \"None\" \n" \
"			} ,\n" \
"		] \n" \
"	} \n" \
"}\n" \
"\n" \

