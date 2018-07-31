#pragma once
#ifndef CES_PROJECTILE_COMPONENT_H
#define CES_PROJECTILE_COMPONENT_H
#include"stdafx.h"
using namespace std;
#include <list>

enum eProjectileComponent
{
	COMPONENT_PROJECTILESMASK = 1 << 0,
	COMPONENT_WOOD = 1 << 1,
	COMPONENT_METAL = 1 << 2,
	COMPONENT_CLIP = 1 << 3

};
struct TProjectileMask
{
	int m_tnProjectileMask = 0;
};
struct Clips
{
	float fShootingCoolDown;
	list<bool> nBulletsAvailables;
	vector<int> nBulletsFired;
	vector<float> fAliveTime;
	int nSizeofClipl;
	bool tryToShoot;
	float FValueOfCoolDown;
};
#endif
