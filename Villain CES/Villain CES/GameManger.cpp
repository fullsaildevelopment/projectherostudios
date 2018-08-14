#include "GameManger.h"
#define AI_ON false
CGameMangerSystem::CGameMangerSystem(HWND window,CInputSystem* _pcInputSystem)
{
	cApplicationWindow = window;
	pcGraphicsSystem = new CGraphicsSystem();
	pcInputSystem = _pcInputSystem;
	pcCollisionSystem = new CCollisionSystem();
	pcPhysicsSystem = new CPhysicsSystem();
	pcProjectileSystem = new CProjectileSystem();
	pcAiSystem = new CAISystem();
	pcGraphicsSystem->InitD3D(cApplicationWindow);
	srand(time(NULL));
}

CGameMangerSystem::~CGameMangerSystem()
{
	pcGraphicsSystem->CleanD3D(&tThisWorld);
	delete pcGraphicsSystem;
	delete pcInputSystem;
	delete pcCollisionSystem;
	delete pcPhysicsSystem;
	delete pcProjectileSystem;
	delete pcAiSystem;
}

void CGameMangerSystem::LoadLevel()
{
	// master
	
	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);
	
	pcAiSystem->SetNumberOfAI(2);
	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
	m_d3dCameraMatrix = pcGraphicsSystem->SetDefaultCameraMatrix();
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();

	

	tCameraMode.bDebugMode = false;
	tCameraMode.bAimMode = false;
	tCameraMode.bWalkMode = true;
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

	GunIndexForPlayer = CreateGun(&tThisWorld, m_d3dWorldMatrix, PlayerStartIndex, -1, 1, 10.5,3,100);
	tThisWorld.atClip[GunIndexForPlayer].bulletSpeed = 0.001;
	AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[0] += -1;
	AILocation.r[3].m128_f32[2] += -2;
	int AiIndex=CreateSimpleGunAi(&tThisWorld, AILocation);
	int GunINdexai=CreateGun(&tThisWorld, m_d3dWorldMatrix, AiIndex, -1.1, 0, 11,10,100);
	tThisWorld.atAIMask[AiIndex].GunIndex = GunINdexai;

	tThisWorld.atClip[GunINdexai].bulletSpeed = 0.0001;//Frame Dependent


	AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[0] += -3;
	AILocation.r[3].m128_f32[2] += -5;
	int SimpleAi2 = CreateSimpleSearchAi(&tThisWorld, AILocation);
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
	//CreateSimpleGunAi(&tThisWorld, AILocation);
	XMMATRIX groundSpawnPoint;
	groundSpawnPoint = m_d3dWorldMatrix;
	groundSpawnPoint.r[3].m128_f32[1] -= 2;
	CreateGround(&tThisWorld, groundSpawnPoint);
	tempImport = pcGraphicsSystem->ReadMesh("meshData_Example_Objects.txt");

	for (int meshIndex = 1; meshIndex < tempImport.meshCount; meshIndex++)
	{
		int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	}

	tempImport = pcGraphicsSystem->ReadMesh("meshData_ScifiRoom.txt");

	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	}
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


	tThisWorld.atWorldMatrix[frustumIndex].worldMatrix= AILocation;
	
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



	if (pcCollisionSystem->m_AAbb.size() !=0 ) {
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
	tCameraMode = pcInputSystem->CameraModeListen(tCameraMode);
	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
	m_d3dCameraMatrix = pcGraphicsSystem->SetDefaultCameraMatrix();
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();

	 XMMATRIX m_d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	 XMMATRIX m_d3dOffsetMatrix = pcGraphicsSystem->SetDefaultOffset();
	if (pcInputSystem->InputCheck(G_KEY_P)) 
	{
		return 3;

	}
	/*if (pcInputSystem->InputCheck(G_KEY_U)) {
	
	}*/
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
		m_d3d_ResultMatrix = pcInputSystem->WalkLookAt(XMVectorSet(0, 1.0f, 0, 0), m_d3d_ResultMatrix);

		m_d3dCameraMatrix = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);

		m_d3dCameraMatrix = XMMatrixMultiply(m_d3dOffsetMatrix, m_d3dCameraMatrix);
	}
	else if (tCameraMode.bAimMode == true)
	{

		m_d3dPlayerMatrix = pcInputSystem->AimMode(m_d3dPlayerMatrix);

		m_d3dCameraMatrix = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);

		m_d3dCameraMatrix = XMMatrixMultiply(m_d3dOffsetMatrix, m_d3dCameraMatrix);
	}
	else
	{

		m_d3d_ResultMatrix = pcInputSystem->DebugCamera(m_d3d_ResultMatrix, m_d3dWorldMatrix);

		m_d3dCameraMatrix = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dWorldMatrix);
		//m_d3d_ResultMatrix = XMMatrixInverse(NULL, m_d3d_ResultMatrix);
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
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tTempVertexBuffer.m_d3dViewMatrix = m_d3dViewMatrix;

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID)) 
		{
			if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK))
			{
				if (tCameraMode.bWalkMode == true)
				{
					m_d3dPlayerMatrix = pcInputSystem->WalkCamera(m_d3dPlayerMatrix);
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
				if (tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1] < -10) 
				{
					return -1;
				}
			}

			tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
			tMyVertexBufferTemp.m_d3dViewMatrix = m_d3dViewMatrix;
			pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], m_d3dCameraMatrix);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		{

			pcGraphicsSystem->InitPrimalShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], m_d3dCameraMatrix);

			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

		}
		// ai code would run here
		if (pcAiSystem->GetNumberOfAI() <= 0) 
		{
			return -1;
		}
		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_FOLLOW) || tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_SHOOT | COMPONENT_AIMASK | COMPONENT_FOLLOW))
		{
#if AI_ON				
			pcAiSystem->FollowObject(tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
			pcAiSystem->ShootGun(&tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex]);
#endif // AI_ON
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
			vector<int> indexes;
			if (pcCollisionSystem->AiVisionCheck(tThisWorld.atAIVision[nCurrentEntity].eyes0,&indexes) == true) {
				float x = 0;
			
			tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			}
			else {
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
			//else if(tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask==())
		
		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_PATHFINDTEST)) {
			if (tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding == true) {
				
				pcAiSystem->FindBestPath(tThisWorld.atPathPlanining[nCurrentEntity].startingNode,
					tThisWorld.atPathPlanining[nCurrentEntity].Goal, 
					&tThisWorld.atPathPlanining[nCurrentEntity].directions);
				
				tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding = false;
			}
			else {
				pcAiSystem->PathPlaningMovement(&tThisWorld.atPathPlanining[nCurrentEntity], &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
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
				if (tThisWorld.atClip[nCurrentEntity].fShootingCoolDown>0)
					tThisWorld.atClip[nCurrentEntity].fShootingCoolDown -= 0.1;
			}
			

	}
		
		
		if (nCurrentEntity == PlayerStartIndex) {
			float x = 0;
		}
			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, true);
			if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK)){
			m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		}
			if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_RAYGUN)) {
				float CloseEstObject = 10000000000000000000.0f;
				float* distanceCalucaltion = new float();
				for (list<TAABB>::iterator ptr = pcCollisionSystem->m_AAbb.begin(); ptr != pcCollisionSystem->m_AAbb.end(); ++ptr) {

					if(ptr->m_IndexLocation!=PlayerStartIndex)
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
						

			//	tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);

			tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
			tTempVertexBuffer.m_d3dViewMatrix = m_d3dViewMatrix;

			pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity], m_d3dCameraMatrix);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atSimpleMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}
	}

	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);
	zValue += 0.001;
	return 2;
}

void CGameMangerSystem::RestartLevel()
{
}

int CGameMangerSystem::LoadMainMenu()
{
	bool switchlevel = false;


	 XMMATRIX m_d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	 XMMATRIX m_d3dOffsetMatrix = pcGraphicsSystem->SetDefaultOffset();
	if (pcInputSystem->InputCheck(G_KEY_P)) {
		return 3;

	}
	if (pcInputSystem->InputCheck(G_KEY_ESCAPE)) {
		//pcCollisionSystem->m_AAbb.clear();
		pcGraphicsSystem->CleanD3DLevel(&tThisWorld);

	//	pcGraphicsSystem->CleanD3D(&tThisWorld);
		return 4;
	}
	/*if (pcInputSystem->InputCheck(G_KEY_U)) {

	}*/
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


	

	
		m_d3dPlayerMatrix = pcInputSystem->AimMode(m_d3dPlayerMatrix);

		m_d3dCameraMatrix = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);

		m_d3dCameraMatrix = XMMatrixMultiply(m_d3dOffsetMatrix, m_d3dCameraMatrix);
		// m_d3dCameraMatrix = XMMatrixMultiply(m_d3dOffsetMatrix, m_d3dCameraMatrix);

	CGraphicsSystem::TPrimalVertexBufferType tTempVertexBuffer;
	CGraphicsSystem::TPrimalPixelBufferType tTempPixelBuffer;
	tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);



	pcGraphicsSystem->UpdateD3D();

	// togle the modes that you are in




	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{

		tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tTempVertexBuffer.m_d3dViewMatrix = m_d3dViewMatrix;

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		{

			pcGraphicsSystem->InitPrimalShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], m_d3dCameraMatrix);

			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

		}
		// ai code would run here

	
		//else if(tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask==())


		


		tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, true);


	


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
					else if (tThisWorld.atRigidBody[nCurrentEntity].ground == false && tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == false && tThisWorld.atRigidBody[nCurrentEntity].wall == false &&
						tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)
						&& (tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_STATIC)) || tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
								COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))
					{
						if (tThisWorld.atAABB[otherCollisionsIndex[i]].m_SceneChange != -30) {
							switchlevel = true;
							break;
						}
						/*if (tThisWorld.atAIMask[otherCollisionsIndex[i]].m_tnAIMask == COMPONENT_AIMASK | COMPONENT_FOLLOW) {
							pcCollisionSystem->RemoveAABBCollider(otherCollisionsIndex[i]);
							pcGraphicsSystem->CleanD3DObject(&tThisWorld, otherCollisionsIndex[i]);
						}*/
					}
				}

				tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);


			}
			else {

				tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;

			}
		}
		else {

			tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;

		}


		tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;


		

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID))
		{

			if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK))
			{

				
					m_d3dPlayerMatrix = pcInputSystem->WalkCamera(m_d3dPlayerMatrix);

					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
					m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;

				

			
			}


			//	tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);

			tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
			tTempVertexBuffer.m_d3dViewMatrix = m_d3dViewMatrix;

			pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity], m_d3dCameraMatrix);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atSimpleMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

		}






	}

	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);
	if (pcInputSystem->InputCheck(G_KEY_ENTER) == 1|| switchlevel==true&&pcInputSystem->InputCheck(G_KEY_ENTER)==0) {
		pcGraphicsSystem->CleanD3DLevel(&tThisWorld);
		pcCollisionSystem->m_AAbb.clear();

		return 1;
	}
	else return 0;
}

void CGameMangerSystem::InitilizeMainMenu()
{
	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);

	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
	m_d3dCameraMatrix = pcGraphicsSystem->SetDefaultCameraMatrix();
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();
	tCameraMode.bDebugMode = false;
	tCameraMode.bAimMode = false;
	tCameraMode.bWalkMode = true;
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
	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++) {
		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask > 1) {
			if (tThisWorld.atSimpleMesh[nCurrentEntity].m_nVertexCount > tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount) {
				TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[nCurrentEntity].m_VertexData,tThisWorld.atAABB[nCurrentEntity]);
				MyAbb.m_IndexLocation = nCurrentEntity;
				tThisWorld.atAABB[nCurrentEntity] = MyAbb;
				pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);
			}
			else {
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

void CGameMangerSystem::LoadPathFindingTest()
{
	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);

	pcAiSystem->SetNumberOfAI(2);
	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
	m_d3dCameraMatrix = pcGraphicsSystem->SetDefaultCameraMatrix();
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();



	tCameraMode.bDebugMode = false;
	tCameraMode.bAimMode = false;
	tCameraMode.bWalkMode = true;
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
		int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	}

	tempImport = pcGraphicsSystem->ReadMesh("meshData_ScifiRoom.txt");

	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	}

	AILocation.r[3].m128_f32[0] -= 7;
	int nodeLocation = CreateNodePoint(&tThisWorld, AILocation);
	tThisWorld.atSimpleMesh[nodeLocation].m_nColor= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT3 nodePosition;
	nodePosition.x = AILocation.r[3].m128_f32[0];
	nodePosition.y = AILocation.r[3].m128_f32[1];
	nodePosition.z = AILocation.r[3].m128_f32[2];

	pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	XMMATRIX part2ofNodeMovement = AILocation;
	part2ofNodeMovement.r[3].m128_f32[2] -= -10;
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
	tThisWorld.atSimpleMesh[aiPathIndex].m_nColor= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	int nodelocation3 = CreateNodePoint(&tThisWorld, pathfindAI);
	tThisWorld.atPathPlanining[aiPathIndex].startingNode = nodelocation3;
	tThisWorld.atPathPlanining[aiPathIndex].Goal = nodeLocation;
	pcAiSystem->AddNodeToPathFinding(nodelocation3, nodePosition, 1);
	vector<int> edges;
	edges.push_back(nodelocation4);
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



	if (pcCollisionSystem->m_AAbb.size() != 0) {
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
	tCameraMode = pcInputSystem->CameraModeListen(tCameraMode);
	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
	m_d3dCameraMatrix = pcGraphicsSystem->SetDefaultCameraMatrix();
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();

	XMMATRIX m_d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	XMMATRIX m_d3dOffsetMatrix = pcGraphicsSystem->SetDefaultOffset();

	/*if (pcInputSystem->InputCheck(G_KEY_U)) {

	}*/
	// ui stuff
	

	if (tCameraMode.bWalkMode == true)
	{
		m_d3d_ResultMatrix = pcInputSystem->WalkLookAt(XMVectorSet(0, 1.0f, 0, 0), m_d3d_ResultMatrix);

		m_d3dCameraMatrix = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);

		m_d3dCameraMatrix = XMMatrixMultiply(m_d3dOffsetMatrix, m_d3dCameraMatrix);
	}
	else if (tCameraMode.bAimMode == true)
	{

		m_d3dPlayerMatrix = pcInputSystem->AimMode(m_d3dPlayerMatrix);

		m_d3dCameraMatrix = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);

		m_d3dCameraMatrix = XMMatrixMultiply(m_d3dOffsetMatrix, m_d3dCameraMatrix);
	}
	else
	{

		m_d3d_ResultMatrix = pcInputSystem->DebugCamera(m_d3d_ResultMatrix, m_d3dWorldMatrix);

		m_d3dCameraMatrix = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dWorldMatrix);
		//m_d3d_ResultMatrix = XMMatrixInverse(NULL, m_d3d_ResultMatrix);
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
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tTempVertexBuffer.m_d3dViewMatrix = m_d3dViewMatrix;

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK))
			{
				if (tCameraMode.bWalkMode == true)
				{
					m_d3dPlayerMatrix = pcInputSystem->WalkCamera(m_d3dPlayerMatrix);
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
				if (tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1] < -10)
				{
					return -1;
				}
			}

			tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
			tMyVertexBufferTemp.m_d3dViewMatrix = m_d3dViewMatrix;
			pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], m_d3dCameraMatrix);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		{

			pcGraphicsSystem->InitPrimalShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], m_d3dCameraMatrix);

			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

		}
		// ai code would run here
	
		

		

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

		
		//else if(tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask==())

		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_PATHFINDTEST)) {
			if (tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding == true) {

				pcAiSystem->FindBestPath(tThisWorld.atPathPlanining[nCurrentEntity].startingNode,
					tThisWorld.atPathPlanining[nCurrentEntity].Goal,
					&tThisWorld.atPathPlanining[nCurrentEntity].directions);

				tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding = false;
			}
			else {
				pcAiSystem->PathPlaningMovement(&tThisWorld.atPathPlanining[nCurrentEntity], &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
			}

		}
	

		if (nCurrentEntity == PlayerStartIndex) {
			float x = 0;
		}
		tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, true);
		if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK)) {
			m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
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

			
		}


		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID))
		{


			//	tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);

			tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
			tTempVertexBuffer.m_d3dViewMatrix = m_d3dViewMatrix;

			pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity], m_d3dCameraMatrix);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atSimpleMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}
	}

	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);
	zValue += 0.001;
	return 6;
}

void CGameMangerSystem::FirstSkeltonAiTestLoad()
{
	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);

	pcAiSystem->SetNumberOfAI(2);
	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
	m_d3dCameraMatrix = pcGraphicsSystem->SetDefaultCameraMatrix();
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();



	tCameraMode.bDebugMode = false;
	tCameraMode.bAimMode = false;
	tCameraMode.bWalkMode = true;
	m_d3dPlayerMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	//m_d3dPlayerMatrix = XMMatrixMultiply(m_d3dPlayerMatrix, XMMatrixScaling(.01, .01, .01));

	m_d3dPlayerMatrix.r[3].m128_f32[2] -= 10;
	ImporterData tempImport;
	tempImport = pcGraphicsSystem->ReadMesh("meshData_Pirate.txt");

	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		PlayerStartIndex = createClayton(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);

	}

	AimingLine(&tThisWorld, m_d3dWorldMatrix, PlayerStartIndex, -1, 1, 10.5);

	GunIndexForPlayer = CreateGun(&tThisWorld, m_d3dWorldMatrix, PlayerStartIndex, -1, 1, 10.5, 3, 100);
	tThisWorld.atClip[GunIndexForPlayer].bulletSpeed = 0.001;
	//tThisWorld.atClip[GunIndexForPlayer].

	tThisWorld.atClayton[PlayerStartIndex].health = 100;
	XMMATRIX wall = m_d3dWorldMatrix;
	wall.r[3].m128_f32[1] += -1;

	CreateWall(&tThisWorld, wall);
	wall.r[3].m128_f32[0] -= 10;
	wall.r[3].m128_f32[1] -= 1;
	wall.r[3].m128_f32[2] -= 1;

	int door1Index = CreateDoorWay(&tThisWorld, wall);
	wall.r[3].m128_f32[0] -= 22;
	int door2Index = CreateDoorWay(&tThisWorld, wall);

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
	AILocation.r[3].m128_f32[0] += -13;
	AILocation.r[3].m128_f32[2] -= -15;
	XMMATRIX CoverLocation = AILocation;
	AILocation = XMMatrixLookAtLH(AILocation.r[3],
		m_d3dPlayerMatrix.r[3], XMVectorSet(0, 1, 0, 0));
	AILocation = XMMatrixInverse(NULL, AILocation);

	CoverLocation.r[3].m128_f32[2] += -1;
	CreateCover(&tThisWorld, CoverLocation);
	CoverLocation.r[3].m128_f32[0] += 5;
	CreateCover(&tThisWorld, CoverLocation);
	XMMATRIX nodeLocation = CoverLocation;
	nodeLocation.r[3].m128_f32[0] -= 1;
	//pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	int nodeindex = CreateNodePoint(&tThisWorld, nodeLocation);
	XMFLOAT3 nodePosition;
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex, nodePosition, 1);

	int spacePirate = CreateSpacePirate(&tThisWorld, AILocation);
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
	CreateGround(&tThisWorld, groundSpawnPoint);
	groundSpawnPoint.r[3].m128_f32[2] -= -20;
	CreateGround(&tThisWorld, groundSpawnPoint);

	tempImport = pcGraphicsSystem->ReadMesh("meshData_Example_Objects.txt");

	for (int meshIndex = 1; meshIndex < tempImport.meshCount; meshIndex++)
	{
		int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	}

	tempImport = pcGraphicsSystem->ReadMesh("meshData_ScifiRoom.txt");

	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
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
				TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[nCurrentEntity].m_VertexData, tThisWorld.atAABB[nCurrentEntity]);
				MyAbb.m_IndexLocation = nCurrentEntity;
				tThisWorld.atAABB[nCurrentEntity] = MyAbb;
				pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);
				if (nCurrentEntity == door1Index || nCurrentEntity == door2Index) {
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


		for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
		{
			if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask > 1)
			{
				tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
			}
		}


		pcGraphicsSystem->CreateBuffers(&tThisWorld);
	}
}

int CGameMangerSystem::SpacePirateGamePlay()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(-1); //Important!
	tCameraMode = pcInputSystem->CameraModeListen(tCameraMode);
	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
	m_d3dCameraMatrix = pcGraphicsSystem->SetDefaultCameraMatrix();
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();

	XMMATRIX m_d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	XMMATRIX m_d3dOffsetMatrix = pcGraphicsSystem->SetDefaultOffset();
	if (pcInputSystem->InputCheck(G_KEY_P))
	{
		return 3;

	}
	/*if (pcInputSystem->InputCheck(G_KEY_U)) {

	}*/
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
		m_d3d_ResultMatrix = pcInputSystem->WalkLookAt(XMVectorSet(0, 1.0f, 0, 0), m_d3d_ResultMatrix);

		m_d3dCameraMatrix = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);

		m_d3dCameraMatrix = XMMatrixMultiply(m_d3dOffsetMatrix, m_d3dCameraMatrix);
	}
	else if (tCameraMode.bAimMode == true)
	{

		m_d3dPlayerMatrix = pcInputSystem->AimMode(m_d3dPlayerMatrix);

		m_d3dCameraMatrix = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);

		m_d3dCameraMatrix = XMMatrixMultiply(m_d3dOffsetMatrix, m_d3dCameraMatrix);
	}
	else
	{

		m_d3d_ResultMatrix = pcInputSystem->DebugCamera(m_d3d_ResultMatrix, m_d3dWorldMatrix);

		m_d3dCameraMatrix = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dWorldMatrix);
		//m_d3d_ResultMatrix = XMMatrixInverse(NULL, m_d3d_ResultMatrix);
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
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tTempVertexBuffer.m_d3dViewMatrix = m_d3dViewMatrix;

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK))
			{
				if (tCameraMode.bWalkMode == true)
				{
					m_d3dPlayerMatrix = pcInputSystem->WalkCamera(m_d3dPlayerMatrix);
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
				if (tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1] < -10)
				{
					return -1;
				}
			}

			tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
			tMyVertexBufferTemp.m_d3dViewMatrix = m_d3dViewMatrix;
			pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], m_d3dCameraMatrix);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		{

			pcGraphicsSystem->InitPrimalShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], m_d3dCameraMatrix);

			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

		}
		// ai code would run here
		if (pcAiSystem->GetNumberOfAI() <= 0)
		{
			return -1;
		}
		
		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_FOLLOW) || tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_SHOOT | COMPONENT_AIMASK | COMPONENT_FOLLOW))
		{
#if AI_ON				
			if (tThisWorld.atActiveAI[nCurrentEntity].active == true) {
				pcAiSystem->FollowObject(tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
				pcAiSystem->ShootGun(&tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex]);
			}
#endif // AI_ON
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
							danger = true;
							tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].tryToShoot = true;
							
						}
						else if (tThisWorld.atProjectiles[indicies[i]].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
							danger = true;
							tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

						}
						else if(danger==false) {	
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

		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_PATHFINDTEST)) {
			if (tThisWorld.atActiveAI[nCurrentEntity].active == true) {
				if (tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding == true) {

					pcAiSystem->FindBestPath(tThisWorld.atPathPlanining[nCurrentEntity].startingNode,
						tThisWorld.atPathPlanining[nCurrentEntity].Goal,
						&tThisWorld.atPathPlanining[nCurrentEntity].directions);

					tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding = false;
				}
				else {
					pcAiSystem->PathPlaningMovement(&tThisWorld.atPathPlanining[nCurrentEntity], &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
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
					pcCollisionSystem->AddAiVisioNCheck(tThisWorld.atAABB[newbullet], newbullet);
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
				if (tThisWorld.atClip[nCurrentEntity].fShootingCoolDown>0)
					tThisWorld.atClip[nCurrentEntity].fShootingCoolDown -= 0.1;
			}


		}


		if (nCurrentEntity == PlayerStartIndex) {
			float x = 0;
		}
		tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, true);
		if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK)) {
			m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
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
							}
							tThisWorld.atClayton[otherCollisionsIndex[i]].health -= 50;
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


			//	tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);

			tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
			tTempVertexBuffer.m_d3dViewMatrix = m_d3dViewMatrix;

			pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity], m_d3dCameraMatrix);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atSimpleMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}
	}

	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);
	zValue += 0.001;
	return 8;
}
