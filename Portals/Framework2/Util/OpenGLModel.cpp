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

#include "OpenGLModel.h"

OpenGLBatch::OpenGLBatch(){
	vertexBuffer = 0;
	indexBuffer  = 0;
}

OpenGLBatch::~OpenGLBatch(){
}

OpenGLModel::OpenGLModel(){

}

OpenGLModel::~OpenGLModel(){

}

bool OpenGLBatch::uploadToVertexBuffer(){
	if (GL_ARB_vertex_buffer_object_supported){
		glGenBuffersARB(1, &vertexBuffer);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexBuffer);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, nVertices * vertexSize, vertices, GL_STATIC_DRAW_ARB);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
		if (nIndices > 0){
			glGenBuffersARB(1, &indexBuffer);
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indexBuffer);
			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, nIndices * indexSize, indices, GL_STATIC_DRAW_ARB);
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
		}
	}

	return true;
}

bool OpenGLBatch::freeVertexBuffer(){
	if (GL_ARB_vertex_buffer_object_supported){
		if (vertexBuffer != 0) glDeleteBuffersARB(1, &vertexBuffer);
		if (indexBuffer  != 0) glDeleteBuffersARB(1, &indexBuffer);
	}
	return true;
}

GLenum getGLType(const AttributeFormat attFormat){
	switch (attFormat){
	case ATT_FLOAT:         return GL_FLOAT;
	case ATT_UNSIGNED_BYTE: return GL_UNSIGNED_BYTE;
	default:
		return 0;
	}
}

GLenum getGLPrimitive(const PrimitiveType primType){
	switch (primType){
	case PRIM_TRIANGLES:      return GL_TRIANGLES;
	case PRIM_QUADS:          return GL_QUADS;
	case PRIM_TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
	case PRIM_LINES:          return GL_LINES;
	default:
		return 0;
	}
}

GLenum arrayType[] = { GL_VERTEX_ARRAY, GL_NORMAL_ARRAY, GL_TEXTURE_COORD_ARRAY, GL_COLOR_ARRAY };

void OpenGLBatch::draw(){
	unsigned int i, nFormats = formats.getCount();

	if (GL_ARB_vertex_buffer_object_supported && vertexBuffer != 0){
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexBuffer);
	}

	for (i = 0; i < nFormats; i++){
		Format format = formats[i];
		GLenum type = getGLType(format.attFormat);
		GLenum array = arrayType[format.attType];

		char *basePointer = (vertexBuffer != 0)? NULL : vertices;

		switch (format.attType){
		case ATT_VERTEX:
			glVertexPointer(format.size, type, vertexSize, basePointer + format.offset);
			break;
		case ATT_NORMAL:
			glNormalPointer(type, vertexSize, basePointer + format.offset);
			break;
		case ATT_TEXCOORD:
			glClientActiveTextureARB(GL_TEXTURE0_ARB + format.index);
			glTexCoordPointer(format.size, type, vertexSize, basePointer + format.offset);
			break;
		case ATT_COLOR:
			glColorPointer(format.size, type, vertexSize, basePointer + format.offset);
			break;
		}
		glEnableClientState(array);
	}

	if (nIndices == 0){
		glDrawArrays(getGLPrimitive(primitiveType), 0, nVertices);
	} else {
		if (indexBuffer != 0) glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indexBuffer);

		glDrawElements(getGLPrimitive(primitiveType), nIndices, indexSize == 2? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (indexBuffer != 0)? NULL : indices);

		if (indexBuffer != 0) glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	}

	for (i = 0; i < nFormats; i++){
		if (formats[i].attType == ATT_TEXCOORD){
			glClientActiveTextureARB(GL_TEXTURE0_ARB + formats[i].index);
		}
		glDisableClientState(arrayType[formats[i].attType]);
	}

	if (GL_ARB_vertex_buffer_object_supported && vertexBuffer != 0){
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}
}

bool OpenGLModel::uploadToVertexBuffer(){
	bool res = true;
	for (unsigned int i = 0; i < batches.getCount(); i++){
		res &= ((OpenGLBatch *) batches[i])->uploadToVertexBuffer();
	}
	return res;
}

void OpenGLModel::freeVertexBuffer(){
	for (unsigned int i = 0; i < batches.getCount(); i++){
		((OpenGLBatch *) batches[i])->freeVertexBuffer();
	}
}

void OpenGLModel::draw(){
	for (unsigned int i = 0; i < batches.getCount(); i++){
		((OpenGLBatch *) batches[i])->draw();
	}
}

