#include "ProjectileSystem.h"



CProjectileSystem::CProjectileSystem()
{
}

CProjectileSystem::~CProjectileSystem()
{
}

void CProjectileSystem::CreateBulletProjectile(int indexofBullet,Clips* Gun)
{
	

		Gun->fShootingCoolDown = Gun->FValueOfCoolDown;
		Gun->nBulletsFired.push_back(indexofBullet);
		Gun->nBulletsAvailables.pop_front();
		Gun->fAliveTime.push_back(0);
		cout << Gun->nBulletsAvailables.size();
		




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
