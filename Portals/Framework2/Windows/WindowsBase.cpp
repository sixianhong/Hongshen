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
#include "Resource.h"
#include <stdio.h>

extern App *app;

bool apiInited, loaded;

void mouseMove(HWND hwnd, int x, int y){
	static bool changeMouse = false;

	if (captureMouse){
		POINT point;

		point.x = x;
		point.y = y;
		ClientToScreen(hwnd, &point);
		float mouseSensibility = 0.0005f * mouseSpeed;

		//app->rotateView(mouseSensibility * (middleY - point.y) * (invertMouse? 1 : -1), mouseSensibility * (middleX - point.x));

		//if (changeMouse = !changeMouse) SetCursorPos(middleX, middleY);
	}
}

void setTitle(HWND hwnd){
	char str[256];
	sprintf(str, "%dx%d", width, height);
	SetWindowText(hwnd, str);
}

//#include <shlobj.h>

LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch (message){
	case WM_MOUSEMOVE:
		mouseMove(hwnd, LOWORD(lParam), HIWORD(lParam));
		app->setMousePos(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_KEYDOWN:
		app->processKey(wParam);
		break;
	case WM_KEYUP:
		app->setKey(wParam, false);
		break;
	case WM_CHAR:
		app->processChar(wParam);
		break;
	case WM_SYSKEYDOWN:
		if ((lParam & (1 << 29)) && wParam == KEY_ENTER){
			app->toggleScreenMode();
			app->closeWindow();
		}
		break;
	case WM_LBUTTONDOWN:
		if (!captureMouse){
			SetCursorPos(middleX, middleY);
			app->showCursor(false);
			captureMouse = true;
		}
		app->setButton(MOUSE_LEFT, true);
 		break;
	case WM_LBUTTONUP:
		app->setButton(MOUSE_LEFT, false);
		break;
	case WM_RBUTTONDOWN:
		app->setButton(MOUSE_RIGHT, true);
		break;
	case WM_RBUTTONUP:
		app->setButton(MOUSE_RIGHT, false);
		break;
	case WM_SIZE:
		app->setViewport(width = LOWORD(lParam), height = HIWORD(lParam));
		setTitle(hwnd);
		break;
	case WM_WINDOWPOSCHANGED:
		if (!toggleFullscreen && !fullscreen){
			windowedLeft   = ((LPWINDOWPOS) lParam)->x;
			windowedRight  = ((LPWINDOWPOS) lParam)->x + ((LPWINDOWPOS) lParam)->cx;
			windowedTop    = ((LPWINDOWPOS) lParam)->y;
			windowedBottom = ((LPWINDOWPOS) lParam)->y + ((LPWINDOWPOS) lParam)->cy;
		}
		middleX = ((LPWINDOWPOS) lParam)->x + ((LPWINDOWPOS) lParam)->cx / 2;
		middleY = ((LPWINDOWPOS) lParam)->y + ((LPWINDOWPOS) lParam)->cy / 2;
		if ((((LPWINDOWPOS) lParam)->flags & SWP_NOSIZE) == 0){
			RECT rect;
			GetClientRect(hwnd, &rect);
			app->setViewport(width = rect.right - rect.left, height = rect.bottom - rect.top);
			setTitle(hwnd);
		}
		break;
	case WM_CREATE:
		ShowWindow(hwnd, SW_SHOW);
		app->setHwnd(hwnd);
		break;
	case WM_CLOSE:
		if (fullscreen) app->resetDisplayMode();

		if (apiInited){
			if (loaded) app->unload();
			app->closeAPI();
		}
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

#include <crtdbg.h>

int WINAPI WinMain(HINSTANCE hThisIns, HINSTANCE hLastIns, LPSTR lpszCmdLine, int nCmdShow){
/*#ifdef _DEBUG
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
	flag |= _CRTDBG_CHECK_ALWAYS_DF; // Turn on CrtCheckMemory 
	_CrtSetDbgFlag(flag); // Set flag to the new value
#endif*/

	SetThreadAffinityMask(GetCurrentThread(), 1);

	MSG msg;
	WNDCLASS wincl;

	wincl.hInstance = hThisIns;
	wincl.lpszClassName = "Demo";
	wincl.lpfnWndProc = WinProc;
	wincl.style = 0;
	wincl.hIcon = LoadIcon(hThisIns, MAKEINTRESOURCE(IDI_MAINICON));
	wincl.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wincl.lpszMenuName = NULL;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;
	wincl.hbrBackground = NULL;

	if (!RegisterClass(&wincl)) return 0;

	initGlobals();

    initCPU();
    cpuHz = getHz();

	if (!app->init()) return 0;
	app->resetCamera();

	app->initDisplayModes();
	app->initMenu();

	time = 0;
	int64 prev, curr = getCycleNumber();

	do {
		toggleFullscreen = false;

		if (fullscreen){
			if (!app->setDisplayMode(fullscreenWidth, fullscreenHeight, refreshRate)){
				MessageBox(NULL, "Could not set display mode", "Error", MB_OK | MB_ICONERROR);
				fullscreen = false;
			}
		}

		int x, y, w, h, flags;
		if (fullscreen){
			x = y = 0;
			w = fullscreenWidth;
			h = fullscreenHeight;
			flags = WS_POPUP;
		} else {
			x = windowedLeft;
			y = windowedTop;
			w = windowedRight  - windowedLeft;
			h = windowedBottom - windowedTop;
			flags = WS_OVERLAPPEDWINDOW;
		}
		middleX = x + w / 2;
		middleY = y + h / 2;
		captureMouse = fullscreen;

		if (captureMouse){
			app->showCursor(false);
			SetCursorPos(middleX, middleY);
		}

		CreateWindow("Demo", "Title", flags | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, x, y, w, h, HWND_DESKTOP, NULL, hThisIns, NULL);
		if (apiInited = app->initAPI()){
			loaded = app->load();
		}
		if (!apiInited || !loaded){
			MessageBox(NULL, getLogString(), "Error", MB_OK | MB_ICONERROR);
			app->closeWindow();
		}

		do {
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (msg.message == WM_QUIT) break;

			prev = curr;
			curr = getCycleNumber();
			frameTime = (float) (double(curr - prev) / cpuHz);
			time += frameTime;

			app->controls();

			app->beginFrame();
			app->drawFrame();
			app->drawGUI();
			app->endFrame();

		} while (true);
	} while (toggleFullscreen);

	app->exit();

	delete app;

	flushGlobals();
	clearLog();

	return msg.wParam;
}
