#pragma once
#include"Projectile_Component.h"
//#include"Entity.h"
class CProjectileSystem {
public:
	CProjectileSystem();
	~CProjectileSystem();
	void CreateBulletProjectile(int indexofBullet,Clips* Gun);
	bool Reload( Clips* Gun);


};

