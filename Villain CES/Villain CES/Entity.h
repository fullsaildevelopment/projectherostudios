#pragma once
#include "Graphics_Component.h"
#include "AI_Component.h"
#include "UI_Component.h"
#include "Collision_Component.h"
#include "Physics_Component.h"

struct TWorld
{
	int				anComponentMask[ENTITYCOUNT];
	//Graphics
	TGraphicsMask	atGraphicsMask[ENTITYCOUNT];
	TMesh			atMesh[ENTITYCOUNT];
	TDebugMesh		atDebugMesh[ENTITYCOUNT];
	TAnimation		atAnimation[ENTITYCOUNT];
	TMaterial		atMaterial[ENTITYCOUNT];
	TTexture		atTexture[ENTITYCOUNT];
	TShaderID		atShaderID[ENTITYCOUNT];
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
	
};

struct TPrimalVert
{
	XMFLOAT3 m_d3dfPosition;
	XMFLOAT4 m_d3dfColor;
};

struct TSimpleMesh
{
	float m_afPosition[4];
	float m_afNormals[3];
	float m_afUVCoordinates[2];
	float m_afJointWeights[4];
	int m_anJointIndices[4];
};


unsigned int createEntity(TWorld *ptWorld);

void destroyEntity(TWorld *ptWorld, unsigned int nEntity);

unsigned int createDebugTransformLines(TWorld *ptWorld, TDebugMesh tDebugMesh);

unsigned int createDebugGrid(TWorld * ptWorld, TDebugMesh tDebugMesh);

unsigned int createMesh(TWorld * ptWorld, ID3D11Device * m_pd3dDevice, TMesh tMesh);
