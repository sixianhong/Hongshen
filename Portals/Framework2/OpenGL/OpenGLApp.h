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

#ifndef _OPENGLAPP_H_
#define _OPENGLAPP_H_

#include "../App.h"
#include "OpenGLRenderer.h"

class OpenGLApp : public App {
public:
	OpenGLApp();

	virtual bool initAPI();
	virtual bool closeAPI();

	void beginFrame();
	bool drawFrame(){ return false; }
	void endFrame();

	virtual void setViewport(unsigned int w, unsigned int h);
	void drawText(char *str, float x, float y, float charWidth, float charHeight, float r = 1.0f, float g = 1.0f, float b = 1.0f);

	bool getScreenshot(Image &img);
protected:

#if defined(_WIN32)
	HDC hdc;
	HGLRC hglrc;
	bool wa;
#elif defined(LINUX)
	GLXContext ctx;
#endif

};

#endif // _OPENGL_APP_
