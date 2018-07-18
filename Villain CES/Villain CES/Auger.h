#pragma once
#include "Graphics_System.h"
#include "InputSystem.h"
#include"Collision_System.h"
#include"Physics_System.h"
#include "Entity.h"
class CAuger
{
public:
	HWND cApplicationWindow;
	CCollisionSystem* pcCollisionSystem;
	TWorld tThisWorld;
	CGraphicsSystem	*pcGraphicsSystem; 
	CInputSystem	*pcInputSystem;
	CPhysicsSystem  *pcPhysicsSystem;
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
	XMMATRIX m_d3dWorldMatrix;
	XMMATRIX m_d3dViewMatrix;
	XMMATRIX m_d3dProjectionMatrix;
	int		nAiminglineIndex;
	int		m_nIndexToBullets;
	int		m_nIndexToBullets2;

	float	fShootingCoolDown = 0;
	int		m_nClipSize = 10;


	bool tempt;
};

