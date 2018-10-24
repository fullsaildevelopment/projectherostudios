#pragma once
#include "Entity.h"
class CPhysicsSystem
{
public:
	CPhysicsSystem();
	~CPhysicsSystem();

	XMMATRIX ResolveForces(TRigidbody* _myRigbody, XMMATRIX worldMatrix, bool RotationWithForce,float delta);
	//	XMMATRIX ResolveGravity(TRigidbody* _myRigbody, XMMATRIX worldMatrix);
	XMVECTOR ZeroVector();
	void AddBulletForce(TRigidbody* _myRigbody, float zForValue);
};
