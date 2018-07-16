#pragma once
#include "Entity.h"
class CPhysicsSystem
{
public:
	CPhysicsSystem();
	~CPhysicsSystem();
	
	XMMATRIX ResolveForces(TRigidbody* _myRigbody,XMMATRIX worldMatrix);
};

