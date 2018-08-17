#include "stdafx.h"
#include "Entity.h"

unsigned int SpawnLevelChanger(TWorld * ptWorld, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_STATIC | COMPONENT_TRIGGER;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID;
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK;
	ptWorld->atAABB[nThisEntity].m_SceneChange = 1;
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
	for (int i = 0; i < ptWorld->atSimpleMesh[nThisEntity].m_nVertexCount; ++i) {
		ptWorld->atSimpleMesh[nThisEntity].m_VertexData.push_back(atCubeVertices[i].m_d3dfPosition);
	}

	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = SpawnPosition;
	return nThisEntity;
}

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
	ptWorld->atInputMask[nThisEntity].m_tnInputMask = COMPONENT_INPUTMASK;
	XMVECTOR zeroVector;
	zeroVector.m128_f32[0] = 0;
	zeroVector.m128_f32[1] = 0;
	zeroVector.m128_f32[2] = 0;
	zeroVector.m128_f32[3] = 0;
	ptWorld->atParentWorldMatrix[nThisEntity] = -1;
	ptWorld->atRigidBody[nThisEntity].totalForce = zeroVector;
	ptWorld->atRigidBody[nThisEntity].velocity = zeroVector;
	ptWorld->atRigidBody[nThisEntity].gravity = zeroVector;
	ptWorld->atRigidBody[nThisEntity].maxVelocity = zeroVector;

	ptWorld->atSimpleMesh[nThisEntity].m_VertexData.clear();
	ptWorld->atAABB[nThisEntity].m_SceneChange = -30;
	ptWorld->atAABB[nThisEntity].m_MaterialType = MATERIAL_METAL;
	ptWorld->atClip[nThisEntity].fAliveTime.clear();
	ptWorld->atClip[nThisEntity].nBulletsAvailables.clear();
	ptWorld->atClip[nThisEntity].nBulletsFired.clear();
	ptWorld->atClip[nThisEntity].tryToShoot = false;
	ptWorld->atClip[nThisEntity].tryToReload  = false;
	ptWorld->atClip[nThisEntity].maderay = false;





	


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
		3,1,0,
		0,2,3,

		4,5,7,
		7,6,4,
		
		4,8,5,
		4,9,8,
		
		11,3,10,
		10,3,2,
		
		12,13,4,
		4,6,12,
		
		14,7,5,
		14,15,7
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

unsigned int CreateClayTon(TWorld * ptWorld)

{
	unsigned int nThisEntity = createEntity(ptWorld);
	
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_NONTRIGGER;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID;
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK| COMPONENT_RIGIDBODY;
	ptWorld->atInputMask[nThisEntity].m_tnInputMask = COMPONENT_INPUTMASK | COMPONENT_CLAYTON;
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
	XMVECTOR playerGravity;
	playerGravity.m128_f32[1] = -0.000001;
	playerGravity.m128_f32[0] = 0;
	playerGravity.m128_f32[2] = 0;
	playerGravity.m128_f32[3] = 0;
	ptWorld->atRigidBody[nThisEntity].gravity = playerGravity;

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


	return nThisEntity;
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
unsigned int CreateBullet(TWorld * ptWorld,XMMATRIX BulletSpawnLocation,int MaterialID)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	switch (MaterialID)
	{
	case 0 :
		ptWorld->atProjectiles[nThisEntity].m_tnProjectileMask = COMPONENT_PROJECTILESMASK | COMPONENT_METAL;
		break;
	case 1:	
		ptWorld->atProjectiles[nThisEntity].m_tnProjectileMask = COMPONENT_METAL | COMPONENT_WOOD;
		break;
	default:
		break;
	}
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
	ptWorld->atRigidBody[nThisEntity].wall = false;
	


	
	
	

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

unsigned int CreateGround(TWorld * ptWorld, XMMATRIX SpawnPosition)
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
	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = SpawnPosition;

	return 0;
}

unsigned int CreateWall(TWorld * ptWorld, XMMATRIX SpawnPosition)
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
	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = SpawnPosition;

	return 0;
}

unsigned int CreateCelling(TWorld * ptWorld, XMMATRIX SpawnPosition)
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

	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = SpawnPosition;
	return 0;
}

unsigned int CreateGun(TWorld * ptWorld, XMMATRIX BulletSpawnLocation,int parentWorldMatrixIndex,float xoffset,float yoffset,float zoffset,int clipSize,float shootingCOooldown)
{
	unsigned int nThisEntity = createEntity(ptWorld);



ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK ;
ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID;
ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK;
ptWorld->atProjectiles[nThisEntity].m_tnProjectileMask = COMPONENT_PROJECTILESMASK| COMPONENT_CLIP;
ptWorld->atClip[nThisEntity].nSizeofClipl = clipSize;
ptWorld->atClip[nThisEntity].FValueOfCoolDown = shootingCOooldown;
ptWorld->atClip[nThisEntity].fShootingCoolDown = 0;
ptWorld->atClip[nThisEntity].GunMode = true;
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

unsigned int CreateSimpleGunAi(TWorld * ptWorld, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_NONTRIGGER;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID;
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK | COMPONENT_FOLLOW;
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
	XMVECTOR AIGravity;
	AIGravity.m128_f32[1] = -0.000001;
	AIGravity.m128_f32[0] = 0;
	AIGravity.m128_f32[2] = 0;
	AIGravity.m128_f32[3] = 0;
	ptWorld->atRigidBody[nThisEntity].gravity = AIGravity;

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
	for (int i = 0; i < ptWorld->atSimpleMesh[nThisEntity].m_nVertexCount; ++i) {
		ptWorld->atSimpleMesh[nThisEntity].m_VertexData.push_back(atCubeVertices[i].m_d3dfPosition);
	}

	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = SpawnPosition;
	return nThisEntity;
}

unsigned int CreateSimpleSearchAi(TWorld * ptWorld, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_NONTRIGGER;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID;
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK | COMPONENT_SEARCH;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK | COMPONENT_RIGIDBODY;
	
	//TPrimalVert{ XMFLOAT3(-0, 0.5f, 0),
	//	TPrimalVert{ XMFLOAT3(0, 0.5f, 10),
	XMVECTOR start;
	start.m128_f32[0] = 0;
	start.m128_f32[1] = 0.5;
	start.m128_f32[2] = 0;
	XMVECTOR end;
	end.m128_f32[0] = 0;
	end.m128_f32[1] = 0.5;
	end.m128_f32[2] = 10;


	ptWorld->atAIVision[nThisEntity].start = start;
	ptWorld->atAIVision[nThisEntity].end = end;

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
	XMVECTOR AIGravity;
	AIGravity.m128_f32[1] = -0.000001;
	AIGravity.m128_f32[0] = 0;
	AIGravity.m128_f32[2] = 0;
	AIGravity.m128_f32[3] = 0;
	ptWorld->atRigidBody[nThisEntity].gravity = AIGravity;

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
	for (int i = 0; i < ptWorld->atSimpleMesh[nThisEntity].m_nVertexCount; ++i) {
		ptWorld->atSimpleMesh[nThisEntity].m_VertexData.push_back(atCubeVertices[i].m_d3dfPosition);
	}

	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = SpawnPosition;
	return nThisEntity;
}

unsigned int CreateRayBullet(TWorld * ptWorld, XMMATRIX bulletSpawnLocation, float zDistance, int parentWorldMatrixIndex, float xoffset, float yoffset, float zoffset)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID;
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK;
	ptWorld->atProjectiles[nThisEntity].m_tnProjectileMask = COMPONENT_PROJECTILESMASK |COMPONENT_RAYGUN;
	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(-0, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(0, 0.5f, 10),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right

	};
	for (int i = 0; i < 16; ++i) {
		//atCubeVertices[i].m_d3dfPosition.x *= 1;
		//atCubeVertices[i].m_d3dfPosition.y *= 1;
		//atCubeVertices[i].m_d3dfPosition.z *= 1;

	}



	ptWorld->atDebugMesh[nThisEntity].m_nVertexCount = 2;

	ptWorld->atDebugMesh[nThisEntity].m_nVertexBufferStride = sizeof(TPrimalVert);
	ptWorld->atDebugMesh[nThisEntity].m_nVertexBufferOffset = 0;

	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.ByteWidth = sizeof(TPrimalVert) * ptWorld->atDebugMesh[nThisEntity].m_nVertexCount;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.MiscFlags = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.StructureByteStride = 0;




	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.pSysMem = atCubeVertices;


	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;



	XMVECTOR zerovector;
	zerovector.m128_f32[0] = 0;
	zerovector.m128_f32[1] = 0;
	zerovector.m128_f32[2] = 0;
	zerovector.m128_f32[3] = 0;
	ptWorld->atRigidBody[nThisEntity].gravity = zerovector;
	ptWorld->atRigidBody[nThisEntity].totalForce = zerovector;
	ptWorld->atRigidBody[nThisEntity].ground = false;





	

	ptWorld->atShaderID[nThisEntity].m_nShaderID = 2;
	for (int i = 0; i < ptWorld->atDebugMesh[nThisEntity].m_nVertexCount; ++i) {
		XMVECTOR tempt;
		tempt.m128_f32[0] = atCubeVertices[i].m_d3dfPosition.x;
		tempt.m128_f32[1] = atCubeVertices[i].m_d3dfPosition.y;
		tempt.m128_f32[2] = atCubeVertices[i].m_d3dfPosition.z;

		ptWorld->atDebugMesh[nThisEntity].m_VertexData.push_back(tempt);
	}
	ptWorld->atParentWorldMatrix[nThisEntity] = parentWorldMatrixIndex;
	//	ptWorld->atOffSetMatrix[nThisEntity]= XMMatrixTranslation(-1, 0, 10.5);
	ptWorld->atOffSetMatrix[nThisEntity] = XMMatrixTranslation(xoffset, yoffset, zoffset);
	return nThisEntity;
}

unsigned int CreateAIVision(TWorld * ptWorld, XMMATRIX VisionSpawnLocation, float zDistance, int parentWorldMatrixIndex, float xoffset, float yoffset, float zoffset)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID;
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK;
	ptWorld->atProjectiles[nThisEntity].m_tnProjectileMask = COMPONENT_PROJECTILESMASK | COMPONENT_RAYGUN;
	
	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(-0, 0.5f, -3),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(0, 0.5f, 10),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },//1 Top F Right

	};
	for (int i = 0; i < 16; ++i) {
		//atCubeVertices[i].m_d3dfPosition.x *= 1;
		//atCubeVertices[i].m_d3dfPosition.y *= 1;
		//atCubeVertices[i].m_d3dfPosition.z *= 1;

	}



	ptWorld->atDebugMesh[nThisEntity].m_nVertexCount = 2;

	ptWorld->atDebugMesh[nThisEntity].m_nVertexBufferStride = sizeof(TPrimalVert);
	ptWorld->atDebugMesh[nThisEntity].m_nVertexBufferOffset = 0;

	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.ByteWidth = sizeof(TPrimalVert) * ptWorld->atDebugMesh[nThisEntity].m_nVertexCount;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.MiscFlags = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.StructureByteStride = 0;




	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.pSysMem = atCubeVertices;


	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;



	XMVECTOR zerovector;
	zerovector.m128_f32[0] = 0;
	zerovector.m128_f32[1] = 0;
	zerovector.m128_f32[2] = 0;
	zerovector.m128_f32[3] = 0;
	ptWorld->atRigidBody[nThisEntity].gravity = zerovector;
	ptWorld->atRigidBody[nThisEntity].totalForce = zerovector;
	ptWorld->atRigidBody[nThisEntity].ground = false;







	ptWorld->atShaderID[nThisEntity].m_nShaderID = 2;
	for (int i = 0; i < ptWorld->atDebugMesh[nThisEntity].m_nVertexCount; ++i) {
		XMVECTOR tempt;
		tempt.m128_f32[0] = atCubeVertices[i].m_d3dfPosition.x;
		tempt.m128_f32[1] = atCubeVertices[i].m_d3dfPosition.y;
		tempt.m128_f32[2] = atCubeVertices[i].m_d3dfPosition.z;
		if (i == 0) {
			ptWorld->atAIVision[nThisEntity].start = tempt;
		}
		else {
			ptWorld->atAIVision[nThisEntity].end = tempt;

		}
		ptWorld->atDebugMesh[nThisEntity].m_VertexData.push_back(tempt);
	}
	ptWorld->atParentWorldMatrix[nThisEntity] = parentWorldMatrixIndex;
	//	ptWorld->atOffSetMatrix[nThisEntity]= XMMatrixTranslation(-1, 0, 10.5);
	ptWorld->atOffSetMatrix[nThisEntity] = XMMatrixTranslation(xoffset, yoffset, zoffset);
	return nThisEntity;
}

unsigned int CreateTemptUIBox(TWorld * ptWorld, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID;
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK;
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
	XMVECTOR AIGravity;
	AIGravity.m128_f32[1] = -0.000001;
	AIGravity.m128_f32[0] = 0;
	AIGravity.m128_f32[2] = 0;
	AIGravity.m128_f32[3] = 0;
	ptWorld->atRigidBody[nThisEntity].gravity = AIGravity;

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
	for (int i = 0; i < ptWorld->atSimpleMesh[nThisEntity].m_nVertexCount; ++i) {
		ptWorld->atSimpleMesh[nThisEntity].m_VertexData.push_back(atCubeVertices[i].m_d3dfPosition);
	}

	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = SpawnPosition;
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
	//for (int i = 0; i < ptWorld->atDebugMesh[nThisEntity].m_nVertexCount; ++i) 
	//{
	//	ptWorld->atDebugMesh[nThisEntity].m_VertexData.push_back(atDebugGridVertices[i].m_d3dfPosition);
	//}

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

	ID3D11Resource * defaultDiffuseTexture;

	
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
	if (tMaterial.m_tPBRFileNames[0] == NULL && tMaterial.m_tFileNames[0] == NULL)
	{
		result = CreateWICTextureFromFile(m_pd3dDevice, L"TestScene_V1.fbm\\Wood01_col.jpg", &diffuseTexture, &srv, NULL);
	}
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID;
	//ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_NONTRIGGER;
	//ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	//ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	//ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK | COMPONENT_RIGIDBODY;
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
	TPrimitiveMesh *pMesh = new TPrimitiveMesh[tMesh.nUniqueVertexCount];
	for (int i = 0; i < tMesh.nUniqueVertexCount; i++)
	{
		TPrimitiveMesh tmp;
		for (int j = 0; j < 4; j++)
		{
			//if (j == 1)
			//{
			//	tmp.pos[j] = -tMesh.meshArrays[i].pos[j];
			//}
			//else
				tmp.pos[j] = tMesh.meshArrays[i].pos[j];
			if (j < 2)
			{
				tmp.uv[j] = tMesh.meshArrays[i].uv[j];
			}
		}
		pMesh[i] = tmp;
		ptWorld->atMesh[nThisEntity].m_VertexData.push_back(XMFLOAT3(tmp.pos[0], tmp.pos[1], tmp.pos[2]));

	}

	ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = srv;
	ptWorld->atMesh[nThisEntity].m_nVertexCount = tMesh.nUniqueVertexCount;
	ptWorld->atMesh[nThisEntity].m_nVertexBufferStride = sizeof(TPrimitiveMesh);
	ptWorld->atMesh[nThisEntity].m_nVertexBufferOffset = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.ByteWidth = sizeof(TPrimitiveMesh) * ptWorld->atMesh[nThisEntity].m_nVertexCount;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.CPUAccessFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.MiscFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.StructureByteStride = 0;

	ptWorld->atMesh[nThisEntity].m_d3dVertexData.pSysMem = pMesh;
	ptWorld->atMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;

	ptWorld->atMesh[nThisEntity].m_nIndexCount = tMesh.nPolygonVertexCount;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.ByteWidth = sizeof(unsigned int) * ptWorld->atMesh[nThisEntity].m_nIndexCount;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.CPUAccessFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.MiscFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.StructureByteStride = 0;

	ptWorld->atMesh[nThisEntity].m_d3dIndexData.pSysMem = tMesh.indexBuffer;
	ptWorld->atMesh[nThisEntity].m_d3dIndexData.SysMemPitch = 0;
	ptWorld->atMesh[nThisEntity].m_d3dIndexData.SysMemSlicePitch = 0;

	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = XMMatrixMultiply(ptWorld->atWorldMatrix[nThisEntity].worldMatrix, XMMatrixTranslation(tMesh.worldTranslation[0], tMesh.worldTranslation[1], tMesh.worldTranslation[2]));
	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = XMMatrixMultiply(ptWorld->atWorldMatrix[nThisEntity].worldMatrix, XMMatrixRotationRollPitchYaw(tMesh.worldRotation[0], tMesh.worldRotation[1], tMesh.worldRotation[2]));
	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = XMMatrixMultiply(ptWorld->atWorldMatrix[nThisEntity].worldMatrix, XMMatrixScaling(tMesh.worldScaling[0], tMesh.worldScaling[1], tMesh.worldScaling[2]));
	return nThisEntity;
}

unsigned int createClayton(TWorld * ptWorld, ID3D11Device * m_pd3dDevice, TMeshImport tMesh, TMaterialImport tMaterial)
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
			mbsrtowcs_s(&result, fnPBR[i], 260, (const char **)(&tTempMaterial.m_tPBRFileNames[i]), tTempMaterial.m_tPBRFileNameSizes[i], &d);
		}
	}

	ID3D11Resource * defaultDiffuseTexture;


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
	if (tMaterial.m_tPBRFileNames[0] == NULL && tMaterial.m_tFileNames[0] == NULL)
	{
		result = CreateWICTextureFromFile(m_pd3dDevice, L"TestScene_V1.fbm\\Wood01_col.jpg", &diffuseTexture, &srv, NULL);
	}
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID;
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_NONTRIGGER;
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK | COMPONENT_RIGIDBODY;
	ptWorld->atInputMask[nThisEntity].m_tnInputMask = COMPONENT_INPUTMASK | COMPONENT_CLAYTON;

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
	TPrimitiveMesh *pMesh = new TPrimitiveMesh[tMesh.nUniqueVertexCount];
	for (int i = 0; i < tMesh.nUniqueVertexCount; i++)
	{
		TPrimitiveMesh tmp;
		for (int j = 0; j < 4; j++)
		{
			//if (j == 1)
			//{
			//	tmp.pos[j] = -tMesh.meshArrays[i].pos[j];
			//}
			//else
			tmp.pos[j] = tMesh.meshArrays[i].pos[j];
			tmp.pos[j] *= 0.01;
			if (j < 2)
			{
				tmp.uv[j] = tMesh.meshArrays[i].uv[j];
			}
		}
		pMesh[i] = tmp;
		ptWorld->atMesh[nThisEntity].m_VertexData.push_back(XMFLOAT3(tmp.pos[0], tmp.pos[1], tmp.pos[2]));
	}

	XMVECTOR playerGravity;
	playerGravity.m128_f32[1] = -0.000001;
	playerGravity.m128_f32[0] = 0;
	playerGravity.m128_f32[2] = 0;
	playerGravity.m128_f32[3] = 0;
	ptWorld->atRigidBody[nThisEntity].gravity = playerGravity;

	ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = srv;
	ptWorld->atMesh[nThisEntity].m_nVertexCount = tMesh.nUniqueVertexCount;
	ptWorld->atMesh[nThisEntity].m_nVertexBufferStride = sizeof(TPrimitiveMesh);
	ptWorld->atMesh[nThisEntity].m_nVertexBufferOffset = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.ByteWidth = sizeof(TPrimitiveMesh) * ptWorld->atMesh[nThisEntity].m_nVertexCount;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.CPUAccessFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.MiscFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.StructureByteStride = 0;

	ptWorld->atMesh[nThisEntity].m_d3dVertexData.pSysMem = pMesh;
	ptWorld->atMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;

	ptWorld->atMesh[nThisEntity].m_nIndexCount = tMesh.nPolygonVertexCount;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.ByteWidth = sizeof(unsigned int) * ptWorld->atMesh[nThisEntity].m_nIndexCount;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.CPUAccessFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.MiscFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.StructureByteStride = 0;

	ptWorld->atMesh[nThisEntity].m_d3dIndexData.pSysMem = tMesh.indexBuffer;
	ptWorld->atMesh[nThisEntity].m_d3dIndexData.SysMemPitch = 0;
	ptWorld->atMesh[nThisEntity].m_d3dIndexData.SysMemSlicePitch = 0;

	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = XMMatrixMultiply(ptWorld->atWorldMatrix[nThisEntity].worldMatrix, XMMatrixTranslation(0, 0, 0));
	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = XMMatrixMultiply(ptWorld->atWorldMatrix[nThisEntity].worldMatrix, XMMatrixRotationRollPitchYaw(tMesh.worldRotation[0], tMesh.worldRotation[1], tMesh.worldRotation[2]));
	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = XMMatrixMultiply(ptWorld->atWorldMatrix[nThisEntity].worldMatrix, XMMatrixScaling(.01, .01, .01));
	return nThisEntity;
}

//unsigned int CreateUIButton(TWorld * ptWorld, XMMATRIX SpawnPosition, float width, float height, float offsetX, float offsetY, LPRECT window, std::vector<TUIVertices*> & atRectVertices)

unsigned int CreateUILabel(TWorld * ptWorld, XMMATRIX SpawnPosition, float width, float height, float offsetX, float offsetY, std::vector<TUIVertices*>& atUIVertices)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	//if (window != nullptr)
	//	uiMask = uiMask | COMPONENT_BUTTON;

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_SHADERID;
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK | COMPONENT_LABEL;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK;

	TUIVertices* rectVerts = new TUIVertices
	{
		TUIVert{ XMFLOAT3((offsetX - (width / 2)) * .1, (offsetY + (height / 2)) * .1, 0), XMFLOAT2(0, 0) },	//0 Top Left
		TUIVert{ XMFLOAT3((offsetX + (width / 2)) * .1, (offsetY + (height / 2)) * .1, 0), XMFLOAT2(1, 0) },	//1 Top Right
		TUIVert{ XMFLOAT3((offsetX - (width / 2)) * .1, (offsetY - (height / 2)) * .1, 0), XMFLOAT2(0, 1) },	//2 Bottom Left
		TUIVert{ XMFLOAT3((offsetX + (width / 2)) * .1, (offsetY - (height / 2)) * .1, 0), XMFLOAT2(1, 1) }		//3 Bottom Right
	};

	atUIVertices.push_back(rectVerts);
	int index = atUIVertices.size() - 1;

	ptWorld->atLabel[nThisEntity].width = width;
	ptWorld->atLabel[nThisEntity].height = height;
	ptWorld->atLabel[nThisEntity].x = offsetX;
	ptWorld->atLabel[nThisEntity].y = offsetY;

	XMMATRIX spawnMatrix = /*XMMatrixMultiply(*/SpawnPosition/*, XMMatrixScaling(width, height, 0))*/;

	static short rectIndices[]
	{
		0, 1, 3,	//tl->tr->br
		3, 2, 0		//br->bl->tl
	};

	ptWorld->atMesh[nThisEntity].m_nIndexCount = 6;
	ptWorld->atMesh[nThisEntity].m_nVertexCount = 4;

	ptWorld->atMesh[nThisEntity].m_nVertexBufferStride = sizeof(TUIVert);
	ptWorld->atMesh[nThisEntity].m_nVertexBufferOffset = 0;

	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.ByteWidth = sizeof(TUIVert) * ptWorld->atMesh[nThisEntity].m_nVertexCount;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.CPUAccessFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.MiscFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.StructureByteStride = 0;

	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.ByteWidth = sizeof(short) * ptWorld->atMesh[nThisEntity].m_nIndexCount;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.CPUAccessFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.MiscFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.StructureByteStride = 0;


	ptWorld->atMesh[nThisEntity].m_d3dVertexData.pSysMem = &atUIVertices[index]->m_d3dfPositions;
	ptWorld->atMesh[nThisEntity].m_d3dIndexData.pSysMem = rectIndices;

	ptWorld->atMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;

	ptWorld->atMesh[nThisEntity].m_d3dIndexData.SysMemPitch = 0;
	ptWorld->atMesh[nThisEntity].m_d3dIndexData.SysMemSlicePitch = 0;

	ptWorld->atShaderID[nThisEntity].m_nShaderID = 7;	//Shader that supports UIShaders

	for (int i = 0; i < ptWorld->atMesh[nThisEntity].m_nVertexCount; ++i) 
	{
		ptWorld->atMesh[nThisEntity].m_VertexData.push_back(atUIVertices[index]->m_d3dfPositions[i].m_d3dfPosition);
	}

	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = spawnMatrix;

	return nThisEntity;
}