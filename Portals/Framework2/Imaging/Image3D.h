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

#ifndef _IMAGE3D_H_
#define _IMAGE3D_H_

#include "ImageDeclares.h"

class Image3D {
protected:
	unsigned char *pixels;
	int width, height, depth;
	int nMipMaps;
	FORMAT format;
	bool ownsMemory;

public:
	Image3D();
	Image3D(const Image3D &img);
	~Image3D();

	void free();
	void clear();

	unsigned char *getImage(const int mipMapLevel = 0) const;
	int getNumberOfMipMaps() const;
	int getNumberOfMipMapsFromDimesions() const;
	int getMipMappedSize(const int firstMipMapLevel = 0, const int nMipMapLevels = 0x7fffffff, FORMAT srcFormat = FORMAT_NONE) const;

	int getWidth (const int mipMapLevel = 0) const;
	int getHeight(const int mipMapLevel = 0) const;
	int getDepth (const int mipMapLevel = 0) const;
	FORMAT getFormat() const { return format; }

protected:
	void buildMipMap(unsigned char *dest, unsigned char *src, int width, int height, int depth, int channels);
public:
	bool loadFromFile(const char *fileName, const bool useRGBA = true);
	void loadFromMemory(void *mem, const int w, const int h, const int d, const FORMAT frmt, bool ownsmemory, bool mipMapped = false);
	bool saveToFile(const char *fileName);
	bool createMipMaps();
};

#endif // _IMAGE3D_H_
