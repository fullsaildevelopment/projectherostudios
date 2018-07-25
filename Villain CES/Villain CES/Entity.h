#pragma once
#include "stdafx.h"
#include "Graphics_Component.h"
#include "AI_Component.h"
#include "UI_Component.h"
#include "Collision_Component.h"
#include "Physics_Component.h"
#include"Collision_System.h"
#include "Input_Component.h"

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
};
struct TCameraToggle
{
	bool bDebugMode;
	bool bWalkMode;
	bool bAimMode;
	//bool bDebugMode;


};
struct TPrimalVert
{
	XMFLOAT3 m_d3dfPosition;
	XMFLOAT4 m_d3dfColor;
};

struct TSmartMesh
{
	float m_afPosition[4];
	float m_afNormals[3];
	float m_afUVCoordinates[2];
	float m_afJointWeights[4];
	int m_anJointIndices[4];
};

unsigned int createEntity(TWorld *ptWorld);

void destroyEntity(TWorld *ptWorld, unsigned int nEntity);

unsigned int createDebugTransformLines(TWorld *ptWorld);

unsigned int createCube(TWorld * ptWorld);

unsigned int createDebugGrid(TWorld * ptWorld);

unsigned int createMesh(TWorld * ptWorld, ID3D11Device * m_pd3dDevice, TMesh tMesh);
unsigned int createPlayerBox(TWorld * ptWorld);
unsigned int CreateBullet(TWorld * ptWorld,XMMATRIX bulletSpawnLocation );
unsigned int AimingLine(TWorld * ptWorld);

unsigned int createDummyPlayer(TWorld* ptWorld, XMMATRIX playerMatrix);
unsigned int CreateGround(TWorld* ptWorld);
unsigned int CreateWall(TWorld* ptWorld);
unsigned int CreateCelling(TWorld* ptWorld);
unsigned int CreateGun(TWorld* ptWorld, XMMATRIX BulletSpawnLocation, int parentWorldMatrixIndex);