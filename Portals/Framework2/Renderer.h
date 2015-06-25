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

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "Globals.h"
#include "Util/Set.h"
#include "Math/Vector.h"
#include "Imaging/Image.h"
#ifndef NO_TEX3D
#include "Imaging/Image3D.h"
#endif

typedef int TextureID;
typedef int ShaderID;

#define TEXTURE_NONE  (-1)
#define BLENDING_NONE (-1)
#define DEPTH_NONE    (-1)
#define SHADER_NONE   (-1)

#define RT_NONE (-1)
#define RT_FB   (-2)

#define TEXTURE_UNDEFINED 0
#define SHADER_UNDEFINED  0

#define MAX_TEXTURE 16

enum TEXKIND {
	TEXKIND_2D      = 0,
	TEXKIND_CUBEMAP = 1,
#ifndef NO_TEX3D
	TEXKIND_3D      = 2,
#endif
	TEXKIND_1D      = 3,
};

struct Texture {
	unsigned int texID;
	TEXKIND textureKind;

	unsigned int width, height, depth;
	FORMAT format;

	union {
		Image *images[6];
#ifndef NO_TEX3D
		Image3D *image3D;
#endif
		void **renderTargetData;
	};
	int levelOfAnisotropy;
	bool clampS, clampT, clampR;
	bool mipmapped;
	bool noLinearFilter;
	bool isRenderTarget;
	bool rtLockScreenSize;
};

struct Shader {
	unsigned int programObj;
	unsigned int vertexShader;
	unsigned int fragmentShader;
	void *vertexShaderConstants;
	void *fragmentShaderConstants;
};

enum TexEnv {
	TEXENV_NONE,
	TEXENV_MODULATE,
	TEXENV_DOT3,
	TEXENV_DOT3_NEXT,
	TEXENV_ADD,
	TEXENV_REPLACE,
	TEXENV_PASSTHRU,
	TEXENV_INTERPOLATE,
	TEXENV_INTERPOLATE_PRIMARY,
	TEXENV_INTERPOLATE_TEX_ALPHA,
};

// Blending constants
extern const int ZERO;
extern const int ONE;
extern const int SRC_COLOR;
extern const int ONE_MINUS_SRC_COLOR;
extern const int DST_COLOR;
extern const int ONE_MINUS_DST_COLOR;
extern const int SRC_ALPHA;
extern const int ONE_MINUS_SRC_ALPHA;
extern const int DST_ALPHA;
extern const int ONE_MINUS_DST_ALPHA;
extern const int SRC_ALPHA_SATURATE;

extern const int BLENDMODE_ADD;
extern const int BLENDMODE_SUBTRACT;
extern const int BLENDMODE_REVERSE_SUBTRACT;
extern const int BLENDMODE_MIN;
extern const int BLENDMODE_MAX;

// Depth testing constants
extern const int NEVER;
extern const int LESS;
extern const int EQUAL;
extern const int LEQUAL;
extern const int GREATER;
extern const int NOTEQUAL;
extern const int GEQUAL;
extern const int ALWAYS;

// Mask constants
#define DEPTH 0x01
#define RED   0x02
#define GREEN 0x04
#define BLUE  0x08
#define ALPHA 0x10

#define COLOR (RED | GREEN | BLUE | ALPHA)
#define ALL (DEPTH | COLOR)
#define NONE 0

// Texture flags
#define TEX_NORMALMAP       0x1
#define TEX_HEIGHTMAP       0x2
#define TEX_NORMALHEIGHTMAP (TEX_NORMALMAP | TEX_HEIGHTMAP)
#define TEX_NOMIPMAPPING    0x4
#define TEX_NOLINEARFILTER  0x8
#define TEX_NOANISO         0x10

#define TEX_CLAMP_S      0x100
#define TEX_CLAMP_T      0x200
#define TEX_CLAMP_R      0x400
#define TEX_CLAMP (TEX_CLAMP_S | TEX_CLAMP_T | TEX_CLAMP_R)

#define TEX_1D 0x800

/* -------------------------------------------------------------------------------------------- */

class Renderer {
protected:
	Set <Texture> textures;
	int nSimultaneousTextures;
	int currentTextureUnit;
	int maxAnisotropic;
	TextureID selectedTextures[MAX_TEXTURE];
	TextureID currentTextures[MAX_TEXTURE];
	TexEnv selectedColorTexEnvs[MAX_TEXTURE];
	TexEnv currentColorTexEnvs[MAX_TEXTURE];
	TexEnv selectedAlphaTexEnvs[MAX_TEXTURE];
	TexEnv currentAlphaTexEnvs[MAX_TEXTURE];
	
	const char *selectedTextureNames[MAX_TEXTURE];
	int nNamedTextures;

	Set <Shader> shaders;
	ShaderID currentShader, selectedShader;

	TextureID currentColorTarget, currentDepthTarget, currentTargetFace;


	int currentSrcFactor,  selectedSrcFactor;
	int currentDestFactor, selectedDestFactor;
	int currentBlendMode,  selectedBlendMode;

	int currentDepthFunc, selectedDepthFunc;
	int currentMask, selectedMask;

	bool byteOrderRGBA;
	bool colorMipMaps;
	bool useAnisotropic;

	virtual bool createRenderTarget(Texture &texture) = 0;

	virtual bool createTexture(Texture &texture) = 0;
	virtual void removeTexture(Texture &texture) = 0;
	virtual void resetTextureUnits() = 0;

	virtual bool createShader(Shader &shader, const char *vertexShaderText, const char *fragmentShaderText) = 0;
	virtual void removeShader(Shader &shader) = 0;

public:
	Renderer();
	virtual ~Renderer(){};

	virtual void setDefaultStates() = 0;
	void resetToDefaults();
	void reset();
	void apply();

	int getSimultaneousTextures() const { return nSimultaneousTextures; }

	void setAnisotropic(const bool value){ useAnisotropic = value; }
	void setMipMapColoring(const bool value){ colorMipMaps = value; }

	void clear();
	TextureID addTexture(Image *img, unsigned int flags = 0);
	TextureID addTexture(const char *fileName, unsigned int flags = 0);

	TextureID addCubemap(Image *imgs[6]);
	TextureID addCubemap(const char *fileNegX, const char *filePosX, const char *fileNegY, const char *filePosY, const char *fileNegZ, const char *filePosZ);

#ifndef NO_TEX3D
	TextureID addTexture3D(Image3D *img, unsigned int flags = 0);
	TextureID addTexture3D(const char *fileName, unsigned int flags = 0);
#endif

	TextureID addRenderTexture(const int width, const int height, const FORMAT format = FORMAT_RGBA8, bool mipmapped = false, bool lockToScreenSize = false);
	TextureID addRenderCubemap(const int size, const FORMAT format = FORMAT_RGBA8, bool mipmapped = false);
	TextureID addRenderDepth(const int width, const int height, const FORMAT format, bool lockToScreenSize = false);

	ShaderID addShader(const char *fileName, const char *extraDefines = NULL);
protected:
	TextureID insertTexture(Texture &texture);
	void deleteTexture(const int index);

	ShaderID insertShader(Shader &shader);
	void deleteShader(const int index);

public:
//	TextureID getNextTexture(TextureID currentTexture) const;

	virtual void changeRenderTarget(const TextureID colorTarget, const TextureID depthTarget, const int face = 0) = 0;
	virtual void changeToDefaultRenderTarget() = 0;

	inline void setTexture(const char *name, const TextureID texID);

	inline void setTextures(const TextureID tex0);
	inline void setTextures(const TextureID tex0, const TextureID tex1);
	inline void setTextures(const TextureID tex0, const TextureID tex1, const TextureID tex2);
	inline void setTextures(const TextureID tex0, const TextureID tex1, const TextureID tex2, const TextureID tex3);
	inline void setTextures(const TextureID tex0, const TextureID tex1, const TextureID tex2, const TextureID tex3, const TextureID tex4);
	inline void setTextures(const TextureID tex0, const TextureID tex1, const TextureID tex2, const TextureID tex3, const TextureID tex4, const TextureID tex5);

	inline void setTexEnvs(const TexEnv texEnv0);
	inline void setTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1);
	inline void setTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1, const TexEnv texEnv2);
	inline void setTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1, const TexEnv texEnv2, const TexEnv texEnv3);
	inline void setTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1, const TexEnv texEnv2, const TexEnv texEnv3, const TexEnv texEnv4);

	inline void setColorTexEnvs(const TexEnv texEnv0);
	inline void setColorTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1);
	inline void setColorTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1, const TexEnv texEnv2);
	inline void setColorTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1, const TexEnv texEnv2, const TexEnv texEnv3);
	inline void setColorTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1, const TexEnv texEnv2, const TexEnv texEnv3, const TexEnv texEnv4);

	inline void setAlphaTexEnvs(const TexEnv texEnv0);
	inline void setAlphaTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1);
	inline void setAlphaTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1, const TexEnv texEnv2);
	inline void setAlphaTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1, const TexEnv texEnv2, const TexEnv texEnv3);
	inline void setAlphaTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1, const TexEnv texEnv2, const TexEnv texEnv3, const TexEnv texEnv4);

	inline void setShader(const ShaderID shader);

	inline void setBlending(const int srcFactor, const int destFactor, const int blendMode = BLENDMODE_ADD);
	inline void setDepthFunc(const int depthFunc);
	inline void setMask(const int mask);

	virtual void assignNamedTextures() = 0;
	virtual void changeTextureUnit(const int textureUnit) = 0;
	virtual void changeLod(const int textureUnit, const float lod) = 0;
	virtual void changeTexture(const TextureID textureID) = 0;
	virtual void changeTexEnv(const TexEnv colorTexEnv, const TexEnv alphaTexEnv) = 0;

	virtual void changeShader(const ShaderID shader) = 0;
	virtual void changeShaderConstant1f(const char *name, const float constant) = 0;
	virtual void changeShaderConstant2f(const char *name, const vec2 &constant) = 0;
	virtual void changeShaderConstant3f(const char *name, const vec3 &constant) = 0;
	virtual void changeShaderConstant4f(const char *name, const vec4 &constant) = 0;
	virtual void changeShaderConstant3x3f(const char *name, const mat3 &constant) = 0;
	virtual void changeShaderConstant4x4f(const char *name, const mat4 &constant) = 0;
	virtual void changeShaderConstantArray3f(const char *name, const vec3 *constant, const unsigned int count) = 0;
	virtual void changeShaderConstantArray4f(const char *name, const vec4 *constant, const unsigned int count) = 0;

	virtual void changeBlending(const int srcFactor, const int destFactor, const int blendMode = BLENDMODE_ADD) = 0;
	virtual void changeDepthFunc(const int depthFunc) = 0;
	virtual void changeMask(const int mask) = 0;
	virtual void changeVSync(const bool value) = 0;
	virtual void changeAnisotropic(const TextureID textureID, const bool value) = 0;
};

/* -------------------------------------------------------------------------------------------- */

inline void Renderer::setTexture(const char *name, const TextureID texID){
	selectedTextureNames[nNamedTextures] = name;
	selectedTextures[nNamedTextures] = texID;
	nNamedTextures++;
}

inline void Renderer::setTextures(const TextureID tex0){
	selectedTextures[0] = tex0;
}

inline void Renderer::setTextures(const TextureID tex0, const TextureID tex1){
	selectedTextures[0] = tex0;
	selectedTextures[1] = tex1;
}

inline void Renderer::setTextures(const TextureID tex0, const TextureID tex1, const TextureID tex2){
	selectedTextures[0] = tex0;
	selectedTextures[1] = tex1;
	selectedTextures[2] = tex2;
}

inline void Renderer::setTextures(const TextureID tex0, const TextureID tex1, const TextureID tex2, const TextureID tex3){
	selectedTextures[0] = tex0;
	selectedTextures[1] = tex1;
	selectedTextures[2] = tex2;
	selectedTextures[3] = tex3;
}

inline void Renderer::setTextures(const TextureID tex0, const TextureID tex1, const TextureID tex2, const TextureID tex3, const TextureID tex4){
	selectedTextures[0] = tex0;
	selectedTextures[1] = tex1;
	selectedTextures[2] = tex2;
	selectedTextures[3] = tex3;
	selectedTextures[4] = tex4;
}

inline void Renderer::setTextures(const TextureID tex0, const TextureID tex1, const TextureID tex2, const TextureID tex3, const TextureID tex4, const TextureID tex5){
	selectedTextures[0] = tex0;
	selectedTextures[1] = tex1;
	selectedTextures[2] = tex2;
	selectedTextures[3] = tex3;
	selectedTextures[4] = tex4;
	selectedTextures[5] = tex5;
}

inline void Renderer::setTexEnvs(const TexEnv texEnv0){
	selectedColorTexEnvs[0] = texEnv0;
	selectedAlphaTexEnvs[0] = texEnv0;
}

inline void Renderer::setColorTexEnvs(const TexEnv texEnv0){
	selectedColorTexEnvs[0] = texEnv0;
}

inline void Renderer::setAlphaTexEnvs(const TexEnv texEnv0){
	selectedAlphaTexEnvs[0] = texEnv0;
}


inline void Renderer::setTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1){
	selectedColorTexEnvs[0] = texEnv0;
	selectedColorTexEnvs[1] = texEnv1;
	selectedAlphaTexEnvs[0] = texEnv0;
	selectedAlphaTexEnvs[1] = texEnv1;
}

inline void Renderer::setColorTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1){
	selectedColorTexEnvs[0] = texEnv0;
	selectedColorTexEnvs[1] = texEnv1;
}

inline void Renderer::setAlphaTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1){
	selectedAlphaTexEnvs[0] = texEnv0;
	selectedAlphaTexEnvs[1] = texEnv1;
}


inline void Renderer::setTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1, const TexEnv texEnv2){
	selectedColorTexEnvs[0] = texEnv0;
	selectedColorTexEnvs[1] = texEnv1;
	selectedColorTexEnvs[2] = texEnv2;
	selectedAlphaTexEnvs[0] = texEnv0;
	selectedAlphaTexEnvs[1] = texEnv1;
	selectedAlphaTexEnvs[2] = texEnv2;
}

inline void Renderer::setColorTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1, const TexEnv texEnv2){
	selectedColorTexEnvs[0] = texEnv0;
	selectedColorTexEnvs[1] = texEnv1;
	selectedColorTexEnvs[2] = texEnv2;
}

inline void Renderer::setAlphaTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1, const TexEnv texEnv2){
	selectedAlphaTexEnvs[0] = texEnv0;
	selectedAlphaTexEnvs[1] = texEnv1;
	selectedAlphaTexEnvs[2] = texEnv2;
}


inline void Renderer::setTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1, const TexEnv texEnv2, const TexEnv texEnv3){
	selectedColorTexEnvs[0] = texEnv0;
	selectedColorTexEnvs[1] = texEnv1;
	selectedColorTexEnvs[2] = texEnv2;
	selectedColorTexEnvs[3] = texEnv3;
	selectedAlphaTexEnvs[0] = texEnv0;
	selectedAlphaTexEnvs[1] = texEnv1;
	selectedAlphaTexEnvs[2] = texEnv2;
	selectedAlphaTexEnvs[3] = texEnv3;
}

inline void Renderer::setColorTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1, const TexEnv texEnv2, const TexEnv texEnv3){
	selectedColorTexEnvs[0] = texEnv0;
	selectedColorTexEnvs[1] = texEnv1;
	selectedColorTexEnvs[2] = texEnv2;
	selectedColorTexEnvs[3] = texEnv3;
}

inline void Renderer::setAlphaTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1, const TexEnv texEnv2, const TexEnv texEnv3){
	selectedAlphaTexEnvs[0] = texEnv0;
	selectedAlphaTexEnvs[1] = texEnv1;
	selectedAlphaTexEnvs[2] = texEnv2;
	selectedAlphaTexEnvs[3] = texEnv3;
}


inline void Renderer::setTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1, const TexEnv texEnv2, const TexEnv texEnv3, const TexEnv texEnv4){
	selectedColorTexEnvs[0] = texEnv0;
	selectedColorTexEnvs[1] = texEnv1;
	selectedColorTexEnvs[2] = texEnv2;
	selectedColorTexEnvs[3] = texEnv3;
	selectedColorTexEnvs[4] = texEnv4;
	selectedAlphaTexEnvs[0] = texEnv0;
	selectedAlphaTexEnvs[1] = texEnv1;
	selectedAlphaTexEnvs[2] = texEnv2;
	selectedAlphaTexEnvs[3] = texEnv3;
	selectedAlphaTexEnvs[4] = texEnv4;
}

inline void Renderer::setColorTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1, const TexEnv texEnv2, const TexEnv texEnv3, const TexEnv texEnv4){
	selectedColorTexEnvs[0] = texEnv0;
	selectedColorTexEnvs[1] = texEnv1;
	selectedColorTexEnvs[2] = texEnv2;
	selectedColorTexEnvs[3] = texEnv3;
	selectedColorTexEnvs[4] = texEnv4;
}

inline void Renderer::setAlphaTexEnvs(const TexEnv texEnv0, const TexEnv texEnv1, const TexEnv texEnv2, const TexEnv texEnv3, const TexEnv texEnv4){
	selectedAlphaTexEnvs[0] = texEnv0;
	selectedAlphaTexEnvs[1] = texEnv1;
	selectedAlphaTexEnvs[2] = texEnv2;
	selectedAlphaTexEnvs[3] = texEnv3;
	selectedAlphaTexEnvs[4] = texEnv4;
}

inline void Renderer::setShader(const ShaderID shader){
	selectedShader = shader;
}

inline void Renderer::setBlending(const int srcFactor, const int destFactor, const int blendMode){
	selectedSrcFactor  = srcFactor;
	selectedDestFactor = destFactor;
	selectedBlendMode  = blendMode;
}

inline void Renderer::setDepthFunc(const int depthFunc){
	selectedDepthFunc = depthFunc;
}

inline void Renderer::setMask(const int mask){
	selectedMask = mask;
}

#endif // _RENDERER_H_
