#pragma once
#ifndef CES_PHYSICS_COMPONENT_H
#define CES_PHYSICS_COMPONENT_H
typedef enum
{
	//COMPONENT_NONE = 0,
	COMPONENT_PHYSICSID = 1 << 1,
	COMPONENT_RIGIDBODY = 1 << 2,//Velocity, mass
} Physics_Component;

typedef struct
{
	int Physics_ID = 0;
} Physics_ID;

typedef struct
{
	/*
	TODO:
		Fill out Rigidbody Struct
	*/

} Rigidbody;
#endif
