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

#ifndef _IMAGEUTILS_H_
#define _IMAGEUTILS_H_

inline void flipChannels(unsigned char *pixels, int nPixels, const int channels){
	unsigned char tmp;
	do {
		tmp = pixels[2];
		pixels[2] = pixels[0];
		pixels[0] = tmp;
		pixels += channels;
	} while (--nPixels);
}

inline int icerp(int a, int b, int c, int d, int x){
	int p = (d - c) - (a - b);
	int q = (a - b) - p;
	int r = c - a;
	return (x * (x * (x * p + (q << 7)) + (r << 14)) + (b << 21)) >> 21;
}

#endif // _IMAGEUTILS_H_
