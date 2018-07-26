#include "ProjectileSystem.h"



CProjectileSystem::CProjectileSystem()
{
}

CProjectileSystem::~CProjectileSystem()
{
}

bool CProjectileSystem::CreateBulletProjectile(int indexofBullet,Clips* Gun)
{
	if (Gun->nBulletsAvailables.size() != 0 && Gun->fShootingCoolDown <= 0) {

		Gun->fShootingCoolDown = Gun->FValueOfCoolDown;
		Gun->nBulletsFired.push_back(indexofBullet);
		Gun->nBulletsAvailables.pop_front();
		Gun->fAliveTime.push_back(0);
		



		return true;

	}
	return false;
}
