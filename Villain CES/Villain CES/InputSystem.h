#pragma once
#include "Entity.h"
#include "stdafx.h"
#include "Input_Component.h"
#include"Gateware Redistribution R5B/Interface/G_System/GInput.h"
#include"Gateware Redistribution R5B/Interface/G_System/GKeyDefines.h"
using namespace GW;
using namespace SYSTEM;

class CInputSystem
{

	
public:
	CInputSystem();
	~CInputSystem();

	GReturn InitializeGInput(HWND cTheWindow);

	/*
		Test Specific Functions
	*/

	/*
	* DebugCamera(): This fuction checks what keys are being pressed and modifing a camera's trasforms according to keys pressed.
	*
	* Ins:
	*                   XMMATRIX View
	*					XMMATRIX World
	* Outs:
	*
	* Returns:          XMMATRIX
	*
	* Mod. Date:              07/12/2018
	* Mod. Initials:          MSM
	*/

	XMMATRIX DebugCamera(XMMATRIX d3d_ViewM, XMMATRIX d3d_WorldM);

	/*
	* WalkCamera(): This fuction makes a camera follow an in-game object.
	*
	* Ins:
	*                 XMMATRIX playerMatrix
					   
	* Outs:
	*
	* Returns:          XMMATRIX
	*
	* Mod. Date:              07/18/2018
	* Mod. Initials:          ZFB
	*/
	XMMATRIX CubeMovement( XMMATRIX d3dplayerMatrix);
	/*
	* AimMode(): This fuction makes a camera rotation locked to player rotation for combat.
	*
	* Ins:
	*              XMMATRIX  d3dplayerMatrix

	* Outs:
	*
	* Returns:          XMMATRIX
	*
	* Mod. Date:              07/16/2018
	* Mod. Initials:          ZFB
	*/
	XMMATRIX AimMode(XMMATRIX d3dplayerMatrix);

	XMMATRIX WalkCameraControls(XMVECTOR U, XMMATRIX viewM);

	float ZoomSight(float fFov);



	TCamera CameraInit(TCamera camera);
	TCamera FreeMoveInit(float height, float characterDist, float pitch);
	TCamera FreeCamControls(float moveRadius, float upDistance, float downDistance);
	/*
		Seth Specific Functions
	*/

	/*
		Clayton Specific Functions
	*/

	/*
		Seth Specific Functions
	*/

	/*
		Caelis Specific Functions
	*/

	int InputCheck(int GInput);

	TCameraToggle CameraModeListen(TCameraToggle tMyCam);

	GInput * m_pcMyInput;

private:
	float				m_fMouseRotationSpeed;
	float				m_fMouseMovementSpeed;
	float				m_fDistance;
};

