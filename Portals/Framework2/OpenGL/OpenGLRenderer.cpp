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

#include "OpenGLRenderer.h"

// Blending constants
const int ZERO                 = GL_ZERO;
const int ONE                  = GL_ONE;
const int SRC_COLOR            = GL_SRC_COLOR;
const int ONE_MINUS_SRC_COLOR  = GL_ONE_MINUS_SRC_COLOR;
const int DST_COLOR            = GL_DST_COLOR;
const int ONE_MINUS_DST_COLOR  = GL_ONE_MINUS_DST_COLOR;
const int SRC_ALPHA            = GL_SRC_ALPHA;
const int ONE_MINUS_SRC_ALPHA  = GL_ONE_MINUS_SRC_ALPHA;
const int DST_ALPHA            = GL_DST_ALPHA;
const int ONE_MINUS_DST_ALPHA  = GL_ONE_MINUS_DST_ALPHA;
const int SRC_ALPHA_SATURATE   = GL_SRC_ALPHA_SATURATE;

const int BLENDMODE_ADD              = GL_FUNC_ADD_EXT;
const int BLENDMODE_SUBTRACT         = GL_FUNC_SUBTRACT_EXT;
const int BLENDMODE_REVERSE_SUBTRACT = GL_FUNC_REVERSE_SUBTRACT_EXT;
const int BLENDMODE_MIN              = GL_MIN_EXT;
const int BLENDMODE_MAX              = GL_MAX_EXT;

// Depth testing constants
const int NEVER    = GL_NEVER;
const int LESS     = GL_LESS;
const int EQUAL    = GL_EQUAL;
const int LEQUAL   = GL_LEQUAL;
const int GREATER  = GL_GREATER;
const int NOTEQUAL = GL_NOTEQUAL;
const int GEQUAL   = GL_GEQUAL;
const int ALWAYS   = GL_ALWAYS;



GLenum OpenGLRenderer::targets[] = { GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_3D_EXT, GL_TEXTURE_1D };

#if defined(WIN32)

OpenGLRenderer::OpenGLRenderer(HDC hDc, HGLRC hGlrc) : Renderer(){
	hdc = hDc;
	hglrc = hGlrc;
	init();
}

#elif defined(LINUX)

#include <stdio.h>

OpenGLRenderer::OpenGLRenderer(GLXContext glXc, Display *disp, int scr) : Renderer(){
	glxc = glXc;
	display = disp;
	screen = scr;
	init();
}

#endif

void OpenGLRenderer::init(){
	byteOrderRGBA = true;
	if (GL_ARB_multitexture_supported){
		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &nSimultaneousTextures);
		if (nSimultaneousTextures > MAX_TEXTURE) nSimultaneousTextures = MAX_TEXTURE;
	} else {
		nSimultaneousTextures = 1;
	}
	maxAnisotropic = 1;
	if (GL_EXT_texture_filter_anisotropic_supported){
		glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropic);
	}

	resetToDefaults();

	setDefaultStates();
}

void OpenGLRenderer::setDefaultStates(){
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glFrontFace(GL_CW);
}

bool OpenGLRenderer::createRenderTarget(Texture &texture){
	if (isCompressedFormat(texture.format)) return false;

	int bpc = 8 * getBytesPerChannel(texture.format);
	int nCh = getChannels(texture.format);
	bool mipMapped = texture.mipmapped && GL_SGIS_generate_mipmap_supported;
	bool cubeMap = (texture.textureKind == TEXKIND_CUBEMAP);

#ifdef _WIN32

	int attList[] = {
		WGL_SUPPORT_OPENGL_ARB,       GL_TRUE, 
		WGL_DRAW_TO_PBUFFER_ARB,      GL_TRUE,
		WGL_BIND_TO_TEXTURE_RGBA_ARB, GL_TRUE,

		WGL_PIXEL_TYPE_ARB,    isFloatFormat(texture.format)? WGL_TYPE_RGBA_FLOAT_ATI : WGL_TYPE_RGBA_ARB,
		WGL_ACCELERATION_ARB,  WGL_FULL_ACCELERATION_ARB,
		
		WGL_RED_BITS_ARB,      bpc,
		WGL_GREEN_BITS_ARB,    (nCh > 1)? bpc : 0,
		WGL_BLUE_BITS_ARB,     (nCh > 2)? bpc : 0,
		WGL_ALPHA_BITS_ARB,    (nCh > 3)? bpc : 0,
		WGL_DEPTH_BITS_ARB,    24,
		WGL_STENCIL_BITS_ARB,  0,
		WGL_DOUBLE_BUFFER_ARB, GL_FALSE,
		0, 0,
	};

	int pixelFormat;
	unsigned int nFormats;
	if (!wglChoosePixelFormatARB(hdc, attList, NULL, 1, &pixelFormat, &nFormats)) return false;
	if (nFormats == 0) return false;

	int pBufAtt[] = {
		WGL_PBUFFER_LARGEST_ARB, GL_TRUE,
		WGL_TEXTURE_FORMAT_ARB,  WGL_TEXTURE_RGBA_ARB,
		WGL_TEXTURE_TARGET_ARB,  cubeMap? WGL_TEXTURE_CUBE_MAP_ARB : WGL_TEXTURE_2D_ARB,
		WGL_MIPMAP_TEXTURE_ARB,  mipMapped? GL_TRUE : GL_FALSE,
		0, 0,
	};

	HPBUFFERARB pBuffer = wglCreatePbufferARB(hdc, pixelFormat, texture.width, texture.height, pBufAtt);
	HDC hPdc = wglGetPbufferDCARB(pBuffer);
	HGLRC hPrc = wglCreateContext(hPdc);

	texture.renderTargetData = new void *[3];
	texture.renderTargetData[0] = (void *) pBuffer;
	texture.renderTargetData[1] = (void *) hPdc;
	texture.renderTargetData[2] = (void *) hPrc;

	wglShareLists(hglrc, hPrc);

	// Set some default states to this context
	wglMakeCurrent(hPdc, hPrc);
	setDefaultStates();
	wglMakeCurrent(hdc, hglrc);

#else

	int attList[] = {
		GLX_BIND_TO_TEXTURE_RGBA_ATI, True,
		GLX_RENDER_TYPE,   isFloatFormat(texture.format)? GLX_RGBA_FLOAT_ATI_BIT : GLX_RGBA_BIT,
		GLX_X_RENDERABLE,  True,
		GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT,
		GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
		GLX_CONFIG_CAVEAT, GLX_NONE,
		GLX_RED_SIZE,      bpc,
		GLX_GREEN_SIZE,    (nCh > 1)? bpc : 0,
		GLX_BLUE_SIZE,     (nCh > 2)? bpc : 0,
		GLX_ALPHA_SIZE,    (nCh > 3)? bpc : 0,
		GLX_DEPTH_SIZE,    24,
		GLX_STENCIL_SIZE,  0,
		GLX_DOUBLEBUFFER,  False,
		None
	};

	int nConfigs = 0;
	GLXFBConfig *configs = glXChooseFBConfig(display, screen, attList, &nConfigs);
	if (nConfigs == 0 || !configs){
		printf("No matching FBConfig\n");
		return false;
	}

	int bestConfig = 0;
	int minBits = 0x7FFFFFFF;
	for (int i = 0; i < nConfigs; i++){
		int red, green, blue, alpha, depth, stencil;
		glXGetFBConfigAttrib(display, configs[i], GLX_RED_SIZE,     &red);
		glXGetFBConfigAttrib(display, configs[i], GLX_GREEN_SIZE,   &green);
		glXGetFBConfigAttrib(display, configs[i], GLX_BLUE_SIZE,    &blue);
		glXGetFBConfigAttrib(display, configs[i], GLX_ALPHA_SIZE,   &alpha);
		glXGetFBConfigAttrib(display, configs[i], GLX_DEPTH_SIZE,   &depth);
		glXGetFBConfigAttrib(display, configs[i], GLX_STENCIL_SIZE, &stencil);

		int bits = red + green + blue + alpha + depth + stencil;
		if (bits < minBits){
			bestConfig = i;
			minBits = bits;
		}
	}
	
	GLXFBConfig config = configs[bestConfig];
	XFree(configs);


	int pBufAtt[] = {
		GLX_PBUFFER_WIDTH,  texture.width,
		GLX_PBUFFER_HEIGHT, texture.height,

		GLX_LARGEST_PBUFFER, True,
		GLX_TEXTURE_FORMAT_ATI, GLX_TEXTURE_RGBA_ATI,
		GLX_TEXTURE_TARGET_ATI, cubeMap? GLX_TEXTURE_CUBE_MAP_ATI : GLX_TEXTURE_2D_ATI,
		GLX_MIPMAP_TEXTURE_ATI, mipMapped? True : False,
		None
	};

	GLXPbuffer pBuffer = glXCreatePbuffer(display, config, pBufAtt);
	
	GLXContext ctx = glXCreateNewContext(display, config, GLX_RGBA_TYPE, glxc, GL_TRUE);
	if (ctx == NULL){
		printf("Couldn't create context\n");
		return false;
	}

	texture.renderTargetData = new void *[2];
	texture.renderTargetData[0] = (void *) pBuffer;
	texture.renderTargetData[1] = (void *) ctx;
	
	glXMakeCurrent(display, pBuffer, ctx);
	setDefaultStates();
	glXMakeCurrent(display, window, glxc);
	
#endif

	GLenum glTarget = cubeMap? GL_TEXTURE_CUBE_MAP_ARB : GL_TEXTURE_2D;

	glGenTextures(1, &texture.texID);
	glBindTexture(glTarget, texture.texID);

	glTexParameteri(glTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
	glTexParameteri(glTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);

	glTexParameteri(glTarget, GL_TEXTURE_MAG_FILTER, texture.noLinearFilter? GL_NEAREST : GL_LINEAR);
	if (texture.noLinearFilter){
		glTexParameteri(glTarget, GL_TEXTURE_MIN_FILTER, mipMapped? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);
	} else {
		glTexParameteri(glTarget, GL_TEXTURE_MIN_FILTER, mipMapped? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	}
	if (mipMapped) glTexParameteri(glTarget, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);

	return true;
}


bool OpenGLRenderer::createTexture(Texture &texture){
	static int type[] = { 0, GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA };
	static int internalFormats[] = {
		0,
		GL_INTENSITY,
		GL_LUMINANCE_ALPHA,
		GL_RGB8,
		GL_RGBA8,

		GL_INTENSITY16,
		0, // Unsupported
		GL_RGB16,
		GL_RGBA16,

		GL_INTENSITY_FLOAT16_ATI,
		0, // Unsupported
		GL_RGB_FLOAT16_ATI,
		GL_RGBA_FLOAT16_ATI,

		GL_INTENSITY_FLOAT32_ATI,
		0, // Unsupported
		GL_RGB_FLOAT32_ATI,
		GL_RGBA_FLOAT32_ATI,
		0, // Unsupported
		GL_COMPRESSED_RGB_S3TC_DXT1_EXT,
		GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
		GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
		GL_COMPRESSED_LUMINANCE_ALPHA_3DC_ATI,
		0, // Unsupported
		0, // Unsupported
	};


	unsigned char *src;


	GLenum target = targets[texture.textureKind];
	int nImages = 1;

	glGenTextures(1, &texture.texID);
	glBindTexture(target, texture.texID);

	switch (texture.textureKind){
	case TEXKIND_2D:
		glTexParameteri(target, GL_TEXTURE_WRAP_S, texture.clampS? GL_CLAMP_TO_EDGE_EXT : GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, texture.clampT? GL_CLAMP_TO_EDGE_EXT : GL_REPEAT);
		break;
	case TEXKIND_CUBEMAP:
		nImages = 6;
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
		break;
#ifndef NO_TEX3D
	case TEXKIND_3D:
		glTexParameteri(target, GL_TEXTURE_WRAP_S, texture.clampS? GL_CLAMP_TO_EDGE_EXT : GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, texture.clampT? GL_CLAMP_TO_EDGE_EXT : GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_R, texture.clampR? GL_CLAMP_TO_EDGE_EXT : GL_REPEAT);
		break;
#endif // NO_TEX3D
	case TEXKIND_1D:
		glTexParameteri(target, GL_TEXTURE_WRAP_S, texture.clampS? GL_CLAMP_TO_EDGE_EXT : GL_REPEAT);
		break;
	default:
		return false;
	}

	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, texture.noLinearFilter? GL_NEAREST : GL_LINEAR);

#ifndef NO_TEX3D
	if (texture.textureKind == TEXKIND_3D){
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, texture.image3D->getNumberOfMipMaps() <= 1? (texture.noLinearFilter? GL_NEAREST : GL_LINEAR) : GL_LINEAR_MIPMAP_LINEAR);

		Image3D *img = texture.image3D;
		FORMAT format = img->getFormat();
		int nChannels = getChannels(format);

		int mipMapLevel = 0;
		while ((src = img->getImage(mipMapLevel)) != NULL){
			glTexImage3DEXT(target, mipMapLevel, internalFormats[format], img->getWidth(mipMapLevel), img->getHeight(mipMapLevel), img->getDepth(mipMapLevel), 0, type[nChannels], GL_UNSIGNED_BYTE, src);
			mipMapLevel++;
		}

	} else {
#endif // NO_TEX3D
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, texture.images[0]->getNumberOfMipMaps() <= 1? (texture.noLinearFilter? GL_NEAREST : GL_LINEAR) : GL_LINEAR_MIPMAP_LINEAR);

		for (int i = 0; i < nImages; i++){
			Image *img = texture.images[i];
			FORMAT format = img->getFormat();

			if ((format >= FORMAT_DXT1 && format <= FORMAT_DXT5 && !(GL_ARB_texture_compression_supported && GL_EXT_texture_compression_s3tc_supported)) ||
				(format == FORMAT_3DC && !GL_ATI_texture_compression_3dc_supported)){
				img->uncompressImage();
				format = img->getFormat();
			}

			GLenum imgTarget = (texture.textureKind == TEXKIND_2D)? GL_TEXTURE_2D : (texture.textureKind == TEXKIND_1D)? GL_TEXTURE_1D : GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + i;
			if (isPlainFormat(format)){
				int nChannels = getChannels(format);

				int mipMapLevel = 0;
				while ((src = img->getImage(mipMapLevel)) != NULL){
					if (texture.textureKind == TEXKIND_1D){
						glTexImage1D(imgTarget, mipMapLevel, internalFormats[format], img->getWidth(mipMapLevel), 0, type[nChannels], GL_UNSIGNED_BYTE, src);
					} else {
						glTexImage2D(imgTarget, mipMapLevel, internalFormats[format], img->getWidth(mipMapLevel), img->getHeight(mipMapLevel), 0, type[nChannels], GL_UNSIGNED_BYTE, src);
					}
					mipMapLevel++;
				}
			} else {
				int mipMapLevel = 0;
				while ((src = img->getImage(mipMapLevel)) != NULL){
					if (texture.textureKind == TEXKIND_1D){
						glCompressedTexImage1DARB(imgTarget, mipMapLevel, internalFormats[format], img->getWidth(mipMapLevel), 0, img->getMipMappedSize(mipMapLevel, 1), src);
					} else {
						glCompressedTexImage2DARB(imgTarget, mipMapLevel, internalFormats[format], img->getWidth(mipMapLevel), img->getHeight(mipMapLevel), 0, img->getMipMappedSize(mipMapLevel, 1), src);
					}
					mipMapLevel++;
				}
			}
		}
#ifndef NO_TEX3D
	}
#endif

	return true;
}

void OpenGLRenderer::removeTexture(Texture &texture){
	if (texture.isRenderTarget){
#ifdef _WIN32
		wglDeleteContext((HGLRC) texture.renderTargetData[2]);
		wglReleasePbufferDCARB((HPBUFFERARB) texture.renderTargetData[0], (HDC) texture.renderTargetData[1]);
		wglDestroyPbufferARB((HPBUFFERARB) texture.renderTargetData[0]);
#else
		glXDestroyContext(display, (GLXContext) texture.renderTargetData[1]);
		glXDestroyPbuffer(display, (GLXPbuffer) texture.renderTargetData[0]);
#endif
		delete texture.renderTargetData;
	}
	glDeleteTextures(1, &texture.texID);
}

void OpenGLRenderer::resetTextureUnits(){
	for (int i = 0; i < nSimultaneousTextures; i++){
		changeTextureUnit(i);

		glBindTexture(GL_TEXTURE_1D, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		if (GL_EXT_texture3D_supported) glBindTexture(GL_TEXTURE_3D_EXT, 0);
		if (GL_ARB_texture_cube_map_supported) glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, 0);
	}
}

bool OpenGLRenderer::createShader(Shader &shader, const char *vertexShaderText, const char *fragmentShaderText){
	const char *shaderStrings[2];
	int vResult, fResult, lResult;
	char str[4096];

	shaderStrings[0] =
		"#define saturate(x) clamp(x,0.0,1.0)\n"
		"#define lerp mix\n";

	shader.programObj = glCreateProgramObjectARB();

	shader.vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	shaderStrings[1] = vertexShaderText;
	glShaderSourceARB(shader.vertexShader, 2, shaderStrings, NULL);
	glCompileShaderARB(shader.vertexShader);
	glGetObjectParameterivARB(shader.vertexShader, GL_OBJECT_COMPILE_STATUS_ARB, &vResult);
	if (vResult){
		glAttachObjectARB(shader.programObj, shader.vertexShader);
	} else {
		addToLog("Vertex shader error:\n");
	}
	glGetInfoLogARB(shader.vertexShader, sizeof(str), NULL, str);
	addToLog(str);


	shader.fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	shaderStrings[1] = fragmentShaderText;
	glShaderSourceARB(shader.fragmentShader, 2, shaderStrings, NULL);
	glCompileShaderARB(shader.fragmentShader);
	glGetObjectParameterivARB(shader.fragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &fResult);
	if (fResult){
		glAttachObjectARB(shader.programObj, shader.fragmentShader);
	} else {
		addToLog("Fragment shader error:\n");
	}
	glGetInfoLogARB(shader.fragmentShader, sizeof(str), NULL, str);
	addToLog(str);


	if (vResult && fResult){
		glLinkProgramARB(shader.programObj);
		glGetObjectParameterivARB(shader.programObj, GL_OBJECT_LINK_STATUS_ARB, &lResult);

		glGetInfoLogARB(shader.programObj, sizeof(str), NULL, str);
		addToLog(str);

		if (lResult) return true;
	}

	removeShader(shader);
	return false;
}

void OpenGLRenderer::removeShader(Shader &shader){
	glDeleteObjectARB(shader.vertexShader);
	glDeleteObjectARB(shader.fragmentShader);
	glDeleteObjectARB(shader.programObj);
}

void OpenGLRenderer::changeRenderTarget(const TextureID colorTarget, const TextureID depthTarget, const int face){
	if (colorTarget != currentColorTarget){
		reset();
		apply();

		// Change active rendering context
#ifdef _WIN32
		if (colorTarget != TEXTURE_NONE){
			wglMakeCurrent((HDC) textures[colorTarget].renderTargetData[1], (HGLRC) textures[colorTarget].renderTargetData[2]);
		} else {
			wglMakeCurrent(hdc, hglrc);
		}
#else // Linux
		if (colorTarget != TEXTURE_NONE){
			glXMakeCurrent(display, (GLXPbuffer) textures[colorTarget].renderTargetData[0], (GLXContext) textures[colorTarget].renderTargetData[1]);
		} else {
			glXMakeCurrent(display, window, glxc);
		}
#endif
		currentColorTarget = colorTarget;
	}
}

void OpenGLRenderer::changeToDefaultRenderTarget(){
	changeRenderTarget(TEXTURE_NONE, TEXTURE_NONE);
}

void OpenGLRenderer::assignNamedTextures(){
	if (currentShader != SHADER_NONE){
		for (int i = 0; i < nNamedTextures; i++){
			int location = glGetUniformLocationARB(shaders[currentShader].programObj, selectedTextureNames[i]);
			if (location >= 0) glUniform1iARB(location, i);
		}
	}
}

void OpenGLRenderer::changeTextureUnit(const int textureUnit){
	if (textureUnit != currentTextureUnit){
		glActiveTextureARB(GL_TEXTURE0_ARB + (currentTextureUnit = textureUnit));
	}
}

void OpenGLRenderer::changeLod(const int textureUnit, const float lod){
	changeTextureUnit(textureUnit);
	glTexEnvf(GL_TEXTURE_FILTER_CONTROL_EXT, GL_TEXTURE_LOD_BIAS_EXT, lod);
}

void OpenGLRenderer::changeTexture(const TextureID textureID){
	TextureID currTex = currentTextures[currentTextureUnit];

	if (textureID != currTex){
		if (currTex != TEXTURE_NONE && textures[currTex].isRenderTarget){
#ifdef _WIN32
			wglReleaseTexImageARB((HPBUFFERARB) textures[currTex].renderTargetData[0], WGL_FRONT_LEFT_ARB);
#else
			glXReleaseTexImageATI(display, (GLXPbuffer) textures[currTex].renderTargetData[0], GLX_FRONT_LEFT_ATI);
#endif
		}

		if (textureID == TEXTURE_NONE){
			glDisable(targets[textures[currTex].textureKind]);
		} else {
			GLenum target = targets[textures[textureID].textureKind];

			if (currTex == TEXTURE_NONE){
				glEnable(target);
			} else {
				if (textures[textureID].textureKind != textures[currTex].textureKind){
					glDisable(targets[textures[currTex].textureKind]);
					glEnable(target);
				}
			}

			glBindTexture(target, textures[textureID].texID);
			if (textures[textureID].isRenderTarget){
#ifdef _WIN32
				wglBindTexImageARB((HPBUFFERARB) textures[textureID].renderTargetData[0], WGL_FRONT_LEFT_ARB);
#else
				glXBindTexImageATI(display, (GLXPbuffer) textures[textureID].renderTargetData[0], GLX_FRONT_LEFT_ATI);
#endif
			}
		}
		currentTextures[currentTextureUnit] = textureID;
	}
}

void OpenGLRenderer::configureTexEnv(const TexEnv texEnv, bool isRGB){
	int combineMode, source0, source1, source2;
	int operand0, operand1, operand2, srcColor;
	if (isRGB){
		combineMode = GL_COMBINE_RGB_ARB;
		source0     = GL_SOURCE0_RGB_ARB;
		operand0    = GL_OPERAND0_RGB_ARB;
		srcColor    = GL_SRC_COLOR;
	} else {
		combineMode = GL_COMBINE_ALPHA_ARB;
		source0     = GL_SOURCE0_ALPHA_ARB;
		operand0    = GL_OPERAND0_ALPHA_ARB;
		srcColor    = GL_SRC_ALPHA;
	}
	source1  = source0  + 1;
	source2  = source1  + 1;
	operand1 = operand0 + 1;
	operand2 = operand1 + 1;

	switch(texEnv){
	case TEXENV_MODULATE:
		glTexEnvi(GL_TEXTURE_ENV, combineMode, GL_MODULATE);

		glTexEnvi(GL_TEXTURE_ENV, source0,  GL_PREVIOUS_ARB);
		glTexEnvi(GL_TEXTURE_ENV, operand0, srcColor);
		
		glTexEnvi(GL_TEXTURE_ENV, source1,  GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, operand1, srcColor);
		break;
	case TEXENV_DOT3:
		if (isRGB){
			glTexEnvi(GL_TEXTURE_ENV, combineMode, GL_DOT3_RGBA_ARB);
			
			glTexEnvi(GL_TEXTURE_ENV, source0,  GL_PREVIOUS_ARB);
			glTexEnvi(GL_TEXTURE_ENV, operand0, srcColor);
			
			glTexEnvi(GL_TEXTURE_ENV, source1,  GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, operand1, srcColor);
		}
		break;
	case TEXENV_DOT3_NEXT:
		if (isRGB){
			glTexEnvi(GL_TEXTURE_ENV, combineMode, GL_DOT3_RGBA_ARB);
			
			glTexEnvi(GL_TEXTURE_ENV, source0,  GL_TEXTURE0_ARB + currentTextureUnit);
			glTexEnvi(GL_TEXTURE_ENV, operand0, srcColor);
			
			glTexEnvi(GL_TEXTURE_ENV, source1,  GL_TEXTURE0_ARB + currentTextureUnit + 1);
			glTexEnvi(GL_TEXTURE_ENV, operand1, srcColor);
		}
		break;
	case TEXENV_ADD:
		glTexEnvi(GL_TEXTURE_ENV, combineMode, GL_ADD);

		glTexEnvi(GL_TEXTURE_ENV, source0,  GL_PREVIOUS_ARB);
		glTexEnvi(GL_TEXTURE_ENV, operand0, srcColor);
		
		glTexEnvi(GL_TEXTURE_ENV, source1,  GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, operand1, srcColor);
		break;
	case TEXENV_REPLACE:
		glTexEnvi(GL_TEXTURE_ENV, combineMode, GL_REPLACE);

		glTexEnvi(GL_TEXTURE_ENV, source0,  GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, operand0, srcColor);
		break;
	case TEXENV_PASSTHRU:
		glTexEnvi(GL_TEXTURE_ENV, combineMode, GL_REPLACE);

		glTexEnvi(GL_TEXTURE_ENV, source0,  GL_PREVIOUS_ARB);
		glTexEnvi(GL_TEXTURE_ENV, operand0, srcColor);
		break;
	case TEXENV_INTERPOLATE:
		glTexEnvi(GL_TEXTURE_ENV, combineMode, GL_INTERPOLATE_ARB);
		
		glTexEnvi(GL_TEXTURE_ENV, source0,  GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, operand0, srcColor);

		glTexEnvi(GL_TEXTURE_ENV, source1,  GL_PREVIOUS_ARB);
		glTexEnvi(GL_TEXTURE_ENV, operand1, srcColor);
		
		glTexEnvi(GL_TEXTURE_ENV, source2,  GL_PRIMARY_COLOR_ARB);
		glTexEnvi(GL_TEXTURE_ENV, operand2, srcColor);
		break;
	case TEXENV_INTERPOLATE_PRIMARY:
		glTexEnvi(GL_TEXTURE_ENV, combineMode, GL_INTERPOLATE_ARB);
		
		glTexEnvi(GL_TEXTURE_ENV, source0,  GL_PRIMARY_COLOR_ARB);
		glTexEnvi(GL_TEXTURE_ENV, operand0, srcColor);
		
		glTexEnvi(GL_TEXTURE_ENV, source1,  GL_PREVIOUS_ARB);
		glTexEnvi(GL_TEXTURE_ENV, operand1, srcColor);

		glTexEnvi(GL_TEXTURE_ENV, source2,  GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, operand2, srcColor);
		break;
	case TEXENV_INTERPOLATE_TEX_ALPHA:
		glTexEnvi(GL_TEXTURE_ENV, combineMode, GL_INTERPOLATE_ARB);
		
		glTexEnvi(GL_TEXTURE_ENV, source0,  GL_PREVIOUS_ARB);
		glTexEnvi(GL_TEXTURE_ENV, operand0, srcColor);
		
		glTexEnvi(GL_TEXTURE_ENV, source1,  GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, operand1, srcColor);

		glTexEnvi(GL_TEXTURE_ENV, source2,  GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, operand2, GL_SRC_ALPHA);
		break;
	default:
		break;
	}
}

void OpenGLRenderer::changeTexEnv(const TexEnv colorTexEnv, const TexEnv alphaTexEnv){
	if (colorTexEnv != currentColorTexEnvs[currentTextureUnit] || 
		alphaTexEnv != currentAlphaTexEnvs[currentTextureUnit]){

		bool handled;
		// Special case for compatibility
		if (handled = (colorTexEnv == alphaTexEnv)){
			switch(colorTexEnv){
			case TEXENV_MODULATE:
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				break;
			case TEXENV_ADD:
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
				break;
			case TEXENV_REPLACE:
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				break;
			default:
				handled = false;
			}
		}

		if (!handled){
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
			if (colorTexEnv != currentColorTexEnvs[currentTextureUnit]){
				configureTexEnv(colorTexEnv, true);
			}

			if (alphaTexEnv != currentAlphaTexEnvs[currentTextureUnit]){
				configureTexEnv(alphaTexEnv, false);
			}
		}

		currentColorTexEnvs[currentTextureUnit] = colorTexEnv;
		currentAlphaTexEnvs[currentTextureUnit] = alphaTexEnv;
	}
}

void OpenGLRenderer::changeShader(const ShaderID shader){
	if (shader != currentShader){
		if (shader == SHADER_NONE){
			glUseProgramObjectARB(0);
		} else {
			glUseProgramObjectARB(shaders[shader].programObj);
		}
		currentShader = shader;
	}
}

void OpenGLRenderer::changeShaderConstant1f(const char *name, const float constant){
	if (currentShader != SHADER_NONE){
		int location = glGetUniformLocationARB(shaders[currentShader].programObj, name);
		if (location >= 0) glUniform1fARB(location, constant);
	}
}

void OpenGLRenderer::changeShaderConstant2f(const char *name, const vec2 &constant){
	if (currentShader != SHADER_NONE){
		int location = glGetUniformLocationARB(shaders[currentShader].programObj, name);
		if (location >= 0) glUniform2fvARB(location, 1, (float *) &constant);
	}
}

void OpenGLRenderer::changeShaderConstant3f(const char *name, const vec3 &constant){
	if (currentShader != SHADER_NONE){
		int location = glGetUniformLocationARB(shaders[currentShader].programObj, name);
		if (location >= 0) glUniform3fvARB(location, 1, (float *) &constant);
	}
}

void OpenGLRenderer::changeShaderConstant4f(const char *name, const vec4 &constant){
	if (currentShader != SHADER_NONE){
		int location = glGetUniformLocationARB(shaders[currentShader].programObj, name);
		if (location >= 0) glUniform4fvARB(location, 1, (float *) &constant);
	}
}

void OpenGLRenderer::changeShaderConstant3x3f(const char *name, const mat3 &constant){
	if (currentShader != SHADER_NONE){
		int location = glGetUniformLocationARB(shaders[currentShader].programObj, name);
		if (location >= 0) glUniformMatrix3fvARB(location, 1, GL_TRUE, (float *) &constant);
	}
}

void OpenGLRenderer::changeShaderConstant4x4f(const char *name, const mat4 &constant){
	if (currentShader != SHADER_NONE){
		int location = glGetUniformLocationARB(shaders[currentShader].programObj, name);
		if (location >= 0) glUniformMatrix4fvARB(location, 1, GL_TRUE, (float *) &constant);
	}
}

void OpenGLRenderer::changeShaderConstantArray3f(const char *name, const vec3 *constant, const unsigned int count){
	if (currentShader != SHADER_NONE){
		int location = glGetUniformLocationARB(shaders[currentShader].programObj, name);
		if (location >= 0) glUniform3fvARB(location, count, (float *) constant);
	}
}

void OpenGLRenderer::changeShaderConstantArray4f(const char *name, const vec4 *constant, const unsigned int count){
	if (currentShader != SHADER_NONE){
		int location = glGetUniformLocationARB(shaders[currentShader].programObj, name);
		if (location >= 0) glUniform4fvARB(location, count, (float *) constant);
	}
}

bool OpenGLRenderer::changeShaderAttribs(const ShaderID shader, const char **names, const unsigned int first, const unsigned int count){
	for (unsigned int i = 0; i < count; i++){
		glBindAttribLocationARB(shaders[shader].programObj, first + i, names[i]);
	}
	glLinkProgramARB(shaders[shader].programObj);
	GLint linkResult;
	glGetObjectParameterivARB(shaders[shader].programObj, GL_OBJECT_LINK_STATUS_ARB, &linkResult);

	if (!linkResult){
		char str[1024];
		glGetInfoLogARB(shaders[shader].programObj, sizeof(str), NULL, str);
		addToLog(str);
		return false;
	}

	return true;
}

void OpenGLRenderer::changeBlending(const int srcFactor, const int destFactor, const int blendMode){
	if (srcFactor != BLENDING_NONE){
		if (currentSrcFactor == BLENDING_NONE){
			glEnable(GL_BLEND);
		}
		if (srcFactor != currentSrcFactor || destFactor != currentDestFactor){
			glBlendFunc(currentSrcFactor = srcFactor, currentDestFactor = destFactor);
		}
		if (blendMode != currentBlendMode){
			glBlendEquationEXT(currentBlendMode = blendMode);
		}
	} else {
		if (currentSrcFactor != BLENDING_NONE){
			glDisable(GL_BLEND);
			currentSrcFactor = BLENDING_NONE;
		}
	}
}

void OpenGLRenderer::changeDepthFunc(const int depthFunc){
	if (depthFunc != currentDepthFunc){
		if (depthFunc == DEPTH_NONE){
			glDisable(GL_DEPTH_TEST);
		} else {
			if (currentDepthFunc == DEPTH_NONE){
				glEnable(GL_DEPTH_TEST);
			}
			glDepthFunc(depthFunc);
		}
		currentDepthFunc = depthFunc;
	}
}

void OpenGLRenderer::changeMask(const int mask){
	if (mask != currentMask){
		if ((mask & DEPTH) != (currentMask & DEPTH)){
			glDepthMask(mask & DEPTH);
		}
		if ((mask & COLOR) != (currentMask & COLOR)){
			glColorMask(((mask & RED) >> 1) & 1, ((mask & GREEN) >> 2) & 1, ((mask & BLUE) >> 3) & 1, ((mask & ALPHA) >> 4) & 1);
		}

		currentMask = mask;
	}
}


void OpenGLRenderer::changeVSync(const bool value){
#if defined(WIN32)
	if (WGL_EXT_swap_control_supported) wglSwapIntervalEXT(value);
#elif defined(LINUX)

#endif
}

void OpenGLRenderer::changeAnisotropic(const TextureID textureID, const bool value){
	if (GL_EXT_texture_filter_anisotropic_supported){
		if (textures[textureID].textureKind == TEXKIND_2D){
			glBindTexture(GL_TEXTURE_2D, textures[textureID].texID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, textures[textureID].levelOfAnisotropy);
		
			//glBindTexture(GL_TEXTURE_2D, textures[currentTextureUnit].texID);
		}
	}
}
