#pragma once
#ifndef CES_PHYSICS_COMPONENT_H
#define CES_PHYSICS_COMPONENT_H
enum ePhysicsComponent
{
	//COMPONENT_NONE = 0,
	COMPONENT_PHYSICSMASK	= 1 << 0,
	COMPONENT_RIGIDBODY		= 1 << 1,//Velocity, mass
};

struct TPhysicsMask
{
	int m_tnPhysicsMask = 0;
};

struct TRigidbody
{
	/*
	TODO:
		Fill out Rigidbody Struct
	*/
	XMVECTOR totalForce;
	XMVECTOR velocity;
	XMVECTOR gravity;
	XMVECTOR maxVelocity;
	XMVECTOR CollissionForce;
	bool NeatrlizeForce;



};
#endif
