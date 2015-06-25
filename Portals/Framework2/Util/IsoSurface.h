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

#ifndef _ISOSURFACE_H_
#define _ISOSURFACE_H_

#include "../Math/Vector.h"
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#define COMPUTE_FIELD        (WM_USER + 1)
#define COMPUTE_VERTEX_ARRAY (WM_USER + 2)
#endif

typedef float (*FIELD_FUNC)(const vec3 &pos);

struct Param {
	int start, end;
};

struct GridNode {
	vec3 vertex;
	float value;
	vec3 normal;
};

struct Edge {
	GridNode *node0;
	GridNode *node1;
	int index;
};

struct Cell {
	GridNode *nodes[8];
	Edge *edges[12];
};

struct ISVertex {
	vec3 vertex;
	vec3 normal;
};

class IsoSurface {
public:
	IsoSurface(const int threadCount);
	~IsoSurface();

	void setSize(const vec3 &minLim, const vec3 &maxLim, const int sX, const int sY, const int sZ);

	void computeField(FIELD_FUNC field, const int start, const int end);
	void computeVertexArray(FIELD_FUNC field, const int start, const int end);
	void computeIndexArray();
	int workerThread();

	void update(FIELD_FUNC field, const int maxThreads);

	unsigned int getVertexCount() const { return nVertices; }
	unsigned int getIndexCount() const { return nIndices; }
	ISVertex *getVertexArray() const { return vertexArray; }
	unsigned int *getIndexArray() const { return indexArray; }

private:
	int getEdgeCount() const;

	int sizeX, sizeY, sizeZ;
	vec3 dx, dy, dz;

	GridNode *nodes;
	Cell *cells;
	Edge *edges;	
	Edge **intersectedEdges;

	ISVertex *vertexArray;
	unsigned int vaSize;
	unsigned int nVertices;

	unsigned int *indexArray;
	unsigned int iaSize;
	unsigned int nIndices;

#ifdef _WIN32
	HANDLE *threadHandles;
	DWORD *threadIDs;
	HANDLE *events;
	Param *param;
	int nThreads;
#endif
};

#endif // _ISOSURFACE_H_
