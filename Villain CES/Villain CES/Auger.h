#pragma once
#include "GameManger.h"
#include <stdint.h>

class CAuger
{
public:

	CGameMangerSystem *pcGameManagerSystem;
	CInputSystem	*pcInputSystem;
	CGraphicsSystem	*pcGraphicsSystem;

	CAuger();
	CAuger(HWND cTheWindow);

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

	
	bool tempt = true;
	
};
