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

#include "../App.h"
#include "../CPU.h"
#include "../OpenGL/glExtensions.h"
#include "../DisplayMode.h"
//#include "~/Portals/Portals/mouseposition.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <limits.h>

extern App *app;

float mousexxxxxx = 0;
float mouseyyyyyy = 0;
int main(int argc, char **argv){
	// Be nice to other processes. Helps reduce mouse lag
	setpriority(PRIO_PROCESS, 0, 20);

	// Make sure we're running in the exe's directory
	char path[PATH_MAX];
	if (realpath("/proc/self/exe", path)){
		char *slash = strrchr(path, '/');
		if (slash) *slash = '\0';
		chdir(path);
	}



	initGlobals();

	float runTime = 1e30f;
	float staticTime = 0;
	bool isStatic = false, lastStatic = false;
	for (int i = 1; i < argc; i++){
		if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--run-for") == 0){
			if (++i < argc){
				runTime = atof(argv[i]);							
			}
		} else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--static") == 0){
			isStatic = true;
			lastStatic = true;
			continue;
		} else if (lastStatic){
			staticTime = atof(argv[i]);								
		} else {
			printf("Unknown argument: %s\n", argv[i]);
		}
		lastStatic = false;
	}



	Display *display;
	int screen;

    initCPU();
    cpuHz = getHz();

	if (!app->init()) return 0;
	app->resetCamera();

	display = XOpenDisplay(0);
	screen = DefaultScreen(display);
	app->setDisplay(display, screen);

	app->initDisplayModes();
	app->initMenu();

	//toggleFullscreen = false;
	time = 0;
//	uint64 prev, curr = getCycleNumber();
	timeval prev, curr;
	gettimeofday(&curr, NULL);

	float rTime = 0;
	
	do {
		//if (toggleFullscreen) fullscreen = !fullscreen;
		toggleFullscreen = false;
		captureMouse = fullscreen;

		if (fullscreen) app->setDisplayMode(width = fullscreenWidth, height = fullscreenHeight, refreshRate);

		if (app->initAPI()){
			app->showCursor(!captureMouse);
			if (captureMouse) XWarpPointer(display, None, window, 0,0,0,0, middleX, middleY);

			if (app->load()){

				XEvent event;
				unsigned int key;
				done = false;
				
				do {
///*
					while (XPending(display) > 0){

						XNextEvent(display, &event);
						
						switch (event.type){
						case Expose:
							if (event.xexpose.count != 0) break;
							break;
						case MotionNotify:
							if (captureMouse){
								static bool changeMouse;
								float mouseSensibility = 0.0005f * mouseSpeed;

								//app->rotateView(mouseSensibility * (middleY - event.xmotion.y) * (invertMouse? 1 : -1),
								//				mouseSensibility * (middleX - event.xmotion.x));
								//float a = mouseSensibility * (middleY - event.xmotion.y) * (invertMouse? 1 : -1);
								//float b = mouseSensibility * (middleX - event.xmotion.x);
								mousexxxxxx = mouseSensibility * (middleY - event.xmotion.y) * (invertMouse? 1 : -1);
								mouseyyyyyy = mouseSensibility * (middleX - event.xmotion.x);
								printf("%f %f\n", mouseSensibility, middleY);
								if (changeMouse = !changeMouse) XWarpPointer(display, None, window, 0,0,0,0, middleX, middleY);
							}
							app->setMousePos(event.xmotion.x, event.xmotion.y, 0);
							break;
						case ConfigureNotify:
							app->setViewport(width = event.xconfigure.width, height = event.xconfigure.height);
							if (!fullscreen){
								windowedLeft   = event.xconfigure.x;
								windowedRight  = event.xconfigure.width + windowedLeft;
								windowedTop    = event.xconfigure.y;
								windowedBottom = event.xconfigure.height + windowedTop;
							}
							middleX = event.xconfigure.width  / 2;
							middleY = event.xconfigure.height / 2;
							break;
						case ButtonPress:
							if (!captureMouse && event.xbutton.button == 1){
								//XGrabPointer(display, window, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, window, blankCursor, CurrentTime);
								app->showCursor(false);
								XWarpPointer(display, None, window, 0, 0, 0, 0, middleX, middleY);
								captureMouse = true;
							}
							app->setButton((event.xbutton.button == 1)? MOUSE_LEFT : MOUSE_RIGHT, true);
							break;
						case ButtonRelease:
							app->setButton((event.xbutton.button == 1)? MOUSE_LEFT : MOUSE_RIGHT, false);
							break;
						case KeyPress:
							key = XLookupKeysym(&event.xkey, 0);
 							if (key == XK_Return && app->getKey(XK_Alt_L)){
								app->toggleScreenMode();
								app->closeWindow();
							} else {
								app->processKey(key);

								char str[8];
								int nChar = XLookupString(&event.xkey, str, sizeof(str), NULL, NULL);
								for (int i = 0; i < nChar; i++) app->processChar(str[i]);
							}
							break;
						case KeyRelease:
							key = XLookupKeysym(&event.xkey, 0);
							app->setKey(key, false);
							break;
						case ClientMessage:
							if (*XGetAtomName(display, event.xclient.message_type) == *"WM_PROTOCOLS"){
								app->closeWindow();
							}
							break;
						default:
							break;
						}
						
					}
//*/
					prev = curr;
//					curr = getCycleNumber();
//					frameTime = (float) (double(curr - prev) / cpuHz);
					gettimeofday(&curr, NULL);
					frameTime = (float(curr.tv_sec - prev.tv_sec) + 0.000001f * float(curr.tv_usec - prev.tv_usec));
					if (isStatic){
						time = staticTime;
					} else {
						time += frameTime;
					}
					rTime += frameTime;
					if (rTime > runTime) done = true;

					app->controls();
					/*int a = 0;
					for (unsigned long int i = 0; i < 10000000000; i++){
						a++;
						
					}
printf("%d ",a);*/
					app->beginFrame();
					app->drawFrame();
					app->drawGUI();
					app->endFrame();

				} while (!done);
				
				app->unload();
			} else {
				printf(getLogString());
			}

			app->closeAPI();
		} else {
			printf(getLogString());
		}
		//if (fullscreen) app->resetDisplayMode();

		XDestroyWindow(display, window);

	} while (toggleFullscreen);
	
	if (fullscreen) app->resetDisplayMode();

	app->exit();
	delete app;
	XCloseDisplay(display);

	flushGlobals();
	clearLog();

	return 0;
}
