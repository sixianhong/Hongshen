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

#include "Model.h"
#include "Tokenizer.h"
#include "OrderedSet.h"

float getValue(const char *src, const unsigned int index, const AttributeFormat attFormat){
	switch (attFormat){
	case ATT_FLOAT:         return *(((float *) src) + index);
	case ATT_UNSIGNED_BYTE: return *(((unsigned char *) src) + index) * (1.0f / 255.0f);
	default:
		return 0;
	}
}

void setValue(const char *dest, const unsigned int index, const AttributeFormat attFormat, float value){
	switch (attFormat){
	case ATT_FLOAT:
		*(((float *) dest) + index) = value;
		break;
	case ATT_UNSIGNED_BYTE:
		*(((unsigned char *) dest) + index) = (unsigned char) (value * 255.0f);
		break;
	}
}

unsigned int getSize(const AttributeFormat attFormat){
	switch (attFormat){
	case ATT_FLOAT:         return 4;
	case ATT_UNSIGNED_BYTE: return 1;
	default:
		return 0;
	}
}

Batch::Batch(){
	vertices = NULL;
	indices  = NULL;

	nVertices = 0;
	nIndices  = 0;
	vertexSize = 0;
	indexSize  = 0;
}

Batch::~Batch(){
	clean();
}

void Batch::clean(){
	if (vertices != NULL) delete vertices;
	if (indices  != NULL) delete indices;
	vertices = NULL;
	indices = NULL;
}

bool Batch::areVerticesEqual(const char *const src0, const char *const src1) const {
	unsigned int i, j, nFormats = formats.getCount();
	for (i = 0; i < nFormats; i++){
		for (j = 0; j < formats[i].size; j++){
			float diff = getValue(src0 + formats[i].offset, j, formats[i].attFormat) - getValue(src1 + formats[i].offset, j, formats[i].attFormat);

			if (fabs(diff) > 0.01f) return false;
		}
	}

	return true;
}

void Batch::lerpVertices(char *dest, const char *const src0, const char *const src1, const float t) const {
	for (unsigned int i = 0; i < formats.getCount(); i++){
		for (unsigned int j = 0; j < formats[i].size; j++){
			float value = lerp(getValue(src0 + formats[i].offset, j, formats[i].attFormat), getValue(src1 + formats[i].offset, j, formats[i].attFormat), t);
			setValue(dest + formats[i].offset, j, formats[i].attFormat, value);
		}
	}
}


bool Batch::findAttribute(const AttributeType attType, const unsigned int index, unsigned int *where) const {
	for (unsigned int i = 0; i < formats.getCount(); i++){
		if (formats[i].attType == attType && formats[i].index == index){
			if (where != NULL) *where = i;
			return true;
		}
	}
	return false;
}

bool Batch::insertAttribute(const AttributeType attType, const AttributeFormat attFormat, const unsigned int size, const unsigned int index){
	unsigned int i;

	for (i = 0; i < formats.getCount(); i++){
		if (formats[i].attType == attType && formats[i].index == index) return false;
	}

	addFormat(attType, attFormat, size, vertexSize, index);

	unsigned int fSize = size * getSize(attFormat);
	unsigned int newVertexSize = vertexSize + fSize;
	char *newVertices = new char[nVertices * newVertexSize];

	char *dest = newVertices;
	char *src = vertices;
	for (i = 0; i < nVertices; i++){
		memcpy(dest, src, vertexSize);
		memset(dest + vertexSize, 0, fSize);
		dest += newVertexSize;
		src += vertexSize;
	}

	delete vertices;
	vertices = newVertices;
	vertexSize = newVertexSize;

	return true;
}

bool Batch::removeAttribute(const AttributeType attType, const unsigned int index){
	unsigned int i, j;

	for (i = 0; i < formats.getCount(); i++){
		if (formats[i].attType == attType && formats[i].index == index){
			Format format = formats[i];
			formats.remove(i);

			unsigned int fSize = format.size * getSize(format.attFormat);
			for (j = i; j < formats.getCount(); j++) formats[i].offset -= fSize;

			unsigned int newVertexSize = vertexSize - fSize;
			char *newVertices = new char[nVertices * newVertexSize];

			char *dest = newVertices;
			char *src = vertices;
			memcpy(dest, src, format.offset);
			dest += format.offset;
			src += format.offset + fSize;
			for (j = 0; j < nVertices - 1; j++){
				memcpy(dest, src, newVertexSize);
				dest += newVertexSize;
				src += vertexSize;
			}
			memcpy(dest, src, vertexSize - (format.offset + fSize));

			delete vertices;
			vertices = newVertices;
			vertexSize = newVertexSize;

			return true;
		}
	}
	return false;
}

bool Batch::reverseWinding(){
	if (primitiveType != PRIM_TRIANGLES && primitiveType != PRIM_QUADS) return false;

	unsigned int primSize = (primitiveType == PRIM_TRIANGLES)? 3 : 4;
	unsigned int n = primSize / 2;

	if (indices != NULL){
		unsigned int nPrims = nIndices / primSize;
		for (unsigned int i = 0; i < nPrims; i++){
			for (unsigned int j = 0; j < n; j++){
				if (indexSize == 2){
					unsigned short temp = ((unsigned short *) indices)[i * primSize + j];
					((unsigned short *) indices)[i * primSize + j] = ((unsigned short *) indices)[i * primSize + primSize - j - 1];
					((unsigned short *) indices)[i * primSize + primSize - j - 1] = temp;
				} else {
					unsigned int temp = ((unsigned int *) indices)[i * primSize + j];
					((unsigned int *) indices)[i * primSize + j] = ((unsigned int *) indices)[i * primSize + primSize - j - 1];
					((unsigned int *) indices)[i * primSize + primSize - j - 1] = temp;
				}
			}
		}
	} else {
		unsigned int nPrims = nVertices / primSize;
		char *temp = new char[vertexSize];
		for (unsigned int i = 0; i < nPrims; i++){
			for (unsigned int j = 0; j < n; j++){
				char *src0 = vertices + (i * primSize + j) * vertexSize;
				char *src1 = vertices + (i * primSize + primSize - j - 1) * vertexSize;
				memcpy(temp, src0, vertexSize);
				memcpy(src0, src1, vertexSize);
				memcpy(src1, temp, vertexSize);
			}
		}
		delete temp;
	}

	return true;
}

bool Batch::flipNormals(){
	unsigned int normalOffset;
	if (!findAttribute(ATT_NORMAL, 0, &normalOffset)) return false;

	AttributeFormat normalFormat = formats[normalOffset].attFormat;
	normalOffset = formats[normalOffset].offset;

	for (unsigned int i = 0; i < nVertices; i++){
		char *src = vertices + i * vertexSize + normalOffset;
		setValue(src, 0, normalFormat, -getValue(src, 0, normalFormat));		
		setValue(src, 1, normalFormat, -getValue(src, 1, normalFormat));		
		setValue(src, 2, normalFormat, -getValue(src, 2, normalFormat));		
	}

	return true;
}

bool Batch::transform(const mat4 &mat, const AttributeType attType, const unsigned int index){
	AttributeFormat format;
	unsigned int i,j,offset, size;
	if (!findAttribute(attType, index, &offset)) return false;
	size   = formats[offset].size;
	format = formats[offset].attFormat;
	offset = formats[offset].offset;

	for (i = 0; i < nVertices; i++){
		char *src = vertices + i * vertexSize + offset;

		vec4 vec(0, 0, 0, 1);
		for (j = 0; j < size; j++){
			vec.operator [](j) = getValue(src, j, format);
		}
		vec = mat * vec;
		for (j = 0; j < size; j++){
			setValue(src, j, format, vec.operator [](j));
		}
	}

	return true;
}

bool Batch::fixTJunctions(){
	if (primitiveType != PRIM_TRIANGLES || nIndices == 0) return false;

	unsigned int offset;
	if (!findAttribute(ATT_VERTEX, 0, &offset) || formats[offset].attFormat != ATT_FLOAT || formats[offset].size != 3) return false;
	offset = formats[offset].offset;

	Set <unsigned int> inds;
	unsigned int i, j, k;

	for (i = 0; i < nIndices; i++){
		if (indexSize == 2){
			inds.add(((unsigned short *) indices)[i]);
		} else {
			inds.add(((unsigned int *) indices)[i]);
		}
	}

	char *tempVertex = new char[vertexSize];

	for (j = 0; j < inds.getCount(); j++){
		char *vp = vertices + inds[j] * vertexSize;
		vec3 point = *(vec3 *) (vp + offset);

		for (i = 0; i < inds.getCount(); i += 3){
			unsigned int indexes[3];
			char *v[3];
			vec3 pos[3];
			
			for (k = 0; k < 3; k++){
				indexes[k] = inds[i + k];
				v[k] = vertices + indexes[k] * vertexSize;
				pos[k] = *(vec3 *) (v[k] + offset);
			}

			for (k = 0; k < 3; k++){
				unsigned int k1 = (k  == 2)? 0 : k  + 1;
				unsigned int k2 = (k1 == 2)? 0 : k1 + 1;

				float t = lineProjection(pos[k], pos[k1], point);

				if (t > 0.001f && t < 0.999f){
					vec3 len = lerp(pos[k], pos[k1], t) - point;
					float distSqr = dot(len, len);
					if (distSqr < 0.01f){
						lerpVertices(tempVertex, v[k], v[k1], t);
						if (areVerticesEqual(tempVertex, vp)){
							inds[i + k1] = inds[j];
							inds.add(inds[j]);
						} else {
							// TODO: optimize this part
							vertices = (char *) realloc(vertices, (nVertices + 1) * vertexSize);
							memcpy(vertices + nVertices * vertexSize, tempVertex, vertexSize);
							inds[i + k1] = nVertices;
							inds.add(nVertices);
							nVertices++;
						}
						inds.add(indexes[k1]);
						inds.add(indexes[k2]);
					}
				}
			}

		}
	}

	delete indices;
	nIndices = inds.getCount();
	if (nIndices > 65535){
		indexSize = 4;
		indices = (char *) new unsigned int[nIndices];
		memcpy(indices, inds.getArray(), 4 * nIndices);
	} else {
		indexSize = 2;
		indices = (char *) new unsigned short[nIndices];
		for (i = 0; i < nIndices; i++){
			((unsigned short *) indices)[i] = inds[i];
		}
	}

	delete tempVertex;
	return true;
}

void Batch::optimize(unsigned int dupVertSearch){
	unsigned int nVert = nVertices;
	unsigned int nInds;
	char *verts;
	unsigned int *inds;
	unsigned int i;

	if (indices == NULL){
		nInds = nVertices;
		inds = new unsigned int[nInds];
		for (i = 0; i < nInds; i++){
			inds[i] = i;
		}
	} else {
		nInds = nIndices;
		inds = new unsigned int[nInds];
		if (indexSize == 2){
			for (i = 0; i < nInds; i++){
				inds[i] = ((unsigned short *) indices)[i];
			}
		} else memcpy(inds, indices, nInds * indexSize);
	}

	verts = new char[nVert * vertexSize];
	memcpy(verts, vertices, nVert * vertexSize);

	bool *removed = new bool[nVert];
	memset(removed, 0, nVert * sizeof(bool));

	Set <unsigned int> refersTo;

	for (i = 1; i < nVert; i++){
		unsigned int m = (i < dupVertSearch)? i : dupVertSearch;

		unsigned int c = 0;
		for (unsigned int j = 1; j <= m; j++){
			if (removed[i - j]){
				c++;
				continue;
			}

			char *src0 = verts +  i      * vertexSize;
			char *src1 = verts + (i - j) * vertexSize;

			if (areVerticesEqual(src0, src1)){
				removed[i] = true;
				refersTo.add(i - j + c);

				break;
			}
		}
	}

	unsigned int *indRef = new unsigned int[nVert];
	unsigned int n = 0;
	for (i = 0; i < nVert; i++){
		if (removed[i]){
			indRef[i] = refersTo[n] - n;
			n++;
		} else {
			indRef[i] = i - n;
		}
	}



	nVert -= refersTo.getCount();

	unsigned int indSize = (nVert < 65536)? 2 : 4;
	if (nVert * vertexSize + nInds * indSize < nVertices * vertexSize + nIndices * indexSize){
		clean();

		nVertices = nVert;
		nIndices = nInds;

		char *newVertices = new char[nVert * vertexSize];

		unsigned int y = (unsigned int) (-1);
		for (unsigned int x = 0; x < nVert; x++){
			do y++; while (removed[y]);
			
			char *dest = newVertices + x * vertexSize;
			char *src = verts + y * vertexSize;

			memcpy(dest, src, vertexSize);
		}
		delete verts;
		vertices = newVertices;

		if ((indexSize = indSize) == 2){
			unsigned short *newInds = new unsigned short[nInds];
			for (i = 0; i < nInds; i++){
				newInds[i] = indRef[inds[i]];
			}
			
			indices = (char *) newInds;
		} else {
			unsigned int *newInds = new unsigned int[nInds];
			for (i = 0; i < nInds; i++){
				newInds[i] = indRef[inds[i]];
			}

			indices = (char *) newInds;
		}
		delete inds;
	}

	delete indRef;
	delete removed;
}

bool Batch::addNormals(){
	if (indices == NULL) return false;
	if (!insertAttribute(ATT_NORMAL, ATT_FLOAT, 3)) return false;
	if (primitiveType != PRIM_TRIANGLES) return false;

	unsigned int vertexOffset, normalOffset;
	if (!findAttribute(ATT_VERTEX, 0, &vertexOffset) || !findAttribute(ATT_NORMAL, 0, &normalOffset)) return false;

	AttributeFormat vertFormat = formats[vertexOffset].attFormat;
	vertexOffset = formats[vertexOffset].offset;
	normalOffset = formats[normalOffset].offset;

	unsigned int i, nTris = nIndices / 3;
	for (i = 0; i < nTris; i++){
		unsigned int i0, i1, i2;
		if (indexSize == 2){
			i0 = ((unsigned short *) indices)[3 * i];
			i1 = ((unsigned short *) indices)[3 * i + 1];
			i2 = ((unsigned short *) indices)[3 * i + 2];
		} else {
			i0 = ((unsigned int *) indices)[3 * i];
			i1 = ((unsigned int *) indices)[3 * i + 1];
			i2 = ((unsigned int *) indices)[3 * i + 2];
		}

		char *vert, *norm;
		vert = vertices + i0 * vertexSize + vertexOffset;
		vec3 v0(getValue(vert, 0, vertFormat), getValue(vert, 1, vertFormat), getValue(vert, 2, vertFormat));
		vert = vertices + i1 * vertexSize + vertexOffset;
		vec3 v1(getValue(vert, 0, vertFormat), getValue(vert, 1, vertFormat), getValue(vert, 2, vertFormat));
		vert = vertices + i2 * vertexSize + vertexOffset;
		vec3 v2(getValue(vert, 0, vertFormat), getValue(vert, 1, vertFormat), getValue(vert, 2, vertFormat));

		vec3 normal = (cross(v1 - v0, v2 - v0));

		norm = vertices + i0 * vertexSize + normalOffset;
		*(vec3 *) norm += normal;
		norm = vertices + i1 * vertexSize + normalOffset;
		*(vec3 *) norm += normal;
		norm = vertices + i2 * vertexSize + normalOffset;
		*(vec3 *) norm += normal;
	}

	for (i = 0; i < nVertices; i++){
		char *norm = vertices + i * vertexSize + normalOffset;
		*((vec3 *) norm) = normalize(*(vec3 *) norm);
	}

	return true;
}

bool Batch::getBoundingBox(float *min, float *max, bool initMinMax) const {
	AttributeFormat format;
	unsigned int i, j, offset, size;
	if (!findAttribute(ATT_VERTEX, 0, &offset)) return false;
	size   = formats[offset].size;
	format = formats[offset].attFormat;
	offset = formats[offset].offset;

	if (initMinMax){
		for (i = 0; i < size; i++){
			min[i] =  1e10f;
			max[i] = -1e10f;
		}
	}

	for (i = 0; i < nVertices; i++){
		char *src = vertices + i * vertexSize + offset;

		for (j = 0; j < size; j++){
			float val = getValue(src, j, format);
			if (val > max[j]) max[j] = val;
			if (val < min[j]) min[j] = val;
		}
	}

	return true;
}

bool Batch::saveToObj(const char *fileName){
	bool normals, texCoords;
	unsigned int vertexOffset, normalOffset, texCoordOffset, texCoordSize = 0;
	AttributeFormat vertexFormat, normalFormat = ATT_FLOAT, texCoordFormat = ATT_FLOAT;
	unsigned int primSize, nPrim;

	if (primitiveType == PRIM_TRIANGLES) primSize = 3;
	else if (primitiveType == PRIM_QUADS) primSize = 4;
	else return false;
	nPrim = nIndices / primSize;

	if (!findAttribute(ATT_VERTEX, 0, &vertexOffset)) return false;
	vertexFormat = formats[vertexOffset].attFormat;
	vertexOffset = formats[vertexOffset].offset;

	if (normals = findAttribute(ATT_NORMAL, 0, &normalOffset)){
		normalFormat = formats[normalOffset].attFormat;
		normalOffset = formats[normalOffset].offset;
	}
	if (texCoords = findAttribute(ATT_TEXCOORD, 0, &texCoordOffset)){
		texCoordFormat = formats[texCoordOffset].attFormat;
		texCoordSize   = formats[texCoordOffset].size;
		texCoordOffset = formats[texCoordOffset].offset;
	}

	FILE *file = fopen(fileName, "wb");
	if (file == NULL) return false;

	fwrite("g default\n", 10, 1, file);

	char str[256];
	unsigned int len;

	for (unsigned int i = 0; i < nVertices; i++){
		char *base = vertices + i * vertexSize;
		char *src = base + vertexOffset;
		len = sprintf(str, "v %.4f %.4f %.4f\n", getValue(src, 0, vertexFormat), getValue(src, 1, vertexFormat), getValue(src, 2, vertexFormat));

		fwrite(str, len, 1, file);
	}
	if (texCoords){
		for (unsigned int i = 0; i < nVertices; i++){
			char *base = vertices + i * vertexSize;
			char *src = base + texCoordOffset;

			len = sprintf(str, "vt ");
			for (unsigned int j = 0; j < texCoordSize; j++){
				len += sprintf(str + len, "%.4f", getValue(src, j, texCoordFormat));
			}
			len += sprintf(str + len, "\n");

			fwrite(str, len, 1, file);
		}
	}
	if (normals){
		for (unsigned int i = 0; i < nVertices; i++){
			char *base = vertices + i * vertexSize;
			char *src = base + normalOffset;
			len = sprintf(str, "vn %.4f %.4f %.4f\n", getValue(src, 0, normalFormat), getValue(src, 1, normalFormat), getValue(src, 2, normalFormat));

			fwrite(str, len, 1, file);
		}
	}

	fwrite("s 1\ng object\nusemtl initialShadingGroup\n", 40, 1, file);

	if (indices == NULL){

	} else {
		unsigned int n = 0;
		for (unsigned int i = 0; i < nPrim; i++){
			len = sprintf(str, "f");
			for (unsigned int j = 0; j < primSize; j++){
				unsigned int index = 1 + ((indexSize == 2)? ((unsigned short *) indices)[n] : ((unsigned int *) indices)[n]);

				len += sprintf(str + len, " %u", index);
				if (normals || texCoords) len += sprintf(str + len, "/");
				if (texCoords) len += sprintf(str + len, "%u", index);
				if (normals) len += sprintf(str + len, "/%u", index);

				n++;
			}
			len += sprintf(str + len, "\n");
			fwrite(str, len, 1, file);
		}
	}

	fclose(file);

	return true;
}

void Batch::readFromFile(FILE *file){
	fread(&nVertices,  sizeof(nVertices),  1, file);
	fread(&nIndices,   sizeof(nIndices),   1, file);
	fread(&vertexSize, sizeof(vertexSize), 1, file);
	fread(&indexSize,  sizeof(indexSize),  1, file);

	fread(&primitiveType, sizeof(primitiveType), 1, file);

	unsigned int nFormats;
	fread(&nFormats, sizeof(nFormats), 1, file);
	formats.setCount(nFormats);
	fread(formats.getArray(), nFormats * sizeof(Format), 1, file);

	vertices = new char[nVertices * vertexSize];
	fread(vertices, nVertices * vertexSize, 1, file);

	if (nIndices > 0){
		indices = new char[nIndices  * indexSize];
		fread(indices,  nIndices  * indexSize,  1, file);
	} else indices = NULL;
}

void Batch::writeToFile(FILE *file){
	fwrite(&nVertices,  sizeof(nVertices),  1, file);
	fwrite(&nIndices,   sizeof(nIndices),   1, file);
	fwrite(&vertexSize, sizeof(vertexSize), 1, file);
	fwrite(&indexSize,  sizeof(indexSize),  1, file);

	fwrite(&primitiveType, sizeof(primitiveType), 1, file);

	unsigned int nFormats = formats.getCount();
	fwrite(&nFormats, sizeof(nFormats), 1, file);
	fwrite(formats.getArray(), nFormats * sizeof(Format), 1, file);

	fwrite(vertices, nVertices * vertexSize, 1, file);
	fwrite(indices,  nIndices  * indexSize,  1, file);
}


Model::Model(){

}

Model::~Model(){
	for (unsigned int i = 0; i < batches.getCount(); i++){
		delete batches[i];
	}
}

bool Model::getBoundingBox(float *min, float *max) const {
	bool result = false;

	for (unsigned int i = 0; i < batches.getCount(); i++){
		result |= batches[i]->getBoundingBox(min, max, !result);
	}

	return result;
}


float readFloat(Tokenizer &tok, char *str){
	str = tok.next();
	if (str[0] == '-'){
		str = tok.next();
		return -(float) atof(str);
	} else {
		return (float) atof(str);
	}
}

#define nextFloat(tok) readFloat(tok, str)

struct ObjIndex {
	unsigned int vert, tex, norm;
	unsigned int index;
};

bool operator != (const ObjIndex &o0, const ObjIndex &o1){
	return (o0.vert != o1.vert || o0.tex != o1.tex || o0.norm != o1.norm);
}

bool operator < (const ObjIndex &o0, const ObjIndex &o1){
	return (o0.vert < o1.vert) || (o0.vert == o1.vert && o0.tex < o1.tex) || (o0.vert == o1.vert && o0.tex == o1.tex && o0.norm < o1.norm);
}

bool Model::loadObj(const char *fileName){
	Tokenizer tok, ltok;
	unsigned int n, start;

	Set <vec3> vertices;
	Set <vec3> normals;
	Set <float> texCoords;
	Set <ObjIndex> objIndices;
	ObjIndex index;
	memset(&index, 0, sizeof(index));

	if (!tok.setFile(fileName)) return false;

	char *str;
	while ((str = tok.next()) != NULL){
		switch (str[0]){
		case 'f':
			str = tok.nextLine();
			ltok.setString(str);

			start = objIndices.getCount();
			n = 0;
			while ((str = ltok.next()) != NULL){
				index.vert = atoi(str) - 1;
				if (texCoords.getCount() || normals.getCount()) ltok.next();
				if (texCoords.getCount()){
					str = ltok.next();
					index.tex = atoi(str) - 1;
				}
				if (normals.getCount()){
					ltok.next();
					str = ltok.next();
					index.norm = atoi(str) - 1;
				}

				if (n > 2){
					objIndices.add(objIndices[start]);
					objIndices.add(objIndices[objIndices.getCount() - 2]);
				}
				objIndices.add(index);
				n++;
			}

			break;
		case 'v':
			float x, y, z;
			switch(str[1]){
			case '\0':
				x = nextFloat(tok);
				y = nextFloat(tok);
				z = nextFloat(tok);
				vertices.add(vec3(x, y, z));
				break;
			case 'n':
				x = nextFloat(tok);
				y = nextFloat(tok);
				z = nextFloat(tok);
				normals.add(vec3(x, y, z));
				break;
			case 't':
				texCoords.add(nextFloat(tok));
				texCoords.add(nextFloat(tok));
				break;
			}
			break;
		default:
			tok.nextLine();
			break;
		}
	}

	OrderedSet <ObjIndex> indexSet;
	unsigned int nIndices = objIndices.getCount();
	unsigned int *indexArray = new unsigned int[nIndices];

	for (unsigned int i = 0; i < nIndices; i++){
		index = objIndices[i];
		if (indexSet.goToObject(index)){
			indexArray[i] = indexSet.getCurrent().index;
		} else {
			index.index = indexSet.getSize();
			indexSet.insert(index);
			indexArray[i] = index.index;
		}
	}



	unsigned int nVertices = indexSet.getSize();
	unsigned int vertexSize = 3 * sizeof(float);
	
	Batch *batch = newBatch();
	batch->addFormat(ATT_VERTEX, ATT_FLOAT, 3, 0);
	if (normals.getCount()){
		batch->addFormat(ATT_NORMAL, ATT_FLOAT, 3, vertexSize);
		vertexSize += 3 * sizeof(float);
	}
	if (texCoords.getCount()){
		batch->addFormat(ATT_TEXCOORD, ATT_FLOAT, 2, vertexSize);
		vertexSize += 2 * sizeof(float);
	}

	char *vertexArray = new char[nVertices * vertexSize];

	bool result = indexSet.goToFirst();
	while (result){
		index = indexSet.getCurrent();

		char *dest = vertexArray + index.index * vertexSize;

		((vec3 *) dest)[0] = vertices[index.vert];
		if (normals.getCount()){
			((vec3 *) dest)[1] = -normals[index.norm];
		}
		if (texCoords.getCount()){
			if (normals.getCount()){
				((float *) dest)[6] = texCoords[2 * index.tex];
				((float *) dest)[7] = texCoords[2 * index.tex + 1];
			} else {
				((float *) dest)[3] = texCoords[2 * index.tex];
				((float *) dest)[4] = texCoords[2 * index.tex + 1];
			}
		}

		result = indexSet.next();
	}

	batch->setVertices(vertexArray, nVertices, vertexSize);
	batch->setIndices(indexArray, nIndices, sizeof(unsigned int));
	batch->setPrimitiveType(PRIM_TRIANGLES);

	batches.add(batch);

	return true;
}

bool Model::loadFromFile(const char *fileName){
	FILE *file = fopen(fileName, "rb");
	if (file == NULL) return false;

	unsigned int version;
	fread(&version, sizeof(version), 1, file);
	unsigned int nBatches;
	fread(&nBatches, sizeof(nBatches), 1, file);

	for (unsigned int i = 0; i < nBatches; i++){
		Batch *batch = newBatch();
		batch->readFromFile(file);
		batches.add(batch);
	}

	fclose(file);
	
	return true;
}

bool Model::saveToFile(const char *fileName){
	FILE *file = fopen(fileName, "wb");
	if (file == NULL) return false;

	unsigned int version = 1;
	fwrite(&version, sizeof(version), 1, file);
	unsigned int nBatches = batches.getCount();
	fwrite(&nBatches, sizeof(nBatches), 1, file);

	for (unsigned int i = 0; i < nBatches; i++){
		batches[i]->writeToFile(file);
	}

	fclose(file);

	return true;
}
