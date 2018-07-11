#include "stdafx.h"
#include "Entity.h"

unsigned int createEntity(TWorld * ptWorld)
{
	unsigned int nCurrentEntity;
	//Mark entity for Creation by finding the first index in the entity list that has no components. 
	//Return the index of this marked entity
	for (nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; ++nCurrentEntity)
	{
		if (ptWorld->anComponentMask[nCurrentEntity] == COMPONENT_NONE)
		{
			printf("Entity created: %d\n", nCurrentEntity);
			return(nCurrentEntity);
		}
	}

	printf("Error!  No more entities left!\n");
	return(ENTITYCOUNT);
}

void destroyEntity(TWorld * ptWorld, unsigned int nThisEntity)
{
	//Set component list for current entity to none.
	printf("Entity destroyed: %d\n", nThisEntity);
	ptWorld->anComponentMask[nThisEntity] = COMPONENT_NONE;
}

unsigned int createDebugTransformLines(TWorld * ptWorld, TDebugMesh atMesh)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	ptWorld->anComponentMask[nThisEntity] =  COMPONENT_DEBUGMESH;
	ptWorld->atDebugMesh[nThisEntity].m_nVertexCount = 6;

	static TSimpleMesh atLineVertices[]
	{
		TSimpleMesh{ 0,	0, 0, 0, 0,0,0,0,0},//00	Green
		TSimpleMesh{ 0,	2.0f, 0, 0,0,0,0,0},//01	Green
		TSimpleMesh{ 0,	0, 0, 0, 0,0,0,0,0},//00	Red
		TSimpleMesh{ 2.0f, 0, 0, 0,0,0,0,0},//01	Red
		TSimpleMesh{ 0,	0, 0, 0, 0,0,0,0,0},//00	Blue
		TSimpleMesh{ 0,	0, 2.0f, 0,0,0,0,0},//01	Blue	
	};

	

	ptWorld->atDebugMesh[nThisEntity].m_nVertexBufferStride = sizeof(TSimpleMesh);
	ptWorld->atDebugMesh[nThisEntity].m_nVertexBufferOffset = 0;

	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.ByteWidth = sizeof(TSimpleMesh) * ptWorld->atDebugMesh[nThisEntity].m_nVertexCount;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.CPUAccessFlags = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.MiscFlags = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.StructureByteStride = 0;

	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.pSysMem = atLineVertices;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;
	return 0;
}
unsigned int createPlayerBox(TWorld * ptWorld, CCollisionSystem* pcCollisionSystem)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	ptWorld->anComponentMask[nThisEntity] = COMPONENT_DEBUGMESH;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID;//138
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_AABB | COMPONENT_NONSTATIC;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK;
	static TPrimalVert atDebugGridVertices[]{
		//8
		TPrimalVert{ XMFLOAT3(-.2f, 0,  -0.4f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//00	Green
		TPrimalVert{ XMFLOAT3(-.2f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//01	Green
		TPrimalVert{ XMFLOAT3(.0f,  0, -.5f),		XMFLOAT4(0 ,1.0F,	0.0F, 1.0F)},
		TPrimalVert{ XMFLOAT3(.0F,  0, -0.4f),		XMFLOAT4(0,1.0F,	0.0F,1.0F)},
		TPrimalVert{ XMFLOAT3(-.2f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//01	Green
		TPrimalVert{ XMFLOAT3(.0f,  0, -.5f),		XMFLOAT4(0 ,1.0F,	0.0F, 1.0F) },
		TPrimalVert{ XMFLOAT3(.0F,  0, -0.4f),		XMFLOAT4(0,1.0F,	0.0F,1.0F) },
		TPrimalVert{ XMFLOAT3(-.2f, 0,  -0.4f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },

		//16
		TPrimalVert{ XMFLOAT3(-.2f, 0.3f,  -0.4f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//00	Green
		TPrimalVert{ XMFLOAT3(-.2f, 0.3f, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//01	Green
		TPrimalVert{ XMFLOAT3(.0f,  0.3f, -.5f),		XMFLOAT4(0 ,1.0F,	0.0F, 1.0F) },
		TPrimalVert{ XMFLOAT3(.0F,  0.3f, -0.4f),		XMFLOAT4(0,1.0F,	0.0F,1.0F) },
		TPrimalVert{ XMFLOAT3(-.2f, 0.3f, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//01	Green
		TPrimalVert{ XMFLOAT3(.0f,  0.3f, -.5f),		XMFLOAT4(0 ,1.0F,	0.0F, 1.0F) },
		TPrimalVert{ XMFLOAT3(.0F,  0.3f, -0.4f),		XMFLOAT4(0,1.0F,	0.0F,1.0F) },
		TPrimalVert{ XMFLOAT3(-.2f, 0.3f,  -0.4f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },
		//26
		TPrimalVert{ XMFLOAT3(-.2f, 0.3f,  -0.4f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },
		TPrimalVert{ XMFLOAT3(-.2f, 0,  -0.4f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },
		TPrimalVert{ XMFLOAT3(-.2f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },
		TPrimalVert{ XMFLOAT3(-.2f, 0.3f, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },
		TPrimalVert{ XMFLOAT3(.0f,  0, -.5f),		XMFLOAT4(0 ,1.0F,	0.0F, 1.0F) },
		TPrimalVert{ XMFLOAT3(.0f,  0.3f, -.5f),		XMFLOAT4(0 ,1.0F,	0.0F, 1.0F) },
		TPrimalVert{ XMFLOAT3(.0F,  0, -0.4f),		XMFLOAT4(0,1.0F,	0.0F,1.0F) },
		TPrimalVert{ XMFLOAT3(.0F,  0.3f, -0.4f),		XMFLOAT4(0,1.0F,	0.0F,1.0F) },
	/*	TPrimalVert{ XMFLOAT3(-.2f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },
		TPrimalVert{ XMFLOAT3(-.2f, 0.3f, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },*/
	};
	TAABB MyAbb = createAABBS(atDebugGridVertices, 24);
	MyAbb.m_IndexLocation = nThisEntity;
	ptWorld->atAABB[nThisEntity] = MyAbb;
	pcCollisionSystem->AddAABBCollider(MyAbb, nThisEntity);
	ptWorld->atDebugMesh[nThisEntity].m_nVertexCount = 24;
	ptWorld->atDebugMesh[nThisEntity].m_nVertexBufferStride = sizeof(TPrimalVert);
	ptWorld->atDebugMesh[nThisEntity].m_nVertexBufferOffset = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.ByteWidth = sizeof(TPrimalVert) * ptWorld->atDebugMesh[nThisEntity].m_nVertexCount;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.CPUAccessFlags = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.MiscFlags = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.StructureByteStride = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.pSysMem = atDebugGridVertices;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;

	ptWorld->atShaderID[nThisEntity].m_nShaderID = 1;//Shader that supports TPrimalVert
	return 0;
}

TAABB createAABBS(TPrimalVert verticies[],int size)
{
	TAABB aabb;
	aabb.m_dMaxPoint.x = verticies[0].m_d3dfPosition.x;
	aabb.m_dMaxPoint.y = verticies[0].m_d3dfPosition.y;
	aabb.m_dMaxPoint.z = verticies[0].m_d3dfPosition.z;
	aabb.m_dMaxPointOrginal.x = verticies[0].m_d3dfPosition.x;
	aabb.m_dMaxPointOrginal.y = verticies[0].m_d3dfPosition.y;
	aabb.m_dMaxPointOrginal.z = verticies[0].m_d3dfPosition.z;
	aabb.m_dMinPointOrginal.x = verticies[0].m_d3dfPosition.x;
	aabb.m_dMinPointOrginal.y = verticies[0].m_d3dfPosition.y;
	aabb.m_dMinPointOrginal.z = verticies[0].m_d3dfPosition.z;



	aabb.m_dMinPoint.x = verticies[0].m_d3dfPosition.x;
	aabb.m_dMinPoint.y = verticies[0].m_d3dfPosition.y;
	aabb.m_dMinPoint.z = verticies[0].m_d3dfPosition.z;

	for (int i = 1; i < size - 1; ++i) {
		if (verticies[i].m_d3dfPosition.x > aabb.m_dMaxPoint.x) {
			aabb.m_dMaxPoint.x = verticies[i].m_d3dfPosition.x;
			aabb.m_dMaxPointOrginal.x = aabb.m_dMaxPoint.x;

		}
		if (verticies[i].m_d3dfPosition.x < aabb.m_dMinPoint.x) {
			aabb.m_dMinPoint.x = verticies[i].m_d3dfPosition.x;
			aabb.m_dMinPointOrginal.x = aabb.m_dMinPoint.x;

		}

		if (verticies[i].m_d3dfPosition.y > aabb.m_dMaxPoint.y) {
			aabb.m_dMaxPoint.y = verticies[i].m_d3dfPosition.y;
			aabb.m_dMaxPointOrginal.y = aabb.m_dMaxPoint.y;

		}
		if (verticies[i].m_d3dfPosition.y < aabb.m_dMinPoint.y) {
			aabb.m_dMinPoint.y = verticies[i].m_d3dfPosition.y;
			aabb.m_dMinPointOrginal.y = aabb.m_dMinPoint.y;

		}

		if (verticies[i].m_d3dfPosition.z > aabb.m_dMaxPoint.z) {
			aabb.m_dMaxPoint.z = verticies[i].m_d3dfPosition.z;
			aabb.m_dMaxPointOrginal.z = aabb.m_dMaxPoint.z;

		}
		if (verticies[i].m_d3dfPosition.z < aabb.m_dMinPoint.z) {
			aabb.m_dMinPoint.z = verticies[i].m_d3dfPosition.z;
			aabb.m_dMinPointOrginal.z = aabb.m_dMinPoint.z;

		}
	}
	return aabb;
	
}

TAABB updateAABB(XMMATRIX worldMatrix,TAABB aabb, CCollisionSystem* pcCollisionSystem)
{
	XMVECTOR max;
	max.m128_f32[0] = aabb.m_dMaxPointOrginal.x;
	max.m128_f32[1] = aabb.m_dMaxPointOrginal.y;
	max.m128_f32[2] = aabb.m_dMaxPointOrginal.z;
	max.m128_f32[3] = 1;
	max=XMVector4Transform(max, worldMatrix);

	XMVECTOR min;
	min.m128_f32[0] = aabb.m_dMinPointOrginal.x;
	min.m128_f32[1] = aabb.m_dMinPointOrginal.y;
	min.m128_f32[2] = aabb.m_dMinPointOrginal.z;
	min.m128_f32[3] = 1;
	min = XMVector4Transform(min, worldMatrix);
	TAABB newaabb;
	newaabb = aabb;
	newaabb.m_dMaxPoint.x = max.m128_f32[0];
	newaabb.m_dMaxPoint.y = max.m128_f32[1];
	newaabb.m_dMaxPoint.z = max.m128_f32[2];

	newaabb.m_dMinPoint.x = min.m128_f32[0];
	newaabb.m_dMinPoint.y = min.m128_f32[1];
	newaabb.m_dMinPoint.z = min.m128_f32[2];
	newaabb.m_dMinPointOrginal = aabb.m_dMinPointOrginal;
	newaabb.m_dMaxPointOrginal = aabb.m_dMaxPointOrginal;

	pcCollisionSystem->replaceAABB(aabb.m_IndexLocation,newaabb);
	return newaabb;

	
}


unsigned int createDebugGrid(TWorld * ptWorld)
{
	/*
		Components:
			Graphics:
				Mask
				DebugMesh
				ShaderID
			Collision:
				Mask
			UI:
				Mask
			AI:
				Mask
			Physics:
				Mask
			Unique:
				Random number
	*/
	unsigned int nThisEntity = createEntity(ptWorld);
	ptWorld->anComponentMask[nThisEntity] = COMPONENT_DEBUGMESH;

	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask	= COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID;//138
	ptWorld->atAIMask[nThisEntity].m_tnAIMask				= COMPONENT_AIMASK;
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask =  COMPONENT_STATIC| COMPONENT_AABB;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask				= COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask		= COMPONENT_PHYSICSMASK;

	static TPrimalVert atDebugGridVertices[]
	{
		TPrimalVert{ XMFLOAT3(-.5f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//00	Green
		TPrimalVert{ XMFLOAT3(-.5f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//01	Green
		TPrimalVert{ XMFLOAT3(-.4f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//02	Red
		TPrimalVert{ XMFLOAT3(-.4f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//03	Red
		TPrimalVert{ XMFLOAT3(-.3f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//04	Blue
		TPrimalVert{ XMFLOAT3(-.3f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//05	Blue	
		TPrimalVert{ XMFLOAT3(-.2f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//06	Blue
		TPrimalVert{ XMFLOAT3(-.2f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//07	Red
		TPrimalVert{ XMFLOAT3(-.1f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//08	Red
		TPrimalVert{ XMFLOAT3(-.1f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//09	Green
		TPrimalVert{ XMFLOAT3( .0f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//10	Green							 		   
		TPrimalVert{ XMFLOAT3( .0f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//11	Green
		TPrimalVert{ XMFLOAT3( .1f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//12	Green	
		TPrimalVert{ XMFLOAT3( .1f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//13	Green
		TPrimalVert{ XMFLOAT3( .2f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//14	Green	
		TPrimalVert{ XMFLOAT3( .2f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//15	Green
		TPrimalVert{ XMFLOAT3( .3f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//16	Green								  
		TPrimalVert{ XMFLOAT3( .3f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//17	Green
		TPrimalVert{ XMFLOAT3( .4f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//18	Green	
		TPrimalVert{ XMFLOAT3( .4f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//19	Green
		TPrimalVert{ XMFLOAT3( .5f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//20	Green	
		TPrimalVert{ XMFLOAT3( .5f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//21	Green
		
		TPrimalVert{ XMFLOAT3( .5f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//00	Green
		TPrimalVert{ XMFLOAT3(-.5f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//01	Green
		TPrimalVert{ XMFLOAT3( .5f, 0, -.4f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//02	Red
		TPrimalVert{ XMFLOAT3(-.5f, 0, -.4f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//03	Red
		TPrimalVert{ XMFLOAT3( .5f, 0, -.3f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//04	Blue
		TPrimalVert{ XMFLOAT3(-.5f, 0, -.3f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//05	Blue	
		TPrimalVert{ XMFLOAT3( .5f, 0, -.2f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//06	Blue
		TPrimalVert{ XMFLOAT3(-.5f, 0, -.2f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//07	Red
		TPrimalVert{ XMFLOAT3( .5f, 0, -.1f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//08	Red
		TPrimalVert{ XMFLOAT3(-.5f, 0, -.1f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//09	Green
		TPrimalVert{ XMFLOAT3( .5f, 0,  .0f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//10	Green							 		   
		TPrimalVert{ XMFLOAT3(-.5f, 0,  .0f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//11	Green
		TPrimalVert{ XMFLOAT3( .5f, 0,  .1f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//12	Green	
		TPrimalVert{ XMFLOAT3(-.5f, 0,  .1f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//13	Green
		TPrimalVert{ XMFLOAT3( .5f, 0,  .2f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//14	Green	
		TPrimalVert{ XMFLOAT3(-.5f, 0,  .2f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//15	Green
		TPrimalVert{ XMFLOAT3( .5f, 0,  .3f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//16	Green								  
		TPrimalVert{ XMFLOAT3(-.5f, 0,  .3f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//17	Green
		TPrimalVert{ XMFLOAT3( .5f, 0,  .4f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//18	Green	
		TPrimalVert{ XMFLOAT3(-.5f, 0,  .4f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//19	Green
		TPrimalVert{ XMFLOAT3( .5f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//20	Green	
		TPrimalVert{ XMFLOAT3(-.5f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//21	Green
	};
	
	ptWorld->atDebugMesh[nThisEntity].m_nVertexCount = 44;
	ptWorld->atDebugMesh[nThisEntity].m_nVertexBufferStride = sizeof(TPrimalVert);
	ptWorld->atDebugMesh[nThisEntity].m_nVertexBufferOffset = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.ByteWidth = sizeof(TPrimalVert) * ptWorld->atDebugMesh[nThisEntity].m_nVertexCount;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.CPUAccessFlags = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.MiscFlags = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.StructureByteStride = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.pSysMem = atDebugGridVertices;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;

	ptWorld->atShaderID[nThisEntity].m_nShaderID = 1;//Shader that supports TPrimalVert

	return 0;
}

unsigned int createMesh(TWorld * ptWorld, ID3D11Device * m_pd3dDevice, TMesh tMesh)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	
	#pragma region ReadAnimationData	
	int nAnimationFrameCount = 0;
	int nNodeCount = 0;
	double dAnimationDuration = 0;
	std::vector<int> m_vnParentIndices;
	std::vector<XMMATRIX> vd3dInvertedBindPoseMatrices;
	std::ifstream cMeshDataFile("../meshData.txt", std::ios::in | std::ios::binary);
	cMeshDataFile.read((char*)&dAnimationDuration, sizeof(double));	
	cMeshDataFile.read((char*)&nNodeCount, sizeof(int));
	cMeshDataFile.read((char*)&nAnimationFrameCount, sizeof(int));
	TAnimationClip tAnimationClip;
	char * pchFileName1, *pchFileName2, *pchFileName3;

	int nFileReadingCursorIndex = sizeof(double) + sizeof(int) + sizeof(int);
	for (int nKeyframeIndex = 0; nKeyframeIndex < nAnimationFrameCount; nKeyframeIndex++)
	{
		TKeyframe tCurrentKeyframe;

		//current frame time offset
		double dCurrentTime = 0;
		cMeshDataFile.read((char*)&dCurrentTime, sizeof(double));
		nFileReadingCursorIndex += sizeof(double);
		XMFLOAT4X4 d3dCurrentJointMatrix;
		for (int nCurrentNodeIndex = 0; nCurrentNodeIndex < nNodeCount; nCurrentNodeIndex++)
		{
			for (int nCurrentRowIndex = 0; nCurrentRowIndex < 4; nCurrentRowIndex++)
			{
				for (int nCurrentColumnIndex = 0; nCurrentColumnIndex < 4; nCurrentColumnIndex++)
				{
					//joint matrix data

					cMeshDataFile.read((char*)&d3dCurrentJointMatrix(nCurrentRowIndex, nCurrentColumnIndex), sizeof(float));
					nFileReadingCursorIndex += sizeof(float);
				}
			}	
			tCurrentKeyframe.m_vd3dJointMatrices.push_back(XMLoadFloat4x4(&d3dCurrentJointMatrix));
		}
		//myKey.joints = XMLoadFloat4x4(&temp);
		tCurrentKeyframe.dTime = dCurrentTime / 1000;
		tAnimationClip.m_vtKeyFrames.push_back(tCurrentKeyframe);
	}
	for (int nCurrentParentIndex = 0; nCurrentParentIndex < nNodeCount; nCurrentParentIndex++)
	{
		int nParentIndexToPush = 0;
		cMeshDataFile.read((char*)&nParentIndexToPush, sizeof(int));
		nFileReadingCursorIndex += sizeof(int);
		m_vnParentIndices.push_back(nParentIndexToPush);
	}
	XMFLOAT4X4 d3dCurrentInvertedBindPoseMatrix;
	for (int nCurrentNodeIndex = 0; nCurrentNodeIndex < nNodeCount; nCurrentNodeIndex++)
	{
		for (int nCurrentRowIndex = 0; nCurrentRowIndex < 4; nCurrentRowIndex++)
		{
			for (int nCurrentColumnIndex = 0; nCurrentColumnIndex < 4; nCurrentColumnIndex++)
			{
				//joint matrix data

				cMeshDataFile.read((char*)&d3dCurrentInvertedBindPoseMatrix(nCurrentRowIndex,nCurrentColumnIndex), sizeof(float));
				nFileReadingCursorIndex += sizeof(float);
			}
		}
		vd3dInvertedBindPoseMatrices.push_back(XMMatrixInverse(NULL, XMLoadFloat4x4(&d3dCurrentInvertedBindPoseMatrix)));
	}

	tAnimationClip.dDuration = dAnimationDuration;

	tAnimationClip.m_vnParentIndicies = m_vnParentIndices;
#pragma endregion

	#pragma region ReadMaterials

	/*
	Things to write:
		-MaterialType(sizeof(char))
			if(phong)
				-AmbientColor(24 byte double3)
				-DiffuseColor(24 byte double3)
				-SpecularColor(24 byte double3)
				-EmmisiveColor(24 byte double3)
				-TransparencyFactor(8 byte double)
				-Shininess(8 byte double)
				-ReflectionFactor(8 byte double)
			if(lambert)
				-AmbientColor(24 byte double3)
				-DiffuseColor(24 byte double3)
				-SpecularColor(24 byte double3)
				-EmmisiveColor(24 byte double3)
				-TransparencyFactor(8 byte double)
	*/
	int nMaterialCount = 0;
	//matFile.read((char*)&materialCount, sizeof(int));
	char chLambert = false;
	double adAmbientColor[3];
	double adDiffuseColor[3];
	double adSpecularColor[3];
	double adEmmissiveColor[3];
	//double transparencyFactor;
	double dShininess, dTransparency;
	//double reflectionFactor;
	int nMaterialBufferSize = 0;
	nMaterialBufferSize = sizeof(chLambert) + sizeof(adAmbientColor) + sizeof(adDiffuseColor) + sizeof(adSpecularColor) + sizeof(adEmmissiveColor) + sizeof(dShininess);
	char *pchMaterialBuffer = new char[nMaterialBufferSize];
	cMeshDataFile.clear();
	cMeshDataFile.seekg(nFileReadingCursorIndex);
	cMeshDataFile.read(pchMaterialBuffer, nMaterialBufferSize);
	/*
		-AmbientColor(24 byte double3)
		- DiffuseColor(24 byte double3)
		- SpecularColor(24 byte double3)
		- EmmisiveColor(24 byte double3)
		- Shininess(8 byte double)
	*/

	memcpy(&chLambert, &pchMaterialBuffer[0], sizeof(chLambert));

	if (chLambert)
	{
		nFileReadingCursorIndex += sizeof(chLambert) + sizeof(adAmbientColor) + sizeof(adDiffuseColor) + sizeof(adEmmissiveColor) + sizeof(dTransparency);

		memcpy(&adAmbientColor, &pchMaterialBuffer[sizeof(chLambert)], sizeof(adAmbientColor));

		memcpy(&adDiffuseColor, &pchMaterialBuffer[sizeof(chLambert) + sizeof(adAmbientColor)], sizeof(adDiffuseColor));

		memcpy(&adEmmissiveColor, &pchMaterialBuffer[sizeof(chLambert) + sizeof(adAmbientColor) + sizeof(adDiffuseColor)], sizeof(adEmmissiveColor));

		memcpy(&dTransparency, &pchMaterialBuffer[sizeof(chLambert) + sizeof(adAmbientColor) + sizeof(adDiffuseColor)] + sizeof(adEmmissiveColor), sizeof(double));
	}
	else
	{
		nFileReadingCursorIndex += sizeof(chLambert) + sizeof(adAmbientColor) + sizeof(adDiffuseColor) + sizeof(adSpecularColor) + sizeof(adEmmissiveColor) + sizeof(dShininess);

		memcpy(&adAmbientColor, &pchMaterialBuffer[sizeof(chLambert)], sizeof(adAmbientColor));

		memcpy(&adDiffuseColor, &pchMaterialBuffer[sizeof(chLambert) + sizeof(adAmbientColor)], sizeof(adDiffuseColor));

		memcpy(&adSpecularColor, &pchMaterialBuffer[sizeof(chLambert) + sizeof(adAmbientColor) + sizeof(adDiffuseColor)], sizeof(adSpecularColor));

		memcpy(&adEmmissiveColor, &pchMaterialBuffer[sizeof(chLambert) + sizeof(adAmbientColor) + sizeof(adDiffuseColor) + sizeof(adSpecularColor)], sizeof(adEmmissiveColor));

		memcpy(&dShininess, &pchMaterialBuffer[sizeof(chLambert) + sizeof(adAmbientColor) + sizeof(adDiffuseColor) + sizeof(adSpecularColor) + sizeof(adEmmissiveColor)], sizeof(double));
	}



#pragma endregion

	#pragma region ReadMesh
	int nPolygonVertexCount = 0;
	cMeshDataFile.clear();
	cMeshDataFile.seekg(nFileReadingCursorIndex);
	cMeshDataFile.read((char*)&nPolygonVertexCount, sizeof(int));
	int nMeshBufferSize = nPolygonVertexCount * sizeof(TSimpleMesh);
	char *pchMeshBuffer = new char[nMeshBufferSize];
	cMeshDataFile.read(pchMeshBuffer, nMeshBufferSize);

	if (!cMeshDataFile) {
		// An error occurred!
		// myFile.gcount() returns the number of bytes read.
		// calling myFile.clear() will reset the stream state
		// so it is usable again.
		cMeshDataFile.clear();
	}

	TSimpleMesh * ptSimpleMesh = new TSimpleMesh[nPolygonVertexCount];
	for (int nCurrentSimpleMeshMember = 0; nCurrentSimpleMeshMember < nPolygonVertexCount; nCurrentSimpleMeshMember++)
	{
		memcpy(&ptSimpleMesh[nCurrentSimpleMeshMember], &pchMeshBuffer[nCurrentSimpleMeshMember * sizeof(TSimpleMesh)], sizeof(TSimpleMesh));
	}

#pragma endregion

	#pragma region ReadTextures

	int anFileSizes[3];
	char*pchTextureFileBuffer = new char[sizeof(anFileSizes)];

	cMeshDataFile.read(pchTextureFileBuffer, sizeof(anFileSizes));
	if (!cMeshDataFile) {
		// An error occurred!
		// matFile.gcount() returns the number of bytes read.
		// calling matFile.clear() will reset the stream state
		// so it is usable again.
		cMeshDataFile.clear();
	}
	int nFileNamesSize = 0;
	for (int i = 0; i < 3; i++)
	{
		memcpy(&anFileSizes[i], &pchTextureFileBuffer[i * sizeof(int)], sizeof(int));
		nFileNamesSize += anFileSizes[i];
	}

	char * pchTextureFileNameBuffer = new char[nFileNamesSize];
	pchFileName1 = new char[anFileSizes[0]];
	pchFileName2 = new char[anFileSizes[1]];
	pchFileName3 = new char[anFileSizes[2]];
	
	//matFile.seekg(sizeof(fileSize), std::ios_base::cur);
	cMeshDataFile.read(pchFileName1, anFileSizes[0]);
	
	//matFile.seekg(fileSize[0], std::ios_base::cur);
	cMeshDataFile.read(pchFileName2, anFileSizes[1]);
	
	//matFile.seekg(fileSize[1], std::ios_base::cur);
	cMeshDataFile.read(pchFileName3, anFileSizes[2]);

	cMeshDataFile.close();

#pragma endregion

	#pragma region CreateTexturesFromFile
	wchar_t acFileName1[260];
	wchar_t acFileName2[260];
	wchar_t acFileName3[260];
	size_t nResult = 0;
	mbstate_t cMBState;

	mbsrtowcs_s(&nResult, acFileName1, 260, (const char **)(&pchFileName1), anFileSizes[0], &cMBState);
	mbsrtowcs_s(&nResult, acFileName2, 260, (const char **)(&pchFileName2), anFileSizes[1], &cMBState);
	mbsrtowcs_s(&nResult, acFileName3, 260, (const char **)(&pchFileName3), anFileSizes[2], &cMBState);


#pragma endregion

	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_ANIMATION | COMPONENT_TEXTURE | COMPONENT_SHADERID;

	ptWorld->atMesh[nThisEntity].m_nVertexCount = nPolygonVertexCount;
	ptWorld->atMesh[nThisEntity].m_nVertexBufferStride = sizeof(TSimpleMesh);
	ptWorld->atMesh[nThisEntity].m_nVertexBufferOffset = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.ByteWidth = sizeof(TSimpleMesh) * ptWorld->atMesh[nThisEntity].m_nVertexCount;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.CPUAccessFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.MiscFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.StructureByteStride = 0;

	ptWorld->atMesh[nThisEntity].m_d3dVertexData.pSysMem = ptSimpleMesh;
	ptWorld->atMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;

	return 0;
}

