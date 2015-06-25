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

#ifndef _MODEL_H_
#define _MODEL_H_

#include "Set.h"
#include <stdio.h>
#include "../Math/Vector.h"

enum PrimitiveType {
	PRIM_TRIANGLES      = 0,
	PRIM_QUADS          = 1,
	PRIM_TRIANGLE_STRIP = 2,
	PRIM_LINES          = 3,
};

enum AttributeType {
	ATT_VERTEX   = 0,
	ATT_NORMAL   = 1,
	ATT_TEXCOORD = 2,
	ATT_COLOR    = 3,
};

enum AttributeFormat {
	ATT_FLOAT         = 0,
	ATT_UNSIGNED_BYTE = 1,
};

float getValue(const char *src, const unsigned int index, const AttributeFormat attFormat);
unsigned int getSize(const AttributeFormat attFormat);

struct Format {
	AttributeType attType;
	AttributeFormat attFormat;
	unsigned int size;
	unsigned int offset;
	unsigned int index;
};

class Batch {
public:
	Batch();
	virtual ~Batch();

	void clean();

	unsigned int getIndex(const unsigned int index) const {
		return (indexSize == 2)? ((unsigned short *) indices)[index] : ((unsigned int *) indices)[index];
	}

	void setIndex(const unsigned int index, const unsigned int value){
		if (indexSize == 2){
			((unsigned short *) indices)[index] = value;
		} else {
			((unsigned int *) indices)[index] = value;
		}
	}

	char *getVertices() const { return vertices; }
	char *getIndices()  const { return indices;  }
	const unsigned int getVertexCount() const { return nVertices;  }
	const unsigned int getIndexCount()  const { return nIndices;   }
	const unsigned int getVertexSize()  const { return vertexSize; }
	const unsigned int getIndexSize()   const { return indexSize;  }

	const unsigned int getFormatCount() const { return formats.getCount(); }
	const Format &getFormat(unsigned int index) const { return formats[index]; }

	const PrimitiveType getPrimitiveType() const { return primitiveType; }
	void setPrimitiveType(const PrimitiveType primType){ primitiveType = primType; }

	void setVertices(void *vertexArray, const unsigned int vertexCount, const unsigned int size){
		vertices = (char *) vertexArray;
		nVertices = vertexCount;
		vertexSize = size;
	}
	void setIndices(void *indexArray, const unsigned int indexCount, const unsigned int size){
		indices = (char *) indexArray;
		nIndices = indexCount;
		indexSize = size;
	}

	void addFormat(const AttributeType attType, const AttributeFormat attFormat, const unsigned int size, const unsigned int offset, const unsigned int index = 0){
		Format format;

		format.attType = attType;
		format.attFormat = attFormat;
		format.size = size;
		format.offset = offset;
		format.index = index;

		formats.add(format);
	}

	bool findAttribute(const AttributeType attType, const unsigned int index = 0, unsigned int *where = NULL) const;
	bool insertAttribute(const AttributeType attType, const AttributeFormat attFormat, const unsigned int size, const unsigned int index = 0);
	bool removeAttribute(const AttributeType attType, const unsigned int index = 0);

	bool reverseWinding();
	bool flipNormals();
	bool transform(const mat4 &mat, const AttributeType attType, const unsigned int index = 0);
	bool fixTJunctions();

	void optimize(unsigned int dupVertSearch = 16);
	bool addNormals();

	bool getBoundingBox(float *min, float *max, bool initMinMax = true) const;

	bool saveToObj(const char *fileName);
	void readFromFile(FILE *file);
	void writeToFile(FILE *file);

protected:
	char *vertices;
	char *indices;

	unsigned int nVertices;
	unsigned int nIndices;
	unsigned int vertexSize;
	unsigned int indexSize;

	Set <Format> formats;
	PrimitiveType primitiveType;

	bool areVerticesEqual(const char *const src0, const char *const src1) const;
	void lerpVertices(char *dest, const char *const src0, const char *const src1, const float t) const;
};



class Model {
public:
	Model();
	virtual ~Model();

	unsigned int getBatchCount() const { return batches.getCount(); }
	Batch *getBatch(unsigned int index) const { return batches[index]; }
	void addBatch(Batch *const batch){ batches.add(batch); }

	bool getBoundingBox(float *min, float *max) const;

	bool loadObj(const char *fileName);
	bool loadFromFile(const char *fileName);
	bool saveToFile(const char *fileName);

protected:
	Set <Batch *> batches;

	virtual Batch *newBatch(){ return new Batch(); };
};



#endif // _MODEL_H_
