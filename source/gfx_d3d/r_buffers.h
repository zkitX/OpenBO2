#pragma once

#include <d3d11.h>

struct SkinnedVertsDef
{
	const char* name;
	unsigned int maxSkinnedVerts;
};

struct GfxVertexBufferState
{
	volatile int used;
	int total;
	ID3D11Buffer* buffer;
	char* verts;
};

struct GfxIndexBufferState
{
	volatile int used;
	int total;
	ID3D11Buffer* buffer;
	unsigned __int16* indices;
};

struct GfxBuffers
{
	GfxIndexBufferState dynamicIndexBufferPool[2];
	GfxVertexBufferState skinnedCacheVbPool[2];
	char* skinnedCacheLockAddr;
	GfxVertexBufferState dynamicVertexBufferPool[2];
	ID3D11Buffer* particleCloudVertexBuffer;
	ID3D11Buffer* particleCloudIndexBuffer;
	int dynamicBufferFrame;
};

GfxBuffers gfxBuf;

int alwaysfails;

char* R_LockVertexBuffer(ID3D11DeviceContext* device, ID3D11Buffer* handle, int offset, int bytes, int lockFlags);
void R_UnlockVertexBuffer(ID3D11DeviceContext* device, ID3D11Buffer* handle);
char* R_LockIndexBuffer(ID3D11DeviceContext* device, ID3D11Buffer* handle, int offset, int bytes, int lockFlags);
void R_UnlockIndexBuffer(ID3D11DeviceContext* device, ID3D11Buffer* handle);
char* R_AllocDynamicVertexBuffer(ID3D11Buffer** vb, int sizeInBytes);
void* R_AllocStaticVertexBuffer(ID3D11Buffer** vb, int sizeInBytes, const void* data);
void* R_AllocDynamicIndexBuffer(ID3D11Buffer** ib, int sizeInBytes);
void* R_AllocStaticIndexBuffer(ID3D11Buffer** ib, int sizeInBytes, const void* data);
void Load_IndexBuffer(ID3D11Buffer** ib, void* bufferData, int indexCount);
void Load_VertexBuffer(ID3D11Buffer** vb, void* bufferData, int sizeInBytes);
void R_InitDynamicVertexBufferState(GfxVertexBufferState* vb, int bytes);
void R_InitSkinnedVertexBuffers(SkinnedVertsDef* sv);
void R_FreeStaticVertexBuffer(ID3D11Buffer* vb);
void R_FreeStaticIndexBuffer(ID3D11Buffer* ib);
void R_ShutdownSkinnedVertexBuffers();