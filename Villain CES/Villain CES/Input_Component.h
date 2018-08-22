#pragma once
#ifndef CES_INPUT_COMPONENT_H
#define CES_INPUT_COMPONENT_H
enum eInput_Component
{
	COMPONENT_INPUTMASK = 1 << 0,
	COMPONENT_TEST		= 1 << 1,
	COMPONENT_CLAYTON	= 1 << 2,
	COMPONENT_SETH		= 1 << 3,
	COMPONENT_CAELIS	= 1 << 4
};

struct TInputMask
{
	int m_tnInputMask = 0;
};

struct TTest
{
	float m_tfMovementSpeed = 1.0;
	float m_tfLookSensitivity = 0.01f;
};

struct TClayton
{
	/*
	Fill out this structure
	*/
	float health = 100;
	float m_tfMovementSpeed = 1.0;
	float m_tfLookSensitivity = 0.01f;
	float m_tfMaterialLoadTime = 0.0f;
	float m_tfMaterialLoadCooldown = 0.0f;
	float m_tfReloadTime = 0.0f;
};

struct TSeth
{
	/*
	Fill out this structure
	*/
	float m_fMovementSpeed = 1.0;
	float m_fLookSensitivity = 0.01f;
	float m_fSpecialCooldown = 0.0f;
	float m_fAttackCooldown = 0.0f;
};

struct TCaelis
{
	/*
	Fill out this structure
	*/
	float m_tfMovementSpeed = 1.0;
	float m_tfLookSensitivity = 0.01f;
	float m_tfSpecialCooldown = 0.0f;
	float m_tfAttackCooldown = 0.0f;
	float m_tfFlightSpeed = 0.0f;
};

struct TCamera
{
public:

	XMMATRIX d3d_Position;
	float fPitch;
	float fYaw;
	float fRoll;

	XMVECTOR getCameraPOS()
	{
		return d3d_Position.r[3];
	}

	float getPitch()
	{
		return fPitch;
	}

	float getYaw()
	{
		return fYaw;
	}

	float getRoll()
	{
		return fRoll;
	}


	float SetPitch(float pitch)
	{
		fPitch = pitch;
		return fPitch;
	}

	float SetRoll(float roll)
	{
		fRoll = roll;
		return fRoll;
	}

};
struct TCameraToggle
{
	bool bDebugMode;
	bool bWalkMode;
	bool bAimMode=true;
	bool bSwitch;
	//bool bDebugMode;
};
#endif
