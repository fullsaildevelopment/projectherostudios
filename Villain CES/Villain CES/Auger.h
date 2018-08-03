#pragma once
#include "Entity.h"
#include "Graphics_System.h"
#include "InputSystem.h"
#include "Collision_System.h"
#include "Physics_System.h"
#include "ProjectileSystem.h"
class CAuger
{
public:
	HWND cApplicationWindow;
	CCollisionSystem* pcCollisionSystem;
	TWorld tThisWorld;
	CGraphicsSystem	*pcGraphicsSystem; 
	CInputSystem	*pcInputSystem;
	CPhysicsSystem  *pcPhysicsSystem;
	CProjectileSystem* pcProjectileSystem;
	list<int> nBulletsAvailables;
	vector<int> nBulletsFired;
	vector<int> nDeadBullets;
	vector<float> fAliveTime;




	CAuger();
	CAuger(HWND cTheWindow);

	~CAuger();

	void Start();
	void InitializeSystems();
	void Update();
	void End();
private:
	XMMATRIX m_d3dWorldMatrix		= XMMatrixIdentity();
	XMMATRIX m_d3dViewMatrix		= XMMatrixIdentity();
	XMMATRIX m_d3dProjectionMatrix	= XMMatrixIdentity();
	XMMATRIX m_d3dPlayerMatrix		= XMMatrixIdentity();
	XMMATRIX m_d3dCameraMatrix		= XMMatrixIdentity();
    XMMATRIX m_d3d_ResultMatrix		= XMMatrixIdentity();
	XMMATRIX m_d3dOffsetMatrix		= XMMatrixIdentity();

	XMMATRIX peguins				= XMMatrixIdentity();
	int		m_nIndexToBullets;
	int		m_nIndexToBullets2;

	float	fShootingCoolDown = 0;
	int		m_nClipSize = 10;

	TCameraToggle tCameraMode;
	bool tempt;
};

