#include "GameManger.h"
#define AI_ON false
CGameMangerSystem::CGameMangerSystem(HWND window, CInputSystem* _pcInputSystem)
{
	cApplicationWindow = window;
	pcGraphicsSystem = new CGraphicsSystem();
	pcInputSystem = _pcInputSystem;
	pcCollisionSystem = new CCollisionSystem();
	pcPhysicsSystem = new CPhysicsSystem();
	//pcInputSystem->InitializeGInput(window);
	pcProjectileSystem = new CProjectileSystem();
	pcAiSystem = new CAISystem();
	pcGraphicsSystem->InitD3D(cApplicationWindow);
	tAugerTimers = new Timers();
	tTimerInfo = new System_Times();
	aimCamera = new TCamera();
	walkCamera = new TCamera();
	debugCamera = new TCamera();
	menuCamera = new TCamera();

	m_RealTimeFov = 90.0f;
	//srand(time(NULL));
}

CGameMangerSystem::~CGameMangerSystem()
{
	//pcGraphicsSystem->CleanD3D(&tThisWorld);
	delete pcGraphicsSystem;
	delete pcInputSystem;
	delete pcCollisionSystem;
	delete pcPhysicsSystem;
	delete pcProjectileSystem;
	delete pcAiSystem;
	delete tAugerTimers;
	delete tTimerInfo;
	delete aimCamera;
	delete debugCamera;
	delete walkCamera;
	delete menuCamera;
}

void CGameMangerSystem::LoadLevel()
{
	// master
	tTimerInfo->StartClock(tAugerTimers->tSceneTimer);

	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);

	pcAiSystem->SetNumberOfAI(2);

	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
	walkCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	aimCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	debugCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	//m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective(m_RealTimeFov);
	tCameraMode.bDebugMode = false;
	tCameraMode.bAimMode = false;
	tCameraMode.bWalkMode = true;
	tCameraMode.bSwitch = false;
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();
	m_d3dPlayerMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	//m_d3dPlayerMatrix = XMMatrixMultiply(m_d3dPlayerMatrix, XMMatrixScaling(.01, .01, .01));
	m_d3dPlayerMatrix.r[3].m128_f32[2] -= 10;
	
	ImporterData tempImport;
	
	tempImport = pcGraphicsSystem->ReadMesh("meshData_Pirate.txt");
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		PlayerStartIndex = createClayton(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	}

	tThisWorld.atClayton[PlayerStartIndex].health = 100;
	XMMATRIX wall = m_d3dWorldMatrix;
	wall.r[3].m128_f32[1] += -1;

	CreateWall(&tThisWorld, wall);
	wall = m_d3dWorldMatrix;
	wall.r[3].m128_f32[1] += -1;
	wall.r[3].m128_f32[0] += -22;
	CreateWall(&tThisWorld, wall);
	XMMATRIX celling = m_d3dWorldMatrix;
	celling.r[3].m128_f32[1] += 10;
	CreateCelling(&tThisWorld, celling);
	XMMATRIX AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[2] += -5;
	AILocation.r[3].m128_f32[0] += -5;
	//CreateSimpleGunAi(&tThisWorld, AILocation);


	AimingLine(&tThisWorld, m_d3dWorldMatrix, PlayerStartIndex, -1, 1, 10.5);

	GunIndexForPlayer = CreateGun(&tThisWorld, m_d3dWorldMatrix, PlayerStartIndex, -1, 1, 10.5, 3, 100);
	tThisWorld.atClip[GunIndexForPlayer].bulletSpeed = 0.001f;
	AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[0] += -1;
	AILocation.r[3].m128_f32[2] += -2;
	int AiIndex = CreateSimpleGunAi(&tThisWorld, AILocation);
	int GunINdexai = CreateGun(&tThisWorld, m_d3dWorldMatrix, AiIndex, -1.1f, 0, 11, 10, 100);
	tThisWorld.atAIMask[AiIndex].GunIndex = GunINdexai;

	tThisWorld.atClip[GunINdexai].bulletSpeed = 0.0001f;//Frame Dependent


	AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[0] += -3;
	AILocation.r[3].m128_f32[2] += -5;
	int SimpleAi2 = CreateSimpleSearchAi(&tThisWorld, AILocation);
	CreateAIVision(&tThisWorld, AILocation, 8, 8, -0.6f, -0.1f, 10.7f);
	int gun2AI = CreateGun(&tThisWorld, m_d3dWorldMatrix, SimpleAi2, -1.1f, 0, 11, 10, 200);
	tThisWorld.atClip[gun2AI].bulletSpeed = 0.0001f;
	XMFLOAT4 blue10;
	blue10.y = 0;
	blue10.z = 1;
	blue10.w = 1;
	blue10.x = 0;
	tThisWorld.atClip[gun2AI].colorofBullets = blue10;
	tThisWorld.atAIMask[SimpleAi2].GunIndex = gun2AI;
	pcAiSystem->FollowObject(m_d3dPlayerMatrix, &tThisWorld.atWorldMatrix[gun2AI].worldMatrix);
	tThisWorld.atWorldMatrix[SimpleAi2].worldMatrix = XMMatrixLookAtLH(tThisWorld.atWorldMatrix[SimpleAi2].worldMatrix.r[3],
		m_d3dPlayerMatrix.r[3], XMVectorSet(0, 1, 0, 0));
	tThisWorld.atWorldMatrix[SimpleAi2].worldMatrix = XMMatrixInverse(NULL, tThisWorld.atWorldMatrix[SimpleAi2].worldMatrix);

	XMMATRIX groundSpawnPoint;
	groundSpawnPoint = m_d3dWorldMatrix;
	groundSpawnPoint.r[3].m128_f32[1] -= 2;
	CreateGround(&tThisWorld, groundSpawnPoint);

	tempImport = pcGraphicsSystem->ReadMesh("meshData_ClaytonGunBullet.txt");
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	}

	tempImport = pcGraphicsSystem->ReadMesh("meshData_ScifiRoom.txt");
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	}

	if (pcCollisionSystem->m_AAbb.size() != 0) 
	{
		float x = 0;
	}
	
	XMFLOAT4 blue;
	blue.x = 0;
	blue.y = 0;
	blue.z = 1;
	blue.w = 1;
	tThisWorld.atSimpleMesh[8].m_nColor = blue;
	
	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask > 1)
		{
			if (tThisWorld.atSimpleMesh[nCurrentEntity].m_nVertexCount > tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount)
			{
				TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[nCurrentEntity].m_VertexData, tThisWorld.atAABB[nCurrentEntity]);
				MyAbb.m_IndexLocation = nCurrentEntity;
				tThisWorld.atAABB[nCurrentEntity] = MyAbb;
				pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);
			}

		}
		if (tThisWorld.atMesh[nCurrentEntity].m_nVertexCount > tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount)
		{
			TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atMesh[nCurrentEntity].m_VertexData, tThisWorld.atAABB[nCurrentEntity]);
			MyAbb.m_IndexLocation = nCurrentEntity;
			tThisWorld.atAABB[nCurrentEntity] = MyAbb;
			pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);
		}
	}


	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask > 1)
		{
			tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
		}
	}


	pcGraphicsSystem->CreateBuffers(&tThisWorld);
}

int CGameMangerSystem::InGameUpdate()
{
	tTimerInfo->applicationTime = tTimerInfo->GetTime(tAugerTimers->tAppTimer, tTimerInfo->applicationTime);
	tTimerInfo->sceneTime = tTimerInfo->GetTime(tAugerTimers->tSceneTimer, tTimerInfo->sceneTime);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(-1); //Important!

	tCameraMode = pcInputSystem->CameraModeListen(tCameraMode);

	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();

	static XMMATRIX m_d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	static XMMATRIX m_d3dOffsetMatrix = pcGraphicsSystem->SetDefaultOffset();
	if (pcInputSystem->InputCheck(G_KEY_P)) 
	{
		return 3;

	}
	// ui stuff
	if (GamePaused == true)
	{
		if (DrawUI == true)
		{
			XMMATRIX UiPos = tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix;
			//UiPos.r[3].m128_f32[1] -= 1;
			XMVECTOR foward;
			foward.m128_f32[0] = 0;
			foward.m128_f32[1] = 1;
			foward.m128_f32[2] = 2;
			//	foward.m128_f32[0] = 1;


			XMMATRIX localMatrix2 = XMMatrixTranslationFromVector(foward);

			UiPos = XMMatrixMultiply(localMatrix2, UiPos);
			DrawUI = false;
			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] -= 1;
			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] += 1;
			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] += 1;
			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] += 1;
			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[0] += 1;
			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[0] += 0.8f;
			UiPos.r[3].m128_f32[1] -= 0.3f;

			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] -= 0.7f;

			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] -= 0.7f;

			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[0] -= 1;

			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
		}
	}

	if (tCameraMode.bWalkMode == true)
	{
		if (tCameraMode.bSwitch == true)
		{
			m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
			tCameraMode.bSwitch = false;
		}
		//pcInputSystem->CameraBehaviorLerp(walkCamera->d3d_Position, m_d3dPlayerMatrix);
		
		m_d3d_ResultMatrix = pcInputSystem->WalkCameraControls(XMVectorSet(0, 1.0f, 0, 0), m_d3d_ResultMatrix);
		
		walkCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);

	walkCamera->d3d_Position = XMMatrixMultiply(m_d3dOffsetMatrix, walkCamera->d3d_Position);
	}

	else if (tCameraMode.bAimMode == true)
	{
		//m_RealTimeFov = pcInputSystem->ZoomSight(m_RealTimeFov);
		if (tCameraMode.bSwitch == true)
		{
			m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
			tCameraMode.bSwitch = false;
		}

		

		m_d3dPlayerMatrix = pcInputSystem->AimMode(m_d3dPlayerMatrix);
		aimCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);
		aimCamera->d3d_Position = XMMatrixMultiply(m_d3dOffsetMatrix, aimCamera->d3d_Position);
		
	}
	else
	{
	
		if (tCameraMode.bSwitch == true)
		{
			m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
			tCameraMode.bSwitch = false;
		}
		m_d3d_ResultMatrix = pcInputSystem->DebugCamera(m_d3d_ResultMatrix, m_d3dWorldMatrix);
		
		debugCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dWorldMatrix);
	
	}

	CGraphicsSystem::TPrimalVertexBufferType tTempVertexBuffer;
	CGraphicsSystem::TPrimalPixelBufferType tTempPixelBuffer;
	CGraphicsSystem::TMyVertexBufferType tMyVertexBufferTemp;
	tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

	pcGraphicsSystem->UpdateD3D();

	#pragma region Input Garbage
	// togle the modes that you are in
	if (pcInputSystem->InputCheck(G_BUTTON_MIDDLE)) {
		tThisWorld.atClip[GunIndexForPlayer].GunMode = !tThisWorld.atClip[GunIndexForPlayer].GunMode;
	}
	// shoot a bullet
	if (pcInputSystem->InputCheck(G_BUTTON_LEFT) == 1 && tThisWorld.atClip[GunIndexForPlayer].GunMode == true) {

		tThisWorld.atClip[GunIndexForPlayer].tryToShoot = true;


	}
	// shoot a ray
	else if (pcInputSystem->InputCheck(G_BUTTON_LEFT) == 1 && tThisWorld.atClip[GunIndexForPlayer].GunMode == false) {
		tThisWorld.atClip[GunIndexForPlayer].tryToShoot = true;

	}
	// turn the ray off
	else if (tThisWorld.atClip[GunIndexForPlayer].GunMode == false)
	{
		tThisWorld.atClip[GunIndexForPlayer].tryToShoot = false;

	}

	// reload
	if (pcInputSystem->InputCheck(G_KEY_R) == 1) {

		tThisWorld.atClip[GunIndexForPlayer].tryToReload = true;

	}

#pragma endregion
	
	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		if (tCameraMode.bWalkMode == true)
		{
			tTempVertexBuffer.m_d3dViewMatrix = walkCamera->d3d_Position;
			tMyVertexBufferTemp.m_d3dViewMatrix = walkCamera->d3d_Position;
		}
		else if (tCameraMode.bAimMode == true)
		{

			tTempVertexBuffer.m_d3dViewMatrix = aimCamera->d3d_Position;
			tMyVertexBufferTemp.m_d3dViewMatrix = aimCamera->d3d_Position;

		}
		else if (tCameraMode.bDebugMode == true)
		{
			tTempVertexBuffer.m_d3dWorldMatrix = m_d3dWorldMatrix;

			tTempVertexBuffer.m_d3dViewMatrix = debugCamera->d3d_Position;


			// ai code would run here
		}
		else
		{
			tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;

			tTempVertexBuffer.m_d3dViewMatrix = m_d3dViewMatrix;
			tMyVertexBufferTemp.m_d3dViewMatrix = m_d3dViewMatrix;
		}


		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID)) 
		{
#if AI_ON				
			pcAiSystem->FollowObject(tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
			pcAiSystem->ShootGun(&tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex]);
			
#endif // AI_ON
			if (tCameraMode.bWalkMode == true)
			{
				m_d3dPlayerMatrix = pcInputSystem->CharacterMovement(m_d3dPlayerMatrix);
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
				m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;

			}
			else if (tCameraMode.bAimMode == true)
			{
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
			}
			if (tCameraMode.bWalkMode == true)
			{
				pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], walkCamera->d3d_Position);

			}
			else if (tCameraMode.bAimMode == true)
			{
				pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], aimCamera->d3d_Position);

			}
			else
			{
				pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);

			}
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SEARCH) || tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SPOTEDPLAYER))
		{
			if (tThisWorld.atAIVision[nCurrentEntity].keepSearching == true)
			{
				if (tThisWorld.atAIVision[nCurrentEntity].visionRotation < 7 && tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true)
				{
					
					//tThisWorld.atAIVision[nCurrentEntity].visionRotation += 0.001f;
				//tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcAiSystem->LookBackLeftToRight(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, true);
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true)
				{
					//tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight = false;
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].visionRotation > -7 && tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == false)
				{
					//tThisWorld.atAIVision[nCurrentEntity].visionRotation -= 0.001f;
					//tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcAiSystem->LookBackLeftToRight(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
				}
			}
			

			/*tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;*/
			

		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		{

			pcGraphicsSystem->InitPrimalShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], debugCamera->d3d_Position);

			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

		}
		// ai code would run here
		/*if (pcAiSystem->GetNumberOfAI() <= 0) 
		{
			return -1;
		}*/
		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_FOLLOW)||tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask==(COMPONENT_SHOOT| COMPONENT_AIMASK | COMPONENT_FOLLOW)) 
		{
				#if AI_ON				
								pcAiSystem->FollowObject(tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
								pcAiSystem->ShootGun(&tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex]);
				#endif // AI_ON
		}

		
		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SEARCH) || tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SPOTEDPLAYER)) {


			if (tThisWorld.atAIVision[nCurrentEntity].keepSearching == true) {
				if (tThisWorld.atAIVision[nCurrentEntity].visionRotation < 7 && tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true) {
					/*tThisWorld.atAIVision[nCurrentEntity].visionRotation += 0.001;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcAiSystem->LookBackLeftToRight(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix,
						true);*/
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true) {
					//tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight = false;
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].visionRotation > -7 && tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == false) {
					/*tThisWorld.atAIVision[nCurrentEntity].visionRotation -= 0.001;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcAiSystem->LookBackLeftToRight(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix,
						false);*/
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == false) {
					//tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight = true;
				}
			}
			float closestWall = 10000000000000000000.0f;
			float closestPlayer = 10000000000000000000.0f;
			float* distanceCalucaltion = new float();
			bool spottedplayer = false;

			for (list<TAABB>::iterator ptr = pcCollisionSystem->m_AAbb.begin(); ptr != pcCollisionSystem->m_AAbb.end(); ++ptr)
			{
				if (pcAiSystem->LookForPlayer(XMVector3Transform(tThisWorld.atAIVision[nCurrentEntity].start, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix)
					, XMVector3Transform(tThisWorld.atAIVision[nCurrentEntity].end, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix), tThisWorld.atWorldMatrix[ptr->m_IndexLocation].worldMatrix, *ptr,
					distanceCalucaltion, pcCollisionSystem) == true)
				{
					if (tThisWorld.atInputMask[ptr->m_IndexLocation].m_tnInputMask == (COMPONENT_INPUTMASK | COMPONENT_CLAYTON))
					{
						XMFLOAT4 red;
						red.y = 0;
						red.z = 0;
						red.w = 1;
						red.x = 1;
						tThisWorld.atSimpleMesh[8].m_nColor = red;
						spottedplayer = true;
						tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask = COMPONENT_AIMASK | COMPONENT_FOLLOW;
						tThisWorld.atAIVision[nCurrentEntity].playerLastKnownLocation = tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix;
						tThisWorld.atAIVision[nCurrentEntity].keepSearching = false;
						pcAiSystem->FollowObject(tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);

						break;
					}
					/*	if (tThisWorld.atRigidBody[ptr->m_IndexLocation].wall == true) {
					if (*distanceCalucaltion < closestWall) {
					closestWall = *distanceCalucaltion;
					}
					cout << "wall" << endl;
					if (tThisWorld.atAIVision[nCurrentEntity].wallIndex != ptr->m_IndexLocation) {
					XMFLOAT3 aiPos;
					aiPos.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[0];
					aiPos.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1];
					aiPos.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[2];
					XMFLOAT3 WallPosition;
					WallPosition.x = tThisWorld.atWorldMatrix[ptr->m_IndexLocation].worldMatrix.r[3].m128_f32[0];
					WallPosition.y = tThisWorld.atWorldMatrix[ptr->m_IndexLocation].worldMatrix.r[3].m128_f32[1];
					WallPosition.z = tThisWorld.atWorldMatrix[ptr->m_IndexLocation].worldMatrix.r[3].m128_f32[2];


					if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == false) {
					tThisWorld.atAIVision[nCurrentEntity].visionRotation = 7;
					tThisWorld.atAIVision[nCurrentEntity].wallIndex = ptr->m_IndexLocation;
					}
					else if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true) {
					tThisWorld.atAIVision[nCurrentEntity].visionRotation = -7;
					tThisWorld.atAIVision[nCurrentEntity].wallIndex = ptr->m_IndexLocation;


					}

					}
					}

					}*/
				}
			}
			/*	if (cloasestPlayer < closestWall) {
			XMFLOAT4 red;
			red.y = 0;
			red.z = 0;
			red.w = 1;
			red.x = 1;
			tThisWorld.atSimpleMesh[8].m_nColor = red;
			spotedplayer = true;
			tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask = COMPONENT_AIMASK | COMPONENT_SPOTEDPLAYER;
			tThisWorld.atAIVision[nCurrentEntity].playerLastKnownLocation = tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix;
			tThisWorld.atAIVision[nCurrentEntity].keepSearching = false;
			}
			else {
			spotedplayer = false;
			tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask = COMPONENT_AIMASK | COMPONENT_SEARCH;
			}*/
			if (spottedplayer == false)
			{
				XMFLOAT4 blue;
				blue.y = 0;
				blue.z = 1;
				blue.w = 1;
				blue.x = 0;
				tThisWorld.atSimpleMesh[8].m_nColor = blue;
			}
		}

		if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_CLIP))
		{
			if (tThisWorld.atClip[nCurrentEntity].GunMode == false && tThisWorld.atClip[nCurrentEntity].tryToShoot == true)
			{
				if (tThisWorld.atClip[nCurrentEntity].maderay == false)
				{
					XMMATRIX gun = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
					gun.r[3].m128_f32[0] += 1;
					rayindex = CreateRayBullet(&tThisWorld, gun, 10, PlayerStartIndex, -0.6f, -0.1f, 10.7f);
					pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, rayindex);
					tThisWorld.atClip[nCurrentEntity].maderay = true;
				}

			}
			else if (tThisWorld.atClip[nCurrentEntity].GunMode == false && tThisWorld.atClip[nCurrentEntity].tryToShoot == false && tThisWorld.atClip[nCurrentEntity].maderay == true)
			{
				pcGraphicsSystem->CleanD3DObject(&tThisWorld, rayindex);
				rayindex = -1;
				tThisWorld.atClip[nCurrentEntity].maderay = false;

			}
			else
			{
				if (nCurrentEntity == 10 && tThisWorld.atClip[nCurrentEntity].tryToShoot == true && tThisWorld.atClip[nCurrentEntity].fShootingCoolDown >= 0) {
					float x = 0;
				}
				if (tThisWorld.atClip[nCurrentEntity].tryToShoot == true && tThisWorld.atClip[nCurrentEntity].nBulletsAvailables.size() > 0 && tThisWorld.atClip[nCurrentEntity].fShootingCoolDown <= 0)
				{
					XMVECTOR foward;
					foward.m128_f32[0] = 0;
					foward.m128_f32[1] = 0;
					foward.m128_f32[2] = 1;

					XMMATRIX localMatrix2 = XMMatrixTranslationFromVector(foward);
					XMMATRIX gunMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
					gunMatrix = XMMatrixMultiply(localMatrix2, gunMatrix);

					int newbullet = CreateBullet(&tThisWorld, gunMatrix,
						tThisWorld.atClip[nCurrentEntity].currentMaterial);
					tThisWorld.atClip[newbullet].gunIndex = nCurrentEntity;
					tThisWorld.atSimpleMesh[newbullet].m_nColor = tThisWorld.atClip[nCurrentEntity].colorofBullets;
					tThisWorld.atClip[newbullet].indexInclip = pcProjectileSystem->CreateBulletProjectile(newbullet, &tThisWorld.atClip[nCurrentEntity]);
					tThisWorld.atAABB[newbullet] = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[newbullet].m_VertexData, tThisWorld.atAABB[newbullet]);
					tThisWorld.atAABB[newbullet].m_IndexLocation = newbullet;

					pcCollisionSystem->AddAABBCollider(tThisWorld.atAABB[newbullet], newbullet);
					pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, newbullet);

					tThisWorld.atClip[nCurrentEntity].tryToShoot = false;

				}
				else if (tThisWorld.atClip[nCurrentEntity].tryToShoot == true)
				{
					tThisWorld.atClip[nCurrentEntity].tryToShoot = false;
				}
				if (tThisWorld.atClip[nCurrentEntity].tryToReload == true)
				{
					pcProjectileSystem->Reload(&tThisWorld.atClip[nCurrentEntity]);
					tThisWorld.atClip[nCurrentEntity].tryToReload = false;

				}
				if (tThisWorld.atClip[nCurrentEntity].nBulletsFired.size() != 0)
				{
					for (int i = 0; i < tThisWorld.atClip[nCurrentEntity].nBulletsFired.size(); ++i)
					{
						if (tThisWorld.atClip[nCurrentEntity].fAliveTime[i] < tThisWorld.atClip[nCurrentEntity].FValueOfCoolDown)
						{
							pcPhysicsSystem->AddBulletForce(&tThisWorld.atRigidBody[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]], tThisWorld.atClip[nCurrentEntity].bulletSpeed);
							tThisWorld.atClip[nCurrentEntity].fAliveTime[i] += 0.1f;

						}
						else
						{
							tThisWorld.atClip[nCurrentEntity].fAliveTime.erase(tThisWorld.atClip[nCurrentEntity].fAliveTime.begin() + i);
							pcCollisionSystem->RemoveAABBCollider(tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]);
							pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]);
							tThisWorld.atClip[nCurrentEntity].nBulletsFired.erase(tThisWorld.atClip[nCurrentEntity].nBulletsFired.begin() + i);
							for (int i = 0; i < tThisWorld.atClip[nCurrentEntity].fAliveTime.size(); ++i)
							{
								if (tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]].indexInclip != 0)
								{
									tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]].indexInclip -= 1;
								}

							}
						}
					}

				}
				if (tThisWorld.atClip[nCurrentEntity].fShootingCoolDown > 0)
				{
					tThisWorld.atClip[nCurrentEntity].fShootingCoolDown -= 0.1f;
				}
			}
		}

		if (tThisWorld.atPhysicsMask[nCurrentEntity].m_tnPhysicsMask == (COMPONENT_PHYSICSMASK | COMPONENT_RIGIDBODY))
		{
			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, true);
		}

		if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_RAYGUN))
		{
			float CloseEstObject = 10000000000000000000.0f;
			float* distanceCalucaltion = new float();
			for (list<TAABB>::iterator ptr = pcCollisionSystem->m_AAbb.begin(); ptr != pcCollisionSystem->m_AAbb.end(); ++ptr)
			{

				if (pcCollisionSystem->IsLineInBox(XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[0], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix)
					, XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[1], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix),
					tThisWorld.atWorldMatrix[ptr->m_IndexLocation].worldMatrix, *ptr, distanceCalucaltion) == true && *distanceCalucaltion < CloseEstObject)
				{
					CloseEstObject = *distanceCalucaltion;
					tThisWorld.atClip[GunIndexForPlayer].currentMaterial = 0;

					tThisWorld.atClip[GunIndexForPlayer].colorofBullets = tThisWorld.atSimpleMesh[ptr->m_IndexLocation].m_nColor;
				}
			}
			delete distanceCalucaltion;
		}

		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
			COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC) || tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
				COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))
		{
			tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
		}


		if ((tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_TRIGGER)
			|| tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)))
		{
			vector<int> otherCollisionsIndex;
			if (pcCollisionSystem->AABBtoAABBCollisionCheck(tThisWorld.atAABB[nCurrentEntity], &otherCollisionsIndex) == true)
			{
				for (int i = 0; i < otherCollisionsIndex.size(); ++i)
				{
					if (tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == true
						&& tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask !=
						(COMPONENT_COLLISIONMASK | COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))
					{
						tThisWorld.atRigidBody[nCurrentEntity].totalForce = -tThisWorld.atRigidBody[nCurrentEntity].velocity;

						tTempVertexBuffer.m_d3dWorldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcCollisionSystem->WalkingThrewObjectCheck(tTempVertexBuffer.m_d3dWorldMatrix, tThisWorld.atAABB[otherCollisionsIndex[i]], tThisWorld.atAABB[nCurrentEntity]);
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tTempVertexBuffer.m_d3dWorldMatrix;
					}
					if (tThisWorld.atRigidBody[nCurrentEntity].ground == false
						&& tThisWorld.atRigidBody[nCurrentEntity].wall == false
						&& tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)
						&& (tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_STATIC)
							|| tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
								COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)))
					{
						tTempVertexBuffer.m_d3dWorldMatrix = pcCollisionSystem->WalkingThrewObjectCheck(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[otherCollisionsIndex[i]], tThisWorld.atAABB[nCurrentEntity]);
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tTempVertexBuffer.m_d3dWorldMatrix;
						if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_INPUTMASK | COMPONENT_CLAYTON))
						{
							m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
						}
					}
					if (tThisWorld.atRigidBody[nCurrentEntity].ground == false
						&& tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == false
						&& tThisWorld.atRigidBody[nCurrentEntity].wall == false
						&& tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)
						&& (tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_STATIC))
						|| tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))
					{
						if (tThisWorld.atAIMask[otherCollisionsIndex[i]].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_FOLLOW))
						{
							pcAiSystem->SetNumberOfAI(pcAiSystem->GetNumberOfAI() - 1);
							pcCollisionSystem->RemoveAABBCollider(otherCollisionsIndex[i]);
							pcGraphicsSystem->CleanD3DObject(&tThisWorld, otherCollisionsIndex[i]);
						}
						if (tThisWorld.atInputMask[otherCollisionsIndex[i]].m_tnInputMask == (COMPONENT_INPUTMASK | COMPONENT_CLAYTON))
						{
							if (tThisWorld.atAIMask[otherCollisionsIndex[i]].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_FOLLOW)) 
							{
								pcAiSystem->SetNumberOfAI(pcAiSystem->GetNumberOfAI() - 1);
								if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1)
								{
									tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.erase
									(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.begin()
										+ tThisWorld.atClip[nCurrentEntity].indexInclip);

									pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
									tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.erase(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.begin()
										+ tThisWorld.atClip[nCurrentEntity].indexInclip);
									pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);
								}
								pcCollisionSystem->RemoveAABBCollider(otherCollisionsIndex[i]);
								pcGraphicsSystem->CleanD3DObject(&tThisWorld, otherCollisionsIndex[i]);
								pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atAIMask[otherCollisionsIndex[i]].GunIndex);
							}
							if (tThisWorld.atInputMask[otherCollisionsIndex[i]].m_tnInputMask == (COMPONENT_INPUTMASK | COMPONENT_CLAYTON))
							{
								if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL))
								{
									if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1)
									{
										tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.erase
										(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.begin()
											+ tThisWorld.atClip[nCurrentEntity].indexInclip);

										pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
										tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.erase(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.begin()
											+ tThisWorld.atClip[nCurrentEntity].indexInclip);

										//pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atAIMask[nCurrentEntity].GunIndex);
										pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);

									}
								}
								tThisWorld.atClayton[otherCollisionsIndex[i]].health -= 50;
							}
						}
					}
				}
				if (tThisWorld.atClayton[PlayerStartIndex].health <= 0)
				{
					return -1;
				}
				tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			}
			else
			{
				tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;
			}
			tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;
		}
		else
		{
			tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;
		}

		if (tThisWorld.atParentWorldMatrix[nCurrentEntity] != -1)
		{
			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(pcGraphicsSystem->SetDefaultWorldPosition(),
				tThisWorld.atWorldMatrix[tThisWorld.atParentWorldMatrix[nCurrentEntity]].worldMatrix);

			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(tThisWorld.atOffSetMatrix[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
		}

		#pragma region Graphics Checks
		
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID))
		{
			if (tCameraMode.bWalkMode == true)
			{
				pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity], walkCamera->d3d_Position);

			}
			else if (tCameraMode.bAimMode == true)
			{
				pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity], aimCamera->d3d_Position);

			}
			else
			{
				pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity], debugCamera->d3d_Position);
			}
		
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atSimpleMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

#pragma endregion
	
	}

	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);
	zValue += 0.001f;
	return 2;
}

void CGameMangerSystem::RestartLevel()
{
}

int CGameMangerSystem::LoadMainMenu()
{
	bool switchlevel = false;


	 XMMATRIX m_d3d_ResultMatrix2 = pcGraphicsSystem->SetDefaultWorldPosition();
	 XMMATRIX m_d3dOffsetMatrix2 = pcGraphicsSystem->SetDefaultOffset();
	if (pcInputSystem->InputCheck(G_KEY_P)) {
		return 3;
	}
	if (pcInputSystem->InputCheck(G_KEY_ESCAPE))
	{
		//pcCollisionSystem->m_AAbb.clear();
		pcGraphicsSystem->CleanD3DLevel(&tThisWorld);

		//	pcGraphicsSystem->CleanD3D(&tThisWorld);
		return 4;
	}

//	bool switchlevel = false;

			
	//		UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
	//		pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
	//	}
	//}


	

	
		m_d3dPlayerMatrix = pcInputSystem->AimMode(m_d3dPlayerMatrix);

		m_d3d_ResultMatrix2 = XMMatrixMultiply(m_d3d_ResultMatrix2, m_d3dPlayerMatrix);

		 menuCamera->d3d_Position = XMMatrixMultiply(m_d3dOffsetMatrix2, m_d3d_ResultMatrix2);
		// m_d3dCameraMatrix = XMMatrixMultiply(m_d3dOffsetMatrix, m_d3dCameraMatrix);

	CGraphicsSystem::TPrimalVertexBufferType tTempVertexBuffer;
	CGraphicsSystem::TPrimalPixelBufferType tTempPixelBuffer;
	tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

	pcGraphicsSystem->UpdateD3D();

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tTempVertexBuffer.m_d3dViewMatrix = menuCamera->d3d_Position;

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		{
			pcGraphicsSystem->InitPrimalShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], menuCamera->d3d_Position);

			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

		tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, true);

		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == 
			(COMPONENT_COLLISIONMASK | COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC) 
			|| tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == 
			(COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)) 
		{
			tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
		}

		if ((tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == 
			(COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_TRIGGER) 
			|| tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == 
			(COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)))
		{
			vector<int> otherCollisionsIndex;
			if (pcCollisionSystem->AABBtoAABBCollisionCheck(tThisWorld.atAABB[nCurrentEntity], &otherCollisionsIndex) == true)
			{
				for (int i = 0; i < otherCollisionsIndex.size(); ++i) {
					if (tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == true && tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask !=
						(COMPONENT_COLLISIONMASK |
							COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)) 
					{
						tThisWorld.atRigidBody[nCurrentEntity].totalForce = -tThisWorld.atRigidBody[nCurrentEntity].velocity;

						tTempVertexBuffer.m_d3dWorldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcCollisionSystem->WalkingThrewObjectCheck(tTempVertexBuffer.m_d3dWorldMatrix, tThisWorld.atAABB[otherCollisionsIndex[i]], tThisWorld.atAABB[nCurrentEntity]);
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tTempVertexBuffer.m_d3dWorldMatrix;

					}
					if (tThisWorld.atRigidBody[nCurrentEntity].ground == false && tThisWorld.atRigidBody[nCurrentEntity].wall == false &&
						tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)
						&& (tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_STATIC) || tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
								COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))) 
					{
						tTempVertexBuffer.m_d3dWorldMatrix = pcCollisionSystem->WalkingThrewObjectCheck(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[otherCollisionsIndex[i]], tThisWorld.atAABB[nCurrentEntity]);
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tTempVertexBuffer.m_d3dWorldMatrix;
						if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_INPUTMASK | COMPONENT_CLAYTON)) 
						{
							m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
						}
					}
					else if (tThisWorld.atRigidBody[nCurrentEntity].ground == false && tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == false && tThisWorld.atRigidBody[nCurrentEntity].wall == false &&
						tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)
						&& (tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_STATIC)) || tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
								COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))
					{
						if (tThisWorld.atAABB[otherCollisionsIndex[i]].m_SceneChange != -30) 
						{
							switchlevel = true;
							break;
						}
					}
				}

				tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			}
			else 
			{
				tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;
			}
		}
		else 
		{
			tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;
		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID))
		{
			
			if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_INPUTMASK | COMPONENT_CLAYTON))
			{
				
					m_d3dPlayerMatrix = pcInputSystem->CharacterMovement(m_d3dPlayerMatrix);

					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
					m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;

			}

			tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
			tTempVertexBuffer.m_d3dViewMatrix = menuCamera->d3d_Position;


			pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity], menuCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atSimpleMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}
	}

	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);

	if (pcInputSystem->InputCheck(G_KEY_ENTER) == 1 
		|| switchlevel == true 
		&& pcInputSystem->InputCheck(G_KEY_ENTER) == 0) 
	{
		pcGraphicsSystem->CleanD3DLevel(&tThisWorld);
		pcCollisionSystem->m_AAbb.clear();

		return 1;
	}
	else 
	{ 
		return 0; 
	}
}

void CGameMangerSystem::InitilizeMainMenu()
{
	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);

	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
//	m_d3dCameraMatrix = pcGraphicsSystem->SetDefaultCameraMatrix();
	menuCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();
	tCameraMode.bDebugMode = false;
	tCameraMode.bAimMode = true;
	tCameraMode.bWalkMode = false;
	tCameraMode.bSwitch = false;
	m_d3dPlayerMatrix = pcGraphicsSystem->SetDefaultWorldPosition();



	PlayerStartIndex = CreateClayTon(&tThisWorld);
	XMMATRIX AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[2] += -5;
	AILocation.r[3].m128_f32[0] += -5;
	SpawnLevelChanger(&tThisWorld, AILocation);
	XMMATRIX wall = m_d3dWorldMatrix;
	wall.r[3].m128_f32[1] += -1;
	CreateWall(&tThisWorld, wall);
	wall = m_d3dWorldMatrix;
	wall.r[3].m128_f32[1] += -1;
	wall.r[3].m128_f32[0] += -22;
	CreateWall(&tThisWorld, wall);
	XMMATRIX celling = m_d3dWorldMatrix;
	celling.r[3].m128_f32[1] += 10;
	CreateCelling(&tThisWorld, celling);
	XMMATRIX groundSpawnPoint;
	groundSpawnPoint = m_d3dWorldMatrix;
	groundSpawnPoint.r[3].m128_f32[1] -= 2;
	CreateGround(&tThisWorld, groundSpawnPoint);
	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask > 1)
		{
			if (tThisWorld.atSimpleMesh[nCurrentEntity].m_nVertexCount > tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount)
			{
				TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[nCurrentEntity].m_VertexData, tThisWorld.atAABB[nCurrentEntity]);
				MyAbb.m_IndexLocation = nCurrentEntity;
				tThisWorld.atAABB[nCurrentEntity] = MyAbb;
				pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);
			}
			else
			{
				/* TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData);
				MyAbb.m_IndexLocation = nCurrentEntity;
				MyAbb.m_dMaxPointOrginal.y += 1;
				tThisWorld.atAABB[nCurrentEntity] = MyAbb;
				pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);*/
			}
		}
	}

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++) {
		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask > 1) {
			tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
		}
	}
	pcGraphicsSystem->CreateBuffers(&tThisWorld);
}
