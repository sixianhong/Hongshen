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

#include "Renderer.h"
#include "Util/String.h"
#include <stdio.h>

void Renderer::resetToDefaults(){
	for (int i = 0; i < nSimultaneousTextures; i++){
		currentTextures[i] = TEXTURE_NONE;
		currentColorTexEnvs[i] = TEXENV_MODULATE;
		currentAlphaTexEnvs[i] = TEXENV_MODULATE;
	}
	currentTextureUnit = 0;
	currentSrcFactor  = BLENDING_NONE;
	currentDestFactor = BLENDING_NONE;
	currentBlendMode  = BLENDMODE_ADD;

	currentDepthFunc = LEQUAL;
	currentMask      = ALL;

	currentShader = SHADER_NONE;

	currentColorTarget = TEXTURE_NONE;
	currentDepthTarget = TEXTURE_NONE;
	currentTargetFace  = 0;

	reset();
}

Renderer::Renderer(){
	colorMipMaps = false;
	useAnisotropic = false;
}

void Renderer::reset(){
	for (int i = 0; i < nSimultaneousTextures; i++){
		selectedTextures[i] = TEXTURE_NONE;
		selectedColorTexEnvs[i] = TEXENV_NONE;
		selectedAlphaTexEnvs[i] = TEXENV_NONE;
	}
	nNamedTextures = 0;
	selectedSrcFactor  = BLENDING_NONE;
	selectedDestFactor = BLENDING_NONE;
	selectedBlendMode  = BLENDMODE_ADD;

	selectedDepthFunc = LEQUAL;
	selectedMask      = ALL;

	selectedShader = SHADER_NONE;
}

void Renderer::apply(){
	changeShader(selectedShader);

	assignNamedTextures();

	for (int textureUnit = 0; textureUnit < nSimultaneousTextures; textureUnit++){
		changeTextureUnit(textureUnit);
		changeTexture(selectedTextures[textureUnit]);
		changeTexEnv(selectedColorTexEnvs[textureUnit], selectedAlphaTexEnvs[textureUnit]);
	}

	changeBlending(selectedSrcFactor, selectedDestFactor, selectedBlendMode);
	changeDepthFunc(selectedDepthFunc);
	changeMask(selectedMask);

	reset();
}

void Renderer::clear(){
	int index;

	reset();
	apply();

	resetTextureUnits();

	index = textures.getCount();
	while (index--)	deleteTexture(index);

	index = shaders.getCount();
	while (index--)	deleteShader(index);
}

TextureID Renderer::addTexture(Image *img, unsigned int flags){
	Texture texture;

	texture.clampS = ((flags & TEX_CLAMP_S) != 0);
	texture.clampT = ((flags & TEX_CLAMP_T) != 0);
	texture.noLinearFilter = ((flags & TEX_NOLINEARFILTER) != 0);
	
	if (flags & TEX_NORMALMAP){
		if (img->getFormat() != FORMAT_I8) img->toGrayScale(byteOrderRGBA, false);
		img->toNormalMap(byteOrderRGBA, (flags & TEX_HEIGHTMAP) != 0);
		img->createMipMaps();

		//if ((flags & TEX_NOMIPMAPPING) == 0) img->createMipMaps();
		//img->toNormalMapAdvanced(1.0f, 2.0f, byteOrderRGBA);
	}

	texture.textureKind = (flags & TEX_1D)? TEXKIND_1D : TEXKIND_2D;
	texture.isRenderTarget = false;
	texture.rtLockScreenSize = false;
	texture.levelOfAnisotropy = (flags & TEX_NOANISO)? 1 : maxAnisotropic;
	texture.width  = img->getWidth();
	texture.height = img->getHeight();
	texture.depth  = 1;
	texture.format = img->getFormat();
	texture.mipmapped = (img->getNumberOfMipMaps() > 1);

	texture.images[0] = img;

	if (createTexture(texture)){
		return insertTexture(texture);
	} else {
		delete img;
		return TEXTURE_NONE;
	}
}


TextureID Renderer::addTexture(const char *fileName, unsigned int flags){
	Image *img = new Image();

	if (img->loadImage(fileName, byteOrderRGBA, (flags & TEX_NOMIPMAPPING)? MIPMAP_NONE : (flags & TEX_NORMALMAP)? MIPMAP_ALLOCATE : MIPMAP_CREATE)){
		return addTexture(img, flags);
	} else {
		addToLog(String("Couldn't load \"") + fileName + "\"");

		delete img;
		return TEXTURE_NONE;
	}
}

TextureID Renderer::addCubemap(Image *imgs[6]){
	Texture texture;

	texture.textureKind = TEXKIND_CUBEMAP;
	texture.isRenderTarget = false;
	texture.rtLockScreenSize = false;
	texture.noLinearFilter = false;
	texture.levelOfAnisotropy = 1;
	texture.width  = imgs[0]->getWidth();
	texture.height = imgs[0]->getHeight();
	texture.depth  = 1;
	texture.format = imgs[0]->getFormat();
	texture.mipmapped = (imgs[0]->getNumberOfMipMaps() > 1);

	memcpy(texture.images, imgs, 6 * sizeof(Image *));

	if (createTexture(texture)){
		return insertTexture(texture);
	} else {
		return TEXTURE_NONE;
	}
}

TextureID Renderer::addCubemap(const char *fileNegX, const char *filePosX, const char *fileNegY, const char *filePosY, const char *fileNegZ, const char *filePosZ){
	const char *fileNames[] = { filePosX, fileNegX, filePosY, fileNegY, filePosZ, fileNegZ };
	Image *images[6];
	
	for (int i = 0; i < 6; i++){
		images[i] = new Image();

		if (!images[i]->loadImage(fileNames[i], byteOrderRGBA, MIPMAP_CREATE)){
			addToLog(String("Couldn't load \"") + fileNames[i] + "\"");

			do {
				delete images[i];
			} while (i--);
			
			return TEXTURE_NONE;
		}
	}
	return addCubemap(images);
}

#ifndef NO_TEX3D
TextureID Renderer::addTexture3D(Image3D *img, unsigned int flags){
	if (!(flags & TEX_NOMIPMAPPING)) img->createMipMaps();

	Texture texture;
	texture.textureKind = TEXKIND_3D;
	texture.isRenderTarget = false;
	texture.rtLockScreenSize = false;
	texture.noLinearFilter = ((flags & TEX_NOLINEARFILTER) != 0);
	texture.clampS = ((flags & TEX_CLAMP_S) != 0);
	texture.clampT = ((flags & TEX_CLAMP_T) != 0);
	texture.clampR = ((flags & TEX_CLAMP_R) != 0);
	texture.levelOfAnisotropy = 1;
	texture.width  = img->getWidth();
	texture.height = img->getHeight();
	texture.depth  = img->getDepth();
	texture.format = img->getFormat();
	texture.mipmapped = (img->getNumberOfMipMaps() > 1);

	texture.image3D = img;

	if (createTexture(texture)){
		return insertTexture(texture);
	} else {
		delete img;
		return TEXTURE_NONE;
	}
}

TextureID Renderer::addTexture3D(const char *fileName, unsigned int flags){
	Image3D *img = new Image3D();

	if (img->loadFromFile(fileName)){
		return addTexture3D(img, flags);
	} else {
		addToLog(String("Couldn't load \"") + fileName + "\"");

		delete img;
		return TEXTURE_NONE;
	}
}
#endif

TextureID Renderer::addRenderTexture(const int width, const int height, const FORMAT format, bool mipmapped, bool lockToScreenSize){
	Texture texture;

	texture.textureKind = TEXKIND_2D;
	texture.isRenderTarget = true;
	texture.rtLockScreenSize = lockToScreenSize;
	texture.noLinearFilter = false;//((flags & TEX_NOLINEARFILTER) != 0);
	texture.clampS = true;
	texture.clampT = true;
	texture.levelOfAnisotropy = 1;
	texture.width  = width;
	texture.height = height;
	texture.depth  = 1;
	texture.format = format;
	texture.mipmapped = mipmapped;
	
	if (createRenderTarget(texture)){
		return insertTexture(texture);
	} else {
		return TEXTURE_NONE;
	}
}

TextureID Renderer::addRenderCubemap(const int size, const FORMAT format, bool mipmapped){
	Texture texture;

	texture.textureKind = TEXKIND_CUBEMAP;
	texture.isRenderTarget = true;
	texture.rtLockScreenSize = false;
	texture.noLinearFilter = false;//((flags & TEX_NOLINEARFILTER) != 0);
	texture.clampS = true;
	texture.clampT = true;
	texture.levelOfAnisotropy = 1;
	texture.width  = size;
	texture.height = size;
	texture.depth  = 1;
	texture.format = format;
	texture.mipmapped = mipmapped;
	
	if (createRenderTarget(texture)){
		return insertTexture(texture);
	} else {
		return TEXTURE_NONE;
	}
}

TextureID Renderer::addRenderDepth(const int width, const int height, const FORMAT format, bool lockToScreenSize){
	Texture texture;

	texture.textureKind = TEXKIND_2D;
	texture.isRenderTarget = true;
	texture.rtLockScreenSize = lockToScreenSize;
	texture.noLinearFilter = false;//((flags & TEX_NOLINEARFILTER) != 0);
	texture.clampS = true;
	texture.clampT = true;
	texture.levelOfAnisotropy = 1;
	texture.width  = width;
	texture.height = height;
	texture.depth  = 1;
	texture.format = format;
	texture.mipmapped = false;
	
	if (createRenderTarget(texture)){
		return insertTexture(texture);
	} else {
		return TEXTURE_NONE;
	}
}


ShaderID Renderer::addShader(const char *fileName, const char *extraDefines){
	FILE *file = fopen(fileName, "rb");
	if (file == NULL){
		addToLog(String("Couldn't load \"") + fileName + "\"");
	} else {
		// Find file size
		fseek(file,  0, SEEK_END);
		int length = ftell(file);
		fseek(file,  0, SEEK_SET);

		char *shaderText = new char[length + 1];
		fread(shaderText, length, 1, file);
		fclose(file);
		shaderText[length] = '\0';

		char *vertexShader   = strstr(shaderText, "[Vertex shader]");
		char *fragmentShader = strstr(shaderText, "[Fragment shader]");

		if (vertexShader != NULL){
			*vertexShader = '\0';
			vertexShader += 15;
		}
		if (fragmentShader != NULL){
			*fragmentShader = '\0';
			fragmentShader += 17;
		}

		String vShaderString, fShaderString;
		if (vertexShader != NULL){
			int n = 0;
			char *str = shaderText;
			while (str < vertexShader){
				if (*str == '\n') n++;
				str++;
			}
			if (extraDefines) vShaderString = extraDefines;
			vShaderString.sprintf("#line %d\n", n);
			vShaderString += vertexShader;
		}
		if (fragmentShader != NULL){
			int n = 0;
			char *str = shaderText;
			while (str < fragmentShader){
				if (*str == '\n') n++;
				str++;
			}
			if (extraDefines) fShaderString = extraDefines;
			fShaderString.sprintf("#line %d\n", n);
			fShaderString += fragmentShader;
		}

		Shader shader;
		bool res = createShader(shader, vertexShader? (const char *) vShaderString : NULL, fragmentShader? (const char *) fShaderString : NULL);

		delete shaderText;

		if (res) return insertShader(shader);
	}
	return SHADER_NONE;
}

TextureID Renderer::insertTexture(Texture &texture){
	int i, len = textures.getCount();
	for (i = 0; i < len; i++){
		if (textures[i].texID == TEXTURE_UNDEFINED){
			textures[i] = texture;
			break;
		}
	}

	if (i == len) i = textures.add(texture);

	changeAnisotropic(i, useAnisotropic);

	return i;
}

void Renderer::deleteTexture(const int index){
	if (textures[index].texID != TEXTURE_UNDEFINED){
		removeTexture(textures[index]);

		if (!textures[index].isRenderTarget){
			switch (textures[index].textureKind){
			case TEXKIND_1D:
			case TEXKIND_2D:
				delete textures[index].images[0];
				break;
			case TEXKIND_CUBEMAP:
				int i;
				for (i = 0; i < 6; i++) delete textures[index].images[i];
				break;
#ifndef NO_TEX3D
			case TEXKIND_3D:
				delete textures[index].image3D;
				break;
#endif
			}
		}
		
		textures[index].texID = TEXTURE_UNDEFINED;
	}
}

ShaderID Renderer::insertShader(Shader &shader){
	int i, len = shaders.getCount();
	for (i = 0; i < len; i++){
		if (shaders[i].programObj == SHADER_UNDEFINED){
			shaders[i] = shader;
			return i;
		}
	}

	return shaders.add(shader);
}

void Renderer::deleteShader(const int index){
	if (shaders[index].programObj != SHADER_UNDEFINED){
		removeShader(shaders[index]);
		shaders[index].programObj = SHADER_UNDEFINED;
	}
}

/*

TextureID Renderer::getNextTexture(TextureID currentTexture) const {
	int len = textures.getSize();

	do {
		currentTexture++;
		if (currentTexture == len) return TEXTURE_NONE;
	} while (textures[currentTexture].texID == TEXTURE_UNDEFINED);
	
	return currentTexture;
}
*/
