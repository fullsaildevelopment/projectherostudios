#pragma once
#ifndef CES_GRAPHICS_COMPONENT_H
#define CES_GRAPHICS_COMPONENT_H
enum eGraphicsComponent
{
	COMPONENT_NONE			= 0,
	COMPONENT_GRAPHICSMASK	= 1 << 0,
	COMPONENT_MESH			= 1 << 1,
	COMPONENT_DEBUGMESH		= 1 << 2,
	COMPONENT_ANIMATION		= 1 << 3,
	COMPONENT_MATERIAL		= 1 << 4,
	COMPONENT_TEXTURE		= 1 << 5,
	COMPONENT_SHADERID		= 1 << 6, // have information on what inputs and outputs go to other shaders as well
	COMPONENT_CAMERA		= 1 << 7,
	COMPONENT_SIMPLEMESH	= 1 << 8,
	COMPONENT_PBRMESH		= 1 << 9
};

struct TGraphicsMask
{
	int m_tnGraphicsMask = 0;
};

struct TKeyframe 
{ 
	double dTime = 0; 
	std::vector<XMMATRIX> m_vd3dJointMatrices = std::vector<XMMATRIX>(); 
}; 

struct TAnimationClip 
{ 
	double dDuration = 0;
	std::vector<TKeyframe> m_vtKeyFrames; 
	std::vector<int> m_vnParentIndicies = std::vector<int>(0);
};

struct TMeshFormat
{
	float pos[4] = { 0, 0, 0, 0 };
	float norm[3] = { 0, 0, 0 };
	float tang[4] = { 0, 0, 0, 0 };
	float binorm[4] = { 0, 0, 0, 0 };
	float uv[2] = { 0, 0 };
	float weights[4] = { 0, 0, 0, 0 };
	int joints[4] = { 0, 0, 0, 0 };
};

struct TPrimitiveMesh
{
	float pos[4] = { 0, 0, 0, 0 };
	float uv[2] = { 0, 0 };
	float& operator[] (int i) { return (&pos[0])[i]; }
};

struct TAnimatedMesh
{
	float pos[4] = { 0, 0, 0, 0 };
	float uv[2] = { 0, 0 };
	float weights[4] = { 0, 0, 0, 0 };
	int joints[4] = { 0, 0, 0, 0 };
	float& operator[] (int i) { return (&pos[0])[i]; }
};

struct TMyAmbientColor
{
	double r = 0.0;
	double g = 0.0;
	double b = 0.0;
	double& operator[] (int i) { return (&r)[i]; }
};

struct TMyDiffuseColor
{
	double r = 0.0;
	double g = 0.0;
	double b = 0.0;
	double& operator[] (int i) { return (&r)[i]; }
};

struct TMySpecularColor
{
	double r = 0.0;
	double g = 0.0;
	double b = 0.0;
	double& operator[] (int i) { return (&r)[i]; }
};

struct TMyEmissiveColor
{
	double r = 0.0;
	double g = 0.0;
	double b = 0.0;
	double& operator[] (int i) { return (&r)[i]; }
};

struct TMyFileNames
{
	char * fileName1 = nullptr;
	char * fileName2 = nullptr;
	char * fileName3 = nullptr;
	char * fileName4 = nullptr;
	char * fileName5 = nullptr;

	char *& operator[] (int i) { return (&fileName1)[i]; }
};

struct TMyFileNameSizes
{
	int fileNameSize1 = 0;
	int fileNameSize2 = 0;
	int fileNameSize3 = 0;
	int fileNameSize4 = 0;
	int fileNameSize5 = 0;
	int & operator[] (int i) { return (&fileNameSize1)[i]; }
};

struct TPBRFileNames
{
	char * pbrFileName1 = nullptr;
	char * pbrFileName2 = nullptr;
	char * pbrFileName3 = nullptr;
	char * pbrFileName4 = nullptr;
	char * pbrFileName5 = nullptr;
	char * pbrFileName6 = nullptr;
	char * pbrFileName7 = nullptr;
	char * pbrFileName8 = nullptr;
	char * pbrFileName9 = nullptr;

	char *& operator[] (int i) { return (&pbrFileName1)[i]; }
};

struct TPBRFileNameSizes
{
	int pbrFileNameSize1 = 0;
	int pbrFileNameSize2 = 0;
	int pbrFileNameSize3 = 0;
	int pbrFileNameSize4 = 0;
	int pbrFileNameSize5 = 0;
	int pbrFileNameSize6 = 0;
	int pbrFileNameSize7 = 0;
	int pbrFileNameSize8 = 0;
	int pbrFileNameSize9 = 0;
	int & operator[] (int i) { return (&pbrFileNameSize1)[i]; }
};

struct TMeshImport
{
	double worldTranslation[3];
	double worldRotation[3];
	double worldScaling[3];
	int hasPolygon = 0;
	int nUniqueVertexCount = 0;
	int nPolygonVertexCount = 0;
	TMeshFormat* meshArrays;
	unsigned int * indexBuffer = nullptr;
};

struct TMaterialImport
{
	int hasMaterial;
	int lambert;
	float fRoughness;
	float fMetallicness;
	double dTransparencyOrShininess;
	TMyAmbientColor		m_tAmbientColor;
	TMyDiffuseColor		m_tDiffuseColor;
	TMySpecularColor	m_tSpecularColor;
	TMyEmissiveColor	m_tEmissiveColor;
	TPBRFileNameSizes	m_tPBRFileNameSizes;
	TPBRFileNames		m_tPBRFileNames;
	TMyFileNameSizes	m_tFileNameSizes;
	TMyFileNames		m_tFileNames;
};

struct TAnimationImport
{
	int hasPose = 0;
	TAnimationClip animClip;
};

struct ImporterData
{
	int meshCount = 0;
	TMeshImport* vtMeshes;
	TMaterialImport* vtMaterials;
	TAnimationImport* vtAnimations;
};

struct TMesh
{
	ID3D11Buffer *m_pd3dVertexBuffer;
	ID3D11Buffer *m_pd3dIndexBuffer;
	D3D11_BUFFER_DESC m_d3dVertexBufferDesc;
	D3D11_BUFFER_DESC m_d3dIndexBufferDesc;
	D3D11_SUBRESOURCE_DATA m_d3dVertexData;
	D3D11_SUBRESOURCE_DATA m_d3dIndexData;
	UINT m_nVertexCount = 0;
	UINT m_nIndexCount = 0;
	UINT m_nVertexBufferStride = 0;
	UINT m_nVertexBufferOffset = 0;
	ID3D11ShaderResourceView* m_d3dSRVDiffuse = nullptr;
	std::vector<XMFLOAT3>	m_VertexData;

};

struct TDebugMesh
{
	ID3D11Buffer *			m_pd3dVertexBuffer;
	D3D11_BUFFER_DESC		m_d3dVertexBufferDesc;
	D3D11_SUBRESOURCE_DATA	m_d3dVertexData;
	UINT					m_nVertexCount = 0;
	UINT					m_nVertexBufferStride = 0;
	UINT					m_nVertexBufferOffset = 0;
	std::vector<XMVECTOR>	m_VertexData;

};	

struct TSimpleMesh
{
	ID3D11Buffer *			m_pd3dVertexBuffer;
	ID3D11Buffer *			m_pd3dIndexBuffer;
	D3D11_BUFFER_DESC		m_d3dVertexBufferDesc;
	D3D11_BUFFER_DESC		m_d3dIndexBufferDesc;

	D3D11_SUBRESOURCE_DATA	m_d3dVertexData;
	D3D11_SUBRESOURCE_DATA	m_d3dIndexData;


	UINT					m_nVertexCount = 0;
	UINT					m_nIndexCount = 0;
	UINT					m_nVertexBufferStride = 0;
	UINT					m_nVertexBufferOffset = 0;

	std::vector<XMFLOAT3>		m_VertexData;
	XMFLOAT4                m_nColor;


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

struct TCamera
{
	/*
		Fill out this structure
	*/
};
struct TWorldMatrix 
{
	XMMATRIX worldMatrix = XMMatrixIdentity();
};
#endif
