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

#ifndef _OPENGLMODEL_H_
#define _OPENGLMODEL_H_

#include "Model.h"
#include "../OpenGL/glExtensions.h"

class OpenGLBatch : public Batch {
protected:
	GLuint vertexBuffer;
	GLuint indexBuffer;
public:
	OpenGLBatch();
	~OpenGLBatch();

	bool uploadToVertexBuffer();
	bool freeVertexBuffer();

	void draw();
};

class OpenGLModel : public Model {
protected:
	Batch *newBatch(){ return new OpenGLBatch(); };
public:
	OpenGLModel();
	~OpenGLModel();

	bool uploadToVertexBuffer();
	void freeVertexBuffer();
	void draw();
};


#endif
