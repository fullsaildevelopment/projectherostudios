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
#include<thread>
#include<mutex>
#include <iostream>
#include <utility>
#include <thread>
#include <chrono>

using namespace std;


#pragma comment(lib, "MSIMG32.lib")

class CGameMangerSystem {
public:

	CGameMangerSystem(HWND window, CInputSystem* pcInputSystem);
	~CGameMangerSystem();
	int LoadMainMenu();
	void InitializeMainMenu();
	int LoadTitleScreen();
	void InitializeTitleScreen();
	void CleanStory();
	int LoadStory();
	void InitializeStory();
	void CleanLoadingScreen();
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
	void ResetLevel();

	//XMMATRIX DoorEventListener(int shaderID);
	//const int DoorEventChanger(int shaderID);

	bool GameOver = false;
	bool GamePaused = false;
	bool loading = false;
	int nCurrentScene = 0;

	

private:
	std::vector<std::thread> workers;
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
	ImporterData bulletMesh;
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
	bool fadeIn = true;
	bool mouseDown = false;
	bool mouseUp = true;
	bool click = false;

	float prevHealth = 0;
	float fallingHealth = 0;
	float hitmarkerTime = 1;

	int loadingImage = 0;
	int loadingImageIndex = -1;
	int loadingTextIndex = -1;

	int fpsIndex = -1;
	int objLogoIndex = -1;
	int hitmarkerIndex = -1;
	int ammoIndex = -1;
	int masterIndex = -1;
	int musicIndex = -1;
	int fxIndex = -1;
	int dialogueIndex = -1;

	int claytonFrustumIndex = -1;

#define SAVE_INDEX 96
#define LOAD_INDEX 97
#define OPTIONS_INDEX 98
#define CREDITS_INDEX 99
#define TIMEUNTILCLICK 0.25

	ID3D11ShaderResourceView* fontTexture = nullptr;
	std::vector<TUIVert*> atUIVertices;
	std::vector<short*> atUIIndices;
	CFPS fpsTimer;
	float clickTime;
	float fadeTime;
	float lerpTime;
	float blinkTime;
	float timeOutTime;

	POINT startDragPoint;
	POINT dragPoint;
	POINT hoverPoint;
	POINT clickPoint = { -1, -1 };

	BulletInfo bulletToCopyFrom;
	EnemyInfo enemyToCopyFrom;

	//ZB Variables
	TCameraToggle tCameraMode;
	XMMATRIX m_d3dWorldMatrix;
	XMMATRIX m_d3dViewMatrix;
	XMMATRIX m_d3dProjectionMatrix;
	XMMATRIX m_d3dPlayerMatrix;

	std::vector<TPrimalVert> atBeamVerts;

	XTime PausedTimer;
	float pausedTimer;
	TCamera *walkCamera;
	TCamera *aimCamera;
	TCamera *debugCamera;
	TCamera * menuCamera;

	System_Times * tAugerTimers;
	System_Times *tTimerInfo;

	float m_fMasterVolume;
	float m_fMusicVolume;
	float m_fSFXVolume;
	float m_fDialogueVolume;
	AKRESULT ErrorResult;

	bool makeBeamBuffer;
	int ExtractionBeamIndex;
	float m_RealTimeFov;
	bool bMoving;
	float Health = 1.0f;
	XTime frameLock;
};
