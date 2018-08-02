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

	~CAuger();

	void Start();
	void InitializeSystems();
	void Update();
	void End();
private:
	
	int CurrentSpotInGame;
	
	
};

