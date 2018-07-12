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

	GInput * m_pcMyInput;

private:
	float				m_fMouseRotationSpeed;
	float				m_fMouseMovementSpeed;
};

