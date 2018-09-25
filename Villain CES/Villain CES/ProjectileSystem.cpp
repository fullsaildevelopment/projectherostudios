#include "ProjectileSystem.h"

CProjectileSystem::CProjectileSystem()
{
}

CProjectileSystem::~CProjectileSystem()
{
}

int CProjectileSystem::CreateBulletProjectile(int indexofBullet, Clips* Gun)
{
	Gun->fShootingCoolDown = Gun->FValueOfCoolDown;

	Gun->nBulletsAvailables.pop_front();
	
	cout << Gun->nBulletsAvailables.size();

	return 1;
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