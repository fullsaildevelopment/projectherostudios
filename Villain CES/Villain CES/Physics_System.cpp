#include "stdafx.h"
#include "Physics_System.h"


CPhysicsSystem::CPhysicsSystem()
{
}


CPhysicsSystem::~CPhysicsSystem()
{
}
/*
* ResolveForces():  Resolves all the Forces
*
* Ins:                  Rigbody of Entity pointer
*						WorldMatrix of the Entity
*
*
* Outs:					Rigbody Pointer
*
* Returns:              XMATRIX that has been impacted by all the forces
*
* Mod. Date:              07/18/2018
* Mod. Initials:          AP
*/
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
/*
* ZeroVector():			Retruns a zero out vector
*
* Ins:                 
*						
*
*
* Outs:					
*
* Returns:              vector that is zeroed out
*
* Mod. Date:              07/18/2018
* Mod. Initials:          AP
*/
XMVECTOR CPhysicsSystem::ZeroVector()
{
	XMVECTOR zerovector;
	zerovector.m128_f32[0] = 0;
	zerovector.m128_f32[1] = 0;
	zerovector.m128_f32[2] = 0;
	zerovector.m128_f32[3] = 0;
	return zerovector;
}
/*
* ZeroVector():				AddBulletForce
*
* Ins:						Rigbody pointer
*
*
*
* Outs:                      Rigbody Pointer
*
* Returns:              
*
* Mod. Date:              07/18/2018
* Mod. Initials:          AP
*/
void CPhysicsSystem::AddBulletForce(TRigidbody * _myRigbody)
{
	XMVECTOR bulletForce;
	bulletForce = ZeroVector();
	bulletForce.m128_f32[2] = 0.0001;
	_myRigbody->totalForce += bulletForce;
}
