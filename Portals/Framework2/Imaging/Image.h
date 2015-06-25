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

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "ImageDeclares.h"

class Image {
public:
	Image();
	Image(const Image &img);
	~Image();

	void free();
	void clear();

	unsigned char *getImage(const int mipMapLevel = 0) const;
	int getNumberOfMipMaps() const { return nMipMaps; }
	int getNumberOfMipMapsFromDimesions() const;
	int getMipMappedSize (const int firstMipMapLevel = 0, const int nMipMapLevels = 0x7fffffff, FORMAT srcFormat = FORMAT_NONE) const;
	int getNumberOfPixels(const int firstMipMapLevel = 0, const int nMipMapLevels = 0x7fffffff) const;

	int getWidth (const int mipMapLevel = 0) const;
	int getHeight(const int mipMapLevel = 0) const;
	FORMAT getFormat() const { return format; }

protected:
	bool loadDds (const char *fileName, const bool useRGBA, const bool wantMipMaps);
#ifndef NO_JPEG
	bool loadJpeg(const char *fileName, const bool useRGBA, const bool wantMipMaps);
#endif // NO_JPEG
#ifndef NO_PNG
	bool loadPng (const char *fileName, const bool useRGBA, const bool wantMipMaps);
#endif // NO_PNG
#ifndef NO_TGA
	bool loadTga (const char *fileName, const bool useRGBA, const bool wantMipMaps);
#endif // NO_TGA
#ifndef NO_BMP
	bool loadBmp (const char *fileName, const bool useRGBA, const bool wantMipMaps);
#endif // NO_BMP
#ifndef NO_PCX
	bool loadPcx (const char *fileName, const bool useRGBA, const bool wantMipMaps);
#endif // NO_PCX

	bool saveDds (const char *fileName, const bool isRGBA);
#ifndef NO_JPEG
	bool saveJpeg(const char *fileName, const bool isRGBA, const int quality);
#endif // NO_JPEG
#ifndef NO_PNG
	bool savePng (const char *fileName, const bool isRGBA);
#endif // NO_PNG
#ifndef NO_TGA
	bool saveTga (const char *fileName, const bool isRGBA);
#endif // NO_TGA
#ifndef NO_BMP
	bool saveBmp (const char *fileName, const bool isRGBA);
#endif // NO_BMP
#ifndef NO_PCX
	bool savePcx (const char *fileName, const bool isRGBA);
#endif // NO_PCX

public:
	bool loadImage(const char *fileName, const bool useRGBA = true, const MIPMAP_ACTION mipmapAction = MIPMAP_NONE);
	bool saveImage(const char *fileName, const bool isRGBA = true, const int quality = 75);

	void loadFromMemory(void *mem, const int w, const int h, const FORMAT frmt, const bool ownsmemory, const bool mipMapped = false);

	bool createMipMaps();
	bool colorMipMaps();
	bool uncompressImage(const bool useRGBA = true);
	bool unpackImage();

	bool convert(const FORMAT newFormat);

	bool removeMipMaps(const int removeMipMaps);

	bool resize(const int newWidth, const int newHeight, const ResizeMethod method = IMAGE_BILINEAR);
	bool toNormalMap(const bool useRGBA = true, const bool keepHeight = false);
	bool toNormalMapAdvanced(float sZ = 1.0f, float mipMapScaleZ = 2.0f, const bool useRGBA = true);
	bool toGrayScale(const bool isRGBA = true, const bool reallocate = true);
	bool alphaFade();

	bool laplace(const WrapMethod wrap);

protected:
	unsigned char *pixels;
	long width, height;
	long nMipMaps;
	FORMAT format;
	bool ownsMemory;
};

#endif // _IMAGE_H_
