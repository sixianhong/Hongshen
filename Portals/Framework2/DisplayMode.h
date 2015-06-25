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

#ifndef _DISPLAYMODE_H_
#define _DISPLAYMODE_H_

#if defined(_WIN32)
#include <windows.h>
#elif defined(LINUX)
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>
#endif

#include "Util/Set.h"

class DisplayMode {
public:
	DisplayMode(int i, int w, int h, int b, int r){
		index  = i;
		width  = w;
		height = h;
		bpp    = b;
		refreshRate = r;
	}

	int width, height;
	int bpp;
	int refreshRate;
	int index;

	unsigned int dist(int w, int h, int b, int r);
};


class DisplayModeHandler {
protected:
	Set <DisplayMode> modes;
	int nModes;

#if defined(_WIN32)
	Set <DEVMODE> dmodes;
public:
	DisplayModeHandler();
#elif defined(LINUX)
    XF86VidModeModeInfo **dmodes;
	Display *display;
	int screen;
public:
	DisplayModeHandler(Display *disp, int scr);

#endif

	~DisplayModeHandler();

	const DisplayMode &getDisplayMode(const unsigned int index) const {	return modes[index]; }
	unsigned int getNumberOfDisplayModes() const { return modes.getCount();	}
	void filterModes(int widthLow, int heightLow, int bppLow, int widthHigh = 0x7fffffff, int heightHigh = 0x7fffffff, int bppHigh = 0x7fffffff, int refreshRateLow = 0, int refreshRateHigh = 0x7fffffff);
	void filterRefreshes(int frequency);

	bool setDisplayMode(unsigned int index) const;
	bool setDisplayMode(int width, int height, int bpp, int refreshRate) const;
	bool resetDisplayMode();
};

#endif // _DISPLAYMODE_H_
