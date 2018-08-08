#pragma once
#include "stdafx.h"
#include "Graphics_Component.h"
#include "AI_Component.h"
#include "UI_Component.h"
#include "Collision_Component.h"
#include "Physics_Component.h"
#include"Collision_System.h"
#include "Input_Component.h"
#include"Projectile_Component.h"
#include "DirectXTex-master\WICTextureLoader\WICTextureLoader.h"
#include "DirectXTex-master\DDSTextureLoader\DDSTextureLoader.h"

struct TWorld
{
	int				anComponentMask[ENTITYCOUNT];
	//Graphics
	TGraphicsMask	atGraphicsMask[ENTITYCOUNT];
	TMesh			atMesh[ENTITYCOUNT];
	TDebugMesh		atDebugMesh[ENTITYCOUNT];
	TSimpleMesh		atSimpleMesh[ENTITYCOUNT];
	TAnimation		atAnimation[ENTITYCOUNT];
	TMaterial		atMaterial[ENTITYCOUNT];
	TTexture		atTexture[ENTITYCOUNT];
	TShaderID		atShaderID[ENTITYCOUNT];
	TCamera			atCamera[ENTITYCOUNT];
	TWorldMatrix	atWorldMatrix[ENTITYCOUNT];
	int				atParentWorldMatrix[ENTITYCOUNT];
	XMMATRIX		atOffSetMatrix[ENTITYCOUNT];

	//UI
	TUIMask			atUIMask[ENTITYCOUNT];
	TBar			atBar[ENTITYCOUNT];
	TText			atText[ENTITYCOUNT];
	TLabel			atLabel[ENTITYCOUNT];
	//Physics
	TPhysicsMask	atPhysicsMask[ENTITYCOUNT];
	TRigidbody		atRigidBody[ENTITYCOUNT];
	//AI
	TAIMask			atAIMask[ENTITYCOUNT];
	//Collision
	TCollisionMask	atCollisionMask[ENTITYCOUNT];
	TAABB			atAABB[ENTITYCOUNT];
	TOBB			atOBB[ENTITYCOUNT];
	TSphere			atSphere[ENTITYCOUNT];
	//Input
	TInputMask		atInputMask[ENTITYCOUNT];
	TTest			atTest[ENTITYCOUNT];
	TClayton		atClayton[ENTITYCOUNT];
	TSeth			atSeth[ENTITYCOUNT];
	TCaelis			atCaelis[ENTITYCOUNT];
	TProjectileMask atProjectiles[ENTITYCOUNT];
	Clips			atClip[ENTITYCOUNT];
};

struct TPrimalVert
{
	XMFLOAT3 m_d3dfPosition;
	XMFLOAT4 m_d3dfColor;
};

unsigned int SpawnLevelChanger(TWorld *ptWorld, XMMATRIX SpawnPosition);
unsigned int createEntity(TWorld *ptWorld);

void destroyEntity(TWorld *ptWorld, unsigned int nEntity);

unsigned int createDebugTransformLines(TWorld *ptWorld);

unsigned int createCube(TWorld * ptWorld);

unsigned int createDebugGrid(TWorld * ptWorld);

unsigned int CreateClayTon(TWorld * ptWorld);
unsigned int CreateBullet(TWorld * ptWorld,XMMATRIX bulletSpawnLocation, int MaterialID);
unsigned int AimingLine(TWorld * ptWorld, XMMATRIX BulletSpawnLocation, int parentWorldMatrixIndex, float xoffset, float yoffset, float zoffset);

unsigned int CreateGround(TWorld* ptWorld, XMMATRIX SpawnPosition);
unsigned int CreateWall(TWorld* ptWorld, XMMATRIX SpawnPosition);
unsigned int CreateCelling(TWorld* ptWorld, XMMATRIX SpawnPosition);
unsigned int CreateGun(TWorld* ptWorld, XMMATRIX BulletSpawnLocation, int parentWorldMatrixIndex, float xoffset, float yoffset, float zoffset,int clipSize, float shootingCOooldown);
unsigned int createMesh(TWorld * ptWorld, ID3D11Device * m_pd3dDevice, TMeshImport tMesh, TMaterialImport tMaterial);
unsigned int CreateTemptUIBox(TWorld * ptWorld, XMMATRIX SpawnPosition);

 //float zDistance = 0.5f;

