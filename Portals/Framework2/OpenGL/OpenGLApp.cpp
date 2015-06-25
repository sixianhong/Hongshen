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

#include "OpenGLApp.h"

#if defined(_WIN32)
#pragma comment (lib, "opengl32.lib")

void initEntryPoints(HWND hwnd, const PIXELFORMATDESCRIPTOR &pfd){
	HDC hdc = ::GetDC(hwnd);

	int nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, nPixelFormat, &pfd);

	HGLRC hglrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hglrc);

	initExtensions(hdc);

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hglrc);
	::ReleaseDC(hwnd, hdc);
}

LRESULT CALLBACK PFWinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	return DefWindowProc(hwnd, message, wParam, lParam);
};

OpenGLApp::OpenGLApp(){
	wa = false;
}

bool OpenGLApp::initAPI(){
	PixelFormat pf;
	initPixelFormat(pf);
	selectPixelFormat(pf);

	int bpp = pf.alphaBits > 0? 32 : 24;
	wa = (pf.accumBits > 0);

    static PIXELFORMATDESCRIPTOR pfd = {
        sizeof (PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		bpp,
		0, 0, 0, 0, 0, 0, 0, 0,
		pf.accumBits, pf.accumBits / 4, pf.accumBits / 4, pf.accumBits / 4, pf.accumBits / 4,
		pf.depthBits,
		pf.stencilBits,
		pf.alphaBits,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
    };


	WNDCLASS wincl;
	HINSTANCE hInst = (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE);

	wincl.hInstance = hInst;
	wincl.lpszClassName = "PFrmt";
	wincl.lpfnWndProc = PFWinProc;
	wincl.style = 0;
	wincl.hIcon = NULL;
	wincl.hCursor = NULL;
	wincl.lpszMenuName = NULL;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;
	wincl.hbrBackground = NULL;
	RegisterClass(&wincl);

	HWND hPFwnd = CreateWindow("PFrmt", "PFormat", WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 8, 8, HWND_DESKTOP, NULL, hInst, NULL);
	initEntryPoints(hPFwnd, pfd);
	SendMessage(hPFwnd, WM_CLOSE, 0, 0);

	hdc = GetDC(hwnd);

	int pixelFormat;
	float fAttribs[256];
	int iAttribs[256] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
		WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,

		WGL_RED_BITS_ARB,       pf.redBits,
		WGL_GREEN_BITS_ARB,     pf.greenBits,
		WGL_BLUE_BITS_ARB,      pf.blueBits,
		WGL_ALPHA_BITS_ARB,     pf.alphaBits,
		WGL_DEPTH_BITS_ARB,     pf.depthBits,
		WGL_STENCIL_BITS_ARB,   pf.stencilBits,
		WGL_ACCUM_BITS_ARB,     pf.accumBits,
	};

	while (true){
		int   *iAtt = iAttribs + 20;
		float *fAtt = fAttribs;

		if (WGL_ARB_multisample_supported && pf.fsaaLevel > 0){
			*iAtt++ = WGL_SAMPLE_BUFFERS_ARB;
			*iAtt++ = GL_TRUE;

			*iAtt++ = WGL_SAMPLES_ARB;
			*iAtt++ = pf.fsaaLevel;
		}

		*iAtt++ = 0;
		*fAtt++ = 0;

		unsigned int nMatchingPixelFormats;
		if (!WGL_ARB_pixel_format_supported || !wglChoosePixelFormatARB(hdc, iAttribs, fAttribs, 1, &pixelFormat, &nMatchingPixelFormats) || nMatchingPixelFormats == 0){
			if (pf.fsaaLevel > 0){
				pf.fsaaLevel -= 2;
			} else {
				pixelFormat = ChoosePixelFormat(hdc, &pfd);
				break;
			}
		} else break;
	}

    SetPixelFormat(hdc, pixelFormat, &pfd);

	hglrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hglrc);

	initExtensions(hdc);

	if (WGL_ARB_multisample_supported && pf.fsaaLevel > 0){
		glEnable(GL_MULTISAMPLE_ARB);
	}


	renderer = new OpenGLRenderer(hdc, hglrc);

	return true;
}

bool OpenGLApp::closeAPI(){
	renderer->clear();
	delete renderer;

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hglrc);
	::ReleaseDC(hwnd, hdc);

	return true;
}


#elif defined(LINUX)

#include <stdio.h>

OpenGLApp::OpenGLApp(){
}

bool OpenGLApp::initAPI(){
	PixelFormat pf;
	initPixelFormat(pf);
	selectPixelFormat(pf);

	int iAttribs[256] = {
		GLX_RGBA,
		GLX_DOUBLEBUFFER,
		GLX_RED_SIZE,     pf.redBits,
		GLX_GREEN_SIZE,   pf.greenBits,
		GLX_BLUE_SIZE,    pf.blueBits,
		GLX_ALPHA_SIZE,   pf.alphaBits,
		GLX_DEPTH_SIZE,   pf.depthBits,
		GLX_STENCIL_SIZE, pf.stencilBits,
		GLX_ACCUM_RED_SIZE,   pf.accumBits / 4,
		GLX_ACCUM_GREEN_SIZE, pf.accumBits / 4,
		GLX_ACCUM_BLUE_SIZE,  pf.accumBits / 4,
		GLX_ACCUM_ALPHA_SIZE, pf.accumBits / 4,
	};

	XVisualInfo *vi;
	while (true){
		int *iAtt = iAttribs + 22;
		if (/*GLX_ARB_multisample_supported && */pf.fsaaLevel > 0){
			*iAtt++ = 100000;
			*iAtt++ = GL_TRUE;

			*iAtt++ = 100001;
			*iAtt++ = pf.fsaaLevel;
		}

		*iAtt++ = None;

		vi = glXChooseVisual(display, screen, iAttribs);
		if (vi == NULL){
			if (pf.fsaaLevel > 0){
				pf.fsaaLevel -= 2;
			} else {
				printf("Error: Couldn't set visual\n");
				return false;
			}
		} else break;
	}


	int x, y, w, h;
	if (fullscreen){
		x = 0;
		y = 0;
		w = fullscreenWidth;
		h = fullscreenHeight;
	} else {
		x = windowedLeft;
		y = windowedTop;
		w = windowedRight - windowedLeft;
		h = windowedBottom - windowedTop;
	}
	middleX = w / 2;
	middleY = h / 2;

    ctx = glXCreateContext(display, vi, 0, GL_TRUE);


	XSetWindowAttributes attr;
	attr.colormap = XCreateColormap(display, RootWindow(display, screen), vi->visual, AllocNone);

	attr.border_pixel = 0;
	attr.override_redirect = fullscreen;
    attr.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask;

	window = XCreateWindow(display, RootWindow(display, vi->screen),
			x, y, w, h, 0, vi->depth, InputOutput, vi->visual,
			CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect, &attr);

	if (!fullscreen){
	    Atom wmDelete;
        wmDelete = XInternAtom(display, "WM_DELETE_WINDOW", True);
        XSetWMProtocols(display, window, &wmDelete, 1);
		char *title = "OpenGL";
        XSetStandardProperties(display, window, title, title, None, NULL, 0, NULL);
	}
    XMapRaised(display, window);

	Pixmap blank;
	XColor dummy;
	char data = 0;

	// Create a blank cursor for cursor hiding
	blank = XCreateBitmapFromData(display, window, &data, 1, 1);
	blankCursor = XCreatePixmapCursor(display, blank, blank, &dummy, &dummy, 0, 0);
	XFreePixmap(display, blank);


	XGrabKeyboard(display, window, True, GrabModeAsync, GrabModeAsync, CurrentTime);
	//if (captureMouse) showCursor(false);//XGrabPointer(display, window, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, window, blankCursor, CurrentTime);
	cursorVisible = true;


    glXMakeCurrent(display, window, ctx);

	//printf((char *) glXGetClientString(display, GLX_EXTENSIONS));


	initExtensions(display, screen);

	// Set some of my preferred defaults
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glFrontFace(GL_CW);

	if (/*GLX_ARB_multisample_supported && */pf.fsaaLevel > 0){
		glEnable(GL_MULTISAMPLE_ARB);
	}

	renderer = new OpenGLRenderer(ctx, display, screen);

//	renderer->setAnisotropic(anisotropic);

	return true;
}

bool OpenGLApp::closeAPI(){
	renderer->clear();
	delete renderer;

    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, ctx);

	return true;
}

#endif // LINUX




void OpenGLApp::beginFrame(){
}

void OpenGLApp::endFrame(){
#if defined(_WIN32)
	SwapBuffers(hdc);
#elif defined(LINUX)
	glXSwapBuffers(display, window);
	glFinish();
#endif
}

void OpenGLApp::setViewport(unsigned int w, unsigned int h){
#ifdef _WIN32
	if (wa){
		int d = (w & 0xF);
		if (d < 3 || d > 13){
			RECT rect;
			GetWindowRect(hwnd, &rect);
			int diff = 3 + 10 * (d > 13) - d;
			rect.right += diff;
			w += diff;
			SetWindowPos(hwnd, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0);
		}
	}
#endif

	glViewport(0, 0, w, h);
}

void OpenGLApp::drawText(char *str, float x, float y, float charWidth, float charHeight, float r, float g, float b){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1, 1, 0, -1, 1);

	glColor3f(r, g, b);

    float startx = x;

	glBegin(GL_QUADS);
    while (*str){
        if (*str == '\n'){
            y += charHeight;
            x = startx;
        } else {
			Character chr = defaultFont.getCharRect(*str);
			float cw = charWidth * chr.ratio;

			glTexCoord2f(chr.x0, chr.y0);
			glVertex2f(x, y);

			glTexCoord2f(chr.x1, chr.y0);
			glVertex2f(x + cw, y);

			glTexCoord2f(chr.x1, chr.y1);
			glVertex2f(x + cw, y + charHeight);

			glTexCoord2f(chr.x0, chr.y1);
			glVertex2f(x, y + charHeight);

            x += cw;
        }
        str++;
    }
    glEnd();
}

bool OpenGLApp::getScreenshot(Image &img){
	unsigned char *pixels  = new unsigned char[width * height * 3];
	unsigned char *flipped = new unsigned char[width * height * 3];;

	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, flipped);
	for (int y = 0; y < height; y++){
		memcpy(pixels + y * width * 3, flipped + (height - y - 1) * width * 3, width * 3);
	}
	delete flipped;

	img.loadFromMemory(pixels, width, height, FORMAT_RGB8, true);
	return true;
}
