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

#ifndef _IMAGE_DECLARES_H_
#define _IMAGE_DECLARES_H_

#include "../Platform.h"

enum FORMAT {
	FORMAT_NONE    = 0,
	// Plain formats
	FORMAT_I8      = 1,
	FORMAT_IA8     = 2,
	FORMAT_RGB8    = 3,
	FORMAT_RGBA8   = 4,
	
	FORMAT_I16     = 5,
	FORMAT_RG16    = 6,
	FORMAT_RGB16   = 7,
	FORMAT_RGBA16  = 8,

	FORMAT_I16F    = 9,
	FORMAT_RG16F   = 10,
	FORMAT_RGB16F  = 11,
	FORMAT_RGBA16F = 12,

	FORMAT_I32F    = 13,
	FORMAT_RG32F   = 14,
	FORMAT_RGB32F  = 15,
	FORMAT_RGBA32F = 16,
	// Packed formats
	FORMAT_RGB565  = 17,
	// Compressed formats
	FORMAT_DXT1    = 18,
	FORMAT_DXT3    = 19,
	FORMAT_DXT5    = 20,
	FORMAT_3DC     = 21,


	// TODO: Fix these
	FORMAT_D16     = 22,
	FORMAT_D24     = 23,
};


inline bool isPlainFormat(const FORMAT format){
	return (format <= FORMAT_RGBA32F);
}

inline bool isPackedFormat(const FORMAT format){
	return (format == FORMAT_RGB565);
}

inline bool isCompressedFormat(const FORMAT format){
	return (format >= FORMAT_DXT1) && (format <= FORMAT_3DC);
}

inline bool isFloatFormat(const FORMAT format){
	return (format >= FORMAT_I16F && format <= FORMAT_RGBA32F);
}

inline bool isDepthFormat(const FORMAT format){
	return (format >= FORMAT_D16 && format <= FORMAT_D24);
}

inline int getChannels(const FORMAT format){
	switch (format){
	case FORMAT_I8:
	case FORMAT_I16:
	case FORMAT_I16F:
	case FORMAT_I32F:
	case FORMAT_D16:
	case FORMAT_D24:
		return 1;
	case FORMAT_IA8:
	case FORMAT_RG16:
	case FORMAT_RG16F:
	case FORMAT_RG32F:
	case FORMAT_3DC:
		return 2;
	case FORMAT_RGB8:
	case FORMAT_RGB16:
	case FORMAT_RGB16F:
	case FORMAT_RGB32F:
	case FORMAT_RGB565:
	case FORMAT_DXT1:
		return 3;
	case FORMAT_RGBA8:
	case FORMAT_RGBA16:
	case FORMAT_RGBA16F:
	case FORMAT_RGBA32F:
	case FORMAT_DXT3:
	case FORMAT_DXT5:
		return 4;
	default:
		return 0;
	}
}

// Accepts only plain formats
inline int getBytesPerChannel(const FORMAT format){
	if (format <= FORMAT_RGBA8) return 1;
	if (format <= FORMAT_RGBA16F) return 2;
	return 4;
}

// Accepts only plain, packed and depth formats
inline int getBytesPerPixel(const FORMAT format){
	switch(format){
	case FORMAT_I8:
		return 1;
	case FORMAT_IA8:
	case FORMAT_I16:
	case FORMAT_I16F:
	case FORMAT_RGB565:
	case FORMAT_D16:
		return 2;
	case FORMAT_RGB8:
	case FORMAT_D24:
		return 3;
	case FORMAT_RGBA8:
	case FORMAT_RG16:
	case FORMAT_RG16F:
	case FORMAT_I32F:
		return 4;
	case FORMAT_RGB16:
	case FORMAT_RGB16F:
		return 6;
	case FORMAT_RG32F:
		return 8;
	case FORMAT_RGBA16:
	case FORMAT_RGBA16F:
	case FORMAT_RGB32F:
		return 12;
	case FORMAT_RGBA32F:
		return 16;
	default:
		return 0;
	}
}

// Accepts only compressed formats
inline int getBytesPerBlock(const FORMAT format){
	return (format == FORMAT_DXT1)? 8 : 16;
}

/* ------------------------------------------ */

enum MIPMAP_ACTION {
	MIPMAP_NONE     = 0,
	MIPMAP_ALLOCATE = 1,
	MIPMAP_CREATE   = 2
};


enum ResizeMethod {
	IMAGE_NEAREST,
	IMAGE_BILINEAR,
	IMAGE_BICUBIC,
};

enum WrapMethod {
	REPEAT = 0,
	CLAMP  = 1,
};

#define LOREPEAT(x, s) ((x <  0)? x + s : x)
#define HIREPEAT(x, s) ((x >= s)? x - s : x)

#define LOCLAMP(x)     ((x <  0)? 0 : x)
#define HICLAMP(x, s)  ((x >= s)? s : x)

#define LOWRAP(x, s, m) ((m == REPEAT)? LOREPEAT(x, s) : LOCLAMP(x))
#define HIWRAP(x, s, m) ((m == REPEAT)? HIREPEAT(x, s) : HICLAMP(x, s))

#endif // _IMAGE_DECLARES_H_
