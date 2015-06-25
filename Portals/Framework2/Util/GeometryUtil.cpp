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

#include "GeometryUtil.h"

unsigned int *getRectStripMO(const int size){
	unsigned int *indices = new unsigned int[getStripSize(size)];

	unsigned int *dest = indices;
	for (int j = 0; j < size - 1; j++){
		if ((j & 1) == 0){
			for (int i = 0; i < size; i++){
				*dest++ = mIndex2D(j, i);
				*dest++ = mIndex2D(j + 1, i);
			}
			*dest++ = mIndex2D(j + 1, size - 1);
		} else {
			for (int i = size - 1; i >= 0; i--){
				*dest++ = mIndex2D(j, i);
				*dest++ = mIndex2D(j + 1, i);
			}
			*dest++ = mIndex2D(j + 1, 0);
		}
	}

	return indices;
}
