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

#ifndef _DEMOTRACK_H_
#define _DEMOTRACK_H_

#include "../Math/Vector.h"
#include "Set.h"

class DemoTrack {
public:
	DemoTrack();

	void setNodeSize(const unsigned int size){ nodeSize = size; }
	void add(const float x){ nodes.add(x); }
	void add(const vec3 &v){
		nodes.add(v.x);
		nodes.add(v.y);
		nodes.add(v.z);
	}

	void getAt(const float t, float *dest, unsigned int pos = 0, unsigned int count = 1) const;
	unsigned int getNodeCount() const { return nodes.getCount(); }

	bool loadFromFile(char *fileName);
	bool writeToFile(char *fileName) const;
private:
	Set <float> nodes;
	unsigned int nodeSize;

	bool clampIndices;
};

#endif // _DEMOTRACK_H_
