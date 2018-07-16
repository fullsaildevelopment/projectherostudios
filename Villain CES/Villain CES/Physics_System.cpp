#include "stdafx.h"
#include "Physics_System.h"


CPhysicsSystem::CPhysicsSystem()
{
}


CPhysicsSystem::~CPhysicsSystem()
{
}

XMMATRIX CPhysicsSystem::ResolveForces(TRigidbody* _myRigbody, XMMATRIX worldMatrix)
{
	_myRigbody->totalForce += _myRigbody->gravity + _myRigbody->velocity;
	_myRigbody->velocity = _myRigbody->totalForce;
	XMMATRIX localMatrix = XMMatrixTranslationFromVector(_myRigbody->totalForce);
	XMMATRIX FinalMatrix;
	FinalMatrix = XMMatrixMultiply(localMatrix, worldMatrix);
	XMVECTOR zerovector;
	zerovector.m128_f32[0] = 0;
	zerovector.m128_f32[1] = 0;
	zerovector.m128_f32[2] = 0;
	zerovector.m128_f32[3] = 0;

	_myRigbody->totalForce = zerovector;
	return FinalMatrix;

}
