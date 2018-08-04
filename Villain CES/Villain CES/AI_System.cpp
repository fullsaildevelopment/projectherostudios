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
	*AIMatrix = XMMatrixLookAtLH(AIMatrix->r[3],
		thingToFollow.r[3], XMVectorSet(0, 1, 0, 0));
	*AIMatrix = XMMatrixInverse(NULL, *AIMatrix);
	XMVECTOR direction = thingToFollow.r[3] - AIMatrix->r[3];
	direction.m128_f32[0] = 0;
	direction.m128_f32[1] = 0;
	direction.m128_f32[2] = 1;
	direction.m128_f32[3] = 0;
	direction = XMVector3Normalize(direction);
	direction *= 0.001f;
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

int CAISystem::GetNumberOfAI()
{
	return numberofAI;
}
