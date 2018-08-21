#pragma once
//#include "Entity.h"
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

	XMMATRIX CharacterMovement(XMMATRIX d3dplayerMatrix);

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

	XMMATRIX WalkCameraControls(XMVECTOR U, XMMATRIX viewM, bool &_movement);
	XMMATRIX CameraBehaviorLerp(XMMATRIX m1, XMMATRIX m2, float scale);
	XMMATRIX CameraOrientationReset(XMMATRIX m1);
	XMMATRIX MyTurnTo(XMMATRIX M, XMVECTOR T, float s, XMMATRIX world);
	float ZoomSight(float fFov);
	void MouseBoundryCheck(float _x, float _y, float &_outX,float &_outY);

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
	float lerp(float v0, float v1, float t) {
		return (1 - t) * v0 + t * v1;
	}
	float mapValue(float mainValue, float inValueMin, float inValueMax, float outValueMin, float outValueMax)
	{
		return (mainValue - inValueMin) * (outValueMax - outValueMin) / (inValueMax - inValueMin) + outValueMin;
	}
	float CalcualteDistance(XMVECTOR  x, XMVECTOR y)
	{
		return sqrtf(
			((x.m128_f32[0] - y.m128_f32[0])*(x.m128_f32[0] - y.m128_f32[0])) +
			((x.m128_f32[1] - y.m128_f32[1])*(x.m128_f32[1] - y.m128_f32[1])) +
			((x.m128_f32[2] - y.m128_f32[2])*(x.m128_f32[2] - y.m128_f32[2]))
		);
	}
	 XMVECTOR lerp(const XMVECTOR A, const XMVECTOR B, float t) {
		return A * t + B * (1.f - t);
	}
};

