#include "stdafx.h"
#include "Auger.h"
#include"Collision_Component.h"
#include <stdint.h>


CAuger::CAuger()
{

}

CAuger::CAuger(HWND window)
{
	cApplicationWindow = window;
	pcGraphicsSystem = new CGraphicsSystem();
	pcInputSystem = new CInputSystem();
	pcCollisionSystem = new CCollisionSystem();
	pcPhysicsSystem = new CPhysicsSystem();
	pcInputSystem->InitializeGInput(window);
	pcProjectileSystem = new CProjectileSystem();
	pcAiSystem = new CAISystem();
	srand(time(NULL));
}


CAuger::~CAuger()
{

}

void CAuger::Start()
{
	//Initializes Systems
	InitializeSystems();
}

void CAuger::InitializeSystems()
{
	for (int i = 0; i < ENTITYCOUNT; ++i) {
		tThisWorld.atParentWorldMatrix[i] = -1;
	}
	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
	m_d3dCameraMatrix = pcGraphicsSystem->SetDefaultCameraMatrix();
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();

	pcGraphicsSystem->InitD3D(cApplicationWindow);
	
	tCameraMode.bDebugMode = false;
	tCameraMode.bAimMode = false;
	tCameraMode.bWalkMode = true;
	m_d3dPlayerMatrix = pcGraphicsSystem->SetDefaultWorldPosition();



	PlayerStartIndex = CreateClayTon(&tThisWorld);
	XMMATRIX wall = m_d3dWorldMatrix;
	wall.r[3].m128_f32[1] += -1;

	CreateWall(&tThisWorld,wall);
	wall = m_d3dWorldMatrix;
	wall.r[3].m128_f32[1] += -1;
	wall.r[3].m128_f32[0] += -22;
	CreateWall(&tThisWorld,wall);
	XMMATRIX celling = m_d3dWorldMatrix;
	celling.r[3].m128_f32[1] += 10;
	CreateCelling(&tThisWorld,celling);
	XMMATRIX AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[2] += -5;
	AILocation.r[3].m128_f32[0] += -5;
	CreateSimpleGunAi(&tThisWorld,AILocation);


	AimingLine(&tThisWorld, m_d3dWorldMatrix, PlayerStartIndex, -1, 0, 10.5);

	GunIndexForPlayer =CreateGunForPlayer(&tThisWorld, m_d3dWorldMatrix, PlayerStartIndex, -1, 0, 10.5);
	AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[0] += -1;
	AILocation.r[3].m128_f32[2] += -2;
	CreateSimpleGunAi(&tThisWorld,AILocation);
	AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[0] += -3;
	AILocation.r[3].m128_f32[2] += -5;
	CreateSimpleGunAi(&tThisWorld,AILocation);
	XMMATRIX groundSpawnPoint;
	groundSpawnPoint = m_d3dWorldMatrix;
	groundSpawnPoint.r[3].m128_f32[1] -= 2;
	CreateGround(&tThisWorld, groundSpawnPoint);
	
	 XMFLOAT4 blue;
	 blue.y = 0;
	 blue.z = 1;
	 blue.w = 1;
	 blue.x = 0;
	 tThisWorld.atSimpleMesh[8].m_nColor = blue;
	 for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++) {
		 if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask > 1) {
			 if (tThisWorld.atSimpleMesh[nCurrentEntity].m_nVertexCount > tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount) {
				 TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[nCurrentEntity].m_VertexData);
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

void CAuger::Update()
{

	
	    tCameraMode = pcInputSystem->CameraModeListen(tCameraMode);


	static XMMATRIX m_d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	static XMMATRIX m_d3dOffsetMatrix  = pcGraphicsSystem ->SetDefaultOffset();
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
		// m_d3dCameraMatrix = XMMatrixMultiply(m_d3dOffsetMatrix, m_d3dCameraMatrix);
	}
	else
	{

		m_d3d_ResultMatrix = pcInputSystem->DebugCamera(m_d3d_ResultMatrix, m_d3dWorldMatrix);


		m_d3dCameraMatrix = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dWorldMatrix);
		//m_d3d_ResultMatrix = XMMatrixInverse(NULL, m_d3d_ResultMatrix);
	}
	
	

	
	

	CGraphicsSystem::TPrimalVertexBufferType tTempVertexBuffer;
	CGraphicsSystem::TPrimalPixelBufferType tTempPixelBuffer;
	tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

	

	pcGraphicsSystem->UpdateD3D();
	// togle the modes that you are in
	if (pcInputSystem->InputCheck(G_BUTTON_MIDDLE)) {
		tThisWorld.atClip[GunIndexForPlayer].GunMode = !tThisWorld.atClip[GunIndexForPlayer].GunMode;
	}
	// shoot a bullet
	if (pcInputSystem->InputCheck(G_BUTTON_LEFT)==1&&tThisWorld.atClip[GunIndexForPlayer].GunMode==true) {

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
		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_FOLLOW )) {
		
			// ai code do not delete
		/*	tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix=XMMatrixLookAtLH(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3],
			tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix.r[3], XMVectorSet(0, 1, 0, 0));
			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixInverse(NULL, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
			XMVECTOR direction = tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix.r[3] - tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3];
			direction.m128_f32[0] = 0;
			direction.m128_f32[1] = 0;
			direction.m128_f32[2] = 1;
			direction.m128_f32[3] = 0;
			direction =XMVector3Normalize(direction);
			direction *= 0.001f;
			XMMATRIX localMatrix2 = XMMatrixTranslationFromVector(direction);

			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix=XMMatrixMultiply(localMatrix2, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
		*/
			pcAiSystem->FollowObject(tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);

		}
		if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_CLIP)) {
			if (tThisWorld.atClip[nCurrentEntity].GunMode == false&& tThisWorld.atClip[nCurrentEntity].tryToShoot == true) {
			
				if (tThisWorld.atClip[nCurrentEntity].maderay == false) {
					XMMATRIX gun = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
					gun.r[3].m128_f32[0] += 1;
					rayindex = CreateRayBullet(&tThisWorld, gun, 10,PlayerStartIndex,-0.6,-0.1,10.7);
					pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, rayindex);
					tThisWorld.atClip[nCurrentEntity].maderay = true;
				}
				
			}
			else if (tThisWorld.atClip[nCurrentEntity].GunMode==false && tThisWorld.atClip[nCurrentEntity].tryToShoot == false&& tThisWorld.atClip[nCurrentEntity].maderay==true) {
				pcGraphicsSystem->CleanD3DObject(&tThisWorld, rayindex);
				rayindex = -1;
				tThisWorld.atClip[nCurrentEntity].maderay = false;

			}
			else {
				if (tThisWorld.atClip[nCurrentEntity].tryToShoot == true && tThisWorld.atClip[nCurrentEntity].nBulletsAvailables.size() > 0 && tThisWorld.atClip[nCurrentEntity].fShootingCoolDown <= 0)
				{

					
					int newbullet = CreateBullet(&tThisWorld, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, 
					tThisWorld.atClip[nCurrentEntity].currentMaterial);
					tThisWorld.atSimpleMesh[newbullet].m_nColor = tThisWorld.atClip[nCurrentEntity].colorofBullets;
					pcProjectileSystem->CreateBulletProjectile(newbullet, &tThisWorld.atClip[nCurrentEntity]);
					tThisWorld.atAABB[newbullet] = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[newbullet].m_VertexData);
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
						if (tThisWorld.atClip[nCurrentEntity].fAliveTime[i] < 100) {
							pcPhysicsSystem->AddBulletForce(&tThisWorld.atRigidBody[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]]);
							tThisWorld.atClip[nCurrentEntity].fAliveTime[i] += 0.1;
							
						}
						else {
							tThisWorld.atClip[nCurrentEntity].fAliveTime.erase(tThisWorld.atClip[nCurrentEntity].fAliveTime.begin() + i);
							pcCollisionSystem->RemoveAABBCollider(tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]);
							pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]);
							tThisWorld.atClip[nCurrentEntity].nBulletsFired.erase(tThisWorld.atClip[nCurrentEntity].nBulletsFired.begin() + i);
						}
					}
					tThisWorld.atClip[nCurrentEntity].fShootingCoolDown -= 0.1;
				}
			}
			
		}
			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, true);
			if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_RAYGUN)) {
				float CloseEstObject = 10000000000000000000.0f;
				float* distanceCalucaltion = new float();
				for (list<TAABB>::iterator ptr = pcCollisionSystem->m_AAbb.begin(); ptr != pcCollisionSystem->m_AAbb.end(); ++ptr) {
					
					if (pcCollisionSystem->IsLineInBox(XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[0], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix)
						, XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[1], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix),
						tThisWorld.atWorldMatrix[ptr->m_IndexLocation].worldMatrix, *ptr,distanceCalucaltion) == true&&*distanceCalucaltion<CloseEstObject) {
						CloseEstObject = *distanceCalucaltion;
						tThisWorld.atClip[GunIndexForPlayer].currentMaterial = ptr->m_MatrtialIndex;
						
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
						if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask==(COMPONENT_CLAYTON| COMPONENT_INPUTMASK)) {
							m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
						}

					}
					else if (tThisWorld.atRigidBody[nCurrentEntity].ground == false && tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == false && tThisWorld.atRigidBody[nCurrentEntity].wall == false &&
						tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)
						&& (tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_STATIC)) | tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
								COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC) && otherCollisionsIndex[i] != 1)
					{
						/*tThisWorld.atWorldMatrix[5].worldMatrix.r[3].m128_f32[0] += rand() % 10-5;
						if (tThisWorld.atWorldMatrix[5].worldMatrix.r[3].m128_f32[0] < -10 | tThisWorld.atWorldMatrix[5].worldMatrix.r[3].m128_f32[0]>7) {
						tThisWorld.atWorldMatrix[5].worldMatrix = m_d3dWorldMatrix;
						tThisWorld.atWorldMatrix[5].worldMatrix.r[3].m128_f32[2] += -5;
						tThisWorld.atWorldMatrix[5].worldMatrix.r[3].m128_f32[0] += -5;
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
		if (tThisWorld.atParentWorldMatrix[nCurrentEntity] != -1)
		{
			
			if(nCurrentEntity == 8) {
				float x = 0;
			}
			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(pcGraphicsSystem->SetDefaultWorldPosition(),
				tThisWorld.atWorldMatrix[tThisWorld.atParentWorldMatrix[nCurrentEntity]].worldMatrix);

			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(tThisWorld.atOffSetMatrix[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
			
		}
		
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID))
		{
			
			if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK))
			{
				
				if (tCameraMode.bWalkMode == true)
				{
					m_d3dPlayerMatrix = pcInputSystem->WalkCamera(m_d3dPlayerMatrix);

					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix,false);
					m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;

				}
			
				else if (tCameraMode.bAimMode == true)
				{
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix,false);
					m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
				}
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
		zValue += 0.001;

		
	}


void CAuger::End()
{
	pcGraphicsSystem->CleanD3D(&tThisWorld);
	delete pcGraphicsSystem;
	delete pcInputSystem;
	delete pcCollisionSystem;
	delete pcPhysicsSystem;
	delete pcProjectileSystem;
	delete pcAiSystem;
}

/*
Notes from Lari about multithreading systems:
Every context has its own set of constant buffers
gonna get some problems if you try to share them
Each thread can have its own data set
One Thread handles the environment
what parts to show
does deformation
One thread handle characters
animation
what they are doing
etc.
Nothing characters doing influences terrain and vice versa
Potential issue with shadows
Everything has a job and a responsibility
Make sure work completes and usable at proper times
Input before physics
some things can be frame locked, and some can be frame unlcoked
frame locked - character update needs to be before rendering character
will use a snapshot of input
frame unlocked - input(tends to be event based), solving A* on one thread, networking
can still recieve input simultaneously as other stuff is happening
An event system that is a queue of results other threads are waiting for
if not ready, use data from last frame
don't lock in too tightly with bindings
Interpolate between last three frames for networking
a spline/hermite curve
Gateware has a math library that does this

Understand sequence what work will be used in
physics produces a set of matrices renderer uses
common way to get around is to cache
input processing output
input - data
process
output - data

seperate processing from output
physics system
produce matrix locations from locations
after physics frames is run
save matrix positions
as much time as you need in the renderer
while physics system keeps running

Want things moving simultaneously

buffering system for an output chain

UML/Diagram things

What is the input for this system, what is the output for this system, what other systems consume this output

If these run on different threads diagram that as well (different color lines?)

some systems don't need buffering chain
if very cheap and low processing cost don't buffer just wait

start small make a couple systems

have a goal but leave room to adapt

have a standard
how to write code
readable
kept seperate
respect the diagram

be careful about making connections between systems
duplicate if necessary
make a second array if necessary
often the best solution when multithreading to get rid of sync points

mutex as few times as possible

great in principle, hard to execute, good goals to have though

Some things that might seem inefficient are not
When two threads are trying to read and write to the same array of data at different indicies,
double the size of the array so
each thread can work on the same array without compromising dependent data
recombine the modified data from both sides of the array into one array for output

the worst thing you can do is have multiple threads accessing a block of memory that is accessed frequently at high speed

be aware of all these things and use them as necessary

certain systems will be faster or slower

Use VS Profiler to see where slow downs are in code

*/
