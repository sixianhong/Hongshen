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

#ifndef _DIRECT3DMODEL_H_
#define _DIRECT3DMODEL_H_

#include "Model.h"
#include <d3d9.h>

class Direct3DBatch : public Batch {
protected:
	LPDIRECT3DVERTEXBUFFER9 vBuf;
	LPDIRECT3DINDEXBUFFER9  iBuf;
	LPDIRECT3DVERTEXDECLARATION9 vDecl;
public:
	Direct3DBatch();

	bool uploadToVertexBuffer(LPDIRECT3DDEVICE9 dev);
	void freeVertexBuffer();
	void draw(LPDIRECT3DDEVICE9 dev);
};

class Direct3DModel : public Model {
protected:
	Batch *newBatch(){ return new Direct3DBatch(); };
public:
	bool uploadToVertexBuffer(LPDIRECT3DDEVICE9 dev);
	void freeVertexBuffer();
	void draw(LPDIRECT3DDEVICE9 dev);
};


#endif // _DIRECT3DMODEL_H_
