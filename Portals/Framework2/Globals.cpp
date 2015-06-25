/***********      .---.         .-"-.      *******************\
* -------- *     /   ._.       / ´ ` \     * ---------------- *
* Author's *     \_  (__\      \_°v°_/     * humus@rogers.com *
*   note   *     //   \\       //   \\     * ICQ #47010716    *
* -------- *    ((     ))     ((     ))    * ---------------- *
*          ****--""---""-------""---""--****                  ********\
* This file is a part of the work done by Humus. You are free to use  *
* the code in any way you like, modified, unmodified or copy'n'pasted *
* into your own work. However, I expect you to respect these points:  *
*  @ If you use this file and its contents unmodified, or use a major *
*    part of this file, please credit the author and leave this note. *
*  @ For use in anything commercial, please request my approval.      *
*  @ Share your work and ideas too as much as you can.                *
\*********************************************************************/

#include "Globals.h"
#include "Util/Config.h"

bool captureMouse;
bool invertMouse;
bool fullscreen;
bool toggleFullscreen;
bool showFPS, showMenu, showConsole;

int mouseSpeed;
int middleX, middleY;
int width, height;

int windowedLeft, windowedRight, windowedTop, windowedBottom;
int fullscreenWidth, fullscreenHeight, refreshRate, fsaaLevel;

int64 cpuHz;
float time, frameTime;



unsigned int leftKey, rightKey, upKey, downKey, forwardKey, backwardKey, resetKey, showFpsKey, menuKey, screenshotKey, consoleKey, demoModeKey;

Config config;

void initGlobals(){
	config.init();

	showMenu = false;
	showConsole = false;

	mouseSpeed  = config.getIntegerDef("MouseSpeed", 5);
	invertMouse = config.getBoolDef("InvertMouse", true);
	showFPS     = config.getBoolDef("ShowFPS", true);

	windowedLeft   = config.getIntegerDef("WindowedLeft",   0);
	windowedRight  = config.getIntegerDef("WindowedRight",  800);
	windowedTop    = config.getIntegerDef("WindowedTop",    0);
	windowedBottom = config.getIntegerDef("WindowedBottom", 600);

	fullscreen       = config.getBoolDef("Fullscreen", false);
	fullscreenWidth  = config.getIntegerDef("Width", 1024);
	fullscreenHeight = config.getIntegerDef("Height", 768);
	refreshRate      = config.getIntegerDef("RefreshRate", 85);

	fsaaLevel        = config.getIntegerDef("FSAALevel", 4);

	leftKey       = KEY_LEFT;
	rightKey      = KEY_RIGHT;
	upKey         = KEY_CTRL;
	downKey       = KEY_SHIFT;
	forwardKey    = KEY_UP;
	backwardKey   = KEY_DOWN;
	resetKey      = KEY_ENTER;
	screenshotKey = KEY_F9;
	showFpsKey    = KEY_SPACE;
	consoleKey    = KEY_TAB;
	demoModeKey   = 'F';
	menuKey       = KEY_F1;
}

void flushGlobals(){
	config.setInteger("MouseSpeed", mouseSpeed);
	config.setBool("InvertMouse", invertMouse);
	config.setBool("ShowFPS", showFPS);

	config.setInteger("WindowedLeft",   windowedLeft);
	config.setInteger("WindowedRight",  windowedRight);
	config.setInteger("WindowedTop",    windowedTop);
	config.setInteger("WindowedBottom", windowedBottom);

	config.setBool("Fullscreen", fullscreen);
	config.setInteger("Width",   fullscreenWidth);
	config.setInteger("Height",  fullscreenHeight);
	config.setInteger("RefreshRate", refreshRate);

	config.flush();
}


#ifdef LINUX
Window window;
Cursor blankCursor;
bool done;
#endif



struct KeyDef {
	unsigned int charCode;
	char *string;
};

#if defined(_WIN32)
#define CH(x) x
#elif defined(LINUX)
#define CH(x) (x + 32)
#endif


static const KeyDef keyDefs[] = {
	{ KEY_TAB,       "Tab"   },
	{ KEY_SHIFT,     "Shift" },
	{ KEY_CTRL,      "Ctrl"  },
	{ KEY_ENTER,     "Enter" },
	{ KEY_BACKSPACE, "Backspace" },
	{ KEY_SPACE,     "Space"  },
	{ KEY_HOME,      "Home"   },
	{ KEY_END,       "End"    },
	{ KEY_INSERT,    "Insert" },
	{ KEY_DELETE,    "Delete" },
	{ KEY_LEFT,      "Left Arrow"  },
	{ KEY_RIGHT,     "Right Arrow" },
	{ KEY_UP,        "Up Arrow"    },
	{ KEY_DOWN,      "Down Arrow"  },


	{'0', "0"}, {'1', "1"}, {'2', "2"}, {'3', "3"}, {'4', "4"}, {'5', "5"}, {'6', "6"}, {'7', "7"}, {'8', "8"}, {'9', "9"},
	{CH('A'), "A"}, {CH('B'), "B"}, {CH('C'), "C"}, {CH('D'), "D"}, {CH('E'), "E"}, {CH('F'), "F"}, {CH('G'), "G"}, {CH('H'), "H"}, {CH('I'), "I"},
	{CH('J'), "J"}, {CH('K'), "K"}, {CH('L'), "L"}, {CH('M'), "M"}, {CH('N'), "N"}, {CH('O'), "O"}, {CH('P'), "P"}, {CH('Q'), "Q"}, {CH('R'), "R"},
	{CH('S'), "S"}, {CH('T'), "T"}, {CH('U'), "U"}, {CH('V'), "V"}, {CH('W'), "W"}, {CH('X'), "X"}, {CH('Y'), "Y"}, {CH('Z'), "Z"},

	{ KEY_NUMPAD0,  "Numpad 0" }, { KEY_NUMPAD1,  "Numpad 1" }, { KEY_NUMPAD2,  "Numpad 2" },
	{ KEY_NUMPAD3,  "Numpad 3" }, { KEY_NUMPAD4,  "Numpad 4" }, { KEY_NUMPAD5,  "Numpad 5" },
	{ KEY_NUMPAD6,  "Numpad 6" }, { KEY_NUMPAD7,  "Numpad 7" }, { KEY_NUMPAD8,  "Numpad 8" },
	{ KEY_NUMPAD9,  "Numpad 9" },

	{ KEY_ADD,       "+" },
	{ KEY_SUBTRACT,  "-" },
	{ KEY_MULTIPLY,  "*" },
	{ KEY_DIVIDE,    "/" },
	{ KEY_SEPARATOR, "Separator" },
	{ KEY_DECIMAL,   "Decimal" },

	{ KEY_F1,  "F1" }, { KEY_F2,  "F2" }, { KEY_F3,  "F3" }, { KEY_F4,  "F4"  }, { KEY_F5,  "F5"  }, { KEY_F6,  "F6"  },
	{ KEY_F7,  "F7" }, { KEY_F8,  "F8" }, { KEY_F9,  "F9" }, { KEY_F10, "F10" }, { KEY_F11, "F11" }, { KEY_F12, "F12" },

//	KEY_PAUSE,   "Pause",
//	KEY_CAPITAL, "Caps Lock",
//	KEY_NUMLOCK, "Num Lock",
//	KEY_SCROLL,  "Scroll Lock",
};

char *getKeyString(unsigned int charCode){
	for (unsigned int i = 0; i < sizeof(keyDefs) / sizeof(KeyDef); i++){
		if (keyDefs[i].charCode == charCode) return keyDefs[i].string;
	}
	return NULL;
}

unsigned int getKeyCharCode(char *string){
	for (unsigned int i = 0; i < sizeof(keyDefs) / sizeof(KeyDef); i++){
		if (strcmp(keyDefs[i].string, string) == 0) return keyDefs[i].charCode;
	}
	return 0;
}



Set <char *> logLines;

void addToLog(const char *text){
	while (*text != '\0'){
		int index = strcspn(text, "\n\r");

		char *str = new char[index + 1];
		strncpy(str, text, index);
		str[index] = '\0';
		
		logLines.add(str);

		text += index;
		while (*text == '\n' || *text == '\r') text++;
	}
}

void clearLog(){
	for (unsigned int i = 0; i < logLines.getCount(); i++){
		delete logLines[i];
	}
	logLines.clear();
}

char *getLogString(){
	unsigned int i, length = 0;
	for (i = 0; i < logLines.getCount(); i++){
		length += strlen(logLines[i]);				
	}
	length += logLines.getCount();
	char *str = new char[length + 1];
	str[length] = 0;
	char *dest = str;
	for (i = 0; i < logLines.getCount(); i++){
		length = strlen(logLines[i]);
		strncpy(dest, logLines[i], length);
		dest += length;
		*dest++ = '\n';
	}
	return str;
}

