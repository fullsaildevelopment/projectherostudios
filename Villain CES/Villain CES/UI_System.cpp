#include "stdafx.h"
#include "UI_System.h"


CUISystem::CUISystem()
{
}


CUISystem::~CUISystem()
{
}

void CUISystem::DrawMenu(TWorld* ptWorld, CGraphicsSystem* pcGraphicsSystem, XMMATRIX cameraMatrix, XMMATRIX worldMatrix)	// PROBABLY CHANGE TO SOMETHING LIKE InitializeMenu()
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID;//138
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK;

	static TPrimalVert atRectVertices[]
	{
		TPrimalVert{ XMFLOAT3(0.3f, 0.2f, 0.0f), XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f) },
		TPrimalVert{ XMFLOAT3(0.3f, 0.4f, 0.0f), XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f) },
		TPrimalVert{ XMFLOAT3(0.5f, 0.2f, 0.0f), XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f) },
		TPrimalVert{ XMFLOAT3(0.5f, 0.4f, 0.0f), XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f) }
	};

	static short rectIndices[]
	{
		0, 2, 1,
		2, 3, 1
	};

	ptWorld->atSimpleMesh[nThisEntity].m_nIndexCount = 6;
	ptWorld->atSimpleMesh[nThisEntity].m_nVertexCount = 4;

	ptWorld->atSimpleMesh[nThisEntity].m_nVertexBufferStride = sizeof(TPrimalVert);
	ptWorld->atSimpleMesh[nThisEntity].m_nVertexBufferOffset = 0;

	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexBufferDesc.ByteWidth = sizeof(TPrimalVert) * ptWorld->atSimpleMesh[nThisEntity].m_nVertexCount;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexBufferDesc.CPUAccessFlags = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexBufferDesc.MiscFlags = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexBufferDesc.StructureByteStride = 0;

	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexBufferDesc.ByteWidth = sizeof(short) * ptWorld->atSimpleMesh[nThisEntity].m_nIndexCount;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexBufferDesc.CPUAccessFlags = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexBufferDesc.MiscFlags = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexBufferDesc.StructureByteStride = 0;

	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.pSysMem = atRectVertices;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.pSysMem = rectIndices;

	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;

	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.SysMemPitch = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.SysMemSlicePitch = 0;

	ptWorld->atShaderID[nThisEntity].m_nShaderID = 3;

	pcGraphicsSystem->CreateEntityBuffer(ptWorld, nThisEntity);
	
	//pcGraphicsSystem->InitPrimalShaderData2()
}
