#pragma once
//#include "Entity.h"
#include "stdafx.h"
#include "Input_Component.h"
#include "AudioSystem.h"
#include"Gateware Redistribution R5B/Interface/G_System/GInput.h"
#include"Gateware Redistribution R5B/Interface/G_System/GKeyDefines.h"
using namespace GW;
using namespace SYSTEM;
class CInputSystem
{
public:
	CInputSystem();
	~CInputSystem();
	float stepCount;
	/*
	*/

	void gameManagerCodeAbstracted(
		const int nButtonLeft, const int nButtonMiddle, const int nKeyP, const int nKeyU, const int nKeyR,
		const HWND cApplicationWindow, const XMMATRIX d3dResetAimModeCameraOffset,
		bool &bGunMode, bool &bTryToShoot, bool &bTryToReload,
		bool &bMouseUp, bool &bMouseDown, bool &bClick,
		bool &bGamePaused, bool &bGameOver, bool &bPauseInit, bool &bOptions,
		bool &bNoMoving,
		float &fRealTimeFov,
		POINT &cStartDragPoint, POINT &cDragPoint, POINT &cHoverPoint, POINT &cClickPoint,
		TCameraToggle &tCameraMode,
		TCamera* tWalkCamera, TCamera* tAimCamera, TCamera* tDebugCamera,
		XMMATRIX &d3dResultMatrix, XMMATRIX &d3dPlayerMatrix, XMMATRIX &d3dOffsetMatrix, XMMATRIX &d3dWorldMatrix,
		XMMATRIX &tMyViewMatrix, XMMATRIX &tTempViewMatrix,
		XMFLOAT4 &d3dCollisionColor, double &delta, CAudioSystem* in_Audio, TClayton &clayton, XMVECTOR &playerVeclocity);
	GReturn InitializeGInput(HWND cTheWindow);																																																													
	

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

	XMMATRIX DebugCamera(XMMATRIX d3d_ViewM, XMMATRIX d3d_WorldM, double delta);

	XMMATRIX CharacterMovement(XMMATRIX d3dplayerMatrix, double delta, CAudioSystem* in_Audio, TClayton &clayton, XMVECTOR &playerVeclocity, bool move);

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
	//XMMATRIX AimMode(XMMATRIX d3dplayerMatrix);

	/*
	* AimMode(): This fuction does camera rotation on Yaw (Y Axis) & Pitch(X Axis) for combat.
	*
	* Ins:
	*              TCamera* in_AimCamera
	*              XMMATRIX  d3dplayerMatriX

	* Outs: N/A
	*
	* Returns:          XMMATRIX
	*
	* Mod. Date:              09/18/2018
	* Mod. Initials:          ZFB
	*/
	
	XMMATRIX AimMode(TCamera * in_AimCamera, XMMATRIX d3dplayerMatrix, double delta, bool move);
	XMMATRIX WalkCameraControls(XMVECTOR U, XMMATRIX viewM, bool &_movement, double delta);
	XMMATRIX CameraUpdate(TCamera* in_walkCamera, TCamera* in_aimMode, TCamera* in_debugCamera, XMMATRIX in_resultMatrix, XMMATRIX offsetMatrix);
	XMMATRIX CameraBehaviorLerp(XMMATRIX m1, XMMATRIX m2, float scale);
	XMMATRIX MyLookAt(XMVECTOR EyePos, XMVECTOR TargetPos, XMVECTOR Up);
	void StoreInitCameraForwardV(XMVECTOR in_prevPos);
	void StorePreviousCameraPos(XMVECTOR in_Pos);
	void GetMousePosition();
	XMMATRIX CameraOrientationReset(XMMATRIX m1);
	XMMATRIX MyTurnTo(XMMATRIX M, XMVECTOR T, float s, XMMATRIX world);
	XMMATRIX CharacterSwitch(int characterIndex);
/*	
	*AimMode() : This fuction does camera rotation on Yaw(Y Axis) & Pitch(X Axis) for combat.
		*
		* Ins :
		*float fFov

		* Outs : N / A
		*
		* Returns : XMMATRIX
		*
		* Mod.Date : 09 / 18 / 2018
		* Mod.Initials:          ZFB
		
*/
	float ZoomSight(float fFov);
	void MouseBoundryCheck(float _x, float _y, float &_outX,float &_outY);
	void SendHWNDToInputSystem(HWND in_WindowHandle);
	
	float GetMouseRotationSpeed();
	void SetMouseRotationSpeed(float newSpeed);
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
	float fXchange;
	float fYchange;
	float prev_X;
	float prev_Y;
	float fXEnd;
	float fYEnd;
	XMVECTOR m_PlayerForwardV;
	XMVECTOR m_CameraInitV;
	XMMATRIX m_prevCameraPos;
    float m_YRotationLimit;
	HWND m_GameWindow;
	RECT m_windowBounds;
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

