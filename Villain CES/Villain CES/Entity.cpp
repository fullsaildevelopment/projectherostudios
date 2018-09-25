#include "stdafx.h"
#include "Entity.h"
#define TEXTURELOADING false

unsigned int CreateDoorWay(TWorld * ptWorld, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	ptWorld->atRigidBody[nThisEntity].ground = false;
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_STATIC | COMPONENT_NONTRIGGER);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
	ptWorld->atSimpleMesh[nThisEntity].m_nColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(20, 10, 0.3f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(4, 10, 0.3f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(20, 0, 0.3f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(4, 0, 0.3f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(20, 10, 3),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(4, 10, 3),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Top B Right
		TPrimalVert{ XMFLOAT3(20, 0, 3),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(4, 0, 3),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(4, 10, 0.3f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 //Top F Right
		TPrimalVert{ XMFLOAT3(20, 10, 0.3f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(20, 0, 3),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//10 Bottom B Left
		TPrimalVert{ XMFLOAT3(4, 0, 3),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(20, 0, 0.3f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(20, 10, 0.3f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(4, 10, 0.3f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
		TPrimalVert{ XMFLOAT3(4, 0, 0.3f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }//13 Bottm F Right
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
	return nThisEntity;
}

unsigned int SpawnLevelChanger(TWorld * ptWorld, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_STATIC | COMPONENT_TRIGGER);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
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
			if (nCurrentEntity == 16) {
				float x = 0;
			}
			return(nCurrentEntity);
		}
	}

	printf("Error!  No more entities left!\n");
	return(ENTITYCOUNT);
}


unsigned int createEntityReverse(TWorld * ptWorld)
{
	unsigned int nCurrentEntity;
	//Mark entity for Creation by finding the first index in the entity list that has no components. 
	//Return the index of this marked entity
	for (nCurrentEntity = ENTITYCOUNT; nCurrentEntity >= 0; --nCurrentEntity)
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
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_NONE);
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
	ptWorld->atProjectiles[nThisEntity].m_tnProjectileMask = (COMPONENT_PROJECTILESMASK);
	ptWorld->atInputMask[nThisEntity].m_tnInputMask = (COMPONENT_INPUTMASK);
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

	ptWorld->atClip[nThisEntity].nBulletsAvailables.clear();

	ptWorld->atClip[nThisEntity].tryToShoot = false;
	ptWorld->atClip[nThisEntity].tryToReload = false;
	ptWorld->atClip[nThisEntity].maderay = false;

	ptWorld->atLabel[nThisEntity].height = 0;
	ptWorld->atLabel[nThisEntity].width = 0;
	ptWorld->atLabel[nThisEntity].x = 0;
	ptWorld->atLabel[nThisEntity].y = 0;

	ptWorld->atButton[nThisEntity].boundingBox = { 0, 0, 0, 0 };
	ptWorld->atButton[nThisEntity].sceneIndex = 0;
	ptWorld->atButton[nThisEntity].enabled = false;

	ptWorld->atText[nThisEntity].textBoundingBox = { 0, 0, 0, 0 };
	ptWorld->atText[nThisEntity].textBuffer = nullptr;
	ptWorld->atText[nThisEntity].textColor[0] = 0;
	ptWorld->atText[nThisEntity].textColor[1] = 0;
	ptWorld->atText[nThisEntity].textColor[2] = 0;
	ptWorld->atText[nThisEntity].textSize = -1;
	ptWorld->atText[nThisEntity].justification = 0;

	//ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = NULL;
}

unsigned int createDebugTransformLines(TWorld * ptWorld)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	//ptWorld->anComponentMask[nThisEntity] =  COMPONENT_DEBUGMESH;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID);//138
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
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
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);

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

unsigned int CreateSkybox(TWorld * ptWorld, ID3D11ShaderResourceView * srv)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_SKYBOX | COMPONENT_TEXTURE | COMPONENT_SHADERID);
	static XMFLOAT3 skyboxVerts[]
	{
		XMFLOAT3(-500.0f, 500.0f, 500.0f),
		XMFLOAT3(500.0f, 500.0f, 500.0f),
		XMFLOAT3(-500.0f, -500.0f, 500.0f),
		XMFLOAT3(500.0f, -500.0f, 500.0f),
		XMFLOAT3(-500.0f, 500.0f, -500.0f),
		XMFLOAT3(500.0f, 500.0f, -500.0f),
		XMFLOAT3(-500.0f, -500.0f, -500.0f),
		XMFLOAT3(500.0f, -500.0f, -500.0f),

		XMFLOAT3(500.0f, 500.0f, 500.0f),
		XMFLOAT3(-500.0f, 500.0f, 500.0f),
		XMFLOAT3(-500.0f, -500.0f, -500.0f),
		XMFLOAT3(500.0f, -500.0f, -500.0f),
		XMFLOAT3(-500.0f, -500.0f, 500.0f),
		XMFLOAT3(-500.0f, 500.0f, 500.0f),
		XMFLOAT3(500.0f, 500.0f, 500.0f),
		XMFLOAT3(500.0f, -500.0f, 500.0f)
	};

	static unsigned int cubeIndices[]
	{
		2,0,3,//0,2,3,
		1,3,0,//3,1,0,
		//
		6,7,4,//7,6,4,
		5,4,7,//4,5,7,
		//
		9,4,8,//4,9,8,
		8,4,5,//4,8,5,
		//
		3,10,2,//10,3,2,
		3,11,10,//11,3,10,
		//
		6,4,12,//4,6,12,
		13,12,4,//12,13,4,
		//
		15,14,7,//14,15,7,
		7,14,5,//14,7,5,
	};
	ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = srv;
	ptWorld->atMesh[nThisEntity].m_nIndexCount = 36;
	ptWorld->atMesh[nThisEntity].m_nVertexCount = 16;

	ptWorld->atMesh[nThisEntity].m_nVertexBufferStride = sizeof(XMFLOAT3);
	ptWorld->atMesh[nThisEntity].m_nVertexBufferOffset = 0;

	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.ByteWidth = sizeof(XMFLOAT3) * ptWorld->atMesh[nThisEntity].m_nVertexCount;
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

	ptWorld->atMesh[nThisEntity].m_d3dVertexData.pSysMem = skyboxVerts;
	ptWorld->atMesh[nThisEntity].m_d3dIndexData.pSysMem = cubeIndices;

	ptWorld->atMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;

	ptWorld->atMesh[nThisEntity].m_d3dIndexData.SysMemPitch = 0;
	ptWorld->atMesh[nThisEntity].m_d3dIndexData.SysMemSlicePitch = 0;

	ptWorld->atShaderID[nThisEntity].m_nShaderID = 9;

	return 0;
}

unsigned int CreateClayTon(TWorld * ptWorld)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_NONTRIGGER);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK | COMPONENT_RIGIDBODY);
	ptWorld->atInputMask[nThisEntity].m_tnInputMask = (COMPONENT_INPUTMASK | COMPONENT_CLAYTON);
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

//Mesh Bullet
unsigned int CreateBullet(TWorld * ptWorld, ID3D11Device * m_pd3dDevice, TMeshImport tMesh, TMaterialImport tMaterial, XMMATRIX BulletSpawnLocation, int MaterialID)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	size_t result = 0;
	mbstate_t d;
	ID3D11ShaderResourceView * srv;

	switch (MaterialID)
	{
	case 0:
	{
		ptWorld->atProjectiles[nThisEntity].m_tnProjectileMask = (COMPONENT_PROJECTILESMASK | COMPONENT_METAL);
		result = CreateWICTextureFromFile(m_pd3dDevice, L"PBRTestScene.fbm\\PaintedMetal04_met.jpg", nullptr, &srv, NULL);
		ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = srv;
		break;
	}
	case 1:
	{
		ptWorld->atProjectiles[nThisEntity].m_tnProjectileMask = (COMPONENT_PROJECTILESMASK | COMPONENT_WOOD);
		result = CreateWICTextureFromFile(m_pd3dDevice, L"TestScene_V1.fbm\\Wood01_col.jpg", nullptr, &srv, NULL);
		ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = srv;
		break;
	}
	default:
		break;
	}
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK | COMPONENT_RIGIDBODY);

	ptWorld->atMesh[nThisEntity].m_nIndexCount = tMesh.nPolygonVertexCount;
	ptWorld->atMesh[nThisEntity].m_nVertexCount = tMesh.nUniqueVertexCount;

	ptWorld->atMesh[nThisEntity].m_nVertexBufferStride = sizeof(TPrimitiveMesh);
	ptWorld->atMesh[nThisEntity].m_nVertexBufferOffset = 0;

	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.ByteWidth = sizeof(TPrimitiveMesh) * ptWorld->atMesh[nThisEntity].m_nVertexCount;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.CPUAccessFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.MiscFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.StructureByteStride = 0;

	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.ByteWidth = sizeof(int) * ptWorld->atMesh[nThisEntity].m_nIndexCount;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.CPUAccessFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.MiscFlags = 0;
	ptWorld->atMesh[nThisEntity].m_d3dIndexBufferDesc.StructureByteStride = 0;

	ptWorld->atMesh[nThisEntity].m_d3dVertexData.pSysMem = tMesh.meshArrays;
	ptWorld->atMesh[nThisEntity].m_d3dIndexData.pSysMem = tMesh.indexBuffer;
	ptWorld->atMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;
	ptWorld->atMesh[nThisEntity].m_d3dIndexData.SysMemPitch = 0;
	ptWorld->atMesh[nThisEntity].m_d3dIndexData.SysMemSlicePitch = 0;

	XMVECTOR zerovector = XMVectorZero();

	ptWorld->atRigidBody[nThisEntity].gravity = zerovector;
	ptWorld->atRigidBody[nThisEntity].totalForce = zerovector;
	ptWorld->atRigidBody[nThisEntity].ground = false;
	ptWorld->atRigidBody[nThisEntity].wall = false;

	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = BulletSpawnLocation;

	ptWorld->atShaderID[nThisEntity].m_nShaderID = 5;

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

unsigned int CreateBullet(TWorld * ptWorld, XMMATRIX BulletSpawnLocation, int MaterialID, int bulletType)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	switch (MaterialID)
	{
	case 0:
		if (bulletType == 0)
		{
			ptWorld->atProjectiles[nThisEntity].m_tnProjectileMask = (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_FRIENDLY);
		}
		else if (bulletType == 1)
		{
			ptWorld->atProjectiles[nThisEntity].m_tnProjectileMask = (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_ENEMY);
		}
		break;
	case 1:
		if (bulletType == 0)
		{
			ptWorld->atProjectiles[nThisEntity].m_tnProjectileMask = (COMPONENT_METAL | COMPONENT_WOOD | COMPONENT_FRIENDLY);
		}
		else if (bulletType == 1)
		{
			ptWorld->atProjectiles[nThisEntity].m_tnProjectileMask = (COMPONENT_METAL | COMPONENT_WOOD | COMPONENT_ENEMY);
		}
		break;
	default:
		break;
	}

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK | COMPONENT_RIGIDBODY);
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

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
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

unsigned int createDebugCamera(TWorld * ptWorld, XMMATRIX debugCamera)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	//ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_STATIC | COMPONENT_NONTRIGGER;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK);
	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = debugCamera;

	return 0;
}

unsigned int CreateGround(TWorld * ptWorld, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	ptWorld->atRigidBody[nThisEntity].ground = true;
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_STATIC | COMPONENT_NONTRIGGER);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
	ptWorld->atSimpleMesh[nThisEntity].m_nColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(-30, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(10, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(-30, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(10, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(-30, 0.5f, -180),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(10, 0.5f, -180),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(-30, -0.5f, -180),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(10, -0.5f, -180),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(10, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(-30, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(-30, -0.5f, -180),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//10 Bottom B Left
		TPrimalVert{ XMFLOAT3(10, -0.5f, -180),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(-30, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(-30, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(10, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
		TPrimalVert{ XMFLOAT3(10, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }//15 Bottm F Right
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

	return nThisEntity;
}

unsigned int CreateWall(TWorld * ptWorld, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	ptWorld->atRigidBody[nThisEntity].wall = true;
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_STATIC | COMPONENT_NONTRIGGER);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
	ptWorld->atSimpleMesh[nThisEntity].m_nColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(2, 10, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(4, 10, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(2, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(4, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(2, 10, -28),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(4, 10, -28),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(2, -0.5f, -28),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(4, -0.5f, -28),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(4, 10, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(2, 10, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(2, -0.5f, -28),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//10 Bottom B Left
		TPrimalVert{ XMFLOAT3(4, -0.5f, -28),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
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

	return nThisEntity;
}

unsigned int CreateCelling(TWorld * ptWorld, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	ptWorld->atRigidBody[nThisEntity].wall = true;
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_STATIC | COMPONENT_NONTRIGGER);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
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
	return nThisEntity;
}

unsigned int CreateGun(TWorld * ptWorld, XMMATRIX BulletSpawnLocation, int parentWorldMatrixIndex, float xoffset, float yoffset, float zoffset, int clipSize, float shootingCOooldown)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
	ptWorld->atProjectiles[nThisEntity].m_tnProjectileMask = (COMPONENT_PROJECTILESMASK | COMPONENT_CLIP);
	ptWorld->atClip[nThisEntity].nSizeofClipl = clipSize;
	ptWorld->atClip[nThisEntity].FValueOfCoolDown = shootingCOooldown;
	ptWorld->atClip[nThisEntity].fShootingCoolDown = 0;
	ptWorld->atClip[nThisEntity].GunMode = true;
	for (int i = 0; i < ptWorld->atClip[nThisEntity].nSizeofClipl; ++i)
	{
		ptWorld->atClip[nThisEntity].nBulletsAvailables.push_back(false);
	}
	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(-0, 0.5f, 0.5f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(-0, -0, 0.5f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(0.5f, -0, 0.5f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(-0, 0.5f, -0.5f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(-0, -0, -0.5f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(0.5f, -0, -0.5f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(-0, 0.5f, 0.5f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(-0, -0, -0.5f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//10 Bottom B Left
		TPrimalVert{ XMFLOAT3(0.5f, -0, -0.5f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(-0, -0, 0.5f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(-0, 0.5f, 0.5f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(0.5f, 0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
		TPrimalVert{ XMFLOAT3(0.5f, -0, 0.5f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }//15 Bottm F Right
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
	ptWorld->atOffSetMatrix[nThisEntity] = XMMatrixTranslation(xoffset, yoffset, zoffset);

	return nThisEntity;
}

unsigned int CreateScyllianGun(TWorld * ptWorld, ID3D11Device * m_pd3dDevice, XMMATRIX BulletSpawnLocation, int parentWorldMatrixIndex, float xoffset, float yoffset, float zoffset, int clipSize, float shootingCOooldown, TMeshImport tMesh, TMaterialImport tMaterial)
{
	unsigned int nThisEntity = createEntity(ptWorld);

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
				ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = srv;
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
				ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = srv;
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
		ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = srv;
	}
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID);
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
	ptWorld->atProjectiles[nThisEntity].m_tnProjectileMask = (COMPONENT_PROJECTILESMASK | COMPONENT_CLIP);
	ptWorld->atClip[nThisEntity].nSizeofClipl = clipSize;
	ptWorld->atClip[nThisEntity].FValueOfCoolDown = shootingCOooldown;
	ptWorld->atClip[nThisEntity].fShootingCoolDown = 0;
	ptWorld->atClip[nThisEntity].GunMode = true;
	for (int i = 0; i < ptWorld->atClip[nThisEntity].nSizeofClipl; ++i)
	{
		ptWorld->atClip[nThisEntity].nBulletsAvailables.push_back(false);
	}

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
			tmp.pos[j] = tMesh.meshArrays[i].pos[j];
			//tmp.pos[j] *= 0.01;
			if (j < 2)
			{
				tmp.uv[j] = tMesh.meshArrays[i].uv[j];
			}
		}
		pMesh[i] = tmp;
		ptWorld->atMesh[nThisEntity].m_VertexData.push_back(XMFLOAT3(tmp.pos[0], tmp.pos[1], tmp.pos[2]));
	}

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

	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = BulletSpawnLocation;
	ptWorld->atParentWorldMatrix[nThisEntity] = parentWorldMatrixIndex;
	ptWorld->atOffSetMatrix[nThisEntity] = XMMatrixTranslation(xoffset, yoffset, zoffset);

	return nThisEntity;
}

unsigned int CreateSimpleGunAi(TWorld * ptWorld, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_NONTRIGGER);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK | COMPONENT_FOLLOW);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK | COMPONENT_RIGIDBODY);
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

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_NONTRIGGER);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK | COMPONENT_SEARCH);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK | COMPONENT_RIGIDBODY);

	//TPrimalVert{ XMFLOAT3(-0, 0.5f, 0),
	//	TPrimalVert{ XMFLOAT3(0, 0.5f, 10),




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

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
	ptWorld->atProjectiles[nThisEntity].m_tnProjectileMask = (COMPONENT_PROJECTILESMASK | COMPONENT_RAYGUN);
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

unsigned int CreateFrustumLines(TWorld * ptWorld,XMFLOAT3 fartopleft, XMFLOAT3 nearbottomleft, XMFLOAT3 neartopright, XMFLOAT3 nearbottomright, XMFLOAT3 fartopright, XMFLOAT3 farbottomleft, XMFLOAT3 farbottomright, XMFLOAT3 neartopleft, int parentWorldMatrixIndex, float xoffset, float yoffset, float zoffset,  XMFLOAT3 normalLine1, XMFLOAT3 normalLine2)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK );
	ptWorld->atAIVision[nThisEntity].indexoffrustum = nThisEntity;

	static TPrimalVert atCubeVertices[]{
		TPrimalVert{ normalLine1 ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		TPrimalVert{ normalLine2 ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		// line 1
		TPrimalVert{ neartopleft ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		TPrimalVert{ fartopleft ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		// line 2
		TPrimalVert{ neartopleft ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		TPrimalVert{ nearbottomleft ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		// line 3
		TPrimalVert{ neartopleft ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		TPrimalVert{ neartopright ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		// line 4
		TPrimalVert{ neartopright ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		TPrimalVert{ nearbottomright ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		// line 5
		TPrimalVert{ neartopright ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		TPrimalVert{ fartopright ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		// line 6
		TPrimalVert{ nearbottomleft ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		TPrimalVert{ farbottomleft ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		// line 7
		TPrimalVert{ nearbottomleft ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		TPrimalVert{ nearbottomright ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		// line 8
		TPrimalVert{ nearbottomleft ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		TPrimalVert{ farbottomright ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		// line 9
		TPrimalVert{ farbottomleft ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		TPrimalVert{ fartopleft ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		// line 10
		TPrimalVert{ farbottomleft ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		TPrimalVert{ farbottomright ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		// line 11
		TPrimalVert{ farbottomright ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		TPrimalVert{ fartopright ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		// line 12
		TPrimalVert{ fartopright ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		TPrimalVert{ fartopleft ,XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
	};
	ptWorld->atDebugMesh[nThisEntity].m_nVertexCount = 26;

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
	//// line 1
	//render->add_debug_line(neartopleft, fartopleft, black);

	//	// line 2

	//render->add_debug_line(neartopleft, nearbottomleft, black);
	//	//  line 3

	//render->add_debug_line(neartopleft, neartopright, black);
	//	// line 4

	//render->add_debug_line(neartopright, nearbottomright, black);

	//	// line 5
	//render->add_debug_line(neartopright, fartopright, black);

	//	// line 6
	//render->add_debug_line(nearbottomleft, farbottomleft, black);

	//	// line 7
	//render->add_debug_line(nearbottomleft, nearbottomright, black);

	//	// line 8
	//render->add_debug_line(nearbottomright, farbottomright, black);


	////// far starting
	//	// line 9
	//render->add_debug_line(farbottomleft, fartopleft, black);

	//	// line 10
	//render->add_debug_line(farbottomleft, farbottomright, black);

	//	//line 11
	//render->add_debug_line(farbottomright, fartopright, black);

	//	// line 12

	//render->add_debug_line(fartopright, fartopleft, black);
	ptWorld->atParentWorldMatrix[nThisEntity] = parentWorldMatrixIndex;

	ptWorld->atOffSetMatrix[nThisEntity] = XMMatrixTranslation(xoffset, yoffset, zoffset);

	return nThisEntity;
}

unsigned int CreateNodePoint(TWorld * ptWorld, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
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

	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = SpawnPosition;
	return nThisEntity;
}

unsigned int CreateTestAIPathFinding(TWorld * ptWorld, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK | COMPONENT_PATHFINDTEST);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
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

	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = SpawnPosition;
	return nThisEntity;
}

unsigned int CreateSpacePirate(TWorld * ptWorld, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_NONTRIGGER);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK | COMPONENT_RIGIDBODY);
	ptWorld->atActiveAI[nThisEntity].active = FALSE;
	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(-0.5f, 1, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(0.5f, 1, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(-0.5f, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(0.5f, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(-0.5f, 1, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(0.5f, 1, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(-0.5f, -0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(0.5f, -0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(0.5f, 1, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(-0.5f, 1, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(-0.5f, -0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//10 Bottom B Left
		TPrimalVert{ XMFLOAT3(0.5f, -0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(-0.5f, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(-0.5f, 1, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(0.5f, 1, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
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
	AIGravity.m128_f32[1] = -0.0001;
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

unsigned int CreateCover(TWorld * ptWorld, XMMATRIX SpawnPosition,vector<int> coverNodes)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_NONTRIGGER);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);

	//TPrimalVert{ XMFLOAT3(-0, 0.5f, 0),
	//	TPrimalVert{ XMFLOAT3(0, 0.5f, 10),

	for (int i = 0; i < coverNodes.size(); ++i) {

		ptWorld->atCover[nThisEntity].CoverPositions.push_back(coverNodes[i]);
	}

	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(-0.5f, 0.8f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(3, 0.8f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(-0.5f, 0, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(3, 0, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(-0.5f, 0.8f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(3, 0.8f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(-0.5f, 0, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(3, 0, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(3, 0.8f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(-0.5f, 0.8f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(-0.5f, 0, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//10 Bottom B Left
		TPrimalVert{ XMFLOAT3(3, 0, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(-0.5f, 0, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(-0.5f, 0.8f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(3, 0.8f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
		TPrimalVert{ XMFLOAT3(3, 0, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }//15 Bottm F Right
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

unsigned int CreateCoverTriggerZone(TWorld * ptWorld, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_STATIC | COMPONENT_TRIGGER);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK | COMPONENT_COVERTRIGGER);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);

	//TPrimalVert{ XMFLOAT3(-0, 0.5f, 0),
	//	TPrimalVert{ XMFLOAT3(0, 0.5f, 10),

	//for (int i = 0; i < CoverIndex.size(); ++i) {
	//	//ptWorld->atCoverTrigger[nThisEntity].coverAiCanGoTo.push_back(ptWorld->atCover[CoverIndex[i]]);
	//}



	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(-0.5f, 0.8f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(5, 0.8f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(-0.5f, 0, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(5, 0, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(-0.5f, 0.8f, -6),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(5, 0.8f, -6),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(-0.5f, 0, -6),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(5, 0, -6),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(5, 0.8f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(-0.5f, 0.8f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(-0.5f, 0, -6),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//10 Bottom B Left
		TPrimalVert{ XMFLOAT3(5, 0, -6),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(-0.5f, 0, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(-0.5f, 0.8f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(5, 0.8f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
		TPrimalVert{ XMFLOAT3(5, 0, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }//15 Bottm F Right
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

unsigned int CreateFriendlySwordsMan(TWorld * ptWorld, XMMATRIX SpawnPosition, unsigned int playerIndex)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_NONTRIGGER);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK | COMPONENT_FIGHTINGAI);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK | COMPONENT_RIGIDBODY);
	ptWorld->atActiveAI[nThisEntity].active = true;
	ptWorld->atAiHeath[nThisEntity].heath = 400;
	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(-0.5f, 1, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(0.5f, 1, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(-0.5f, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(0.5f, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(-0.5f, 1, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(0.5f, 1, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(-0.5f, -0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(0.5f, -0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(0.5f, 1, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(-0.5f, 1, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(-0.5f, -0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//10 Bottom B Left
		TPrimalVert{ XMFLOAT3(0.5f, -0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(-0.5f, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(-0.5f, 1, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(0.5f, 1, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
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

unsigned int CreateHealingAI(TWorld * ptWorld, XMMATRIX SpawnPosition, unsigned playerIndex)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_NONTRIGGER);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK | COMPONENT_HEALINGAI);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK | COMPONENT_RIGIDBODY);
	ptWorld->atActiveAI[nThisEntity].active = true;
	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(-0.5f, 1, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(0.5f, 1, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(-0.5f, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(0.5f, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(-0.5f, 1, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(0.5f, 1, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(-0.5f, -0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(0.5f, -0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(0.5f, 1, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(-0.5f, 1, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(-0.5f, -0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//10 Bottom B Left
		TPrimalVert{ XMFLOAT3(0.5f, -0.5f, -0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(-0.5f, -0.5f, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(-0.5f, 1, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(0.5f, 1, 0.5f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
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

unsigned int CreateAIVision(TWorld * ptWorld, XMMATRIX VisionSpawnLocation, float zDistance, int parentWorldMatrixIndex, float xoffset, float yoffset, float zoffset)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
	ptWorld->atProjectiles[nThisEntity].m_tnProjectileMask = (COMPONENT_PROJECTILESMASK | COMPONENT_RAYGUN);

	
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
	for (int i = 0; i < ptWorld->atDebugMesh[nThisEntity].m_nVertexCount; ++i)
	{
		//if (i == 0)
		//{
		//	//Nothing
		//}
		//else
		//{
		//	//ptWorld->atAIVision[nThisEntity].keepSearching = tempt;
		//}
	}
	ptWorld->atParentWorldMatrix[nThisEntity] = parentWorldMatrixIndex;
	//	ptWorld->atOffSetMatrix[nThisEntity]= XMMatrixTranslation(-1, 0, 10.5);
	ptWorld->atOffSetMatrix[nThisEntity] = XMMatrixTranslation(xoffset, yoffset, zoffset);
	return nThisEntity;
}

unsigned int CreateTemptUIBox(TWorld * ptWorld, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
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

	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID);//138
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_STATIC | COMPONENT_AABB);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);

	static TPrimalVert atDebugGridVertices[]
	{
		TPrimalVert{ XMFLOAT3(-.5f, 0,  .5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//00	Green
		TPrimalVert{ XMFLOAT3(-.5f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//01	Green
		TPrimalVert{ XMFLOAT3(-.4f, 0,  .5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//02	Red
		TPrimalVert{ XMFLOAT3(-.4f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//03	Red
		TPrimalVert{ XMFLOAT3(-.3f, 0,  .5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//04	Blue

		TPrimalVert{ XMFLOAT3(-.2f, 0,  .5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//06	Blue
		TPrimalVert{ XMFLOAT3(-.2f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//07	Red
		TPrimalVert{ XMFLOAT3(-.1f, 0,  .5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//08	Red
		TPrimalVert{ XMFLOAT3(-.1f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//09	Green

		TPrimalVert{ XMFLOAT3(.0f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//11	Green

		TPrimalVert{ XMFLOAT3(.1f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//13	Green

		TPrimalVert{ XMFLOAT3(.2f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//15	Green

		TPrimalVert{ XMFLOAT3(.3f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//17	Green

		TPrimalVert{ XMFLOAT3(.4f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//19	Green

		TPrimalVert{ XMFLOAT3(.5f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//21	Green

		TPrimalVert{ XMFLOAT3(.5f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//00	Green
		TPrimalVert{ XMFLOAT3(-.5f, 0, -.5f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//01	Green
		TPrimalVert{ XMFLOAT3(.5f, 0, -.4f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//02	Red
		TPrimalVert{ XMFLOAT3(-.5f, 0, -.4f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//03	Red
		TPrimalVert{ XMFLOAT3(.5f, 0, -.3f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//04	Blue

		TPrimalVert{ XMFLOAT3(.5f, 0, -.2f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//06	Blue
		TPrimalVert{ XMFLOAT3(-.5f, 0, -.2f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//07	Red
		TPrimalVert{ XMFLOAT3(.5f, 0, -.1f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//08	Red
		TPrimalVert{ XMFLOAT3(-.5f, 0, -.1f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//09	Green

		TPrimalVert{ XMFLOAT3(-.5f, 0,  .0f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//11	Green

		TPrimalVert{ XMFLOAT3(-.5f, 0,  .1f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//13	Green

		TPrimalVert{ XMFLOAT3(-.5f, 0,  .2f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//15	Green

		TPrimalVert{ XMFLOAT3(-.5f, 0,  .3f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//17	Green
		TPrimalVert{ XMFLOAT3(.5f, 0,  .4f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//18	Green
		TPrimalVert{ XMFLOAT3(-.5f, 0,  .4f),		XMFLOAT4(1.0f, 1.0f,	1.0f, 1.0f) },//19	Green

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

	//{
	//	ptWorld->atDebugMesh[nThisEntity].m_VertexData.push_back(atDebugGridVertices[i].m_d3dfPosition);
	//}

	ptWorld->atShaderID[nThisEntity].m_nShaderID = 2;//Shader that supports TPrimalVert

	return 0;
}

unsigned int createMesh(TWorld * ptWorld, ID3D11Device * m_pd3dDevice, TMeshImport tMesh, TMaterialOptimized tMaterial, int meshIndex)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atRigidBody[nThisEntity].ground = true;

	TMaterialOptimized temp = tMaterial;

	//TMaterialImport tTempMaterial = tMaterial;
	int entityMatIndex = temp.materialIndex[meshIndex];
	int srvIndex = 1;
	if (entityMatIndex == -2)
	{
		ptWorld->atShaderID[nThisEntity].m_nShaderID = 10;
	}
	else
	{
		for (int i = 0; i < temp.numberOfMaterials; i++)
		{
			if (temp.Map_SRVIndex_EntityIndex[i] == entityMatIndex)
			{
				srvIndex = i;
			}
		}
		ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = temp.SRVArrayOfMaterials[srvIndex];
		ptWorld->atShaderID[nThisEntity].m_nShaderID = 6;

	}


#if TEXTURELOADING
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

	//ID3D11Resource * ambientTexture;
	//ID3D11Resource * diffuseTexture;
	//ID3D11Resource * specularTexture;
	//ID3D11Resource * emissiveTexture;
	//ID3D11Resource * normalTexture;
	//ID3D11Resource * bumpTexture;

	//ID3D11Resource * d3dColorMap;
	//ID3D11Resource * d3dNormalMap;
	//ID3D11Resource * d3dEmissiveMap;
	//ID3D11Resource * d3dMetallicMap;
	//ID3D11Resource * d3dRoughnessMap;
	//ID3D11Resource * d3dAmbientOcclusionMap;
	//ID3D11Resource * d3dGlobalDiffuseCubeMap;
	//ID3D11Resource * d3dGlobalSpecularCubeMap;
	//ID3D11Resource * d3dIBLCubeMap;

	ID3D11ShaderResourceView * srv;

	for (unsigned int i = 0; i < 9; i++)
	{
		if (tMaterial.m_tPBRFileNames[i])
		{
			if (tMaterial.m_tPBRFileNames[i][0] == 'c')
			{
				result = CreateWICTextureFromFile(m_pd3dDevice, fnPBR[i], NULL, &srv, NULL);
				ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = srv;

				break;
			}

			//if (tMaterial.m_tPBRFileNames[i][0] == 'n')
			//{
			//	result = CreateWICTextureFromFile(m_pd3dDevice, fnPBR[i], &d3dNormalMap, nullptr, NULL);
			//}
			//if (tMaterial.m_tPBRFileNames[i][0] == 'e')
			//{
			//	result = CreateWICTextureFromFile(m_pd3dDevice, fnPBR[i], &d3dEmissiveMap, nullptr, NULL);
			//}
			//if (tMaterial.m_tPBRFileNames[i][0] == 'm')
			//{
			//	result = CreateWICTextureFromFile(m_pd3dDevice, fnPBR[i], &d3dMetallicMap, nullptr, NULL);
			//}
			//if (tMaterial.m_tPBRFileNames[i][0] == 'r')
			//{
			//	result = CreateWICTextureFromFile(m_pd3dDevice, fnPBR[i], &d3dRoughnessMap, nullptr, NULL);
			//}
			//if (tMaterial.m_tPBRFileNames[i][0] == 'a')
			//{
			//	result = CreateWICTextureFromFile(m_pd3dDevice, fnPBR[i], &d3dAmbientOcclusionMap, nullptr, NULL);
			//}
			//if (tMaterial.m_tPBRFileNames[i][0] == 'd')
			//{
			//	result = CreateDDSTextureFromFile(m_pd3dDevice, fnPBR[i], &d3dGlobalDiffuseCubeMap, nullptr, NULL);
			//}
			//if (tMaterial.m_tPBRFileNames[i][0] == 's')
			//{
			//	result = CreateDDSTextureFromFile(m_pd3dDevice, fnPBR[i], &d3dGlobalSpecularCubeMap, nullptr, NULL);
			//}
			//if (tMaterial.m_tPBRFileNames[i][0] == 'l')
			//{
			//	result = CreateDDSTextureFromFile(m_pd3dDevice, fnPBR[i], &d3dIBLCubeMap, nullptr, NULL);
			//}
		}

		if (tMaterial.m_tFileNames[i] && i < 5)
		{

			//if (tMaterial.m_tFileNames[i][0] == 'a')
			//{
			//	result = CreateWICTextureFromFile(m_pd3dDevice, fnTRAD[i], &ambientTexture, nullptr, NULL);
			//}
			//if (tMaterial.m_tFileNames[i][0] == 'b')
			//{
			//	result = CreateWICTextureFromFile(m_pd3dDevice, fnTRAD[i], &bumpTexture, nullptr, NULL);
			//}

			if (tMaterial.m_tFileNames[i][0] == 'd')
			{
				result = CreateWICTextureFromFile(m_pd3dDevice, fnTRAD[i], NULL, &srv, NULL);
				ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = srv;

				break;
			}

			//if (tMaterial.m_tFileNames[i][0] == 'e')
			//{
			//	result = CreateWICTextureFromFile(m_pd3dDevice, fnTRAD[i], &emissiveTexture, nullptr, NULL);
			//}
			//if (tMaterial.m_tFileNames[i][0] == 'n')
			//{
			//	result = CreateWICTextureFromFile(m_pd3dDevice, fnTRAD[i], &normalTexture, nullptr, NULL);
			//}
			//if (tMaterial.m_tFileNames[i][0] == 's')
			//{
			//	result = CreateWICTextureFromFile(m_pd3dDevice, fnTRAD[i], &specularTexture, nullptr, NULL);
			//}
		}
	}
	if (tMaterial.m_tPBRFileNames[0] == NULL && tMaterial.m_tFileNames[0] == NULL)
	{
		result = CreateWICTextureFromFile(m_pd3dDevice, L"TestScene_V1.fbm\\Wood01_col.jpg", NULL, &srv, NULL);
		ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = srv;
	}
#pragma endregion
#endif
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID);
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_STATIC | COMPONENT_NONTRIGGER);

	//ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK;
	//ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK;
	//ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK | COMPONENT_RIGIDBODY;
	
	//switch (tMaterial.lambert)
	//{
	//case 0:
	//{
	//	ptWorld->atShaderID[nThisEntity].m_nShaderID = 4;
	//	break;//Phong
	//}
	//case 1:
	//{
	//	ptWorld->atShaderID[nThisEntity].m_nShaderID = 5;
	//	break;//Lambert
	//}
	//case 2:
	//{
	//	ptWorld->atShaderID[nThisEntity].m_nShaderID = 6;
	//	break;//PBR
	//}
	//default:
	//	break;
	//}

	TPrimitiveMesh *pMesh = new TPrimitiveMesh[tMesh.nUniqueVertexCount];
	XMFLOAT3 pos;
	ptWorld->atMesh[nThisEntity].m_VertexData.clear();

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
		
		pos.x = tmp.pos[0];
		pos.y = tmp.pos[1];
		pos.z = tmp.pos[2];

		ptWorld->atMesh[nThisEntity].m_VertexData.push_back(pos);

		pMesh[i] = tmp;
	}

	
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

	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = XMMatrixIdentity(); /*XMMatrixMultiply(ptWorld->atWorldMatrix[nThisEntity].worldMatrix, XMMatrixTranslation(tMesh.worldTranslation[0], tMesh.worldTranslation[1], tMesh.worldTranslation[2]));
	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = XMMatrixMultiply(ptWorld->atWorldMatrix[nThisEntity].worldMatrix, XMMatrixRotationRollPitchYaw(tMesh.worldRotation[0], tMesh.worldRotation[1], tMesh.worldRotation[2]));
	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = XMMatrixMultiply(ptWorld->atWorldMatrix[nThisEntity].worldMatrix, XMMatrixScaling(tMesh.worldScaling[0], tMesh.worldScaling[1], tMesh.worldScaling[2]));*/
	return nThisEntity;
}

unsigned int createGSQuad(TWorld * ptWorld, XMFLOAT4 backgroundColor, int target)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID);
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK | COMPONENT_BAR);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
	ptWorld->atProjectiles[nThisEntity].m_tnProjectileMask = (COMPONENT_PROJECTILESMASK);

	static TPrimalVert atPointVertex[]
	{
		TPrimalVert{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }
	};

	ptWorld->atDebugMesh[nThisEntity].m_nVertexCount = 1;

	ptWorld->atDebugMesh[nThisEntity].m_nVertexBufferStride = sizeof(TPrimalVert);
	ptWorld->atDebugMesh[nThisEntity].m_nVertexBufferOffset = 0;

	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.ByteWidth = sizeof(TPrimalVert) * ptWorld->atDebugMesh[nThisEntity].m_nVertexCount;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.MiscFlags = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexBufferDesc.StructureByteStride = 0;
	
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.pSysMem = atPointVertex;

	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atDebugMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;
	ptWorld->atBar[nThisEntity].backgroundColor = backgroundColor;

	if (target != -1)
		ptWorld->atBar[nThisEntity].entityToFollow = target;

	ptWorld->atShaderID[nThisEntity].m_nShaderID = 8;//Geometry Shader


	return nThisEntity;
}

unsigned int CreateStoneObstacle(TWorld * ptWorld, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	ptWorld->atRigidBody[nThisEntity].ground = true;
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_STATIC | COMPONENT_NONTRIGGER);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
	ptWorld->atSimpleMesh[nThisEntity].m_nColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(-30, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(10, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(-30, -0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(10, -0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(-30, 0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(10, 0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(-30, -0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(10, -0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(10, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(-30, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(-30, -0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//10 Bottom B Left
		TPrimalVert{ XMFLOAT3(10, -0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(-30, -0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(-30, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(10, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
		TPrimalVert{ XMFLOAT3(10, -0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }//15 Bottm F Right
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

	return nThisEntity;
}

unsigned int CreateStoneObstacleHorizontal(TWorld * ptWorld, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	ptWorld->atRigidBody[nThisEntity].ground = true;
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_STATIC | COMPONENT_NONTRIGGER);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
	ptWorld->atSimpleMesh[nThisEntity].m_nColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(0, 0.5f, -8),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(1, 0.5f, -8),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(0, -0.5f, -8),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(1, -0.5f, -8),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(0, 0.5f, 10),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(1, 0.5f, 10),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(0, -0.5f, 10),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(1, -0.5f, 10),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(1, 0.5f, -8),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(0, 0.5f, -8),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(0, -0.5f, 10),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Bottom B Left
		TPrimalVert{ XMFLOAT3(1, -0.5f, 10),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(0, -0.5f, -8),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(0, 0.5f, -8),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(1, 0.5f, -8),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
		TPrimalVert{ XMFLOAT3(1, -0.5f, -8),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }//15 Bottm F Right
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

	return nThisEntity;
}

unsigned int CreateStairCase(TWorld * ptWorld, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	ptWorld->atRigidBody[nThisEntity].ground = true;
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_STATIC | COMPONENT_NONTRIGGER);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
	ptWorld->atSimpleMesh[nThisEntity].m_nColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(0, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(2, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(0, -0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(2, -0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(0, 0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(2, 0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(0, -0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(2, -0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(2, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(0, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(0, -0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom B Left
		TPrimalVert{ XMFLOAT3(2, -0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(0, -0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(0, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(2, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
		TPrimalVert{ XMFLOAT3(2, -0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }//15 Bottm F Right
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

	return nThisEntity;
}

unsigned int CreateTree(TWorld * ptWorld, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	ptWorld->atRigidBody[nThisEntity].ground = true;
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_STATIC | COMPONENT_NONTRIGGER);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
	ptWorld->atSimpleMesh[nThisEntity].m_nColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(0, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(2, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(0, -0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(2, -0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(0, 0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(2, 0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(0, -0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(2, -0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(2, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(0, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(0, -0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom B Left
		TPrimalVert{ XMFLOAT3(2, -0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(0, -0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(0, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(2, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
		TPrimalVert{ XMFLOAT3(2, -0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }//15 Bottm F Right
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

	return nThisEntity;
}

unsigned int CreateCylinder(TWorld * ptWorld, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);
	ptWorld->atRigidBody[nThisEntity].ground = true;
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID);
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_STATIC | COMPONENT_NONTRIGGER);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);
	ptWorld->atSimpleMesh[nThisEntity].m_nColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	static TPrimalVert atCubeVertices[]
	{
		TPrimalVert{ XMFLOAT3(0, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//0 Top F Left
		TPrimalVert{ XMFLOAT3(2, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//1 Top F Right
		TPrimalVert{ XMFLOAT3(0, -0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom F Left
		TPrimalVert{ XMFLOAT3(2, -0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//3 Bottom F Right
		TPrimalVert{ XMFLOAT3(0, 0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//4 Top B Left
		TPrimalVert{ XMFLOAT3(2, 0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//5 Top B Right
		TPrimalVert{ XMFLOAT3(0, -0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//6 Bottom B Left
		TPrimalVert{ XMFLOAT3(2, -0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//7 Bottom B Right

		TPrimalVert{ XMFLOAT3(2, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//8 //Top F Right
		TPrimalVert{ XMFLOAT3(0, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//9 // Top F Left
		TPrimalVert{ XMFLOAT3(0, -0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//2 Bottom B Left
		TPrimalVert{ XMFLOAT3(2, -0.5f, 1),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//11 Bottom B Right
		TPrimalVert{ XMFLOAT3(0, -0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//12 Bottom F Left
		TPrimalVert{ XMFLOAT3(0, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//13 Top F Left
		TPrimalVert{ XMFLOAT3(2, 0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//14 Top F Right
		TPrimalVert{ XMFLOAT3(2, -0.5f, 0),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }//15 Bottm F Right
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

	return nThisEntity;
}

unsigned int createClayton(TWorld * ptWorld, ID3D11Device * m_pd3dDevice, TMeshImport tMesh, TMaterialImport tMaterial)
{
	unsigned int nThisEntity = createEntity(ptWorld);



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
				ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = srv;
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
				ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = srv;
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
		ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = srv;
	}
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID);
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_NONTRIGGER);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK | COMPONENT_RIGIDBODY);
	ptWorld->atInputMask[nThisEntity].m_tnInputMask = (COMPONENT_INPUTMASK | COMPONENT_CLAYTON);

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
	playerGravity.m128_f32[1] = -0.0001;
	playerGravity.m128_f32[0] = 0;
	playerGravity.m128_f32[2] = 0;
	playerGravity.m128_f32[3] = 0;
	ptWorld->atRigidBody[nThisEntity].gravity = playerGravity;

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

unsigned int createClaytonAnim(TWorld * ptWorld, ID3D11Device * m_pd3dDevice, TMeshImport tMesh, TMaterialOptimized tMaterial, TAnimationImport tAnim, int meshIndex)
{
	unsigned int nThisEntity = createEntity(ptWorld);

	ptWorld->atShaderID[nThisEntity].m_nShaderID = 11;
	TMaterialOptimized temp = tMaterial;

	int entityMatIndex = temp.materialIndex[meshIndex];
	int srvIndex = 1;

	for (int i = 0; i < temp.numberOfMaterials; i++)
	{
		if (temp.Map_SRVIndex_EntityIndex[i] == entityMatIndex)
		{
			srvIndex = i;
		}
	}
	ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = temp.SRVArrayOfMaterials[srvIndex];



	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_ANIMATION | COMPONENT_SHADERID);
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_NONTRIGGER);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK | COMPONENT_RIGIDBODY);
	ptWorld->atInputMask[nThisEntity].m_tnInputMask = (COMPONENT_INPUTMASK | COMPONENT_CLAYTON);
	ptWorld->atAnimationMask[nThisEntity].m_tnAnimationMask = (COMPONENT_ANIMATIONMASK | COMPONENT_CLAYTONANIM);


	TAnimatedMesh *pMesh = new TAnimatedMesh[tMesh.nUniqueVertexCount];
	for (int i = 0; i < tMesh.nUniqueVertexCount; i++)
	{
		TAnimatedMesh tmp;
		for (int j = 0; j < 4; j++)
		{
			tmp.pos[j] = tMesh.meshArrays[i].pos[j];
			tmp.joints[j] = tMesh.meshArrays[i].joints[j];
			tmp.weights[j] = tMesh.meshArrays[i].weights[j];
			if (j < 2)
			{
				tmp.uv[j] = tMesh.meshArrays[i].uv[j];
			}
		}
		pMesh[i] = tmp;
		ptWorld->atMesh[nThisEntity].m_VertexData.push_back(XMFLOAT3(tmp.pos[0], tmp.pos[1], tmp.pos[2]));
	}

	XMVECTOR playerGravity;
	playerGravity.m128_f32[1] = -0.00000f;
	playerGravity.m128_f32[0] = 0;
	playerGravity.m128_f32[2] = 0;
	playerGravity.m128_f32[3] = 0;
	ptWorld->atRigidBody[nThisEntity].gravity = playerGravity;

	ptWorld->atMesh[nThisEntity].m_nVertexCount = tMesh.nUniqueVertexCount;
	ptWorld->atMesh[nThisEntity].m_nVertexBufferStride = sizeof(TAnimatedMesh);
	ptWorld->atMesh[nThisEntity].m_nVertexBufferOffset = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ptWorld->atMesh[nThisEntity].m_d3dVertexBufferDesc.ByteWidth = sizeof(TAnimatedMesh) * ptWorld->atMesh[nThisEntity].m_nVertexCount;
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

	ptWorld->atAnimationVariant[nThisEntity].tClaytonAnim.forward = true;
	ptWorld->atAnimationVariant[nThisEntity].tClaytonAnim.animType = 1;//Walking
	ptWorld->atAnimationVariant[nThisEntity].tClaytonAnim.currentFrame = 0;//Walking
	ptWorld->atAnimationVariant[nThisEntity].tClaytonAnim.nextFrame = 1;//Walking

	ptWorld->atAnimation[nThisEntity].invBindPosesForJoints = tAnim.invBindPosesForJoints;
	ptWorld->atAnimation[nThisEntity].m_tAnim = tAnim.animClip;

	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = XMMatrixIdentity();//XMMatrixMultiply(ptWorld->atWorldMatrix[nThisEntity].worldMatrix, XMMatrixTranslation(0, 0, 0));
	//ptWorld->atWorldMatrix[nThisEntity].worldMatrix = XMMatrixMultiply(ptWorld->atWorldMatrix[nThisEntity].worldMatrix, XMMatrixRotationRollPitchYaw(tMesh.worldRotation[0], tMesh.worldRotation[1], tMesh.worldRotation[2]));
	//ptWorld->atWorldMatrix[nThisEntity].worldMatrix = XMMatrixMultiply(ptWorld->atWorldMatrix[nThisEntity].worldMatrix, XMMatrixScaling(.01, .01, .01));
	return nThisEntity;
}

unsigned int CreateScyllian(TWorld * ptWorld, ID3D11Device * m_pd3dDevice, TMeshImport tMesh, TMaterialImport tMaterial, XMMATRIX SpawnPosition)
{
	unsigned int nThisEntity = createEntity(ptWorld);

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
				ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = srv;
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
				ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = srv;
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
		ptWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = srv;
	}
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID;
	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_NONTRIGGER;
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST;
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = COMPONENT_UIMASK;
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK | COMPONENT_RIGIDBODY;
	ptWorld->atInputMask[nThisEntity].m_tnInputMask = COMPONENT_INPUTMASK;
	ptWorld->atActiveAI[nThisEntity].active = FALSE;

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
			tmp.pos[j] = tMesh.meshArrays[i].pos[j];
			//tmp.pos[j] *= 0.01;
			if (j < 2)
			{
				tmp.uv[j] = tMesh.meshArrays[i].uv[j];
			}
		}
		pMesh[i] = tmp;
		ptWorld->atMesh[nThisEntity].m_VertexData.push_back(XMFLOAT3(tmp.pos[0], tmp.pos[1], tmp.pos[2]));
	}

	XMVECTOR playerGravity;
	playerGravity.m128_f32[1] = -0.0001;
	playerGravity.m128_f32[0] = 0;
	playerGravity.m128_f32[2] = 0;
	playerGravity.m128_f32[3] = 0;
	ptWorld->atRigidBody[nThisEntity].gravity = playerGravity;

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

	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = SpawnPosition;

	return nThisEntity;
}

unsigned int CreateUILabel(TWorld * ptWorld, XMMATRIX SpawnPosition, float width, float height, float offsetX, float offsetY, std::vector<TUIVert*>* atUIVertices, int _nThisEntity, float z)
{
	unsigned int nThisEntity;

	if (_nThisEntity == -1)
		nThisEntity = createEntity(ptWorld);
	else
		nThisEntity = _nThisEntity;

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK | COMPONENT_LABEL);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);

	TUIVert* rectVerts = new TUIVert[4];
	//{
		rectVerts[0].m_d3dfPosition = XMFLOAT3((offsetX - (width / 2)) * .1, (offsetY + (height / 2)) * .1, z);	//0 Top Left
		rectVerts[0].m_d3dfUVs = XMFLOAT2(0, 0);
		rectVerts[1].m_d3dfPosition = XMFLOAT3((offsetX + (width / 2)) * .1, (offsetY + (height / 2)) * .1, z);	//1 Top Right
		rectVerts[1].m_d3dfUVs = XMFLOAT2(1, 0);
		rectVerts[2].m_d3dfPosition = XMFLOAT3((offsetX - (width / 2)) * .1, (offsetY - (height / 2)) * .1, z);	//2 Bottom Left
		rectVerts[2].m_d3dfUVs = XMFLOAT2(0, 1);
		rectVerts[3].m_d3dfPosition = XMFLOAT3((offsetX + (width / 2)) * .1, (offsetY - (height / 2)) * .1, z);		//3 Bottom Right
		rectVerts[3].m_d3dfUVs = XMFLOAT2(1, 1);
		//};

	//rectVerts->m_d3dfPositions = verts;

	atUIVertices->push_back(rectVerts);
	int index = atUIVertices->size() - 1;

	ptWorld->atLabel[nThisEntity].width = width;
	ptWorld->atLabel[nThisEntity].height = height;
	ptWorld->atLabel[nThisEntity].x = offsetX;
	ptWorld->atLabel[nThisEntity].y = offsetY;

	XMMATRIX spawnMatrix = SpawnPosition;

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


	ptWorld->atMesh[nThisEntity].m_d3dVertexData.pSysMem = &atUIVertices->at(index)[0];
	ptWorld->atMesh[nThisEntity].m_d3dIndexData.pSysMem = rectIndices;

	ptWorld->atMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;

	ptWorld->atMesh[nThisEntity].m_d3dIndexData.SysMemPitch = 0;
	ptWorld->atMesh[nThisEntity].m_d3dIndexData.SysMemSlicePitch = 0;

	ptWorld->atShaderID[nThisEntity].m_nShaderID = 7;	//Shader that supports UIShaders

	for (int i = 0; i < ptWorld->atMesh[nThisEntity].m_nVertexCount; ++i) 
	{
		ptWorld->atMesh[nThisEntity].m_VertexData.push_back(atUIVertices->at(index)[i].m_d3dfPosition);
	}

	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = spawnMatrix;

	return nThisEntity;
}

unsigned int CreateUILabelForText(TWorld* ptWorld, XMMATRIX SpawnPosition, float width, float height, float offsetX, float offsetY, std::vector<TUIVert*>* atUIVertices, std::vector<short*>* atUIIndices, wchar_t* text, unsigned int _textSize, int _nThisEntity, float z)
{
	unsigned int nThisEntity;
	unsigned int totalTextSize = 0;
	unsigned int textSize = 0;
	unsigned int lines = 1;

	std::vector<unsigned int> textSizes;

	if (_nThisEntity == -1)
		nThisEntity = createEntity(ptWorld);
	else
		nThisEntity = _nThisEntity;

	for (int i = 0; i < _textSize - 1; ++i)
	{
		if (text[i] == '\n')
		{
			textSizes.push_back(textSize);

			textSize = 0;
			//++textSize;
			++lines;

			continue;
		}

		++textSize;
		++totalTextSize;
	}
	
	textSizes.push_back(textSize);

	ptWorld->atCollisionMask[nThisEntity].m_tnCollisionMask = (COMPONENT_COLLISIONMASK);
	ptWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_SHADERID);
	ptWorld->atAIMask[nThisEntity].m_tnAIMask = (COMPONENT_AIMASK);
	ptWorld->atUIMask[nThisEntity].m_tnUIMask = (COMPONENT_UIMASK | COMPONENT_LABEL);
	ptWorld->atPhysicsMask[nThisEntity].m_tnPhysicsMask = (COMPONENT_PHYSICSMASK);

	XMFLOAT3 topLeftPos = XMFLOAT3{ (offsetX - (width / 2)) * .1f, (offsetY + (height / 2)) * .1f, z };
	XMFLOAT3 topRightPos = XMFLOAT3{ (offsetX + (width / 2)) * .1f, (offsetY + (height / 2)) * .1f, z };
	XMFLOAT3 bottomLeftPos = XMFLOAT3{ (offsetX - (width / 2)) * .1f, (offsetY - (height / 2)) * .1f, z };
	//XMFLOAT3 ratioPos4 = XMVectorSet((offsetX + (width / 2)) * .1, (offsetY - (height / 2)) * .1, z);
	
	float tWidth = topRightPos.x - topLeftPos.x;
	float tHeight = bottomLeftPos.y - topLeftPos.y;

	TUIVert* rectVerts = new TUIVert[4 * totalTextSize];

	XMFLOAT2 tempUVs[4];

	int lineIndex = 0;
	int loopIndex = 0;
	int countIndex = 0;
	for (int i = 0; countIndex < _textSize - 1; i+=4)
	{
		if (text[countIndex] == '\n')
		{
			i -= 4;

			loopIndex = 0;
			++lineIndex;
			++countIndex;

			continue;
		}

		GetUVsForCharacter(&text[countIndex], &tempUVs[0]);

		rectVerts[i + 0].m_d3dfPosition = XMFLOAT3(topLeftPos.x + ((tWidth / textSizes[lineIndex]) * loopIndex), topLeftPos.y + ((tHeight / lines) * lineIndex), topLeftPos.z);
		rectVerts[i + 0].m_d3dfUVs = XMFLOAT2(tempUVs[0].x, tempUVs[0].y);
		rectVerts[i + 1].m_d3dfPosition = XMFLOAT3(topLeftPos.x + ((tWidth / textSizes[lineIndex]) * (loopIndex + 1)), topLeftPos.y + ((tHeight / lines) * lineIndex), topLeftPos.z);
		rectVerts[i + 1].m_d3dfUVs = XMFLOAT2(tempUVs[1].x, tempUVs[1].y);
		rectVerts[i + 2].m_d3dfPosition = XMFLOAT3(topLeftPos.x + ((tWidth / textSizes[lineIndex]) * loopIndex), topLeftPos.y + ((tHeight / lines) * (lineIndex + 1)), topLeftPos.z);
		rectVerts[i + 2].m_d3dfUVs = XMFLOAT2(tempUVs[2].x, tempUVs[2].y);
		rectVerts[i + 3].m_d3dfPosition = XMFLOAT3(topLeftPos.x + ((tWidth / textSizes[lineIndex]) * (loopIndex + 1)), topLeftPos.y + ((tHeight / lines) * (lineIndex + 1)), topLeftPos.z);
		rectVerts[i + 3].m_d3dfUVs = XMFLOAT2(tempUVs[3].x, tempUVs[3].y);

		++loopIndex;
		++countIndex;
	}

	atUIVertices->push_back(rectVerts);
	ptWorld->atLabel[nThisEntity].vIndex = atUIVertices->size() - 1;

	ptWorld->atLabel[nThisEntity].width = width;
	ptWorld->atLabel[nThisEntity].height = height;
	ptWorld->atLabel[nThisEntity].x = offsetX;
	ptWorld->atLabel[nThisEntity].y = offsetY;

	XMMATRIX spawnMatrix = SpawnPosition;

	short* rectIndices = new short[6 * totalTextSize];

	loopIndex = 0;
	int counterIndex = 0;
	for (int i = 0; loopIndex < totalTextSize; i+=6)
	{
		rectIndices[i] = counterIndex;
		rectIndices[i + 1] = counterIndex + 1;
		rectIndices[i + 2] = counterIndex + 3;	//tl->tr->br
		rectIndices[i + 3] = counterIndex + 3;	//br->bl->tl
		rectIndices[i + 4] = counterIndex + 2;
		rectIndices[i + 5] = counterIndex;

		++loopIndex;
		counterIndex += 4;
	}

	atUIIndices->push_back(rectIndices);
	ptWorld->atLabel[nThisEntity].iIndex = atUIIndices->size() - 1;

	ptWorld->atMesh[nThisEntity].m_nIndexCount = 6 * totalTextSize;
	ptWorld->atMesh[nThisEntity].m_nVertexCount = 4 * totalTextSize;

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

	ptWorld->atMesh[nThisEntity].m_d3dVertexData.pSysMem = &atUIVertices->at(ptWorld->atLabel[nThisEntity].vIndex)[0];
	ptWorld->atMesh[nThisEntity].m_d3dIndexData.pSysMem = &atUIIndices->at(ptWorld->atLabel[nThisEntity].iIndex)[0];

	ptWorld->atMesh[nThisEntity].m_d3dVertexData.SysMemPitch = 0;
	ptWorld->atMesh[nThisEntity].m_d3dVertexData.SysMemSlicePitch = 0;

	ptWorld->atMesh[nThisEntity].m_d3dIndexData.SysMemPitch = 0;
	ptWorld->atMesh[nThisEntity].m_d3dIndexData.SysMemSlicePitch = 0;

	ptWorld->atShaderID[nThisEntity].m_nShaderID = 7;	//Shader that supports UIShaders

	ptWorld->atMesh[nThisEntity].m_VertexData.resize(ptWorld->atMesh[nThisEntity].m_nVertexCount);
	for (int i = 0; i < ptWorld->atMesh[nThisEntity].m_nVertexCount; ++i)
	{
		ptWorld->atMesh[nThisEntity].m_VertexData.push_back(atUIVertices->at(ptWorld->atLabel[nThisEntity].vIndex)[i].m_d3dfPosition);
	}

	ptWorld->atWorldMatrix[nThisEntity].worldMatrix = spawnMatrix;

	return nThisEntity;
}

void GetUVsForCharacter(wchar_t* character, XMFLOAT2* UVs)
{
	switch (*character)
	{
	case ' ':
	{
		UVs[0] = XMFLOAT2{ 0 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 10 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 0 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 10 / 950.0f, 14 / 20.0f };
	}
	break;
	case '!':
	{
		UVs[0] = XMFLOAT2{ 10 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 20 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 10 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 20 / 950.0f, 14 / 20.0f };
	}
	break;
	case '\'':
	{
		UVs[0] = XMFLOAT2{ 70 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 80 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 70 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 80 / 950.0f, 14 / 20.0f };
	}
	break;
	case '.':
	{
		UVs[0] = XMFLOAT2{ 140 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 150 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 140 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 150 / 950.0f, 14 / 20.0f };
	}
	break;
	case 0:
	case '0':
	{
		UVs[0] = XMFLOAT2{ 160 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 170 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 160 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 170 / 950.0f, 15 / 20.0f };
	}
	break;
	case 1:
	case '1':
	{
		UVs[0] = XMFLOAT2{ 170 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 180 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 170 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 180 / 950.0f, 15 / 20.0f };
	}
	break;
	case 2:
	case '2':
	{
		UVs[0] = XMFLOAT2{ 180 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 190 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 180 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 190 / 950.0f, 15 / 20.0f };
	}
	break;
	case 3:
	case '3':
	{
		UVs[0] = XMFLOAT2{ 190 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 200 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 190 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 200 / 950.0f, 15 / 20.0f };
	}
	break;
	case 4:
	case '4':
	{
		UVs[0] = XMFLOAT2{ 200 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 210 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 200 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 210 / 950.0f, 15 / 20.0f };
	}
	break;
	case 5:
	case '5':
	{
		UVs[0] = XMFLOAT2{ 210 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 220 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 210 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 220 / 950.0f, 15 / 20.0f };
	}
	break;
	case 6:
	case '6':
	{
		UVs[0] = XMFLOAT2{ 220 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 230 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 220 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 230 / 950.0f, 15 / 20.0f };
	}
	break;
	case 7:
	case '7':
	{
		UVs[0] = XMFLOAT2{ 230 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 240 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 230 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 240 / 950.0f, 15 / 20.0f };
	}
	break;
	case 8:
	case '8':
	{
		UVs[0] = XMFLOAT2{ 240 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 250 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 240 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 250 / 950.0f, 15 / 20.0f };
	}
	break;
	case 9:
	case '9':
	{
		UVs[0] = XMFLOAT2{ 250 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 260 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 250 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 260 / 950.0f, 15 / 20.0f };
	}
	break;
	case ':':
	{
		UVs[0] = XMFLOAT2{ 260 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 270 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 260 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 270 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'A':
	{
		UVs[0] = XMFLOAT2{ 330 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 340 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 330 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 340 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'B':
	{
		UVs[0] = XMFLOAT2{ 340 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 350 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 340 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 350 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'C':
	{
		UVs[0] = XMFLOAT2{ 350 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 360 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 350 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 360 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'D':
	{
		UVs[0] = XMFLOAT2{ 360 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 370 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 360 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 370 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'E':
	{
		UVs[0] = XMFLOAT2{ 370 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 380 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 370 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 380 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'F':
	{
		UVs[0] = XMFLOAT2{ 380 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 390 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 380 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 390 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'G':
	{
		UVs[0] = XMFLOAT2{ 390 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 400 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 390 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 400 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'H':
	{
		UVs[0] = XMFLOAT2{ 400 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 410 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 400 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 410 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'I':
	{
		UVs[0] = XMFLOAT2{ 410 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 420 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 410 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 420 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'J':
	{
		UVs[0] = XMFLOAT2{ 420 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 430 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 420 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 430 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'K':
	{
		UVs[0] = XMFLOAT2{ 430 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 440 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 430 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 440 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'L':
	{
		UVs[0] = XMFLOAT2{ 440 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 450 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 440 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 450 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'M':
	{
		UVs[0] = XMFLOAT2{ 450 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 460 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 450 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 460 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'N':
	{
		UVs[0] = XMFLOAT2{ 460 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 470 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 460 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 470 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'O':
	{
		UVs[0] = XMFLOAT2{ 470 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 480 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 470 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 480 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'P':
	{
		UVs[0] = XMFLOAT2{ 480 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 490 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 480 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 490 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'Q':
	{
		UVs[0] = XMFLOAT2{ 490 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 500 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 490 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 500 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'R':
	{
		UVs[0] = XMFLOAT2{ 500 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 510 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 500 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 510 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'S':
	{
		UVs[0] = XMFLOAT2{ 510 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 520 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 510 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 520 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'T':
	{
		UVs[0] = XMFLOAT2{ 520 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 530 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 520 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 530 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'U':
	{
		UVs[0] = XMFLOAT2{ 530 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 540 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 530 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 540 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'V':
	{
		UVs[0] = XMFLOAT2{ 540 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 550 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 540 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 550 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'W':
	{
		UVs[0] = XMFLOAT2{ 550 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 560 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 550 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 560 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'X':
	{
		UVs[0] = XMFLOAT2{ 560 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 570 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 560 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 570 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'Y':
	{
		UVs[0] = XMFLOAT2{ 570 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 580 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 570 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 580 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'Z':
	{
		UVs[0] = XMFLOAT2{ 580 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 590 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 580 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 590 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'a':
	{
		UVs[0] = XMFLOAT2{ 650 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 660 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 650 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 660 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'b':
	{
		UVs[0] = XMFLOAT2{ 660 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 670 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 660 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 670 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'c':
	{
		UVs[0] = XMFLOAT2{ 670 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 680 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 670 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 680 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'd':
	{
		UVs[0] = XMFLOAT2{ 680 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 690 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 680 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 690 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'e':
	{
		UVs[0] = XMFLOAT2{ 690 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 700 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 690 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 700 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'f':
	{
		UVs[0] = XMFLOAT2{ 700 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 710 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 700 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 710 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'g':
	{
		UVs[0] = XMFLOAT2{ 710 / 950.0f, 2.5 / 20.0f };
		UVs[1] = XMFLOAT2{ 720 / 950.0f, 2.5 / 20.0f };
		UVs[2] = XMFLOAT2{ 710 / 950.0f, 16 / 20.0f };
		UVs[3] = XMFLOAT2{ 720 / 950.0f, 16 / 20.0f };
	}
	break;
	case 'h':
	{
		UVs[0] = XMFLOAT2{ 720 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 730 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 720 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 730 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'i':
	{
		UVs[0] = XMFLOAT2{ 730 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 740 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 730 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 740 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'j':
	{
		UVs[0] = XMFLOAT2{ 740 / 950.0f, 2.5 / 20.0f };
		UVs[1] = XMFLOAT2{ 750 / 950.0f, 2.5 / 20.0f };
		UVs[2] = XMFLOAT2{ 740 / 950.0f, 16 / 20.0f };
		UVs[3] = XMFLOAT2{ 750 / 950.0f, 16 / 20.0f };
	}
	break;
	case 'k':
	{
		UVs[0] = XMFLOAT2{ 750 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 760 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 750 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 760 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'l':
	{
		UVs[0] = XMFLOAT2{ 760 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 770 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 760 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 770 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'm':
	{
		UVs[0] = XMFLOAT2{ 770 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 780 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 770 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 780 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'n':
	{
		UVs[0] = XMFLOAT2{ 780 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 790 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 780 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 790 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'o':
	{
		UVs[0] = XMFLOAT2{ 790 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 800 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 790 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 800 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'p':
	{
		UVs[0] = XMFLOAT2{ 800 / 950.0f, 2.5 / 20.0f };
		UVs[1] = XMFLOAT2{ 810 / 950.0f, 2.5 / 20.0f };
		UVs[2] = XMFLOAT2{ 800 / 950.0f, 16 / 20.0f };
		UVs[3] = XMFLOAT2{ 810 / 950.0f, 16 / 20.0f };
	}
	break;
	case 'q':
	{
		UVs[0] = XMFLOAT2{ 810 / 950.0f, 2.5 / 20.0f };
		UVs[1] = XMFLOAT2{ 820 / 950.0f, 2.5 / 20.0f };
		UVs[2] = XMFLOAT2{ 810 / 950.0f, 16 / 20.0f };
		UVs[3] = XMFLOAT2{ 820 / 950.0f, 16 / 20.0f };
	}
	break;
	case 'r':
	{
		UVs[0] = XMFLOAT2{ 820 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 830 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 820 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 830 / 950.0f, 15 / 20.0f };
	}
	break;
	case 's':
	{
		UVs[0] = XMFLOAT2{ 830 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 840 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 830 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 840 / 950.0f, 15 / 20.0f };
	}
	break;
	case 't':
	{
		UVs[0] = XMFLOAT2{ 840 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 850 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 840 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 850 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'u':
	{
		UVs[0] = XMFLOAT2{ 850 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 860 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 850 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 860 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'v':
	{
		UVs[0] = XMFLOAT2{ 860 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 870 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 860 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 870 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'w':
	{
		UVs[0] = XMFLOAT2{ 870 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 880 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 870 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 880 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'x':
	{
		UVs[0] = XMFLOAT2{ 880 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 890 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 880 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 890 / 950.0f, 15 / 20.0f };
	}
	break;
	case 'y':
	{
		UVs[0] = XMFLOAT2{ 890 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 900 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 890 / 950.0f, 17 / 20.0f };
		UVs[3] = XMFLOAT2{ 900 / 950.0f, 17 / 20.0f };
	}
	break;
	case 'z':
	{
		UVs[0] = XMFLOAT2{ 900 / 950.0f, 3 / 20.0f };
		UVs[1] = XMFLOAT2{ 910 / 950.0f, 3 / 20.0f };
		UVs[2] = XMFLOAT2{ 900 / 950.0f, 15 / 20.0f };
		UVs[3] = XMFLOAT2{ 910 / 950.0f, 15 / 20.0f };
	}
	break;
	default:
		UVs[0] = XMFLOAT2{ -1, -1 };
		UVs[1] = XMFLOAT2{ -1, -1 };
		UVs[2] = XMFLOAT2{ -1, -1 };
		UVs[3] = XMFLOAT2{ -1, -1 };
		break;
	}
}