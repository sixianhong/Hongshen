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

#ifndef _COLLISIONSET_H_
#define _COLLISIONSET_H_

#include "../Math/Vector.h"
#include "Set.h"

struct Triangle {
	Triangle(const vec3 &v0, const vec3 &v1, const vec3 &v2, void *aData){
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;
		auxData = aData;
		//next = NULL;
	}

	vec3 v[3];
	Triangle *next;
	void *auxData;
};


class CollNode {
public:
	CollNode();
	~CollNode();

	void build(Set <Triangle *> &triangles, const unsigned int cutWeight, const unsigned int diffWeight, const unsigned int coplanarWeight);
	bool intersect(const vec3 &v0, const vec3 &v1, vec3 *point = NULL, bool solid = true, void **auxData = NULL) const;

	bool pushSphere(vec3 &pos, const float radius) const;

	Triangle *aboveTriangle(const vec3 &pos) const;

protected:
	bool isect(const vec3 &v0, const vec3 &v1, vec3 *point, void **auxData) const;
	
	vec3 normal;
	float offset;

	CollNode *front;
	CollNode *back;

	Triangle *triangle;
};


class CollisionSet {
public:
	CollisionSet();
	~CollisionSet();

	void addTriangle(const vec3 &v0, const vec3 &v1, const vec3 &v2, void *auxData = NULL){
		triangles.add(new Triangle(v0, v1, v2, auxData));
	}
	void build(const unsigned int cutWeight = 3, const unsigned int diffWeight = 1, const unsigned int coplanarWeight = 2);
	bool intersect(const vec3 &v0, const vec3 &v1, vec3 *point = NULL, bool solid = true, void **auxData = NULL) const;

	bool pushSphere(vec3 &pos, const float radius) const;

protected:
	Set <Triangle *> triangles;
	CollNode *top;
};


#endif // _COLLISIONSET_H_
