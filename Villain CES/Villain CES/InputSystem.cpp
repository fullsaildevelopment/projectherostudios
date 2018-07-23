#include "InputSystem.h"

CInputSystem::CInputSystem()
{
	m_fMouseRotationSpeed = .0001f;
	m_fMouseMovementSpeed = .01f;
}


CInputSystem::~CInputSystem()
{
	m_pcMyInput->DecrementCount();
}

int CInputSystem::InputCheck(int GInput)
{
	float fInputReturnValue = -1;
	m_pcMyInput->GetState(GInput, fInputReturnValue);
	return fInputReturnValue;
}

TCameraToggle CInputSystem::CameraModeListen(TCameraToggle tMyCam)
{	
	TCameraToggle tTempCamMode = tMyCam;
	if (InputCheck(G_KEY_9) == 1)
	{
		tTempCamMode.bDebugMode = true;
		tTempCamMode.bWalkMode = false;
		tTempCamMode.bAimMode = false;
	}

	if (InputCheck(G_KEY_8) == 1)
	{
		tTempCamMode.bWalkMode = true;
		tTempCamMode.bDebugMode = false;
		tTempCamMode.bAimMode = false;

	}
	if (InputCheck(G_KEY_7) == 1)
	{
		tTempCamMode.bAimMode = true;
		tTempCamMode.bDebugMode = false;
		tTempCamMode.bWalkMode = false;
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

	if (InputCheck(G_BUTTON_LEFT) == 1) {
		d3dMovementM = XMMatrixTranslation(0, 0, m_fMouseMovementSpeed);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	// down key movement
	if (InputCheck(G_BUTTON_RIGHT) == 1) {
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
	if (InputCheck(G_KEY_W) == 1) {
		d3dMovementM = XMMatrixTranslation(0, m_fMouseMovementSpeed, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	if (InputCheck(G_KEY_S) == 1) {
		d3dMovementM = XMMatrixTranslation(0, -m_fMouseMovementSpeed, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);
	}

	//Up && Down Rotation(keybord implemented, soon to be changed in the mouse)

	//Right && Left Rotation(keybord implemented, soon to be changed in the mouse)
	if (fXchange < 0 && fYchange < 0)
	{
		d3dRotation = XMMatrixRotationY(fXchange * m_fMouseRotationSpeed);

		d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);

		d3d_existingZ = d3dTmpViewM.r[2];
		d3d_newX = XMVector3Cross(d3d_WorldM.r[1], d3d_existingZ);
		d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

		d3d_newX = XMVector3Normalize(d3d_newX);
		d3d_newY = XMVector3Normalize(d3d_newY);

		d3d_existingZ = XMVector3Normalize(d3d_existingZ);

		d3dTmpViewM.r[0] = d3d_newX;
		d3dTmpViewM.r[1] = d3d_newY;
		d3dTmpViewM.r[2] = d3d_existingZ;
		

	}

	if (fXchange > 0 && fYchange < 0)
	{
		d3dRotation = XMMatrixRotationY(fXchange * m_fMouseRotationSpeed);

		d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);

		d3d_existingZ = d3dTmpViewM.r[2];
		d3d_newX = XMVector3Cross(d3d_WorldM.r[1], d3d_existingZ);
		d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

		d3d_newX = XMVector3Normalize(d3d_newX);
		d3d_newY = XMVector3Normalize(d3d_newY);

		d3d_existingZ = XMVector3Normalize(d3d_existingZ);

		d3dTmpViewM.r[0] = d3d_newX;
		d3dTmpViewM.r[1] = d3d_newY;
		d3dTmpViewM.r[2] = d3d_existingZ;
		

	}

	if (fXchange < 0 && fYchange > 0)
	{
		d3dRotation = XMMatrixRotationY(fXchange * m_fMouseRotationSpeed);

		d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);

		d3d_existingZ = d3dTmpViewM.r[2];
		d3d_newX = XMVector3Cross(d3d_WorldM.r[1], d3d_existingZ);
		d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

		d3d_newX = XMVector3Normalize(d3d_newX);
		d3d_newY = XMVector3Normalize(d3d_newY);

		d3d_existingZ = XMVector3Normalize(d3d_existingZ);

		d3dTmpViewM.r[0] = d3d_newX;
		d3dTmpViewM.r[1] = d3d_newY;
		d3dTmpViewM.r[2] = d3d_existingZ;
		

	}
	if (fXchange > 0 && fYchange > 0)
	{
		d3dRotation = XMMatrixRotationY(fXchange * m_fMouseRotationSpeed);

		d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);
		d3d_existingZ = d3dTmpViewM.r[2];
		d3d_newX = XMVector3Cross(d3d_WorldM.r[1], d3d_existingZ);
		d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

		d3d_newX = XMVector3Normalize(d3d_newX);
		d3d_newY = XMVector3Normalize(d3d_newY);

		d3d_existingZ = XMVector3Normalize(d3d_existingZ);

		d3dTmpViewM.r[0] = d3d_newX;
		d3dTmpViewM.r[1] = d3d_newY;
		d3dTmpViewM.r[2] = d3d_existingZ;

		

	}

	return d3dTmpViewM;
}

XMMATRIX CInputSystem::WalkCamera(XMMATRIX d3dplayerMatrix)
{

	XMMATRIX d3dTmpViewM, d3dMovementM, d3dRotation;
	//float fXchange = 0, fYchange = 0, fXEnd = 0, fYEnd = 0;

	//m_d3dTmpWorldM.r[0].m128_f32[0] = 0;


	d3dTmpViewM = d3dplayerMatrix;

	XMVECTOR d3d_newX, d3d_newY, d3d_existingZ;
	//m_pcMyInput->GetMouseDelta(fXchange, fYchange);

	//m_pcMyInput->GetMousePosition(fXEnd, fYEnd);


	//Forward && Back Movement

	// up key movement

	if (InputCheck(G_BUTTON_LEFT) == 1) {
		d3dMovementM = XMMatrixTranslation(0, 0, m_fMouseMovementSpeed);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	// down key movement
	if (InputCheck(G_BUTTON_RIGHT) == 1) {
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
	if (InputCheck(G_KEY_W) == 1) {
		d3dMovementM = XMMatrixTranslation(0, m_fMouseMovementSpeed, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	if (InputCheck(G_KEY_S) == 1) {
		d3dMovementM = XMMatrixTranslation(0, -m_fMouseMovementSpeed, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);
	}

	//Up && Down Rotation(keybord implemented, soon to be changed in the mouse)

	//Right && Left Rotation(keybord implemented, soon to be changed in the mouse)
	/*if (InputCheck(G_KEY_J) == 1)
	{
		d3dRotation = XMMatrixRotationY(-1.0f * m_fMouseRotationSpeed);

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
		

		d3dTmpViewM = XMMatrixMultiply(d3dTmpViewM, d3dRotation);

	}

	if (InputCheck(G_KEY_L) == 1)
	{
		d3dRotation = XMMatrixRotationY(1.0f * m_fMouseRotationSpeed);

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
		
		d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);

	}

	if (InputCheck(G_KEY_I) == 1)
	{
		d3dRotation = XMMatrixRotationX(1.0f * m_fMouseRotationSpeed);

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
		

		d3dTmpViewM = XMMatrixMultiply(d3dTmpViewM, d3dRotation);


	}
	if (InputCheck(G_KEY_K) == 1)
	{
		d3dRotation = XMMatrixRotationX(-1.0f * m_fMouseRotationSpeed);

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

		
		d3dTmpViewM = XMMatrixMultiply(d3dTmpViewM, d3dRotation);*/


	//}

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

	if (InputCheck(G_BUTTON_LEFT) == 1) {
		d3dMovementM = XMMatrixTranslation(0, 0, m_fMouseMovementSpeed);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	// down key movement
	if (InputCheck(G_BUTTON_RIGHT) == 1) {
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
	if (InputCheck(G_KEY_W) == 1) {
		d3dMovementM = XMMatrixTranslation(0, m_fMouseMovementSpeed, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	if (InputCheck(G_KEY_S) == 1) {
		d3dMovementM = XMMatrixTranslation(0, -m_fMouseMovementSpeed, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);
	}

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


XMMATRIX CInputSystem::WalkLookAt(XMVECTOR U, XMMATRIX viewM) {
	//XMVECTOR X, Y, Z, X2, Y2, Z2, W;
	XMVECTOR d3d_newX, d3d_newY, d3d_existingZ;
	XMMATRIX d3dTmpViewM, d3dMovementM,d3dRotation;
	d3dTmpViewM = viewM;
	//XMMATRIX newV;
	/*W = T;
	W.m128_f32[0] = T.m128_f32[0] - 0.5f;
	W.m128_f32[1] = T.m128_f32[1] + 0.5;
	W.m128_f32[2] = T.m128_f32[2] - 3;
	*/
	/*Z = DirectX::XMVectorSubtract(T, V);
	Z2.m128_f32[0] = T.m128_f32[0] - V.m128_f32[0];
	Z2.m128_f32[1] = T.m128_f32[1] - V.m128_f32[1];
	Z2.m128_f32[2] = T.m128_f32[2] - V.m128_f32[2];
	Z2.m128_f32[3] = 0;

	Z = DirectX::XMVector3Normalize(Z);

	X = DirectX::XMVector3Cross(U, Z);

	X2.m128_f32[0] = (U.m128_f32[1] * Z.m128_f32[2]) - (U.m128_f32[2] * Z.m128_f32[1]);
	X2.m128_f32[1] = (U.m128_f32[2] * Z.m128_f32[0]) - (U.m128_f32[0] * Z.m128_f32[2]);
	X2.m128_f32[2] = (U.m128_f32[0] * Z.m128_f32[1]) - (U.m128_f32[1] * Z.m128_f32[0]);
	X2.m128_f32[3] = 0;

	X = DirectX::XMVector3Normalize(X);

	Y = DirectX::XMVector3Cross(Z, X);
	Y2.m128_f32[0] = (Z.m128_f32[1] * X.m128_f32[2]) - (Z.m128_f32[2] * X.m128_f32[1]);
	Y2.m128_f32[1] = (Z.m128_f32[2] * X.m128_f32[0]) - (Z.m128_f32[0] * X.m128_f32[2]);
	Y2.m128_f32[2] = (Z.m128_f32[0] * X.m128_f32[1]) - (Z.m128_f32[1] * X.m128_f32[0]);
	Y2.m128_f32[3] = 0;
	Y = DirectX::XMVector3Normalize(Y);


	d3dTmpViewM.r[0] = X;
	d3dTmpViewM.r[1] = Y;
	d3dTmpViewM.r[2] = Z; */

	//if (Z2.m128_f32[0] > 10.0f)
	//{
	//	d3dMovementM = XMMatrixTranslation(1, 0, 0);
	//	d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);
	//}
	//if (Z2.m128_f32[0] < -10.0f)
	//{
	//	d3dMovementM = XMMatrixTranslation(1, 0, 0);
	//	d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);
	//}
	//if (Z2.m128_f32[2] > 4.0f)
	//{
	////	/*XMVECTOR zCheck;
	////	zCheck = T;
	////	zCheck.m128_f32[2] = d3dTmpViewM.r[3].m128_f32[2];
	////	zCheck.m128_f32[2] += 0.5f;
	////	m_fDistance = zCheck.m128_f32[2] / d3dTmpViewM.r[3].m128_f32[2];*/
	//	d3dTmpViewM.r[3].m128_f32[2] += 0.1f;



	//}
	//if (Z2.m128_f32[2] < -4.0f)
	//{
	//	/*XMVECTOR zCheck;
	//	zCheck = T;
	//	zCheck.m128_f32[2] = d3dTmpViewM.r[3].m128_f32[2];
	//	zCheck.m128_f32[2] += 0.5f;
	//	m_fDistance = zCheck.m128_f32[2] / d3dTmpViewM.r[3].m128_f32[2];*/
	//	 d3dTmpViewM.r[3].m128_f32[2] -= 0.1f;
	// }
	//if (InputCheck(G_BUTTON_LEFT) == 1) {
	//	d3dMovementM = XMMatrixTranslation(0, 0, m_fMouseMovementSpeed);
	//	d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	//}
	//// down key movement
	//if (InputCheck(G_BUTTON_RIGHT) == 1) {
	//	d3dMovementM = XMMatrixTranslation(0, 0, -m_fMouseMovementSpeed);

	//	d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	//}
	//// left key movement
	//if (InputCheck(G_KEY_A) == 1) {
	//	d3dMovementM = XMMatrixTranslation(-m_fMouseMovementSpeed, 0, 0);
	//	d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	//}
	//// right key movement
	//if (InputCheck(G_KEY_D) == 1) {
	//	d3dMovementM = XMMatrixTranslation(m_fMouseMovementSpeed, 0, 0);
	//	d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	//}
	//if (InputCheck(G_KEY_W) == 1) {
	//	d3dMovementM = XMMatrixTranslation(0, m_fMouseMovementSpeed, 0);
	//	d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	//}
	//if (InputCheck(G_KEY_S) == 1) {
	//	d3dMovementM = XMMatrixTranslation(0, -m_fMouseMovementSpeed, 0);
	//	d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);
	//}

	//}
	//Right && Left Rotation(keybord implemented, soon to be changed in the mouse)

// Left Rotation
	if (InputCheck(G_KEY_J) == 1)
	{
		d3dRotation = XMMatrixRotationY(-1.0f * m_fMouseRotationSpeed);

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


		//newV = XMMatrixMultiply(d3dTmpViewM, d3dRotation);

	}
	//Right Rotation
	if (InputCheck(G_KEY_L) == 1)
	{
		d3dRotation = XMMatrixRotationY(1.0f * m_fMouseRotationSpeed);

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

		//newV = XMMatrixMultiply(d3dRotation, d3dTmpViewM);

	}
	// Up rotation
	if (InputCheck(G_KEY_I) == 1)
	{
		d3dRotation = XMMatrixRotationX(-1.0f * m_fMouseRotationSpeed);

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


		//newV = XMMatrixMultiply(d3dTmpViewM, d3dRotation);


	}
	// Down rotation

	if (InputCheck(G_KEY_K) == 1)
	{
		d3dRotation = XMMatrixRotationX(1.0f * m_fMouseRotationSpeed);

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


	//	//newV = XMMatrixMultiply(d3dTmpViewM, d3dRotation);


	}
	/*Z = DirectX::XMVectorSubtract(T, V);
	Z2.m128_f32[0] = T.m128_f32[0] - V.m128_f32[0];
	Z2.m128_f32[1] = T.m128_f32[1] - V.m128_f32[1];
	Z2.m128_f32[2] = T.m128_f32[2] - V.m128_f32[2];
	Z2.m128_f32[3] = 0;

	Z = DirectX::XMVector3Normalize(Z);

	X = DirectX::XMVector3Cross(U, Z);

	X2.m128_f32[0] = (U.m128_f32[1] * Z.m128_f32[2]) - (U.m128_f32[2] * Z.m128_f32[1]);
	X2.m128_f32[1] = (U.m128_f32[2] * Z.m128_f32[0]) - (U.m128_f32[0] * Z.m128_f32[2]);
	X2.m128_f32[2] = (U.m128_f32[0] * Z.m128_f32[1]) - (U.m128_f32[1] * Z.m128_f32[0]);
	X2.m128_f32[3] = 0;

	X = DirectX::XMVector3Normalize(X);

	Y = DirectX::XMVector3Cross(Z, X);
	Y2.m128_f32[0] = (Z.m128_f32[1] * X.m128_f32[2]) - (Z.m128_f32[2] * X.m128_f32[1]);
	Y2.m128_f32[1] = (Z.m128_f32[2] * X.m128_f32[0]) - (Z.m128_f32[0] * X.m128_f32[2]);
	Y2.m128_f32[2] = (Z.m128_f32[0] * X.m128_f32[1]) - (Z.m128_f32[1] * X.m128_f32[0]);
	Y2.m128_f32[3] = 0;
	Y = DirectX::XMVector3Normalize(Y);

	
	d3dTmpViewM.r[0] = X;
		d3dTmpViewM.r[1] = Y;
		d3dTmpViewM.r[2] = Z;*/
	
	


	//d3dTmpViewM.r[3] = W;
	return d3dTmpViewM;
}