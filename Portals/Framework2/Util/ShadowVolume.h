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

#ifndef _SHADOWVOLUME_H_
#define _SHADOWVOLUME_H_

#include "../Math/Vector.h"
#include "Set.h"

struct Edge {
	Edge(){};
	Edge(unsigned int iv0, unsigned int iv1);

	unsigned int v0;
	unsigned int v1;

	vec3 norm0;
	float d0;
	vec3 norm1;
	float d1;
};

bool operator != (const Edge &e0, const Edge &e1);
bool operator <  (const Edge &e0, const Edge &e1);


class ShadowVolume {
public:
	ShadowVolume();
	~ShadowVolume();

	bool setEdges(vec3 *verts, unsigned int vertexCount, unsigned short *inds, unsigned int indexCount);

	void findContours(const vec3 &position);
	unsigned int *getIndices() const { return contourEdges.getArray(); }
	vec4 *getVertices() const { return vertices; }
	unsigned int getIndexCount() const { return contourEdges.getCount(); }
	unsigned int getVertexCount() const { return nVertices; }

	bool loadFromFile(char *fileName);
	bool saveToFile(char *fileName);

protected:
	Set <Edge> edges;
	Set <unsigned int> contourEdges;

	vec4 *vertices;
	unsigned int nVertices;
};

#endif // _SHADOWVOLUME_H_
