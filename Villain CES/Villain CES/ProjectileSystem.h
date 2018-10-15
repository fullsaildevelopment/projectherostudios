#pragma once
#include"Projectile_Component.h"
#include"Entity.h"
class CProjectileSystem {
public:
	CProjectileSystem();
	~CProjectileSystem();
	int CreateBulletProjectile(int indexofBullet, Clips* Gun);
	bool Reload(Clips* Gun);
	int ExtractMaterial(TWorld *tThisWorld, Clips* Gun, int currentEntity);
	XMVECTOR FindBeamEndPoint(XMVECTOR pointInScreenSpace, XMMATRIX in_WorldMatrix, XMMATRIX in_ViewMatrix, XMMATRIX in_ProjectionMatrix, HWND in_WindowHandle, D3D11_VIEWPORT m_d3dViewport);
	
};
