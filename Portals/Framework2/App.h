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

#ifndef _APP_H_
#define _APP_H_

#include "Renderer.h"
#include "PixelFormat.h"
#include "Util/Font.h"
#include "Menu.h"
#include "DisplayMode.h"
#include "Util/String.h"
#include "Util/LinkedList.h"

#include <stdio.h>

#ifdef LINUX

#include <stdlib.h>
#include <string.h>

#endif

class App {
public:
	App();
	virtual ~App();

	void initDisplayModes();
	bool setDisplayMode(int width, int height, int refreshRate);
	bool resetDisplayMode();

	MenuSystem *getMenuSystem() const { return menuSystem; }
	virtual void initMenu();
	virtual bool processMenu();

	virtual bool init(){ return true; }
	virtual bool exit(){ return true; }

#if defined(_WIN32)
	void setHwnd(HWND hWnd){ hwnd = hWnd; }
#elif defined(LINUX)
	void setDisplay(Display *disp, int scr){ display = disp; screen = scr; }
#endif
	void showCursor(bool val);
	void closeWindow();
	void toggleScreenMode();

	virtual bool initAPI() = 0;
	void initPixelFormat(PixelFormat &pf);
	virtual void selectPixelFormat(PixelFormat &pf){};
	virtual bool closeAPI() = 0;

	virtual bool load(){ return true; }
	virtual bool unload(){ return true; }

	virtual void beginFrame() = 0;
	virtual bool drawFrame() = 0;
	virtual void endFrame() = 0;

	virtual void setViewport(unsigned int w, unsigned int h) = 0;

	virtual void resetCamera(){
		position = vec3(0, 0, 0);
		wx = wy = wz = 0;
	}
	virtual void controls();
	virtual void processMovement(const vec3 &newPosition);
	virtual void rotateView(const float Wx, const float Wy){
		wx += Wx;
		wy += Wy;
	}
	void processKey(unsigned int key);
	void processChar(char ch);
	virtual void setKey(unsigned int key, bool value){
		keys[key & 0xffff] = value;
	}
	bool getKey(unsigned int key) const { return keys[key & 0xffff]; }
	virtual void setButton(unsigned int button, bool value){
		if (button == MOUSE_LEFT) lButton = value; else rButton = value;
	}
	void setMousePos(int x, int y, int z){
		mouseX = x;
		mouseY = y;
		mouseZ = z;
	}

	virtual bool processConsole(String &results);

	float getFps();
	virtual void drawText(char *str, float x, float y, float charWidth, float charHeight, float r = 1.0f, float g = 1.0f, float b = 1.0f) = 0;
	virtual void drawGUI();

	virtual bool getScreenshot(Image &img) = 0;
	void snapScreenshot();

	bool setDefaultFont(const char *fontFile, const char *textureFile);
protected:
	Renderer *renderer;

	float speed;
	vec3 position;
	float wx, wy, wz;

	mat4 modelView, projection;

	TextureFont defaultFont;
	TextureID textTexture;

	MenuSystem *menuSystem;
	Menu *configMenu, *modesMenu, *controlsMenu;

	String console;
	unsigned int cursorPos;
	LinkedList <String> consoleHistory;

	bool keys[65536];
	int mouseX, mouseY, mouseZ;
	bool lButton, rButton;

	bool cursorVisible;
#if defined(_WIN32)
	HWND hwnd;
#elif defined(LINUX)
	Display *display;
	int screen;
#endif
	DisplayModeHandler *modes;

#ifndef NO_BENCH

	struct DemoNode {
		vec3 pos;
		float wx, wy, wz;
	};

	// Benchmarking stuff
	bool demoRecMode;
	bool demoPlayMode;
	FILE *demoFile;
	DemoNode *demoArray;
	unsigned int demoSize;
	unsigned int demoFrameCounter;

	bool beginDemo(char *fileName);
	void recordDemo();
	void stopDemo();
	bool loadDemo(char *fileName);
#endif
};

#endif // _APP_H_
