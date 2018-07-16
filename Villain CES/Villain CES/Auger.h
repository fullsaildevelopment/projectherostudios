#pragma once
#include "Graphics_System.h"
#include "InputSystem.h"
#include"Collision_System.h"
#include "Entity.h"
class CAuger
{
public:
	HWND cApplicationWindow;
	CCollisionSystem* pcCollisionSystem;
	TWorld tThisWorld;
	CGraphicsSystem	*pcGraphicsSystem; 
	CInputSystem	*pcInputSystem;
	list<int> bulletsAvailables;
	list<int> BulletsFired;


	CAuger();
	CAuger(HWND cTheWindow);

	~CAuger();

	void Start();
	void InitializeSystems();
	void Update();
	void End();
private:
	XMMATRIX m_d3dWorldMatrix;
	XMMATRIX m_d3dViewMatrix;
	XMMATRIX m_d3dProjectionMatrix;
	XMMATRIX peguins;
	int aiminglineIndex;
	float shootingCoolDown = 0;

	bool tempt;
};

