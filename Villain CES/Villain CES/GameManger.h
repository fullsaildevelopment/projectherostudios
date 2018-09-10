#pragma once
#include "Graphics_System.h"
#include "InputSystem.h"
#include "Collision_System.h"
#include "Physics_System.h"
#include "Timer.h"
#include "Entity.h"
#include "ProjectileSystem.h"
#include "AI_System.h"
#include "UI_System.h"
#include <array>
#pragma comment(lib, "MSIMG32.lib")

class CGameMangerSystem {
public:

	CGameMangerSystem(HWND window, CInputSystem* pcInputSystem);
	~CGameMangerSystem();
	void LoadLevel();
	int InGameUpdate();
	void RestartLevel();
	int LoadMainMenu();
	void InitializeMainMenu();
	int LoadTitleScreen();
	void InitializeTitleScreen();
	int LoadPauseScreen();
	void InitializePauseScreen();
	void InitializeDeathScreen();
	int LoadOptionsMenu();
	void InitializeOptionsMenu();

	void LoadPathFindingTest();
	int PathFindingExample();
	void FirstSkeltonAiTestLoad();
	int SpacePirateGamePlay();
	void LoadMikesGraphicsSandbox();
	int MikesGraphicsSandbox();
	bool GameOver = false;
	bool GamePaused = false;
	void LoadLevelWithMapInIt();
	int RealLevelUpdate();
private:
	TMaterialOptimized matOpt;

	bool drawtext = true;
	float scale = 0;
	XMMATRIX CameraNewPosition;
	XMMATRIX secondCam;
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
	int PlayerStartIndex = -10;
	int GunIndexForPlayer = -10;
	int rayindex = -10;
	int frustumIndex;
	float zValue = 5;
	bool GameStart;
	bool DrawUI = true;
	int renderToTexturePassIndex = 0;
	float*xPos = new float();
	float*yPos = new float();
	TMeshImport bulletMesh;
	RECT windowRect;
	int screenWidth;
	int screenHeight;
	
	bool options = false;

	//ZB Variables
	TCameraToggle tCameraMode;
	XMMATRIX m_d3dWorldMatrix;
	XMMATRIX m_d3dViewMatrix;
	XMMATRIX m_d3dProjectionMatrix;
	XMMATRIX m_d3dPlayerMatrix;
	
	
	std::vector<TUIVertices*> atUIVertices;
	XTime clickTimer;
	float clickTime;

	TCamera *walkCamera;
	TCamera *aimCamera;
	TCamera *debugCamera;
	TCamera * menuCamera;
	Timers * tAugerTimers;
	System_Times *tTimerInfo;
	float m_RealTimeFov;
	bool bMoving;
	float Health = 1.0f;

};