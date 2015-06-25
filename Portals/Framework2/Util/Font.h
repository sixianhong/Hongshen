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

#ifndef _FONT_H_
#define _FONT_H_

#define _CRT_SECURE_NO_DEPRECATE
#include <string.h>

struct Character {
	float x0, y0;
	float x1, y1;
	float ratio;
};


class TextureFont {
public:
	TextureFont(){
		memset(chars, 0, sizeof(chars));
	}

	/*void load(int w, int h){
		struct Chr {
			char ch;
			int x, y;
			int w, h;
		};

		Chr chrs[] = {
			65, 0, 0, 43, 72,
			66, 44, 0, 38, 72,
			67, 83, 0, 38, 72,
			68, 122, 0, 38, 72,
			69, 161, 0, 34, 72,
			70, 196, 0, 33, 72,
			71, 230, 0, 38, 72,
			72, 269, 0, 39, 72,
			73, 309, 0, 20, 72,
			74, 330, 0, 35, 72,
			75, 366, 0, 33, 72,
			76, 400, 0, 34, 72,
			77, 435, 0, 53, 72,
			78, 0, 73, 43, 72,
			79, 44, 73, 38, 72,
			80, 83, 73, 39, 72,
			81, 123, 73, 43, 72,
			82, 167, 73, 43, 72,
			83, 211, 73, 38, 72,
			84, 250, 73, 42, 72,
			85, 293, 73, 38, 72,
			86, 332, 73, 38, 72,
			87, 371, 73, 52, 72,
			88, 424, 73, 43, 72,
			89, 468, 73, 33, 72,
			90, 0, 146, 34, 72,
			-59, 35, 146, 44, 72,
			-60, 80, 146, 44, 72,
			-42, 125, 146, 38, 72,
			97, 164, 146, 33, 72,
			98, 198, 146, 33, 72,
			99, 232, 146, 33, 72,
			100, 266, 146, 33, 72,
			101, 300, 146, 33, 72,
			102, 334, 146, 19, 72,
			103, 354, 146, 33, 72,
			104, 388, 146, 33, 72,
			105, 422-1, 146, 19, 72,
			106, 442, 146, 20, 72,
			107, 463+1, 146, 33, 72,
			108, 0, 219, 20, 72,
			109, 21, 219, 43, 72,
			110, 65, 219, 33, 72,
			111, 99, 219, 33, 72,
			112, 133, 219, 33, 72,
			113, 167, 219, 33, 72,
			114, 201, 219, 28, 72,
			115, 230, 219, 33, 72,
			116, 264, 219, 29, 72,
			117, 294, 219, 33, 72,
			118, 328, 219, 33, 72,
			119, 362, 219, 42, 72,
			120, 405, 219, 33, 72,
			121, 439, 219, 33, 72,
			122, 473, 219, 34, 72,
			'å', 0, 292, 33, 72,
			-28, 34, 292, 33, 72,
			-10, 68, 292, 33, 72,
			48, 102, 292, 38, 72,
			49, 141, 292, 29, 72,
			50, 171, 292, 38, 72,
			51, 210, 292, 38, 72,
			52, 249, 292, 33, 72,
			53, 283, 292, 39, 72,
			54, 323, 292, 38, 72,
			55, 362, 292, 33, 72,
			56, 396, 292, 38, 72,
			57, 435, 292, 38, 72,
			46, 474, 292, 20, 72,
			58, 0, 365, 20, 72,
			44, 21, 365, 20, 72,
			59, 42, 365, 20, 72,
			43, 63, 365, 33, 72,
			45, 97, 365, 33, 72,
			42, 131, 365, 42, 72,
			47, 174, 365, 33, 72,
			61, 208, 365, 33, 72,
			40, 242, 365, 25, 72,
			41, 268, 365, 25, 72,
			91, 294, 365, 20, 72,
			93, 315, 365, 20, 72,
			38, 336, 365, 48, 72,
			37, 385, 365, 42, 72,
			34, 428, 365, 24, 72,
			39, 453, 365, 15, 72,
			64, 0, 438, 48, 72,
			33, 49, 438, 20, 72,
			63, 70, 438, 35, 72,
			92, 106, 438, 33, 72,

			'>', 106+33+1,438,27,72,
			'<', 106+33+27+2,438,27,72,
			'_', 106+33+2*27+3,438,45,72,

			' ', 106+33+2*27+45+4,438,33,72,

		};

		for (unsigned int i = 0; i < sizeof(chrs) / sizeof(Chr); i++){
			int x = (unsigned char) chrs[i].ch;

			chars[x].x0 = float(chrs[i].x) / float(w - 1);
			chars[x].y0 = float(chrs[i].y) / float(h - 1);
			chars[x].x1 = float(chrs[i].x + chrs[i].w) / float(w - 1);
			chars[x].y1 = float(chrs[i].y + chrs[i].h) / float(h - 1);
			chars[x].ratio = float(chrs[i].w) / float(chrs[i].h);
		}
	}*/


	Character getCharRect(const char ch) const;
	float getStringLength(char *string, int length = -1) const;
	bool isCharDefined(const unsigned char ch) const { return (chars[ch].ratio != 0); }

	bool loadFromFile(const char *fileName);
	bool saveToFile(const char *fileName) const;

protected:
	Character chars[256];
};


#endif // _FONT_H_
