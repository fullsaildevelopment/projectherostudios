#pragma once
#include "Graphics_System.h"
#include "Entity.h"
class Auger
{
public:
	HWND appWindow;
	Graphics_System* vGraphicsSystem; 
	World thisWorld;

	Auger();
	Auger(HWND window);

	~Auger();

	void Start();
	void InitializeSystems();
	void Update();
	void End();
};

