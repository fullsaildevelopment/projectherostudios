#pragma once
#include "Graphics_System.h"
#include "Entity.h"
class CAuger
{
public:
	HWND cApplicationWindow;
	CGraphicsSystem* pcGraphicsSystem; 
	TWorld tThisWorld;

	CAuger();
	CAuger(HWND cTheWindow);

	~CAuger();

	void Start();
	void InitializeSystems();
	void Update();
	void End();
};

