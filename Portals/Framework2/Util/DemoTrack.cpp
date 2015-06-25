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

#include "DemoTrack.h"
#include <stdio.h>

DemoTrack::DemoTrack(){
	nodeSize = 1;
	clampIndices = false;
}

int clamp(int x, const int len){
	if (x < 0) return 0;
	if (x >= len) return len - 1; else return x;
}

int wrap(int x, const int len){
	x %= len;
	if (x < 0) x += len;
	return x;
}

void DemoTrack::getAt(const float t, float *dest, unsigned int pos, unsigned int count) const {
	int len, i0, i1, i2, i3;

	len = nodes.getCount() / nodeSize;
	int ind = (int) floorf(t);
	float tFrac = t - ind;

	if (clampIndices){
		i0 = clamp(ind - 1, len);
		i1 = clamp(ind,     len);
		i2 = clamp(ind + 1, len);
		i3 = clamp(ind + 2, len);
	} else {
		i0 = wrap(ind - 1, len);
		i1 = wrap(ind,     len);
		i2 = wrap(ind + 1, len);
		i3 = wrap(ind + 2, len);
	}
	i0 = i0 * nodeSize + pos;
	i1 = i1 * nodeSize + pos;
	i2 = i2 * nodeSize + pos;
	i3 = i3 * nodeSize + pos;

	for (unsigned int i = 0; i < count; i++){
		*dest++ = cerp(nodes[i0 + i], nodes[i1 + i], nodes[i2 + i], nodes[i3 + i], tFrac);
	}
}

bool DemoTrack::loadFromFile(char *fileName){
	unsigned int count;
	
	FILE *file = fopen(fileName, "rb");
	if (file == NULL) return false;

	fread(&count,    sizeof(int), 1, file);
	fread(&nodeSize, sizeof(int), 1, file);

	nodes.setCount(count);
	fread(nodes.getArray(), count * sizeof(float), 1, file);
	fclose(file);

	return true;
}

bool DemoTrack::writeToFile(char *fileName) const {
	unsigned int count = nodes.getCount();
	
	FILE *file = fopen(fileName, "wb");
	if (file == NULL) return false;

	fwrite(&count,    sizeof(int), 1, file);
	fwrite(&nodeSize, sizeof(int), 1, file);
	fwrite(nodes.getArray(), count * sizeof(float), 1, file);
	fclose(file);

	return true;
}
