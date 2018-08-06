#pragma once
#include"ProjectileSystem.h"
#include"stdafx.h"
class CAISystem
{
public:
	CAISystem();
	~CAISystem();
	
	//void FollowAndShoot(XMMATRIX playerMatrix, Clips* AiGun);
	void FollowObject(XMMATRIX thingToFollow, XMMATRIX* AIMatrix);
	void ShootGun(Clips* AIGun);
	void SetNumberOfAI(int aiCount);
	bool LookForPlayer(XMVECTOR start,XMVECTOR end,XMMATRIX boxWorldMaxtrix,TAABB abbcolider,float* distance, CCollisionSystem* pcCollisionPointer);
	XMMATRIX LookBackLeftToRight(XMMATRIX worldMatrix);
	int GetNumberOfAI();
private:
	int numberofAI;
};

