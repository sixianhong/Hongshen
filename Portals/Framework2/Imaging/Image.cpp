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

#include "Image.h"
#include "ImageUtils.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" {
#ifdef WIN32
#include "jpeglib.h"
#else
// Use system provided header on Linux
#include <jpeglib.h>
#endif
}
#ifdef WIN32
#include "png.h"
#else
// Use system provided header on Linux
#include <png.h>
#endif

#include "../Math/MyMath.h"

#ifdef WIN32

#ifndef NO_JPEG
#pragma comment (lib, "libjpeg.lib")
#endif // NO_JPEG

#ifndef NO_PNG
#pragma comment (lib, "libpng.lib")
#pragma comment (lib, "zlib.lib")
#endif // NO_PNG

#endif // WIN32

#pragma pack (push, 1)

#define DDPF_ALPHAPIXELS 0x00000001 
#define DDPF_FOURCC      0x00000004 
#define DDPF_RGB         0x00000040

#define DDSD_CAPS        0x00000001
#define DDSD_HEIGHT      0x00000002
#define DDSD_WIDTH       0x00000004
#define DDSD_PITCH       0x00000008
#define DDSD_PIXELFORMAT 0x00001000
#define DDSD_MIPMAPCOUNT 0x00020000
#define DDSD_LINEARSIZE  0x00080000
#define DDSD_DEPTH       0x00800000
#define DDSCAPS_COMPLEX  0x00000008 
#define DDSCAPS_TEXTURE  0x00001000 
#define DDSCAPS_MIPMAP   0x00400000 

struct DdsHeader {
	unsigned int ddsIdentifier;
	unsigned int size;
	unsigned int flags;
	unsigned int height;
	unsigned int width;
	unsigned int pitchOrLinearSize;
	unsigned int depth;
	unsigned int nMipMaps;
	unsigned int reserved[11];
	unsigned int size2;
	unsigned int flags2;
	unsigned int fourCC;
	unsigned int bpp;

	unsigned int rBitMask;
	unsigned int gBitMask;
	unsigned int bBitMask;
	unsigned int aBitMask;

	unsigned int caps1;
	unsigned int caps2;
	unsigned int reserved2[3];
};

struct TgaHeader {
	unsigned char  descriptionlen;
	unsigned char  cmaptype;
	unsigned char  imagetype;
	unsigned short cmapstart;
	unsigned short cmapentries;
	unsigned char  cmapbits;
	unsigned short xoffset;
	unsigned short yoffset;
	unsigned short width;
	unsigned short height;
	unsigned char  bpp;
	unsigned char  attrib;
};

struct BmpHeader {
	unsigned short bmpIdentifier;
	unsigned char  junk[16];
	unsigned int   width;
	unsigned int   height;
	unsigned short junk2;
	unsigned short bpp;
	unsigned short compression;
	unsigned char  junk3[22];
};

struct PcxHeader {
	unsigned char  junk[3];
	unsigned char  bitsPerChannel;
	unsigned char  junk2[4];
	unsigned short width;
	unsigned short height;
	unsigned char  junk3[53];
	unsigned char  nChannels;
	unsigned short scanlineLen;
	unsigned char  junk4[60];
};

#pragma pack (pop)


/* ---------------------------------------------- */




Image::Image(){
	pixels = NULL;
	width  = 0;
	height = 0;
	nMipMaps  = 0;
	format = FORMAT_NONE;
	ownsMemory = true;
}

Image::Image(const Image &img){
	width  = img.width;
	height = img.height;
	nMipMaps  = img.nMipMaps;
	format = img.format;
	ownsMemory = true;

	int size = getMipMappedSize(0, nMipMaps);
	pixels = new unsigned char[size];
	memcpy(pixels, img.pixels, size);
}

Image::~Image(){
	if (ownsMemory && (pixels != NULL)) delete pixels;
}

void Image::free(){
	if (pixels != NULL){
		if (ownsMemory) delete pixels;
		pixels = NULL;
	}
	ownsMemory = true;
}

void Image::clear(){
	free();
	width  = 0;
	height = 0;
	nMipMaps  = 0;
	format = FORMAT_NONE;
}

unsigned char *Image::getImage(const int mipMapLevel) const {
	return (mipMapLevel < nMipMaps)? pixels + getMipMappedSize(0, mipMapLevel) : NULL;
}

int Image::getNumberOfMipMapsFromDimesions() const {
	int max = (width > height)? width : height;
	int i = 0;

	while (max > 0){
		max >>= 1;
		i++;
	}

	return i;
}


int Image::getMipMappedSize(const int firstMipMapLevel, const int nMipMapLevels, FORMAT srcFormat) const {
	int w = getWidth (firstMipMapLevel) << 1,
		h = getHeight(firstMipMapLevel) << 1;
	int level = 0, size = 0;

	if (srcFormat == FORMAT_NONE) srcFormat = format;
	
	while (level < nMipMapLevels && (w != 1 || h != 1)){
		if (w > 1) w >>= 1;
		if (h > 1) h >>= 1;
		if (isCompressedFormat(srcFormat)){
			size += ((w + 3) >> 2) * ((h + 3) >> 2);
		} else {
			size += w * h;
		}
		level++;
	}

	if (isCompressedFormat(srcFormat)){
		size *= getBytesPerBlock(srcFormat);
	} else {
		size *= getBytesPerPixel(srcFormat);
	}

	return size;
}

int Image::getNumberOfPixels(const int firstMipMapLevel, const int nMipMapLevels) const {
	int w = getWidth (firstMipMapLevel) << 1,
		h = getHeight(firstMipMapLevel) << 1;
	int level = 0, size = 0;

	while (level < nMipMapLevels && (w != 1 || h != 1)){
		if (w > 1) w >>= 1;
		if (h > 1) h >>= 1;
		size += w * h;
		level++;
	}

	return size;
}

int Image::getWidth(const int mipMapLevel) const {
	int a = width >> mipMapLevel;
	return (a == 0)? 1 : a;
}

int Image::getHeight(const int mipMapLevel) const {
	int a = height >> mipMapLevel;
	return (a == 0)? 1 : a;
}

bool Image::loadDds(const char *fileName, const bool useRGBA, const bool wantMipMaps){
	DdsHeader header;

	FILE *file;
	int size, readSize;
	
	if ((file = fopen(fileName, "rb")) == NULL) return false;

	fread(&header, sizeof(header), 1, file);
	if (header.ddsIdentifier != MCHAR4('D', 'D', 'S', ' ')){
		fclose(file);
		return false;
	}

	width    = header.width;
	height   = header.height;
	nMipMaps = header.nMipMaps;
	if (nMipMaps <= 0 || !wantMipMaps) nMipMaps = 1; else nMipMaps = getNumberOfMipMapsFromDimesions();

	switch (header.fourCC){
		case MCHAR4('D', 'X', 'T', '1'):
			format = FORMAT_DXT1;
			break;
		case MCHAR4('D', 'X', 'T', '3'):
			format = FORMAT_DXT3;
			break;
		case MCHAR4('D', 'X', 'T', '5'):
			format = FORMAT_DXT5;
			break;
		case MCHAR4('A', 'T', 'I', '2'):
			format = FORMAT_3DC;
			break;
		default:
			switch (header.bpp){
				case 8:
					format = FORMAT_I8;
					break;
				case 16:
					if (header.aBitMask){
						format = FORMAT_IA8;
					} else {
						format = FORMAT_I16;
					}
					break;
				case 24:
					format = FORMAT_RGB8;
					break;
				case 32:
					format = FORMAT_RGBA8;
					break;
				default:
					fclose(file);
					return false;
			}
	}
	
	// Calculate how large buffer we need possibly including mipmaps
	readSize = getMipMappedSize(0, nMipMaps);

	if (isPlainFormat(format) && wantMipMaps) size = getMipMappedSize(0, 0x7fffffff); else size = readSize;

	pixels = new unsigned char[size];
	fread(pixels, 1, readSize, file);

	if (useRGBA && (format == FORMAT_RGB8 || format == FORMAT_RGBA8)){
		int nChannels = getChannels(format);
		flipChannels(pixels, readSize / nChannels, nChannels);
	}
	fclose(file);

	return true;
}

#ifndef NO_JPEG
bool Image::loadJpeg(const char *fileName, const bool useRGBA, const bool wantMipMaps){
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	FILE *file;
	if ((file = fopen(fileName, "rb")) == NULL) return false;
	
	jpeg_stdio_src(&cinfo, file);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	switch(cinfo.num_components){
	case 1:
		format = FORMAT_I8;
		break;
	case 3:
		format = FORMAT_RGB8;
		break;
	case 4:

		format = FORMAT_RGBA8;
		break;
	}
	width  = cinfo.output_width;
	height = cinfo.output_height;

	bool flip = (cinfo.num_components >= 3) && !useRGBA;

	pixels = new unsigned char[wantMipMaps? getMipMappedSize() : width * height * cinfo.num_components];
	unsigned char *curr_scanline = pixels;

	while (cinfo.output_scanline < cinfo.output_height){
		jpeg_read_scanlines(&cinfo, &curr_scanline, 1);
		if (flip) flipChannels(curr_scanline, width, cinfo.num_components);
		curr_scanline += width * cinfo.num_components;
	}
	
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	fclose(file);

	return true;
}
#endif // NO_JPEG

#ifndef NO_PNG
bool Image::loadPng(const char *fileName, const bool useRGBA, const bool wantMipMaps){
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
    FILE *file;

    png_byte pbSig[8];
    int iBitDepth, iColorType;
//    double dGamma;
//    png_color_16 *pBackground;
    png_byte **ppbRowPointers;

    // open the PNG input file
    if ((file = fopen(fileName, "rb")) == NULL) return false;

    // first check the eight byte PNG signature
    fread(pbSig, 1, 8, file);
    if (!png_check_sig(pbSig, 8)){
		fclose(file);
		return false;
	}

    // create the two png(-info) structures
    if ((png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, (png_error_ptr) NULL, (png_error_ptr) NULL)) == NULL){
		fclose(file);
        return false;
    }

    if ((info_ptr = png_create_info_struct(png_ptr)) == NULL){
        png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(file);
        return false;
    }

	// initialize the png structure
	png_init_io(png_ptr, file);
	png_set_sig_bytes(png_ptr, 8);
	
	// read all PNG info up to image data
	png_read_info(png_ptr, info_ptr);
	
	// get width, height, bit-depth and color-type
	png_get_IHDR(png_ptr, info_ptr, (png_uint_32 *) &width, (png_uint_32 *) &height, &iBitDepth, &iColorType, NULL, NULL, NULL);
/*	
	// expand images of all color-type and bit-depth to 3x8 bit RGB images
	// let the library process things like alpha, transparency, background
	if (iBitDepth == 16) png_set_strip_16(png_ptr);
	if (iColorType == PNG_COLOR_TYPE_PALETTE) png_set_expand(png_ptr);
	if (iBitDepth < 8) png_set_expand(png_ptr);
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_expand(png_ptr);
	
	// set the background color to draw transparent and alpha images over.
	if (png_get_bKGD(png_ptr, info_ptr, &pBackground))
		png_set_background(png_ptr, pBackground, PNG_BACKGROUND_GAMMA_FILE, 1, 1.0);
	
	// if required set gamma conversion
	if (png_get_gAMA(png_ptr, info_ptr, &dGamma))
		png_set_gamma(png_ptr, 2.2, dGamma);

	if (!useRGBA) png_set_bgr(png_ptr);
	
	// after the transformations have been registered update info_ptr data
	png_read_update_info(png_ptr, info_ptr);
	
	// get again width, height and the new bit-depth and color-type
	png_get_IHDR(png_ptr, info_ptr, (png_uint_32 *) &width, (png_uint_32 *) &height, &iBitDepth, &iColorType, NULL, NULL, NULL);
*/

	int nChannels = png_get_channels(png_ptr, info_ptr);
	switch (nChannels){
	case 1:
		format = FORMAT_I8;
		break;
	case 3:
		format = FORMAT_RGB8;
		break;
	case 4:
		format = FORMAT_RGBA8;
		break;
	default:
		fclose(file);
		return false;
	}
	
	int rowSize = width * nChannels * iBitDepth / 8;

	// now we can allocate memory to store the image
	pixels = new unsigned char[wantMipMaps? getMipMappedSize() : rowSize * height];

	// set the individual row-pointers to point at the correct offsets
	ppbRowPointers = new png_bytep[height];
	for (int i = 0; i < height; i++)
		ppbRowPointers[i] = pixels + i * rowSize;
	
	// now we can go ahead and just read the whole image
	png_read_image(png_ptr, ppbRowPointers);
	
	// read the additional chunks in the PNG file (not really needed)
	png_read_end(png_ptr, NULL);
	
	delete ppbRowPointers;

	if (iColorType == PNG_COLOR_TYPE_PALETTE){
		png_colorp palette;
		int num_palette;
		png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);

		format = FORMAT_RGB8;
		unsigned char *newPixels = new unsigned char[wantMipMaps? getMipMappedSize() : width * height * 3];
		if (iBitDepth == 4){
			for (int i = 0; i < rowSize * height; i++){
				unsigned int i0 = pixels[i] >> 4;
				unsigned int i1 = pixels[i] & 0xF;
				newPixels[6 * i    ] = palette[i0].red;
				newPixels[6 * i + 1] = palette[i0].green;
				newPixels[6 * i + 2] = palette[i0].blue;
				newPixels[6 * i + 3] = palette[i1].red;
				newPixels[6 * i + 4] = palette[i1].green;
				newPixels[6 * i + 5] = palette[i1].blue;
			}
		} else {
			for (int i = 0; i < width * height; i++){
				newPixels[3 * i    ] = palette[pixels[i]].red;
				newPixels[3 * i + 1] = palette[pixels[i]].green;
				newPixels[3 * i + 2] = palette[pixels[i]].blue;
			}
		}

		delete pixels;
		pixels = newPixels;
	}
	
	// and we're done
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    fclose(file);
	
    return true;
}
#endif // NO_PNG

#ifndef NO_TGA
bool Image::loadTga(const char *fileName, const bool useRGBA, const bool wantMipMaps){
	TgaHeader header;

	int size, x, y, pixelSize, palLength;
	unsigned char *tempBuffer, *fBuffer, *dest, *src;
	unsigned int tempPixel;
	unsigned char palette[768];
	FILE *file;


	if ((file = fopen(fileName, "rb")) == NULL) return false;
	
	// Find file size
	fseek(file,  0, SEEK_END);
	size = ftell(file);
	fseek(file,  0, SEEK_SET);

	// Read the header
	fread(&header, sizeof(header), 1, file);
	
	width  = header.width;
	height = header.height;

	pixelSize = header.bpp / 8;

	if ((palLength = header.descriptionlen + header.cmapentries * header.cmapbits / 8) > 0) fread(palette, sizeof(palette), 1, file);
	
	// Read the file data
	fBuffer = new unsigned char[size - sizeof(header) - palLength];
	fread(fBuffer, size - sizeof(header) - palLength, 1, file);
	fclose(file);

	size = width * height * pixelSize;

	tempBuffer = new unsigned char[size];

	// Decode if rle compressed. Bit 3 of .imagetype tells if the file is compressed
	if (header.imagetype & 0x08){
		unsigned int c,count;

		dest = tempBuffer;
		src = fBuffer;
		
		while (size > 0) {
			// Get packet header
			c = *src++;

			count = (c & 0x7f) + 1;
			size -= count * pixelSize;
			
			if (c & 0x80) {
				// Rle packet
				do {
					memcpy(dest,src,pixelSize);
					dest += pixelSize;
				} while (--count);
				src += pixelSize;
			} else {
				// Raw packet
				count *= pixelSize;
				memcpy(dest,src,count);
				src += count;
				dest += count;
			}
		}
		
		src = tempBuffer;
	} else {
		src = fBuffer;
	}

	src += (header.bpp / 8) * width * (height - 1);	

	int red, blue;

	if (useRGBA){
		red  = 2;
		blue = 0;
	} else {
		red  = 0;
		blue = 2;
	}

	switch(header.bpp) {
	case 8:
		if (palLength > 0){
			format = FORMAT_RGB8;
			dest = pixels = new unsigned char[getMipMappedSize(0, wantMipMaps?  0x7fffffff : 1)];
			for (y = 0; y < height; y++){
				for (x = 0; x < width; x++){
					tempPixel = 3 * (*src++);
					*dest++ = palette[tempPixel + red];
					*dest++ = palette[tempPixel + 1];
					*dest++ = palette[tempPixel + blue];
				}
				src -= 2 * width;
			}
		} else {
			format = FORMAT_I8;
			dest = pixels = new unsigned char[getMipMappedSize(0, wantMipMaps?  0x7fffffff : 1)];
			for (y = 0; y < height; y++){
				memcpy(dest, src, width);
				dest += width;
				src -= width;
			}
		}
		break;
	case 16:
		format = FORMAT_RGBA8;
		dest = pixels = new unsigned char[getMipMappedSize(0, wantMipMaps?  0x7fffffff : 1)];
		for (y = 0; y < height; y++){
			for (x = 0; x < width; x++){
				tempPixel = *((unsigned short *) src);

				dest[blue] = ((tempPixel >> 10) & 0x1F) << 3;
				dest[1]    = ((tempPixel >>  5) & 0x1F) << 3;
				dest[red]  = ((tempPixel      ) & 0x1F) << 3;
				dest[3]    = ((tempPixel >> 15) ? 0xFF : 0);
				dest += 4;
				src += 2;
			}
			src -= 4 * width;
		}
		break;
	case 24:
		format = FORMAT_RGB8;
		dest = pixels = new unsigned char[getMipMappedSize(0, wantMipMaps?  0x7fffffff : 1)];
		for (y = 0; y < height; y++){
			for (x = 0; x < width; x++){
				*dest++ = src[red];
				*dest++ = src[1];
				*dest++ = src[blue];
				src += 3;
			}
			src -= 6 * width;
		}
		break;
	case 32:
		format = FORMAT_RGBA8;
		dest = pixels = new unsigned char[getMipMappedSize(0, wantMipMaps?  0x7fffffff : 1)];
		for (y = 0; y < height; y++){
			for (x = 0; x < width; x++){
				*dest++ = src[red];
				*dest++ = src[1];
				*dest++ = src[blue];
				*dest++ = src[3];
				src += 4;
			}
			src -= 8 * width;
		}
		break;
	}

	delete tempBuffer;
	delete fBuffer;

	return true;
}
#endif

#ifndef NO_BMP
bool Image::loadBmp(const char *fileName, const bool useRGBA, const bool wantMipMaps){
	BmpHeader header;

	FILE *file;
	unsigned char *dest, *src, *tmp;
	int i, len;
	unsigned char palette[1024];

	if ((file = fopen(fileName, "rb")) == NULL) return false;
	
	// Read the header
	fread(&header, sizeof(header), 1, file);
	if (header.bmpIdentifier != MCHAR2('B', 'M')){
		fclose(file);
		return false;
	}

	width  = header.width;
	height = header.height;

	switch (header.bpp){
	case 8:
		if (header.compression){
			// No support for RLE compressed bitmaps
			fclose(file);
			return false;
		}
		format = FORMAT_RGB8;
		pixels = new unsigned char[getMipMappedSize(0, wantMipMaps? 0x7fffffff : 1)];

		fread(palette, sizeof(palette), 1, file);
		if (useRGBA) flipChannels(palette, 256, 4);

			// Normal unencoded 8 bit paletted bitmap
		tmp = new unsigned char[width];
		for (i = height - 1; i >= 0; i--){
			dest = pixels + i * width * 3;
			fread(tmp, width, 1, file);
			len = width;
			do {
				src = palette + ((*tmp++) << 2);
				*dest++ = *src++;
				*dest++ = *src++;
				*dest++ = *src++;
			} while (--len);
			tmp -= width;
		}
		delete tmp;
		break;
	case 24:
	case 32:
		int nChannels;
		nChannels = (header.bpp == 24)? 3 : 4;
		format    = (header.bpp == 24)? FORMAT_RGB8 : FORMAT_RGBA8;
		pixels = new unsigned char[getMipMappedSize(0, wantMipMaps? 0x7fffffff : 1)];
		for (i = height - 1; i >= 0; i--){
			dest = pixels + i * width * nChannels;
			fread(dest, width * nChannels, 1, file);
			if (useRGBA) flipChannels(dest, width, nChannels);
		}
		break;
	default:
		fclose(file);
		return false;
	}

	fclose(file);
	return true;
}
#endif // NO_BMP

#ifndef NO_PCX
bool Image::loadPcx(const char *fileName, const bool useRGBA, const bool wantMipMaps){
	PcxHeader header;

	FILE *file;
	int size, bpp, i, x,y,n, length, len, col;
	unsigned char *fBuffer, *src, *palette;

	if ((file = fopen(fileName, "rb")) == NULL) return false;
	
	// Find file size
	fseek(file,  0, SEEK_END);
	size = ftell(file) - sizeof(header);
	fseek(file,  0, SEEK_SET);

	// Read the header
	fread(&header, sizeof(header), 1, file);
	
	width     = header.width  + 1;
	height    = header.height + 1;
	format    = FORMAT_RGB8;

	bpp = header.nChannels * header.bitsPerChannel;

	pixels = new unsigned char[getMipMappedSize(0, wantMipMaps? 0x7fffffff : 1)];
	fBuffer = new unsigned char[size];
	fread(fBuffer, size, 1, file);
	fclose(file);

	src = fBuffer;

	int red, blue;

	if (useRGBA){
		red  = 0;
		blue = 2;
	} else {
		red  = 2;
		blue = 0;
	}

	switch(bpp){
	case 8:
		palette = fBuffer + size - 768;
		len = width * height * 3;
		i = 0;
		do {
			if (*src < 192){
				col = 3 * (*src++);
				pixels[i++] = palette[col + red];
				pixels[i++] = palette[col + 1];
				pixels[i++] = palette[col + blue];
			} else {
				length = (*src++) - 192;
				col = 3 * (*src++);
				do {
					pixels[i++] = palette[col + red];
					pixels[i++] = palette[col + 1];
					pixels[i++] = palette[col + blue];
				} while (--length);
			}
		} while (i < len);
		break;
	case 24:
		len = width * height * 3;
		i = 0;

		x = width;
		y = n = 0;

		do {
			if (x == 0){
				x = width;
				if (n == 2){
					if (++y == height) break;
					n = 0;
				} else n++;

				i = y * width * 3 + (useRGBA? n : 2 - n);
			}

			if (*src < 192){
				pixels[i += 3] = *src++;
				x--;
			} else {
				length = (*src++) - 192;
				x -= length;
				do {
					pixels[i += 3] = *src;
				} while (--length);
				src++;
			}

		} while (true);
		break;
	default:
		delete fBuffer;
		return false;
	}

	delete fBuffer;

	return true;
}
#endif // NO_PCX

bool Image::loadImage(const char *fileName, const bool useRGBA, const MIPMAP_ACTION mipmapAction){
	const char *extension = strrchr(fileName, '.');
	bool wantMipMaps = (mipmapAction > MIPMAP_NONE);

	clear();

	if (extension != NULL){
		if (stricmp(extension,".dds") == 0){
			if (!loadDds(fileName, useRGBA, wantMipMaps)) return false;
		}
#ifndef NO_JPEG
		else if (stricmp(extension,".jpg") == 0 || 
			       stricmp(extension,".jpeg") == 0){
			if (!loadJpeg(fileName, useRGBA, wantMipMaps)) return false;
		}
#endif // NO_JPEG
#ifndef NO_PNG
		else if (stricmp(extension,".png") == 0){
			if (!loadPng(fileName, useRGBA, wantMipMaps)) return false;
		}
#endif // NO_PNG
#ifndef NO_TGA
		else if (stricmp(extension,".tga") == 0){
			if (!loadTga(fileName, useRGBA, wantMipMaps)) return false;
		}
#endif // NO_TGA
#ifndef NO_BMP
		else if (stricmp(extension,".bmp") == 0){
			if (!loadBmp(fileName, useRGBA, wantMipMaps)) return false;
		}
#endif // NO_BMP
#ifndef NO_PCX
		else if (stricmp(extension,".pcx") == 0){
			if (!loadPcx(fileName, useRGBA, wantMipMaps)) return false;
		}
#endif // NO_PCX
		else return false;

		if (wantMipMaps){
			int w = getClosestPowerOfTwo(width);
			int h = getClosestPowerOfTwo(height);

			if (w != width || h != height){
				resize(w, h, IMAGE_BILINEAR);
			}

			if (nMipMaps == 0) nMipMaps = getNumberOfMipMapsFromDimesions();
			if (mipmapAction == MIPMAP_CREATE) createMipMaps();
		} else {
			nMipMaps = 1;
		}
		return true;
	}
	return false;
}

bool Image::saveDds(const char *fileName, const bool isRGBA){
	if ((format < FORMAT_I8 || format > FORMAT_RGBA8) && format != FORMAT_RGB565 && !isCompressedFormat(format)) return false;

	int nChannels = getChannels(format);
	//if (isCompressedFormat(format)) nChannels = 0;

	unsigned int fourCC[] = { MCHAR4('D','X','T','1'), MCHAR4('D','X','T','3'), MCHAR4('D','X','T','5'), MCHAR4('A','T','I','2') };
	DdsHeader header = {
		MCHAR4('D','D','S',' '),
		124,
		DDSD_CAPS | DDSD_PIXELFORMAT | DDSD_WIDTH | DDSD_HEIGHT | (nMipMaps > 1? DDSD_MIPMAPCOUNT : 0),
		height,
		width,
		0,/*width * nChannels*/
		0,
		(nMipMaps > 1)? nMipMaps : 0,
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		32,
		isCompressedFormat(format)? DDPF_FOURCC : ((nChannels < 3)? 0x00020000 : DDPF_RGB) | ((nChannels & 1)? 0 : DDPF_ALPHAPIXELS),
		isCompressedFormat(format)? fourCC[format - FORMAT_DXT1] : 0,
		isCompressedFormat(format)? 0 : 8 * nChannels,
		isCompressedFormat(format)? 0 : (nChannels >= 3)? 0x00ff0000 : 0xFF,
		isCompressedFormat(format)? 0 : (nChannels >= 3)? 0x0000ff00 : 0,
		isCompressedFormat(format)? 0 : (nChannels >= 3)? 0x000000ff : 0,
		isCompressedFormat(format)? 0 : (nChannels >= 3)? 0xff000000 : (nChannels == 2)? 0xFF00 : 0,
		DDSCAPS_TEXTURE | (nMipMaps > 1? DDSCAPS_MIPMAP | DDSCAPS_COMPLEX : 0),
		0,
		{ 0, 0, 0 }, 
	};

	FILE *file;
	if ((file = fopen(fileName, "wb")) == NULL) return false;

	fwrite(&header, sizeof(header), 1, file);
	int size = getMipMappedSize(0, nMipMaps);
	if (isRGBA && (format == FORMAT_RGB8 || format == FORMAT_RGBA8)) flipChannels(pixels, size / nChannels, nChannels);
	fwrite(pixels, size, 1, file);
	if (isRGBA && (format == FORMAT_RGB8 || format == FORMAT_RGBA8)) flipChannels(pixels, size / nChannels, nChannels);

	fclose(file);

	return true;
}


#ifndef NO_JPEG
bool Image::saveJpeg(const char *fileName, const bool isRGBA, const int quality){
	if (format != FORMAT_I8 && format != FORMAT_RGB8) return false;

	jpeg_compress_struct cinfo;
	jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	FILE *file;
	if ((file = fopen(fileName, "wb")) == NULL) return false;

	int nChannels = getChannels(format);

	cinfo.in_color_space = (nChannels == 1)? JCS_GRAYSCALE : JCS_RGB;
	jpeg_set_defaults(&cinfo);

	cinfo.input_components = nChannels;
	cinfo.num_components   = nChannels;
	cinfo.image_width  = width;
	cinfo.image_height = height;
	cinfo.data_precision = 8;
	cinfo.input_gamma = 1.0;

	jpeg_set_quality(&cinfo, quality, FALSE);

	jpeg_stdio_dest(&cinfo, file);
	jpeg_start_compress(&cinfo, TRUE);

	unsigned char *curr_scanline = pixels;

	for (int y = 0; y < height; y++){
		if (!isRGBA) flipChannels(curr_scanline, width, nChannels);
		jpeg_write_scanlines(&cinfo, &curr_scanline, 1);
		if (!isRGBA) flipChannels(curr_scanline, width, nChannels);
		curr_scanline += nChannels * width;
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	fclose(file);

	return true;
}
#endif // NO_JPEG


#ifndef NO_PNG
bool Image::savePng(const char *fileName, const bool isRGBA){
	int type;

	switch(format){
	case FORMAT_I8:
		type = PNG_COLOR_TYPE_GRAY;
		break;
	case FORMAT_RGB8:
		type = PNG_COLOR_TYPE_RGB;
		break;
	case FORMAT_RGBA8:
		type = PNG_COLOR_TYPE_RGBA;
		break;
	default:
		return false;
	}

	png_structp png_ptr;
	png_infop info_ptr;
	FILE *file;

    if ((file = fopen(fileName, "wb")) == NULL) return false;

    if ((png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, (png_error_ptr) NULL, (png_error_ptr) NULL)) == NULL){
		fclose(file);
        return false;
    }

    if ((info_ptr = png_create_info_struct(png_ptr)) == NULL){
        png_destroy_write_struct(&png_ptr, NULL);
		fclose(file);
        return false;
    }

	png_init_io(png_ptr, file);

	//if (!isRGBA) png_set_bgr(png_ptr);
	png_set_IHDR(png_ptr, info_ptr, width, height, 8, type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_ptr, info_ptr);

	int nChannels = getChannels(format);
	for (int y = 0; y < height; y++){
		png_write_row(png_ptr, pixels + y * width * nChannels);
	}

	png_write_end(png_ptr, info_ptr);
	
    png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(file);

	return true;
}
#endif // NO_PNG


#ifndef NO_TGA
bool Image::saveTga(const char *fileName, const bool isRGBA){
	if (format != FORMAT_I8 && format != FORMAT_RGB8 && format != FORMAT_RGBA8) return false;

	FILE *file;
	if ((file = fopen(fileName, "wb")) == NULL) return false;

	int nChannels = getChannels(format);

	TgaHeader header = {
		0x00,
		(format == FORMAT_I8)? 1 : 0,
		(format == FORMAT_I8)? 1 : 2,
		0x0000,
		(format == FORMAT_I8)? 256 : 0,
		(format == FORMAT_I8)? 24  : 0,
		0x0000,
		0x0000,
		(unsigned short) width,
		(unsigned short) height,
		nChannels * 8,
		0x00
	};

	fwrite(&header, sizeof(header), 1, file);

	unsigned char *dest, *src, *buffer;

	if (format == FORMAT_I8){
		unsigned char pal[768];
		int p = 0;
		for (int i = 0; i < 256; i++){
			pal[p++] = i;
			pal[p++] = i;
			pal[p++] = i;
		}
		fwrite(pal, sizeof(pal), 1, file);

		src = pixels + width * height;
		for (int y = 0; y < height; y++){
			src -= width;
			fwrite(src, width, 1, file);
		}

	} else {
		int red, blue;
		if (isRGBA){
			red   = 0;
			blue  = 2;
		} else {
			red   = 2;
			blue  = 0;
		}

		bool useAlpha = (nChannels == 4);
		int lineLength = width * (useAlpha? 4 : 3);

		buffer = new unsigned char[height * lineLength];
		int len;

		for (int y = 0; y < height; y++){
			dest = buffer + (height - y - 1) * lineLength;
			src  = pixels + y * width * nChannels;
			len = width;
			do {
				*dest++ = src[blue];
				*dest++ = src[1];
				*dest++ = src[red];
				if (useAlpha) *dest++ = src[3];
				src += nChannels;
			} while (--len);
		}

		fwrite(buffer, height * lineLength, 1, file);
		delete buffer;
	}

	fclose(file);

	return false;
}
#endif // NO_TGA

#ifndef NO_BMP
bool Image::saveBmp(const char *fileName, const bool isRGBA){
	if (format != FORMAT_I8 && format != FORMAT_RGB8 && format != FORMAT_RGBA8) return false;

	FILE *file;
	if ((file = fopen(fileName, "wb")) == NULL) return false;

	int nChannels = getChannels(format);
	BmpHeader header = {
		MCHAR2('B','M'), 
		{0x36, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00},
		width,
		height,
		0x0001,
		nChannels * 8,
		0,
		{0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x13, 0x0b, 0x00, 0x00, 0x13, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	};

	fwrite(&header, sizeof(header), 1, file);

	if (format == FORMAT_I8){
		unsigned char pal[1024];
		for (int i = 0; i < 1024; i++){
			pal[i] = i >> 2;
		}
		fwrite(pal, sizeof(pal), 1, file);

		unsigned char *src = pixels + width * height;
		for (int y = 0; y < height; y++){
			src -= width;
			fwrite(src, width, 1, file);
		}
	} else {
		int red, blue;
		if (isRGBA){
			red   = 0;
			blue  = 2;
		} else {
			red   = 2;
			blue  = 0;
		}
		
		bool useAlpha = (nChannels == 4);
		int paddBytes = (width * nChannels) & 3;
		if (paddBytes) paddBytes = 4 - paddBytes;

		int len, size = (width * nChannels + paddBytes) * height;
		unsigned char *dest, *src, *buffer = new unsigned char[size];

		for (int y = 0; y < height; y++){
			dest = buffer + (height - y - 1) * (width * nChannels + paddBytes);
			src  = pixels + y * width * nChannels;
			len = width;
			do {
				*dest++ = src[blue];
				*dest++ = src[1];
				*dest++ = src[red];
				if (useAlpha) *dest++ = src[3];
				src += nChannels;
			} while (--len);
		}
		fwrite(buffer, size, 1, file);
		delete buffer;
	}

	fclose(file);

	return true;
}
#endif // NO_BMP

#ifndef NO_PCX
bool Image::savePcx(const char *fileName, const bool isRGBA){
	if (format != FORMAT_I8 && format != FORMAT_RGB8 && format != FORMAT_RGBA8) return false;

	int nChannels = getChannels(format);
	int destChannels = (format == FORMAT_RGBA8)? 3 : nChannels;


	PcxHeader header = {
		{0x0a, 0x05, 0x01},
		8,
		{0x00, 0x00, 0x00, 0x00}, 
		(unsigned short) (width  - 1),
		(unsigned short) (height - 1),
		{0x48, 0x00, 0x48, 0x00, 0x0f, 0x0f, 0x0f, 0x0e, 0x0e, 0x0e,
		 0x0d, 0x0d, 0x0d, 0x0c, 0x0c, 0x0c, 0x0b, 0x0b, 0x0b, 0x0a,
		 0x0a, 0x0a, 0x09, 0x09, 0x09, 0x08, 0x08, 0x08, 0x07, 0x07,
		 0x07, 0x06, 0x06, 0x06, 0x05, 0x05, 0x05, 0x04, 0x04, 0x04,
		 0x03, 0x03, 0x03, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00,
		 0x00, 0x00, 0x00},
		destChannels, // 3 channels
		(unsigned short) width,
		{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	};

	FILE *file;
	if ((file = fopen(fileName, "wb")) == NULL) return false;

	fwrite(&header, sizeof(header), 1, file);


	unsigned char *src, *buffer = new unsigned char[width * 2];
	int pos;

	for (int y = 0; y < height; y++){
		for (int i = 0; i < destChannels; i++){
			src = pixels + y * width * nChannels + i;
			pos = 0;

			unsigned int count;
			int prevx, x = 0;
			unsigned char curr = *src;

			do {
				prevx = x;
				curr = src[x * nChannels];
				do {
					x++;
				} while (x < width && src[x * nChannels] == curr);

				count = x - prevx;
				if (count == 1){
					if (curr < 192){
						buffer[pos++] = curr;
					} else {
						buffer[pos++] = 193;
						buffer[pos++] = curr;
					}
				} else {
					do {
						unsigned char len = (count > 63)? 63 : count;
						
						buffer[pos++] = 192 + len;
						buffer[pos++] = curr;
						
						count -= len;
					} while (count);
				}

			} while (x < width);

			fwrite(buffer, pos, 1, file);			
		}
	}

	delete buffer;

	if (format == FORMAT_I8){
		unsigned char pal[768];
		int p = 0;
		for (int i = 0; i < 256; i++){
			pal[p++] = i;
			pal[p++] = i;
			pal[p++] = i;
		}
		fwrite(pal, sizeof(pal), 1, file);
	}

	fclose(file);

	return true;
}
#endif // NO_PCX

bool Image::saveImage(const char *fileName, const bool isRGBA, const int quality){
	const char *extension = strrchr(fileName, '.');

	if (extension != NULL){
		if (stricmp(extension,".dds") == 0){
			return saveDds(fileName, isRGBA);
		}
#ifndef NO_JPEG
		else if (stricmp(extension,".jpg") == 0 || 
                   stricmp(extension,".jpeg") == 0){
			return saveJpeg(fileName, isRGBA, quality);
		}
#endif // NO_JPEG
#ifndef NO_PNG
		else if (stricmp(extension,".png") == 0){
			return savePng(fileName, isRGBA);
		}
#endif // NO_PNG
#ifndef NO_TGA
		else if (stricmp(extension,".tga") == 0){
			return saveTga(fileName, isRGBA);
		}
#endif // NO_TGA
#ifndef NO_BMP
		else if (stricmp(extension,".bmp") == 0){
			return saveBmp(fileName, isRGBA);
		}
#endif // NO_BMP
#ifndef NO_PCX
		else if (stricmp(extension,".pcx") == 0){
			return savePcx(fileName, isRGBA);
		}
#endif // NO_PCX
	}
	return false;
}

void Image::loadFromMemory(void *mem, const int w, const int h, const FORMAT frmt, const bool ownsmemory, const bool mipMapped){
	free();

	pixels = (unsigned char *) mem;
	width  = w;
	height = h;
	format = frmt;
	ownsMemory = ownsmemory;

	nMipMaps = mipMapped? getNumberOfMipMapsFromDimesions() : 1;
}



void buildMipMap8(unsigned char *dest, unsigned char *src, int width, int height, int channels){
	int xOff = (width  < 2)? 0 : channels;
	int yOff = (height < 2)? 0 : width * channels;

	for (int y = 0; y < height; y += 2){
		for (int x = 0; x < width; x += 2){
			for (int i = 0; i < channels; i++){
				*dest++ = ((src[0] +  src[xOff] + src[yOff] + src[yOff + xOff]) + 2) >> 2;
				src++;
			}
			src += xOff;
		}
		src += yOff;
	}
}

void buildMipMap32f(float *dest, float *src, int width, int height, int channels){
	int xOff = (width  < 2)? 0 : channels;
	int yOff = (height < 2)? 0 : width * channels;

	for (int y = 0; y < height; y += 2){
		for (int x = 0; x < width; x += 2){
			for (int i = 0; i < channels; i++){
				*dest++ = (src[0] + src[xOff] + src[yOff] + src[yOff + xOff]) * 0.25f;
				src++;
			}
			src += xOff;
		}
		src += yOff;
	}
}

void buildMipMapRGB565(unsigned short *dest, unsigned short *src, int width, int height){
	int x,y,diff;
	int xOff = (width  < 2)? 0 : 1;
	int yOff = (height < 2)? 0 : 1;

	unsigned int r,g,b;
	
	diff = yOff * width;

	for (y = 0; y < height; y += 2){
		for (x = 0; x < width; x += 2){
			r = (((src[0] >> 8) & 0xF8) + 
                 ((src[xOff] >> 8) & 0xF8) + 
                 ((src[diff] >> 8) & 0xF8) + 
                 ((src[diff + xOff] >> 8) & 0xF8));

			g = (((src[0] >> 3) & 0xFC) + 
                 ((src[xOff] >> 3) & 0xFC) + 
                 ((src[diff] >> 3) & 0xFC) + 
                 ((src[diff + xOff] >> 3) & 0xFC));

			b = (((src[0] << 3) & 0xF8) + 
                 ((src[xOff] << 3) & 0xF8) + 
                 ((src[diff] << 3) & 0xF8) + 
                 ((src[diff + xOff] << 3) & 0xF8));

			*dest++ = ((r << 6) & 0xF800) | ((g << 1) & 0x07E0) | ((b >> 5) & 0x1F);
			src += 2;
		}
		src += width;
	}
}

bool Image::createMipMaps(){
	if (isCompressedFormat(format)) return false;

	int w = width, h = height;
	union {
		unsigned char *src;
		unsigned short *src16;
		float *src32f;
	};
	union {
		unsigned char *dest;
		unsigned short *dest16;
		float *dest32f;
	};

	if (nMipMaps <= 1){
		pixels = (unsigned char *) realloc(pixels, getMipMappedSize());
		nMipMaps = getNumberOfMipMapsFromDimesions();
	}
	dest = pixels;

	int nChannels = getChannels(format);

	while (w > 1 || h > 1){
		src = dest;
		if (isPlainFormat(format)){
			if (isFloatFormat(format)){
				dest32f += w * h * nChannels;
				buildMipMap32f(dest32f, src32f, w, h, nChannels);
			} else {
				dest += w * h * nChannels;
				buildMipMap8(dest, src, w, h, nChannels);
			}
		} else {
			dest16 += w * h;
			buildMipMapRGB565(dest16, src16, w, h);
		}
		//buildMipMapSinc(dest, src, w, h, channels, 3);

		if (w > 1) w >>= 1;
		if (h > 1) h >>= 1;
	}

	return true;
}

bool Image::colorMipMaps(){
	int nChannels = getChannels(format);

	if (nChannels < 3 || nMipMaps <= 1) return false;

	int size,len, i, level, w = width, h = height;
	bool flip, flip2;

	union {
		unsigned char  *src;
		unsigned short *src16;
		unsigned int   *src32;
	};
	
	src = pixels + getMipMappedSize(0, 1);

	static unsigned char colors[] = {
		0xFF, 0x00, 0x00, 0xFF, // Red
		0x00, 0xFF, 0x00, 0xFF, // Green
		0x00, 0x00, 0xFF, 0xFF, // Blue
		0xFF, 0xFF, 0x00, 0xFF, // Yellow
		0xFF, 0x00, 0xFF, 0xFF, // Magenta
		0x00, 0xFF, 0xFF, 0xFF, // Cyan
	};

	static unsigned short colors16[] = {
		0xF800, // Red
		0x07E0, // Green
		0x001F, // Blue
		0xFFE0, // Yellow
		0xF81F, // Magenta
		0x07FF, // Cyan
	};


	level = 0;
	do {
		if (w > 1) w >>= 1;
		if (h > 1) h >>= 1;

		if (isPlainFormat(format)){
			len = w * h * nChannels;
			for (i = 0; i < len; i++){
				src[i] &= colors[4 * level + (i % nChannels)];
			}
			src += len;
		} else if (isPackedFormat(format)){
			len = w * h;
			do {
				*src16++ &= colors16[level];
			} while (--len);
		} else if (isCompressedFormat(format)){
			len  = ((w + 3) >> 2) * ((h + 3) >> 2);
			size = (format == FORMAT_DXT1)? 8 : 16;
			do {
				flip = (src16[0] > src16[1]);

				src16[0] &= colors16[level];
				src16[1] &= colors16[level];

				flip2 = (src16[0] > src16[1]);

				if (flip != flip2){
					unsigned short temp = src16[0];
					src16[0] = src16[1];
					src16[1] = temp;

					if (flip){
						if (src16[0] == src16[1]){
							src16[0]++;
						}
						src32[1] ^= 0x55555555;
					} 
				}

				src += size;
			} while (--len);
		}
				
		level++;
		if (level >= 6) level = 0;
	} while (w > 1 || h > 1);

	return true;
}

void decodeColorBlock(unsigned char *dest, int w, int h, int xOff, int yOff, FORMAT format, int red, int blue, unsigned char *src){
	unsigned char colors[4][3];

	uint16 c0 = *(uint16 *) src;
	uint16 c1 = *(uint16 *) (src + 2);
	
	colors[0][0] = ((c0 >> 11) & 0x1F) << 3;
	colors[0][1] = ((c0 >>  5) & 0x3F) << 2;
	colors[0][2] =  (c0        & 0x1F) << 3;
	
	colors[1][0] = ((c1 >> 11) & 0x1F) << 3;
	colors[1][1] = ((c1 >>  5) & 0x3F) << 2;
	colors[1][2] =  (c1        & 0x1F) << 3;

	if (c0 > c1 || format == FORMAT_DXT5){
		for (int i = 0; i < 3; i++){
			colors[2][i] = (2 * colors[0][i] +     colors[1][i] + 1) / 3;
			colors[3][i] = (    colors[0][i] + 2 * colors[1][i] + 1) / 3;
		}
	} else {
		for (int i = 0; i < 3; i++){
			colors[2][i] = (colors[0][i] + colors[1][i] + 1) >> 1;
			colors[3][i] = 0;
		}
	}

	src += 4;
	for (int y = 0; y < h; y++){
		unsigned char *dst = dest + yOff * y;
		unsigned int indexes = src[y];
		for (int x = 0; x < w; x++){
			unsigned int index = indexes & 0x3;
			dst[red]  = colors[index][0];
			dst[1]    = colors[index][1];
			dst[blue] = colors[index][2];
			indexes >>= 2;

			dst += xOff;
		}
	}
}

void decodeDXT3AlphaBlock(unsigned char *dest, int w, int h, int xOff, int yOff, unsigned char *src){
	for (int y = 0; y < h; y++){
		unsigned char *dst = dest + yOff * y;
		unsigned int alpha = ((unsigned short *) src)[y];
		for (int x = 0; x < w; x++){
			*dst = (alpha & 0xF) * 17;
			alpha >>= 4;
			dst += xOff;
		}
	}
}

void decodeDXT5AlphaBlock(unsigned char *dest, int w, int h, int xOff, int yOff, unsigned char *src){
	unsigned char a0 = src[0];
	unsigned char a1 = src[1];
	uint64 alpha = (*(uint64 *) src) >> 16;

	for (int y = 0; y < h; y++){
		unsigned char *dst = dest + yOff * y;
		for (int x = 0; x < w; x++){
			int k = ((unsigned int) alpha) & 0x7;
			if (k == 0){
				*dst = a0;
			} else if (k == 1){
				*dst = a1;
			} else if (a0 > a1){
				*dst = ((8 - k) * a0 + (k - 1) * a1) / 7;
			} else if (k >= 6){
				*dst = (k == 6)? 0 : 255;
			} else {
				*dst = ((6 - k) * a0 + (k - 1) * a1) / 5;
			}
			alpha >>= 3;

			dst += xOff;
		}
		if (w < 4) alpha >>= (3 * (4 - w));
	}
}

void decodeCompressedImage(unsigned char *dest, unsigned char *src, const int width, const int height, const FORMAT format, const bool useRGBA){
	int sx = (width  < 4)? width  : 4;
	int sy = (height < 4)? height : 4;

	int nChannels = (format == FORMAT_DXT1 || format == FORMAT_3DC)? 3 : 4;

	int red  = useRGBA? 0 : 2;
	int blue = useRGBA? 2 : 0;

	for (int y = 0; y < height; y += 4){
		for (int x = 0; x < width; x += 4){
			unsigned char *dst = dest + (y * width + x) * nChannels;
			if (format == FORMAT_DXT3){
				decodeDXT3AlphaBlock(dst + 3, sx, sy, nChannels, width * nChannels, src);
				src += 8;
			} else if (format == FORMAT_DXT5){
				decodeDXT5AlphaBlock(dst + 3, sx, sy, nChannels, width * nChannels, src);
				src += 8;
			}
			if (format == FORMAT_3DC){
				decodeDXT5AlphaBlock(dst + red, sx, sy, nChannels, width * nChannels, src + 8);
				decodeDXT5AlphaBlock(dst + 1,   sx, sy, nChannels, width * nChannels, src);
				src += 16;
			} else {
				decodeColorBlock(dst, sx, sy, nChannels, width * nChannels, format, red, blue, src);
				src += 8;
			}
		}
	}
	if (format == FORMAT_3DC){
		unsigned char *dst = dest;
		for (int y = 0; y < height; y++){
			for (int x = 0; x < width; x++){
				float fx = (dst[red] - 127.5f);
				float fy = (dst[1] - 127.5f);
				float f = 16256.25f - fx * fx - fy * fy;
				if (f < 0) f = 0;
				dst[blue] = (unsigned char) (127.5f + sqrtf(f));
				dst += 3;
			}
		}
	}
}

bool Image::uncompressImage(const bool useRGBA){
	if (isCompressedFormat(format)){
		FORMAT destFormat = (format == FORMAT_DXT1 || format == FORMAT_3DC)? FORMAT_RGB8 : FORMAT_RGBA8;

		unsigned char *src, *dest, *newPixels = new unsigned char[getMipMappedSize(0, nMipMaps, destFormat)];
		int w = width, h = height;
		int i = 0;
		while ((src = getImage(i)) != NULL){
			dest = newPixels + getMipMappedSize(0, i, destFormat);
			
			decodeCompressedImage(dest, src, w, h, format, useRGBA);
			
			if (w > 1) w >>= 1;
			if (h > 1) h >>= 1;
			i++;
		}
		
		format = destFormat;
		
		free();
		pixels = newPixels;
	}

	return true;
}

bool Image::unpackImage(){
	if (isPackedFormat(format)){
		unsigned char *dest, *newPixels;
		unsigned short *src;
		int len;

		FORMAT destFormat = FORMAT_RGB8;

		dest = newPixels = new unsigned char[getMipMappedSize(0, nMipMaps, destFormat)];
		src = (unsigned short *) pixels;
		len = getNumberOfPixels(0, nMipMaps);

		do {
			*dest++ = ((*src) >> 8) & 0xF8;
			*dest++ = ((*src) >> 3) & 0xFC;
			*dest++ =  (*src) << 3;
			src++;
		} while (--len);
		
		format = destFormat;
		
		free();
		pixels = newPixels;
	}

	return true;
}

bool Image::convert(const FORMAT newFormat){
	if (!isPlainFormat(format) || !isPlainFormat(newFormat)) return false;
	if (format == newFormat) return true;


	unsigned char *src, *dest, *newPixels = new unsigned char[getMipMappedSize(0, nMipMaps, newFormat)];
	dest = newPixels;

	if (format == FORMAT_RGB8 && newFormat == FORMAT_RGBA8){
		unsigned int length = getNumberOfPixels(0, nMipMaps);
		src = pixels;
		do {
			dest[0] = src[0];
			dest[1] = src[1];
			dest[2] = src[2];
			dest[3] = 255;
			dest += 4;
			src += 3;
		} while (--length);

	} else {

		int srcBytesPerChannel = getBytesPerChannel(format);
		int nSrcChannels = getChannels(format);

		int destBytesPerChannel = getBytesPerChannel(newFormat);
		int nDestChannels = getChannels(newFormat);

		int level = 0;
		while ((src = getImage(level)) != NULL){
			int w = getWidth (level);
			int h = getHeight(level);

			for (int y = 0; y < h; y++){
				for (int x = 0; x < w; x++){
					float rgba[4];

					if (isFloatFormat(format)){
						for (int i = 0; i < nSrcChannels; i++) rgba[i] = ((float *) src)[i];
					} else {
						for (int i = 0; i < nSrcChannels; i++) rgba[i] = (float) src[i] * (1.0f / 255.0f);
					}
					if (nSrcChannels  < 4) rgba[3] = 1.0f;
					if (nSrcChannels == 1) rgba[2] = rgba[1] = rgba[0];
					
					if (nDestChannels == 1)	rgba[0] = 0.30f * rgba[0] + 0.59f * rgba[1] + 0.11f * rgba[2];

					if (isFloatFormat(newFormat)){
						for (int i = 0; i < nDestChannels; i++)	((float *) dest)[i] = rgba[i];
					} else {
						for (int i = 0; i < nDestChannels; i++)	dest[i] = (unsigned char) (255 * min(max(rgba[i], 0.0f), 1.0f));
					}

					src  += nSrcChannels  * srcBytesPerChannel;
					dest += nDestChannels * destBytesPerChannel;
				}
			}	


			level++;
		}

	}
	delete pixels;
	pixels = newPixels;
	format = newFormat;

	return true;
}

bool Image::removeMipMaps(const int removeMipMaps){
	if (removeMipMaps >= nMipMaps) return false;

	int size = getMipMappedSize(removeMipMaps, nMipMaps - removeMipMaps);
	unsigned char *newPixels = new unsigned char[size];

	memcpy(newPixels, getImage(removeMipMaps), size);

	delete pixels;
	pixels = newPixels;
	width = getWidth(removeMipMaps);
	height = getHeight(removeMipMaps);
	nMipMaps -= removeMipMaps;

	return true;
}

bool Image::resize(const int newWidth, const int newHeight, const ResizeMethod method){
	if (!isPlainFormat(format) || isFloatFormat(format)) return false;

	int nChannels = getChannels(format);

	unsigned char *newPixels = new unsigned char[newWidth * newHeight * nChannels];
	int x,y,k,sampleX, sampleY, wX, wY;
	unsigned char *src, *dest = newPixels;

	switch(method){
	case IMAGE_NEAREST:
		for (y = 0; y < newHeight; y++){
			sampleY = (height - 1) * y / (newHeight - 1);
			for (x = 0; x < newWidth; x++){
				sampleX = (width - 1) * x / (newWidth - 1);
				for (k = 0; k < nChannels; k++){
					*dest++ = pixels[(sampleY * width + sampleX) * nChannels + k];
				}
			}
		}
		break;
	case IMAGE_BILINEAR:
		for (y = 0; y < newHeight; y++){
			sampleY = (((height - 1) * y) << 8) / (newHeight - 1);
			if (y == newHeight - 1) sampleY--;
			wY = sampleY & 0xFF;
			sampleY >>= 8;

			for (x = 0; x < newWidth; x++){
				sampleX = (((width - 1) * x) << 8) / (newWidth - 1);
				wX = sampleX & 0xFF;
				sampleX >>= 8;

				src = pixels + (sampleY * width + sampleX) * nChannels;

				for (k = 0; k < nChannels; k++){
					*dest++ = (
						(256 - wX) * (256 - wY) * src[0] + 
						(256 - wX) * (      wY) * src[width * nChannels] + 
						(      wX) * (256 - wY) * src[nChannels] + 
 						(      wX) * (      wY) * src[(width + 1) * nChannels]) >> 16;
					*src++;
				}
			}
		}
		break;
	case IMAGE_BICUBIC:
		int a,b,c,d;
		int res;

		for (y = 0; y < newHeight; y++){
			sampleY = (((height - 1) * y) << 7) / (newHeight - 1);
			wY = sampleY & 0x7F;
			sampleY >>= 7;

			for (x = 0; x < newWidth; x++){
				sampleX = (((width - 1) * x) << 7) / (newWidth - 1);
				wX = sampleX & 0x7F;
				sampleX >>= 7;

				src = pixels + ((sampleY - 1) * width + (sampleX - 1)) * nChannels;

				for (k = 0; k < nChannels; k++){
					b = icerp(src[    width * nChannels], src[(    width + 1) * nChannels], src[(    width + 2) * nChannels], src[(    width + 3) * nChannels], wX);
					if (sampleY > 0){
						a = icerp(src[0], src[nChannels], src[2 * nChannels], src[3 * nChannels], wX);
					} else a = b;
					c = icerp(src[2 * width * nChannels], src[(2 * width + 1) * nChannels], src[(2 * width + 2) * nChannels], src[(2 * width + 3) * nChannels], wX);
					if (sampleY < newHeight - 1){
						d = icerp(src[3 * width * nChannels], src[(3 * width + 1) * nChannels], src[(3 * width + 2) * nChannels], src[(3 * width + 3) * nChannels], wX);
					} else d = c;

					res = icerp(a,b,c,d, wY);
					*dest++ = (res < 0)? 0 : (res > 255)? 255 : res;
					*src++;
				}
			}
		}
		break;
	}

	delete pixels;
	pixels = newPixels;
	width  = newWidth;
	height = newHeight;
	nMipMaps = 1;

	return true;
}


bool Image::toNormalMap(const bool useRGBA, const bool keepHeight){
	if (format != FORMAT_I8) return false;

	int blue, red;
	if (useRGBA){
		red  = 0;
		blue = 2;
	} else {
		red  = 2;
		blue = 0;
	}

	unsigned char *newPixels, *row0, *row1, *row2, *src, *dest;
	int x,y,w,h,mipmap;
	int sx, sy, len, predx, succx;

	format = keepHeight? FORMAT_RGBA8 : FORMAT_RGB8;
	dest = newPixels = new unsigned char[getMipMappedSize(0, nMipMaps)];

	mipmap = 0;
	w = width;
	h = height;

	src = pixels;

	do {
		row1 = src + (h - 1) * w;
		row2 = src;
		
		y = h;
		
		do {
			row0 = row1;
			row1 = row2;
			row2 += w;
			if (y == 1) row2 = src;
			
			x = w - 1;
			succx = 0;
			do {
				predx = x;
				x = succx++;
				if (succx == w) succx = 0;
				
				sx = (row0[predx] + 2 * row1[predx] + row2[predx]) - (row0[succx] + 2 * row1[succx] + row2[succx]);
				sy = (row0[predx] + 2 * row0[x]     + row0[succx]) - (row2[predx] + 2 * row2[x]     + row2[succx]);
				
				len = int(1044480.0f * rsqrtf(float(sx * sx + sy * sy + 65536)));
				sx *= len;
				sy *= len;
				
				dest[red]  = ((sx  + 1044480) >> 13);
				dest[1]    = ((sy  + 1044480) >> 13);
				dest[blue] = ((len +    4080) >> 5);
				if (keepHeight){
					dest[3] = row1[x];
					dest += 4;
				} else {
					dest += 3;
				}
				
			} while (succx);
		} while (--y);

		src += w * h;

		if (w > 1) w >>= 1;
		if (h > 1) h >>= 1;

		mipmap++;
	} while (mipmap < nMipMaps);

	delete pixels;
	pixels = newPixels;

	return true;
}

bool Image::toNormalMapAdvanced(float sZ, float mipMapScaleZ, const bool useRGBA){
	if (format != FORMAT_I8) return false;

	float sobelX[5][5] = {
		{ 1,  2,  0,  -2, -1 },
		{ 4,  8,  0,  -8, -4 },
		{ 6, 12,  0, -12, -6 },
		{ 4,  8,  0,  -8, -4 },
		{ 1,  2,  0,  -2, -1 },
	};
	float sobelY[5][5] = {
		{  1,  4,   6,  4,  1 },
		{  2,  8,  12,  8,  2 },
		{  0,  0,   0,  0,  0 },
		{ -2, -8, -12, -8, -2 },
		{ -1, -4,  -6, -4, -1 },
	};

	int xOff, zOff;
	if (useRGBA){
		xOff = 0;
		zOff = 2;
	} else {
		xOff = 2;
		zOff = 0;
	}

	sZ *= 128.0f / ((width > height)? width : height);

	unsigned char *newPixels = new unsigned char[3 * getMipMappedSize(0, nMipMaps)];
	unsigned char *dest = newPixels;

	for (int mipMap = 0; mipMap < nMipMaps; mipMap++){
		unsigned char *src = getImage(mipMap);

		int w = getWidth(mipMap);
		int h = getHeight(mipMap);

		for (int y = 0; y < h; y++){
			for (int x = 0; x < w; x++){
				float sX = 0;
				float sY = 0;
				for (int dy = 0; dy < 5; dy++){
					int fy = (y + dy - 2 + h) % h;
					for (int dx = 0; dx < 5; dx++){
						int fx = (x + dx - 2 + w) % w;

						sX += sobelX[dy][dx] * src[fy * w + fx];
						sY += sobelY[dy][dx] * src[fy * w + fx];
					}
				}
				sX *= 1.0f / (48 * 255);
				sY *= 1.0f / (48 * 255);
				float invLen = rsqrtf(sX * sX + sY * sY + sZ * sZ);

				dest[xOff] = (unsigned char) (127.5f * (sX * invLen + 1));
				dest[1]    = (unsigned char) (127.5f * (sY * invLen + 1));
				dest[zOff] = (unsigned char) (127.5f * (sZ * invLen + 1));
				dest += 3;
			}
		}
		sZ *= mipMapScaleZ;
	}

	format = FORMAT_RGB8;
	delete pixels;
	pixels = newPixels;
	return true;
}

bool Image::toGrayScale(const bool isRGBA, const bool reallocate){
	int nChannels = getChannels(format);

	if (!isPlainFormat(format) || isFloatFormat(format) || nChannels < 3) return false;

	unsigned int size, len;
	unsigned char *src = pixels, *dest = pixels;
	size = len = getNumberOfPixels(0, nMipMaps);

	if (isRGBA){
		do {
			*dest++ = (77 * src[0] + 151 * src[1] + 28 * src[2] + 128) >> 8;
			src += nChannels;
		} while (--len);
	} else {
		do {
			*dest++ = (77 * src[2] + 151 * src[1] + 28 * src[0] + 128) >> 8;
			src += nChannels;
		} while (--len);
	}

	format = FORMAT_I8;
	if (reallocate) pixels = (unsigned char *) realloc(pixels, size);

	return true;
}

bool Image::alphaFade(){
	int nChannels = getChannels(format);

	if (!isPlainFormat(format) || isFloatFormat(format) || nChannels < 3 || nMipMaps <= 1) return false;

	unsigned char *newPixels = new unsigned char[getMipMappedSize(0, nMipMaps, FORMAT_RGBA8)];

	unsigned char *src, *dest;
	unsigned char alpha;

	dest = newPixels;
	int level = 0;
	while ((src = getImage(level)) != NULL){
		alpha = ~(unsigned char) (255 * float(level) / (nMipMaps - 1));

		unsigned int nPixels = getNumberOfPixels(level, 1);
		for (unsigned int i = 0; i < nPixels; i++){
			dest[0] = src[0];
			dest[1] = src[1];
			dest[2] = src[2];
			dest[3] = alpha;

			dest += 4;
			src += nChannels;
		}
		level++;
	}

	delete pixels;
	pixels = newPixels;
	format = FORMAT_RGBA8;

	return true;
}


bool Image::laplace(const WrapMethod wrap){
	if (!isPlainFormat(format)) return false;

	int nChannels = getChannels(format);
	unsigned char *dest, *newPixels = new unsigned char[width * height * nChannels];
	
	dest = newPixels;
	int sum;
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			for (int i = 0; i < nChannels; i++){
				sum  = 4 * pixels[(y * width + x) * nChannels + i];
				sum -= pixels[(LOWRAP(y - 1, height, wrap) * width + x) * nChannels + i];
				sum -= pixels[(HIWRAP(y + 1, height, wrap) * width + x) * nChannels + i];
				sum -= pixels[(y * width + LOWRAP(x - 1, width, wrap)) * nChannels + i];
				sum -= pixels[(y * width + HIWRAP(x + 1, width, wrap)) * nChannels + i];
				
				
				//sum *= 3;
				//sum = abs(sum);
				if (sum < 0) sum = 0; else sum *= 4;
				if (sum > 255) sum = 255;
				
				*dest++ = sum;
			}
		}
	}
	nMipMaps = 1;
	delete pixels;
	pixels = newPixels;

	return true;
}
