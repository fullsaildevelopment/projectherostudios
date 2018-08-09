#pragma once
#include "Graphics_System.h"
#include "InputSystem.h"
#include"Collision_System.h"
#include"Physics_System.h"
#include "Entity.h"
#include"ProjectileSystem.h"
#include"AI_System.h"
#include "UI_System.h"
class CAuger
{
public:
	HWND cApplicationWindow;
	CCollisionSystem* pcCollisionSystem;
	TWorld tThisWorld;
	CGraphicsSystem	*pcGraphicsSystem; 
	CInputSystem	*pcInputSystem;
	CPhysicsSystem  *pcPhysicsSystem;
	CProjectileSystem* pcProjectileSystem;
	CAISystem		*pcAiSystem;
	CUISystem		*pcUiSystem;






	CAuger();
	CAuger(HWND cTheWindow);

	~CAuger();

	void Start();
	void InitializeSystems();
	void Update();
	void End();
private:
	XMMATRIX m_d3dWorldMatrix;
	XMMATRIX m_d3dViewMatrix;
	XMMATRIX m_d3dProjectionMatrix;
	XMMATRIX m_d3dPlayerMatrix;
	XMMATRIX m_d3dCameraMatrix;
    XMMATRIX m_d3d_ResultMatrix;

	int PlayerStartIndex = -10;
	int GunIndexForPlayer = -10;
	int rayindex = -10;
	float zValue=5;

	bool paused = false;
	bool firstCheck = false;

	TCameraToggle tCameraMode;
	
};

