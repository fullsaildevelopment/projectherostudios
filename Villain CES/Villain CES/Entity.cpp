#include "stdafx.h"
#include "Entity.h"

unsigned int createEntity(TWorld * ptWorld)
{
	unsigned int nCurrentEntity;
	//Mark entity for Creation by finding the first index in the entity list that has no components. 
	//Return the index of this marked entity
	for (nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; ++nCurrentEntity)
	{
		if (ptWorld->atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == COMPONENT_NONE)
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
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_NONE;
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK;
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK;
	ptWorld->atProjectiles[nThisEntity].m_tnProjectileMask = COMPONENT_PROJECTILESMASK;
	XMVECTOR zeroVector;
	zeroVector.m128_f32[0] = 0;
	zeroVector.m128_f32[1] = 0;
	zeroVector.m128_f32[2] = 0;
	zeroVector.m128_f32[3] = 0;

	ptWorld->atRigidBody[nThisEntity].totalForce = zeroVector;
	ptWorld->atRigidBody[nThisEntity].velocity = zeroVector;
	ptWorld->atRigidBody[nThisEntity].gravity = zeroVector;
	ptWorld->atRigidBody[nThisEntity].maxVelocity = zeroVector;



}

unsigned int createDebugTransformLines(TWorld * ptWorld)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	//ptWorld->anComponentMask[nThisEntity] =  COMPONENT_DEBUGMESH;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID;//138
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK;
	ptWorld->atDebugMesh[nThisEntity].m_nVertexCount = 6;

	static TPrimalVert atLineVertices[]
	{
		TPrimalVert{ XMFLOAT3(0, 0, 0),    XMFLOAT4(1,0,0,1)},//00	Green
		TPrimalVert{ XMFLOAT3(0, 2.0f, 0), XMFLOAT4(1,0,0,1)},//01	Green
		TPrimalVert{ XMFLOAT3(0, 0, 0),    XMFLOAT4(0,1,0,1)},//00	Red
		TPrimalVert{ XMFLOAT3(2.0f, 0, 0), XMFLOAT4(0,1,0,1)},//01	Red
		TPrimalVert{ XMFLOAT3(0, 0, 0),    XMFLOAT4(0,0,1,1)},//00	Blue
		TPrimalVert{ XMFLOAT3(0, 0, 2.0f), XMFLOAT4(0,0,1,1)},//01	Blue	
	};

	

	ptWorld->atDebugMesh[nThisEntity].m_nVertexBufferStride = sizeof(TPrimalVert);
	ptWorld->atDebugMesh[nThisEntity].m_nVertexBufferOffset = 0;

	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.ByteWidth = sizeof(TPrimalVert) * ptWorld->atDebugMesh[nThisEntity].m_nVertexCount;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.CPUAccessFlags = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.MiscFlags = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.StructureByteStride = 0;

	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.pSysMem = atLineVertices;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;
	ptWorld->atShaderID[nThisEntity].m_nShaderID = 2;
	return 0;
}

unsigned int createCube(TWorld * ptWorld)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID;
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK;


	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{XMFLOAT3(-0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(-0.5f, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(0.5f, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(-0.5f, 0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(-0.5f, -0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(0.5f, -0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right
														   					 
		TPrimalVert{ XMFLOAT3(0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(-0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(-0.5f, -0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//10 Bottom B Left
		TPrimalVert{ XMFLOAT3(0.5f, -0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(-0.5f, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(-0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
		TPrimalVert{ XMFLOAT3(0.5f, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }//15 Bottm F Right
	};

	static short cubeIndices[]
	{
		3,1,0,0,2,3,
		4,5,7,7,6,4,
		4,8,5,4,9,8,
		11,3,10,10,3,2,
		12,13,4,4,6,12,
		14,7,5,14,15,7
	};

	ptWorld->atSimpleMesh[nThisEntity].m_nIndexCount = 36;
	ptWorld->atSimpleMesh[nThisEntity].m_nVertexCount = 16;

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

					   
	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.pSysMem = atCubeVertices;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.pSysMem = cubeIndices;

	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;

	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.SysMemPitch = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.SysMemSlicePitch = 0;

	ptWorld->atShaderID[nThisEntity].m_nShaderID = 3;
	return 0;
}
/*
* createPlayerBox():  creates a simple box
*
* Ins:                  the world pointer
*						the location to spawn the inactive Bullet
*
*
* Outs:
*
* Returns:              returns the index of the bullet that was made for the world index
*
* Mod. Date:              07/16/2018
* Mod. Initials:          AP
*/
/*
unsigned int createDummyPlayer(TWorld* ptWorld, XMMATRIX playerMatrix)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	//ptWorld->anComponentMask[nThisEntity] =  COMPONENT_DEBUGMESH;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID;//138
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK;
	ptWorld->atDebugMesh[nThisEntity].m_nVertexCount = 6;
	XMVECTOR vectorPos = playerMatrix.r[3];
	XMFLOAT3 playerPos;
	playerPos.x = vectorPos.m128_f32[0];
	playerPos.y = vectorPos.m128_f32[1];
	playerPos.z = vectorPos.m128_f32[2];

	static TPrimalVert atPlayerVertices[]
	{
		TPrimalVert{ XMFLOAT3(0, 0, 0),    XMFLOAT4(1,0,0,1) },//00	Green
		TPrimalVert{ XMFLOAT3(0, playerPos.y , 0), XMFLOAT4(1,0,0,1) },//01	Green
		TPrimalVert{ XMFLOAT3(0, 0, 0),    XMFLOAT4(0,1,0,1) },//00	Red
		TPrimalVert{ XMFLOAT3(playerPos.x, 0, 0), XMFLOAT4(0,1,0,1) },//01	Red
		TPrimalVert{ XMFLOAT3(0, 0, 0),    XMFLOAT4(0,0,1,1) },//00	Blue
		TPrimalVert{ XMFLOAT3(0, 0, playerPos.z), XMFLOAT4(0,0,1,1) },//01	Blue	
	};



	ptWorld->atDebugMesh[nThisEntity].m_nVertexBufferStride = sizeof(TPrimalVert);
	ptWorld->atDebugMesh[nThisEntity].m_nVertexBufferOffset = 0;

	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.ByteWidth = sizeof(TPrimalVert) * ptWorld->atDebugMesh[nThisEntity].m_nVertexCount;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.CPUAccessFlags = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.MiscFlags = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.StructureByteStride = 0;

	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.pSysMem = atPlayerVertices;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;
	ptWorld->atShaderID[nThisEntity].m_nShaderID = 2;
	return 0;
}
*/


unsigned int createPlayerBox(TWorld * ptWorld, CCollisionSystem* pcCollisionSystem)
{
	return 0;
}

unsigned int createPlayerBox(TWorld * ptWorld)

{
	unsigned int nThisEntity = createEntity(ptWorld);
	
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_NONTRIGGER;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID;
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK| COMPONENT_RIGIDBODY;
	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(-0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(-0.5f, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(0.5f, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(-0.5f, 0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(-0.5f, -0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(0.5f, -0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(-0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(-0.5f, -0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//10 Bottom B Left
		TPrimalVert{ XMFLOAT3(0.5f, -0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(-0.5f, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(-0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
		TPrimalVert{ XMFLOAT3(0.5f, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }//15 Bottm F Right
	};
	ptWorld->atSimpleMesh[nThisEntity].m_nColor = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	static short cubeIndices[]
	{
		3,1,0,0,2,3,
		4,5,7,7,6,4,
		4,8,5,4,9,8,
		11,3,10,10,3,2,
		12,13,4,4,6,12,
		14,7,5,14,15,7
	};

	ptWorld->atSimpleMesh[nThisEntity].m_nIndexCount = 36;
	ptWorld->atSimpleMesh[nThisEntity].m_nVertexCount = 16;

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


	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.pSysMem = atCubeVertices;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.pSysMem = cubeIndices;

	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;

	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.SysMemPitch = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.SysMemSlicePitch = 0;

	ptWorld->atShaderID[nThisEntity].m_nShaderID = 3;
	for (unsigned int i = 0; i < ptWorld->atSimpleMesh[nThisEntity].m_nVertexCount; ++i) {
		ptWorld->atSimpleMesh[nThisEntity].m_VertexData.push_back(atCubeVertices[i].m_d3dfPosition);
	}


	return 0;
}
/*
* CreateBullet():  This function creates a inactivebullet
*
* Ins:                  the world pointer
*						the location to spawn the inactive Bullet
*
*
* Outs:
*
* Returns:              returns the index of the bullet that was made for the world index
*
* Mod. Date:              07/16/2018
* Mod. Initials:          AP
*/
unsigned int CreateBullet(TWorld * ptWorld,XMMATRIX BulletSpawnLocation)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK |COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID;
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK | COMPONENT_RIGIDBODY;
	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(-0, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(-0, -0, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(0.5f, -0, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(-0, 0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(-0, -0, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(0.5f, -0, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(-0, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(-0, -0, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//10 Bottom B Left
		TPrimalVert{ XMFLOAT3(0.5f, -0, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(-0, -0, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(-0, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
		TPrimalVert{ XMFLOAT3(0.5f, -0, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }//15 Bottm F Right
	};
	for (unsigned int i = 0; i < 16; ++i) {
		//atCubeVertices[i].m_d3dfPosition.x *= 1;
		//atCubeVertices[i].m_d3dfPosition.y *= 1;
		//atCubeVertices[i].m_d3dfPosition.z *= 1;

	}
	static short cubeIndices[]
	{
		3,1,0,0,2,3,
		4,5,7,7,6,4,
		4,8,5,4,9,8,
		11,3,10,10,3,2,
		12,13,4,4,6,12,
		14,7,5,14,15,7
	};

	ptWorld->atSimpleMesh[nThisEntity].m_nIndexCount = 36;
	ptWorld->atSimpleMesh[nThisEntity].m_nVertexCount = 16;

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


	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.pSysMem = atCubeVertices;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.pSysMem = cubeIndices;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.SysMemPitch = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.SysMemSlicePitch = 0;

	XMVECTOR zerovector;
	zerovector.m128_f32[0] = 0;
	zerovector.m128_f32[1] = 0;
	zerovector.m128_f32[2] = 0;
	zerovector.m128_f32[3] = 0;
	ptWorld->atRigidBody[nThisEntity].gravity = zerovector;
	ptWorld->atRigidBody[nThisEntity].totalForce = zerovector;
	ptWorld->atRigidBody[nThisEntity].ground = false;


	
	

	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = BulletSpawnLocation;

	ptWorld->atShaderID[nThisEntity].m_nShaderID = 3;
	for (unsigned int i = 0; i < ptWorld->atSimpleMesh[nThisEntity].m_nVertexCount; ++i) {
		ptWorld->atSimpleMesh[nThisEntity].m_VertexData.push_back(atCubeVertices[i].m_d3dfPosition);
	}
	return nThisEntity;
}

/*
* AimingLine():  Creates a line for the player to aim with
*
* Ins:                  the world pointer
*				
*
*
* Outs:
*
* Returns:              returns the index of the bullet that was made for the world index
*
* Mod. Date:              07/16/2018
* Mod. Initials:          AP
*/

unsigned int AimingLine(TWorld * ptWorld, XMMATRIX BulletSpawnLocation, int parentWorldMatrixIndex, float xoffset, float yoffset, float zoffset)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID;
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK;
	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(0.3f, 0.4f, 5.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.4f, 5.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(0.3f, 0.3f, 5.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.3f, 5.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(0.3f, 0.4f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.4f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(0.3f, 0.3f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.3f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(0.5f, 0.4f, 5.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(0.3f, 0.4f, 5.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(0.3f, 0.3f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//10 Bottom B Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.3f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(0.3f, 0.3f, 5.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(0.3f, 0.4f, 5.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.4f, 5.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
		TPrimalVert{ XMFLOAT3(0.5f, 0.3f, 5.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }//15 Bottm F Right
	};
	for (unsigned int i = 0; i < 16; ++i) {
		//atCubeVertices[i].m_d3dfPosition.x *= 1;
		//atCubeVertices[i].m_d3dfPosition.y *= 1;
		//atCubeVertices[i].m_d3dfPosition.z *= 1;

	}
	static short cubeIndices[]
	{
		3,1,0,0,2,3,
		4,5,7,7,6,4,
		4,8,5,4,9,8,
		11,3,10,10,3,2,
		12,13,4,4,6,12,
		14,7,5,14,15,7
	};

	ptWorld->atSimpleMesh[nThisEntity].m_nIndexCount = 36;
	ptWorld->atSimpleMesh[nThisEntity].m_nVertexCount = 16;

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


	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.pSysMem = atCubeVertices;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.pSysMem = cubeIndices;

	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;

	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.SysMemPitch = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.SysMemSlicePitch = 0;
	

	
	// the 90 is for fov if we want to implament field of view
	/*BulletSpawnLocation.r[0].m128_f32[0] = 0.5;
	BulletSpawnLocation.r[1].m128_f32[1] = 0.5;
	BulletSpawnLocation.r[2].m128_f32[2] = 0.5;*/
	//	BulletSpawnLocation.r[3].m128_f32[0] += 0.5;
	
	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = BulletSpawnLocation;
	ptWorld->atParentWorldMatrix[nThisEntity] = parentWorldMatrixIndex;
	ptWorld->atOffSetMatrix[nThisEntity] = XMMatrixTranslation(xoffset, yoffset, zoffset);


	ptWorld->atShaderID[nThisEntity].m_nShaderID = 3;

	return nThisEntity;
}

unsigned int CreateGround(TWorld * ptWorld)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	ptWorld->atRigidBody[nThisEntity].ground = true;
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_STATIC| COMPONENT_NONTRIGGER;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID;
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK;
	ptWorld->atSimpleMesh[nThisEntity].m_nColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(-20, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(4, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(-20, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(4, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(-20, 0.5f, -20),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(4, 0.5f, -20),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(-20, -0.5f, -20),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(4, -0.5f, -20),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(4, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(-20, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(-20, -0.5f, -20),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//10 Bottom B Left
		TPrimalVert{ XMFLOAT3(4, -0.5f, -20),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(-20, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(-20, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(4, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
		TPrimalVert{ XMFLOAT3(4, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }//15 Bottm F Right
	};

	static short cubeIndices[]
	{
		3,1,0,0,2,3,
		4,5,7,7,6,4,
		4,8,5,4,9,8,
		11,3,10,10,3,2,
		12,13,4,4,6,12,
		14,7,5,14,15,7
	};

	ptWorld->atSimpleMesh[nThisEntity].m_nIndexCount = 36;
	ptWorld->atSimpleMesh[nThisEntity].m_nVertexCount = 16;

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


	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.pSysMem = atCubeVertices;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.pSysMem = cubeIndices;

	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;

	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.SysMemPitch = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.SysMemSlicePitch = 0;

	ptWorld->atShaderID[nThisEntity].m_nShaderID = 3;
	for (unsigned int i = 0; i < ptWorld->atSimpleMesh[nThisEntity].m_nVertexCount; ++i) {
		ptWorld->atSimpleMesh[nThisEntity].m_VertexData.push_back(atCubeVertices[i].m_d3dfPosition);
	}


	return 0;
}

unsigned int CreateWall(TWorld * ptWorld)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	ptWorld->atRigidBody[nThisEntity].wall = true;
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_STATIC| COMPONENT_NONTRIGGER;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID;
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK;
	ptWorld->atSimpleMesh[nThisEntity].m_nColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(2, 10, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(4, 10, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(2, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(4, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(2, 10, -20),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(4, 10, -20),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(2, -0.5f, -20),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(4, -0.5f, -20),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(4, 10, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(2, 10, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(2, -0.5f, -20),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//10 Bottom B Left
		TPrimalVert{ XMFLOAT3(4, -0.5f, -20),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(2, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(2, 10, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(4, 10, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
		TPrimalVert{ XMFLOAT3(4, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }//15 Bottm F Right
	};

	static short cubeIndices[]
	{
		3,1,0,0,2,3,
		4,5,7,7,6,4,
		4,8,5,4,9,8,
		11,3,10,10,3,2,
		12,13,4,4,6,12,
		14,7,5,14,15,7
	};

	ptWorld->atSimpleMesh[nThisEntity].m_nIndexCount = 36;
	ptWorld->atSimpleMesh[nThisEntity].m_nVertexCount = 16;

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


	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.pSysMem = atCubeVertices;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.pSysMem = cubeIndices;

	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;

	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.SysMemPitch = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.SysMemSlicePitch = 0;

	ptWorld->atShaderID[nThisEntity].m_nShaderID = 3;
	for (unsigned int i = 0; i < ptWorld->atSimpleMesh[nThisEntity].m_nVertexCount; ++i) {
		ptWorld->atSimpleMesh[nThisEntity].m_VertexData.push_back(atCubeVertices[i].m_d3dfPosition);
	}


	return 0;
}

unsigned int CreateCelling(TWorld * ptWorld)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	ptWorld->atRigidBody[nThisEntity].wall = true;
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_STATIC| COMPONENT_NONTRIGGER;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID;
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK;
	ptWorld->atSimpleMesh[nThisEntity].m_nColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(-20, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(4, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(-20, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(4, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(-20, 0.5f, -20),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(4, 0.5f, -20),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(-20, -0.5f, -20),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(4, -0.5f, -20),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(4, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(-20, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(-20, -0.5f, -20),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//10 Bottom B Left
		TPrimalVert{ XMFLOAT3(4, -0.5f, -20),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(-20, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(-20, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(4, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
		TPrimalVert{ XMFLOAT3(4, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }//15 Bottm F Right
	};

	static short cubeIndices[]
	{
		3,1,0,0,2,3,
		4,5,7,7,6,4,
		4,8,5,4,9,8,
		11,3,10,10,3,2,
		12,13,4,4,6,12,
		14,7,5,14,15,7
	};

	ptWorld->atSimpleMesh[nThisEntity].m_nIndexCount = 36;
	ptWorld->atSimpleMesh[nThisEntity].m_nVertexCount = 16;

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


	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.pSysMem = atCubeVertices;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.pSysMem = cubeIndices;

	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;

	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.SysMemPitch = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.SysMemSlicePitch = 0;

	ptWorld->atShaderID[nThisEntity].m_nShaderID = 3;
	for (unsigned int i = 0; i < ptWorld->atSimpleMesh[nThisEntity].m_nVertexCount; ++i) {
		ptWorld->atSimpleMesh[nThisEntity].m_VertexData.push_back(atCubeVertices[i].m_d3dfPosition);
	}


	return 0;
}

unsigned int CreateGun(TWorld * ptWorld, XMMATRIX BulletSpawnLocation,int parentWorldMatrixIndex,float xoffset,float yoffset,float zoffset)
{
	unsigned int nThisEntity = createEntity(ptWorld);



	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK ;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID;
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK;
	ptWorld->atProjectiles[nThisEntity].m_tnProjectileMask = COMPONENT_PROJECTILESMASK|COMPONENT_CLIP;
	ptWorld->atClip[nThisEntity].nSizeofClipl = 3;
	ptWorld->atClip[nThisEntity].FValueOfCoolDown = 50;
	ptWorld->atClip[nThisEntity].fShootingCoolDown = 0;
	for (int i = 0; i < ptWorld->atClip[nThisEntity].nSizeofClipl; ++i) {
		ptWorld->atClip[nThisEntity].nBulletsAvailables.push_back(false);
	}
	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(-0, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(-0, -0, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(0.5f, -0, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(-0, 0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(-0, -0, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(0.5f, -0, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(-0, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(-0, -0, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//10 Bottom B Left
		TPrimalVert{ XMFLOAT3(0.5f, -0, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(-0, -0, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(-0, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
		TPrimalVert{ XMFLOAT3(0.5f, -0, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }//15 Bottm F Right
	};
	ptWorld->atSimpleMesh[nThisEntity].m_nColor = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	static short cubeIndices[]
	{
		3,1,0,0,2,3,
		4,5,7,7,6,4,
		4,8,5,4,9,8,
		11,3,10,10,3,2,
		12,13,4,4,6,12,
		14,7,5,14,15,7
	};

	ptWorld->atSimpleMesh[nThisEntity].m_nIndexCount = 36;
	ptWorld->atSimpleMesh[nThisEntity].m_nVertexCount = 16;

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


	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.pSysMem = atCubeVertices;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.pSysMem = cubeIndices;

	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;

	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.SysMemPitch = 0;
	ptWorld->atSimpleMesh[nThisEntity].m_d3dIndexData.SysMemSlicePitch = 0;

	ptWorld->atShaderID[nThisEntity].m_nShaderID = 3;


	
	

	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = BulletSpawnLocation;
	ptWorld->atParentWorldMatrix[nThisEntity] = parentWorldMatrixIndex;
//	ptWorld->atOffSetMatrix[nThisEntity]= XMMatrixTranslation(-1, 0, 10.5);
	ptWorld->atOffSetMatrix[nThisEntity] = XMMatrixTranslation(xoffset,yoffset,zoffset);


	
	return nThisEntity;
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
		TPrimalVert{ XMFLOAT3(-.5f, 0,  .5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//00	Green
		TPrimalVert{ XMFLOAT3(-.5f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//01	Green
		TPrimalVert{ XMFLOAT3(-.4f, 0,  .5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//02	Red
		TPrimalVert{ XMFLOAT3(-.4f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//03	Red
		TPrimalVert{ XMFLOAT3(-.3f, 0,  .5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//04	Blue
		TPrimalVert{ XMFLOAT3(-.3f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//05	Blue	
		TPrimalVert{ XMFLOAT3(-.2f, 0,  .5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//06	Blue
		TPrimalVert{ XMFLOAT3(-.2f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//07	Red
		TPrimalVert{ XMFLOAT3(-.1f, 0,  .5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//08	Red
		TPrimalVert{ XMFLOAT3(-.1f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//09	Green
		TPrimalVert{ XMFLOAT3( .0f, 0,  .5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//10	Green							 		   
		TPrimalVert{ XMFLOAT3( .0f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//11	Green
		TPrimalVert{ XMFLOAT3( .1f, 0,  .5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//12	Green	
		TPrimalVert{ XMFLOAT3( .1f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//13	Green
		TPrimalVert{ XMFLOAT3( .2f, 0,  .5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//14	Green	
		TPrimalVert{ XMFLOAT3( .2f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//15	Green
		TPrimalVert{ XMFLOAT3( .3f, 0,  .5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//16	Green								  
		TPrimalVert{ XMFLOAT3( .3f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//17	Green
		TPrimalVert{ XMFLOAT3( .4f, 0,  .5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//18	Green	
		TPrimalVert{ XMFLOAT3( .4f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//19	Green
		TPrimalVert{ XMFLOAT3( .5f, 0,  .5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//20	Green	
		TPrimalVert{ XMFLOAT3( .5f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//21	Green
		
		TPrimalVert{ XMFLOAT3( .5f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//00	Green
		TPrimalVert{ XMFLOAT3(-.5f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//01	Green
		TPrimalVert{ XMFLOAT3( .5f, 0, -.4f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//02	Red
		TPrimalVert{ XMFLOAT3(-.5f, 0, -.4f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//03	Red
		TPrimalVert{ XMFLOAT3( .5f, 0, -.3f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//04	Blue
		TPrimalVert{ XMFLOAT3(-.5f, 0, -.3f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//05	Blue	
		TPrimalVert{ XMFLOAT3( .5f, 0, -.2f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//06	Blue
		TPrimalVert{ XMFLOAT3(-.5f, 0, -.2f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//07	Red
		TPrimalVert{ XMFLOAT3( .5f, 0, -.1f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//08	Red
		TPrimalVert{ XMFLOAT3(-.5f, 0, -.1f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//09	Green
		TPrimalVert{ XMFLOAT3( .5f, 0,  .0f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//10	Green							 		   
		TPrimalVert{ XMFLOAT3(-.5f, 0,  .0f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//11	Green
		TPrimalVert{ XMFLOAT3( .5f, 0,  .1f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//12	Green	
		TPrimalVert{ XMFLOAT3(-.5f, 0,  .1f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//13	Green
		TPrimalVert{ XMFLOAT3( .5f, 0,  .2f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//14	Green	
		TPrimalVert{ XMFLOAT3(-.5f, 0,  .2f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//15	Green
		TPrimalVert{ XMFLOAT3( .5f, 0,  .3f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//16	Green								  
		TPrimalVert{ XMFLOAT3(-.5f, 0,  .3f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//17	Green
		TPrimalVert{ XMFLOAT3( .5f, 0,  .4f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//18	Green	
		TPrimalVert{ XMFLOAT3(-.5f, 0,  .4f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//19	Green
		TPrimalVert{ XMFLOAT3( .5f, 0,  .5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//20	Green	
		TPrimalVert{ XMFLOAT3(-.5f, 0,  .5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//21	Green
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
	for (unsigned int i = 0; i < ptWorld->atDebugMesh[nThisEntity].m_nVertexCount; ++i) {
		ptWorld->atDebugMesh[nThisEntity].m_VertexData.push_back(atDebugGridVertices[i].m_d3dfPosition);
	}

	ptWorld->atShaderID[nThisEntity].m_nShaderID = 2;//Shader that supports TPrimalVert

	return 0;
}

unsigned int createMesh(TWorld * ptWorld, ID3D11Device * m_pd3dDevice, TMeshImport tMesh, TMaterialImport tMaterial)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	
	#pragma region CreateTexturesFromFile

	wchar_t fnPBR[9][260];
	wchar_t fnTRAD[5][260];
	size_t result = 0;
	mbstate_t d;
	TMaterialImport tTempMaterial = tMaterial;
	
	for (unsigned int i = 0; i < 9; i++)
	{

		if (i < 5 && tTempMaterial.m_tFileNames[i])
		{
			tTempMaterial.m_tFileNames[i] = &tMaterial.m_tFileNames[i][1];
			mbsrtowcs_s(&result, fnTRAD[i], 260, (const char **)(&tTempMaterial.m_tFileNames[i]), tTempMaterial.m_tFileNameSizes[i], &d);
		}
		if (tTempMaterial.m_tPBRFileNames[i])
		{
			tTempMaterial.m_tPBRFileNames[i] = &tMaterial.m_tPBRFileNames[i][1];
			mbsrtowcs_s(&result, fnPBR[i], 260,	(const char **)(&tTempMaterial.m_tPBRFileNames[i]), tTempMaterial.m_tPBRFileNameSizes[i], &d);
		}
	}
	
	ID3D11Resource * ambientTexture;
	ID3D11Resource * diffuseTexture;										
	ID3D11Resource * specularTexture;										
	ID3D11Resource * emissiveTexture;
	ID3D11Resource * normalTexture;
	ID3D11Resource * bumpTexture;

	ID3D11Resource * d3dColorMap;
	ID3D11Resource * d3dNormalMap;
	ID3D11Resource * d3dEmissiveMap;
	ID3D11Resource * d3dMetallicMap;
	ID3D11Resource * d3dRoughnessMap;
	ID3D11Resource * d3dAmbientOcclusionMap;
	ID3D11Resource * d3dGlobalDiffuseCubeMap;
	ID3D11Resource * d3dGlobalSpecularCubeMap;
	ID3D11Resource * d3dIBLCubeMap;

	ID3D11ShaderResourceView * srv;

	for (unsigned int i = 0; i < 9; i++)
	{
		if (tMaterial.m_tPBRFileNames[i])
		{
			if (tMaterial.m_tPBRFileNames[i][0] == 'c')
			{
				result = CreateWICTextureFromFile(m_pd3dDevice, fnPBR[i], &d3dColorMap, &srv, NULL);
			}
			if (tMaterial.m_tPBRFileNames[i][0] == 'n')
			{
				result = CreateWICTextureFromFile(m_pd3dDevice, fnPBR[i], &d3dNormalMap, nullptr, NULL);
			}
			if (tMaterial.m_tPBRFileNames[i][0] == 'e')
			{
				result = CreateWICTextureFromFile(m_pd3dDevice, fnPBR[i], &d3dEmissiveMap, nullptr, NULL);
			}
			if (tMaterial.m_tPBRFileNames[i][0] == 'm')
			{
				result = CreateWICTextureFromFile(m_pd3dDevice, fnPBR[i], &d3dMetallicMap, nullptr, NULL);
			}
			if (tMaterial.m_tPBRFileNames[i][0] == 'r')
			{
				result = CreateWICTextureFromFile(m_pd3dDevice, fnPBR[i], &d3dRoughnessMap, nullptr, NULL);
			}
			if (tMaterial.m_tPBRFileNames[i][0] == 'a')
			{
				result = CreateWICTextureFromFile(m_pd3dDevice, fnPBR[i], &d3dAmbientOcclusionMap, nullptr, NULL);
			}
			if (tMaterial.m_tPBRFileNames[i][0] == 'd')
			{
				result = CreateDDSTextureFromFile(m_pd3dDevice, fnPBR[i], &d3dGlobalDiffuseCubeMap, nullptr, NULL);
			}
			if (tMaterial.m_tPBRFileNames[i][0] == 's')
			{
				result = CreateDDSTextureFromFile(m_pd3dDevice, fnPBR[i], &d3dGlobalSpecularCubeMap, nullptr, NULL);
			}
			if (tMaterial.m_tPBRFileNames[i][0] == 'l')
			{
				result = CreateDDSTextureFromFile(m_pd3dDevice, fnPBR[i], &d3dIBLCubeMap, nullptr, NULL);
			}
		}

		if (tMaterial.m_tFileNames[i] && i < 5)
		{
			if (tMaterial.m_tFileNames[i][0] == 'a')
			{
				result = CreateWICTextureFromFile(m_pd3dDevice, fnTRAD[i], &ambientTexture, nullptr, NULL);
			}
			if (tMaterial.m_tFileNames[i][0] == 'b')
			{
				result = CreateWICTextureFromFile(m_pd3dDevice, fnTRAD[i], &bumpTexture, nullptr, NULL);
			}
			if (tMaterial.m_tFileNames[i][0] == 'd')
			{
				result = CreateWICTextureFromFile(m_pd3dDevice, fnTRAD[i], &diffuseTexture, &srv, NULL);
			}
			if (tMaterial.m_tFileNames[i][0] == 'e')
			{
				result = CreateWICTextureFromFile(m_pd3dDevice, fnTRAD[i], &emissiveTexture, nullptr, NULL);
			}
			if (tMaterial.m_tFileNames[i][0] == 'n')
			{
				result = CreateWICTextureFromFile(m_pd3dDevice, fnTRAD[i], &normalTexture, nullptr, NULL);
			}
			if (tMaterial.m_tFileNames[i][0] == 's')
			{
				result = CreateWICTextureFromFile(m_pd3dDevice, fnTRAD[i], &specularTexture, nullptr, NULL);
			}

		}
	}
#pragma endregion

	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID;
	switch (tMaterial.lambert)
	{
		case 0:
		{
			ptWorld->atShaderID[nThisEntity].m_nShaderID = 4;
			break;//Phong
		}
		case 1:
		{
			ptWorld->atShaderID[nThisEntity].m_nShaderID = 5;
			break;//Lambert
		}
		case 2:
		{
			ptWorld->atShaderID[nThisEntity].m_nShaderID = 6;
			break;//PBR
		}
		default:
			break;
	}
	ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = srv;
	ptWorld->atMesh[nThisEntity].m_nVertexCount = tMesh.nPolygonVertexCount;
	ptWorld->atMesh[nThisEntity].m_nVertexBufferStride = sizeof(TMeshFormat);
	ptWorld->atMesh[nThisEntity].m_nVertexBufferOffset = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.ByteWidth = sizeof(TMeshFormat) * ptWorld->atMesh[nThisEntity].m_nVertexCount;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.CPUAccessFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.MiscFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.StructureByteStride = 0;

	ptWorld->atMesh[nThisEntity].m_d3dVertexData.pSysMem = tMesh.meshArrays;
	ptWorld->atMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;

	ptWorld->atMesh[nThisEntity].m_nIndexCount = tMesh.nPolyCount;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.ByteWidth = sizeof(int) * ptWorld->atMesh[nThisEntity].m_nIndexCount;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.CPUAccessFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.MiscFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.StructureByteStride = 0;

	ptWorld->atMesh[nThisEntity].m_d3dIndexData.pSysMem = tMesh.indexBuffer;
	ptWorld->atMesh[nThisEntity].m_d3dIndexData.SysMemPitch = 0;
	ptWorld->atMesh[nThisEntity].m_d3dIndexData.SysMemSlicePitch = 0;
	return 0;
}

