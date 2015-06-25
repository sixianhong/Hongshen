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

#include "Font.h"
#include <stdio.h>

Character TextureFont::getCharRect(const char ch) const {
	return chars[(unsigned char) ch];
}

float TextureFont::getStringLength(char *string, int length) const {
	if (length < 0) length = strlen(string);

	float len = 0;
	for (int i = 0; i < length; i++){
		len += chars[(unsigned char) string[i]].ratio;
	}

	return len;
}


bool TextureFont::loadFromFile(const char *fileName){
	FILE *file = fopen(fileName, "rb");
	if (file == NULL) return false;

	unsigned int version = 0;
	fread(&version, sizeof(version), 1, file);
	fread(chars, sizeof(chars), 1, file);
	fclose(file);

/*	load(512, 512);
	saveToFile(fileName);
*/
	return true;
}

bool TextureFont::saveToFile(const char *fileName) const {
	FILE *file = fopen(fileName, "wb");
	if (file == NULL) return false;

	unsigned int version = 1;
	fwrite(&version, sizeof(version), 1, file);
	fwrite(chars, sizeof(chars), 1, file);
	fclose(file);

	return true;
}
