#include "GameManger.h"
#define AI_ON true
#define MIKES_SANDBOX_ON false
#define SKELETON_LOAD_ON false
#define MAIN_LEVEL_ON true
CGameMangerSystem::CGameMangerSystem(HWND window,CInputSystem* _pcInputSystem)
{
	cApplicationWindow = window;
	pcGraphicsSystem = new CGraphicsSystem();
	pcInputSystem = _pcInputSystem;
	pcInputSystem->SendHWNDToInputSystem(cApplicationWindow);
	pcCollisionSystem = new CCollisionSystem();
	pcPhysicsSystem = new CPhysicsSystem();
	pcProjectileSystem = new CProjectileSystem();
	pcAiSystem = new CAISystem();
	pcUISystem = new CUISystem();
	pcGraphicsSystem->InitD3D(cApplicationWindow);
//	tAugerTimers = new Timers();
	tTimerInfo = new System_Times();
	aimCamera = new TCamera();
	walkCamera = new TCamera();
	debugCamera = new TCamera();
	menuCamera = new TCamera();
	pcAudioSystem = new CAudioSystem();
	//srand(time(NULL));

	GetWindowRect(cApplicationWindow, &windowRect);

	screenWidth = windowRect.right - windowRect.left;
	screenHeight = windowRect.bottom - windowRect.top;
}

CGameMangerSystem::~CGameMangerSystem()
{
	pcGraphicsSystem->CleanD3D(&tThisWorld);
	pcAudioSystem->TermSoundEngine();
	delete pcGraphicsSystem;
	delete pcInputSystem;
	delete pcCollisionSystem;
	delete pcPhysicsSystem;
	delete pcProjectileSystem;
	delete pcAiSystem;
	//delete tAugerTimers;
	delete tTimerInfo;
	delete aimCamera;
	delete debugCamera;
	delete walkCamera;
	delete menuCamera;
	delete pcAudioSystem;
	delete pcUISystem;

	fontTexture->Release();
	atUIVertices.clear();
	atUIIndices.clear();
}

void CGameMangerSystem::LoadLevel()
{
	// master	
	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);
	for (int i = 0; i < matOpt.numberOfMaterials; i++)
	{
		matOpt.SRVArrayOfMaterials[i]->Release();
	}
	InitializeHUD();
	InitializePauseScreen();
	InitializeEndScreen();
	GameOver = false;
	GamePaused = false;

	pcAiSystem->SetNumberOfAI(2);
	//tTimerInfo->StartClock(tAugerTimers->tSceneTimer);
	ImporterData tempImport;
	TMaterialOptimized matOpt;

	#pragma region Create Skybox
	ID3D11Resource * spaceMap[6];


	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceBlue/bkg1_back.png", &spaceMap[0], NULL, NULL);
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceBlue/bkg1_left.png", &spaceMap[1], NULL, NULL);
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceBlue/bkg1_right.png", &spaceMap[2], NULL, NULL);

	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceBlue/bkg1_front.png", &spaceMap[3], NULL, NULL);
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceBlue/bkg1_top.png", &spaceMap[4], NULL, NULL);
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceBlue/bkg1_bot.png", &spaceMap[5], NULL, NULL);

	ID3D11ShaderResourceView * tempSrv = pcGraphicsSystem->TexturesToCubeMap(pcGraphicsSystem->m_pd3dDeviceContext, spaceMap);
	renderToTexturePassIndex = CreateSkybox(&tThisWorld, tempSrv);

#pragma endregion
	
	tempImport = pcGraphicsSystem->ReadMesh("meshData_NoBrewery7.txt");
	matOpt = pcGraphicsSystem->CreateTexturesFromFile(tempImport.vtMaterials, tempImport.meshCount);
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], matOpt, meshIndex);
	}

	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
	walkCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	aimCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	debugCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective(m_RealTimeFov);
	m_d3dPlayerMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	tCameraMode.bWalkMode = true;
	tCameraMode.bDebugMode = false;
	tCameraMode.bAimMode = false;
	tCameraMode.bSwitch = false;
	bMoving = false;
	m_RealTimeFov = 90.0f;
	//m_d3dPlayerMatrix = XMMatrixMultiply(m_d3dPlayerMatrix, XMMatrixScaling(.01, .01, .01));

	m_d3dPlayerMatrix.r[3].m128_f32[2] -= 10;
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
	//CreateSimpleGunAi(&tThisWorld, AILocation);


	AimingLine(&tThisWorld, m_d3dWorldMatrix, PlayerStartIndex, -1, 1, 10.5);


	#pragma region Gun & AI Init
	XMMATRIX AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[2] += -5;
	AILocation.r[3].m128_f32[0] += -5;
	GunIndexForPlayer = CreateGun(&tThisWorld, m_d3dWorldMatrix, PlayerStartIndex, -1, 1, 10.5,3,100);
	tThisWorld.atClip[GunIndexForPlayer].bulletSpeed = 0.001;
	AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[0] += -1;
	AILocation.r[3].m128_f32[2] += -2;
	int AiIndex=CreateSimpleGunAi(&tThisWorld, AILocation);

	createGSQuad(&tThisWorld, XMFLOAT4(1, 0, 0, 1), AiIndex);
	createGSQuad(&tThisWorld, XMFLOAT4(0, 0, 0, 1), AiIndex);

	int GunINdexai=CreateGun(&tThisWorld, m_d3dWorldMatrix, AiIndex, -1.1, 0, 11,10,100);
	tThisWorld.atAIMask[AiIndex].GunIndex = GunINdexai;

	tThisWorld.atClip[GunINdexai].bulletSpeed = 0.0001;//Frame Dependent

	AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[0] += -3;
	AILocation.r[3].m128_f32[2] += -5;
	int SimpleAi2 = CreateSimpleSearchAi(&tThisWorld, AILocation);

	createGSQuad(&tThisWorld, XMFLOAT4(1, 0, 0, 1), SimpleAi2);
	createGSQuad(&tThisWorld, XMFLOAT4(0, 0, 0, 1), SimpleAi2);

//	CreateAIVision(&tThisWorld, AILocation, 8, 8, -0.6, -0.1, 10.7);
	int gun2AI = CreateGun(&tThisWorld, m_d3dWorldMatrix, SimpleAi2, -1.1, 0, 11, 10, 200);
	tThisWorld.atClip[gun2AI].bulletSpeed = 0.0001;
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
#pragma endregion

	//CreateSimpleGunAi(&tThisWorld, AILocation);
	XMMATRIX groundSpawnPoint;
	groundSpawnPoint = m_d3dWorldMatrix;
	groundSpawnPoint.r[3].m128_f32[1] -= 2;
	CreateGround(&tThisWorld, groundSpawnPoint);

	//tempImport = pcGraphicsSystem->ReadMesh("meshData_Example_Objects.txt");

	//for (int meshIndex = 1; meshIndex < tempImport.meshCount; meshIndex++)
	//{
	//	//int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	//}

	//tempImport = pcGraphicsSystem->ReadMesh("meshData_ScifiRoom.txt");

	//for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	//{
	//	//int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	//}

	#pragma region Second Camera Init
	std::array<plane_t, 6> planes;
	float4x4 secondcamera;
	secondcamera.row1.x = 1;
	secondcamera.row1.y = 0;
	secondcamera.row1.z = 0;
	secondcamera.row1.w = 0;
	secondcamera.row2.x = 0;
	secondcamera.row2.y = cos((-18 * 3.14) / 180.0f);
	secondcamera.row2.z = -sin((-18 * 3.14) / 180.0f);
	secondcamera.row2.w = 0;
	secondcamera.row3.x = 0;
	secondcamera.row3.y = sin((-18 * 3.14) / 180.0f);
	secondcamera.row3.z = cos((-18 * 3.14) / 180.0f);
	secondcamera.row3.w = 0;
	secondcamera.row4.x = 0;
	secondcamera.row4.y = 0;
	secondcamera.row4.z = -5;
	secondcamera.row4.w = 1;
	 
	secondCam.r[0].m128_f32[0] = 1;
	secondCam.r[0].m128_f32[1] = 0;
	secondCam.r[0].m128_f32[2] = 0;
	secondCam.r[0].m128_f32[3] = 0;

	secondCam.r[1].m128_f32[0] = 0;
	secondCam.r[1].m128_f32[1] = cos((-18 * 3.14) / 180.0f);
	secondCam.r[1].m128_f32[2] = -sin((-18 * 3.14) / 180.0f);
	secondCam.r[1].m128_f32[3] = 0;


	secondCam.r[2].m128_f32[0] = 0;
	secondCam.r[2].m128_f32[1] = sin((-18 * 3.14) / 180.0f);
	secondCam.r[2].m128_f32[2] = cos((-18 * 3.14) / 180.0f);
	secondCam.r[2].m128_f32[3] = 0;

	secondCam.r[3].m128_f32[0] = 0;
	secondCam.r[3].m128_f32[1] = 0;
	secondCam.r[3].m128_f32[2] = -5;
	secondCam.r[3].m128_f32[3] = 1;

	secondcamera.row1.x = tThisWorld.atWorldMatrix[SimpleAi2].worldMatrix.r[0].m128_f32[0];
	secondcamera.row1.y = tThisWorld.atWorldMatrix[SimpleAi2].worldMatrix.r[0].m128_f32[1];
	secondcamera.row1.z = tThisWorld.atWorldMatrix[SimpleAi2].worldMatrix.r[0].m128_f32[2];
	secondcamera.row1.w = tThisWorld.atWorldMatrix[SimpleAi2].worldMatrix.r[0].m128_f32[3];

	secondcamera.row2.x = tThisWorld.atWorldMatrix[SimpleAi2].worldMatrix.r[1].m128_f32[0];
	secondcamera.row2.y = tThisWorld.atWorldMatrix[SimpleAi2].worldMatrix.r[1].m128_f32[1];
	secondcamera.row2.z = tThisWorld.atWorldMatrix[SimpleAi2].worldMatrix.r[1].m128_f32[2];
	secondcamera.row2.w = tThisWorld.atWorldMatrix[SimpleAi2].worldMatrix.r[1].m128_f32[3];

	secondcamera.row3.x = tThisWorld.atWorldMatrix[SimpleAi2].worldMatrix.r[2].m128_f32[0];
	secondcamera.row3.y = tThisWorld.atWorldMatrix[SimpleAi2].worldMatrix.r[2].m128_f32[1];
	secondcamera.row3.z = tThisWorld.atWorldMatrix[SimpleAi2].worldMatrix.r[2].m128_f32[2];
	secondcamera.row3.w = tThisWorld.atWorldMatrix[SimpleAi2].worldMatrix.r[2].m128_f32[3];

	secondcamera.row4.x = tThisWorld.atWorldMatrix[SimpleAi2].worldMatrix.r[3].m128_f32[0];
	secondcamera.row4.y = tThisWorld.atWorldMatrix[SimpleAi2].worldMatrix.r[3].m128_f32[1];
	secondcamera.row4.z = tThisWorld.atWorldMatrix[SimpleAi2].worldMatrix.r[3].m128_f32[2];
	secondcamera.row4.w = tThisWorld.atWorldMatrix[SimpleAi2].worldMatrix.r[3].m128_f32[3];

	frustumIndex=pcAiSystem->calculate_frustum(&tThisWorld, planes, secondcamera, 70, 1, 0.1, 20, SimpleAi2, -2.1, 1.4, 19.6);
#pragma endregion

	#pragma region AI Init
	tThisWorld.atWorldMatrix[frustumIndex].worldMatrix = AILocation;
	
	tThisWorld.atAIVision[SimpleAi2].eyes0 = planes;
	tThisWorld.atAIVision[SimpleAi2].normalAtBegining[0] = planes[0].normal;
	tThisWorld.atAIVision[SimpleAi2].normalAtBegining[1] = planes[1].normal;
	tThisWorld.atAIVision[SimpleAi2].normalAtBegining[2] = planes[2].normal;
	tThisWorld.atAIVision[SimpleAi2].normalAtBegining[3] = planes[3].normal;
	tThisWorld.atAIVision[SimpleAi2].normalAtBegining[4] = planes[4].normal;
	tThisWorld.atAIVision[SimpleAi2].normalAtBegining[5] = planes[5].normal;
	AILocation.r[3].m128_f32[0] -= 7;
	int nodeLocation = CreateNodePoint(&tThisWorld, AILocation);
	XMFLOAT3 nodePosition;
	nodePosition.x = AILocation.r[3].m128_f32[0];
	nodePosition.y = AILocation.r[3].m128_f32[1];
	nodePosition.z = AILocation.r[3].m128_f32[2];

	pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition,1);
	AILocation.r[3].m128_f32[0] += 4;
	AILocation.r[3].m128_f32[2] += -4;

	int nodeLocation2 = CreateNodePoint(&tThisWorld, AILocation);

	nodePosition.x = AILocation.r[3].m128_f32[0];
	nodePosition.y = AILocation.r[3].m128_f32[1];
	nodePosition.z = AILocation.r[3].m128_f32[2];

	pcAiSystem->AddNodeToPathFinding(nodeLocation2, nodePosition, 0);
	XMMATRIX pathfindAI = AILocation;
	pathfindAI.r[3].m128_f32[0] += 3;
	nodePosition.x = pathfindAI.r[3].m128_f32[0];
	nodePosition.y = pathfindAI.r[3].m128_f32[1];
	nodePosition.z = pathfindAI.r[3].m128_f32[2];
	int aiPathIndex =CreateTestAIPathFinding(&tThisWorld, pathfindAI);
	tThisWorld.atPathPlanining[aiPathIndex].startingNode = 21;
	tThisWorld.atPathPlanining[aiPathIndex].Goal = 18;

	int nodelocation3  = CreateNodePoint(&tThisWorld, pathfindAI);
	pcAiSystem->AddNodeToPathFinding(nodelocation3, nodePosition, 1);
	vector<int> edges;
	edges.push_back(nodeLocation2);
	edges.push_back(nodelocation3);
	pcAiSystem->AddEdgestoNode(nodeLocation, edges);
	edges.clear();
	edges.push_back(nodeLocation);
	edges.push_back(nodelocation3);
	pcAiSystem->AddEdgestoNode(nodeLocation2, edges);
	edges.clear();
//	edges.push_back(nodeLocation);
	edges.push_back(nodeLocation2);
	pcAiSystem->AddEdgestoNode(nodelocation3, edges);


#pragma endregion

	#pragma region Collision Init
	if (pcCollisionSystem->m_AAbb.size() !=0 ) 
	{
		float x = 0;
	}
	XMFLOAT4 blue;
	blue.y = 0;
	blue.z = 1;
	blue.w = 1;
	blue.x = 0;
	tThisWorld.atSimpleMesh[8].m_nColor = blue;
	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++) 
	{
		
		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask > 1) 
		{
			if (tThisWorld.atSimpleMesh[nCurrentEntity].m_nVertexCount > tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount) 
			{
				TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[nCurrentEntity].m_VertexData,tThisWorld.atAABB[nCurrentEntity]);
				MyAbb.m_IndexLocation = nCurrentEntity;
				tThisWorld.atAABB[nCurrentEntity] = MyAbb;
				pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);
				if (nCurrentEntity == PlayerStartIndex) {
					pcCollisionSystem->AddAiVisioNCheck(MyAbb, nCurrentEntity);

				}
			
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

#pragma endregion

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
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(-1); //Important!
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective(m_RealTimeFov);

	//tTimerInfo->applicationTime = tTimerInfo->GetTime(tAugerTimers->tAppTimer, tTimerInfo->applicationTime);
	//tTimerInfo->sceneTime = tTimerInfo->GetTime(tAugerTimers->tSceneTimer, tTimerInfo->sceneTime);

	tCameraMode = pcInputSystem->CameraModeListen(tCameraMode);

	static XMMATRIX m_d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	static XMMATRIX m_d3dOffsetMatrix = pcGraphicsSystem->SetDefaultOffset();
	 
	if (pcInputSystem->InputCheck(G_KEY_P) && !GameOver) 
	{
		GamePaused = true;
	}
	if (pcInputSystem->InputCheck(G_KEY_U) && !options && !GameOver)
	{
		GamePaused = false;
	}

	clickTimer.Signal();

	// ui stuff
	//if (GamePaused == true) {
	//	if (DrawUI == true) {
	//		XMMATRIX UiPos = tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix;
	//		//UiPos.r[3].m128_f32[1] -= 1;
	//		XMVECTOR foward;
	//		foward.m128_f32[0] = 0;
	//		foward.m128_f32[1] = 1;
	//		foward.m128_f32[2] = 2;
	//		//	foward.m128_f32[0] = 1;
	//		XMMATRIX localMatrix2 = XMMatrixTranslationFromVector(foward);
	//		UiPos = XMMatrixMultiply(localMatrix2, UiPos);
	//		DrawUI = false;
	//		UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
	//		pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
	//		UiPos.r[3].m128_f32[1] -= 1;
	//		UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
	//		pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
	//		UiPos.r[3].m128_f32[1] += 1;
	//		UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
	//		pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
	//		UiPos.r[3].m128_f32[1] += 1;
	//		UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
	//		pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
	//		UiPos.r[3].m128_f32[1] += 1;
	//		UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
	//		pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
	//		UiPos.r[3].m128_f32[0] += 1;
	//		UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
	//		pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
	//		UiPos.r[3].m128_f32[0] += 0.8;
	//		UiPos.r[3].m128_f32[1] -= 0.3;
	//		UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
	//		pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
	//		UiPos.r[3].m128_f32[1] -= 0.7;
	//		UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
	//		pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
	//		UiPos.r[3].m128_f32[1] -= 0.7;
	//		UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
	//		pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
	//		UiPos.r[3].m128_f32[0] -= 1;
	//		UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
	//		pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
	//	}
	//}

	if (!GamePaused && !GameOver)
	{
		if (tCameraMode.bWalkMode == true)
		{
			if (tCameraMode.bSwitch == true)
			{
				m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
				m_d3dOffsetMatrix = pcGraphicsSystem->SetDefaultOffset();
				tCameraMode.bSwitch = false;
			}
			m_d3d_ResultMatrix = pcInputSystem->WalkCameraControls(XMVectorSet(0, 1.0f, 0, 0), m_d3d_ResultMatrix, bMoving);

			walkCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);
			walkCamera->d3d_Position = XMMatrixMultiply(m_d3dOffsetMatrix, walkCamera->d3d_Position);






		}
		else if (tCameraMode.bAimMode == true)
		{
			m_RealTimeFov = pcInputSystem->ZoomSight(m_RealTimeFov);
			if (tCameraMode.bSwitch == true)
			{
				m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
				//m_d3d_ResultMatrix = pcInputSystem->CameraBehaviorLerp(m_d3d_ResultMatrix, m_d3dPlayerMatrix);
				m_d3dOffsetMatrix = pcGraphicsSystem->ResetAimModeCameraOffset();
				tCameraMode.bSwitch = false;
			}


			m_d3dPlayerMatrix = pcInputSystem->AimMode(m_d3dPlayerMatrix);

			aimCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);
			aimCamera->d3d_Position = XMMatrixMultiply(m_d3dOffsetMatrix, aimCamera->d3d_Position);
		}
		if (tCameraMode.bSwitch == true)
		{
			if (tCameraMode.bSwitch == true)
			{
				m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
				tCameraMode.bSwitch = false;
			}
			m_d3d_ResultMatrix = pcInputSystem->DebugCamera(m_d3d_ResultMatrix, m_d3dWorldMatrix);
			debugCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dWorldMatrix);
		}
	}

	CGraphicsSystem::TPrimalVertexBufferType tTempVertexBuffer;
	CGraphicsSystem::TPrimalPixelBufferType tTempPixelBuffer;
	CGraphicsSystem::TMyVertexBufferType tMyVertexBufferTemp;
	tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

	pcGraphicsSystem->UpdateD3D();
	pcGraphicsSystem->UpdateD3D_RenderToTexture();

	for (int nCurrentEntity = 0; nCurrentEntity <= renderToTexturePassIndex; nCurrentEntity++)
	{
		tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		//Possible Spaceships
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
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
		//Skybox
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_SKYBOX | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			if (tCameraMode.bWalkMode == true)
			{
				tMyVertexBufferTemp.m_d3dViewMatrix = walkCamera->d3d_Position;
				tTempVertexBuffer.m_d3dViewMatrix = walkCamera->d3d_Position;
				pcGraphicsSystem->InitSkyboxShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], walkCamera->d3d_Position);
			}
			else if (tCameraMode.bAimMode == true)
			{
				tTempVertexBuffer.m_d3dViewMatrix = aimCamera->d3d_Position;
				tMyVertexBufferTemp.m_d3dViewMatrix = aimCamera->d3d_Position;
				pcGraphicsSystem->InitSkyboxShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], aimCamera->d3d_Position);
			}
			else
			{
				tTempVertexBuffer.m_d3dWorldMatrix = m_d3dWorldMatrix;
				tTempVertexBuffer.m_d3dViewMatrix = debugCamera->d3d_Position;
				tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;
				pcGraphicsSystem->InitSkyboxShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);
			}
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

		}

	}

	pcGraphicsSystem->m_pd3dDevice->CreateShaderResourceView(pcGraphicsSystem->m_pd3dOutsideGlassRenderToTexture, NULL, &pcGraphicsSystem->m_pd3dOutsideGlassSRV);
	pcGraphicsSystem->m_pd3dDeviceContext->GenerateMips(pcGraphicsSystem->m_pd3dOutsideGlassSRV);

	//Assign Glass Texture to Glass Entities
	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			if (tThisWorld.atShaderID[nCurrentEntity].m_nShaderID == 10)
			{
				tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse = pcGraphicsSystem->m_pd3dOutsideGlassSRV;
			}
		}
	}

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

		CGraphicsSystem::TUIVertexBufferType tUIVertexBuffer;
		CGraphicsSystem::TUIPixelBufferType tUIPixelBuffer;

		POINT hoverPoint;
		GetCursorPos(&hoverPoint);
		ScreenToClient(cApplicationWindow, &hoverPoint);

		POINT clickPoint = { -1, -1 };
		if (pcInputSystem->InputCheck(G_BUTTON_LEFT) == 1)
		{
			GetCursorPos(&clickPoint);
			ScreenToClient(cApplicationWindow, &clickPoint);

			if (GameOver && tThisWorld.atClayton[PlayerStartIndex].health > 0)
				return 2;
		}

		pcGraphicsSystem->UpdateD3D();

		GetWindowRect(cApplicationWindow, &windowRect);

		bool initializeTextThisFrame = false;
		for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
		{
			tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
			tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;


			if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SEARCH) || tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SPOTEDPLAYER)) {


				//if (tThisWorld.atAIVision[nCurrentEntity].keepSearching == true) {
				//	/*if (tThisWorld.atAIVision[nCurrentEntity].visionRotation < 7 && tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true) {
				//		tThisWorld.atAIVision[nCurrentEntity].visionRotation += 0.001;
				//		tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcAiSystem->LookBackLeftToRight(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix,
				//			true);
				//	}
				//	else if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true) {
				//		tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight = false;
				//	}
				//	else if (tThisWorld.atAIVision[nCurrentEntity].visionRotation > -7 && tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == false) {
				//		tThisWorld.atAIVision[nCurrentEntity].visionRotation -= 0.001;
				//		tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcAiSystem->LookBackLeftToRight(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix,
				//			false);
				//	}
				//	else if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == false) {
				//		tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight = true;
				//	}*/
				//}
				//float4x4 secondcamera;
				//secondcamera.row1.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[0];
				//secondcamera.row1.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[1];
				//secondcamera.row1.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[2];
				//secondcamera.row1.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[3];

				//secondcamera.row2.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[0];
				//secondcamera.row2.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[1];
				//secondcamera.row2.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[2];
				//secondcamera.row2.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[3];

				//secondcamera.row3.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[0];
				//secondcamera.row3.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[1];
				//secondcamera.row3.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[2];
				//secondcamera.row3.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[3];

				//secondcamera.row4.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[0];
				//secondcamera.row4.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1];
				//secondcamera.row4.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[2];
				//secondcamera.row4.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[3];
				//pcAiSystem->UpdateFrustum(tThisWorld.atAIVision[nCurrentEntity].eyes0, secondcamera, 70, 1, 0.1, 20);
				//	 pcAiSystem->calculate_frustum(&tThisWorld,tThisWorld.atAIVision[nCurrentEntity].eyes0, secondcamera,70,1,0.1,20, nCurrentEntity, -2.1, 1.4, 19.6);
				vector<int> indexes;
				if (pcCollisionSystem->AiVisionCheck(tThisWorld.atAIVision[nCurrentEntity].eyes0, &indexes) == true) 
				{
					float x = 0;

					tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
				}
				else 
				{
					tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
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

			}

			if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_CLIP))
			{
				if (tThisWorld.atClip[nCurrentEntity].GunMode == false && tThisWorld.atClip[nCurrentEntity].tryToShoot == true)
				{
					if (tThisWorld.atClip[nCurrentEntity].maderay == false)
					{
						XMMATRIX gun = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
						gun.r[3].m128_f32[0] += 1;
						rayindex = CreateRayBullet(&tThisWorld, gun, 10, PlayerStartIndex, -0.6, -0.1, 10.7);
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

						wchar_t* textBuffer = new wchar_t[1];

						textBuffer[0] = (tThisWorld.atClip[nCurrentEntity].nBulletsAvailables.size() - 1);

						pcUISystem->UpdateText(&tThisWorld, 1094, &atUIVertices, textBuffer, atUIVertices.at(tThisWorld.atLabel[1094].vIndex));

						pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, 1094);

						delete[] textBuffer;

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
					else if (tThisWorld.atClip[nCurrentEntity].tryToShoot == true) {
						tThisWorld.atClip[nCurrentEntity].tryToShoot = false;
					}
					if (tThisWorld.atClip[nCurrentEntity].tryToReload == true) {
						pcProjectileSystem->Reload(&tThisWorld.atClip[nCurrentEntity]);
						tThisWorld.atClip[nCurrentEntity].tryToReload = false;

						wchar_t* textBuffer = new wchar_t[1];

						textBuffer[0] = (tThisWorld.atClip[nCurrentEntity].nBulletsAvailables.size() - 1);

						pcUISystem->UpdateText(&tThisWorld, 1094, &atUIVertices, textBuffer, atUIVertices.at(tThisWorld.atLabel[1094].vIndex));

						pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, 1094);

						delete[] textBuffer;
					}
					if (tThisWorld.atClip[nCurrentEntity].nBulletsFired.size() != 0) {
						for (int i = 0; i < tThisWorld.atClip[nCurrentEntity].nBulletsFired.size(); ++i) {
							if (tThisWorld.atClip[nCurrentEntity].fAliveTime[i] < tThisWorld.atClip[nCurrentEntity].FValueOfCoolDown) {
								pcPhysicsSystem->AddBulletForce(&tThisWorld.atRigidBody[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]], tThisWorld.atClip[nCurrentEntity].bulletSpeed);
								tThisWorld.atClip[nCurrentEntity].fAliveTime[i] += 0.1;

							}
							else {
								tThisWorld.atClip[nCurrentEntity].fAliveTime.erase(tThisWorld.atClip[nCurrentEntity].fAliveTime.begin() + i);
								pcCollisionSystem->RemoveAABBCollider(tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]);
								pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]);
								tThisWorld.atClip[nCurrentEntity].nBulletsFired.erase(tThisWorld.atClip[nCurrentEntity].nBulletsFired.begin() + i);
								for (int i = 0; i < tThisWorld.atClip[nCurrentEntity].fAliveTime.size(); ++i) {
									if (tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]].indexInclip != 0) {
										tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]].indexInclip -= 1;
									}

								}
							}
						}

					}
					if (tThisWorld.atClip[nCurrentEntity].fShootingCoolDown > 0)
						tThisWorld.atClip[nCurrentEntity].fShootingCoolDown -= 1.1;
				}


			}

			if (nCurrentEntity == PlayerStartIndex) {
				float x = 0;
			}

			if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_RAYGUN)) {
				float CloseEstObject = 10000000000000000000.0f;
				float* distanceCalucaltion = new float();
				for (list<TAABB>::iterator ptr = pcCollisionSystem->m_AAbb.begin(); ptr != pcCollisionSystem->m_AAbb.end(); ++ptr) {

					if (ptr->m_IndexLocation != PlayerStartIndex)
						if (pcCollisionSystem->IsLineInBox(XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[0], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix)
							, XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[1], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix),
							tThisWorld.atWorldMatrix[ptr->m_IndexLocation].worldMatrix, *ptr, distanceCalucaltion) == true && *distanceCalucaltion < CloseEstObject) {
							CloseEstObject = *distanceCalucaltion;
							tThisWorld.atClip[GunIndexForPlayer].currentMaterial = 0;

							tThisWorld.atClip[GunIndexForPlayer].colorofBullets = tThisWorld.atSimpleMesh[ptr->m_IndexLocation].m_nColor;
						}
				}
				delete distanceCalucaltion;
			}


			if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
				COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC) || tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
					COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)) {
				tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
			}


			if ((tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_TRIGGER) | 
				tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)))
			{

				vector<int> otherCollisionsIndex;
				if (pcCollisionSystem->AABBtoAABBCollisionCheck(tThisWorld.atAABB[nCurrentEntity], &otherCollisionsIndex) == true)
				{
					for (int i = 0; i < otherCollisionsIndex.size(); ++i) {
						if (tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == true && tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask !=
							(COMPONENT_COLLISIONMASK |
								COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)) {
							if (nCurrentEntity == 8)
							{
								float x = 0;
							}

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
									COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))) {
							tTempVertexBuffer.m_d3dWorldMatrix = pcCollisionSystem->WalkingThrewObjectCheck(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[otherCollisionsIndex[i]], tThisWorld.atAABB[nCurrentEntity]);
							tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tTempVertexBuffer.m_d3dWorldMatrix;
							/*if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK)) {
								m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
							}*/

						}
						if (tThisWorld.atRigidBody[nCurrentEntity].ground == false && tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == false && tThisWorld.atRigidBody[nCurrentEntity].wall == false &&
							tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
								COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)
							&& (tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
								COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_STATIC)) | tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
									COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))
						{
							if (tThisWorld.atAIMask[otherCollisionsIndex[i]].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_FOLLOW)) {
								pcAiSystem->SetNumberOfAI(pcAiSystem->GetNumberOfAI() - 1);
								if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1) {
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
							if (tThisWorld.atInputMask[otherCollisionsIndex[i]].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK)) {
								if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
									if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1) {
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
					if (tThisWorld.atClayton[PlayerStartIndex].health <= 0)
					{
						GameOver = true;
						//return -1;
					}
					//		tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);


				}
				else {

					//tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;

				}
			}

			if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
				{

				if (tThisWorld.atBar[nCurrentEntity].entityToFollow != -1)
				{
					unsigned int targetEntity = tThisWorld.atBar[nCurrentEntity].entityToFollow;

					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tThisWorld.atWorldMatrix[targetEntity].worldMatrix;

					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1] += 2;

					if (tThisWorld.atBar[nCurrentEntity].backgroundColor.x == 0 &&
						tThisWorld.atBar[nCurrentEntity].backgroundColor.y == 0 &&
						tThisWorld.atBar[nCurrentEntity].backgroundColor.z == 0)
					{
						if (tCameraMode.bWalkMode == true)
							pcGraphicsSystem->InitQuadShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], walkCamera->d3d_Position, tThisWorld.atBar[nCurrentEntity].backgroundColor, 1.0f);
						else if (tCameraMode.bAimMode == true)
							pcGraphicsSystem->InitQuadShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], aimCamera->d3d_Position, tThisWorld.atBar[nCurrentEntity].backgroundColor, 1.0f);
						else if (tCameraMode.bDebugMode == true)
							pcGraphicsSystem->InitQuadShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], debugCamera->d3d_Position, tThisWorld.atBar[nCurrentEntity].backgroundColor, 1.0f);
					}
					else
					{
						if (tCameraMode.bWalkMode == true)
							pcGraphicsSystem->InitQuadShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], walkCamera->d3d_Position, tThisWorld.atBar[nCurrentEntity].backgroundColor, tThisWorld.atAiHeath[targetEntity].heath * .01);
						else if (tCameraMode.bAimMode == true)
							pcGraphicsSystem->InitQuadShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], aimCamera->d3d_Position, tThisWorld.atBar[nCurrentEntity].backgroundColor, tThisWorld.atAiHeath[targetEntity].heath * .01);
						else if (tCameraMode.bDebugMode == true)
							pcGraphicsSystem->InitQuadShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], debugCamera->d3d_Position, tThisWorld.atBar[nCurrentEntity].backgroundColor, tThisWorld.atAiHeath[targetEntity].heath * .01);
					}
				}
				else
					pcGraphicsSystem->InitPrimalShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, debugCamera->d3d_Position, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], debugCamera->d3d_Position);

					pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

				}
			if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
				{
					if (tCameraMode.bWalkMode == true)
					{
						tMyVertexBufferTemp.m_d3dViewMatrix = walkCamera->d3d_Position;
						tTempVertexBuffer.m_d3dViewMatrix = walkCamera->d3d_Position;
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
						tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;

					}
					else {
						tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
						tTempVertexBuffer.m_d3dViewMatrix = m_d3dViewMatrix;
						tMyVertexBufferTemp.m_d3dViewMatrix = m_d3dViewMatrix;
					}

					if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK))
					{
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
							m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
						}
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

			if (GamePaused)
			{
				if (!options)
				{
					if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON | COMPONENT_PAUSESCREEN))
					{
						tUIVertexBuffer.start = -1;
						tUIVertexBuffer.end = -1;
						tUIVertexBuffer.ratio = -1;

						tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 0);

						if (tThisWorld.atButton[nCurrentEntity].enabled && clickTime > .2)
						{
							if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
							{
								clickTime = 0;

								if (tThisWorld.atButton[nCurrentEntity].sceneIndex == 98)
									options = true;
								else if (tThisWorld.atButton[nCurrentEntity].sceneIndex == nCurrentScene)
									GamePaused = false;
								else if (tThisWorld.atButton[nCurrentEntity].sceneIndex == 96 || tThisWorld.atButton[nCurrentEntity].sceneIndex == 97)
								{

								}
								else
									return tThisWorld.atButton[nCurrentEntity].sceneIndex;

							}
							else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
								tUIPixelBuffer.hoverColor = XMFLOAT4(.6, .6, .6, 0);
							else
								tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 0);
						}

						pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
						pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
					}
				}
				else if (options)
				{
					if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON | COMPONENT_OPTIONS))
					{
						tUIVertexBuffer.start = -1;
						tUIVertexBuffer.end = -1;
						tUIVertexBuffer.ratio = -1;

						tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 0);

						if (tThisWorld.atButton[nCurrentEntity].enabled && clickTime > .2)
						{
							if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
							{
								clickTime = 0;

								if (tThisWorld.atButton[nCurrentEntity].sceneIndex == nCurrentScene)
									options = false;
							}
							else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
							{
								tUIPixelBuffer.hoverColor = XMFLOAT4(.6, .6, .6, 0);
							}
							else
							{
								tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 0);
							}
						}

						pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
						pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

					}

					if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BAR | COMPONENT_OPTIONS))
					{
						if (PtInRect(&tThisWorld.atBar[nCurrentEntity].barBoundingBox, clickPoint) && clickTime > .2)
						{
							// bar manipulation with mouse click try and use for enemy health bar - ZB                   
							tThisWorld.atBar[nCurrentEntity].ratio = (clickPoint.x - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left - 5.0) / (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left - 10);
							
							//clickTime = 0;
						}

						tUIVertexBuffer.start = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.left + 14 - (screenWidth / 2.0)) / (screenWidth / 2);
						tUIVertexBuffer.end = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right + 4 - (screenWidth / 2.0)) / (screenWidth / 2);
						tUIVertexBuffer.ratio = tThisWorld.atBar[nCurrentEntity].ratio;

						tUIPixelBuffer.hoverColor = XMFLOAT4(1, 0, 0, 1);

						if (tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse != nullptr)
							tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse = nullptr;

						pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
						pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
					}

					if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BAR | COMPONENT_BACKGROUND | COMPONENT_OPTIONS))
					{
						tUIVertexBuffer.start = -1;
						tUIVertexBuffer.end = -1;
						tUIVertexBuffer.ratio = -1;

						tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 1);

						if (tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse != nullptr)
							tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse = nullptr;

						pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
						pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
					}
				}
			}
			else
			{
				if (GameOver)
				{
					if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON | COMPONENT_DEATHSCREEN))
					{
						if (tThisWorld.atClayton[PlayerStartIndex].health <= 0)
						{
							tUIVertexBuffer.start = -1;
							tUIVertexBuffer.end = -1;
							tUIVertexBuffer.ratio = -1;

							tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 0);

							if (tThisWorld.atButton[nCurrentEntity].enabled && clickTime > .2)
							{
								if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
								{
									clickTime = 0;

									if (tThisWorld.atButton[nCurrentEntity].sceneIndex == 4)
										return tThisWorld.atButton[nCurrentEntity].sceneIndex;
									else if (tThisWorld.atButton[nCurrentEntity].sceneIndex == 2)
										return tThisWorld.atButton[nCurrentEntity].sceneIndex;

								}
								else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
								{
									tUIPixelBuffer.hoverColor = XMFLOAT4(.6, .6, .6, 0);
								}
								else
								{
									tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 0);
								}
							}

							pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
							pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
						}
					}

					if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_DEATHSCREEN))
					{
						if (tThisWorld.atClayton[PlayerStartIndex].health > 0)
						{
							if (tThisWorld.atLabel[nCurrentEntity].addTexture && tThisWorld.atLabel[nCurrentEntity].lastUIElement)
							{
								wchar_t textBuffer[] =
								{ L"SUCCESS" };

								CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2, 1, 0, 4.8, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nCurrentEntity, 0.1);

								pcUISystem->AddTextureToUI(&tThisWorld, nCurrentEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

								pcUISystem->AddMaskToUI(&tThisWorld, nCurrentEntity, COMPONENT_DEATHSCREEN);
								pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, nCurrentEntity);
							}
							else if (tThisWorld.atLabel[nCurrentEntity].addTexture)
							{
								wchar_t textBuffer[] =
								{ L"You Completed The Objective" };

								CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 6, 1, 0, 2.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nCurrentEntity, 0.1);

								pcUISystem->AddTextureToUI(&tThisWorld, nCurrentEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

								pcUISystem->AddMaskToUI(&tThisWorld, nCurrentEntity, COMPONENT_DEATHSCREEN);
								pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, nCurrentEntity);
							}
						}
						else
						{
							if (tThisWorld.atLabel[nCurrentEntity].addTexture && tThisWorld.atLabel[nCurrentEntity].lastUIElement)
							{
								wchar_t textBuffer[] =
								{ L"DEFEAT" };

								CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2, 1, 0, 4.8, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nCurrentEntity, 0.1);

								pcUISystem->AddTextureToUI(&tThisWorld, nCurrentEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

								pcUISystem->AddMaskToUI(&tThisWorld, nCurrentEntity, COMPONENT_DEATHSCREEN);
								pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, nCurrentEntity);
							}
							else if (tThisWorld.atLabel[nCurrentEntity].addTexture)
							{
								wchar_t textBuffer[] =
								{ L"You Failed To Complete The Objective" };

								CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 6, 1, 0, 2.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nCurrentEntity, 0.1);

								pcUISystem->AddTextureToUI(&tThisWorld, nCurrentEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

								pcUISystem->AddMaskToUI(&tThisWorld, nCurrentEntity, COMPONENT_DEATHSCREEN);
								pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, nCurrentEntity);
							}
						}
					}

					if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_DEATHSCREEN))
					{
						tUIVertexBuffer.start = -1;
						tUIVertexBuffer.end = -1;
						tUIVertexBuffer.ratio = -1;

						if (tThisWorld.atLabel[nCurrentEntity].addTexture && !tThisWorld.atLabel[nCurrentEntity].lastUIElement)
							tUIPixelBuffer.hoverColor = XMFLOAT4(1, 0, 0, 0);
						else
							tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 0);

						pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
						pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
					}
				}
			}

			if (GamePaused || GameOver || options)
			{
				if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_PAUSESCREEN | COMPONENT_DEATHSCREEN | COMPONENT_OPTIONS))
				{
					tUIVertexBuffer.start = -1;
					tUIVertexBuffer.end = -1;
					tUIVertexBuffer.ratio = -1;

					tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, .2);

					pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
					pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
				}
			}

			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_HUD))
			{
				tUIVertexBuffer.start = -1;
				tUIVertexBuffer.end = -1;
				tUIVertexBuffer.ratio = -1;

				if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
					tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 0);
				else
					tUIPixelBuffer.hoverColor = XMFLOAT4(.5, .5, .5, 1);

				//if (tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse != nullptr)
				//	tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse = nullptr;

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

			}

			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_HUD))
			{
				tUIVertexBuffer.start = -1;
				tUIVertexBuffer.end = -1;
				tUIVertexBuffer.ratio = -1;

				if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
					tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 0);
				else
					tUIPixelBuffer.hoverColor = XMFLOAT4(.5, .5, .5, 1);

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

			}

			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BAR | COMPONENT_HUD))
			{
				if (tThisWorld.atBar[nCurrentEntity].backgroundColor.x == 0 &&
					tThisWorld.atBar[nCurrentEntity].backgroundColor.y == 0 &&
					tThisWorld.atBar[nCurrentEntity].backgroundColor.z == 1 &&
					tThisWorld.atBar[nCurrentEntity].backgroundColor.w == 1)
				{
					tUIVertexBuffer.start = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.left + 14 - (screenWidth / 2.0)) / (screenWidth / 2);
					tUIVertexBuffer.end = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right + 4 - (screenWidth / 2.0)) / (screenWidth / 2);
					tUIVertexBuffer.ratio = (100 - tThisWorld.atClip[GunIndexForPlayer].fShootingCoolDown) * .01;
				}
				else if (tThisWorld.atBar[nCurrentEntity].backgroundColor.x == 1 &&
					tThisWorld.atBar[nCurrentEntity].backgroundColor.y == 0 &&
					tThisWorld.atBar[nCurrentEntity].backgroundColor.z == 0 &&
					tThisWorld.atBar[nCurrentEntity].backgroundColor.w == 1)
				{
					tUIVertexBuffer.start = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.left + 14 - (screenWidth / 2.0)) / (screenWidth / 2);
					tUIVertexBuffer.end = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right + 4 - (screenWidth / 2.0)) / (screenWidth / 2);
					tUIVertexBuffer.ratio = tThisWorld.atClayton[PlayerStartIndex].health * .01;
				}
				else
				{
					tUIVertexBuffer.start = -1;
					tUIVertexBuffer.end = -1;
					tUIVertexBuffer.ratio = -1;
				}

				tUIPixelBuffer.hoverColor = tThisWorld.atBar[nCurrentEntity].backgroundColor;

				if (tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse != nullptr)
					tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse = nullptr;

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}
		}

		pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);

		if (pcInputSystem->InputCheck(G_KEY_V))
		{
			GameOver = true;
		}

		if (pcInputSystem->InputCheck(G_KEY_B))
		{
			tThisWorld.atClayton[PlayerStartIndex].health = 0;
			GameOver = true;
		}

		if (GameOver && tThisWorld.atClayton[PlayerStartIndex].health > 0)
		{
			for (auto i = G_KEY_UNKNOWN; i <= G_KEY_9; ++i)
			{
				if (pcInputSystem->InputCheck(i) == 1)
				{
					if (i == G_KEY_V)
						continue;

					return 2;
				}
			}
		}

		if (pcInputSystem->InputCheck(G_KEY_X))
		{
			tThisWorld.atAiHeath[901].heath -= 1;
		}

		if (pcInputSystem->InputCheck(G_KEY_C))
		{
			tThisWorld.atAiHeath[905].heath -= 1;
		}

		if (pcInputSystem->InputCheck(G_KEY_F))
		{
			tThisWorld.atClayton[PlayerStartIndex].health -= 1;
		}

		clickTime += clickTimer.Delta();

		zValue += 0.001;
		return 5;
}

void CGameMangerSystem::RestartLevel()
{
}

int CGameMangerSystem::LoadMainMenu()
{
	if (pcInputSystem->InputCheck(G_KEY_H) == 1)
	{
	pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_FOOTSTEP,footSteps);
	}
	
	clickTimer.Signal();

	//////////
	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();
	//////////
	m_RealTimeFov = pcInputSystem->ZoomSight(m_RealTimeFov);
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective(m_RealTimeFov);

	CGraphicsSystem::TUIVertexBufferType tTempVertexBuffer;
	CGraphicsSystem::TUIPixelBufferType tTempPixelBuffer;

	POINT hoverPoint;
	GetCursorPos(&hoverPoint);
	ScreenToClient(cApplicationWindow, &hoverPoint);

	POINT clickPoint = { -1, -1 };
	if (pcInputSystem->InputCheck(G_BUTTON_LEFT) == 1)
	{
		GetCursorPos(&clickPoint);
		ScreenToClient(cApplicationWindow, &clickPoint);
	}

	pcGraphicsSystem->UpdateD3D();

	GetWindowRect(cApplicationWindow, &windowRect);

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if (!options)
		{
			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL))
			{
				tTempVertexBuffer.start = -1;
				tTempVertexBuffer.end = -1;
				tTempVertexBuffer.ratio = -1;
				tTempVertexBuffer.padding = -1;

				tTempPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 1);

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}

			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON))
			{
				if (tThisWorld.atButton[nCurrentEntity].enabled && clickTime > 0.2)
				{
					if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
					{
						clickTime = 0;

						if (tThisWorld.atButton[nCurrentEntity].sceneIndex < 96)
						{
							atUIVertices.clear();
							atUIIndices.clear();

							tTempPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 1);
							
							return tThisWorld.atButton[nCurrentEntity].sceneIndex;
						}
						else if (tThisWorld.atButton[nCurrentEntity].sceneIndex == 98)
							options = true;
						else if (tThisWorld.atButton[nCurrentEntity].sceneIndex == 96 || tThisWorld.atButton[nCurrentEntity].sceneIndex == 97)
						{

						}
					}
					else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
					{
						if (nCurrentEntity == 4 || nCurrentEntity == 5 || nCurrentEntity == 6 || nCurrentEntity == 7)
						{
							tTempPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 1);
						}
						else
						{
							tTempPixelBuffer.hoverColor = XMFLOAT4(.65, .65, .65, 1);
						}
					}
					else
					{
						tTempPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 1);
					}
				}
				else
				{
					tTempPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 1);
				}

				tTempVertexBuffer.start = -1;
				tTempVertexBuffer.end = -1;
				tTempVertexBuffer.ratio = -1;

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}
		}
		else
		{
			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON | COMPONENT_OPTIONS))
			{
				tTempVertexBuffer.start = -1;
				tTempVertexBuffer.end = -1;
				tTempVertexBuffer.ratio = -1;
				tTempVertexBuffer.padding = -1;

				tTempPixelBuffer.hoverColor = XMFLOAT4(1, 0, 0, 0);

				if (tThisWorld.atButton[nCurrentEntity].enabled && clickTime > 0.2)
				{
					if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
					{
						clickTime = 0;

						if (tThisWorld.atButton[nCurrentEntity].sceneIndex == 3)
						{
							options = false;
						}
					}
					else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
					{
						tTempPixelBuffer.hoverColor = XMFLOAT4(1, .6, .6, 0);
					}
					else
					{
						tTempPixelBuffer.hoverColor = XMFLOAT4(1, 0, 0, 0);
					}
				}

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}

			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_OPTIONS))
			{
				tTempVertexBuffer.start = -1;
				tTempVertexBuffer.end = -1;
				tTempVertexBuffer.ratio = -1;

				tTempPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 1);

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}

			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BAR | COMPONENT_OPTIONS))
			{
				if (PtInRect(&tThisWorld.atBar[nCurrentEntity].barBoundingBox, clickPoint))
					tThisWorld.atBar[nCurrentEntity].ratio = (clickPoint.x - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left - 5.0) / (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left - 10);

				if (tThisWorld.atBar[nCurrentEntity].backgroundColor.x == 1 &&
					tThisWorld.atBar[nCurrentEntity].backgroundColor.y == 0 &&
					tThisWorld.atBar[nCurrentEntity].backgroundColor.z == 0 &&
					tThisWorld.atBar[nCurrentEntity].backgroundColor.w == 1 )
				{
					tTempVertexBuffer.start = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.left + 14 - (screenWidth / 2.0)) / (screenWidth / 2);
					tTempVertexBuffer.end = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right + 4 - (screenWidth / 2.0)) / (screenWidth / 2);
					tTempVertexBuffer.ratio = tThisWorld.atBar[nCurrentEntity].ratio;
				}
				else 
				{
					tTempVertexBuffer.start = -1;
					tTempVertexBuffer.end = -1;
					tTempVertexBuffer.ratio = -1;
				}

				tTempPixelBuffer.hoverColor = tThisWorld.atBar[nCurrentEntity].backgroundColor;

				tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse = nullptr;

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}

			/*if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BAR | COMPONENT_OPTIONS | COMPONENT_BACKGROUND))
			{
				tTempVertexBuffer.start = -1;
				tTempVertexBuffer.end = -1;
				tTempVertexBuffer.ratio = -1;

				tTempPixelBuffer.hoverColor = tThisWorld.atBar[nCurrentEntity].backgroundColor;

				tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse = nullptr;

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}*/
		}
	}
	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);

	if (pcInputSystem->InputCheck(G_KEY_ESCAPE) == 1)
		return -1;

	clickTime += clickTimer.Delta();

#if MIKES_SANDBOX_ON
	return 9;
#endif
	return 3;
}

void CGameMangerSystem::InitializeMainMenu()
{
	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);
	atUIVertices.clear();
	atUIIndices.clear();

	options = false;

	int nThisEntity;
	{
		wchar_t wideChar[] =
		{ L"UI_Textures.fbm/Auger_MainMenu.png" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 20, 20, 0, 0, &atUIVertices, -1, .2);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, wideChar);
	}
	{
		wchar_t wideChar[] =
		{ L"UI_Textures.fbm/play.png" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, 1, .5, 0, &atUIVertices, -1, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, wideChar);
		//                                                  modify this to switch testing levels in Augur.cpp
	
	//	pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 1, true);

#if MIKES_SANDBOX_ON
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 9, true);

#endif
#if SKELETON_LOAD_ON
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 7, true);

#endif
#if MAIN_LEVEL_ON
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 13, true);

#endif
	}
	{
		wchar_t wideChar[] =
		{ L"UI_Textures.fbm/options.png" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, 1, .5, -2.4, &atUIVertices, -1, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, wideChar);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 98, true);
	}
	{
		wchar_t wideChar[] =
		{ L"UI_Textures.fbm/credits.png" }; 
	
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, 1, .5, -3.6, &atUIVertices, -1, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, wideChar);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 99, true);
	}

	InitializeOptionsMenu();

	pcGraphicsSystem->CreateBuffers(&tThisWorld);
}

int CGameMangerSystem::LoadTitleScreen()
{
	pcAudioSystem->SetListener(Listener, 1, ErrorResult);
	//////////
	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();
	//////////

	CGraphicsSystem::TUIVertexBufferType tTempVertexBuffer;
	CGraphicsSystem::TUIPixelBufferType tTempPixelBuffer;

	POINT hoverPoint;
	GetCursorPos(&hoverPoint);
	ScreenToClient(cApplicationWindow, &hoverPoint);

	//POINT clickPoint = { -1, -1 };
	//if (pcInputSystem->InputCheck(G_BUTTON_LEFT) == 1)
	//{
	//	GetCursorPos(&clickPoint);
	//	ScreenToClient(cApplicationWindow, &clickPoint);
	//}

	pcGraphicsSystem->UpdateD3D();

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL))
		{
			//if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
			//{
			//	return tThisWorld.atButton[nCurrentEntity].sceneIndex;
			//}
			//else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
			//{
			//	tTempPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 1);
			//}
			//else
			//{
			//	tTempPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 1);
			//}

			tTempVertexBuffer.start = -1;
			tTempVertexBuffer.end = -1;
			tTempVertexBuffer.ratio = -1;

			tTempPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 1);

			pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}
	}

	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);

	for (auto i = G_KEY_UNKNOWN; i <= G_KEY_9; ++i)
	{
		if (pcInputSystem->InputCheck(i) == 1)
		{
			if (i == G_KEY_ESCAPE)
				return -1;
	
			return 2;
		}
	}
	
	if (pcInputSystem->InputCheck(G_BUTTON_LEFT))
		return 2;

	return 1;
}

void CGameMangerSystem::InitializeTitleScreen()
{
	pcAudioSystem->IntiializeSystem(ErrorResult);
	pcAudioSystem->SetBanksFolderPath(AKTEXT("../Villain CES/GeneratedSoundBanks/Windows"));
	pcAudioSystem->RegisterGameObj(Listener);
	pcAudioSystem->RegisterGameObj(footSteps);
	pcAudioSystem->LoadBankFile(INIT_BNK, init_bnkID,ErrorResult);
	pcAudioSystem->LoadBankFile(FOOTSTEP_BNK, footsteps_bnkID, ErrorResult);
	
	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);
	atUIVertices.clear();
	atUIIndices.clear();
	
	menuCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	
	wchar_t wideChar[] =
	{ L"UI_Textures.fbm/Auger_TitleScreen.png" };
	
	unsigned int nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 20, 20, 0, 0, &atUIVertices);
	m_RealTimeFov = 90.0f;

	pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, wideChar);
	//pcUISystem->AddButtonToUI(&tThisWorld, nThisEntity, -3, cApplicationWindow);

	pcGraphicsSystem->CreateBuffers(&tThisWorld);
}

void CGameMangerSystem::InitializePauseScreen()
{
	unsigned int nThisEntity;
	
	{
		wchar_t filePath[] =
		{ L"../UI_Textures.fbm/transparentSquare.png" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 20, 20, 0, 0, &atUIVertices, nThisEntity);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, filePath);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_PAUSESCREEN);
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_DEATHSCREEN);
	}

	{
		wchar_t textBuffer[] =
		{ L"CONTINUE" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2, 1, 0, 4.8, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, nCurrentScene + 1, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_PAUSESCREEN);
	}
	
	{
		wchar_t textBuffer[] =
		{ L"SAVE" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 1, 1, 0, 3.6, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 96, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_PAUSESCREEN);
	}

	{
		wchar_t textBuffer[] =
		{ L"LOAD" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 1, 1, 0, 2.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 97, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_PAUSESCREEN);
	}

	{
		wchar_t textBuffer[] =
		{ L"OPTIONS" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2, 1, 0, 1.2, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 98, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_PAUSESCREEN);
	}

	{
		wchar_t textBuffer[] =
		{ L"EXIT" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 1, 1, 0, 0, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 2, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_PAUSESCREEN);
	}

	{
		wchar_t textBuffer[] =
		{ L"SUBTITLES:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.05, .8, -1.545, 6.1, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"MASTER VOLUME:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 3, .8, -2.001, 4.9, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 4.8, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 4.8, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"DIALOGUE VOLUME:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 3.4, .8, -2.22, 3.7, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 3.6, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 3.6, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"MUSIC VOLUME:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.8, .8, -1.915, 2.5, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 2.4, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 2.4, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"FX VOLUME:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.595, 1.3, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 1.2, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 1.2, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"DIFFICULTY:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.60, 0.1, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"BRIGHTNESS:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.59, -1.1, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, -1.2, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, -1.2, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"BACK" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 1, 1, 0, -2.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, nCurrentScene + 1, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	pcGraphicsSystem->CreateBuffers(&tThisWorld);
}

void CGameMangerSystem::InitializeEndScreen()
{
	unsigned int nThisEntity;

	{
		nThisEntity = createEntityReverse(&tThisWorld);

		tThisWorld.atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK;
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_DEATHSCREEN);

		tThisWorld.atLabel[nThisEntity].addTexture = true;
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);

		tThisWorld.atGraphicsMask[nThisEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK;
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_DEATHSCREEN);

		tThisWorld.atLabel[nThisEntity].addTexture = true;
		tThisWorld.atLabel[nThisEntity].lastUIElement = true;
	}

	{
		wchar_t textBuffer[] =
		{ L"CONTINUE" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 1.4, 1, -1, -1.2, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);

		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 13, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_DEATHSCREEN);
	}

	{
		wchar_t textBuffer[] =
		{ L"EXIT" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, .8, 1, 1, -1.2, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);

		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 2, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_DEATHSCREEN);
	}

	pcGraphicsSystem->CreateBuffers(&tThisWorld);
}

void CGameMangerSystem::InitializeOptionsMenu()
{
	unsigned int nThisEntity;

	//if (pcUISystem->myFont == nullptr)
	//{
	//	pcUISystem->myFont = CreateFontA(24, 12, 0, 0, FW_MEDIUM, false, false, false, 0, 0, 0, 0, 0, 0);
	//}

	if (fontTexture == nullptr)
	{
		ID3D11Resource* resource;

		wchar_t filepath[] =
		{ L"UI_Textures.fbm/FontB.png" };

		CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, filepath, &resource, &fontTexture, NULL);
	}

	{
		wchar_t wideChar[] =
		{ L"UI_Textures.fbm/Auger_Space.png" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 20, 20, 0, 0, &atUIVertices, -1, 0.2);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, wideChar);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"SUBTITLES:" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.05, .8, -1.545, 6.1, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"MASTER VOLUME:" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 3, .8, -2.001, 4.9, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);
		
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 4.8, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1));
	
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 4.8, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 1, 1, 1));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
		//pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_BACKGROUND);
	}

	{
		wchar_t textBuffer[] =
		{ L"DIALOGUE VOLUME:" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 3.4, .8, -2.22, 3.7, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{	
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 3.6, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1));
	
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 3.6, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 1, 1, 1));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
		//pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_BACKGROUND);
	}

	{
		wchar_t textBuffer[] =
		{ L"MUSIC VOLUME:" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.8, .8, -1.915, 2.5, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{	
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 2.4, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1));
	
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 2.4, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 1, 1, 1));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
		//pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_BACKGROUND);
	}

	{
		wchar_t textBuffer[] =
		{ L"FX VOLUME:" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.595, 1.3, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{	
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 1.2, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1));
	
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 1.2, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 1, 1, 1));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
		//pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_BACKGROUND);
	}

	{
		wchar_t textBuffer[] =
		{ L"DIFFICULTY:" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.60, 0.1, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"BRIGHTNESS:" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.59, -1.1, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{	
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, -1.2, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1));
	
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, -1.2, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 1, 1, 1));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
		//pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_BACKGROUND);
	}

	{
		wchar_t textBuffer[] =
		{ L"BACK" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 1, 1, 0, -2.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, nCurrentScene + 1, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}
}

void CGameMangerSystem::InitializeHUD()
{
	atUIVertices.clear();
	atUIIndices.clear();

	unsigned int nThisEntity;

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, .35, .15, .4, -.4, &atUIVertices, nThisEntity, .1);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, .35, .15, -.24, -.4, &atUIVertices, nThisEntity, .1);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, .1, .45, .08, 0.05, &atUIVertices, nThisEntity, .1);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, .1, .45, .08, -.85, &atUIVertices, nThisEntity, .1);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, 2, 6, -8, &atUIVertices, nThisEntity, .1);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
	}

	{
		wchar_t textBuffer[] =
		{ L"3" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, .75, 1, 6.625, -7.5, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 3, 1, -7, -8, &atUIVertices, nThisEntity, .1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 3, 1, -7, -8, &atUIVertices, nThisEntity, .15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 3, 1, -7, -9.1, &atUIVertices, nThisEntity, .1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 1, 1));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 3, 1, -7, -9.1, &atUIVertices, nThisEntity, .15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
	}
}

void CGameMangerSystem::LoadPathFindingTest()
{
	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);

	pcAiSystem->SetNumberOfAI(2);
	//tTimerInfo->StartClock(tAugerTimers->tSceneTimer);

	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();
	debugCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	walkCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	aimCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();

	tCameraMode.bDebugMode = false;
	tCameraMode.bAimMode = true;
	tCameraMode.bWalkMode = false;
	tCameraMode.bSwitch = false;
	bMoving = false;
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
	wall.r[3].m128_f32[0] -= 10;
	wall.r[3].m128_f32[1] -= 1;
	wall.r[3].m128_f32[2] -= 1;

	CreateDoorWay(&tThisWorld, wall);
	wall.r[3].m128_f32[0] -= 22;
	CreateDoorWay(&tThisWorld, wall);

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


	
	AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[0] += -1;
	AILocation.r[3].m128_f32[2] += -2;



	AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[0] += -3;
	AILocation.r[3].m128_f32[2] += -5;
	
	//CreateSimpleGunAi(&tThisWorld, AILocation);
	XMMATRIX groundSpawnPoint;
	groundSpawnPoint = m_d3dWorldMatrix;
	groundSpawnPoint.r[3].m128_f32[1] -= 2;
	CreateGround(&tThisWorld, groundSpawnPoint);
	groundSpawnPoint.r[3].m128_f32[2] -= -20;
	CreateGround(&tThisWorld, groundSpawnPoint);

	tempImport = pcGraphicsSystem->ReadMesh("meshData_Example_Objects.txt");

	for (int meshIndex = 1; meshIndex < tempImport.meshCount; meshIndex++)
	{
		//int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	}

	tempImport = pcGraphicsSystem->ReadMesh("meshData_ScifiRoom.txt");

	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		//int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	}

#pragma region 
	AILocation.r[3].m128_f32[0] -= 7;
	int nodeLocation = CreateNodePoint(&tThisWorld, AILocation);
//	tThisWorld.atSimpleMesh[nodeLocation].m_nColor= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT3 nodePosition;
	nodePosition.x = AILocation.r[3].m128_f32[0];
	nodePosition.y = AILocation.r[3].m128_f32[1];
	nodePosition.z = AILocation.r[3].m128_f32[2];

	pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	XMMATRIX part2ofNodeMovement = AILocation;
	part2ofNodeMovement.r[3].m128_f32[2] -= -10;
	int nodelocation5 = CreateNodePoint(&tThisWorld, part2ofNodeMovement);
	nodePosition.x = part2ofNodeMovement.r[3].m128_f32[0];
	nodePosition.y = part2ofNodeMovement.r[3].m128_f32[1];
	nodePosition.z = part2ofNodeMovement.r[3].m128_f32[2];

	pcAiSystem->AddNodeToPathFinding(nodelocation5, nodePosition, 1);
	part2ofNodeMovement.r[3].m128_f32[0] -= 6;

	int nodelocation4 = CreateNodePoint(&tThisWorld, part2ofNodeMovement);
	nodePosition.x = part2ofNodeMovement.r[3].m128_f32[0];
	nodePosition.y = part2ofNodeMovement.r[3].m128_f32[1];
	nodePosition.z = part2ofNodeMovement.r[3].m128_f32[2];

	pcAiSystem->AddNodeToPathFinding(nodelocation4, nodePosition, 1);
	AILocation.r[3].m128_f32[0] += 4;
	AILocation.r[3].m128_f32[2] += -4;

	int nodeLocation2 = CreateNodePoint(&tThisWorld, AILocation);

	nodePosition.x = AILocation.r[3].m128_f32[0];
	nodePosition.y = AILocation.r[3].m128_f32[1];
	nodePosition.z = AILocation.r[3].m128_f32[2];

	pcAiSystem->AddNodeToPathFinding(nodeLocation2, nodePosition, 1);
	XMMATRIX pathfindAI = AILocation;
	pathfindAI.r[3].m128_f32[0] += 3;
	nodePosition.x = pathfindAI.r[3].m128_f32[0];
	nodePosition.y = pathfindAI.r[3].m128_f32[1];
	nodePosition.z = pathfindAI.r[3].m128_f32[2];
	int aiPathIndex = CreateTestAIPathFinding(&tThisWorld, pathfindAI);
//	tThisWorld.atSimpleMesh[aiPathIndex].m_nColor= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	int nodelocation3 = CreateNodePoint(&tThisWorld, pathfindAI);
	tThisWorld.atPathPlanining[aiPathIndex].startingNode = nodelocation3;
	tThisWorld.atPathPlanining[aiPathIndex].Goal = nodelocation4;
	pcAiSystem->AddNodeToPathFinding(nodelocation3, nodePosition, 1);
	vector<int> edges;
//	edges.push_back(nodelocation4);
	edges.push_back(nodeLocation2);
	edges.push_back(nodelocation3);
	edges.push_back(nodelocation5);

	pcAiSystem->AddEdgestoNode(nodeLocation, edges);
	edges.clear();
	edges.push_back(nodeLocation);
	edges.push_back(nodelocation3);
	pcAiSystem->AddEdgestoNode(nodeLocation2, edges);
	edges.clear();
	//	edges.push_back(nodeLocation);
	edges.push_back(nodeLocation2);
	pcAiSystem->AddEdgestoNode(nodelocation3, edges);
	edges.clear();
	edges.push_back(nodelocation4);
	pcAiSystem->AddEdgestoNode(nodelocation5, edges);
#pragma endregion Nodes & Edges


	if (pcCollisionSystem->m_AAbb.size() != 0) {
		float x = 0;
	}
	XMFLOAT4 blue;
	blue.y = 0;
	blue.z = 1;
	blue.w = 1;
	blue.x = 0;
	tThisWorld.atSimpleMesh[nodelocation4].m_nColor = blue;
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
				if (nCurrentEntity == PlayerStartIndex) {
					pcCollisionSystem->AddAiVisioNCheck(MyAbb, nCurrentEntity);

				}

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

int CGameMangerSystem::PathFindingExample()
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(-1); //Important!
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective(m_RealTimeFov);

//	tTimerInfo->applicationTime = tTimerInfo->GetTime(tAugerTimers->tAppTimer, tTimerInfo->applicationTime);
	//tTimerInfo->sceneTime = tTimerInfo->GetTime(tAugerTimers->tSceneTimer, tTimerInfo->sceneTime);

	tCameraMode = pcInputSystem->CameraModeListen(tCameraMode);

	static XMMATRIX m_d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	static XMMATRIX m_d3dOffsetMatrix = pcGraphicsSystem->SetDefaultOffset();

	if (pcInputSystem->InputCheck(G_KEY_P))
	{
		return 3;
	}
	// ui stuff
	if (GamePaused == true) {
		if (DrawUI == true) {
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
			UiPos.r[3].m128_f32[0] += 0.8;
			UiPos.r[3].m128_f32[1] -= 0.3;

			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] -= 0.7;

			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] -= 0.7;

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
		m_d3d_ResultMatrix = pcInputSystem->WalkCameraControls(XMVectorSet(0, 1.0f, 0, 0), m_d3d_ResultMatrix, bMoving);

		walkCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);
		walkCamera->d3d_Position = XMMatrixMultiply(m_d3dOffsetMatrix, walkCamera->d3d_Position);






	}
	else if (tCameraMode.bAimMode == true)
	{
		if (tCameraMode.bSwitch == true)
		{
			m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
			//CameraNewPosition = pcInputSystem->CameraBehaviorLerp(m_d3d_ResultMatrix, m_d3dPlayerMatrix);
			m_d3dPlayerMatrix = pcInputSystem->AimMode(m_d3dPlayerMatrix);

			CameraNewPosition = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);

			CameraNewPosition = XMMatrixMultiply(m_d3dOffsetMatrix, CameraNewPosition);
			aimCamera->d3d_Position = pcInputSystem->CameraBehaviorLerp(walkCamera->d3d_Position, CameraNewPosition, scale);
			scale += 0.001;
			if (scale > 1) {
				tCameraMode.bSwitch = false;
				scale = 0;

			}

		}
		else {
			m_RealTimeFov = pcInputSystem->ZoomSight(m_RealTimeFov);




			m_d3dPlayerMatrix = pcInputSystem->AimMode(m_d3dPlayerMatrix);

			aimCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);

			aimCamera->d3d_Position = XMMatrixMultiply(m_d3dOffsetMatrix, aimCamera->d3d_Position);
		}

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
		tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		{

			pcGraphicsSystem->InitPrimalShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, debugCamera->d3d_Position, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], debugCamera->d3d_Position);

			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

		}
		// ai code would run here
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			if (tCameraMode.bWalkMode == true)
			{
				tMyVertexBufferTemp.m_d3dViewMatrix = walkCamera->d3d_Position;
				tTempVertexBuffer.m_d3dViewMatrix = walkCamera->d3d_Position;
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
				tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;

			}
			else {
				tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
				tTempVertexBuffer.m_d3dViewMatrix = m_d3dViewMatrix;
				tMyVertexBufferTemp.m_d3dViewMatrix = m_d3dViewMatrix;
			}

			if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK))
			{
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
					m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
				}
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

		if (pcAiSystem->GetNumberOfAI() <= 0)
		{
			return -1;
		}
		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_FOLLOW) || tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_SHOOT | COMPONENT_AIMASK | COMPONENT_FOLLOW) || (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST) == tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask)
		{
#if AI_ON				
			if (tThisWorld.atActiveAI[nCurrentEntity].active == true) {
				pcAiSystem->FollowObject(tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
				pcAiSystem->ShootGun(&tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex]);
			}
#endif // AI_ON
		}
		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_HEALINGAI)) {

		}

		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SEARCH) || tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SPOTEDPLAYER)) {



			if (tThisWorld.atAIVision[nCurrentEntity].keepSearching == true) {
				if (tThisWorld.atAIVision[nCurrentEntity].visionRotation < 7 && tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true) {
					tThisWorld.atAIVision[nCurrentEntity].visionRotation += 0.001;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcAiSystem->LookBackLeftToRight(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix,
						true);
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true) {
					tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight = false;
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].visionRotation > -7 && tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == false) {
					tThisWorld.atAIVision[nCurrentEntity].visionRotation -= 0.001;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcAiSystem->LookBackLeftToRight(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix,
						false);
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == false) {
					tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight = true;
				}
			}
			else if (tThisWorld.atAIVision[nCurrentEntity].keepSearching == false) {
				pcAiSystem->LookAtObject(tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
			}

			float4x4 secondcamera;
			secondcamera.row1.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[0];
			secondcamera.row1.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[1];
			secondcamera.row1.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[2];
			secondcamera.row1.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[3];

			secondcamera.row2.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[0];
			secondcamera.row2.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[1];
			secondcamera.row2.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[2];
			secondcamera.row2.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[3];

			secondcamera.row3.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[0];
			secondcamera.row3.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[1];
			secondcamera.row3.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[2];
			secondcamera.row3.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[3];

			secondcamera.row4.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[0];
			secondcamera.row4.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1];
			secondcamera.row4.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[2];
			secondcamera.row4.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[3];
			pcAiSystem->UpdateFrustum(tThisWorld.atAIVision[nCurrentEntity].eyes0, secondcamera, 70, 1, 0.1, 20);
			//	 pcAiSystem->calculate_frustum(&tThisWorld,tThisWorld.atAIVision[nCurrentEntity].eyes0, secondcamera,70,1,0.1,20, nCurrentEntity, -2.1, 1.4, 19.6);
			vector<int> indicies;
			if (pcCollisionSystem->AiVisionCheck(tThisWorld.atAIVision[nCurrentEntity].eyes0, &indicies) == true) {
				float x = 0;
				bool danger = false;
				for (int i = 0; i < indicies.size(); ++i) {
					if (PlayerStartIndex == indicies[i]) {
						tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
						tThisWorld.atAIVision[nCurrentEntity].keepSearching = false;
						danger = true;
						if (tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].nBulletsAvailables.size() <= 0) {
							tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].tryToReload = true;

						}
						tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].tryToShoot = true;

						pcAiSystem->AddAiInCombat(nCurrentEntity);
						//		tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask = COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST;
						/*	if (tThisWorld.atPathPlanining[nCurrentEntity].foundDestination == true) {
						int previousgoal = tThisWorld.atPathPlanining[nCurrentEntity].Goal;
						int previousStartPosition = tThisWorld.atPathPlanining[nCurrentEntity].startingNode;
						tThisWorld.atPathPlanining[nCurrentEntity].Goal = previousStartPosition;
						tThisWorld.atPathPlanining[nCurrentEntity].startingNode = previousgoal;
						tThisWorld.atPathPlanining[nCurrentEntity].foundDestination = false;
						tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding = true;

						}*/

					}
					else if (tThisWorld.atProjectiles[indicies[i]].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
						danger = true;
						tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

					}
					else if (danger == false) {
						tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
					}
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

		}
		//else if(tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask==())
		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_PATHFINDTEST) || (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST) == tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask) {
			 
				if (tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding == true) {

					pcAiSystem->FindBestPath(tThisWorld.atPathPlanining[nCurrentEntity].startingNode,
						tThisWorld.atPathPlanining[nCurrentEntity].Goal,
						&tThisWorld.atPathPlanining[nCurrentEntity].directions);

					tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding = false;
				}
				else {
					if (tThisWorld.atPathPlanining[nCurrentEntity].DelayMovement <= 0) {
						pcAiSystem->PathPlaningMovement(&tThisWorld.atPathPlanining[nCurrentEntity], &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
					}
					else {
						tThisWorld.atPathPlanining[nCurrentEntity].DelayMovement -= 0.001;
					}
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
					rayindex = CreateRayBullet(&tThisWorld, gun, 10, PlayerStartIndex, -0.6, -0.1, 10.7);
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
				else if (tThisWorld.atClip[nCurrentEntity].tryToShoot == true) {
					tThisWorld.atClip[nCurrentEntity].tryToShoot = false;
				}
				if (tThisWorld.atClip[nCurrentEntity].tryToReload == true) {
					pcProjectileSystem->Reload(&tThisWorld.atClip[nCurrentEntity]);
					tThisWorld.atClip[nCurrentEntity].tryToReload = false;

				}
				if (tThisWorld.atClip[nCurrentEntity].nBulletsFired.size() != 0) {
					for (int i = 0; i < tThisWorld.atClip[nCurrentEntity].nBulletsFired.size(); ++i) {
						if (tThisWorld.atClip[nCurrentEntity].fAliveTime[i] < tThisWorld.atClip[nCurrentEntity].FValueOfCoolDown) {
							pcPhysicsSystem->AddBulletForce(&tThisWorld.atRigidBody[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]], tThisWorld.atClip[nCurrentEntity].bulletSpeed);
							tThisWorld.atClip[nCurrentEntity].fAliveTime[i] += 0.1;

						}
						else {
							tThisWorld.atClip[nCurrentEntity].fAliveTime.erase(tThisWorld.atClip[nCurrentEntity].fAliveTime.begin() + i);
							pcCollisionSystem->RemoveAABBCollider(tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]);
							pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]);
							tThisWorld.atClip[nCurrentEntity].nBulletsFired.erase(tThisWorld.atClip[nCurrentEntity].nBulletsFired.begin() + i);
							for (int i = 0; i < tThisWorld.atClip[nCurrentEntity].fAliveTime.size(); ++i) {
								if (tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]].indexInclip != 0) {
									tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]].indexInclip -= 1;
								}

							}
						}
					}

				}
				if (tThisWorld.atClip[nCurrentEntity].fShootingCoolDown > 0)
					tThisWorld.atClip[nCurrentEntity].fShootingCoolDown -= 0.1;
			}


		}


		if (nCurrentEntity == PlayerStartIndex) {
			float x = 0;
		}
		tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, true);

		if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_RAYGUN)) {
			float CloseEstObject = 10000000000000000000.0f;
			float* distanceCalucaltion = new float();
			for (list<TAABB>::iterator ptr = pcCollisionSystem->m_AAbb.begin(); ptr != pcCollisionSystem->m_AAbb.end(); ++ptr) {

				if (ptr->m_IndexLocation != PlayerStartIndex)
					if (pcCollisionSystem->IsLineInBox(XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[0], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix)
						, XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[1], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix),
						tThisWorld.atWorldMatrix[ptr->m_IndexLocation].worldMatrix, *ptr, distanceCalucaltion) == true && *distanceCalucaltion < CloseEstObject) {
						CloseEstObject = *distanceCalucaltion;
						tThisWorld.atClip[GunIndexForPlayer].currentMaterial = 0;

						tThisWorld.atClip[GunIndexForPlayer].colorofBullets = tThisWorld.atSimpleMesh[ptr->m_IndexLocation].m_nColor;
					}
			}
			delete distanceCalucaltion;
		}


		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
			COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC) || tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
				COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)) {
			tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
		}


		if ((tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_TRIGGER) | tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)))
		{

			vector<int> otherCollisionsIndex;
			if (pcCollisionSystem->AABBtoAABBCollisionCheck(tThisWorld.atAABB[nCurrentEntity], &otherCollisionsIndex) == true)
			{
				for (int i = 0; i < otherCollisionsIndex.size(); ++i) {
					if (tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == true && tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask !=
						(COMPONENT_COLLISIONMASK |
							COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)) {
						if (nCurrentEntity == 8)
						{
							float x = 0;
						}

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
								COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))) {
						tTempVertexBuffer.m_d3dWorldMatrix = pcCollisionSystem->WalkingThrewObjectCheck(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[otherCollisionsIndex[i]], tThisWorld.atAABB[nCurrentEntity]);
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tTempVertexBuffer.m_d3dWorldMatrix;
						if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK)) {
							m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
						}

					}
					if (tThisWorld.atRigidBody[nCurrentEntity].ground == false && tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == false && tThisWorld.atRigidBody[nCurrentEntity].wall == false &&
						tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)
						&& (tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_STATIC)) | tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
								COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))
					{

						if (tThisWorld.atAIMask[otherCollisionsIndex[i]].m_tnAIMask >1) {
							if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
								if (tThisWorld.atAiHeath[otherCollisionsIndex[i]].heath <= 0) {
									pcAiSystem->SetNumberOfAI(pcAiSystem->GetNumberOfAI() - 1);
									if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1) {
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
								else {
									tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.erase
									(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.begin()
										+ tThisWorld.atClip[nCurrentEntity].indexInclip);

									pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
									tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.erase(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.begin()
										+ tThisWorld.atClip[nCurrentEntity].indexInclip);


									pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);
									tThisWorld.atAiHeath[otherCollisionsIndex[i]].heath -= 50;
									if (tThisWorld.atAiHeath[otherCollisionsIndex[i]].heath <= 0) {
										pcAiSystem->SetNumberOfAI(pcAiSystem->GetNumberOfAI() - 1);
										pcCollisionSystem->RemoveAABBCollider(otherCollisionsIndex[i]);
										pcGraphicsSystem->CleanD3DObject(&tThisWorld, otherCollisionsIndex[i]);
										pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atAIMask[otherCollisionsIndex[i]].GunIndex);
									}
								}
							}
						}
						else if (tThisWorld.atInputMask[otherCollisionsIndex[i]].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK)) {
							if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
								if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1) {
									tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.erase
									(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.begin()
										+ tThisWorld.atClip[nCurrentEntity].indexInclip);

									pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
									tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.erase(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.begin()
										+ tThisWorld.atClip[nCurrentEntity].indexInclip);

									//pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atAIMask[nCurrentEntity].GunIndex);
									pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);

								}
								tThisWorld.atClayton[otherCollisionsIndex[i]].health -= 50;
							}


						}
						if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
							if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1) {
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

					}
					if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK) && tThisWorld.atAIMask[otherCollisionsIndex[i]].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_COVERTRIGGER)) {

						pcAiSystem->MoveAiToCoverLocation(tThisWorld.atCoverTrigger[otherCollisionsIndex[i]], &tThisWorld);
					}
				}
				if (tThisWorld.atClayton[PlayerStartIndex].health <= 0)
				{
					return -1;
				}
				//		tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);


			}
			else {

				//tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;

			}
		}
		else {

			//	tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;

		}
		tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;

		if (tThisWorld.atParentWorldMatrix[nCurrentEntity] != -1)
		{

			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(pcGraphicsSystem->SetDefaultWorldPosition(),
				tThisWorld.atWorldMatrix[tThisWorld.atParentWorldMatrix[nCurrentEntity]].worldMatrix);

			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(tThisWorld.atOffSetMatrix[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);

			if (nCurrentEntity == frustumIndex) {
				XMMATRIX empty;
				empty = pcGraphicsSystem->SetDefaultWorldPosition();
				XMMATRIX rotation;
				XMMATRIX d3dTmpViewM;
				rotation = XMMatrixRotationY(-0.3);
				empty.r[1] = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1];
				empty.r[0] = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0];
				d3dTmpViewM = empty;
				XMVECTOR d3d_newX, d3d_newY, d3d_existingZ;
				d3dTmpViewM = XMMatrixMultiply(rotation, d3dTmpViewM);
				d3d_existingZ = d3dTmpViewM.r[2];
				d3d_newX = XMVector3Cross(empty.r[1], d3d_existingZ);
				d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

				d3d_newX = XMVector3Normalize(d3d_newX);
				d3d_newY = XMVector3Normalize(d3d_newY);

				d3d_existingZ = XMVector3Normalize(d3d_existingZ);

				d3dTmpViewM.r[0] = d3d_newX;
				d3dTmpViewM.r[1] = d3d_newY;
				d3dTmpViewM.r[2] = d3d_existingZ;
				empty = d3dTmpViewM;
				empty.r[3].m128_f32[0] = 3;
				empty.r[3].m128_f32[1] = 0;
				empty.r[3].m128_f32[2] = 1;


				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = empty;
			}
		}

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
		if (tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix.r[3].m128_f32[1] < -10) {
			return 7;
		}

	}

	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);
	zValue += 0.001;
	return 6;
}

void CGameMangerSystem::FirstSkeltonAiTestLoad()
{
	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);

	pcAiSystem->SetNumberOfAI(1);
	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
	walkCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	aimCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	debugCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();

	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();

	tCameraMode.bDebugMode = false;
	tCameraMode.bAimMode = true;
	tCameraMode.bWalkMode = false;
	tCameraMode.bSwitch = false;
	bMoving = false;
	m_d3dPlayerMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	//m_d3dPlayerMatrix = XMMatrixMultiply(m_d3dPlayerMatrix, XMMatrixScaling(.01, .01, .01));
	m_d3dPlayerMatrix = m_d3dWorldMatrix;
	m_d3dPlayerMatrix.r[3].m128_f32[2] -= 10;
	ImporterData tempImport;
	tempImport = pcGraphicsSystem->ReadMesh("meshData_Pirate.txt");

	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		PlayerStartIndex = createClayton(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);

	}
	XMMATRIX AISowrdGuy = m_d3dPlayerMatrix;
	int swordGuy = CreateFriendlySwordsMan(&tThisWorld, AISowrdGuy, PlayerStartIndex);
	tThisWorld.atSimpleMesh[swordGuy].m_nColor = XMFLOAT4(0, 0, 0, 1);

	//AimingLine(&tThisWorld, m_d3dWorldMatrix, PlayerStartIndex, -1, 1, 10.5);

	GunIndexForPlayer = CreateGun(&tThisWorld, m_d3dWorldMatrix, PlayerStartIndex, -1, 1, 10.5, 3, 100);
	tThisWorld.atClip[GunIndexForPlayer].bulletSpeed = 0.001;
	//tThisWorld.atClip[GunIndexForPlayer].

	tThisWorld.atClayton[PlayerStartIndex].health = 100000;
	XMMATRIX wall = m_d3dWorldMatrix;
	wall.r[3].m128_f32[1] += -1;

	int wallidex=CreateWall(&tThisWorld, wall);
	wall.r[3].m128_f32[0] -= 10;
	wall.r[3].m128_f32[1] -= 1;
	wall.r[3].m128_f32[2] -= 1;
	tThisWorld.atSimpleMesh[wallidex].m_nColor = XMFLOAT4(1, 0, 0, 1);

	int door1Index = CreateDoorWay(&tThisWorld, wall);
	tThisWorld.atSimpleMesh[door1Index].m_nColor = XMFLOAT4(1, 0, 0, 1);

	wall.r[3].m128_f32[0] -= 22;
	int door2Index = CreateDoorWay(&tThisWorld, wall);
	tThisWorld.atSimpleMesh[door2Index].m_nColor = XMFLOAT4(1, 0, 0, 1);


	wall = m_d3dWorldMatrix;
	wall.r[3].m128_f32[1] += -1;
	wall.r[3].m128_f32[0] += -22;
	int wall2index=CreateWall(&tThisWorld, wall);
	tThisWorld.atSimpleMesh[wall2index].m_nColor = XMFLOAT4(0, 1, 0, 1);

	XMMATRIX celling = m_d3dWorldMatrix;
	celling.r[3].m128_f32[1] += 10;
	CreateCelling(&tThisWorld, celling);
	XMMATRIX AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[2] += -5;
	AILocation.r[3].m128_f32[0] += -5;
	//CreateSimpleGunAi(&tThisWorld, AILocation);



	AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[0] += -13;
	AILocation.r[3].m128_f32[2] -= -15;
	XMMATRIX CoverLocation = AILocation;
	AILocation = XMMatrixLookAtLH(AILocation.r[3],
		m_d3dPlayerMatrix.r[3], XMVectorSet(0, 1, 0, 0));
	AILocation = XMMatrixInverse(NULL, AILocation);

	CoverLocation.r[3].m128_f32[2] += -1;
	vector<int> coverPosition;
	int cover1=	CreateCover(&tThisWorld, CoverLocation, coverPosition);
	CoverLocation.r[3].m128_f32[0] += 10;
	int cover2=CreateCover(&tThisWorld, CoverLocation, coverPosition);
	tThisWorld.atSimpleMesh[cover1].m_nColor = XMFLOAT4(0, 0, 1, 1);
	tThisWorld.atSimpleMesh[cover2].m_nColor = XMFLOAT4(0, 0, 1, 1);

	XMMATRIX coverTriggerMatrix = CoverLocation;
	coverTriggerMatrix.r[3].m128_f32[2] -= 14;
	coverTriggerMatrix.r[3].m128_f32[1] -= 1;
	coverTriggerMatrix.r[3].m128_f32[0] -= 8;

	vector<int> coverIndexs;
	coverIndexs.push_back(cover2);

	//int coverTrigerIndex=CreateCoverTriggerZone(&tThisWorld, coverTriggerMatrix);
	coverTriggerMatrix.r[3].m128_f32[2] += 10;
	coverTriggerMatrix.r[3].m128_f32[0] += -3;

	int coverTrigerIndex2 = CreateCoverTriggerZone(&tThisWorld, coverTriggerMatrix);
	coverTriggerMatrix.r[3].m128_f32[0] += 10;

	int coverTrigerIndex3 = CreateCoverTriggerZone(&tThisWorld, coverTriggerMatrix);


	//tThisWorld.atSimpleMesh[coverTrigerIndex].m_nColor = XMFLOAT4(0, 0, 1, 1);
	tThisWorld.atSimpleMesh[coverTrigerIndex2].m_nColor = XMFLOAT4(0, 0, 1, 1);
	tThisWorld.atSimpleMesh[coverTrigerIndex3].m_nColor = XMFLOAT4(0, 0, 1, 1);




	XMMATRIX nodeLocation = CoverLocation;
	nodeLocation.r[3].m128_f32[0] += 3;
	nodeLocation.r[3].m128_f32[1] -= 1;
	nodeLocation.r[3].m128_f32[2] += 2;

	//pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	int nodeindex = CreateNodePoint(&tThisWorld, nodeLocation);
	XMFLOAT3 nodePosition;
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex, nodePosition, 1);
	nodeLocation = AILocation;
	nodeLocation.r[3].m128_f32[2] += 2;
	int nodeindex2 = CreateNodePoint(&tThisWorld, AILocation);
	tThisWorld.atCover[cover1].CoverPositions.push_back(nodeindex);
	tThisWorld.atCover[cover2].CoverPositions.push_back(nodeindex2);
	/*tThisWorld.atCoverTrigger[coverTrigerIndex2].coverAiCanGoTo.push_back(tThisWorld.atCover[cover1]);
	tThisWorld.atCoverTrigger[coverTrigerIndex].coverAiCanGoTo.push_back(tThisWorld.atCover[cover2]);
	tThisWorld.atCoverTrigger[coverTrigerIndex3].coverAiCanGoTo.push_back(tThisWorld.atCover[cover2]);
*/
	AILocation.r[3].m128_f32[1] -= 1;
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex2, nodePosition, 1);
	AILocation.r[3].m128_f32[2] -= 4;
	AILocation.r[3].m128_f32[0] -= 3;
	nodePosition.x = AILocation.r[3].m128_f32[0];
	nodePosition.y = AILocation.r[3].m128_f32[1];
	nodePosition.z = AILocation.r[3].m128_f32[2];
	int nodeindex3 = CreateNodePoint(&tThisWorld, AILocation);
	pcAiSystem->AddNodeToPathFinding(nodeindex3, nodePosition, 1);
	tThisWorld.atCoverTrigger[coverTrigerIndex2].coverAiCanGoTo.push_back(tThisWorld.atCover[cover1]);
	//tThisWorld.atCoverTrigger[coverTrigerIndex].coverAiCanGoTo.push_back(tThisWorld.atCover[cover2]);
	tThisWorld.atCoverTrigger[coverTrigerIndex3].coverAiCanGoTo.push_back(tThisWorld.atCover[cover2]);


	vector<int> edges;
	edges.push_back(nodeindex);
	pcAiSystem->AddEdgestoNode(nodeindex2, edges);
	pcAiSystem->AddEdgestoNode(nodeindex3, edges);

	edges.clear();
	edges.push_back(nodeindex2);
	pcAiSystem->AddEdgestoNode(nodeindex, edges);
	edges.clear();
	edges.push_back(nodeindex2);
	pcAiSystem->AddEdgestoNode(nodeindex3, edges);



	int spacePirate = CreateSpacePirate(&tThisWorld, AILocation);
	tThisWorld.atAiHeath[spacePirate].heath = 100;
	
	//tThisWorld.atPathPlanining[spacePirate].Goal = nodeindex;
	tThisWorld.atPathPlanining[spacePirate].startingNode = nodeindex3;
	tThisWorld.atPathPlanining[spacePirate].Goal = nodeindex2;


	int GunINdexai = CreateGun(&tThisWorld, m_d3dWorldMatrix, spacePirate, -1.1, 0.5, 11.5, 10, 70);
	tThisWorld.atAIMask[spacePirate].GunIndex = GunINdexai;

	tThisWorld.atClip[GunINdexai].bulletSpeed = 0.0001;//Frame Dependent
	std::array<plane_t, 6> planes;
	float4x4 secondcamera;

	secondcamera.row1.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[0];
	secondcamera.row1.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[1];
	secondcamera.row1.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[2];
	secondcamera.row1.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[3];

	secondcamera.row2.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[0];
	secondcamera.row2.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[1];
	secondcamera.row2.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[2];
	secondcamera.row2.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[3];

	secondcamera.row3.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[0];
	secondcamera.row3.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[1];
	secondcamera.row3.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[2];
	secondcamera.row3.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[3];

	secondcamera.row4.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[0];
	secondcamera.row4.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[1];
	secondcamera.row4.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[2];
	secondcamera.row4.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[3];

	frustumIndex = pcAiSystem->calculate_frustum(&tThisWorld, planes, secondcamera, 70, 1, 0.1, 20, spacePirate, -2.1, 1.4, 19.6);
	tThisWorld.atWorldMatrix[frustumIndex].worldMatrix = AILocation;

	tThisWorld.atAIVision[spacePirate].eyes0 = planes;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[0] = planes[0].normal;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[1] = planes[1].normal;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[2] = planes[2].normal;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[3] = planes[3].normal;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[4] = planes[4].normal;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[5] = planes[5].normal;
	AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[0] += -3;
	AILocation.r[3].m128_f32[2] += -5;

	//CreateSimpleGunAi(&tThisWorld, AILocation);
	XMMATRIX groundSpawnPoint;
	groundSpawnPoint = m_d3dWorldMatrix;
	groundSpawnPoint.r[3].m128_f32[1] -= 2;
	int groundindex=CreateGround(&tThisWorld, groundSpawnPoint);
	groundSpawnPoint.r[3].m128_f32[2] -= -20;
	int groundindex2=	CreateGround(&tThisWorld, groundSpawnPoint);
	tThisWorld.atSimpleMesh[groundindex].m_nColor= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	tThisWorld.atSimpleMesh[groundindex2].m_nColor = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	tThisWorld.atAIVision[spacePirate].indexLookingAt=PlayerStartIndex;

	tempImport = pcGraphicsSystem->ReadMesh("meshData_Example_Objects.txt");

	for (int meshIndex = 1; meshIndex < tempImport.meshCount; meshIndex++)
	{
		//int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	}

	tempImport = pcGraphicsSystem->ReadMesh("meshData_ScifiRoom.txt");

	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		//int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	}
	//AILocation.r[3].m128_f32[0] -= 7;
	//int nodeLocation = CreateNodePoint(&tThisWorld, AILocation);
	//tThisWorld.atSimpleMesh[nodeLocation].m_nColor = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	//XMFLOAT3 nodePosition;
	//nodePosition.x = AILocation.r[3].m128_f32[0];
	//nodePosition.y = AILocation.r[3].m128_f32[1];
	//nodePosition.z = AILocation.r[3].m128_f32[2];

	//pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	//XMMATRIX part2ofNodeMovement = AILocation;
	//part2ofNodeMovement.r[3].m128_f32[2] -= -10;
	//int nodelocation4 = CreateNodePoint(&tThisWorld, part2ofNodeMovement);
	//nodePosition.x = part2ofNodeMovement.r[3].m128_f32[0];
	//nodePosition.y = part2ofNodeMovement.r[3].m128_f32[1];
	//nodePosition.z = part2ofNodeMovement.r[3].m128_f32[2];

	//pcAiSystem->AddNodeToPathFinding(nodelocation4, nodePosition, 1);
	//AILocation.r[3].m128_f32[0] += 4;
	//AILocation.r[3].m128_f32[2] += -4;

	//int nodeLocation2 = CreateNodePoint(&tThisWorld, AILocation);

	//nodePosition.x = AILocation.r[3].m128_f32[0];
	//nodePosition.y = AILocation.r[3].m128_f32[1];
	//nodePosition.z = AILocation.r[3].m128_f32[2];

	//pcAiSystem->AddNodeToPathFinding(nodeLocation2, nodePosition, 1);
	//XMMATRIX pathfindAI = AILocation;
	//pathfindAI.r[3].m128_f32[0] += 3;
	//nodePosition.x = pathfindAI.r[3].m128_f32[0];
	//nodePosition.y = pathfindAI.r[3].m128_f32[1];
	//nodePosition.z = pathfindAI.r[3].m128_f32[2];
	//int aiPathIndex = CreateTestAIPathFinding(&tThisWorld, pathfindAI);
	//tThisWorld.atSimpleMesh[aiPathIndex].m_nColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	//int nodelocation3 = CreateNodePoint(&tThisWorld, pathfindAI);
	//tThisWorld.atPathPlanining[aiPathIndex].startingNode = nodelocation3;
	//tThisWorld.atPathPlanining[aiPathIndex].Goal = nodeLocation;
	//pcAiSystem->AddNodeToPathFinding(nodelocation3, nodePosition, 1);
	//vector<int> edges;
	//edges.push_back(nodelocation4);
	//edges.push_back(nodeLocation2);
	//edges.push_back(nodelocation3);
	//pcAiSystem->AddEdgestoNode(nodeLocation, edges);
	//edges.clear();
	//edges.push_back(nodeLocation);
	//edges.push_back(nodelocation3);
	//pcAiSystem->AddEdgestoNode(nodeLocation2, edges);
	//edges.clear();
	////	edges.push_back(nodeLocation);
	//edges.push_back(nodeLocation2);
	//pcAiSystem->AddEdgestoNode(nodelocation3, edges);



	if (pcCollisionSystem->m_AAbb.size() != 0) {
		float x = 0;
	}

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
				if (nCurrentEntity == door1Index || nCurrentEntity == door2Index|| swordGuy==nCurrentEntity) {
					pcCollisionSystem->AddAiVisioNCheck(MyAbb, nCurrentEntity);
				}

			}
			if (tThisWorld.atMesh[nCurrentEntity].m_nVertexCount > tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount)
			{
				TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atMesh[nCurrentEntity].m_VertexData, tThisWorld.atAABB[nCurrentEntity]);
				MyAbb.m_IndexLocation = nCurrentEntity;
				tThisWorld.atAABB[nCurrentEntity] = MyAbb;
				pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);
				if (nCurrentEntity == PlayerStartIndex) {
					pcCollisionSystem->AddAiVisioNCheck(MyAbb, nCurrentEntity);

				}
			}
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

int CGameMangerSystem::SpacePirateGamePlay()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(-1); //Important!
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective(m_RealTimeFov);

	//tTimerInfo->applicationTime = tTimerInfo->GetTime(tAugerTimers->tAppTimer, tTimerInfo->applicationTime);
	//tTimerInfo->sceneTime = tTimerInfo->GetTime(tAugerTimers->tSceneTimer, tTimerInfo->sceneTime);

	tCameraMode = pcInputSystem->CameraModeListen(tCameraMode);

	static XMMATRIX m_d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	static XMMATRIX m_d3dOffsetMatrix = pcGraphicsSystem->SetDefaultOffset();

	if (pcInputSystem->InputCheck(G_KEY_P))
	{
		return 3;
	}
	// ui stuff
	if (GamePaused == true) {
		if (DrawUI == true) {
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
			UiPos.r[3].m128_f32[0] += 0.8;
			UiPos.r[3].m128_f32[1] -= 0.3;

			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] -= 0.7;

			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] -= 0.7;

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
		m_d3d_ResultMatrix = pcInputSystem->WalkCameraControls(XMVectorSet(0, 1.0f, 0, 0), m_d3d_ResultMatrix, bMoving);

		walkCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);
		walkCamera->d3d_Position = XMMatrixMultiply(m_d3dOffsetMatrix, walkCamera->d3d_Position);






	}
	else if (tCameraMode.bAimMode == true)
	{
		if (tCameraMode.bSwitch == true)
		{
			m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
			//CameraNewPosition = pcInputSystem->CameraBehaviorLerp(m_d3d_ResultMatrix, m_d3dPlayerMatrix);
			m_d3dPlayerMatrix = pcInputSystem->AimMode(m_d3dPlayerMatrix);

			CameraNewPosition = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);

			CameraNewPosition = XMMatrixMultiply(m_d3dOffsetMatrix, CameraNewPosition);
			aimCamera->d3d_Position = pcInputSystem->CameraBehaviorLerp(walkCamera->d3d_Position, CameraNewPosition,scale);
			scale += 0.001;
			if (scale > 1) {
				tCameraMode.bSwitch = false;
				scale = 0;

			}
		
		}
		else {
			m_RealTimeFov = pcInputSystem->ZoomSight(m_RealTimeFov);


			m_d3dPlayerMatrix = pcInputSystem->AimMode(m_d3dPlayerMatrix);

			aimCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);

			aimCamera->d3d_Position = XMMatrixMultiply(m_d3dOffsetMatrix, aimCamera->d3d_Position);
		}

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
		tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		{
			if (nCurrentEntity == rayindex) {
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix;
			}
			pcGraphicsSystem->InitPrimalShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tMyVertexBufferTemp.m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], tMyVertexBufferTemp.m_d3dViewMatrix);

			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

		}
		// ai code would run here
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			if (tCameraMode.bWalkMode == true)
			{
				tMyVertexBufferTemp.m_d3dViewMatrix = walkCamera->d3d_Position;
				tTempVertexBuffer.m_d3dViewMatrix = walkCamera->d3d_Position;
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
				tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;

			}
			else {
				tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
				tTempVertexBuffer.m_d3dViewMatrix = m_d3dViewMatrix;
				tMyVertexBufferTemp.m_d3dViewMatrix = m_d3dViewMatrix;
			}

			if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK))
			{
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
					m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
				}
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
		
		if (pcAiSystem->GetNumberOfAI() <= 0)
		{
		return -1;
		}
				if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_FOLLOW) || tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_SHOOT | COMPONENT_AIMASK | COMPONENT_FOLLOW)|| (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST)== tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask)
				{
		#if AI_ON				
					if (tThisWorld.atActiveAI[nCurrentEntity].active == true) {
						pcAiSystem->LookAtObject(tThisWorld.atWorldMatrix[tThisWorld.atAIVision[nCurrentEntity].indexLookingAt].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
						pcAiSystem->ShootGun(&tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex]);
					}
		#endif // AI_ON
				}

		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SEARCH) || tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SPOTEDPLAYER)||(COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST)== tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask) {



			if (tThisWorld.atAIVision[nCurrentEntity].keepSearching == true) {
				if (tThisWorld.atAIVision[nCurrentEntity].visionRotation < 7 && tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true) {
					tThisWorld.atAIVision[nCurrentEntity].visionRotation += 0.001;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcAiSystem->LookBackLeftToRight(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix,
						true);
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true) {
					tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight = false;
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].visionRotation > -7 && tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == false) {
					tThisWorld.atAIVision[nCurrentEntity].visionRotation -= 0.001;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcAiSystem->LookBackLeftToRight(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix,
						false);
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == false) {
					tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight = true;
				}
			}
			else if(tThisWorld.atAIVision[nCurrentEntity].keepSearching == false) {
				pcAiSystem->LookAtObject(tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
			}
		
			float4x4 secondcamera;
			secondcamera.row1.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[0];
			secondcamera.row1.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[1];
			secondcamera.row1.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[2];
			secondcamera.row1.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[3];
		
			secondcamera.row2.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[0];
			secondcamera.row2.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[1];
			secondcamera.row2.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[2];
			secondcamera.row2.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[3];
		
			secondcamera.row3.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[0];
			secondcamera.row3.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[1];
			secondcamera.row3.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[2];
			secondcamera.row3.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[3];
		
			secondcamera.row4.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[0];
			secondcamera.row4.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1];
			secondcamera.row4.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[2];
			secondcamera.row4.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[3];
			pcAiSystem->UpdateFrustum(tThisWorld.atAIVision[nCurrentEntity].eyes0, secondcamera, 70, 1, 0.1, 20);
			//	 pcAiSystem->calculate_frustum(&tThisWorld,tThisWorld.atAIVision[nCurrentEntity].eyes0, secondcamera,70,1,0.1,20, nCurrentEntity, -2.1, 1.4, 19.6);
			vector<int> indicies;
			if (pcCollisionSystem->AiVisionCheck(tThisWorld.atAIVision[nCurrentEntity].eyes0,&indicies) == true) {
				float x = 0;
				bool danger = false;
				for (int i = 0; i < indicies.size(); ++i) {
					if (PlayerStartIndex == indicies[i]) {
						tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
						tThisWorld.atAIVision[nCurrentEntity].keepSearching = false;
						tThisWorld.atActiveAI[nCurrentEntity].active = true;
						danger = true;
						if (tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].nBulletsAvailables.size() <= 0) {
							tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].tryToReload = true;
		
						}
						tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].tryToShoot = true;
						
						pcAiSystem->AddAiInCombat(nCurrentEntity);
				//		tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask = COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST;
					/*	if (tThisWorld.atPathPlanining[nCurrentEntity].foundDestination == true) {
							int previousgoal = tThisWorld.atPathPlanining[nCurrentEntity].Goal;
							int previousStartPosition = tThisWorld.atPathPlanining[nCurrentEntity].startingNode;
							tThisWorld.atPathPlanining[nCurrentEntity].Goal = previousStartPosition;
							tThisWorld.atPathPlanining[nCurrentEntity].startingNode = previousgoal;
							tThisWorld.atPathPlanining[nCurrentEntity].foundDestination = false;
							tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding = true;
		
						}*/
						
					}
					else if (tThisWorld.atProjectiles[indicies[i]].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
						danger = true;
						tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		
					}
					else if(danger==false) {	
				tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
					}
					else if (tThisWorld.atAIMask[indicies[i]].m_tnAIMask == COMPONENT_AIMASK | COMPONENT_FIGHTINGAI) {
						float x = 0;
					}
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

		}
		//else if(tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask==())
		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_PATHFINDTEST) || (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST) == tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask) {
			if (tThisWorld.atActiveAI[nCurrentEntity].active == true) {
				if (tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding == true) {
		
					pcAiSystem->FindBestPath(tThisWorld.atPathPlanining[nCurrentEntity].startingNode,
						tThisWorld.atPathPlanining[nCurrentEntity].Goal,
						&tThisWorld.atPathPlanining[nCurrentEntity].directions);
		
					tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding = false;
				}
				else {
					if (tThisWorld.atPathPlanining[nCurrentEntity].DelayMovement <= 0) {
						pcAiSystem->PathPlaningMovement(&tThisWorld.atPathPlanining[nCurrentEntity], &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
					}
					else {
						tThisWorld.atPathPlanining[nCurrentEntity].DelayMovement -= 0.001;
					}
				}
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
					rayindex = CreateRayBullet(&tThisWorld, gun, 10, GunIndexForPlayer, -0.6, 0.6, 10.7);
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
				else if (tThisWorld.atClip[nCurrentEntity].tryToShoot == true) {
					tThisWorld.atClip[nCurrentEntity].tryToShoot = false;
				}
				if (tThisWorld.atClip[nCurrentEntity].tryToReload == true) {
					pcProjectileSystem->Reload(&tThisWorld.atClip[nCurrentEntity]);
					tThisWorld.atClip[nCurrentEntity].tryToReload = false;

				}
				if (tThisWorld.atClip[nCurrentEntity].nBulletsFired.size() != 0) {
					for (int i = 0; i < tThisWorld.atClip[nCurrentEntity].nBulletsFired.size(); ++i) {
						if (tThisWorld.atClip[nCurrentEntity].fAliveTime[i] < tThisWorld.atClip[nCurrentEntity].FValueOfCoolDown) {
							pcPhysicsSystem->AddBulletForce(&tThisWorld.atRigidBody[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]], tThisWorld.atClip[nCurrentEntity].bulletSpeed);
							tThisWorld.atClip[nCurrentEntity].fAliveTime[i] += 0.1;

						}
						else {
							tThisWorld.atClip[nCurrentEntity].fAliveTime.erase(tThisWorld.atClip[nCurrentEntity].fAliveTime.begin() + i);
							pcCollisionSystem->RemoveAABBCollider(tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]);
							pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]);
							tThisWorld.atClip[nCurrentEntity].nBulletsFired.erase(tThisWorld.atClip[nCurrentEntity].nBulletsFired.begin() + i);
							for (int i = 0; i < tThisWorld.atClip[nCurrentEntity].fAliveTime.size(); ++i) {
								if (tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]].indexInclip != 0) {
									tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]].indexInclip -= 1;
								}

							}
						}
					}

				}
				if (tThisWorld.atClip[nCurrentEntity].fShootingCoolDown > 0)
					tThisWorld.atClip[nCurrentEntity].fShootingCoolDown -= 0.1;
			}


		}


		if (nCurrentEntity == PlayerStartIndex) {
			float x = 0;
		}
		tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, true);

		if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_RAYGUN)) {
			float CloseEstObject = 10000000000000000000.0f;
			float* distanceCalucaltion = new float();
			for (list<TAABB>::iterator ptr = pcCollisionSystem->m_AAbb.begin(); ptr != pcCollisionSystem->m_AAbb.end(); ++ptr) {

				if (ptr->m_IndexLocation != PlayerStartIndex&&ptr->m_IndexLocation!=GunIndexForPlayer)
					if (pcCollisionSystem->intersectRayAABox2(XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[0], tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix)
						, XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[1], tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix),*ptr
						) == true) {
						//CloseEstObject = *distanceCalucaltion;
						tThisWorld.atClip[GunIndexForPlayer].currentMaterial = 0;
						cout << "turtle" << '/n';
						tThisWorld.atClip[GunIndexForPlayer].colorofBullets = tThisWorld.atSimpleMesh[ptr->m_IndexLocation].m_nColor;
					}
			}
			delete distanceCalucaltion;
		}


		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
			COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC) || tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
			COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)) {
			tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
		}
			
			
	if ((tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_TRIGGER) | tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)))
	{

		vector<int> otherCollisionsIndex;
		if (pcCollisionSystem->AABBtoAABBCollisionCheck(tThisWorld.atAABB[nCurrentEntity], &otherCollisionsIndex) == true)
		{
			for (int i = 0; i < otherCollisionsIndex.size(); ++i) {
				if (tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == true && tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask !=
					(COMPONENT_COLLISIONMASK |
						COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)) {
					if (nCurrentEntity == 8)
					{
						float x = 0;
					}

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
							COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))) {
					tTempVertexBuffer.m_d3dWorldMatrix = pcCollisionSystem->WalkingThrewObjectCheck(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[otherCollisionsIndex[i]], tThisWorld.atAABB[nCurrentEntity]);
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tTempVertexBuffer.m_d3dWorldMatrix;
					if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK)) {
						m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
					}

				}
				if (tThisWorld.atRigidBody[nCurrentEntity].ground == false && tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == false && tThisWorld.atRigidBody[nCurrentEntity].wall == false &&
					tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
						COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)
					&& (tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
						COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_STATIC)) | tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))
				{
					
					if (tThisWorld.atAIMask[otherCollisionsIndex[i]].m_tnAIMask >1) {
						if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
							if (tThisWorld.atAiHeath[otherCollisionsIndex[i]].heath <= 0) {
								pcAiSystem->SetNumberOfAI(pcAiSystem->GetNumberOfAI() - 1);
								if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1) {
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
							else {
								tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.erase
								(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.begin()
									+ tThisWorld.atClip[nCurrentEntity].indexInclip);

								pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
								tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.erase(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.begin()
									+ tThisWorld.atClip[nCurrentEntity].indexInclip);


								pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);
								tThisWorld.atAiHeath[otherCollisionsIndex[i]].heath -= 50;
								if (tThisWorld.atAiHeath[otherCollisionsIndex[i]].heath <= 0) {
									pcAiSystem->SetNumberOfAI(pcAiSystem->GetNumberOfAI() - 1);
									pcCollisionSystem->RemoveAABBCollider(otherCollisionsIndex[i]);
									pcGraphicsSystem->CleanD3DObject(&tThisWorld, otherCollisionsIndex[i]);
									pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atAIMask[otherCollisionsIndex[i]].GunIndex);
								}
							}
						}
					}
					else if (tThisWorld.atInputMask[otherCollisionsIndex[i]].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK)) {
						if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
							if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1) {
								tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.erase
								(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.begin()
									+ tThisWorld.atClip[nCurrentEntity].indexInclip);

								pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
								tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.erase(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.begin()
									+ tThisWorld.atClip[nCurrentEntity].indexInclip);

								//pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atAIMask[nCurrentEntity].GunIndex);
								pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);

							}
							tThisWorld.atClayton[otherCollisionsIndex[i]].health -= 50;
						}
						
						
					}
					if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
						if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1) {
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
					
				}
				if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK) && tThisWorld.atAIMask[otherCollisionsIndex[i]].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_COVERTRIGGER)) {
					
					pcAiSystem->MoveAiToCoverLocation(tThisWorld.atCoverTrigger[otherCollisionsIndex[i]], &tThisWorld);
				}
			}
			if (tThisWorld.atClayton[PlayerStartIndex].health <= 0)
			{
				return -1;
			}
			//		tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);


		}
		else {

			//tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;

		}
	}
	else {

		//	tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;

	}
		tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;

		if (tThisWorld.atParentWorldMatrix[nCurrentEntity] != -1)
		{
		tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(pcGraphicsSystem->SetDefaultWorldPosition(),
		tThisWorld.atWorldMatrix[tThisWorld.atParentWorldMatrix[nCurrentEntity]].worldMatrix);

		tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(tThisWorld.atOffSetMatrix[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);

		if (nCurrentEntity == rayindex) {
			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix;
		}
		if (nCurrentEntity == frustumIndex) {
		XMMATRIX empty;
		empty = pcGraphicsSystem->SetDefaultWorldPosition();
		XMMATRIX rotation;
		XMMATRIX d3dTmpViewM;
		rotation = XMMatrixRotationY(-0.3);
		empty.r[1] = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1];
		empty.r[0] = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0];
		d3dTmpViewM = empty;
		XMVECTOR d3d_newX, d3d_newY, d3d_existingZ;
		d3dTmpViewM = XMMatrixMultiply(rotation, d3dTmpViewM);
		d3d_existingZ = d3dTmpViewM.r[2];
		d3d_newX = XMVector3Cross(empty.r[1], d3d_existingZ);
		d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

		d3d_newX = XMVector3Normalize(d3d_newX);
		d3d_newY = XMVector3Normalize(d3d_newY);

		d3d_existingZ = XMVector3Normalize(d3d_existingZ);

		d3dTmpViewM.r[0] = d3d_newX;
		d3dTmpViewM.r[1] = d3d_newY;
		d3dTmpViewM.r[2] = d3d_existingZ;
		empty = d3dTmpViewM;
		empty.r[3].m128_f32[0] = 3;
		empty.r[3].m128_f32[1] = 0;
		empty.r[3].m128_f32[2] = 1;


		tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = empty;
		}
	}

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
		if (tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix.r[3].m128_f32[1] < -10) {
			return 7;
		}

	}
	


	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);
	zValue += 0.001;
	return 8;


}

void CGameMangerSystem::LoadMikesGraphicsSandbox()
{
	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);
	for (int  i = 0; i < matOpt.numberOfMaterials; i++)
	{
		matOpt.SRVArrayOfMaterials[i]->Release();
	}
	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();

	debugCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	tCameraMode.bDebugMode = true;
	tCameraMode.bAimMode = false;
	tCameraMode.bWalkMode = false;
	tCameraMode.bSwitch = false;

	ImporterData tempImport;
	//tempImport = pcGraphicsSystem->ReadMesh("meshData_ScifiRoom.txt");

	//for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	//{
	//	int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	//}

	#pragma region Create Skybox
	ID3D11Resource * spaceMap[6];
	
	
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceBlue/bkg1_back.png", &spaceMap[0], NULL, NULL);
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceBlue/bkg1_left.png", &spaceMap[1], NULL, NULL);
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceBlue/bkg1_right.png", &spaceMap[2], NULL, NULL);
	
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceBlue/bkg1_front.png", &spaceMap[3], NULL, NULL);
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceBlue/bkg1_top.png", &spaceMap[4], NULL, NULL);
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceBlue/bkg1_bot.png", &spaceMap[5], NULL, NULL);

	ID3D11ShaderResourceView * tempSrv = pcGraphicsSystem->TexturesToCubeMap(pcGraphicsSystem->m_pd3dDeviceContext, spaceMap);
	renderToTexturePassIndex = CreateSkybox(&tThisWorld, tempSrv);

#pragma endregion

	//tempImport = pcGraphicsSystem->ReadMesh("meshData_Satellite.txt");
	//TMaterialOptimized matOpt = pcGraphicsSystem->CreateTexturesFromFile(tempImport.vtMaterials, tempImport.meshCount);
	//for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	//{
	//	int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], matOpt, meshIndex);
	//}

	tempImport = pcGraphicsSystem->ReadMesh("meshData_NoBrewery7.txt");
	matOpt =  pcGraphicsSystem->CreateTexturesFromFile(tempImport.vtMaterials, tempImport.meshCount);
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], matOpt, meshIndex);
	}

	//tempImport = pcGraphicsSystem->ReadMesh("meshData_Pirate.txt");
	//matOpt = pcGraphicsSystem->CreateTexturesFromFile(tempImport.vtMaterials, tempImport.meshCount);

	//for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	//{
	//	int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], matOpt, meshIndex);		
	//}
	createGSQuad(&tThisWorld, XMFLOAT4(1, 0, 0, 1));
	createGSQuad(&tThisWorld, XMFLOAT4(1, 1, 1, 1));
	
	pcGraphicsSystem->CreateBuffers(&tThisWorld);
}

int CGameMangerSystem::MikesGraphicsSandbox()
{
	#pragma region Camera
	static XMMATRIX m_d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	static XMMATRIX m_d3dOffsetMatrix = pcGraphicsSystem->SetDefaultOffset();
	tCameraMode = pcInputSystem->CameraModeListen(tCameraMode);
	if (pcInputSystem->InputCheck(G_KEY_P))
	{
		return 3;
	}

	if (pcInputSystem->InputCheck(G_KEY_Y))
	{
		if (Health > 0)
		{
			Health -= .1f;
		}
	}

	if (tCameraMode.bSwitch == true)
	{
		m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
		tCameraMode.bSwitch = false;
	}
	m_d3d_ResultMatrix = pcInputSystem->DebugCamera(m_d3d_ResultMatrix, m_d3dWorldMatrix);

	debugCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dWorldMatrix);

#pragma endregion

	CGraphicsSystem::TPrimalVertexBufferType tTempVertexBuffer;
	CGraphicsSystem::TPrimalPixelBufferType tTempPixelBuffer;
	CGraphicsSystem::TMyVertexBufferType tMyVertexBufferTemp;
	tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
	pcGraphicsSystem->UpdateD3D();
	pcGraphicsSystem->UpdateD3D_RenderToTexture();

	for (int nCurrentEntity = 0; nCurrentEntity <= renderToTexturePassIndex; nCurrentEntity++)
	{
		tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			tTempVertexBuffer.m_d3dWorldMatrix = m_d3dWorldMatrix;
			tTempVertexBuffer.m_d3dViewMatrix = debugCamera->d3d_Position;
			tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;

			pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_SKYBOX | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;
			tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
			pcGraphicsSystem->InitSkyboxShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}
	}
	//pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);

	pcGraphicsSystem->m_pd3dDevice->CreateShaderResourceView(pcGraphicsSystem->m_pd3dOutsideGlassRenderToTexture, NULL, &pcGraphicsSystem->m_pd3dOutsideGlassSRV);
	pcGraphicsSystem->m_pd3dDeviceContext->GenerateMips(pcGraphicsSystem->m_pd3dOutsideGlassSRV);

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			if (tThisWorld.atShaderID[nCurrentEntity].m_nShaderID == 10)
			{
				tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse = pcGraphicsSystem->m_pd3dOutsideGlassSRV;
			}
		}
	}

	pcGraphicsSystem->UpdateD3D();

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		{
			tTempVertexBuffer.m_d3dWorldMatrix = m_d3dWorldMatrix;
			tTempVertexBuffer.m_d3dViewMatrix = debugCamera->d3d_Position;
			tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;
			pcGraphicsSystem->InitQuadShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], debugCamera->d3d_Position, tThisWorld.atBar[nCurrentEntity].backgroundColor, Health);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			tTempVertexBuffer.m_d3dWorldMatrix = m_d3dWorldMatrix;
			tTempVertexBuffer.m_d3dViewMatrix = debugCamera->d3d_Position;
			tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;

			pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			if (tThisWorld.atShaderID[nCurrentEntity].m_nShaderID == 10)
			{
				//tThisWorld.atMesh->m_d3dSRVDiffuse->Release();
			}
		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_SKYBOX | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;
			tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
			pcGraphicsSystem->InitSkyboxShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

	}
	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);

	return 10;
}

bool CGameMangerSystem::GetWalkCameraState()
{
	return tCameraMode.bWalkMode;
}

void CGameMangerSystem::LoadLevelWithMapInIt()
{
	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);

	InitializeHUD();
	InitializePauseScreen();
	InitializeEndScreen();
	GameOver = false;
	GamePaused = false;

	pcAiSystem->SetNumberOfAI(2);
//	tTimerInfo->StartClock(tAugerTimers->tSceneTimer);
	ImporterData tempImport;
	TMaterialOptimized matOpt;

#pragma region Create Skybox
	ID3D11Resource * spaceMap[6];


	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceBlue/bkg1_back.png", &spaceMap[0], NULL, NULL);
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceBlue/bkg1_left.png", &spaceMap[1], NULL, NULL);
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceBlue/bkg1_right.png", &spaceMap[2], NULL, NULL);

	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceBlue/bkg1_front.png", &spaceMap[3], NULL, NULL);
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceBlue/bkg1_top.png", &spaceMap[4], NULL, NULL);
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceBlue/bkg1_bot.png", &spaceMap[5], NULL, NULL);

	ID3D11ShaderResourceView * tempSrv = pcGraphicsSystem->TexturesToCubeMap(pcGraphicsSystem->m_pd3dDeviceContext, spaceMap);
	renderToTexturePassIndex = CreateSkybox(&tThisWorld, tempSrv);

#pragma endregion

	tempImport = pcGraphicsSystem->ReadMesh("meshData_NoBrewery7.txt");
	matOpt = pcGraphicsSystem->CreateTexturesFromFile(tempImport.vtMaterials, tempImport.meshCount);
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
	
		int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], matOpt, meshIndex);
	}

	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
	walkCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	aimCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	debugCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective(m_RealTimeFov);
	m_d3dPlayerMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	tCameraMode.bWalkMode = false;
	tCameraMode.bDebugMode = false;
	tCameraMode.bAimMode = true;
	tCameraMode.bSwitch = false;
	bMoving = false;
	m_RealTimeFov = 90.0f;
	//m_d3dPlayerMatrix = XMMatrixMultiply(m_d3dPlayerMatrix, XMMatrixScaling(.01, .01, .01));

	m_d3dPlayerMatrix.r[3].m128_f32[2] -= 10;
	m_d3dPlayerMatrix.r[3].m128_f32[0] -= 5;
	m_d3dPlayerMatrix.r[3].m128_f32[1] += 0.2;



	tempImport = pcGraphicsSystem->ReadMesh("meshData_Pirate.txt");

	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		PlayerStartIndex = createClayton(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	}
	GunIndexForPlayer = CreateGun(&tThisWorld, m_d3dWorldMatrix, PlayerStartIndex, -1, 1, 10.5, 3, 100);
	tThisWorld.atClip[GunIndexForPlayer].bulletSpeed = 0.01;
	XMMATRIX AILocation= pcGraphicsSystem->SetDefaultWorldPosition();
	AILocation.r[3].m128_f32[2] -= 60;
	AILocation.r[3].m128_f32[0] -= 8;
	AILocation.r[3].m128_f32[1] += 0.2;
	pcAiSystem->LookAtObject(AILocation, &m_d3dPlayerMatrix);
	XMMATRIX nodeLocation= pcGraphicsSystem->SetDefaultWorldPosition();
	nodeLocation.r[3].m128_f32[2] -= 60;
	nodeLocation.r[3].m128_f32[0] -= 8;
	nodeLocation.r[3].m128_f32[1] += -1;

	tThisWorld.atClayton[PlayerStartIndex].health = 100;
	int nodeindex = CreateNodePoint(&tThisWorld, nodeLocation);
	XMFLOAT3 nodePosition;
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	vector<int> edges;
	
	pcAiSystem->AddNodeToPathFinding(nodeindex, nodePosition, 1);
	nodeLocation.r[3].m128_f32[2] += 3;
	//nodeLocation.r[3].m128_f32[0] += 5;
	int nodeindex2 = CreateNodePoint(&tThisWorld, nodeLocation);
	
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex2, nodePosition, 1);
	edges.push_back(nodeindex2);
	pcAiSystem->AddEdgestoNode(nodeindex, edges);
	edges.clear();
	edges.push_back(nodeindex);
	pcAiSystem->AddEdgestoNode(nodeindex2, edges);



	
	/*int nodeindex3 = CreateNodePoint(&tThisWorld, nodeLocation);
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex3, nodePosition, 1);
*/

	int spacePirate = CreateSpacePirate(&tThisWorld, AILocation);
	tThisWorld.atAiHeath[spacePirate].heath = 100;

	tThisWorld.atPathPlanining[spacePirate].Goal = nodeindex2;
	tThisWorld.atPathPlanining[spacePirate].startingNode = nodeindex;
	



	int GunINdexai = CreateGun(&tThisWorld, m_d3dWorldMatrix, spacePirate, -1.1, 0.5, 11.5, 10, 30);
	tThisWorld.atAIMask[spacePirate].GunIndex = GunINdexai;

	tThisWorld.atClip[GunINdexai].bulletSpeed = 0.01;//Frame Dependent
	std::array<plane_t, 6> planes;
	float4x4 secondcamera;

	secondcamera.row1.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[0];
	secondcamera.row1.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[1];
	secondcamera.row1.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[2];
	secondcamera.row1.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[3];

	secondcamera.row2.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[0];
	secondcamera.row2.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[1];
	secondcamera.row2.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[2];
	secondcamera.row2.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[3];

	secondcamera.row3.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[0];
	secondcamera.row3.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[1];
	secondcamera.row3.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[2];
	secondcamera.row3.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[3];

	secondcamera.row4.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[0];
	secondcamera.row4.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[1];
	secondcamera.row4.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[2];
	secondcamera.row4.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[3];

	frustumIndex = pcAiSystem->calculate_frustum(&tThisWorld, planes, secondcamera, 70, 1, 0.1, 20, spacePirate, -2.1, 1.4, 19.6);
	tThisWorld.atWorldMatrix[frustumIndex].worldMatrix = AILocation;

	tThisWorld.atAIVision[spacePirate].eyes0 = planes;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[0] = planes[0].normal;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[1] = planes[1].normal;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[2] = planes[2].normal;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[3] = planes[3].normal;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[4] = planes[4].normal;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[5] = planes[5].normal;
	AILocation.r[3].m128_f32[0] += 3;
	int spacePirate2 = CreateSpacePirate(&tThisWorld, AILocation);
	tThisWorld.atAiHeath[spacePirate2].heath = 100;

	//tThisWorld.atPathPlanining[spacePirate].Goal = nodeindex;
	/*tThisWorld.atPathPlanining[spacePirate].startingNode = nodeindex3;
	tThisWorld.atPathPlanining[spacePirate].Goal = nodeindex2;*/


	int GunINdexai2 = CreateGun(&tThisWorld, m_d3dWorldMatrix, spacePirate2, -1.1, 0.5, 11.5, 10, 70);
	tThisWorld.atAIMask[spacePirate2].GunIndex = GunINdexai2;
	tThisWorld.atAIVision[spacePirate2].keepRotatingRight = false;

	tThisWorld.atClip[GunINdexai2].bulletSpeed = 0.0001;//Frame Dependent
	std::array<plane_t, 6> planes2;
	secondcamera;

	secondcamera.row1.x = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[0].m128_f32[0];
	secondcamera.row1.y = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[0].m128_f32[1];
	secondcamera.row1.z = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[0].m128_f32[2];
	secondcamera.row1.w = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[0].m128_f32[3];

	secondcamera.row2.x = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[1].m128_f32[0];
	secondcamera.row2.y = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[1].m128_f32[1];
	secondcamera.row2.z = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[1].m128_f32[2];
	secondcamera.row2.w = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[1].m128_f32[3];

	secondcamera.row3.x = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[2].m128_f32[0];
	secondcamera.row3.y = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[2].m128_f32[1];
	secondcamera.row3.z = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[2].m128_f32[2];
	secondcamera.row3.w = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[2].m128_f32[3];

	secondcamera.row4.x = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[3].m128_f32[0];
	secondcamera.row4.y = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[3].m128_f32[1];
	secondcamera.row4.z = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[3].m128_f32[2];
	secondcamera.row4.w = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[3].m128_f32[3];

	frustumIndex = pcAiSystem->calculate_frustum(&tThisWorld, planes2, secondcamera, 70, 1, 0.1, 20, spacePirate2, -2.1, 1.4, 19.6);
	tThisWorld.atWorldMatrix[frustumIndex].worldMatrix = AILocation;

	tThisWorld.atAIVision[spacePirate2].eyes0 = planes2;
	tThisWorld.atAIVision[spacePirate2].normalAtBegining[0] = planes2[0].normal;
	tThisWorld.atAIVision[spacePirate2].normalAtBegining[1] = planes2[1].normal;
	tThisWorld.atAIVision[spacePirate2].normalAtBegining[2] = planes2[2].normal;
	tThisWorld.atAIVision[spacePirate2].normalAtBegining[3] = planes2[3].normal;
	tThisWorld.atAIVision[spacePirate2].normalAtBegining[4] = planes2[4].normal;
	tThisWorld.atAIVision[spacePirate2].normalAtBegining[5] = planes2[5].normal;
	nodeLocation = AILocation;
	nodeLocation.r[3].m128_f32[1] += -1;

	int nodeindex3 = CreateNodePoint(&tThisWorld, nodeLocation);
	 nodePosition;
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	

	pcAiSystem->AddNodeToPathFinding(nodeindex3, nodePosition, 1);
	nodeLocation.r[3].m128_f32[2] += 3;
	//nodeLocation.r[3].m128_f32[0] += 5;
	int nodeindex4 = CreateNodePoint(&tThisWorld, nodeLocation);
	edges.clear();
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex4, nodePosition, 1);
	edges.push_back(nodeindex4);
	pcAiSystem->AddEdgestoNode(nodeindex3, edges);
	edges.clear();
	edges.push_back(nodeindex3);
	pcAiSystem->AddEdgestoNode(nodeindex4, edges);

	tThisWorld.atPathPlanining[spacePirate2].Goal = nodeindex3;
	tThisWorld.atPathPlanining[spacePirate2].startingNode = nodeindex4;
	//int nodeindex2 = CreateNodePoint(&tThisWorld, nodeLocation);

	//nodePosition.x = nodeLocation.r[3].m128_f32[0];
	//nodePosition.y = nodeLocation.r[3].m128_f32[1];
	//nodePosition.z = nodeLocation.r[3].m128_f32[2];
	//pcAiSystem->AddNodeToPathFinding(nodeindex2, nodePosition, 1);
	//edges.push_back(nodeindex2);
	//pcAiSystem->AddEdgestoNode(nodeindex, edges);
	//edges.clear();
	//edges.push_back(nodeindex);
	//pcAiSystem->AddEdgestoNode(nodeindex2, edges);


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
				/*if (nCurrentEntity == door1Index || nCurrentEntity == door2Index || swordGuy == nCurrentEntity) {
					pcCollisionSystem->AddAiVisioNCheck(MyAbb, nCurrentEntity);
				}*/

			}
			if (tThisWorld.atMesh[nCurrentEntity].m_nVertexCount > tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount)
			{
				TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atMesh[nCurrentEntity].m_VertexData, tThisWorld.atAABB[nCurrentEntity]);
				MyAbb.m_IndexLocation = nCurrentEntity;
				tThisWorld.atAABB[nCurrentEntity] = MyAbb;
				pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);
				if (nCurrentEntity == PlayerStartIndex) {
					pcCollisionSystem->AddAiVisioNCheck(MyAbb, nCurrentEntity);

				}
			}
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

int CGameMangerSystem::RealLevelUpdate()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(-1); //Important!
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective(m_RealTimeFov);

	//tTimerInfo->applicationTime = tTimerInfo->GetTime(tAugerTimers->tAppTimer, tTimerInfo->applicationTime);
	//tTimerInfo->sceneTime = tTimerInfo->GetTime(tAugerTimers->tSceneTimer, tTimerInfo->sceneTime);

	tCameraMode = pcInputSystem->CameraModeListen(tCameraMode);

	static XMMATRIX m_d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	static XMMATRIX m_d3dOffsetMatrix = pcGraphicsSystem->SetDefaultOffset();
	CGraphicsSystem::TPrimalVertexBufferType tTempVertexBuffer;
	CGraphicsSystem::TPrimalPixelBufferType tTempPixelBuffer;
	CGraphicsSystem::TMyVertexBufferType tMyVertexBufferTemp;
	CGraphicsSystem::TUIVertexBufferType tUIVertexBuffer;
	CGraphicsSystem::TUIPixelBufferType tUIPixelBuffer;

	if (pcInputSystem->InputCheck(G_KEY_P) && !GameOver)
	{
		GamePaused = true;
	}
	if (pcInputSystem->InputCheck(G_KEY_U) && !options && !GameOver)
	{
		GamePaused = false;
	}

	clickTimer.Signal();

	/*// ui stuff
	if (GamePaused == true) {
		if (DrawUI == true) {
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
			UiPos.r[3].m128_f32[0] += 0.8;
			UiPos.r[3].m128_f32[1] -= 0.3;

			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] -= 0.7;

			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] -= 0.7;

			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[0] -= 1;

			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
		}
	}*/

	if (tCameraMode.bWalkMode == true)
	{

		if (tCameraMode.bSwitch == true)
		{
			m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
			tCameraMode.bSwitch = false;
		}
		m_d3d_ResultMatrix = pcInputSystem->WalkCameraControls(XMVectorSet(0, 1.0f, 0, 0), m_d3d_ResultMatrix, bMoving);

		walkCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);
		walkCamera->d3d_Position = XMMatrixMultiply(m_d3dOffsetMatrix, walkCamera->d3d_Position);
		tMyVertexBufferTemp.m_d3dViewMatrix = walkCamera->d3d_Position;
		tTempVertexBuffer.m_d3dViewMatrix = walkCamera->d3d_Position;
	}
	else if (tCameraMode.bAimMode == true)
	{
	
		if (tCameraMode.bSwitch == true)
		{
			m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
			//CameraNewPosition = pcInputSystem->CameraBehaviorLerp(m_d3d_ResultMatrix, m_d3dPlayerMatrix);
			m_d3dPlayerMatrix = pcInputSystem->AimMode(m_d3dPlayerMatrix);

			CameraNewPosition = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);

			CameraNewPosition = XMMatrixMultiply(m_d3dOffsetMatrix, CameraNewPosition);
			aimCamera->d3d_Position = pcInputSystem->CameraBehaviorLerp(walkCamera->d3d_Position, CameraNewPosition, scale);
			scale += 0.001;
			if (scale > 1) {
				tCameraMode.bSwitch = false;
				scale = 0;

			}


		}
		else {
			m_RealTimeFov = pcInputSystem->ZoomSight(m_RealTimeFov);




			m_d3dPlayerMatrix = pcInputSystem->AimMode(m_d3dPlayerMatrix);

			aimCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);

			aimCamera->d3d_Position = XMMatrixMultiply(m_d3dOffsetMatrix, aimCamera->d3d_Position);
		}
		tMyVertexBufferTemp.m_d3dViewMatrix = aimCamera->d3d_Position;
		tTempVertexBuffer.m_d3dViewMatrix = aimCamera->d3d_Position;

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
		tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;
		tTempVertexBuffer.m_d3dViewMatrix = debugCamera->d3d_Position;

	}

	
	tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
	pcGraphicsSystem->UpdateD3D();
	pcGraphicsSystem->UpdateD3D_RenderToTexture();

	for (int nCurrentEntity = 0; nCurrentEntity <= renderToTexturePassIndex; nCurrentEntity++)
	{
		tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			tTempVertexBuffer.m_d3dWorldMatrix = m_d3dWorldMatrix;
		
	

			pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], tMyVertexBufferTemp.m_d3dViewMatrix);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_SKYBOX | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
	
			tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
			pcGraphicsSystem->InitSkyboxShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], tMyVertexBufferTemp.m_d3dViewMatrix);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}
	}
	//pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);

	pcGraphicsSystem->m_pd3dDevice->CreateShaderResourceView(pcGraphicsSystem->m_pd3dOutsideGlassRenderToTexture, NULL, &pcGraphicsSystem->m_pd3dOutsideGlassSRV);
	pcGraphicsSystem->m_pd3dDeviceContext->GenerateMips(pcGraphicsSystem->m_pd3dOutsideGlassSRV);

	POINT hoverPoint;
	GetCursorPos(&hoverPoint);
	ScreenToClient(cApplicationWindow, &hoverPoint);

	POINT clickPoint = { -1, -1 };
	if (pcInputSystem->InputCheck(G_BUTTON_LEFT) == 1)
	{
		GetCursorPos(&clickPoint);
		ScreenToClient(cApplicationWindow, &clickPoint);

		if (GameOver && tThisWorld.atClayton[PlayerStartIndex].health > 0)
			return 2;
	}

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			if (tThisWorld.atShaderID[nCurrentEntity].m_nShaderID == 10)
			{
				tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse = pcGraphicsSystem->m_pd3dOutsideGlassSRV;
			}
		}
	}

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
		tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_SKYBOX | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
			pcGraphicsSystem->InitSkyboxShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		{
			if (nCurrentEntity == rayindex) {
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix;
			}
			pcGraphicsSystem->InitPrimalShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tMyVertexBufferTemp.m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], tMyVertexBufferTemp.m_d3dViewMatrix);

			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

		}
		// ai code would run here
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{

			/*if (nCurrentEntity == 2 || nCurrentEntity == 3) {
				continue;
			}*/
			if (tCameraMode.bWalkMode == true)
			{
				tMyVertexBufferTemp.m_d3dViewMatrix = walkCamera->d3d_Position;
				tTempVertexBuffer.m_d3dViewMatrix = walkCamera->d3d_Position;
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
				tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;

			}
			else {
				tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
				tTempVertexBuffer.m_d3dViewMatrix = m_d3dViewMatrix;
				tMyVertexBufferTemp.m_d3dViewMatrix = m_d3dViewMatrix;
			}

			if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK))
			{
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
					m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
				}
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

		if (pcAiSystem->GetNumberOfAI() <= 0)
		{
			return -1;
		}
		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_FOLLOW) || tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_SHOOT | COMPONENT_AIMASK | COMPONENT_FOLLOW) || (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST) == tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask)
		{
#if AI_ON				
			if (tThisWorld.atActiveAI[nCurrentEntity].active == true) {
				pcAiSystem->LookAtObject(tThisWorld.atWorldMatrix[tThisWorld.atAIVision[nCurrentEntity].indexLookingAt].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
				pcAiSystem->ShootGun(&tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex]);
			}
#endif // AI_ON
		}

		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SEARCH) || tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SPOTEDPLAYER) || (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST) == tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask) {



			if (tThisWorld.atAIVision[nCurrentEntity].keepSearching == true) {
				if (tThisWorld.atAIVision[nCurrentEntity].visionRotation < 7 && tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true) {
					tThisWorld.atAIVision[nCurrentEntity].visionRotation += 0.01;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcAiSystem->LookBackLeftToRight(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix,
						true);
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true) {
					tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight = false;
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].visionRotation > -7 && tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == false) {
					tThisWorld.atAIVision[nCurrentEntity].visionRotation -= 0.01;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcAiSystem->LookBackLeftToRight(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix,
						false);
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == false) {
					tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight = true;
				}
			}
			else if (tThisWorld.atAIVision[nCurrentEntity].keepSearching == false) {
				pcAiSystem->LookAtObject(tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
			}

			float4x4 secondcamera;
			secondcamera.row1.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[0];
			secondcamera.row1.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[1];
			secondcamera.row1.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[2];
			secondcamera.row1.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[3];

			secondcamera.row2.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[0];
			secondcamera.row2.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[1];
			secondcamera.row2.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[2];
			secondcamera.row2.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[3];

			secondcamera.row3.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[0];
			secondcamera.row3.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[1];
			secondcamera.row3.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[2];
			secondcamera.row3.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[3];

			secondcamera.row4.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[0];
			secondcamera.row4.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1];
			secondcamera.row4.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[2];
			secondcamera.row4.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[3];
			pcAiSystem->UpdateFrustum(tThisWorld.atAIVision[nCurrentEntity].eyes0, secondcamera, 70, 1, 0.1, 20);
			//	 pcAiSystem->calculate_frustum(&tThisWorld,tThisWorld.atAIVision[nCurrentEntity].eyes0, secondcamera,70,1,0.1,20, nCurrentEntity, -2.1, 1.4, 19.6);
			vector<int> indicies;
			if (pcCollisionSystem->AiVisionCheck(tThisWorld.atAIVision[nCurrentEntity].eyes0, &indicies) == true) {
				float x = 0;
				bool danger = false;
				for (int i = 0; i < indicies.size(); ++i) {
					if (PlayerStartIndex == indicies[i]) {
						tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
						tThisWorld.atAIVision[nCurrentEntity].keepSearching = false;
						tThisWorld.atActiveAI[nCurrentEntity].active = true;
						danger = true;
						if (tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].nBulletsAvailables.size() <= 0) {
							tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].tryToReload = true;

						}
						tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].tryToShoot = true;

						//pcAiSystem->AddAiInCombat(nCurrentEntity);
						//		tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask = COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST;
						/*	if (tThisWorld.atPathPlanining[nCurrentEntity].foundDestination == true) {
						int previousgoal = tThisWorld.atPathPlanining[nCurrentEntity].Goal;
						int previousStartPosition = tThisWorld.atPathPlanining[nCurrentEntity].startingNode;
						tThisWorld.atPathPlanining[nCurrentEntity].Goal = previousStartPosition;
						tThisWorld.atPathPlanining[nCurrentEntity].startingNode = previousgoal;
						tThisWorld.atPathPlanining[nCurrentEntity].foundDestination = false;
						tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding = true;

						}*/

					}
					else if (tThisWorld.atProjectiles[indicies[i]].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
						danger = true;
						tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

					}
					else if (danger == false) {
						tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
					}
					else if (tThisWorld.atAIMask[indicies[i]].m_tnAIMask == COMPONENT_AIMASK | COMPONENT_FIGHTINGAI) {
						float x = 0;
					}
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

		}
		//else if(tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask==())
		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_PATHFINDTEST) || (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST) == tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask) {
			if (tThisWorld.atActiveAI[nCurrentEntity].active == true) {
				if (tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding == true) {

					pcAiSystem->FindBestPath(tThisWorld.atPathPlanining[nCurrentEntity].startingNode,
						tThisWorld.atPathPlanining[nCurrentEntity].Goal,
						&tThisWorld.atPathPlanining[nCurrentEntity].directions);

					tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding = false;
				}
				else {
					if (tThisWorld.atPathPlanining[nCurrentEntity].DelayMovement <= 0) {
						pcAiSystem->PathPlaningMovement(&tThisWorld.atPathPlanining[nCurrentEntity], &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
					}
					else {
						tThisWorld.atPathPlanining[nCurrentEntity].DelayMovement -= 0.001;
					}
				}
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
					rayindex = CreateRayBullet(&tThisWorld, gun, 10, GunIndexForPlayer, -0.6, 0.6, 10.7);
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
					wchar_t* textBuffer = new wchar_t[1];

					textBuffer[0] = (tThisWorld.atClip[nCurrentEntity].nBulletsAvailables.size() - 1);

					pcUISystem->UpdateText(&tThisWorld, 1094, &atUIVertices, textBuffer, atUIVertices.at(tThisWorld.atLabel[1094].vIndex));

					pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, 1094);

					delete[] textBuffer;

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
				else if (tThisWorld.atClip[nCurrentEntity].tryToShoot == true) {
					tThisWorld.atClip[nCurrentEntity].tryToShoot = false;
				}
				if (tThisWorld.atClip[nCurrentEntity].tryToReload == true) {
					pcProjectileSystem->Reload(&tThisWorld.atClip[nCurrentEntity]);
					tThisWorld.atClip[nCurrentEntity].tryToReload = false;

					wchar_t* textBuffer = new wchar_t[1];

					textBuffer[0] = (tThisWorld.atClip[nCurrentEntity].nBulletsAvailables.size());

					pcUISystem->UpdateText(&tThisWorld, 1094, &atUIVertices, textBuffer, atUIVertices.at(tThisWorld.atLabel[1094].vIndex));

					pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, 1094);

					delete[] textBuffer;
				}
				if (tThisWorld.atClip[nCurrentEntity].nBulletsFired.size() != 0) {
					for (int i = 0; i < tThisWorld.atClip[nCurrentEntity].nBulletsFired.size(); ++i) {
						if (tThisWorld.atClip[nCurrentEntity].fAliveTime[i] < tThisWorld.atClip[nCurrentEntity].FValueOfCoolDown) {
							pcPhysicsSystem->AddBulletForce(&tThisWorld.atRigidBody[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]], tThisWorld.atClip[nCurrentEntity].bulletSpeed);
							tThisWorld.atClip[nCurrentEntity].fAliveTime[i] += 0.1;

						}
						else {
							tThisWorld.atClip[nCurrentEntity].fAliveTime.erase(tThisWorld.atClip[nCurrentEntity].fAliveTime.begin() + i);
							pcCollisionSystem->RemoveAABBCollider(tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]);
							pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]);
							tThisWorld.atClip[nCurrentEntity].nBulletsFired.erase(tThisWorld.atClip[nCurrentEntity].nBulletsFired.begin() + i);
							for (int i = 0; i < tThisWorld.atClip[nCurrentEntity].fAliveTime.size(); ++i) {
								if (tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]].indexInclip != 0) {
									tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]].indexInclip -= 1;
								}

							}
						}
					}

				}
				if (tThisWorld.atClip[nCurrentEntity].fShootingCoolDown > 0)
					tThisWorld.atClip[nCurrentEntity].fShootingCoolDown -= 1.1;
			}


		}


		if (nCurrentEntity == PlayerStartIndex) {
			float x = 0;
		}
		tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, true);

		if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_RAYGUN)) {
			float CloseEstObject = 10000000000000000000.0f;
			float* distanceCalucaltion = new float();
			for (list<TAABB>::iterator ptr = pcCollisionSystem->m_AAbb.begin(); ptr != pcCollisionSystem->m_AAbb.end(); ++ptr) {

				if (ptr->m_IndexLocation != PlayerStartIndex && ptr->m_IndexLocation != GunIndexForPlayer)
					if (pcCollisionSystem->intersectRayAABox2(XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[0], tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix)
						, XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[1], tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix), *ptr
					) == true) {
						//CloseEstObject = *distanceCalucaltion;
						tThisWorld.atClip[GunIndexForPlayer].currentMaterial = 0;
						cout << "turtle" << '/n';
						tThisWorld.atClip[GunIndexForPlayer].colorofBullets = tThisWorld.atSimpleMesh[ptr->m_IndexLocation].m_nColor;
					}
			}
			delete distanceCalucaltion;
		}


		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
			COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC) || tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
				COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)) {
			tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
		}

		
			if ((tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_TRIGGER) | tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)))
			{
				if (nCurrentEntity == PlayerStartIndex) {
					float x = 0;
				}
				vector<int> otherCollisionsIndex;
				if (pcCollisionSystem->AABBtoAABBCollisionCheck(tThisWorld.atAABB[nCurrentEntity], &otherCollisionsIndex) == true)
				{
					
					for (int i = 0; i < otherCollisionsIndex.size(); ++i) {
						/*if (otherCollisionsIndex[i] == 2 || otherCollisionsIndex[i] == 3) {
							break;
						}*/
						if (tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == true && tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask !=
							(COMPONENT_COLLISIONMASK |
								COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)) {
							if (nCurrentEntity == 8)
							{
								float x = 0;
							}

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
									COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))) {
							tTempVertexBuffer.m_d3dWorldMatrix = pcCollisionSystem->WalkingThrewObjectCheck(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[otherCollisionsIndex[i]], tThisWorld.atAABB[nCurrentEntity]);
							tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tTempVertexBuffer.m_d3dWorldMatrix;
							if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK)) {
								m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
							}

						}
						if (tThisWorld.atRigidBody[nCurrentEntity].ground == false && tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == false && tThisWorld.atRigidBody[nCurrentEntity].wall == false &&
							tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
								COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)
							&& (tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
								COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_STATIC)) | tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
									COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))
						{

							if (tThisWorld.atAIMask[otherCollisionsIndex[i]].m_tnAIMask > 1) {
								if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
									if (tThisWorld.atAiHeath[otherCollisionsIndex[i]].heath <= 0) {
										pcAiSystem->SetNumberOfAI(pcAiSystem->GetNumberOfAI() - 1);
										if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1) {
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
									else {
										tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.erase
										(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.begin()
											+ tThisWorld.atClip[nCurrentEntity].indexInclip);

										pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
										tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.erase(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.begin()
											+ tThisWorld.atClip[nCurrentEntity].indexInclip);


										pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);
										tThisWorld.atAiHeath[otherCollisionsIndex[i]].heath -= 50;
										if (tThisWorld.atAiHeath[otherCollisionsIndex[i]].heath <= 0) {
											pcAiSystem->SetNumberOfAI(pcAiSystem->GetNumberOfAI() - 1);
											pcCollisionSystem->RemoveAABBCollider(otherCollisionsIndex[i]);
											pcGraphicsSystem->CleanD3DObject(&tThisWorld, otherCollisionsIndex[i]);
											pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atAIMask[otherCollisionsIndex[i]].GunIndex);
										}
									}
								}
							}
							else if (tThisWorld.atInputMask[otherCollisionsIndex[i]].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK)) {
								if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
									if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1) {
										tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.erase
										(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.begin()
											+ tThisWorld.atClip[nCurrentEntity].indexInclip);

										pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
										tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.erase(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.begin()
											+ tThisWorld.atClip[nCurrentEntity].indexInclip);

										//pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atAIMask[nCurrentEntity].GunIndex);
										pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);

									}
									tThisWorld.atClayton[otherCollisionsIndex[i]].health -= 50;
								}


							}
							if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
								if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1) {
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

						}
						if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK) && tThisWorld.atAIMask[otherCollisionsIndex[i]].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_COVERTRIGGER)) {

							pcAiSystem->MoveAiToCoverLocation(tThisWorld.atCoverTrigger[otherCollisionsIndex[i]], &tThisWorld);
						}
					}
					if (tThisWorld.atClayton[PlayerStartIndex].health <= 0)
					{
						GameOver = true;
						//return -1;
					}
					//		tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);


				}
				else {

					//tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;

				}
			}
			else {

				//	tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;

			}
		
		tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;

		if (tThisWorld.atParentWorldMatrix[nCurrentEntity] != -1)
		{
			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(pcGraphicsSystem->SetDefaultWorldPosition(),
				tThisWorld.atWorldMatrix[tThisWorld.atParentWorldMatrix[nCurrentEntity]].worldMatrix);

			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(tThisWorld.atOffSetMatrix[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);

			if (nCurrentEntity == rayindex) {
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix;
			}
			if (nCurrentEntity == frustumIndex) {
				XMMATRIX empty;
				empty = pcGraphicsSystem->SetDefaultWorldPosition();
				XMMATRIX rotation;
				XMMATRIX d3dTmpViewM;
				rotation = XMMatrixRotationY(-0.3);
				empty.r[1] = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1];
				empty.r[0] = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0];
				d3dTmpViewM = empty;
				XMVECTOR d3d_newX, d3d_newY, d3d_existingZ;
				d3dTmpViewM = XMMatrixMultiply(rotation, d3dTmpViewM);
				d3d_existingZ = d3dTmpViewM.r[2];
				d3d_newX = XMVector3Cross(empty.r[1], d3d_existingZ);
				d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

				d3d_newX = XMVector3Normalize(d3d_newX);
				d3d_newY = XMVector3Normalize(d3d_newY);

				d3d_existingZ = XMVector3Normalize(d3d_existingZ);

				d3dTmpViewM.r[0] = d3d_newX;
				d3dTmpViewM.r[1] = d3d_newY;
				d3dTmpViewM.r[2] = d3d_existingZ;
				empty = d3dTmpViewM;
				empty.r[3].m128_f32[0] = 3;
				empty.r[3].m128_f32[1] = 0;
				empty.r[3].m128_f32[2] = 1;


				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = empty;
			}
		}

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
		if (tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix.r[3].m128_f32[1] < -10) {
			return 12;
		}

		if (GamePaused)
		{
			if (!options)
			{
				if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON | COMPONENT_PAUSESCREEN))
				{
					tUIVertexBuffer.start = -1;
					tUIVertexBuffer.end = -1;
					tUIVertexBuffer.ratio = -1;

					tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 0);

					if (tThisWorld.atButton[nCurrentEntity].enabled && clickTime > .2)
					{
						if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
						{
							clickTime = 0;

							if (tThisWorld.atButton[nCurrentEntity].sceneIndex == 98)
								options = true;
							else if (tThisWorld.atButton[nCurrentEntity].sceneIndex == nCurrentScene)
								GamePaused = false;
							else if (tThisWorld.atButton[nCurrentEntity].sceneIndex == 96 || tThisWorld.atButton[nCurrentEntity].sceneIndex == 97)
							{

							}
							else
								return tThisWorld.atButton[nCurrentEntity].sceneIndex;

						}
						else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
							tUIPixelBuffer.hoverColor = XMFLOAT4(.6, .6, .6, 0);
						else
							tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 0);
					}

					pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
					pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
				}
			}
			else if (options)
			{
				if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON | COMPONENT_OPTIONS))
				{
					tUIVertexBuffer.start = -1;
					tUIVertexBuffer.end = -1;
					tUIVertexBuffer.ratio = -1;

					tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 0);

					if (tThisWorld.atButton[nCurrentEntity].enabled && clickTime > .2)
					{
						if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
						{
							clickTime = 0;

							if (tThisWorld.atButton[nCurrentEntity].sceneIndex == nCurrentScene)
								options = false;
						}
						else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
						{
							tUIPixelBuffer.hoverColor = XMFLOAT4(.6, .6, .6, 0);
						}
						else
						{
							tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 0);
						}
					}

					pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
					pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

				}

				if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BAR | COMPONENT_OPTIONS))
				{
					if (PtInRect(&tThisWorld.atBar[nCurrentEntity].barBoundingBox, clickPoint) && clickTime > .2)
					{
						// bar manipulation with mouse click try and use for enemy health bar - ZB                   
						tThisWorld.atBar[nCurrentEntity].ratio = (clickPoint.x - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left - 5.0) / (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left - 10);

						//clickTime = 0;
					}

					if (tThisWorld.atBar[nCurrentEntity].backgroundColor.x == 1 &&
						tThisWorld.atBar[nCurrentEntity].backgroundColor.y == 0 &&
						tThisWorld.atBar[nCurrentEntity].backgroundColor.z == 0 &&
						tThisWorld.atBar[nCurrentEntity].backgroundColor.w == 1)
					{
						tUIVertexBuffer.start = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.left + 14 - (screenWidth / 2.0)) / (screenWidth / 2);
						tUIVertexBuffer.end = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right + 4 - (screenWidth / 2.0)) / (screenWidth / 2);
						tUIVertexBuffer.ratio = tThisWorld.atBar[nCurrentEntity].ratio;
					}
					else
					{
						tUIVertexBuffer.start = -1;
						tUIVertexBuffer.end = -1;
						tUIVertexBuffer.ratio = -1;
					}

					tUIPixelBuffer.hoverColor = tThisWorld.atBar[nCurrentEntity].backgroundColor;

					if (tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse != nullptr)
						tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse = nullptr;

					pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
					pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
				}
			}
		}
		else
		{
			if (GameOver)
			{
				if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON | COMPONENT_DEATHSCREEN))
				{
					if (tThisWorld.atClayton[PlayerStartIndex].health <= 0)
					{
						tUIVertexBuffer.start = -1;
						tUIVertexBuffer.end = -1;
						tUIVertexBuffer.ratio = -1;

						tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 0);

						if (tThisWorld.atButton[nCurrentEntity].enabled && clickTime > .2)
						{
							if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
							{
								clickTime = 0;

								if (tThisWorld.atButton[nCurrentEntity].sceneIndex < 96)
									return tThisWorld.atButton[nCurrentEntity].sceneIndex;
							}
							else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
							{
								tUIPixelBuffer.hoverColor = XMFLOAT4(.6, .6, .6, 0);
							}
							else
							{
								tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 0);
							}
						}

						pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
						pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
					}
				}

				if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_DEATHSCREEN))
				{
					if (tThisWorld.atClayton[PlayerStartIndex].health > 0)
					{
						if (tThisWorld.atLabel[nCurrentEntity].addTexture && tThisWorld.atLabel[nCurrentEntity].lastUIElement)
						{
							wchar_t textBuffer[] =
							{ L"SUCCESS" };

							CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2, 1, 0, 4.8, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nCurrentEntity, 0.1);

							pcUISystem->AddTextureToUI(&tThisWorld, nCurrentEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

							pcUISystem->AddMaskToUI(&tThisWorld, nCurrentEntity, COMPONENT_DEATHSCREEN);
							pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, nCurrentEntity);
						}
						else if (tThisWorld.atLabel[nCurrentEntity].addTexture)
						{
							wchar_t textBuffer[] =
							{ L"You Completed The Objective" };

							CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 6, 1, 0, 2.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nCurrentEntity, 0.1);

							pcUISystem->AddTextureToUI(&tThisWorld, nCurrentEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

							pcUISystem->AddMaskToUI(&tThisWorld, nCurrentEntity, COMPONENT_DEATHSCREEN);
							pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, nCurrentEntity);
						}
					}
					else
					{
						if (tThisWorld.atLabel[nCurrentEntity].addTexture && tThisWorld.atLabel[nCurrentEntity].lastUIElement)
						{
							wchar_t textBuffer[] =
							{ L"DEFEAT" };

							CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2, 1, 0, 4.8, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nCurrentEntity, 0.1);

							pcUISystem->AddTextureToUI(&tThisWorld, nCurrentEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

							pcUISystem->AddMaskToUI(&tThisWorld, nCurrentEntity, COMPONENT_DEATHSCREEN);
							pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, nCurrentEntity);
						}
						else if (tThisWorld.atLabel[nCurrentEntity].addTexture)
						{
							wchar_t textBuffer[] =
							{ L"You Failed To Complete The Objective" };

							CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 6, 1, 0, 2.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nCurrentEntity, 0.1);

							pcUISystem->AddTextureToUI(&tThisWorld, nCurrentEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

							pcUISystem->AddMaskToUI(&tThisWorld, nCurrentEntity, COMPONENT_DEATHSCREEN);
							pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, nCurrentEntity);
						}
					}
				}

				if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_DEATHSCREEN))
				{
					tUIVertexBuffer.start = -1;
					tUIVertexBuffer.end = -1;
					tUIVertexBuffer.ratio = -1;

					if (tThisWorld.atLabel[nCurrentEntity].addTexture && !tThisWorld.atLabel[nCurrentEntity].lastUIElement)
						tUIPixelBuffer.hoverColor = XMFLOAT4(1, 0, 0, 0);
					else
						tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 0);

					pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
					pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
				}
			}
		}

		if (GamePaused || GameOver || options)
		{
			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_PAUSESCREEN | COMPONENT_DEATHSCREEN | COMPONENT_OPTIONS))
			{
				tUIVertexBuffer.start = -1;
				tUIVertexBuffer.end = -1;
				tUIVertexBuffer.ratio = -1;

				tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, .2);

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}
		}

		if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_HUD))
		{
			tUIVertexBuffer.start = -1;
			tUIVertexBuffer.end = -1;
			tUIVertexBuffer.ratio = -1;

			if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
				tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 0);
			else
				tUIPixelBuffer.hoverColor = XMFLOAT4(.5, .5, .5, 1);

			pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

		}

		if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BAR | COMPONENT_HUD))
		{
			if (tThisWorld.atBar[nCurrentEntity].backgroundColor.x == 0 &&
				tThisWorld.atBar[nCurrentEntity].backgroundColor.y == 0 && 
				tThisWorld.atBar[nCurrentEntity].backgroundColor.z == 1 && 
				tThisWorld.atBar[nCurrentEntity].backgroundColor.w == 1)
			{
				tUIVertexBuffer.start = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.left + 14 - (screenWidth / 2.0)) / (screenWidth / 2);
				tUIVertexBuffer.end = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right + 4 - (screenWidth / 2.0)) / (screenWidth / 2);
				tUIVertexBuffer.ratio = (100 - tThisWorld.atClip[GunIndexForPlayer].fShootingCoolDown) * .01;
			}
			else if (tThisWorld.atBar[nCurrentEntity].backgroundColor.x == 1 &&
					 tThisWorld.atBar[nCurrentEntity].backgroundColor.y == 0 &&
					 tThisWorld.atBar[nCurrentEntity].backgroundColor.z == 0 &&
					 tThisWorld.atBar[nCurrentEntity].backgroundColor.w == 1)
			{
				tUIVertexBuffer.start = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.left + 14 - (screenWidth / 2.0)) / (screenWidth / 2);
				tUIVertexBuffer.end = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right + 4 - (screenWidth / 2.0)) / (screenWidth / 2);
				tUIVertexBuffer.ratio = tThisWorld.atClayton[PlayerStartIndex].health * .01;
			}
			else
			{
				tUIVertexBuffer.start = -1;
				tUIVertexBuffer.end = -1;
				tUIVertexBuffer.ratio = -1;
			}

			tUIPixelBuffer.hoverColor = tThisWorld.atBar[nCurrentEntity].backgroundColor;

			if (tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse != nullptr)
				tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse = nullptr;

			pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}
	}

	if (pcInputSystem->InputCheck(G_KEY_V))
	{
		GameOver = true;
	}

	if (pcInputSystem->InputCheck(G_KEY_B))
	{
		tThisWorld.atClayton[PlayerStartIndex].health = 0;
		GameOver = true;
	}

	if (GameOver && tThisWorld.atClayton[PlayerStartIndex].health > 0)
	{
		for (auto i = G_KEY_UNKNOWN; i <= G_KEY_9; ++i)
		{
			if (pcInputSystem->InputCheck(i) == 1)
			{
				if (i == G_KEY_V)
					continue;

				return 2;
			}
		}
	}

	if (pcInputSystem->InputCheck(G_KEY_X))
	{
		tThisWorld.atAiHeath[901].heath -= 1;
	}

	if (pcInputSystem->InputCheck(G_KEY_C))
	{
		tThisWorld.atAiHeath[905].heath -= 1;
	}

	if (pcInputSystem->InputCheck(G_KEY_F))
	{
		tThisWorld.atClayton[PlayerStartIndex].health -= 1;
	}

	clickTime += clickTimer.Delta();

	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);
	zValue += 0.001;
	return 14;

}

