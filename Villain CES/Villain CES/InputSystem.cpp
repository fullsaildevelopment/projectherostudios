#include "InputSystem.h"

CInputSystem::CInputSystem()
{
	m_fMouseRotationSpeed = .003f;//Frame Dependent
	m_fMouseMovementSpeed = .01f;//Frame Dependent
}


CInputSystem::~CInputSystem()
{
	m_pcMyInput->DecrementCount();
}

int CInputSystem::InputCheck(int GInput)
{
	float fInputReturnValue = -1;
	m_pcMyInput->GetState(GInput, fInputReturnValue);
	return (int)fInputReturnValue;
}

TCameraToggle CInputSystem::CameraModeListen(TCameraToggle tMyCam)
{
	TCameraToggle tTempCamMode = tMyCam;
	if (InputCheck(G_KEY_9) == 1)
	{
		tTempCamMode.bDebugMode = true;
		tTempCamMode.bWalkMode = false;
		tTempCamMode.bAimMode = false;
		tTempCamMode.bSwitch = true;

	}

	if (InputCheck(G_KEY_8) == 1)
	{
		tTempCamMode.bWalkMode = true;
		tTempCamMode.bDebugMode = false;
		tTempCamMode.bAimMode = false;
		tTempCamMode.bSwitch = true;

	}
	if (InputCheck(G_BUTTON_RIGHT) == 1)
	{
		tTempCamMode.bAimMode = true;
		tTempCamMode.bDebugMode = false;
		tTempCamMode.bWalkMode = false;
		tTempCamMode.bSwitch = true;

	}

	return tTempCamMode;
}

GReturn CInputSystem::InitializeGInput(HWND cTheWindow)
{
	// returns the results of function
	return CreateGInput(cTheWindow, sizeof(cTheWindow), &m_pcMyInput);
}

XMMATRIX CInputSystem::DebugCamera(XMMATRIX d3d_ViewM, XMMATRIX d3d_WorldM)
{
	XMMATRIX d3dTmpViewM, d3dMovementM, d3dRotation;
	float fXchange = 0, fYchange = 0, fXEnd = 0, fYEnd = 0;

	//m_d3dTmpWorldM.r[0].m128_f32[0] = 0;


	d3dTmpViewM = d3d_ViewM;

	XMVECTOR d3d_newX, d3d_newY, d3d_existingZ;
	m_pcMyInput->GetMouseDelta(fXchange, fYchange);

	m_pcMyInput->GetMousePosition(fXEnd, fYEnd);


	//Forward && Back Movement

	// up key movement

	if (InputCheck(G_KEY_W) == 1) {
		d3dMovementM = XMMatrixTranslation(0, 0, m_fMouseMovementSpeed);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	// down key movement
	if (InputCheck(G_KEY_S) == 1) {
		d3dMovementM = XMMatrixTranslation(0, 0, -m_fMouseMovementSpeed);

		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	// left key movement
	if (InputCheck(G_KEY_A) == 1) {
		d3dMovementM = XMMatrixTranslation(-m_fMouseMovementSpeed, 0, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	// right key movement
	if (InputCheck(G_KEY_D) == 1) {
		d3dMovementM = XMMatrixTranslation(m_fMouseMovementSpeed, 0, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	if (InputCheck(G_KEY_SPACE) == 1) {
		d3dMovementM = XMMatrixTranslation(0, m_fMouseMovementSpeed, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	if (InputCheck(G_KEY_DOWN) == 1) {
		d3dMovementM = XMMatrixTranslation(0, -m_fMouseMovementSpeed, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);
	}

	d3dRotation = XMMatrixRotationY(fXchange * m_fMouseRotationSpeed);

	d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);

	d3d_existingZ = d3dTmpViewM.r[2];
	d3d_newX = XMVector3Cross(XMVectorSet(0, 1, 0, 0), d3d_existingZ);
	d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

	d3d_newX = XMVector3Normalize(d3d_newX);
	d3d_newY = XMVector3Normalize(d3d_newY);

	d3d_existingZ = XMVector3Normalize(d3d_existingZ);

	d3dTmpViewM.r[0] = d3d_newX;
	d3dTmpViewM.r[1] = d3d_newY;
	d3dTmpViewM.r[2] = d3d_existingZ;
	d3dRotation = XMMatrixRotationX(fYchange * m_fMouseRotationSpeed);

	d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);


	d3d_existingZ = d3dTmpViewM.r[2];
	d3d_newX = XMVector3Cross(XMVectorSet(0, 1, 0, 0), d3d_existingZ);
	d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

	d3d_newX = XMVector3Normalize(d3d_newX);
	d3d_newY = XMVector3Normalize(d3d_newY);

	d3d_existingZ = XMVector3Normalize(d3d_existingZ);

	d3dTmpViewM.r[0] = d3d_newX;
	d3dTmpViewM.r[1] = d3d_newY;
	d3dTmpViewM.r[2] = d3d_existingZ;

	return d3dTmpViewM;
}

XMMATRIX CInputSystem::CharacterMovement(XMMATRIX d3dplayerMatrix)
{

	XMMATRIX d3dTmpViewM, d3dMovementM;



	d3dTmpViewM = d3dplayerMatrix;

	//Forward && Back Movement

	// up key movement

	if (InputCheck(G_KEY_W) == 1) {
		d3dMovementM = XMMatrixTranslation(0, 0, m_fMouseMovementSpeed);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	// down key movement
	if (InputCheck(G_KEY_S) == 1) {
		d3dMovementM = XMMatrixTranslation(0, 0, -m_fMouseMovementSpeed);

		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	// left key movement
	if (InputCheck(G_KEY_A) == 1) {
		d3dMovementM = XMMatrixTranslation(-m_fMouseMovementSpeed, 0, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	// right key movement
	if (InputCheck(G_KEY_D) == 1) {
		d3dMovementM = XMMatrixTranslation(m_fMouseMovementSpeed, 0, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	if (InputCheck(G_KEY_SPACE) == 1) {
		d3dMovementM = XMMatrixTranslation(0, m_fMouseMovementSpeed, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}

	return d3dTmpViewM;
}

XMMATRIX CInputSystem::AimMode(XMMATRIX d3dplayerMatrix)
{
	XMMATRIX d3dTmpViewM, d3dMovementM, d3dRotation;
	float fXchange = 0, fYchange = 0, fXEnd = 0, fYEnd = 0;

	//m_d3dTmpWorldM.r[0].m128_f32[0] = 0;


	d3dTmpViewM = d3dplayerMatrix;

	XMVECTOR d3d_newX, d3d_newY, d3d_existingZ;
	m_pcMyInput->GetMouseDelta(fXchange, fYchange);

	m_pcMyInput->GetMousePosition(fXEnd, fYEnd);


	//Forward && Back Movement

	// up key movement

	if (InputCheck(G_KEY_W) == 1) {
		d3dMovementM = XMMatrixTranslation(0, 0, m_fMouseMovementSpeed);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	// down key movement
	if (InputCheck(G_KEY_S) == 1) {
		d3dMovementM = XMMatrixTranslation(0, 0, -m_fMouseMovementSpeed);

		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	// left key movement
	if (InputCheck(G_KEY_A) == 1) {
		d3dMovementM = XMMatrixTranslation(-m_fMouseMovementSpeed, 0, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	// right key movement
	if (InputCheck(G_KEY_D) == 1) {
		d3dMovementM = XMMatrixTranslation(m_fMouseMovementSpeed, 0, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	if (InputCheck(G_KEY_SPACE) == 1) {
		d3dMovementM = XMMatrixTranslation(0, m_fMouseMovementSpeed, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	/*if (InputCheck(G_KEY_S) == 1) {
		d3dMovementM = XMMatrixTranslation(0, -m_fMouseMovementSpeed, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);
	}*/

	//Up && Down Rotation(keybord implemented, soon to be changed in the mouse)

	//Right && Left Rotation(keybord implemented, soon to be changed in the mouse)
	if (fXchange < 0 && fYchange < 0)
	{
		d3dRotation = XMMatrixRotationY(fXchange * m_fMouseRotationSpeed);

		d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);

		d3d_existingZ = d3dTmpViewM.r[2];
		d3d_newX = XMVector3Cross(d3dplayerMatrix.r[1], d3d_existingZ);
		d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

		d3d_newX = XMVector3Normalize(d3d_newX);
		d3d_newY = XMVector3Normalize(d3d_newY);

		d3d_existingZ = XMVector3Normalize(d3d_existingZ);

		d3dTmpViewM.r[0] = d3d_newX;
		d3dTmpViewM.r[1] = d3d_newY;
		d3dTmpViewM.r[2] = d3d_existingZ;
		d3dRotation = XMMatrixRotationX(fYchange * m_fMouseRotationSpeed);

		d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);

	}

	if (fXchange > 0 && fYchange < 0)
	{
		d3dRotation = XMMatrixRotationY(fXchange * m_fMouseRotationSpeed);

		d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);

		d3d_existingZ = d3dTmpViewM.r[2];
		d3d_newX = XMVector3Cross(XMVectorSet(0,1,0,0), d3d_existingZ);
		d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

		d3d_newX = XMVector3Normalize(d3d_newX);
		d3d_newY = XMVector3Normalize(d3d_newY);

		d3d_existingZ = XMVector3Normalize(d3d_existingZ);

		d3dTmpViewM.r[0] = d3d_newX;
		d3dTmpViewM.r[1] = d3d_newY;
		d3dTmpViewM.r[2] = d3d_existingZ;
		d3dRotation = XMMatrixRotationX(fYchange * m_fMouseRotationSpeed);

		d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);

	}

	if (fXchange < 0 && fYchange > 0)
	{
		d3dRotation = XMMatrixRotationY(fXchange * m_fMouseRotationSpeed);

		d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);

		d3d_existingZ = d3dTmpViewM.r[2];
		d3d_newX = XMVector3Cross(XMVectorSet(0, 1, 0, 0), d3d_existingZ);
		d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

		d3d_newX = XMVector3Normalize(d3d_newX);
		d3d_newY = XMVector3Normalize(d3d_newY);

		d3d_existingZ = XMVector3Normalize(d3d_existingZ);

		d3dTmpViewM.r[0] = d3d_newX;
		d3dTmpViewM.r[1] = d3d_newY;
		d3dTmpViewM.r[2] = d3d_existingZ;
		d3dRotation = XMMatrixRotationX(fYchange * m_fMouseRotationSpeed);

		d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);

	}
	if (fXchange > 0 && fYchange > 0)
	{
		d3dRotation = XMMatrixRotationY(fXchange * m_fMouseRotationSpeed);

		d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);
		d3d_existingZ = d3dTmpViewM.r[2];
		d3d_newX = XMVector3Cross(XMVectorSet(0, 1, 0, 0), d3d_existingZ);
		d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

		d3d_newX = XMVector3Normalize(d3d_newX);
		d3d_newY = XMVector3Normalize(d3d_newY);

		d3d_existingZ = XMVector3Normalize(d3d_existingZ);

		d3dTmpViewM.r[0] = d3d_newX;
		d3dTmpViewM.r[1] = d3d_newY;
		d3dTmpViewM.r[2] = d3d_existingZ;

		d3dRotation = XMMatrixRotationX(fYchange * m_fMouseRotationSpeed);
		d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);

	}

	return d3dTmpViewM;
}


XMMATRIX CInputSystem::WalkCameraControls(XMVECTOR U, XMMATRIX viewM) {
	//Orbit Camera 
	//XMVECTOR X, Y, Z, X2, Y2, Z2, W;
	XMVECTOR d3d_newX, d3d_newY, d3d_existingZ;
	XMMATRIX d3dTmpViewM, d3dRotation;
	float fXchange = 0, fYchange = 0, fXEnd = 0, fYEnd = 0;

	d3dTmpViewM = viewM;
	m_pcMyInput->GetMouseDelta(fXchange, fYchange);
	if (fXchange == 0) {
		float x = 0;
	}

	m_pcMyInput->GetMousePosition(fXEnd, fYEnd);

	
	d3dRotation = XMMatrixRotationY(fXchange * m_fMouseRotationSpeed);

	d3dTmpViewM = XMMatrixMultiply(d3dTmpViewM, d3dRotation);

	d3d_existingZ = d3dTmpViewM.r[2];
	d3d_newX = XMVector3Cross(XMVectorSet(0, 1, 0, 0), d3d_existingZ);
	d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

	d3d_newX = XMVector3Normalize(d3d_newX);
	d3d_newY = XMVector3Normalize(d3d_newY);

	d3d_existingZ = XMVector3Normalize(d3d_existingZ);

	d3dTmpViewM.r[0] = d3d_newX;
	d3dTmpViewM.r[1] = d3d_newY;
	d3dTmpViewM.r[2] = d3d_existingZ;
	d3dRotation = XMMatrixRotationX(fYchange * m_fMouseRotationSpeed);

	d3dTmpViewM = XMMatrixMultiply(d3dTmpViewM, d3dRotation);


	d3d_existingZ = d3dTmpViewM.r[2];
	d3d_newX = XMVector3Cross(XMVectorSet(0, 1, 0, 0), d3d_existingZ);
	d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

	d3d_newX = XMVector3Normalize(d3d_newX);
	d3d_newY = XMVector3Normalize(d3d_newY);

	d3d_existingZ = XMVector3Normalize(d3d_existingZ);

	d3dTmpViewM.r[0] = d3d_newX;
	d3dTmpViewM.r[1] = d3d_newY;
	d3dTmpViewM.r[2] = d3d_existingZ;


	return d3dTmpViewM;
}

XMMATRIX CInputSystem::CameraBehaviorLerp(XMMATRIX m1, XMMATRIX m2)
{
	XMMATRIX lerpedMatrix;

	lerpedMatrix.r[0] = (m2.r[0] - m1.r[0]) * 0.1f + m1.r[0];
	lerpedMatrix.r[1] = (m2.r[1] - m1.r[1]) * 0.1f + m1.r[1];
	lerpedMatrix.r[2] = (m2.r[2] - m1.r[2]) * 0.1f + m1.r[2];
	lerpedMatrix.r[3] = (m2.r[3] - m1.r[3]) * 0.1f + m1.r[3];



	return lerpedMatrix;
}
XMMATRIX CInputSystem::CameraOrientationReset(XMMATRIX m1)
{
	XMMATRIX tmpMatrix = XMMatrixIdentity();

	tmpMatrix.r[3] = XMVectorSet(1.0f, 0, -10.0f, 1.0f);
	return tmpMatrix;
}
float CInputSystem::ZoomSight(float fFov)
{
	float ftmpFov = fFov;

	if (InputCheck(G_BUTTON_RIGHT) == 1)
	{
		if (ftmpFov <= 90.0f && ftmpFov >= 120.0f)
		{
			if (ftmpFov > 120.f)
			{
				ftmpFov = 120.0f;
			}
			else if (ftmpFov < 90.0f)
			{
				ftmpFov = 90.0f;
			}
			else {
			ftmpFov += 0.1f;
			}

			
		}

	}
	else
	{
		if (ftmpFov <= 90.0f && ftmpFov >= 120.0f)
		{
			ftmpFov -= 0.1f;
		}
		else if (ftmpFov >= 120.f)
		{
			ftmpFov = 120.0f;
		}
		else if (ftmpFov < 90.0f)
		{
			ftmpFov = 90.0f;
		}
	}
	return ftmpFov;
}