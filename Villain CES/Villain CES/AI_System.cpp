#include "stdafx.h"
#include "AI_System.h"

CAISystem::CAISystem()
{
}

CAISystem::~CAISystem()
{
}

void CAISystem::FollowObject(XMMATRIX thingToFollow, XMMATRIX * AIMatrix)
{
	XMMATRIX beforeitChanges = *AIMatrix;
	*AIMatrix = XMMatrixLookAtLH(AIMatrix->r[3],
		thingToFollow.r[3], XMVectorSet(0, 1, 0, 0));
	*AIMatrix = XMMatrixInverse(NULL, *AIMatrix);

	XMVECTOR direction = thingToFollow.r[3] - AIMatrix->r[3];
	direction.m128_f32[0] = 0;
	direction.m128_f32[1] = 0;
	direction.m128_f32[2] = 1;
	direction.m128_f32[3] = 0;
	direction = XMVector3Normalize(direction);
	direction *= 0.001f;//Frame Dependent
	XMMATRIX localMatrix2 = XMMatrixTranslationFromVector(direction);

	*AIMatrix = XMMatrixMultiply(localMatrix2, *AIMatrix);
}

void CAISystem::ShootGun(Clips* AIGun)
{
	AIGun->tryToShoot = true;
}

void CAISystem::SetNumberOfAI(int aiCount)
{
	numberofAI = aiCount;
}

bool CAISystem::LookForPlayer(XMVECTOR start, XMVECTOR end, XMMATRIX boxWorldMaxtrix, TAABB abbcolider, float * distance, CCollisionSystem * pcCollisionPointer)
{
	if (abbcolider.m_IndexLocation == 1) {
		float x = 0;
	}
	return pcCollisionPointer->IsLineInBox(start, end, boxWorldMaxtrix, abbcolider, distance);
}

XMMATRIX CAISystem::LookBackLeftToRight(XMMATRIX AiMatrix, bool leftorRight)
{
	XMMATRIX d3dTmpViewM, d3dMovementM, d3dRotation;
	float fXchange = 0, fYchange = 0, fXEnd = 0, fYEnd = 0;
	if (leftorRight == true)
		d3dRotation = XMMatrixRotationY(0.0001);
	else {
		d3dRotation = XMMatrixRotationY(-0.0001);
	}
	d3dTmpViewM = AiMatrix;
	XMVECTOR d3d_newX, d3d_newY, d3d_existingZ;
	d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);

	d3d_existingZ = d3dTmpViewM.r[2];
	d3d_newX = XMVector3Cross(AiMatrix.r[1], d3d_existingZ);
	d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

	d3d_newX = XMVector3Normalize(d3d_newX);
	d3d_newY = XMVector3Normalize(d3d_newY);

	d3d_existingZ = XMVector3Normalize(d3d_existingZ);

	d3dTmpViewM.r[0] = d3d_newX;
	d3dTmpViewM.r[1] = d3d_newY;
	d3dTmpViewM.r[2] = d3d_existingZ;
	//d3dRotation = XMMatrixRotationX(0);

//	d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);

	//d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);
	return d3dTmpViewM;
}

int CAISystem::GetNumberOfAI()
{
	return numberofAI;
}