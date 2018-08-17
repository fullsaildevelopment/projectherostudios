#pragma once
#include "Graphics_System.h"
#include "InputSystem.h"
#include"Collision_System.h"
#include"Physics_System.h"
#include "Entity.h"
#include"ProjectileSystem.h"
#include"AI_System.h"
#include "UI_System.h"
#include <array>

class CGameMangerSystem {
public:

	CGameMangerSystem(HWND window, CInputSystem* pcInputSystem);
	~CGameMangerSystem();
	void LoadLevel();
	int InGameUpdate();
	void RestartLevel();
	int LoadMainMenu();
	void InitilizeMainMenu();
	int LoadTitleScreen();
	void InitializeTitleScreen();

private:
	CCollisionSystem * pcCollisionSystem;
	TWorld tThisWorld;
	CGraphicsSystem	*pcGraphicsSystem;
	CInputSystem	*pcInputSystem;
	CPhysicsSystem  *pcPhysicsSystem;
	CProjectileSystem* pcProjectileSystem;
	CAISystem		*pcAiSystem;
	CUISystem		*pcUISystem;
	HWND cApplicationWindow;
	vector<int> UIIndex;
	TCameraToggle tCameraMode;
	int PlayerStartIndex = -10;
	int GunIndexForPlayer = -10;
	int rayindex = -10;
	float zValue = 5;
	bool GamePaused = false;
	bool GameStart;
	bool DrawUI = true;
	XMMATRIX m_d3dWorldMatrix;
	XMMATRIX m_d3dViewMatrix;
	XMMATRIX m_d3dProjectionMatrix;
	XMMATRIX m_d3dPlayerMatrix;
	XMMATRIX m_d3dCameraMatrix;
	XMMATRIX m_d3d_ResultMatrix;
	std::vector<TUIVertices*> atUIVertices;
};