#pragma once
#include "Entity.h"
#include "Graphics_System.h"
class CUISystem
{
public:
	CUISystem();
	~CUISystem();

	void DrawMenu(TWorld* ptWorld, CGraphicsSystem* pcGraphicsSystem, XMMATRIX cameraMatrix, XMMATRIX worldMatrix);
};

