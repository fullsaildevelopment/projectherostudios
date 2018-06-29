#pragma once
#ifndef CES_COLLISION_COMPONENT_H
#define CES_COLLISION_COMPONENT_H
enum eCollision_Component
{
	COMPONENT_COLLISIONMASK = 1 << 0,
	COMPONENT_AABB			= 1 << 1,
	COMPONENT_OBB			= 1 << 2,
	COMPONENT_SPHERE		= 1 << 3
};

struct TCollisionMask
{
	int m_tnCollisionMask = 0;
};

struct TAABB
{
	XMFLOAT3 m_d3dSmallestWidth;
	XMFLOAT3 m_d3dLargestWidth;
	XMFLOAT3 m_d3dSmallestHeight;
	XMFLOAT3 m_d3dLargestHeight;
};

struct TOBB
{
	/*
		Fill out this structure
	*/
};

struct TSphere
{
	/*
		Fill out this structure
	*/
};
#endif
