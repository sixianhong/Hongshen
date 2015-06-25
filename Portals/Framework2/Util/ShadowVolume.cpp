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

#include "ShadowVolume.h"
#include "OrderedSet.h"
#include <stdio.h>

Edge::Edge(unsigned int iv0, unsigned int iv1){
//	if (iv0 > iv1){
		v0 = iv0;
		v1 = iv1;
/*	} else {
		v0 = iv1;
		v1 = iv0;
	}*/
}

bool operator != (const Edge &e0, const Edge &e1){
	unsigned int min0, max0, min1, max1;

	if (e0.v0 > e0.v1){
		min0 = e0.v0;
		max0 = e0.v1;
	} else {
		min0 = e0.v1;
		max0 = e0.v0;
	}
	if (e1.v0 > e1.v1){
		min1 = e1.v0;
		max1 = e1.v1;
	} else {
		min1 = e1.v1;
		max1 = e1.v0;
	}
	return (max0 != max1 || min0 != min1);
}

bool operator < (const Edge &e0, const Edge &e1){
	unsigned int min0, max0, min1, max1;

	if (e0.v0 > e0.v1){
		min0 = e0.v0;
		max0 = e0.v1;
	} else {
		min0 = e0.v1;
		max0 = e0.v0;
	}
	if (e1.v0 > e1.v1){
		min1 = e1.v0;
		max1 = e1.v1;
	} else {
		min1 = e1.v1;
		max1 = e1.v0;
	}

	if (max0 < max1) return true;
	if (max0 > max1) return false;
	return (min0 < min1);
}

ShadowVolume::ShadowVolume(){
	vertices = NULL;
	nVertices = 0;
}

ShadowVolume::~ShadowVolume(){
	delete vertices;
}

bool ShadowVolume::setEdges(vec3 *verts, unsigned int vertexCount, unsigned short *inds, unsigned int indexCount){
	OrderedSet <Edge> sEdges;
	unsigned int i, k;

	for (i = 0; i < indexCount; i += 3){
		vec3 v0 = verts[inds[i]];
		vec3 d0 = normalize(verts[inds[i + 1]] - v0);
		vec3 d1 = normalize(verts[inds[i + 2]] - v0);
		vec3 normal = normalize(cross(d0, d1));
		float d = -dot(v0, normal);

		for (k = 0; k < 3; k++){
			Edge edge(inds[i + k], inds[i + ((k + 1) % 3)]);
			if (sEdges.goToObject(edge)){
				sEdges.getCurrent().norm1 = normal;
				sEdges.getCurrent().d1 = d;
			} else {
				edge.norm0 = normal;
				edge.d0 = d;
				edge.norm1 = vec3(0,0,0);
				edge.d1 = 0;
				sEdges.insert(edge);
			}
		}
	}

	bool res = sEdges.goToFirst();
	while (res){
		if (dot(sEdges.getCurrent().norm0, sEdges.getCurrent().norm1) < 0.99f){
			edges.add(sEdges.getCurrent());
		}
		res = sEdges.next();
	}


	vertices = new vec4[nVertices = 2 * vertexCount];
	for (i = 0; i < vertexCount; i++){
		vertices[2 * i    ] = vec4(verts[i], 0);
		vertices[2 * i + 1] = vec4(verts[i], 1);
	}

	return true;
}


void ShadowVolume::findContours(const vec3 &position){
	contourEdges.clear();

	for (unsigned int i = 0; i < edges.getCount(); i++){
		float dist0 = dot(position, edges[i].norm0) + edges[i].d0;
		float dist1 = dot(position, edges[i].norm1) + edges[i].d1;

		if (dist0 * dist1 < 0){
			//contourEdges.add(2 * edges[i].v0);
			//contourEdges.add(2 * edges[i].v0 + 1);
			//contourEdges.add(2 * edges[i].v1);
			//contourEdges.add(2 * edges[i].v1 + 1);

			if (dist0 > 0){
				contourEdges.add(2 * edges[i].v0);
				contourEdges.add(2 * edges[i].v0 + 1);
				contourEdges.add(2 * edges[i].v1);

				contourEdges.add(2 * edges[i].v1);
				contourEdges.add(2 * edges[i].v0 + 1);
				contourEdges.add(2 * edges[i].v1 + 1);
			} else {
				contourEdges.add(2 * edges[i].v0);
				contourEdges.add(2 * edges[i].v1);
				contourEdges.add(2 * edges[i].v0 + 1);

				contourEdges.add(2 * edges[i].v1);
				contourEdges.add(2 * edges[i].v1 + 1);
				contourEdges.add(2 * edges[i].v0 + 1);
			}
		}
	}
}

bool ShadowVolume::loadFromFile(char *fileName){
	FILE *file = fopen(fileName, "rb");
	if (file == NULL) return false;

	fread(&nVertices, sizeof(nVertices), 1, file);
	vertices = new vec4[nVertices];
	fread(vertices, nVertices * sizeof(vec4), 1, file);

	unsigned int nEdges;
	fread(&nEdges, sizeof(nEdges), 1, file);
	edges.setCount(nEdges);
	fread(edges.getArray(), nEdges * sizeof(Edge), 1, file);

	fclose(file);

	return true;
}

bool ShadowVolume::saveToFile(char *fileName){
	FILE *file = fopen(fileName, "wb");
	if (file == NULL) return false;

	fwrite(&nVertices, sizeof(nVertices), 1, file);
	fwrite(vertices, nVertices * sizeof(vec4), 1, file);

	unsigned int nEdges = edges.getCount();
	fwrite(&nEdges, sizeof(nEdges), 1, file);
	fwrite(edges.getArray(), nEdges * sizeof(Edge), 1, file);

	fclose(file);

	return true;
}
