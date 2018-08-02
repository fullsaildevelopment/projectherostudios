#pragma once
#include"ProjectileSystem.h"
class CAISystem
{
public:
	CAISystem();
	~CAISystem();
	void FollowAndShoot(XMMATRIX playerMatrix, Clips* AiGun);
};

