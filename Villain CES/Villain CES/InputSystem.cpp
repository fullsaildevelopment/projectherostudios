#include "InputSystem.h"
CInputSystem::CInputSystem()
{
	m_fMouseRotationSpeed = .0055f;
	m_fMouseMovementSpeed = 3.0f;
	m_YRotationLimit = 0;
	prev_X = prev_Y = fXchange = 0, fYchange = 0, fXEnd = 0, fYEnd = 0;
	 m_Companion1 = m_Companion2 = stepCount = 0;
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

void CInputSystem::gameManagerCodeAbstracted(
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
	XMFLOAT4 &d3dCollisionColor, double &delta, CAudioSystem* in_Audio, TClayton &clayton, XMVECTOR &playerVeclocity, XMMATRIX &Caelis_Matrix, int PlayerIndex)
{
	cHoverPoint = { -1, -1 };
	//POINT hoverPoint;
	GetCursorPos(&cHoverPoint);
	ScreenToClient(cApplicationWindow, &cHoverPoint);

	//POINT 
	cClickPoint = { -1, -1 };
	if (nButtonLeft == 1 && bMouseUp)
	{
		GetCursorPos(&cStartDragPoint);
		ScreenToClient(cApplicationWindow, &cStartDragPoint);

		GetCursorPos(&cDragPoint);
		ScreenToClient(cApplicationWindow, &cDragPoint);

		bMouseUp = false;
		bMouseDown = true;
	}
	else if (nButtonLeft == 1 && bMouseDown)
	{
		GetCursorPos(&cDragPoint);
		ScreenToClient(cApplicationWindow, &cDragPoint);
	}
	else if (nButtonLeft == 0 && bMouseDown)
	{
		GetCursorPos(&cClickPoint);
		ScreenToClient(cApplicationWindow, &cClickPoint);

		bMouseUp = true;
		bMouseDown = false;

		bClick = true;

		cStartDragPoint = { -1, -1 };
		cDragPoint = { -1, -1 };
	}

	if (nKeyP && !bGameOver)
	{
		bGamePaused = true;
		if (!bPauseInit)
		{
			bPauseInit = true;
			ShowCursor(true);
		}
	}
	if (nKeyU && !bGameOver)
	{
		bGamePaused = false;
		bOptions = false;
		if (bPauseInit)
		{
			bPauseInit = false;
			ShowCursor(false);
		}
	}

	//d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
	
	//Camera Functions here will move to a input system function when all behaviors are finalized - ZFB
	if (bGamePaused == false && bGameOver == false)
	{
		// Walk mode not needed in demo at the moment - ZFB
		//Clayton 
		if (PlayerIndex == 919)
		{
			if (tCameraMode.bWalkMode == true)
			{
				if (tCameraMode.bSwitch == true)
				{
					d3dResultMatrix = this->CameraOrientationReset(d3dResultMatrix);
					tCameraMode.bSwitch = false;
				}

				d3dResultMatrix = this->WalkCameraControls(XMVectorSet(0, 1.0f, 0, 0), d3dResultMatrix, bNoMoving, delta);

				tWalkCamera->d3d_Position = XMMatrixMultiply(d3dResultMatrix, d3dPlayerMatrix);
				tWalkCamera->d3d_Position = XMMatrixMultiply(d3dOffsetMatrix, tWalkCamera->d3d_Position);

				tMyViewMatrix = tWalkCamera->d3d_Position;
				tTempViewMatrix = tWalkCamera->d3d_Position;
			}
			//Aim Mode Functions are Here - ZFB 
			else if (tCameraMode.bAimMode == true)
			{
				d3dOffsetMatrix = d3dResetAimModeCameraOffset;
				if (tCameraMode.bSwitch == true)
				{
					d3dResultMatrix = this->CameraOrientationReset(d3dResultMatrix);

					tCameraMode.bSwitch = false;
				}

				fRealTimeFov = this->ZoomSight(fRealTimeFov);
				// Camera rotation Done here
				tAimCamera->d3d_Position = this->AimMode(tAimCamera, d3dResultMatrix, delta, bNoMoving);
				//Does Character Rotation and Movement
				d3dPlayerMatrix = this->CharacterMovement(d3dPlayerMatrix, delta, in_Audio, clayton, playerVeclocity, bNoMoving);

				tAimCamera->d3d_Position = XMMatrixMultiply(tAimCamera->d3d_Position, d3dPlayerMatrix);
				// for shoulder offset 
				tAimCamera->d3d_Position = XMMatrixMultiply(d3dOffsetMatrix, tAimCamera->d3d_Position);

				tMyViewMatrix = tAimCamera->d3d_Position;
				tTempViewMatrix = tAimCamera->d3d_Position;
			}
			//This is Debug Camera Function here - ZFB
			else
			{
				if (tCameraMode.bSwitch == true)
				{
					d3dResultMatrix = this->CameraOrientationReset(d3dResultMatrix);
					tCameraMode.bSwitch = false;
				}
				d3dResultMatrix = this->DebugCamera(d3dResultMatrix, d3dWorldMatrix, delta);

				tDebugCamera->d3d_Position = XMMatrixMultiply(d3dResultMatrix, d3dWorldMatrix);
				tMyViewMatrix = tDebugCamera->d3d_Position;
				tTempViewMatrix = tDebugCamera->d3d_Position;
			}
		}

		else if (PlayerIndex == 920)
		{
			if (tCameraMode.bAimMode == true)
			{
				d3dOffsetMatrix = d3dResetAimModeCameraOffset;
				if (tCameraMode.bSwitch == true)
				{
					d3dResultMatrix = this->CameraOrientationReset(d3dResultMatrix);

					tCameraMode.bSwitch = false;
				}

				fRealTimeFov = this->ZoomSight(fRealTimeFov);
				// Camera rotation Done here
				tAimCamera->d3d_Position = this->AimMode(tAimCamera, d3dResultMatrix, delta, bNoMoving);
				//Does Character Rotation and Movement
				Caelis_Matrix = this->CharacterMovement(Caelis_Matrix, delta, in_Audio, clayton, playerVeclocity, bNoMoving);

				tAimCamera->d3d_Position = XMMatrixMultiply(tAimCamera->d3d_Position, Caelis_Matrix);
				// for shoulder offset 
				tAimCamera->d3d_Position = XMMatrixMultiply(d3dOffsetMatrix, tAimCamera->d3d_Position);

				tMyViewMatrix = tAimCamera->d3d_Position;
				tTempViewMatrix = tAimCamera->d3d_Position;
			}
		}
	}

	// toggle the modes that you are in
	if (bGamePaused == false && bGameOver == false)
	{
		if (nButtonMiddle)
		{
			bGunMode = !bGunMode;
		}
		// shoot a bullet
		if (nButtonLeft == 1
			&& bGunMode == true)
		{
			bTryToShoot = true;
		}
		// shoot a ray
		else if (nButtonLeft == 1
			&& bGunMode == false)
		{
			bTryToShoot = true;
		}
		// turn the ray off
		else if (bGunMode == false)
		{
			bTryToShoot = false;
		}

		// reload
		if (nKeyR == 1)
		{
			bTryToReload = true;
		}
	}
}

GReturn CInputSystem::InitializeGInput(HWND cTheWindow)
{
	// returns the results of function
	return CreateGInput(cTheWindow, sizeof(cTheWindow), &m_pcMyInput);
}

XMMATRIX CInputSystem::DebugCamera(XMMATRIX d3d_ViewM, XMMATRIX d3d_WorldM, double delta)
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
		d3dMovementM = XMMatrixTranslation(0, 0, m_fMouseMovementSpeed * delta);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	// down key movement
	if (InputCheck(G_KEY_S) == 1) {
		d3dMovementM = XMMatrixTranslation(0, 0, -m_fMouseMovementSpeed * delta);

		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	// left key movement
	if (InputCheck(G_KEY_A) == 1) {
		d3dMovementM = XMMatrixTranslation(-m_fMouseMovementSpeed * delta, 0, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	// right key movement
	if (InputCheck(G_KEY_D) == 1) {
		d3dMovementM = XMMatrixTranslation(m_fMouseMovementSpeed * delta, 0, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	if (InputCheck(G_KEY_SPACE) == 1) {
		d3dMovementM = XMMatrixTranslation(0, m_fMouseMovementSpeed * delta, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);

	}
	if (InputCheck(G_KEY_LEFTSHIFT) == 1) {
		d3dMovementM = XMMatrixTranslation(0, -m_fMouseMovementSpeed * delta, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);
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

XMMATRIX CInputSystem::CharacterMovement(XMMATRIX d3dplayerMatrix, double delta, CAudioSystem* in_Audio, TClayton &clayton, XMVECTOR &playerVeclocity, bool move)
{

	XMMATRIX d3dTmpViewM, d3dMovementM, d3dRotation;
	XMVECTOR d3d_newX, d3d_newY, d3d_existingZ;
	d3dTmpViewM = d3dplayerMatrix;
	bool keyPressed = false;
	
	if (fXchange > 0.5f || fYchange > 0.5f || fXchange < -0.5f || fYchange < -0.5f)
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
	}
	

	//Forward && Back Movement

	// up key movement

	if (InputCheck(G_KEY_W) == 1) 
	{
		d3dMovementM = XMMatrixTranslation(0, 0, m_fMouseMovementSpeed * delta);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);
		stepCount++;
		keyPressed = true;
	}
	// down key movement
	if (InputCheck(G_KEY_S) == 1)
	{
		d3dMovementM = XMMatrixTranslation(0, 0, -m_fMouseMovementSpeed * delta);

		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);
		keyPressed = true;
		stepCount++;
	}
	// left key movement
	if (InputCheck(G_KEY_A) == 1)
	{
		d3dMovementM = XMMatrixTranslation(-m_fMouseMovementSpeed * delta, 0, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);
		keyPressed = true;
		stepCount++;

	}
	// right key movement
	if (InputCheck(G_KEY_D) == 1) 
	{
		d3dMovementM = XMMatrixTranslation(m_fMouseMovementSpeed * delta, 0, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);
		keyPressed = true;
		stepCount++;
	}
	if (InputCheck(G_KEY_SPACE) == 1 && clayton.jumpTime > 0 && playerVeclocity.m128_f32[1] > -.1)
	{
		d3dMovementM = XMMatrixTranslation(0, m_fMouseMovementSpeed * delta, 0);
		d3dTmpViewM = XMMatrixMultiply(d3dMovementM, d3dTmpViewM);
		keyPressed = true;
		stepCount++;

		clayton.jumpTime -= delta;

		if (clayton.jumpTime <= 0)
		{
			clayton.jumpCooldown = 1;
		}
	}
	else if (InputCheck(G_KEY_SPACE) == 0 && clayton.jumpTime > 0)
	{
		if (clayton.jumpTime < 1)
		{
			clayton.jumpTime += delta;
		}
	}

	if (keyPressed == true && stepCount == 20)
	{
#if MUSIC_ON
		in_Audio->SendSoundsToEngine(AK::EVENTS::PLAY_WALK, in_Audio->m_WalkSound);
		stepCount = 0;
#endif
	}
	if (stepCount > 20)
	{
		stepCount = 0;
	}

	return d3dTmpViewM;
}



XMMATRIX CInputSystem::MyLookAt(XMVECTOR EyePos, XMVECTOR TargetPos, XMVECTOR Up) {
	XMVECTOR X, Y, Z, X2, Y2, Z2;
	XMMATRIX newV;
	newV.r[3] = EyePos;
	Z = DirectX::XMVectorSubtract(TargetPos, EyePos);
	//For testing to get right values
	Z2.m128_f32[0] = TargetPos.m128_f32[0] - EyePos.m128_f32[0];
	Z2.m128_f32[1] = TargetPos.m128_f32[1] - EyePos.m128_f32[1];
	Z2.m128_f32[2] = TargetPos.m128_f32[2] - EyePos.m128_f32[2];
	Z2.m128_f32[3] = 0;

	Z = DirectX::XMVector3Normalize(Z);

	X = DirectX::XMVector3Cross(Up, Z);
	//For testing
	X2.m128_f32[0] = (Up.m128_f32[1] * Z.m128_f32[2]) - (Up.m128_f32[2] * Z.m128_f32[1]);
	X2.m128_f32[1] = (Up.m128_f32[2] * Z.m128_f32[0]) - (Up.m128_f32[0] * Z.m128_f32[2]);
	X2.m128_f32[2] = (Up.m128_f32[0] * Z.m128_f32[1]) - (Up.m128_f32[1] * Z.m128_f32[0]);
	X2.m128_f32[3] = 0;

	X = DirectX::XMVector3Normalize(X);

	Y = DirectX::XMVector3Cross(Z, X);
	// for testing 
	Y2.m128_f32[0] = (Z.m128_f32[1] * X.m128_f32[2]) - (Z.m128_f32[2] * X.m128_f32[1]);
	Y2.m128_f32[1] = (Z.m128_f32[2] * X.m128_f32[0]) - (Z.m128_f32[0] * X.m128_f32[2]);
	Y2.m128_f32[2] = (Z.m128_f32[0] * X.m128_f32[1]) - (Z.m128_f32[1] * X.m128_f32[0]);
	Y2.m128_f32[3] = 0;
	Y = DirectX::XMVector3Normalize(Y);
	//Get new eyepos to account for 
	XMVECTOR newEyePos;
	newEyePos.m128_f32[0] = XMVector3Dot(X, EyePos).m128_f32[0];
	newEyePos.m128_f32[1] = XMVector3Dot(Y, EyePos).m128_f32[0];
	newEyePos.m128_f32[2] = XMVector3Dot(Z, EyePos).m128_f32[0];
	newEyePos.m128_f32[3] = 1.0f;

	newV.r[0] = X;
	newV.r[1] = Y;
	newV.r[2] = Z;
	newV.r[3] = newEyePos;


	return newV;
}

void CInputSystem::StoreInitCameraForwardV(XMVECTOR in_prevPos)
{
	m_CameraInitV = in_prevPos;
}

void CInputSystem::StorePreviousCameraPos(XMVECTOR in_Pos)
{
	m_prevCameraPos.r[3] = in_Pos;
}

void CInputSystem::GetMousePosition()
{
	m_pcMyInput->GetMousePosition(fXEnd, fYEnd);
}

XMMATRIX CInputSystem::AimMode(TCamera * in_AimCamera, XMMATRIX d3dplayerMatrix, double delta, bool move)
{
	XMMATRIX d3dTmpViewM, d3dMovementM, d3dRotation;
	GReturn input_Check;
	d3dTmpViewM = d3dplayerMatrix;

	XMVECTOR d3d_newX, d3d_newY, d3d_existingZ;
	m_pcMyInput->GetMousePosition(fXEnd, fYEnd);

	//MouseBoundryCheck(fXEnd, fYEnd, fXchange, fYchange);
	//Display current mouse pose

	// Check thatacts like a deadzone and for getting a good mouse pos delta
	//if (fXEnd > 2.0f && fXEnd < 1410.0f && fYEnd > 2.0f && fYEnd < 700.0f)
	//{
	if (move == false)
	{

		if (InputCheck(G_KEY_K) == 1)
		{
			std::string stTimeDisplay;
			stTimeDisplay = "X Change: ";
			stTimeDisplay += std::to_string(fXchange);
			cout << stTimeDisplay << endl;

			stTimeDisplay = "Y Change: ";
			stTimeDisplay += std::to_string(fYchange);
			cout << stTimeDisplay << endl;
		}

		input_Check = m_pcMyInput->GetMouseDelta(fXchange, fYchange);
		if (prev_X == fXchange && prev_Y == fYchange && (prev_X != 0 || prev_Y != 0) && input_Check == REDUNDANT_OPERATION)
		{
			fXchange = 0;
			fYchange = 0;
			//in_AimCamera->fPitch = 0;

		}
		else if (fYchange > 0.3f || fYchange < -0.3f)
		{
			in_AimCamera->fPitch += fYchange;

			if (in_AimCamera->fPitch >= 90.0f)
			{
				in_AimCamera->fPitch = 89.0f;
			}
			else if (in_AimCamera->fPitch <= -90.0f)
			{
				in_AimCamera->fPitch = -89.0f;
			}
		}

		d3dRotation = XMMatrixRotationX(in_AimCamera->fPitch * m_fMouseRotationSpeed);
		d3dTmpViewM = XMMatrixMultiply(d3dTmpViewM, d3dRotation);

		//d3dTmpViewM = XMMatrixMultiply(d3dTmpViewM, d3dRotation);

		// see if you can lerp fxchange or end to get a lerped value
		d3dRotation = XMMatrixRotationY(fXchange * m_fMouseRotationSpeed * delta);
		d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);
		//d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM); 
		//Debug 


		//Gimbal-Lock implementation
		d3d_existingZ = d3dTmpViewM.r[2];
		d3d_newX = XMVector3Cross(XMVectorSet(0, 1, 0, 0), d3d_existingZ);
		d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

		d3d_newX = XMVector3Normalize(d3d_newX);
		d3d_newY = XMVector3Normalize(d3d_newY);
		d3d_existingZ = XMVector3Normalize(d3d_existingZ);

		d3dTmpViewM.r[0] = d3d_newX;
		d3dTmpViewM.r[1] = d3d_newY;
		d3dTmpViewM.r[2] = d3d_existingZ;

		if (input_Check != REDUNDANT_OPERATION)
		{
			prev_X = fXchange;
			prev_Y = fYchange;
		}

	}

	return d3dTmpViewM;
}

XMMATRIX CInputSystem::WalkCameraControls(XMVECTOR U, XMMATRIX viewM, bool &_movement, double delta) {
	//Orbit Camera 
	//XMVECTOR X, Y, Z, X2, Y2, Z2, W;
	XMVECTOR d3d_newX, d3d_newY, d3d_existingZ;
	XMMATRIX d3dTmpViewM, d3dRotation, d3dTranslation;
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

	//MouseBoundryCheck(fXEnd, fYEnd, fXchange, fYchange);

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
	
		d3dRotation = XMMatrixRotationRollPitchYaw(fYchange * m_fMouseRotationSpeed * delta, fXchange * m_fMouseRotationSpeed * delta, 0);        //XMMatrixRotationX(fYchange * m_fMouseRotationSpeed);

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
		
		
		//d3dRotation = XMMatrixRotationY(fXchange * m_fMouseRotationSpeed); //d3dRotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(0), XMConvertToRadians(fXchange + m_fMouseRotationSpeed), XMConvertToRadians(0));

		//d3dTmpViewM = XMMatrixMultiply(d3dTmpViewM, d3dRotation);


		/*d3d_existingZ = d3dTmpViewM.r[2];
		d3d_newX = XMVector3Cross(XMVectorSet(0, 1, 0, 0), d3d_existingZ);
		d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

		d3d_newX = XMVector3Normalize(d3d_newX);
		d3d_newY = XMVector3Normalize(d3d_newY);

		d3d_existingZ = XMVector3Normalize(d3d_existingZ);

		d3dTmpViewM.r[0] = d3d_newX;
		d3dTmpViewM.r[1] = d3d_newY;
		d3dTmpViewM.r[2] = d3d_existingZ;*/
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
	//space.m128_f32[0] = TargetPos.m128_f32[0] - viewV.m128_f32[0];
	//space.m128_f32[1] = TargetPos.m128_f32[1] - viewV.m128_f32[1];
	//space.m128_f32[2] = TargetPos.m128_f32[2] - viewV.m128_f32[2];
	//space.m128_f32[3] = 0;
	//space = TargetPos - viewV;
	/*//space = XMVectorSubtract(TargetPos,);*/
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

int CInputSystem::CharacterSwitch(int characterIndex)
{
	int m_CurrentPlayerIndex = characterIndex;
	int CurrentCompanion1 = m_Companion1;
	int currentCompanion2 = m_Companion2;
	// Switch to Companion 1
	if (InputCheck(G_KEY_0) == 1 && m_buttonPressed == false)
	{
		// switch playerStartIndex to companion 1 
		m_CurrentPlayerIndex = CurrentCompanion1;
		//Switch previous character index to be Companion 1
		m_Companion1 = characterIndex;
		m_ToCompanion1 = true;
	}
	//Switch to Companion 2
	else if (InputCheck(G_KEY_5) == 1 && m_buttonPressed == false)
	{
		// switch playerStartIndex to companion 2
		m_CurrentPlayerIndex = currentCompanion2;
		//Switch previous character index to be Companion 2
		m_Companion2 = characterIndex;
		m_ToCompanion2 = true;
	}
	m_characterSwitch = true;
	return  m_CurrentPlayerIndex;
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

float CInputSystem::GetMouseRotationSpeed()
{
	return m_fMouseRotationSpeed;
}

void CInputSystem::SetMouseRotationSpeed(float newSpeed)
{
	m_fMouseRotationSpeed = newSpeed;
}