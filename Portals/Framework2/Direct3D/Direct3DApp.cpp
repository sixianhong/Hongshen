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

#include "Direct3DApp.h"

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

Direct3DApp::Direct3DApp(){
	d3d = NULL;
	dev = NULL;
}

bool Direct3DApp::initAPI(){
	PixelFormat pf;
	initPixelFormat(pf);
	selectPixelFormat(pf);


	renderer = NULL;

	if ((d3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL){
		MessageBox(hwnd, "Couldn't initialize Direct3D\nMake sure you have DirectX 9.0c or later installed.", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

    memset(&d3dpp, 0, sizeof(d3dpp));

	//D3DDISPLAYMODE displayMode;
	//d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);

	if (pf.alphaBits > 0){
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	} else {
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	}

	RECT r;
	GetClientRect(hwnd, &r);
	d3dpp.BackBufferWidth  = r.right;
	d3dpp.BackBufferHeight = r.bottom;

	if (fullscreen){
		d3dpp.Windowed = FALSE;
		d3dpp.FullScreen_RefreshRateInHz = refreshRate;
	} else {
		d3dpp.Windowed = TRUE;
	}
	d3dpp.BackBufferCount        = 1;
	d3dpp.MultiSampleType        = (D3DMULTISAMPLE_TYPE) pf.fsaaLevel;
	d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;

	d3dpp.EnableAutoDepthStencil = pf.depthBits > 0;
	d3dpp.AutoDepthStencilFormat = (pf.depthBits > 16)? ((pf.stencilBits > 0)? D3DFMT_D24S8 : D3DFMT_D24X8) : D3DFMT_D16;

    if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, /*D3DCREATE_PUREDEVICE | */ D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &dev))){
		MessageBox(hwnd, "Couldn't create Direct3D device interface.", "Error", MB_OK | MB_ICONERROR);
        return false;
    }

	renderer = new Direct3DRenderer(caps, dev);

	return true;
}

bool Direct3DApp::closeAPI(){
	if (renderer != NULL){
		renderer->clear();
		delete renderer;
	}

    if (dev != NULL){
		dev->Release();
		dev = NULL;
	}

    if (d3d != NULL) d3d->Release();

	return true;
}

void Direct3DApp::beginFrame(){
    dev->BeginScene();
}

void Direct3DApp::endFrame(){
    dev->EndScene();
    dev->Present(NULL, NULL, NULL, NULL);
}

void Direct3DApp::setViewport(unsigned int w, unsigned int h){
	if (dev != NULL){
		d3dpp.BackBufferWidth  = w;
		d3dpp.BackBufferHeight = h;

		((Direct3DRenderer *) renderer)->removeRenderTargets();
		((Direct3DRenderer *) renderer)->freeRTSurfaces();

		releaseDPRes();

		dev->Reset(&d3dpp);

		createDPRes();

		renderer->resetToDefaults();
		renderer->setDefaultStates();
		((Direct3DRenderer *) renderer)->initRTSurfaces();
		((Direct3DRenderer *) renderer)->recreateRenderTargets();

		D3DVIEWPORT9 vp = { 0, 0, width, height, 0.0f, 1.0f };
		dev->SetViewport(&vp);
	}
}

void Direct3DApp::drawText(char *str, float x, float y, float charWidth, float charHeight, float r, float g, float b){
	D3DXMATRIX mat;

	D3DXMatrixIdentity(&mat);
	dev->SetTransform(D3DTS_VIEW, &mat);

	D3DXMatrixOrthoOffCenterLH(&mat, 0, 1, 1, 0, -1, 1);
	dev->SetTransform(D3DTS_PROJECTION, &mat);

	dev->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | (1 << D3DFVF_TEXCOUNT_SHIFT) | D3DFVF_TEXCOORDSIZE2(0));

	struct qv {
		vec3 pos;
		DWORD color;
		vec2 uv;
	} quadVertices[4];

	DWORD color = ((unsigned int) (255 * b)) | ((unsigned int) (255 * g) << 8) | ((unsigned int) (255 * r) << 16);
	quadVertices[0].color = quadVertices[1].color = quadVertices[2].color = quadVertices[3].color = color;

	vec2 off(0.5f / 512.0f, 0.5f / 512.0f);

    float startx = x;
    while (*str){
        if (*str == '\n'){
            y += charHeight;
            x = startx;
        } else {
			Character chr = defaultFont.getCharRect(*str);
			float cw = charWidth * chr.ratio;

			quadVertices[0].pos = vec3(x, y, 0);
			quadVertices[0].uv  = vec2(chr.x0, chr.y0) + off;
			quadVertices[1].pos = vec3(x + cw, y, 0);
			quadVertices[1].uv  = vec2(chr.x1, chr.y0) + off;
			quadVertices[2].pos = vec3(x + cw, y + charHeight, 0);
			quadVertices[2].uv  = vec2(chr.x1, chr.y1) + off;
			quadVertices[3].pos = vec3(x, y + charHeight, 0);
			quadVertices[3].uv  = vec2(chr.x0, chr.y1) + off;

			dev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, quadVertices, sizeof(quadVertices) / 4);

            x += cw;
        }
        str++;
    }
}

bool Direct3DApp::getScreenshot(Image &img){
	POINT topLeft;
	topLeft.x = 0;
	topLeft.y = 0;
	ClientToScreen(hwnd, &topLeft);
	
	int mw = GetSystemMetrics(SM_CXSCREEN);
	int mh = GetSystemMetrics(SM_CYSCREEN);

	LPDIRECT3DSURFACE9 surface;

	dev->CreateOffscreenPlainSurface(mw, mh, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &surface, NULL);
	dev->GetFrontBufferData(0, surface);

	D3DLOCKED_RECT lockedRect;
	memset(&lockedRect, 0, sizeof(lockedRect));
	if (surface->LockRect(&lockedRect, NULL, D3DLOCK_READONLY) != D3D_OK){
		surface->Release();
		return false;
	}

	unsigned char *pixels  = new unsigned char[width * height * 4];
	unsigned char *src, *dest = pixels;
	for (int y = 0; y < height; y++){
		src = ((unsigned char *) lockedRect.pBits) + ((topLeft.y + y) * mw + topLeft.x) * 4;
		for (int x = 0; x < width; x++){
			dest[0] = src[2];
			dest[1] = src[1];
			dest[2] = src[0];
			dest += 3;
			src  += 4;
		}
	}
	surface->UnlockRect();
	surface->Release();

	img.loadFromMemory(pixels, width, height, FORMAT_RGB8, true);
	return true;
}
