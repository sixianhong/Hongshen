/***********      .---.         .-"-.      *******************\
* -------- *     /   ._.       / � ` \     * ---------------- *
* Author's *     \_  (__\      \_�v�_/     * humus@rogers.com *
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

#ifndef _OPENGLRENDERER_H_
#define _OPENGLRENDERER_H_

#include "glExtensions.h"
#include "../Renderer.h"

class OpenGLRenderer : public Renderer {
public:

#if defined(WIN32)
	HDC hdc;
	HGLRC hglrc;
	OpenGLRenderer(HDC hDc, HGLRC hGlrc);

#elif defined(LINUX)

    GLXContext glxc;
	Display *display;
	int screen;
	OpenGLRenderer(GLXContext glXc, Display *disp, int scr);
#endif

protected:
	void init();
	static GLenum targets[];

	void setDefaultStates();
	bool createRenderTarget(Texture &texture);

	bool createTexture(Texture &texture);
	void removeTexture(Texture &texture);
	void resetTextureUnits();

	bool createShader(Shader &shader, const char *vertexShaderText, const char *fragmentShaderText);
	void removeShader(Shader &shader);

	void configureTexEnv(const TexEnv texEnv, bool isRGB);
public:

	void changeRenderTarget(const TextureID colorTarget, const TextureID depthTarget, const int face = 0);
	void changeToDefaultRenderTarget();

	void assignNamedTextures();
	void changeTextureUnit(const int textureUnit);
	void changeLod(const int textureUnit, const float lod);
	void changeTexture(const TextureID textureID);
	void changeTexEnv(const TexEnv colorTexEnv, const TexEnv alphaTexEnv);

	void changeShader(const ShaderID shader);
	void changeShaderConstant1f(const char *name, const float constant);
	void changeShaderConstant2f(const char *name, const vec2 &constant);
	void changeShaderConstant3f(const char *name, const vec3 &constant);
	void changeShaderConstant4f(const char *name, const vec4 &constant);
	void changeShaderConstant3x3f(const char *name, const mat3 &constant);
	void changeShaderConstant4x4f(const char *name, const mat4 &constant);
	void changeShaderConstantArray3f(const char *name, const vec3 *constant, const unsigned int count);
	void changeShaderConstantArray4f(const char *name, const vec4 *constant, const unsigned int count);

	bool changeShaderAttribs(const ShaderID shader, const char **names, const unsigned int first, const unsigned int count);

	void changeBlending(const int srcFactor, const int destFactor, const int blendMode);
	void changeDepthFunc(const int depthFunc);
	void changeMask(const int mask);
	void changeVSync(const bool value);
	void changeAnisotropic(const TextureID textureID, const bool value);
};


#endif // _OPENGLRENDERER_H_
