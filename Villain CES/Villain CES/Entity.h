#pragma once
#include "Graphics_Component.h"
#define NearRight 1.0f
#define NearTop	  1.0f
#define NearLeftt -1.0f
#define NearBot -1.0f

#define FarrRight  1.5f
#define FarrTop	  1.5f
#define FarrLeftt  -1.5f
#define FarrBot   -1.5f

#define Near -2.0f
#define Farr 2.0f
struct World
{
	int mask[ENTITY_COUNT];
	Mesh		mesh[ENTITY_COUNT];
	DebugMesh	debug_mesh[ENTITY_COUNT];
	Animation	animation[ENTITY_COUNT];
	
};

struct primalVert
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

struct simple_mesh
{
	float pos[4];
	float norm[3];
	float uv[2];
	float weights[4];
	int joints[4];
};


unsigned int createEntity(World *world);

void destroyEntity(World *world, unsigned int entity);

unsigned int createDebugTransformLines(World *world, DebugMesh meshd);

unsigned int createDebugGrid(World * world, DebugMesh mesh);

unsigned int createFrustum(World * world, Mesh meshd);

unsigned int createMesh(ID3D11Device * dev, World * world, Mesh mesh);
