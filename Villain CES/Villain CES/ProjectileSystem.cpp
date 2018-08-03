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

bool CProjectileSystem::Reload(Clips * Gun)
{
	if (Gun->nBulletsAvailables.size() != Gun->nSizeofClipl) {
		for (size_t i = Gun->nBulletsAvailables.size(); i < Gun->nSizeofClipl; ++i) {
			Gun->nBulletsAvailables.push_back(true);
		}
		return true;
	}
	return false;
}
