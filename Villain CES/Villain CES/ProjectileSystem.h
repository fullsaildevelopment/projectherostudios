#pragma once
#include"Projectile_Component.h"
#include"Entity.h"
class CProjectileSystem {
public:
	CProjectileSystem();
	~CProjectileSystem();
	bool CreateBulletProjectile(int indexofBullet,Clips* Gun);
};

