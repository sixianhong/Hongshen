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

#include "App.h"
#include "Util/Tokenizer.h"

#ifndef min
#define min(x,y) (x < y)? x : y;
#endif

App::App(){
	memset(keys, 0, sizeof(keys));
	lButton = false;
	rButton = false;

	cursorVisible = true;

	speed = 1024;
	textTexture = TEXTURE_NONE;

	menuSystem = new MenuSystem();
	cursorPos = 0;

#ifndef NO_BENCH
	demoRecMode = false;
	demoPlayMode = false;
	demoArray = NULL;
#endif
}

App::~App(){
#ifndef NO_BENCH
	if (demoRecMode) stopDemo();
	delete demoArray;
	if (demoPlayMode) fclose(demoFile);
#endif
	delete menuSystem;
	delete modes;
}

void App::initDisplayModes(){
	modes = new DisplayModeHandler(
#ifdef LINUX
		display, screen
#endif
	);
	modes->filterModes(640, 480, 32);
	modes->filterRefreshes(85);
}

bool App::setDisplayMode(int width, int height, int refreshRate){
	return modes->setDisplayMode(width, height, 32, refreshRate);
}

bool App::resetDisplayMode(){
	return modes->resetDisplayMode();
}

void App::initMenu(){
	Menu *menu = menuSystem->getMainMenu();
	menu->addMenuItem("Toggle Fullscreen");

	configMenu = new Menu();
	modesMenu = new Menu();
	for (unsigned int i = 0; i < modes->getNumberOfDisplayModes(); i++){
		int w = modes->getDisplayMode(i).width;
		int h = modes->getDisplayMode(i).height;

		char str[64];
		sprintf(str, "%dX%d", w, h);
		MenuID item = modesMenu->addMenuItem(str);
		if (w == fullscreenWidth && h == fullscreenHeight){
			modesMenu->setItemChecked(item, true);
		}
	}
	configMenu->addSubMenu(configMenu->addMenuItem("Set Fullscreen mode"), modesMenu);

	controlsMenu = new Menu();
	controlsMenu->addMenuItem("Invert Mouse: ", &invertMouse,   INPUT_BOOL);
	controlsMenu->addMenuItem("Forward: ",      &forwardKey,    INPUT_KEY);
	controlsMenu->addMenuItem("Backward: ",     &backwardKey,   INPUT_KEY);
	controlsMenu->addMenuItem("Left: ",         &leftKey,       INPUT_KEY);
	controlsMenu->addMenuItem("Right: ",        &rightKey,      INPUT_KEY);
	controlsMenu->addMenuItem("Up: ",           &upKey,         INPUT_KEY);
	controlsMenu->addMenuItem("Down: ",         &downKey,       INPUT_KEY);
	controlsMenu->addMenuItem("Reset camera: ", &resetKey,      INPUT_KEY);
	controlsMenu->addMenuItem("Toggle Fps: ",   &showFpsKey,    INPUT_KEY);
	controlsMenu->addMenuItem("Menu: ",         &menuKey,       INPUT_KEY);
	controlsMenu->addMenuItem("Console: ",      &consoleKey,    INPUT_KEY);
	controlsMenu->addMenuItem("Screenshot: ",   &screenshotKey, INPUT_KEY);
	configMenu->addSubMenu(configMenu->addMenuItem("Controls"), controlsMenu);

	//configMenu->addMenuItem("Options");
	menu->addSubMenu(menu->addMenuItem("Configure"), configMenu);

	menu->addMenuItem("Exit");
}

bool App::processMenu(){
	if (menuSystem->getCurrentMenu() == modesMenu){
		MenuID item = modesMenu->getCurrentItem();
		fullscreenWidth  = modes->getDisplayMode(item).width;
		fullscreenHeight = modes->getDisplayMode(item).height;
		refreshRate      = modes->getDisplayMode(item).refreshRate;
		modesMenu->setExclusiveItemChecked(item);
	} else {
		return false;
	}

	return true;
}

void App::showCursor(bool val){
	if (val != cursorVisible){
#if defined(_WIN32)
		ShowCursor(val);
#elif defined(LINUX)
		if (val){
			XUngrabPointer(display, CurrentTime);
		} else {
			XGrabPointer(display, window, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, window, blankCursor, CurrentTime);
		}
#endif
		cursorVisible = val;
	}
}

void App::closeWindow(){
#if defined(_WIN32)
	PostMessage(hwnd, WM_CLOSE, 0, 0);
#elif defined(LINUX)
	done = true;
#endif
}

void App::toggleScreenMode(){
	toggleFullscreen = true;
	if (fullscreen){
		resetDisplayMode();
		showCursor(true);
	}

	fullscreen = !fullscreen;
}

void App::initPixelFormat(PixelFormat &pf){
	pf.redBits   = 8;
	pf.greenBits = 8;
	pf.blueBits  = 8;
	pf.alphaBits = 8;
	pf.depthBits   = 24;
	pf.stencilBits = 0;
	pf.accumBits   = 0;
	pf.fsaaLevel = fsaaLevel;
}

void App::controls(){
#ifndef NO_BENCH
	if (demoPlayMode){
		char str[256];
		unsigned int len = 0;
		if (demoFrameCounter == 0){
			len = sprintf(str, "[Beginning of demo]\r\n");
		}
		len += sprintf(str + len, "%f\r\n", frameTime);
		fwrite(str, len, 1, demoFile);

		position = demoArray[demoFrameCounter].pos;
		wx = demoArray[demoFrameCounter].wx;
		wy = demoArray[demoFrameCounter].wy;
		wz = demoArray[demoFrameCounter].wz;
		demoFrameCounter++;
		if (demoFrameCounter >= demoSize) demoFrameCounter = 0;
	} else {
#endif	
		float sqrLen;
		vec3 dir(0,0,0);

		vec3 dx(modelView.elem[0][0], modelView.elem[0][1], modelView.elem[0][2]);
		vec3 dy(modelView.elem[1][0], modelView.elem[1][1], modelView.elem[1][2]);
		vec3 dz(modelView.elem[2][0], modelView.elem[2][1], modelView.elem[2][2]);

		if (keys[leftKey ]) dir -= dx;
		if (keys[rightKey]) dir += dx;

		if (keys[backwardKey]) dir -= dz;
		if (keys[forwardKey ]) dir += dz;

		if (keys[downKey]) dir -= dy;
		if (keys[upKey  ]) dir += dy;

		if (keys[resetKey]) resetCamera();

		if ((sqrLen = dot(dir, dir)) != 0){
			dir *= 1.0f / sqrtf(sqrLen);
		}

		processMovement(position + frameTime * speed * dir);
#ifndef NO_BENCH
	}
	if (demoRecMode) recordDemo();
#endif
}

void App::processMovement(const vec3 &newPosition){
	position = newPosition;
}

void App::processKey(unsigned int key){
	static bool waitKey = false;

	if (waitKey){
		getMenuSystem()->getCurrentMenu()->setInputKey(key);
		waitKey = false;
	} else if (key == menuKey){
		if (!showConsole) showMenu = !showMenu;
	} else if (key == consoleKey){
		if (!showMenu) showConsole = !showConsole;		
	} else if (showMenu){
		MenuSystem *menuSystem = getMenuSystem();

		if (key == KEY_DOWN){
			menuSystem->getCurrentMenu()->nextItem();
		} else if (key == KEY_UP){
			menuSystem->getCurrentMenu()->prevItem();
		} else if (key == KEY_ESCAPE){
			showMenu = menuSystem->stepUp();
		} else if (key == KEY_ENTER){
			if (!menuSystem->goSubMenu()){
				if (menuSystem->getCurrentMenu()->isCurrentItemInput()){
					if (menuSystem->getCurrentMenu()->getCurrentInputType() == INPUT_KEY){
						waitKey = true;
						menuSystem->getCurrentMenu()->setInputWait();
					} else {
						menuSystem->getCurrentMenu()->nextValue();
					}
				} else if (menuSystem->getCurrentMenu() == menuSystem->getMainMenu()){
					if (strcmp(menuSystem->getCurrentItemString(), "Exit") == 0){
						//PostMessage(hwnd, WM_CLOSE, 0, 0);
						closeWindow();
					} else if (strcmp(menuSystem->getCurrentItemString(), "Toggle Fullscreen") == 0){
						toggleScreenMode();
						//PostMessage(hwnd, WM_CLOSE, 0, 0);
						closeWindow();
					} else {
						processMenu();
					}
				} else {
					processMenu();
				}
			}
		} else if (key == KEY_LEFT){
			if (menuSystem->getCurrentMenu()->getCurrentInputType() != INPUT_KEY){
				menuSystem->getCurrentMenu()->prevValue();
			}
		} else if (key == KEY_RIGHT){
			if (menuSystem->getCurrentMenu()->getCurrentInputType() != INPUT_KEY){
				menuSystem->getCurrentMenu()->nextValue();
			}
		}
	} else if (showConsole){
		if (key == KEY_ESCAPE){
			showConsole = false;
		} else if (key == KEY_LEFT){
			if (cursorPos > 0) cursorPos--;
		} else if (key == KEY_RIGHT){
			if (cursorPos < console.getLength()) cursorPos++;
		} else if (key == KEY_BACKSPACE){
			if (cursorPos > 0){
				cursorPos--;
				console.remove(cursorPos, 1);
			}
		} else if (key == KEY_DELETE){
			console.remove(cursorPos, 1);
		} else if (key == KEY_ENTER && console.getLength() > 0){
			String results;

			consoleHistory.addObjectLast(">" + console);
			bool res = processConsole(results);
			if (results.getLength() == 0){
				results = (res? "Command OK" : "Unknown command");
			}
			unsigned int index, i = 0;

			while (true){
				if (results.find('\n', i, &index)){
					String str(((const char *) results) + i, index - i);

					consoleHistory.addObjectLast(str);
					i = index + 1;
				} else {
					consoleHistory.addObjectLast(results);
					break;
				}
			}

			/*while (consoleHistory.getCount() > 100){
				consoleHistory.removeNode(consoleHistory.getFirst());
			}*/
			cursorPos = 0;
			console = "";
		}

#ifdef _WIN32

		else if ((key == 'C' || key == KEY_INSERT) && (GetAsyncKeyState(KEY_CTRL) & 0x8000)){
			String str;
			ListNode <String> *node = consoleHistory.getFirst();
			while (node != NULL){
				str += node->object;
				str += "\r\n";
				node = node->next;
			}

			if (str.getLength() > 0 && OpenClipboard(hwnd)){
				EmptyClipboard();

				HGLOBAL handle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, str.getLength() + 1);
				char *mem = (char *) GlobalLock(handle);
				if (mem != NULL){
					strcpy(mem, str);
					GlobalUnlock(handle);
					HANDLE hand = SetClipboardData(CF_TEXT, handle);
				}
				BOOL b = CloseClipboard();
			}
		}
#endif

	} else if (key == KEY_ESCAPE){
		if (!fullscreen && captureMouse){
			showCursor(true);
			captureMouse = false;
		} else {
			//PostMessage(hwnd, WM_CLOSE, 0, 0);
			closeWindow();
		}
	} else if (key == showFpsKey){
		showFPS = !showFPS;
	} else if (key == screenshotKey){
		snapScreenshot();
	} else setKey(key, true);
}

void App::processChar(char ch){
	if (showConsole){
		if (defaultFont.isCharDefined(ch)){
			console.insert(cursorPos, (const char *) &ch, 1);
			cursorPos++;
		}		
	} 
}

#define CHECK_ARG(x) if ((x) == NULL){ results = tooFew; return false; }

bool App::processConsole(String &results){
	static char *tooFew = "Too few arguments";

	Tokenizer tok;
	tok.setString(console);
	char *str = tok.next();

	if (str == NULL){
		results.sprintf("No command given");
	} else if (stricmp(str, "pos") == 0){
		results.sprintf("%g, %g, %g", position.x, position.y, position.z);
	} else if (stricmp(str, "setpos") == 0){
		CHECK_ARG(str = tok.next());
		if (str[0] == '-'){
			CHECK_ARG(str = tok.next());
			position.x = -(float) atof(str);
		} else position.x = (float) atof(str);
		CHECK_ARG(str = tok.next());
		if (*str == ',') CHECK_ARG(str = tok.next());
		if (str[0] == '-'){
			CHECK_ARG(str = tok.next());
			position.y = -(float) atof(str);
		} else position.y = (float) atof(str);
		CHECK_ARG(str = tok.next());
		if (*str == ',') CHECK_ARG(str = tok.next());
		if (str[0] == '-'){
			CHECK_ARG(str = tok.next());
			position.z = -(float) atof(str);
		} else position.z = (float) atof(str);
	} else if (stricmp(str, "angles") == 0){
		results.sprintf("%g, %g, %g", wx, wy, wz);
	} else if (stricmp(str, "setangles") == 0){
		CHECK_ARG(str = tok.next());
		if (str[0] == '-'){
			CHECK_ARG(str = tok.next());
			wx = -(float) atof(str);
		} else wx = (float) atof(str);
		CHECK_ARG(str = tok.next());
		if (*str == ',') CHECK_ARG(str = tok.next());
		if (str[0] == '-'){
			CHECK_ARG(str = tok.next());
			wy = -(float) atof(str);
		} else wy = (float) atof(str);
		CHECK_ARG(str = tok.next());
		if (*str == ',') CHECK_ARG(str = tok.next());
		if (str[0] == '-'){
			CHECK_ARG(str = tok.next());
			wz = -(float) atof(str);
		} else wz = (float) atof(str);
	} else if (stricmp(str, "setspeed") == 0){
		CHECK_ARG(str = tok.next());
		speed = (float) atof(str);
	} else if (stricmp(str, "width") == 0){
		results.sprintf("%d", width);
	} else if (stricmp(str, "height") == 0){
		results.sprintf("%d", height);
	}
#ifndef NO_BENCH
	else if (stricmp(str, "demorec") == 0){
		if (demoRecMode){
			results = "Demo already being recorded";
		} else {
			if ((str = tok.nextLine()) != NULL){
				if (demoRecMode = beginDemo(str + 1)){
					results = "Demo recording initialized";
				} else {
					results = "Error recording demo";
				}
			} else {
				results = "No filename specified";
			}
		}
	} else if (stricmp(str, "demostop") == 0){
		if (demoRecMode){
			stopDemo();
			results = "Demo recording stopped";
		} else if (demoPlayMode){
			fclose(demoFile);
			demoPlayMode = false;
			results = "Demo play stopped";
		} else {
			results = "No demo active";
		}
	} else if (stricmp(str, "demoplay") == 0){
		if (demoRecMode){
			results = "Stop demo recording first";
		} else {
			if ((str = tok.nextLine()) != NULL){
				if (demoPlayMode = loadDemo(str + 1)){
					results = "Demo play initialized";
					demoFrameCounter = 0;
					demoFile = fopen("demo.log", "wb");
				} else {
					results = "Error playing demo";
				}
			} else {
				results = "No filename specified";
			}
		}
	}
#endif
	else {
		return false;
	}

	return true;
}

float App::getFps(){
	static float fps[15];
	static int currPos = 0;

	fps[currPos] = 1.0f / frameTime;
	currPos++;
	if (currPos > 14) currPos = 0;

	// Apply a median filter to get rid of temporal peeks
	float min = 0, cmin;
	for (int i = 0; i < 8; i++){
		cmin = 1e30f;
		for (int j = 0; j < 15; j++){
			if (fps[j] > min && fps[j] < cmin){
				cmin = fps[j];
			}
		}
		min = cmin;
	}
	return min;
}

void App::drawGUI(){
	if (textTexture != TEXTURE_NONE){
		if (showFPS || showMenu || showConsole){
			renderer->setDepthFunc(DEPTH_NONE);
			renderer->setMask(COLOR);
			renderer->setTextures(textTexture);
			renderer->setBlending(SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
			renderer->apply();
		}

		if (showFPS){
			char str[32];
			sprintf(str, "%d", (int) (getFps() + 0.5f));

			drawText(str, 0.02f, 0.02f, 0.045f, 0.07f);
		}

		if (showMenu){
			Menu *menu = menuSystem->getCurrentMenu();

			unsigned int i, n = menu->getItemCount();

			float charHeight = min(0.12f, 0.98f / n);
			float y = 0.5f * (1 - charHeight * n);
			for (i = 0; i < n; i++){
				float len = defaultFont.getStringLength(menu->getItemString(i));

				float charWidth = min(0.08f, 0.98f / len);
				drawText(menu->getItemString(i), 0.5f * (1 - len * charWidth), y, charWidth, charHeight, 1, !menu->isItemChecked(i), i != menu->getCurrentItem());

				y += charHeight;
			}
		}

		if (showConsole){
			ListNode <String> *node = consoleHistory.getLast();

			float y = 0.85f;
			while (node != NULL && y > -0.05f){
				drawText((char *) (const char *) node->object, 0, y, 0.035f, 0.05f);
				node = node->prev;
				y -= 0.05f;
			}




			String str = ">" + console;
			char *st = (char *) (const char *) str;

			drawText(st, 0, 0.9f, 0.07f, 0.10f);

			float r = 1;
			if (cursorPos < console.getLength()){
				r = defaultFont.getStringLength(st + cursorPos + 1, 1) / defaultFont.getStringLength("_", 1);
			}

			drawText("_", 0.07f * defaultFont.getStringLength(st, cursorPos + 1), 0.9f, 0.07f * r, 0.10f);
		}
	}
}

bool App::setDefaultFont(const char *fontFile, const char *textureFile){
	if (!defaultFont.loadFromFile(fontFile)) return false;
	textTexture = renderer->addTexture(textureFile);

	return (textTexture != TEXTURE_NONE);
}

#ifdef _WIN32
#include <shlobj.h>
#endif

void App::snapScreenshot(){
	char path[256];

#if defined(_WIN32)
	SHGetSpecialFolderPath(NULL, path, CSIDL_DESKTOPDIRECTORY, FALSE);
#elif defined(LINUX)
	strcpy(path, getenv("HOME"));
	strcat(path, "/Desktop");
#endif

	FILE *file;
	int pos = strlen(path);

	strcpy(path + pos, "/Screenshot00."
#if !defined(NO_PNG)
	"png"
#elif !defined(NO_TGA)
	"tga"
#else
	"dds"
#endif
	);

	pos += 11;

	int i = 0;
	do {
		path[pos]     = '0' + (i / 10);
		path[pos + 1] = '0' + (i % 10);

		if ((file = fopen(path, "r")) != NULL){
			fclose(file);
		} else {
			Image img;
			if (getScreenshot(img)) img.saveImage(path, true);
			break;
		}
		i++;
	} while (i < 100);
}

#ifndef NO_BENCH

bool App::beginDemo(char *fileName){
	return ((demoFile = fopen(fileName, "wb")) != NULL);
}

void App::recordDemo(){
	static DemoNode node;

	if (node.pos != position || node.wx != wx || node.wy != wy || node.wz != wz){
		node.pos = position;
		node.wx = wx;
		node.wy = wy;
		node.wz = wz;
		fwrite(&node, sizeof(node), 1, demoFile);
	}
}

void App::stopDemo(){
	fclose(demoFile);
	demoRecMode = false;
}

bool App::loadDemo(char *fileName){
	if (demoArray != NULL) delete demoArray;
	if ((demoFile = fopen(fileName, "rb")) == NULL) return false;

	fseek(demoFile, 0, SEEK_END);
	demoSize = ftell(demoFile) / sizeof(DemoNode);
	fseek(demoFile, 0, SEEK_SET);

	demoArray = new DemoNode[demoSize];
	fread(demoArray, demoSize * sizeof(DemoNode), 1, demoFile);
	fclose(demoFile);

	return true;
}

#endif
