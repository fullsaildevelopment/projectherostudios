#pragma once
#include "Entity.h"
class CPhysicsSystem
{
public:
	CPhysicsSystem();
	~CPhysicsSystem();
	
	XMMATRIX ResolveForces(TRigidbody* _myRigbody,XMMATRIX worldMatrix,bool RotationWithForce);
//	XMMATRIX ResolveGravity(TRigidbody* _myRigbody, XMMATRIX worldMatrix);
	XMVECTOR ZeroVector();
	void AddBulletForce(TRigidbody* _myRigbody, float zForValue);
};

