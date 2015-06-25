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

#include "Direct3DRenderer.h"

// Blending constants
const int ZERO                 = D3DBLEND_ZERO;
const int ONE                  = D3DBLEND_ONE;
const int SRC_COLOR            = D3DBLEND_SRCCOLOR;
const int ONE_MINUS_SRC_COLOR  = D3DBLEND_INVSRCCOLOR;
const int DST_COLOR            = D3DBLEND_DESTCOLOR;
const int ONE_MINUS_DST_COLOR  = D3DBLEND_INVDESTCOLOR;
const int SRC_ALPHA            = D3DBLEND_SRCALPHA;
const int ONE_MINUS_SRC_ALPHA  = D3DBLEND_INVSRCALPHA;
const int DST_ALPHA            = D3DBLEND_DESTALPHA;
const int ONE_MINUS_DST_ALPHA  = D3DBLEND_INVDESTALPHA;
const int SRC_ALPHA_SATURATE   = D3DBLEND_SRCALPHASAT;

const int BLENDMODE_ADD              = D3DBLENDOP_ADD;
const int BLENDMODE_SUBTRACT         = D3DBLENDOP_SUBTRACT;
const int BLENDMODE_REVERSE_SUBTRACT = D3DBLENDOP_REVSUBTRACT;
const int BLENDMODE_MIN              = D3DBLENDOP_MIN;
const int BLENDMODE_MAX              = D3DBLENDOP_MAX;

// Depth testing constants
const int NEVER    = D3DCMP_NEVER;
const int LESS     = D3DCMP_LESS;
const int EQUAL    = D3DCMP_EQUAL;
const int LEQUAL   = D3DCMP_LESSEQUAL;
const int GREATER  = D3DCMP_GREATER;
const int NOTEQUAL = D3DCMP_NOTEQUAL;
const int GEQUAL   = D3DCMP_GREATEREQUAL;
const int ALWAYS   = D3DCMP_ALWAYS;

Direct3DRenderer::Direct3DRenderer(const D3DCAPS9 &d3dcaps, const LPDIRECT3DDEVICE9 d3ddev) : Renderer(){
	dev = d3ddev;
	byteOrderRGBA = false;

	nSimultaneousTextures = d3dcaps.MaxSimultaneousTextures;
	if (nSimultaneousTextures > MAX_TEXTURE) nSimultaneousTextures = MAX_TEXTURE;
	maxAnisotropic = d3dcaps.MaxAnisotropy;

	resetToDefaults();
	setDefaultStates();

	initRTSurfaces();
}

Direct3DRenderer::~Direct3DRenderer(){
	freeRTSurfaces();
}

void Direct3DRenderer::setDefaultStates(){
	// Disable some odd defaults
	dev->SetRenderState(D3DRS_LIGHTING, FALSE);
	dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void Direct3DRenderer::initRTSurfaces(){
    dev->GetRenderTarget(0, &mainColorSurface);
    dev->GetDepthStencilSurface(&mainDepthSurface);
}

void Direct3DRenderer::freeRTSurfaces(){
    if (mainColorSurface != NULL) mainColorSurface->Release();
    if (mainDepthSurface != NULL) mainDepthSurface->Release();
	mainColorSurface = NULL;
	mainDepthSurface = NULL;
}

void Direct3DRenderer::removeRenderTargets(){
	for (unsigned int i = 0; i < textures.getCount(); i++){
		if (textures[i].isRenderTarget){
			removeTexture(textures[i]);
		}
	}
}

void Direct3DRenderer::recreateRenderTargets(){
	for (unsigned int i = 0; i < textures.getCount(); i++){
		if (textures[i].isRenderTarget){
			createRenderTarget(textures[i]);
		}
	}
}

D3DFORMAT getFormat(const FORMAT format){
	static D3DFORMAT formats[] = {
		D3DFMT_UNKNOWN,
		D3DFMT_L8,
		D3DFMT_A8L8,
		D3DFMT_X8R8G8B8,
		D3DFMT_A8R8G8B8,

		D3DFMT_L16,
		D3DFMT_G16R16,
		D3DFMT_UNKNOWN, // Unsupported
		D3DFMT_A16B16G16R16,

		D3DFMT_R16F,
		D3DFMT_G16R16F,
		D3DFMT_UNKNOWN, // Unsupported
		D3DFMT_A16B16G16R16F,

		D3DFMT_R32F,
		D3DFMT_G32R32F,
		D3DFMT_UNKNOWN, // Unsupported
		D3DFMT_A32B32G32R32F,
		D3DFMT_R5G6B5,
		D3DFMT_DXT1,
		D3DFMT_DXT3,
		D3DFMT_DXT5,
		(D3DFORMAT) '2ITA', // 3Dc
		D3DFMT_D16,
		D3DFMT_D24X8,
	};

	return formats[format];
}

bool Direct3DRenderer::createRenderTarget(Texture &texture){
	D3DFORMAT pFormat = getFormat(texture.format);

	if (texture.textureKind == TEXKIND_2D){
		if (texture.rtLockScreenSize){
			texture.width  = width;
			texture.height = height;
		}

		if (isDepthFormat(texture.format)){
			LPDIRECT3DSURFACE9 surface;

			if (dev->CreateDepthStencilSurface(texture.width, texture.height, pFormat, D3DMULTISAMPLE_NONE, 0, TRUE, &surface, NULL) != D3D_OK) return false;

			texture.texID = TEXTURE_NONE;
			texture.renderTargetData = new void *[1];
			texture.renderTargetData[0] = (void *) surface;
		} else {
			if (dev->CreateTexture(texture.width, texture.height, texture.mipmapped? 0 : 1, D3DUSAGE_RENDERTARGET, pFormat, D3DPOOL_DEFAULT, (LPDIRECT3DTEXTURE9 *) &texture.texID, NULL) != D3D_OK) return false;
		
			texture.renderTargetData = new void *[1];
			((LPDIRECT3DTEXTURE9) texture.texID)->GetSurfaceLevel(0, (LPDIRECT3DSURFACE9 *) &texture.renderTargetData[0]);
		}

	} else {
		if (dev->CreateCubeTexture(texture.width, texture.mipmapped? 0 : 1, D3DUSAGE_RENDERTARGET, pFormat, D3DPOOL_DEFAULT, (LPDIRECT3DCUBETEXTURE9 *) &texture.texID, NULL) != D3D_OK) return false;

		texture.renderTargetData = new void *[6];
		for (unsigned int i = 0; i < 6; i++){
			((LPDIRECT3DCUBETEXTURE9) texture.texID)->GetCubeMapSurface((D3DCUBEMAP_FACES) i, 0, (LPDIRECT3DSURFACE9 *) &texture.renderTargetData[i]);
		}
	}

	return true;
}

bool Direct3DRenderer::createTexture(Texture &texture){
	FORMAT format;
	unsigned char *src;

	switch (texture.textureKind){
	case TEXKIND_2D:
		format = texture.images[0]->getFormat();
		if (dev->CreateTexture(texture.images[0]->getWidth(), texture.images[0]->getHeight(), texture.images[0]->getNumberOfMipMaps(),
			0, getFormat(format), D3DPOOL_MANAGED, (LPDIRECT3DTEXTURE9 *) &texture.texID, NULL) != D3D_OK){
			addToLog("Couldn't create texture");
			return false;
		}
		break;
	case TEXKIND_CUBEMAP:
		format = texture.images[0]->getFormat();
		if (dev->CreateCubeTexture(texture.images[0]->getWidth(), texture.images[0]->getNumberOfMipMaps(),
			0, getFormat(format), D3DPOOL_MANAGED, (LPDIRECT3DCUBETEXTURE9 *) &texture.texID, NULL) != D3D_OK){
			addToLog("Couldn't create cubemap");
			return false;
		}
		break;
#ifndef NO_TEX3D
	case TEXKIND_3D:
		format = texture.image3D->getFormat();
		if (dev->CreateVolumeTexture(texture.image3D->getWidth(), texture.image3D->getHeight(), texture.image3D->getDepth(), texture.image3D->getNumberOfMipMaps(),
			0, getFormat(format), D3DPOOL_MANAGED, (LPDIRECT3DVOLUMETEXTURE9 *) &texture.texID, NULL) != D3D_OK){
			addToLog("Couldn't create volumetric texture");
			return false;
		}
		break;
#endif
	}

#ifndef NO_TEX3D
	if (texture.textureKind == TEXKIND_3D){
		Image3D *img = texture.image3D;
		D3DLOCKED_BOX box;

		int mipMapLevel = 0;
		while ((src = img->getImage(mipMapLevel)) != NULL){

			if (((LPDIRECT3DVOLUMETEXTURE9) texture.texID)->LockBox(mipMapLevel, &box, NULL, 0) == D3D_OK){
				memcpy(box.pBits, src, img->getMipMappedSize(mipMapLevel, 1));
				((LPDIRECT3DVOLUMETEXTURE9) texture.texID)->UnlockBox(mipMapLevel);
			}

			mipMapLevel++;
		}
	} else {
#endif
		D3DLOCKED_RECT rect;
		int nImages = (texture.textureKind == TEXKIND_CUBEMAP)? 6 : 1;

		for (int i = 0; i < nImages; i++){
			Image *img = texture.images[i];
			FORMAT format = img->getFormat();
			if (format == FORMAT_RGB8) img->convert(FORMAT_RGBA8);
			
			int mipMapLevel = 0;
			while ((src = img->getImage(mipMapLevel)) != NULL){
				if (texture.textureKind == TEXKIND_2D){
					if (((LPDIRECT3DTEXTURE9) texture.texID)->LockRect(mipMapLevel, &rect, NULL, 0) == D3D_OK){
						memcpy(rect.pBits, src, img->getMipMappedSize(mipMapLevel, 1));
						((LPDIRECT3DTEXTURE9) texture.texID)->UnlockRect(mipMapLevel);
					}
				} else {
					if (((LPDIRECT3DCUBETEXTURE9) texture.texID)->LockRect((D3DCUBEMAP_FACES) i, mipMapLevel, &rect, NULL, 0) == D3D_OK){
						memcpy(rect.pBits, src, img->getMipMappedSize(mipMapLevel, 1));
						((LPDIRECT3DCUBETEXTURE9) texture.texID)->UnlockRect((D3DCUBEMAP_FACES) i, mipMapLevel);
					}
				}

				mipMapLevel++;
			}
		}
#ifndef NO_TEX3D
	}
#endif

	return true;
}

void Direct3DRenderer::removeTexture(Texture &texture){
	if (texture.isRenderTarget){
		unsigned int i, n = (texture.textureKind == TEXKIND_CUBEMAP)? 6 : 1;
		for (i = 0; i < n; i++) ((LPDIRECT3DSURFACE9) texture.renderTargetData[i])->Release();

		delete texture.renderTargetData;
	}
	if (texture.texID != TEXTURE_NONE) ((LPDIRECT3DTEXTURE9) texture.texID)->Release();
}

void Direct3DRenderer::resetTextureUnits(){
	for (int i = 0; i < nSimultaneousTextures; i++){
		dev->SetTexture(i, NULL);
	}
}

bool Direct3DRenderer::createShader(Shader &shader, const char *vertexShaderText, const char *fragmentShaderText){
	LPD3DXBUFFER shaderBuf = NULL, errorsBuf = NULL;

	shader.programObj     = SHADER_NONE;
	shader.vertexShader   = SHADER_UNDEFINED;
	shader.fragmentShader = SHADER_UNDEFINED;
	shader.vertexShaderConstants   = NULL;
	shader.fragmentShaderConstants = NULL;

	if (vertexShaderText != NULL){
		if (strstr(vertexShaderText, "vs.1.") != NULL || strstr(vertexShaderText, "vs.2.") != NULL){
			if (D3DXAssembleShader(vertexShaderText, strlen(vertexShaderText), NULL, NULL, D3DXSHADER_DEBUG, &shaderBuf, &errorsBuf) == D3D_OK){
				dev->CreateVertexShader((DWORD *) shaderBuf->GetBufferPointer(), (LPDIRECT3DVERTEXSHADER9 *) &shader.vertexShader);
			} else {
				addToLog((const char *) errorsBuf->GetBufferPointer());
			}
		} else {
			if (D3DXCompileShader(vertexShaderText, strlen(vertexShaderText), NULL, NULL, "main", D3DXGetVertexShaderProfile(dev), D3DXSHADER_DEBUG, &shaderBuf, &errorsBuf, (ID3DXConstantTable **) &shader.vertexShaderConstants) == D3D_OK){
				dev->CreateVertexShader((DWORD *) shaderBuf->GetBufferPointer(), (LPDIRECT3DVERTEXSHADER9 *) &shader.vertexShader);
			} else {
				addToLog((const char *) errorsBuf->GetBufferPointer());
			}
		}
		if (shaderBuf != NULL) shaderBuf->Release();
		if (errorsBuf != NULL) errorsBuf->Release();

		if (shader.vertexShader == SHADER_UNDEFINED) return false;
	}

	if (fragmentShaderText != NULL){
		if (strstr(fragmentShaderText, "ps.1.") != NULL || strstr(fragmentShaderText, "ps.2.") != NULL){
			if (D3DXAssembleShader(fragmentShaderText, strlen(fragmentShaderText), NULL, NULL, D3DXSHADER_DEBUG, &shaderBuf, &errorsBuf) == D3D_OK){
				dev->CreatePixelShader((DWORD *) shaderBuf->GetBufferPointer(), (LPDIRECT3DPIXELSHADER9 *) &shader.fragmentShader);
			} else {
				addToLog((const char *) errorsBuf->GetBufferPointer());
			}
		} else {
			if (D3DXCompileShader(fragmentShaderText, strlen(fragmentShaderText), NULL, NULL, "main", D3DXGetPixelShaderProfile(dev), D3DXSHADER_DEBUG, &shaderBuf, &errorsBuf, (ID3DXConstantTable **) &shader.fragmentShaderConstants) == D3D_OK){
				dev->CreatePixelShader((DWORD *) shaderBuf->GetBufferPointer(), (LPDIRECT3DPIXELSHADER9 *) &shader.fragmentShader);
			} else {
				addToLog((const char *) errorsBuf->GetBufferPointer());
			}
		}
		if (shaderBuf != NULL) shaderBuf->Release();
		if (errorsBuf != NULL) errorsBuf->Release();

		if (shader.fragmentShader == SHADER_UNDEFINED) return false;
	}

	return true;
}

void Direct3DRenderer::removeShader(Shader &shader){
	if (shader.vertexShader != NULL) ((LPDIRECT3DVERTEXSHADER9) shader.vertexShader)->Release();
	if (shader.vertexShaderConstants != NULL) ((ID3DXConstantTable *) shader.vertexShaderConstants)->Release();

	if (shader.fragmentShader != NULL) ((LPDIRECT3DVERTEXSHADER9) shader.fragmentShader)->Release();
	if (shader.fragmentShaderConstants != NULL) ((ID3DXConstantTable *) shader.fragmentShaderConstants)->Release();
}

void Direct3DRenderer::changeRenderTarget(const TextureID colorTarget, const TextureID depthTarget, const int face){
	if (colorTarget != currentColorTarget || face != currentTargetFace){
		if (colorTarget == RT_FB){
			dev->SetRenderTarget(0, mainColorSurface);
		} else if (colorTarget == RT_NONE){
			dev->SetRenderTarget(0, NULL);
		} else {
			dev->SetRenderTarget(0, (LPDIRECT3DSURFACE9) textures[colorTarget].renderTargetData[face]);
		}
		currentColorTarget = colorTarget;
		currentTargetFace = face;
	}

	if (depthTarget != currentDepthTarget){
		if (depthTarget == RT_FB){
			dev->SetDepthStencilSurface(mainDepthSurface);
		} else if (depthTarget == RT_NONE){
			dev->SetDepthStencilSurface(NULL);
		} else {
			dev->SetDepthStencilSurface((LPDIRECT3DSURFACE9) textures[depthTarget].renderTargetData[face]);
		}
		currentDepthTarget = depthTarget;
	}
}

void Direct3DRenderer::changeToDefaultRenderTarget(){
	changeRenderTarget(RT_FB, RT_FB);
}

void Direct3DRenderer::assignNamedTextures(){
	if (currentShader != SHADER_NONE && nNamedTextures > 0){
		TextureID tex[MAX_TEXTURE];
		int i;

		for (i = 0; i < MAX_TEXTURE; i++){
			tex[i] = TEXTURE_NONE;
		}

		ID3DXConstantTable *table = (ID3DXConstantTable *) shaders[currentShader].fragmentShaderConstants;
		for (i = 0; i < nNamedTextures; i++){
			D3DXHANDLE handle;

			if (handle = table->GetConstantByName(NULL, selectedTextureNames[i])){
				unsigned int index = table->GetSamplerIndex(handle);

				tex[index] = selectedTextures[i];
			}
		}
		memcpy(selectedTextures, tex, sizeof(tex));
	}
}

void Direct3DRenderer::changeTextureUnit(const int textureUnit){
	currentTextureUnit = textureUnit;
}

void Direct3DRenderer::changeLod(const int textureUnit, const float lod){
	dev->SetSamplerState(textureUnit, D3DSAMP_MIPMAPLODBIAS, *(DWORD *) &lod);
}

void Direct3DRenderer::changeTexture(const TextureID textureID){
	TextureID currTex = currentTextures[currentTextureUnit];

	if (textureID != currTex){
		if (textureID == TEXTURE_NONE){
			dev->SetTexture(currentTextureUnit, NULL);
		} else {
			dev->SetTexture(currentTextureUnit, (LPDIRECT3DTEXTURE9) (textures[textureID].texID));

			dev->SetSamplerState(currentTextureUnit, D3DSAMP_MIPFILTER, textures[textureID].mipmapped? D3DTEXF_LINEAR : D3DTEXF_NONE);
			if (textures[textureID].levelOfAnisotropy > 1){
				dev->SetSamplerState(currentTextureUnit, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
				dev->SetSamplerState(currentTextureUnit, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
				dev->SetSamplerState(currentTextureUnit, D3DSAMP_MAXANISOTROPY, textures[textureID].levelOfAnisotropy);
			} else {
				dev->SetSamplerState(currentTextureUnit, D3DSAMP_MINFILTER, textures[textureID].noLinearFilter? D3DTEXF_POINT : D3DTEXF_LINEAR);
				dev->SetSamplerState(currentTextureUnit, D3DSAMP_MAGFILTER, textures[textureID].noLinearFilter? D3DTEXF_POINT : D3DTEXF_LINEAR);
			}

			dev->SetSamplerState(currentTextureUnit, D3DSAMP_ADDRESSU, textures[textureID].clampS? D3DTADDRESS_CLAMP : D3DTADDRESS_WRAP);
			dev->SetSamplerState(currentTextureUnit, D3DSAMP_ADDRESSV, textures[textureID].clampT? D3DTADDRESS_CLAMP : D3DTADDRESS_WRAP);
#ifndef NO_TEX3D
			if (textures[textureID].textureKind == TEXKIND_3D){
				dev->SetSamplerState(currentTextureUnit, D3DSAMP_ADDRESSW, textures[textureID].clampR? D3DTADDRESS_CLAMP : D3DTADDRESS_WRAP);
			}
#endif
		}
		currentTextures[currentTextureUnit] = textureID;
	}
}

void Direct3DRenderer::configureTexEnv(const TexEnv texEnv, bool isRGB){
	D3DTEXTURESTAGESTATETYPE combineMode, source0, source1, source2;
	if (isRGB){
		combineMode = D3DTSS_COLOROP;
		source0     = D3DTSS_COLORARG1;
		source1     = D3DTSS_COLORARG2;
		source2     = D3DTSS_COLORARG0;
	} else {
		combineMode = D3DTSS_ALPHAOP;
		source0     = D3DTSS_ALPHAARG1;
		source1     = D3DTSS_ALPHAARG2;
		source2     = D3DTSS_ALPHAARG0;
	}

	switch(texEnv){
	case TEXENV_MODULATE:
		dev->SetTextureStageState(currentTextureUnit, combineMode, D3DTOP_MODULATE);
		dev->SetTextureStageState(currentTextureUnit, source0,     D3DTA_CURRENT);
		dev->SetTextureStageState(currentTextureUnit, source1,     D3DTA_TEXTURE);
		break;
	case TEXENV_DOT3:
		if (isRGB){
			dev->SetTextureStageState(currentTextureUnit, combineMode, D3DTOP_DOTPRODUCT3);
			dev->SetTextureStageState(currentTextureUnit, source0,     D3DTA_CURRENT);
			dev->SetTextureStageState(currentTextureUnit, source1,     D3DTA_TEXTURE);
		}
		break;
	case TEXENV_ADD:
		dev->SetTextureStageState(currentTextureUnit, combineMode, D3DTOP_ADD);
		dev->SetTextureStageState(currentTextureUnit, source0,     D3DTA_CURRENT);
		dev->SetTextureStageState(currentTextureUnit, source1,     D3DTA_TEXTURE);
		break;
	case TEXENV_REPLACE:
		dev->SetTextureStageState(currentTextureUnit, combineMode, D3DTOP_SELECTARG1);
		dev->SetTextureStageState(currentTextureUnit, source0,     D3DTA_TEXTURE);
		break;
	case TEXENV_PASSTHRU:
		dev->SetTextureStageState(currentTextureUnit, combineMode, D3DTOP_SELECTARG1);
		dev->SetTextureStageState(currentTextureUnit, source0,     D3DTA_CURRENT);
		break;
	case TEXENV_INTERPOLATE:
		dev->SetTextureStageState(currentTextureUnit, combineMode, D3DTOP_LERP);
		dev->SetTextureStageState(currentTextureUnit, source0,     D3DTA_TEXTURE);
		dev->SetTextureStageState(currentTextureUnit, source1,     D3DTA_CURRENT);
		dev->SetTextureStageState(currentTextureUnit, source2,     D3DTA_DIFFUSE);
		break;
	case TEXENV_INTERPOLATE_PRIMARY:
		dev->SetTextureStageState(currentTextureUnit, combineMode, D3DTOP_LERP);
		dev->SetTextureStageState(currentTextureUnit, source0,     D3DTA_DIFFUSE);
		dev->SetTextureStageState(currentTextureUnit, source1,     D3DTA_CURRENT);
		dev->SetTextureStageState(currentTextureUnit, source2,     D3DTA_TEXTURE);
		break;
	case TEXENV_INTERPOLATE_TEX_ALPHA:
		dev->SetTextureStageState(currentTextureUnit, combineMode, D3DTOP_BLENDTEXTUREALPHA);
		dev->SetTextureStageState(currentTextureUnit, source0,     D3DTA_CURRENT);
		dev->SetTextureStageState(currentTextureUnit, source1,     D3DTA_TEXTURE);
		break;
	default:
		break;
	}

}

void Direct3DRenderer::changeTexEnv(const TexEnv colorTexEnv, const TexEnv alphaTexEnv){
	if (colorTexEnv != currentColorTexEnvs[currentTextureUnit] || 
		alphaTexEnv != currentAlphaTexEnvs[currentTextureUnit]){

		if (colorTexEnv != currentColorTexEnvs[currentTextureUnit]){
			configureTexEnv(colorTexEnv, true);
		}

		if (alphaTexEnv != currentAlphaTexEnvs[currentTextureUnit]){
			configureTexEnv(alphaTexEnv, false);
		}

		currentColorTexEnvs[currentTextureUnit] = colorTexEnv;
		currentAlphaTexEnvs[currentTextureUnit] = alphaTexEnv;
	}
}

void Direct3DRenderer::changeShader(const ShaderID shader){
	if (shader != currentShader){
		if (shader == SHADER_NONE){
			dev->SetVertexShader(NULL);
			dev->SetPixelShader(NULL);
		} else {
			dev->SetVertexShader((LPDIRECT3DVERTEXSHADER9) shaders[shader].vertexShader);
			dev->SetPixelShader((LPDIRECT3DPIXELSHADER9) shaders[shader].fragmentShader);
		}

		currentShader = shader;
	}
}

void Direct3DRenderer::changeShaderConstant1f(const char *name, const float constant){
	changeShaderConstant4f(name, vec4(constant, constant, constant, constant));
}

void Direct3DRenderer::changeShaderConstant2f(const char *name, const vec2 &constant){
	changeShaderConstant4f(name, vec4(constant, 0, 1));
}

void Direct3DRenderer::changeShaderConstant3f(const char *name, const vec3 &constant){
	changeShaderConstant4f(name, vec4(constant, 1));
}

void Direct3DRenderer::changeShaderConstant4f(const char *name, const vec4 &constant){
	if (currentShader != SHADER_NONE){
		D3DXHANDLE handle;

		if (shaders[currentShader].vertexShader != SHADER_UNDEFINED){
			if (shaders[currentShader].vertexShaderConstants != NULL){
				if (handle = ((ID3DXConstantTable *) shaders[currentShader].vertexShaderConstants)->GetConstantByName(NULL, name)){
					((ID3DXConstantTable *) shaders[currentShader].vertexShaderConstants)->SetVector(dev, handle, (D3DXVECTOR4 *) &constant);
				}
			}
		}

		if (shaders[currentShader].fragmentShader != SHADER_UNDEFINED){
			if (shaders[currentShader].fragmentShaderConstants != NULL){
				if (handle = ((ID3DXConstantTable *) shaders[currentShader].fragmentShaderConstants)->GetConstantByName(NULL, name)){
					((ID3DXConstantTable *) shaders[currentShader].fragmentShaderConstants)->SetVector(dev, handle, (D3DXVECTOR4 *) &constant);
				}
			}
		}
	}
}

void Direct3DRenderer::changeShaderConstant3x3f(const char *name, const mat3 &constant){
	if (currentShader != SHADER_NONE){
		D3DXHANDLE handle;

		if (shaders[currentShader].vertexShader != SHADER_UNDEFINED){
			if (shaders[currentShader].vertexShaderConstants != NULL){
				if (handle = ((ID3DXConstantTable *) shaders[currentShader].vertexShaderConstants)->GetConstantByName(NULL, name)){
					((ID3DXConstantTable *) shaders[currentShader].vertexShaderConstants)->SetFloatArray(dev, handle, constant, 9);
				}
			}
		}

		if (shaders[currentShader].fragmentShader != SHADER_UNDEFINED){
			if (shaders[currentShader].fragmentShaderConstants != NULL){
				if (handle = ((ID3DXConstantTable *) shaders[currentShader].fragmentShaderConstants)->GetConstantByName(NULL, name)){
					((ID3DXConstantTable *) shaders[currentShader].fragmentShaderConstants)->SetFloatArray(dev, handle, constant, 9);
				}
			}
		}
	}
}

void Direct3DRenderer::changeShaderConstant4x4f(const char *name, const mat4 &constant){
	if (currentShader != SHADER_NONE){
		D3DXHANDLE handle;

		if (shaders[currentShader].vertexShader != SHADER_UNDEFINED){
			if (shaders[currentShader].vertexShaderConstants != NULL){
				if (handle = ((ID3DXConstantTable *) shaders[currentShader].vertexShaderConstants)->GetConstantByName(NULL, name)){
					((ID3DXConstantTable *) shaders[currentShader].vertexShaderConstants)->SetMatrix(dev, handle, (D3DXMATRIX *) &constant);
				}
			}
		}

		if (shaders[currentShader].fragmentShader != SHADER_UNDEFINED){
			if (shaders[currentShader].fragmentShaderConstants != NULL){
				if (handle = ((ID3DXConstantTable *) shaders[currentShader].fragmentShaderConstants)->GetConstantByName(NULL, name)){
					((ID3DXConstantTable *) shaders[currentShader].fragmentShaderConstants)->SetMatrix(dev, handle, (D3DXMATRIX *) &constant);
				}
			}
		}
	}
}

void Direct3DRenderer::changeShaderConstantArray3f(const char *name, const vec3 *constant, const unsigned int count){

}

void Direct3DRenderer::changeShaderConstantArray4f(const char *name, const vec4 *constant, const unsigned int count){
	if (currentShader != SHADER_NONE){
		D3DXHANDLE handle;

		if (shaders[currentShader].vertexShader != SHADER_UNDEFINED){
			if (shaders[currentShader].vertexShaderConstants != NULL){
				if (handle = ((ID3DXConstantTable *) shaders[currentShader].vertexShaderConstants)->GetConstantByName(NULL, name)){
					((ID3DXConstantTable *) shaders[currentShader].vertexShaderConstants)->SetVectorArray(dev, handle, (D3DXVECTOR4 *) constant, count);
				}
			}
		}

		if (shaders[currentShader].fragmentShader != SHADER_UNDEFINED){
			if (shaders[currentShader].fragmentShaderConstants != NULL){
				if (handle = ((ID3DXConstantTable *) shaders[currentShader].fragmentShaderConstants)->GetConstantByName(NULL, name)){
					((ID3DXConstantTable *) shaders[currentShader].fragmentShaderConstants)->SetVectorArray(dev, handle, (D3DXVECTOR4 *) constant, count);
				}
			}
		}
	}
}


void Direct3DRenderer::changeBlending(const int srcFactor, const int destFactor, const int blendMode){
	if (srcFactor != BLENDING_NONE){
		if (currentSrcFactor == BLENDING_NONE){
			dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		}
		if (srcFactor != currentSrcFactor){
			dev->SetRenderState(D3DRS_SRCBLEND, currentSrcFactor = srcFactor);
		}
		if (destFactor != currentDestFactor){
			dev->SetRenderState(D3DRS_DESTBLEND, currentDestFactor = destFactor);
		}
		if (blendMode != currentBlendMode){
			dev->SetRenderState(D3DRS_BLENDOP, currentBlendMode = blendMode);
		}
	} else {
		if (currentSrcFactor != BLENDING_NONE){
			dev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			currentSrcFactor = BLENDING_NONE;
		}
	}
}

void Direct3DRenderer::changeDepthFunc(const int depthFunc){
	if (depthFunc != currentDepthFunc){
		if (depthFunc == DEPTH_NONE){
			dev->SetRenderState(D3DRS_ZENABLE, FALSE);
		} else {
			if (currentDepthFunc == DEPTH_NONE){
				dev->SetRenderState(D3DRS_ZENABLE, TRUE);
			}
			dev->SetRenderState(D3DRS_ZFUNC, depthFunc);
		}
		currentDepthFunc = depthFunc;
	}
}

void Direct3DRenderer::changeMask(const int mask){
	if (mask != currentMask){
		if ((mask & DEPTH) != (currentMask & DEPTH)){
			dev->SetRenderState(D3DRS_ZWRITEENABLE, mask & DEPTH);
		}
		if ((mask & COLOR) != (currentMask & COLOR)){
			dev->SetRenderState(D3DRS_COLORWRITEENABLE, mask >> 1);
		}

		currentMask = mask;
	}
}


void Direct3DRenderer::changeVSync(const bool value){
}

void Direct3DRenderer::changeAnisotropic(const TextureID textureID, const bool value){
}
