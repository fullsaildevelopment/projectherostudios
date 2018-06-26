#pragma once
#ifndef CES_COLLISION_COMPONENT_H
#define CES_COLLISION_COMPONENT_H
typedef enum
{
//	COMPONENT_NONE = 0,
	COMPONENT_COLLISIONID = 1 << 1
} Collision_Component;
typedef struct
{
	int Collision_ID = 0;
} Collision_ID;
#endif
