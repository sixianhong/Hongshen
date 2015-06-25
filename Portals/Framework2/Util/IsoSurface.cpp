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

#include "IsoSurface.h"

unsigned int indexList[256][16] = {
	0,		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	3,		 0,  8,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	3,		 0,  1,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	6,		 1,  8,  3,  9,  8,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	3,		 3, 11,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	6,		 0, 11,  2,  8, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	6,		 1,  9,  0,  2,  3, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 1, 11,  2,  1,  9, 11,  9,  8, 11,  0,  0,  0,  0,  0,  0,
	3,		 1,  2, 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	6,		 0,  8,  3,  1,  2, 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	6,		 9,  2, 10,  0,  2,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 2,  8,  3,  2, 10,  8, 10,  9,  8,  0,  0,  0,  0,  0,  0,
	6,		 3, 10,  1, 11, 10,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 0, 10,  1,  0,  8, 10,  8, 11, 10,  0,  0,  0,  0,  0,  0,
	9,		 3,  9,  0,  3, 11,  9, 11, 10,  9,  0,  0,  0,  0,  0,  0,
	6,		 9,  8, 10, 10,  8, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	3,		 4,  7,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	6,		 4,  3,  0,  7,  3,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	6,		 0,  1,  9,  8,  4,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 4,  1,  9,  4,  7,  1,  7,  3,  1,  0,  0,  0,  0,  0,  0,
	6,		 8,  4,  7,  3, 11,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		11,  4,  7, 11,  2,  4,  2,  0,  4,  0,  0,  0,  0,  0,  0,
	9,		 9,  0,  1,  8,  4,  7,  2,  3, 11,  0,  0,  0,  0,  0,  0,
	12,		 4,  7, 11,  9,  4, 11,  9, 11,  2,  9,  2,  1,  0,  0,  0,
	6,		 1,  2, 10,  8,  4,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 3,  4,  7,  3,  0,  4,  1,  2, 10,  0,  0,  0,  0,  0,  0,
	9,		 9,  2, 10,  9,  0,  2,  8,  4,  7,  0,  0,  0,  0,  0,  0,
	12,		 2, 10,  9,  2,  9,  7,  2,  7,  3,  7,  9,  4,  0,  0,  0,
	9,		 3, 10,  1,  3, 11, 10,  7,  8,  4,  0,  0,  0,  0,  0,  0,
	12,		 1, 11, 10,  1,  4, 11,  1,  0,  4,  7, 11,  4,  0,  0,  0,
	12,		 4,  7,  8,  9,  0, 11,  9, 11, 10, 11,  0,  3,  0,  0,  0,
	9,		 4,  7, 11,  4, 11,  9,  9, 11, 10,  0,  0,  0,  0,  0,  0,
	3,		 9,  5,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	6,		 9,  5,  4,  0,  8,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	6,		 0,  5,  4,  1,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 8,  5,  4,  8,  3,  5,  3,  1,  5,  0,  0,  0,  0,  0,  0,
	6,		 9,  5,  4,  2,  3, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 0, 11,  2,  0,  8, 11,  4,  9,  5,  0,  0,  0,  0,  0,  0,
	9,		 0,  5,  4,  0,  1,  5,  2,  3, 11,  0,  0,  0,  0,  0,  0,
	12,		 2,  1,  5,  2,  5,  8,  2,  8, 11,  4,  8,  5,  0,  0,  0,
	6,		 1,  2, 10,  9,  5,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 3,  0,  8,  1,  2, 10,  4,  9,  5,  0,  0,  0,  0,  0,  0,
	9,		 5,  2, 10,  5,  4,  2,  4,  0,  2,  0,  0,  0,  0,  0,  0,
	12,		 2, 10,  5,  3,  2,  5,  3,  5,  4,  3,  4,  8,  0,  0,  0,
	9,		10,  3, 11, 10,  1,  3,  9,  5,  4,  0,  0,  0,  0,  0,  0,
	12,		 4,  9,  5,  0,  8,  1,  8, 10,  1,  8, 11, 10,  0,  0,  0,
	12,		 5,  4,  0,  5,  0, 11,  5, 11, 10, 11,  0,  3,  0,  0,  0,
	9,		 5,  4,  8,  5,  8, 10, 10,  8, 11,  0,  0,  0,  0,  0,  0,
	6,		 9,  7,  8,  5,  7,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 9,  3,  0,  9,  5,  3,  5,  7,  3,  0,  0,  0,  0,  0,  0,
	9,		 0,  7,  8,  0,  1,  7,  1,  5,  7,  0,  0,  0,  0,  0,  0,
	6,		 1,  5,  3,  3,  5,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 7,  9,  5,  7,  8,  9,  3, 11,  2,  0,  0,  0,  0,  0,  0,
	12,		 9,  5,  7,  9,  7,  2,  9,  2,  0,  2,  7, 11,  0,  0,  0,
	12,		 2,  3, 11,  0,  1,  8,  1,  7,  8,  1,  5,  7,  0,  0,  0,
	9,		11,  2,  1, 11,  1,  7,  7,  1,  5,  0,  0,  0,  0,  0,  0,
	9,		 9,  7,  8,  9,  5,  7, 10,  1,  2,  0,  0,  0,  0,  0,  0,
	12,		10,  1,  2,  9,  5,  0,  5,  3,  0,  5,  7,  3,  0,  0,  0,
	12,		 8,  0,  2,  8,  2,  5,  8,  5,  7, 10,  5,  2,  0,  0,  0,
	9,		 2, 10,  5,  2,  5,  3,  3,  5,  7,  0,  0,  0,  0,  0,  0,
	12,		 9,  5,  8,  8,  5,  7, 10,  1,  3, 10,  3, 11,  0,  0,  0,
	15,		 5,  7,  0,  5,  0,  9,  7, 11,  0,  1,  0, 10, 11, 10,  0,
	15,		11, 10,  0, 11,  0,  3, 10,  5,  0,  8,  0,  7,  5,  7,  0,
	6,		11, 10,  5,  7, 11,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	3,		 7,  6, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	6,		 3,  0,  8, 11,  7,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	6,		 0,  1,  9, 11,  7,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 8,  1,  9,  8,  3,  1, 11,  7,  6,  0,  0,  0,  0,  0,  0,
	6,		 7,  2,  3,  6,  2,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 7,  0,  8,  7,  6,  0,  6,  2,  0,  0,  0,  0,  0,  0,  0,
	9,		 2,  7,  6,  2,  3,  7,  0,  1,  9,  0,  0,  0,  0,  0,  0,
	12,		 1,  6,  2,  1,  8,  6,  1,  9,  8,  8,  7,  6,  0,  0,  0,
	6,		10,  1,  2,  6, 11,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 1,  2, 10,  3,  0,  8,  6, 11,  7,  0,  0,  0,  0,  0,  0,
	9,		 2,  9,  0,  2, 10,  9,  6, 11,  7,  0,  0,  0,  0,  0,  0,
	12,		 6, 11,  7,  2, 10,  3, 10,  8,  3, 10,  9,  8,  0,  0,  0,
	9,		10,  7,  6, 10,  1,  7,  1,  3,  7,  0,  0,  0,  0,  0,  0,
	12,		10,  7,  6,  1,  7, 10,  1,  8,  7,  1,  0,  8,  0,  0,  0,
	12,		 0,  3,  7,  0,  7, 10,  0, 10,  9,  6, 10,  7,  0,  0,  0,
	9,		 7,  6, 10,  7, 10,  8,  8, 10,  9,  0,  0,  0,  0,  0,  0,
	6,		 6,  8,  4, 11,  8,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 3,  6, 11,  3,  0,  6,  0,  4,  6,  0,  0,  0,  0,  0,  0,
	9,		 8,  6, 11,  8,  4,  6,  9,  0,  1,  0,  0,  0,  0,  0,  0,
	12,		 9,  4,  6,  9,  6,  3,  9,  3,  1, 11,  3,  6,  0,  0,  0,
	9,		 8,  2,  3,  8,  4,  2,  4,  6,  2,  0,  0,  0,  0,  0,  0,
	6,		 0,  4,  2,  4,  6,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	12,		 1,  9,  0,  2,  3,  4,  2,  4,  6,  4,  3,  8,  0,  0,  0,
	9,		 1,  9,  4,  1,  4,  2,  2,  4,  6,  0,  0,  0,  0,  0,  0,
	9,		 6,  8,  4,  6, 11,  8,  2, 10,  1,  0,  0,  0,  0,  0,  0,
	12,		 1,  2, 10,  3,  0, 11,  0,  6, 11,  0,  4,  6,  0,  0,  0,
	12,		 4, 11,  8,  4,  6, 11,  0,  2,  9,  2, 10,  9,  0,  0,  0,
	15,		10,  9,  3, 10,  3,  2,  9,  4,  3, 11,  3,  6,  4,  6,  3,
	12,		 8,  1,  3,  8,  6,  1,  8,  4,  6,  6, 10,  1,  0,  0,  0,
	9,		10,  1,  0, 10,  0,  6,  6,  0,  4,  0,  0,  0,  0,  0,  0,
	15,		 4,  6,  3,  4,  3,  8,  6, 10,  3,  0,  3,  9, 10,  9,  3,
	6,		10,  9,  4,  6, 10,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	6,		 4,  9,  5,  7,  6, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 0,  8,  3,  4,  9,  5, 11,  7,  6,  0,  0,  0,  0,  0,  0,
	9,		 5,  0,  1,  5,  4,  0,  7,  6, 11,  0,  0,  0,  0,  0,  0,
	12,		11,  7,  6,  8,  3,  4,  3,  5,  4,  3,  1,  5,  0,  0,  0,
	9,		 7,  2,  3,  7,  6,  2,  5,  4,  9,  0,  0,  0,  0,  0,  0,
	12,		 9,  5,  4,  0,  8,  6,  0,  6,  2,  6,  8,  7,  0,  0,  0,
	12,		 3,  6,  2,  3,  7,  6,  1,  5,  0,  5,  4,  0,  0,  0,  0,
	15,		 6,  2,  8,  6,  8,  7,  2,  1,  8,  4,  8,  5,  1,  5,  8,
	9,		 9,  5,  4, 10,  1,  2,  7,  6, 11,  0,  0,  0,  0,  0,  0,
	12,		 6, 11,  7,  1,  2, 10,  0,  8,  3,  4,  9,  5,  0,  0,  0,
	12,		 7,  6, 11,  5,  4, 10,  4,  2, 10,  4,  0,  2,  0,  0,  0,
	15,		 3,  4,  8,  3,  5,  4,  3,  2,  5, 10,  5,  2, 11,  7,  6,
	12,		 9,  5,  4, 10,  1,  6,  1,  7,  6,  1,  3,  7,  0,  0,  0,
	15,		 1,  6, 10,  1,  7,  6,  1,  0,  7,  8,  7,  0,  9,  5,  4,
	15,		 4,  0, 10,  4, 10,  5,  0,  3, 10,  6, 10,  7,  3,  7, 10,
	12,		 7,  6, 10,  7, 10,  8,  5,  4, 10,  4,  8, 10,  0,  0,  0,
	9,		 6,  9,  5,  6, 11,  9, 11,  8,  9,  0,  0,  0,  0,  0,  0,
	12,		 3,  6, 11,  0,  6,  3,  0,  5,  6,  0,  9,  5,  0,  0,  0,
	12,		 0, 11,  8,  0,  5, 11,  0,  1,  5,  5,  6, 11,  0,  0,  0,
	9,		 6, 11,  3,  6,  3,  5,  5,  3,  1,  0,  0,  0,  0,  0,  0,
	12,		 5,  8,  9,  5,  2,  8,  5,  6,  2,  3,  8,  2,  0,  0,  0,
	9,		 9,  5,  6,  9,  6,  0,  0,  6,  2,  0,  0,  0,  0,  0,  0,
	15,		 1,  5,  8,  1,  8,  0,  5,  6,  8,  3,  8,  2,  6,  2,  8,
	6,		 1,  5,  6,  2,  1,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	12,		 1,  2, 10,  9,  5, 11,  9, 11,  8, 11,  5,  6,  0,  0,  0,
	15,		 0, 11,  3,  0,  6, 11,  0,  9,  6,  5,  6,  9,  1,  2, 10,
	15,		11,  8,  5, 11,  5,  6,  8,  0,  5, 10,  5,  2,  0,  2,  5,
	12,		 6, 11,  3,  6,  3,  5,  2, 10,  3, 10,  5,  3,  0,  0,  0,
	15,		 1,  3,  6,  1,  6, 10,  3,  8,  6,  5,  6,  9,  8,  9,  6,
	12,		10,  1,  0, 10,  0,  6,  9,  5,  0,  5,  6,  0,  0,  0,  0,
	6,		 0,  3,  8,  5,  6, 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	3,		10,  5,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	3,		10,  6,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	6,		 0,  8,  3,  5, 10,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	6,		 9,  0,  1,  5, 10,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 1,  8,  3,  1,  9,  8,  5, 10,  6,  0,  0,  0,  0,  0,  0,
	6,		 2,  3, 11, 10,  6,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		11,  0,  8, 11,  2,  0, 10,  6,  5,  0,  0,  0,  0,  0,  0,
	9,		 0,  1,  9,  2,  3, 11,  5, 10,  6,  0,  0,  0,  0,  0,  0,
	12,		 5, 10,  6,  1,  9,  2,  9, 11,  2,  9,  8, 11,  0,  0,  0,
	6,		 1,  6,  5,  2,  6,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 1,  6,  5,  1,  2,  6,  3,  0,  8,  0,  0,  0,  0,  0,  0,
	9,		 9,  6,  5,  9,  0,  6,  0,  2,  6,  0,  0,  0,  0,  0,  0,
	12,		 5,  9,  8,  5,  8,  2,  5,  2,  6,  3,  2,  8,  0,  0,  0,
	9,		 6,  3, 11,  6,  5,  3,  5,  1,  3,  0,  0,  0,  0,  0,  0,
	12,		 0,  8, 11,  0, 11,  5,  0,  5,  1,  5, 11,  6,  0,  0,  0,
	12,		 3, 11,  6,  0,  3,  6,  0,  6,  5,  0,  5,  9,  0,  0,  0,
	9,		 6,  5,  9,  6,  9, 11, 11,  9,  8,  0,  0,  0,  0,  0,  0,
	6,		 5, 10,  6,  4,  7,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 4,  3,  0,  4,  7,  3,  6,  5, 10,  0,  0,  0,  0,  0,  0,
	9,		 1,  9,  0,  5, 10,  6,  8,  4,  7,  0,  0,  0,  0,  0,  0,
	12,		10,  6,  5,  1,  9,  7,  1,  7,  3,  7,  9,  4,  0,  0,  0,
	9,		 3, 11,  2,  7,  8,  4, 10,  6,  5,  0,  0,  0,  0,  0,  0,
	12,		 5, 10,  6,  4,  7,  2,  4,  2,  0,  2,  7, 11,  0,  0,  0,
	12,		 0,  1,  9,  4,  7,  8,  2,  3, 11,  5, 10,  6,  0,  0,  0,
	15,		 9,  2,  1,  9, 11,  2,  9,  4, 11,  7, 11,  4,  5, 10,  6,
	9,		 6,  1,  2,  6,  5,  1,  4,  7,  8,  0,  0,  0,  0,  0,  0,
	12,		 1,  2,  5,  5,  2,  6,  3,  0,  4,  3,  4,  7,  0,  0,  0,
	12,		 8,  4,  7,  9,  0,  5,  0,  6,  5,  0,  2,  6,  0,  0,  0,
	15,		 7,  3,  9,  7,  9,  4,  3,  2,  9,  5,  9,  6,  2,  6,  9,
	12,		 8,  4,  7,  3, 11,  5,  3,  5,  1,  5, 11,  6,  0,  0,  0,
	15,		 5,  1, 11,  5, 11,  6,  1,  0, 11,  7, 11,  4,  0,  4, 11,
	15,		 0,  5,  9,  0,  6,  5,  0,  3,  6, 11,  6,  3,  8,  4,  7,
	12,		 6,  5,  9,  6,  9, 11,  4,  7,  9,  7, 11,  9,  0,  0,  0,
	6,		10,  4,  9,  6,  4, 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 4, 10,  6,  4,  9, 10,  0,  8,  3,  0,  0,  0,  0,  0,  0,
	9,		10,  0,  1, 10,  6,  0,  6,  4,  0,  0,  0,  0,  0,  0,  0,
	12,		 8,  3,  1,  8,  1,  6,  8,  6,  4,  6,  1, 10,  0,  0,  0,
	9,		10,  4,  9, 10,  6,  4, 11,  2,  3,  0,  0,  0,  0,  0,  0,
	12,		 0,  8,  2,  2,  8, 11,  4,  9, 10,  4, 10,  6,  0,  0,  0,
	12,		 3, 11,  2,  0,  1,  6,  0,  6,  4,  6,  1, 10,  0,  0,  0,
	15,		 6,  4,  1,  6,  1, 10,  4,  8,  1,  2,  1, 11,  8, 11,  1,
	9,		 1,  4,  9,  1,  2,  4,  2,  6,  4,  0,  0,  0,  0,  0,  0,
	12,		 3,  0,  8,  1,  2,  9,  2,  4,  9,  2,  6,  4,  0,  0,  0,
	6,		 0,  2,  4,  4,  2,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 8,  3,  2,  8,  2,  4,  4,  2,  6,  0,  0,  0,  0,  0,  0,
	12,		 9,  6,  4,  9,  3,  6,  9,  1,  3, 11,  6,  3,  0,  0,  0,
	15,		 8, 11,  1,  8,  1,  0, 11,  6,  1,  9,  1,  4,  6,  4,  1,
	9,		 3, 11,  6,  3,  6,  0,  0,  6,  4,  0,  0,  0,  0,  0,  0,
	6,		 6,  4,  8, 11,  6,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 7, 10,  6,  7,  8, 10,  8,  9, 10,  0,  0,  0,  0,  0,  0,
	12,		 0,  7,  3,  0, 10,  7,  0,  9, 10,  6,  7, 10,  0,  0,  0,
	12,		10,  6,  7,  1, 10,  7,  1,  7,  8,  1,  8,  0,  0,  0,  0,
	9,		10,  6,  7, 10,  7,  1,  1,  7,  3,  0,  0,  0,  0,  0,  0,
	12,		 2,  3, 11, 10,  6,  8, 10,  8,  9,  8,  6,  7,  0,  0,  0,
	15,		 2,  0,  7,  2,  7, 11,  0,  9,  7,  6,  7, 10,  9, 10,  7,
	15,		 1,  8,  0,  1,  7,  8,  1, 10,  7,  6,  7, 10,  2,  3, 11,
	12,		11,  2,  1, 11,  1,  7, 10,  6,  1,  6,  7,  1,  0,  0,  0,
	12,		 1,  2,  6,  1,  6,  8,  1,  8,  9,  8,  6,  7,  0,  0,  0,
	15,		 2,  6,  9,  2,  9,  1,  6,  7,  9,  0,  9,  3,  7,  3,  9,
	9,		 7,  8,  0,  7,  0,  6,  6,  0,  2,  0,  0,  0,  0,  0,  0,
	6,		 7,  3,  2,  6,  7,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	15,		 8,  9,  6,  8,  6,  7,  9,  1,  6, 11,  6,  3,  1,  3,  6,
	6,		 0,  9,  1, 11,  6,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	12,		 7,  8,  0,  7,  0,  6,  3, 11,  0, 11,  6,  0,  0,  0,  0,
	3,		 7, 11,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	6,		11,  5, 10,  7,  5, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		11,  5, 10, 11,  7,  5,  8,  3,  0,  0,  0,  0,  0,  0,  0,
	9,		 5, 11,  7,  5, 10, 11,  1,  9,  0,  0,  0,  0,  0,  0,  0,
	12,		10,  7,  5, 10, 11,  7,  9,  8,  1,  8,  3,  1,  0,  0,  0,
	9,		 2,  5, 10,  2,  3,  5,  3,  7,  5,  0,  0,  0,  0,  0,  0,
	12,		 8,  2,  0,  8,  5,  2,  8,  7,  5, 10,  2,  5,  0,  0,  0,
	12,		 9,  0,  1,  5, 10,  3,  5,  3,  7,  3, 10,  2,  0,  0,  0,
	15,		 9,  8,  2,  9,  2,  1,  8,  7,  2, 10,  2,  5,  7,  5,  2,
	9,		11,  1,  2, 11,  7,  1,  7,  5,  1,  0,  0,  0,  0,  0,  0,
	12,		 0,  8,  3,  1,  2,  7,  1,  7,  5,  7,  2, 11,  0,  0,  0,
	12,		 9,  7,  5,  9,  2,  7,  9,  0,  2,  2, 11,  7,  0,  0,  0,
	15,		 7,  5,  2,  7,  2, 11,  5,  9,  2,  3,  2,  8,  9,  8,  2,
	6,		 1,  3,  5,  3,  7,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 0,  8,  7,  0,  7,  1,  1,  7,  5,  0,  0,  0,  0,  0,  0,
	9,		 9,  0,  3,  9,  3,  5,  5,  3,  7,  0,  0,  0,  0,  0,  0,
	6,		 9,  8,  7,  5,  9,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 5,  8,  4,  5, 10,  8, 10, 11,  8,  0,  0,  0,  0,  0,  0,
	12,		 5,  0,  4,  5, 11,  0,  5, 10, 11, 11,  3,  0,  0,  0,  0,
	12,		 0,  1,  9,  8,  4, 10,  8, 10, 11, 10,  4,  5,  0,  0,  0,
	15,		10, 11,  4, 10,  4,  5, 11,  3,  4,  9,  4,  1,  3,  1,  4,
	12,		 2,  5, 10,  3,  5,  2,  3,  4,  5,  3,  8,  4,  0,  0,  0,
	9,		 5, 10,  2,  5,  2,  4,  4,  2,  0,  0,  0,  0,  0,  0,  0,
	15,		 3, 10,  2,  3,  5, 10,  3,  8,  5,  4,  5,  8,  0,  1,  9,
	12,		 5, 10,  2,  5,  2,  4,  1,  9,  2,  9,  4,  2,  0,  0,  0,
	12,		 2,  5,  1,  2,  8,  5,  2, 11,  8,  4,  5,  8,  0,  0,  0,
	15,		 0,  4, 11,  0, 11,  3,  4,  5, 11,  2, 11,  1,  5,  1, 11,
	15,		 0,  2,  5,  0,  5,  9,  2, 11,  5,  4,  5,  8, 11,  8,  5,
	6,		 9,  4,  5,  2, 11,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 8,  4,  5,  8,  5,  3,  3,  5,  1,  0,  0,  0,  0,  0,  0,
	6,		 0,  4,  5,  1,  0,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	12,		 8,  4,  5,  8,  5,  3,  9,  0,  5,  0,  3,  5,  0,  0,  0,
	3,		 9,  4,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 4, 11,  7,  4,  9, 11,  9, 10, 11,  0,  0,  0,  0,  0,  0,
	12,		 0,  8,  3,  4,  9,  7,  9, 11,  7,  9, 10, 11,  0,  0,  0,
	12,		 1, 10, 11,  1, 11,  4,  1,  4,  0,  7,  4, 11,  0,  0,  0,
	15,		 3,  1,  4,  3,  4,  8,  1, 10,  4,  7,  4, 11, 10, 11,  4,
	12,		 2,  9, 10,  2,  7,  9,  2,  3,  7,  7,  4,  9,  0,  0,  0,
	15,		 9, 10,  7,  9,  7,  4, 10,  2,  7,  8,  7,  0,  2,  0,  7,
	15,		 3,  7, 10,  3, 10,  2,  7,  4, 10,  1, 10,  0,  4,  0, 10,
	6,		 1, 10,  2,  8,  7,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	12,		 4, 11,  7,  9, 11,  4,  9,  2, 11,  9,  1,  2,  0,  0,  0,
	15,		 9,  7,  4,  9, 11,  7,  9,  1, 11,  2, 11,  1,  0,  8,  3,
	9,		11,  7,  4, 11,  4,  2,  2,  4,  0,  0,  0,  0,  0,  0,  0,
	12,		11,  7,  4, 11,  4,  2,  8,  3,  4,  3,  2,  4,  0,  0,  0,
	9,		 4,  9,  1,  4,  1,  7,  7,  1,  3,  0,  0,  0,  0,  0,  0,
	12,		 4,  9,  1,  4,  1,  7,  0,  8,  1,  8,  7,  1,  0,  0,  0,
	6,		 4,  0,  3,  7,  4,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	3,		 4,  8,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	6,		 9, 10,  8, 10, 11,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 3,  0,  9,  3,  9, 11, 11,  9, 10,  0,  0,  0,  0,  0,  0,
	9,		 0,  1, 10,  0, 10,  8,  8, 10, 11,  0,  0,  0,  0,  0,  0,
	6,		 3,  1, 10, 11,  3, 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 2,  3,  8,  2,  8, 10, 10,  8,  9,  0,  0,  0,  0,  0,  0,
	6,		 9, 10,  2,  0,  9,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	12,		 2,  3,  8,  2,  8, 10,  0,  1,  8,  1, 10,  8,  0,  0,  0,
	3,		 1, 10,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	9,		 1,  2, 11,  1, 11,  9,  9, 11,  8,  0,  0,  0,  0,  0,  0,
	12,		 3,  0,  9,  3,  9, 11,  1,  2,  9,  2, 11,  9,  0,  0,  0,
	6,		 0,  2, 11,  8,  0, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	3,		 3,  2, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	6,		 1,  3,  8,  9,  1,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	3,		 0,  9,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	3,		 0,  3,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

#define NEW_EDGE(dst, n0, n1) \
	edge->node0 = cell->nodes[n0]; \
	edge->node1 = cell->nodes[n1]; \
	cell->edges[dst] = edge; \
	edge++;

#define OLD_EDGE(dst, x, y, z, n) cell->edges[dst] = cells[((z) * sY + (y)) * sX + (x)].edges[n];

DWORD WINAPI thread(LPVOID param){
	return ((IsoSurface *) param)->workerThread();
}

IsoSurface::IsoSurface(const int threadCount){
	nodes = NULL;
	cells = NULL;
	edges = NULL;
	intersectedEdges = NULL;

	vertexArray = NULL;
	indexArray  = NULL;
	vaSize = 0;
	iaSize = 0;

#ifdef _WIN32
	nThreads = threadCount;
	threadHandles = new HANDLE[nThreads];
	threadIDs = new DWORD[nThreads];
	events = new HANDLE[nThreads];

	for (int i = 0; i < nThreads; i++){
		threadHandles[i] = CreateThread(NULL, 0, thread, this, 0, &threadIDs[i]);

		events[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	param = new Param[nThreads];
#endif
}

IsoSurface::~IsoSurface(){
	delete indexArray;
	delete vertexArray;

	delete intersectedEdges;
	delete edges;
	delete cells;
	delete nodes;

#ifdef _WIN32
	for (int i = 0; i < nThreads; i++){
		PostThreadMessage(threadIDs[i], WM_QUIT, 0, 0);
	}
	WaitForMultipleObjects(nThreads, threadHandles, TRUE, INFINITE);
	for (int n = 0; n < nThreads; n++){
		CloseHandle(events[n]);
	}
	delete [] param;
	delete [] threadHandles;
	delete [] threadIDs;
	delete [] events;
#endif
}

int IsoSurface::getEdgeCount() const {
	return 3 * sizeX * sizeY * sizeZ + 2 * (sizeX * sizeY + sizeX * sizeZ + sizeY * sizeZ) + sizeX + sizeY + sizeZ;
}

void IsoSurface::setSize(const vec3 &minLim, const vec3 &maxLim, const int sX, const int sY, const int sZ){
	sizeX = sX;
	sizeY = sY;
	sizeZ = sZ;

	// Direction vectors for normal computation
	dx = vec3((maxLim.x - minLim.x) / float(8 * sX), 0, 0);
	dy = vec3(0, (maxLim.y - minLim.y) / float(8 * sY), 0);
	dz = vec3(0, 0, (maxLim.z - minLim.z) / float(8 * sZ));


	delete nodes;
	nodes = new GridNode[(sX + 1) * (sY + 1) * (sZ + 1)];
	GridNode *node = nodes;

	// Setup nodes
	vec3 pos;
	int x, y, z;
	for (z = 0; z <= sZ; z++){
		pos.z = lerp(minLim.z, maxLim.z, float(z) / sZ);
		for (y = 0; y <= sY; y++){
			pos.y = lerp(minLim.y, maxLim.y, float(y) / sY);
			for (x = 0; x <= sX; x++){
				pos.x = lerp(minLim.x, maxLim.x, float(x) / sX);

				node->vertex = pos;
				node++;
			}
		}
	}

	delete edges;
	edges = new Edge[getEdgeCount()];
	Edge *edge = edges;

	intersectedEdges = new Edge *[getEdgeCount()];

	delete cells;
	cells = new Cell[sX * sY * sZ];
	Cell *cell = cells;

	// Setup cells and edges
	for (z = 0; z < sZ; z++){
		for (y = 0; y < sY; y++){
			for (x = 0; x < sX; x++){
				cell->nodes[0] = nodes + ( z      * (sY + 1) + y    ) * (sX + 1) + x;
				cell->nodes[1] = nodes + ( z      * (sY + 1) + y    ) * (sX + 1) + x + 1;
				cell->nodes[2] = nodes + ( z      * (sY + 1) + y + 1) * (sX + 1) + x;
				cell->nodes[3] = nodes + ( z      * (sY + 1) + y + 1) * (sX + 1) + x + 1;
				cell->nodes[4] = nodes + ((z + 1) * (sY + 1) + y    ) * (sX + 1) + x;
				cell->nodes[5] = nodes + ((z + 1) * (sY + 1) + y    ) * (sX + 1) + x + 1;
				cell->nodes[6] = nodes + ((z + 1) * (sY + 1) + y + 1) * (sX + 1) + x;
				cell->nodes[7] = nodes + ((z + 1) * (sY + 1) + y + 1) * (sX + 1) + x + 1;

				if (z == 0){
					if (y == 0){
						NEW_EDGE(0,  0, 1);
					} else {
						OLD_EDGE(0, x, y - 1, z, 2);
					}
					NEW_EDGE(1,  1, 3);
					NEW_EDGE(2,  2, 3);
					if (x == 0){
						NEW_EDGE(3,  0, 2);
					} else {
						OLD_EDGE(3, x - 1, y, z, 1);
					}
				} else {
					OLD_EDGE(0, x, y, z - 1, 4);
					OLD_EDGE(1, x, y, z - 1, 5);
					OLD_EDGE(2, x, y, z - 1, 6);
					OLD_EDGE(3, x, y, z - 1, 7);
				}


				if (y == 0){
					NEW_EDGE(4,  4, 5);
				} else {
					OLD_EDGE(4, x, y - 1, z, 6);
				}
				NEW_EDGE(5,  5, 7);
				NEW_EDGE(6,  6, 7);
				if (x == 0){
					NEW_EDGE(7,  4, 6);
				} else {
					OLD_EDGE(7, x - 1, y, z, 5);
				}


				if (y == 0){
					if (x == 0){
						NEW_EDGE(8,  0, 4);
					} else {
						OLD_EDGE(8, x - 1, y, z, 9);
					}
					if (y == 0){
						NEW_EDGE(9,  1, 5);
					}
				} else {
					OLD_EDGE(8, x, y - 1, z, 11);
					OLD_EDGE(9, x, y - 1, z, 10);
				}
				NEW_EDGE(10,  3, 7);
				if (x == 0){
					NEW_EDGE(11,  2, 6);
				} else {
					OLD_EDGE(11, x - 1, y, z, 10);
				}

				cell++;
			}
		}
	}
}

inline vec3 interpolate(const vec3 &p0, const vec3 &p1, const float v0, const float v1){
	return p0 + (v0 / (v0 - v1)) * (p1 - p0);
}

void IsoSurface::computeField(FIELD_FUNC field, const int start, const int end){
	for (int i = start; i < end; i++){
		nodes[i].value = field(nodes[i].vertex);
	}
}

void IsoSurface::computeVertexArray(FIELD_FUNC field, const int start, const int end){
	// Compute all vertices and normals
	for (int i = start; i < end; i++){
		Edge *edge = intersectedEdges[i];
		vec3 vertex = interpolate(edge->node0->vertex, edge->node1->vertex, edge->node0->value, edge->node1->value);

		vec3 normal;
		float val = field(vertex);
		normal.x = val - field(vertex + dx);
		normal.y = val - field(vertex + dy);
		normal.z = val - field(vertex + dz);

		vertexArray[i].vertex = vertex;
		vertexArray[i].normal = normalize(normal);
	}
}

void IsoSurface::computeIndexArray(){
	nIndices = 0;

	// Compute the mesh topology
	Cell *cell = cells;
	int nCells = sizeX * sizeY * sizeZ;
	unsigned int *iDest = indexArray;
	for (int c = 0; c < nCells; c++){
		// Extract all sign bits
		unsigned int cubeIndex;
		cubeIndex  = ((*(unsigned int *) &cell->nodes[0]->value) >> 31);
		cubeIndex |= ((*(unsigned int *) &cell->nodes[1]->value) >> 30) & 0x2;
		cubeIndex |= ((*(unsigned int *) &cell->nodes[2]->value) >> 29) & 0x4;
		cubeIndex |= ((*(unsigned int *) &cell->nodes[3]->value) >> 28) & 0x8;
		cubeIndex |= ((*(unsigned int *) &cell->nodes[4]->value) >> 27) & 0x10;
		cubeIndex |= ((*(unsigned int *) &cell->nodes[5]->value) >> 26) & 0x20;
		cubeIndex |= ((*(unsigned int *) &cell->nodes[6]->value) >> 25) & 0x40;
		cubeIndex |= ((*(unsigned int *) &cell->nodes[7]->value) >> 24) & 0x80;

		unsigned int nInds = indexList[cubeIndex][0];
		if (nInds){
			unsigned int newIndexCount = nIndices + nInds;

			// Allocate more memory in neccesary
			if (newIndexCount >= iaSize){
				iaSize += 64;
				indexArray = (unsigned int *) realloc(indexArray, iaSize * sizeof(unsigned int));
				iDest = indexArray + nIndices;
			}

			unsigned int *inds = indexList[cubeIndex] + 1;
			for (unsigned int i = 0; i < nInds; i++){
				*iDest++ = cell->edges[inds[i]]->index;
			}

			nIndices = newIndexCount;
		}
		cell++;
	}
}

int IsoSurface::workerThread(){
#ifdef _WIN32
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0){
		Param *p = param + msg.wParam;
		switch (msg.message){
		case COMPUTE_FIELD:
			computeField((FIELD_FUNC) msg.lParam, p->start, p->end);
			SetEvent(events[msg.wParam]);
			break;
		case COMPUTE_VERTEX_ARRAY:
			computeVertexArray((FIELD_FUNC) msg.lParam, p->start, p->end);
			SetEvent(events[msg.wParam]);
			break;
		}
	}
#endif

	return 0;
}

void IsoSurface::update(FIELD_FUNC field, const int maxThreads){
	int nNodes = (sizeX + 1) * (sizeY + 1) * (sizeZ + 1);
#ifdef _WIN32
	int count = nNodes / maxThreads;
	for (int i = 0; i < maxThreads; i++){
		param[i].start = i * count;
		if (i < maxThreads - 1){
			param[i].end = param[i].start + count;
		} else {
			param[i].end = nNodes;
		}
		PostThreadMessage(threadIDs[i], COMPUTE_FIELD, i, (LPARAM) field);
	}
	WaitForMultipleObjects(maxThreads, events, TRUE, INFINITE);
#else
	computeField(field, 0, nNodes);
#endif

	// Assigned indices
	unsigned int nEdges = getEdgeCount();
	nVertices = 0;
	for (unsigned int e = 0; e < nEdges; e++){
		if (((*(unsigned int *) &edges[e].node0->value) ^ (*(unsigned int *) &edges[e].node1->value)) >> 31){
			edges[e].index = nVertices;
			intersectedEdges[nVertices] = edges + e;

			nVertices++;
		}
	}

	// Allocate more memory if neccesary
	if (nVertices >= vaSize){
		vaSize = nVertices;
		vertexArray = (ISVertex *) realloc(vertexArray, vaSize * sizeof(ISVertex));
	}

#ifdef _WIN32
/*
	count = nVertices / maxThreads;
	for (int i = 0; i < maxThreads; i++){
		param[i].start = i * count;
		if (i < maxThreads - 1){
			param[i].end = param[i].start + count;
		} else {
			param[i].end = nVertices;
		}

		PostThreadMessage(threadIDs[i], COMPUTE_VERTEX_ARRAY, i, (LPARAM) field);
	}
*/
	if (maxThreads > 1){
		param[0].start = 0;
		param[0].end = nVertices;

		PostThreadMessage(threadIDs[0], COMPUTE_VERTEX_ARRAY, 0, (LPARAM) field);
	} else
#endif
		computeVertexArray(field, 0, nVertices);

	computeIndexArray();

#ifdef _WIN32
	if (maxThreads > 1){
		WaitForSingleObject(events[0], INFINITE);
	}
//	WaitForMultipleObjects(maxThreads, events, TRUE, INFINITE);
#endif
}
