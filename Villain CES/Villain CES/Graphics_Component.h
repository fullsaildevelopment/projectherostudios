#pragma once
#ifndef CES_GRAPHICS_COMPONENT_H
#define CES_GRAPHICS_COMPONENT_H

enum eGraphicsComponent
{
	COMPONENT_NONE			= 0,
	COMPONENT_GRAPHICSMASK	= 1 << 1,
	COMPONENT_MESH			= 1 << 2,
	COMPONENT_DEBUGMESH		= 1 << 3,
	COMPONENT_ANIMATION		= 1 << 4,
	COMPONENT_MATERIAL		= 1 << 5,
	COMPONENT_TEXTURE		= 1 << 6,
	COMPONENT_SHADERID		= 1 << 7 // have information on what inputs and outputs go to other shaders as well
};

struct TKeyframe 
{ 
	double dTime = 0; 
	std::vector<XMMATRIX> m_vd3dJointMatrices; 
}; 

struct TAnimationClip 
{ 
	double dDuration = 0;
	std::vector<TKeyframe> m_vtKeyFrames; 
	std::vector<int> m_vnParentIndicies; 
};

struct TGraphicsMask
{
	int m_tnGraphicsMask = 0;
};

struct TMesh
{
	ID3D11Buffer *m_pd3dVertexBuffer;
	ID3D11Buffer *m_pd3dIndexBuffer;
	D3D11_BUFFER_DESC m_d3dVertexBufferDesc;
	D3D11_BUFFER_DESC m_d3dIndexBufferDesc;
	D3D11_SUBRESOURCE_DATA m_d3dVertexData;
	D3D11_SUBRESOURCE_DATA m_d3dIndexData;
	UINT m_nVertexCount;
	UINT m_nIndexCount;
	UINT m_nVertexBufferStride;
	UINT m_nVertexBufferOffset;
};

struct TDebugMesh
{
	ID3D11Buffer *m_pd3dVertexBuffer;
	int m_nVertexCount;
	D3D11_BUFFER_DESC m_d3dVertexBufferDesc;
	D3D11_SUBRESOURCE_DATA m_d3dVertexData;
	UINT			m_nVertexBufferStride;
	UINT			m_nVertexBufferOffset;
};

struct TAnimation
{
	TAnimationClip m_tAnim;
	std::vector<int> m_vnParentIndices;
};

struct TMaterial
{
	/*
		TODO:
		Fill out Material Struct
	*/
};

struct TTexture
{
	/*
		TODO:
		Fill out Material Struct
	*/
};

struct TShaderID	
{
	int m_nShaderID = 0;
};


#endif
