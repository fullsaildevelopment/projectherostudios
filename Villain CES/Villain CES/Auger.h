#pragma once

#include "InputSystem.h"
#include"Entity.h"
#include"GameManger.h"

class CAuger
{
public:

	CGameMangerSystem* pcGameMangerSystem;
	CInputSystem	*pcInputSystem;
	CGraphicsSystem	*pcGraphicsSystem;



	CAuger();
	CAuger(HWND cTheWindow);
	HWND window2;

	~CAuger();

	void Start();
	void InitializeSystems();
	void Update(bool* loopGame);
	void End();
private:
	
	int CurrentSpotInGame;
	//XMMATRIX m_d3dViewMatrix;



	XMMATRIX peguins;
	int nAiminglineIndex;
	int		m_nIndexToBullets;
	int		m_nIndexToBullets2;

	float	fShootingCoolDown = 0;
	int		m_nClipSize = 10;

	
	bool tempt;
	
};

