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

//	tThisWorld.atWorldMatrix[*ptr].worldMatrix = XMMatrixMultiply(localMatrix, tThisWorld.atWorldMatrix[*ptr].worldMatrix);
	_myRigbody->totalForce += _myRigbody->gravity + _myRigbody->velocity;
	_myRigbody->velocity = _myRigbody->totalForce;
	XMMATRIX localMatrix2 = XMMatrixTranslationFromVector(_myRigbody->totalForce);
	XMMATRIX FinalMatrix;
	FinalMatrix = XMMatrixMultiply(localMatrix2, worldMatrix);


	_myRigbody->totalForce = ZeroVector();
	return FinalMatrix;

}

XMVECTOR CPhysicsSystem::ZeroVector()
{
	XMVECTOR zerovector;
	zerovector.m128_f32[0] = 0;
	zerovector.m128_f32[1] = 0;
	zerovector.m128_f32[2] = 0;
	zerovector.m128_f32[3] = 0;
	return zerovector;
}

void CPhysicsSystem::AddBulletForce(TRigidbody * _myRigbody)
{
	XMVECTOR bulletForce;
	bulletForce = ZeroVector();
	bulletForce.m128_f32[2] = 0.0001;
	_myRigbody->totalForce += bulletForce;
}
