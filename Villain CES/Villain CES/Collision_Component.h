#pragma once
#ifndef CES_COLLISION_COMPONENT_H
#define CES_COLLISION_COMPONENT_H
enum eCollision_Component
{
	COMPONENT_COLLISIONMASK = 1 << 1,
};

struct TCollisionMask
{
	int m_tnCollisionMask = 0;
};
#endif
