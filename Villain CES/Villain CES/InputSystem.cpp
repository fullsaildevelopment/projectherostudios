#include "InputSystem.h"
#include<string>
CInputSystem::CInputSystem()
{
	m_fMouseRotationSpeed = .03f;//Frame Dependent
	m_fMouseMovementSpeed = 1.0f;//Frame Dependent
	m_YRotationLimit = 0;
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
		if (tTempCamMode.bAimMode == false) {
			tTempCamMode.bAimMode = true;
			tTempCamMode.bDebugMode = false;
			tTempCamMode.bWalkMode = false;
			tTempCamMode.bSwitch = true;
		}

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
	m_pcMyInput->GetMousePosition(fXEnd, fYEnd);

	MouseBoundryCheck(fXEnd, fYEnd, fXchange, fYchange);


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
	if (InputCheck(G_KEY_LEFTSHIFT) == 1) {
		d3dMovementM = XMMatrixTranslation(0, -m_fMouseMovementSpeed, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);
	}


	if (InputCheck(G_KEY_RIGHTSHIFT))
	{
		m_fMouseMovementSpeed += .05;
	}

	if (InputCheck(G_KEY_FOWARDSLASH))
	{
		m_fMouseMovementSpeed = .05;
	}


	if (fXEnd > 2.0f && fXEnd < 1410.0f && fYEnd > 2.0f && fYEnd < 700.0f)
	{
		m_pcMyInput->GetMouseDelta(fXchange, fYchange);
	}
	if (fXchange > 1.0f || fYchange > 1.0f || fXchange < -1.0f || fYchange < -1.0f)
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


		d3d_existingZ = d3dTmpViewM.r[2];
		d3d_newX = XMVector3Cross(XMVectorSet(0, 1, 0, 0), d3d_existingZ);
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

	if (InputCheck(G_KEY_RIGHTSHIFT))
	{
		m_fMouseMovementSpeed *= 10.5;
	}

	if (InputCheck(G_KEY_FOWARDSLASH))
	{
		m_fMouseMovementSpeed /= 10.5;
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
	m_pcMyInput->GetMousePosition(fXEnd, fYEnd);
	
	MouseBoundryCheck(fXEnd, fYEnd, fXchange, fYchange);
	

	if (InputCheck(G_KEY_K) == 1)
	{
		std::string stTimeDisplay;
		stTimeDisplay = "X Coord: ";
		stTimeDisplay += std::to_string(fXEnd);
		cout << stTimeDisplay << endl;

		stTimeDisplay = "Y Coord: ";
		stTimeDisplay += std::to_string(fYEnd);
		cout << stTimeDisplay << endl;
	}

	if (fXEnd > 2.0f && fXEnd < 1410.0f && fYEnd > 2.0f && fYEnd < 700.0f)
	{
	m_pcMyInput->GetMouseDelta(fXchange, fYchange);
	}
	if (fXchange > 1.0f || fYchange > 1.0f || fXchange < -1.0f || fYchange < -1.0f)
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
	
	/*if (fXchange < 0 && fYchange < 0)
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
		d3dRotation = XMMatrixRotationX(fYchange * m_fMouseRotationSpeed);

		d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);

	}
	if (fXchange > 0 && fYchange > 0)
	{
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

		d3dRotation = XMMatrixRotationX(fYchange * m_fMouseRotationSpeed);
		d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);

	}*/

	
	
	return d3dTmpViewM;
}


XMMATRIX CInputSystem::WalkCameraControls(XMVECTOR U, XMMATRIX viewM, bool &_movement) {
	//Orbit Camera 
	//XMVECTOR X, Y, Z, X2, Y2, Z2, W;
	XMVECTOR d3d_newX, d3d_newY, d3d_existingZ;
	XMMATRIX d3dTmpViewM, d3dRotation;
	float fXchange = 0, fYchange = 0, fXEnd = 0, fYEnd = 0;
	// gets mouse's position to check if it is within the Game Window
	m_pcMyInput->GetMousePosition(fXEnd, fYEnd);

	d3dTmpViewM = viewM;

	if (InputCheck(G_KEY_K) == 1)
	{
		std::string stTimeDisplay;
		stTimeDisplay = "X Coord: ";
		stTimeDisplay += std::to_string(fXEnd);
		cout << stTimeDisplay << endl;

		stTimeDisplay = "Y Coord: ";
		stTimeDisplay += std::to_string(fYEnd);
		cout << stTimeDisplay << endl;
	}

	MouseBoundryCheck(fXEnd, fYEnd, fXchange, fYchange);

	if (fXEnd > 2.0f && fXEnd < 1410.0f && fYEnd > 2.0f && fYEnd < 700.0f)
	{
		m_pcMyInput->GetMouseDelta(fXchange, fYchange);
	}


	if (fXchange > 1.0f || fYchange > 1.0f || fXchange < -1.0f || fYchange < -1.0f )
	{
		_movement = true;
	
		/*if (fYchange > 3.0f || fYchange < -3.0f)
		{
			d3dRotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fYchange + m_fMouseRotationSpeed), XMConvertToRadians(0.0f), XMConvertToRadians(0));*/
		//XMMatrixRotationY(fXchange * m_fMouseRotationSpeed);

		//d3dTmpViewM = XMMatrixMultiply(d3dTmpViewM, d3dRotation);
	//	}
		
		//if (fXchange > 3.0f || fXchange < -3.0f)
	//	{
		d3dRotation = XMMatrixRotationY(fXchange * m_fMouseRotationSpeed); //d3dRotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(0), XMConvertToRadians(fXchange + m_fMouseRotationSpeed), XMConvertToRadians(0));

		d3dTmpViewM = XMMatrixMultiply(d3dTmpViewM, d3dRotation);
		//}
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
	}
	else
	{
		_movement = false;
	}

	return d3dTmpViewM;
	
	
}

XMMATRIX CInputSystem::CameraBehaviorLerp(XMMATRIX m1, XMMATRIX m2,float scale)
{
	XMMATRIX lerpedMatrix;
	XMVECTOR q1=XMQuaternionRotationMatrix(m1);
	XMVECTOR q2 = XMQuaternionRotationMatrix(m2);
	XMVECTOR lerpQuaternion=XMQuaternionSlerp(q1, q2, scale);
	lerpedMatrix =XMMatrixRotationQuaternion(lerpQuaternion);

	//lerpedMatrix.r[0] = m2.r[0];
	//lerpedMatrix.r[1] = m2.r[1];
	lerpedMatrix.r[3] = m2.r[3];

	
	
	
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

	if (InputCheck(G_KEY_Z) == 1)
	{
		if (ftmpFov >= 45.0f)
		{
			ftmpFov -= 0.1f;
			
			 if (ftmpFov < 45.0f)
			{
				ftmpFov = 45.0f;
			}
			
			
			
		}

	}	
	else
	{
		if (ftmpFov <= 90.0f)
		{
			ftmpFov += 0.1f;
		}
		else if (ftmpFov >= 90.f)
		{
			ftmpFov = 90.0f;
		}
		

		

	}
	return ftmpFov;
}
XMMATRIX CInputSystem::MyTurnTo(XMMATRIX M, XMVECTOR T, float s, XMMATRIX world)
{
//	float Cspace, C2space;
	//XMVECTOR space, viewV;
	//XMMATRIX View, rotationY, rotationX;
	//View = M;
	//viewV = View.r[3];
	//space.m128_f32[0] = T.m128_f32[0] - viewV.m128_f32[0];
	//space.m128_f32[1] = T.m128_f32[1] - viewV.m128_f32[1];
	//space.m128_f32[2] = T.m128_f32[2] - viewV.m128_f32[2];
	//space.m128_f32[3] = 0;
	//space = T - viewV;
	/*//space = XMVectorSubtract(T,);*/
	//space = XMVector3Normalize(space);

	/*Cspace = MyScalarDotVector(space, View.r[0]);
	C2space = MyScalarDotVector(space, View.r[1]);*/

	//rotationY = XMMatrixRotationY(Cspace * s);
	//rotationX = XMMatrixRotationX(C2space * s);

	//View = XMMatrixMultiply(rotationY, View);
	//View = XMMatrixMultiply(rotationX, View);
//	GrandSchmit(View, XMMatrixIdentity());
//	M = View;
	return M;
}
void CInputSystem::MouseBoundryCheck(float _x, float _y, float &_outX, float &_outY)
{
	RECT tmp_WindowPos;
	POINT mouseCurrPos;
	UpdateWindow(m_GameWindow);
	GetWindowRect(m_GameWindow, &tmp_WindowPos);
	//GetRect()
	//Update window rect 
	// using POINT variable try and check the mouse pos with GetCursorPos
	
	// use these to set cursor pos no matter where the window handle is on the screen



	//Window Dimensions 1440 x 759 
	float restrictedX = _x, restrictedY = _y;
	//MaX & Min X window check
	if (restrictedX >= 1400.0f || restrictedX <= 15.0f)
	{
		restrictedX -= 712.0f;
		_outX = restrictedX * m_fMouseRotationSpeed;
		GetCursorPos(&mouseCurrPos);
		if (restrictedX >= 1400.0f)
		{
			SetCursorPos(tmp_WindowPos.left - 5, restrictedY);
		}
		else if ( restrictedX <= 15.0f)
		{
			SetCursorPos(tmp_WindowPos.left + 5, restrictedY);
		}
		

		//SetCursorPos(15, _outY);
		
	}
	
	//Max & Min Y window check
	//if (m_YRotationLimit <= 90.0f && m_YRotationLimit >= -90.0f)
	//{
		if (restrictedY >= 680.0f || restrictedY <= 10.0f)
		{
			restrictedY -= 360;
			_outY = restrictedY * m_fMouseRotationSpeed;
			//SetCursorPos(_outX, 700);
		/*	if (restrictedY > 360)
			{
				m_YRotationLimit++;
				m_YRotationLimit * 
			}
			else
			{
				m_YRotationLimit--;
			}*/
		}
	//}
	
	
	



}

void CInputSystem::SendHWNDToInputSystem(HWND in_WindowHandle)
{
	m_GameWindow = in_WindowHandle;
}
