#pragma once
#include "Entity.h"
class CPhysicsSystem
{
public:
	CPhysicsSystem();
	~CPhysicsSystem();
	
	XMMATRIX ResolveForces(TRigidbody* _myRigbody,XMMATRIX worldMatrix);
	XMVECTOR ZeroVector();
	void AddBulletForce(TRigidbody* _myRigbody);
};

