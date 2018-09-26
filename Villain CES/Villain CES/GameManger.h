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
#include "CAnimationSystem.h"
#include"AudioSystem.h"
#include <array>
#include <exception>
#pragma comment(lib, "MSIMG32.lib")

class CGameMangerSystem {
public:

	CGameMangerSystem(HWND window, CInputSystem* pcInputSystem);
	~CGameMangerSystem();
	int LoadMainMenu();
	void InitializeMainMenu();
	int LoadTitleScreen();
	void InitializeTitleScreen();
	int LoadLoadingScreen(bool _continue);
	void InitializeLoadingScreen();
	void InitializePauseScreen();
	void InitializeEndScreen(bool playerWin);
	void InitializeOptionsMenu();
	void InitializeCredits();
	void InitializeHUD();

	void LoadPathFindingTest();
	int PathFindingExample();
	void FirstSkeltonAiTestLoad();
	int SpacePirateGamePlay();
	void LoadMikesGraphicsSandbox();
	int MikesGraphicsSandbox();
	void LoadLevelWithMapInIt();
	int RealLevelUpdate();


	bool GameOver = false;
	bool GamePaused = false;
	bool loading = false;
	int nCurrentScene = 0;

private:
	TMaterialOptimized matOpt;
	bool drawtext = true;
	float scale = 0;
	XMMATRIX CameraNewPosition;
	XMMATRIX secondCam;
	TWorld tThisWorld;
	CCollisionSystem * pcCollisionSystem;
	CGraphicsSystem	*pcGraphicsSystem;
	CInputSystem	*pcInputSystem;
	CAudioSystem    *pcAudioSystem;
	CPhysicsSystem  *pcPhysicsSystem;
	CProjectileSystem* pcProjectileSystem;
	CAnimationSystem* pcAnimationSystem;
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
	float playerDamage = 25;
	float pirateDamage = 18;
	bool options = false;
	bool credits = false;
	bool pauseInit = false;
	bool endInit = false;
	bool fadeOut = false;

	float damage = 0;
	float prevHealth = 0;
	float fallingHealth = 0;

	//ZB Variables
	TCameraToggle tCameraMode;
	XMMATRIX m_d3dWorldMatrix;
	XMMATRIX m_d3dViewMatrix;
	XMMATRIX m_d3dProjectionMatrix;
	XMMATRIX m_d3dPlayerMatrix;

	ID3D11ShaderResourceView* fontTexture = nullptr;
	std::vector<TUIVert*> atUIVertices;
	std::vector<short*> atUIIndices;
	CFPS fpsTimer;
	float clickTime;
	float fadeTime;
	float lerpTime;

	POINT startDragPoint;
	POINT dragPoint;
	POINT hoverPoint;
	POINT clickPoint = { -1, -1 };

	XTime PausedTimer;
	float pausedTimer;
	TCamera *walkCamera;
	TCamera *aimCamera;
	TCamera *debugCamera;
	TCamera * menuCamera;

	System_Times * tAugerTimers;
	System_Times *tTimerInfo;

	AkBankID init_bnkID;
	AkGameObjectID Listener;
	AkGameObjectID m_AkMainMenuMusic;
	AkBankID MainMenu_bnkID;
	AkGameObjectID m_AkHallwayBattle;
	//SFX Objects
	AkGameObjectID m_MetalReload;
	AkBankID m_SFX_bnkID;
	AkGameObjectID m_AkMetalFired;

	AKRESULT ErrorResult;

	float m_RealTimeFov;
	bool bMoving;
	float Health = 1.0f;
	bool mouseDown = false;
	bool mouseUp = true;
	bool click = false;
	XTime frameLock;
};
