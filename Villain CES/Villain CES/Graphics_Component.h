#pragma once
#ifndef CES_GRAPHICS_COMPONENT_H
#define CES_GRAPHICS_COMPONENT_H

typedef enum
{
	COMPONENT_NONE = 0,
	COMPONENT_MESH = 1 << 1,
	COMPONENT_DEBUGMESH = 1 << 2,
	COMPONENT_ANIMATION = 1 << 3,
	COMPONENT_MATERIAL = 1 << 4,
	COMPONENT_TEXTURE = 1 << 5,
	COMPONENT_SHADERID = 1 << 6 // have information on what inputs and outputs go to other shaders as well
} Graphics_Component;

struct keyframe { double time = 0; std::vector<XMMATRIX> joints; }; struct anim_clip { double duration = 0; std::vector<keyframe> frames; std::vector<int> parent_Indicies; };

typedef struct
{
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *indexBuffer;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;
	UINT vertexCount;
	UINT indexCount;
	UINT vertexBufferStride;
	UINT vertexBufferOffset;
} Mesh;

typedef struct
{
	ID3D11Buffer *vertexBuffer;
	int vertexCount;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	UINT			vertexBufferStride;
	UINT			vertexBufferOffset;
} DebugMesh;

typedef struct
{
	anim_clip anim;
	std::vector<int> parent_Indices;
} Animation;

typedef struct
{
	/*
		TODO:
		Fill out Material Struct
	*/
} Material;

typedef struct
{
	/*
		TODO:
		Fill out Material Struct
	*/
} Texture;

typedef struct
{
	int shaderID = 0;
} ShaderID;


#endif
