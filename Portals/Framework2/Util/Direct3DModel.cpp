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

#include "Direct3DModel.h"

BYTE getD3DType(const AttributeFormat attFormat, const int size){
	switch (attFormat){
	case ATT_FLOAT:         return D3DDECLTYPE_FLOAT1 + (size - 1);
	case ATT_UNSIGNED_BYTE: return D3DDECLTYPE_UBYTE4;
	default:
		return 0;
	}
}

D3DPRIMITIVETYPE getD3DPrimitive(const PrimitiveType primType){
	switch (primType){
	case PRIM_TRIANGLES: return D3DPT_TRIANGLELIST;
	// case PRIM_QUADS: // unsupported
	case PRIM_TRIANGLE_STRIP: return D3DPT_TRIANGLESTRIP;
	default:
		return (D3DPRIMITIVETYPE) 0;
	}
}

UINT getPrimitives(const PrimitiveType primType, unsigned int nIndices){
	switch (primType){
	case PRIM_TRIANGLES:
		return nIndices / 3;
	// Quads unsupported
	// case PRIM_QUADS:
	//	return nIndices / 4;
	case PRIM_TRIANGLE_STRIP:
		return nIndices - 2;
	default:
		return 0;
	}	
}

static BYTE d3dSemantics[] = { D3DDECLUSAGE_POSITION, D3DDECLUSAGE_NORMAL, D3DDECLUSAGE_TEXCOORD, D3DDECLUSAGE_COLOR };

Direct3DBatch::Direct3DBatch(){
	vBuf = NULL;
	iBuf = NULL;
	vDecl = NULL;
}

bool Direct3DBatch::uploadToVertexBuffer(LPDIRECT3DDEVICE9 dev){
	void *dest;

	dev->CreateVertexBuffer(nVertices * vertexSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &vBuf, NULL);
	vBuf->Lock(0, 0, &dest, 0);
	memcpy(dest, vertices, nVertices * vertexSize);
	vBuf->Unlock();

	if (indices > 0){
		dev->CreateIndexBuffer(nIndices * indexSize, D3DUSAGE_WRITEONLY, (indexSize == 2)? D3DFMT_INDEX16 : D3DFMT_INDEX32, D3DPOOL_MANAGED, &iBuf, NULL);
		iBuf->Lock(0, 0, &dest, 0);
		memcpy(dest, indices, nIndices * indexSize);
		iBuf->Unlock();
	}

	unsigned int nFormat = formats.getCount();
	D3DVERTEXELEMENT9 *vertexFormat = new D3DVERTEXELEMENT9[nFormat + 1];
	for (unsigned int i = 0; i < nFormat; i++){
		vertexFormat[i].Stream = 0;
		vertexFormat[i].Offset = formats[i].offset;
		vertexFormat[i].Type   = getD3DType(formats[i].attFormat, formats[i].size);
		vertexFormat[i].Method = D3DDECLMETHOD_DEFAULT;
		vertexFormat[i].Usage  = d3dSemantics[formats[i].attType];
		vertexFormat[i].UsageIndex = formats[i].index;
	}
	vertexFormat[nFormat].Stream = 0xFF;
	vertexFormat[nFormat].Offset = 0;
	vertexFormat[nFormat].Type   = D3DDECLTYPE_UNUSED;
	vertexFormat[nFormat].Method = 0;
	vertexFormat[nFormat].Usage  = 0;
	vertexFormat[nFormat].UsageIndex = 0;

	dev->CreateVertexDeclaration(vertexFormat, &vDecl);
	delete vertexFormat;

	return true;
}

void Direct3DBatch::freeVertexBuffer(){
	if (vBuf != NULL) vBuf->Release();
	if (iBuf != NULL) iBuf->Release();
	if (vDecl != NULL) vDecl->Release();
}

void Direct3DBatch::draw(LPDIRECT3DDEVICE9 dev){
	dev->SetVertexDeclaration(vDecl);
	if (nIndices > 0){
		dev->SetIndices(iBuf);
		dev->SetStreamSource(0, vBuf, 0, vertexSize);
		dev->DrawIndexedPrimitive(getD3DPrimitive(primitiveType), 0, 0, nVertices, 0, getPrimitives(primitiveType, nIndices));
	} else {
		dev->SetStreamSource(0, vBuf, 0, vertexSize);
		dev->DrawPrimitive(getD3DPrimitive(primitiveType), 0, getPrimitives(primitiveType, nIndices));
	}
}

bool Direct3DModel::uploadToVertexBuffer(LPDIRECT3DDEVICE9 dev){
	bool res = true;
	for (unsigned int i = 0; i < batches.getCount(); i++){
		res &= ((Direct3DBatch *) batches[i])->uploadToVertexBuffer(dev);
	}
	return res;
}

void Direct3DModel::freeVertexBuffer(){
	for (unsigned int i = 0; i < batches.getCount(); i++){
		((Direct3DBatch *) batches[i])->freeVertexBuffer();
	}
}

void Direct3DModel::draw(LPDIRECT3DDEVICE9 dev){
	for (unsigned int i = 0; i < batches.getCount(); i++){
		((Direct3DBatch *) batches[i])->draw(dev);
	}
}
