#pragma once
#ifndef CES_PROJECTILE_COMPONENT_H
#define CES_PROJECTILE_COMPONENT_H
#include"stdafx.h"
using namespace std;
#include <list>

enum eProjectileComponent
{
	COMPONENT_PROJECTILESMASK	= 1 << 0,
	COMPONENT_WOOD				= 1 << 1,
	COMPONENT_METAL				= 1 << 2,
	COMPONENT_CLIP				= 1 << 3,
	COMPONENT_RAYGUN			= 1 << 4,
	COMPONENT_ENEMY				= 1 << 5,
	COMPONENT_FRIENDLY			= 1 << 6
};
struct TProjectileMask
{
	int m_tnProjectileMask = 0;
	XMVECTOR Direction;
};
struct Clips
{
	bool maderay;
	bool GunMode = false;
	float fShootingCoolDown;
	list<bool> nBulletsAvailables;
	int nSizeofClipl;
	bool tryToShoot = false;
	bool tryToReload;
	float FValueOfCoolDown;
	int currentMaterial=0;
	XMFLOAT4 colorofBullets;
	float bulletSpeed;
	int gunIndex;
	int indexInclip;
};
#endif
