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

#ifndef _SPRINGSYSTEM_H_
#define _SPRINGSYSTEM_H_

#include "Set.h"
#include "../Math/Vector.h"

struct SNode;

struct Spring {
	SNode *node;
	float naturalLength;
};

struct SNode {
	SNode(vec3 *position, vec3 *norm){
		pos = position;
		normal = norm;
		dir = vec3(0, 0, 0);
		locked = false;
		nNormal = 0;
	}
	void addSpring(SNode *node){
		Spring spring;

		spring.node = node;
		spring.naturalLength = distance(*pos, *node->pos);

		springs.add(spring);
	}

	vec3 *pos;
	vec3 *normal;
	vec3 dir;

	Set <Spring> springs;
	unsigned int nNormal;

	bool locked;
};

typedef void (*ProcessNodeFunc)(SNode *node, float *attribs);

class SpringSystem {
public:
	SpringSystem();

	void addRectField(unsigned int width, unsigned int height, void *pos, void *norm, unsigned int stride);

	void update(float dTime, ProcessNodeFunc process = NULL, float *attribs = NULL);
	void evaluateNormals();

	SNode *getNode(unsigned int index) const { return nodes[index]; }
	unsigned int getNodeCount() const { return nodes.getCount(); }

	void setGravity(const vec3 &grav){ gravity = grav; }

protected:
	vec3 gravity;
	Set <SNode *> nodes;
};


#endif // _SPRINGSYSTEM_H_
