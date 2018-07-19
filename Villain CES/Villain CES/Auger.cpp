#include "stdafx.h"
#include "Auger.h"
#include"Collision_Component.h"


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
	pcGraphicsSystem->InitD3D(cApplicationWindow);
	//createDebugGrid(&tThisWorld);
	CreatePlane(&tThisWorld);
//createCube(&tThisWorld);
	m_bDebugMode = false;

	//createPlayerBox(&tThisWorld, pcCollisionSystem);
	//createPlayerBox(&tThisWorld, pcCollisionSystem);
	//createDummyPlayer(&tThisWorld, m_d3dPlayerMatrix);
	m_d3dPlayerMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	

	createPlayerBox(&tThisWorld);
	createPlayerBox(&tThisWorld);
	//createPlayerBox(&tThisWorld);//3
	//createPlayerBox(&tThisWorld);//4
	//createPlayerBox(&tThisWorld);//5

	
	XMMATRIX  m_dDefaultWorldMa4rix = pcGraphicsSystem->SetDefaultWorldPosition();
	m_dDefaultWorldMa4rix.r[3].m128_f32[1] += 10;
	for (int i = 0; i < m_nClipSize; ++i) {
		nBulletsAvailables.push_back(CreateBullet(&tThisWorld, m_dDefaultWorldMa4rix));	
	}


	nAiminglineIndex = AimingLine(&tThisWorld);

	// do not make things that u want to draw after this line of code or shit will  break;
	//createDebugTransformLines(&tThisWorld);
	pcGraphicsSystem->CreateBuffers(&tThisWorld);
	//createEntity(&tThisWorld);

	 m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	 m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
	 m_d3dCameraMatrix = pcGraphicsSystem->SetDefaultCameraMatrix();
	 m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();
	 tThisWorld.atWorldMatrix[1].worldMatrix = m_d3dWorldMatrix;
	 for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++) {
		 if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask > 1) {
			 if (tThisWorld.atSimpleMesh[nCurrentEntity].m_nVertexCount > tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount) {
				 TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[nCurrentEntity].m_VertexData);
				 MyAbb.m_IndexLocation = nCurrentEntity;
				 tThisWorld.atAABB[nCurrentEntity] = MyAbb;
				 pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);
			 }
			 else {
				 TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData);
				 MyAbb.m_IndexLocation = nCurrentEntity;
				 MyAbb.m_dMaxPointOrginal.y += 1;
				 tThisWorld.atAABB[nCurrentEntity] = MyAbb;
				 pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);
			 }
		 }
	 }
	
	 tThisWorld.atWorldMatrix[0].worldMatrix = m_d3dWorldMatrix;
	 tThisWorld.atWorldMatrix[0].worldMatrix.r[3].m128_f32[1] -= 2;

	 tThisWorld.atWorldMatrix[2].worldMatrix = m_d3dWorldMatrix;
	 tThisWorld.atWorldMatrix[3].worldMatrix = m_d3dWorldMatrix;
	 tThisWorld.atWorldMatrix[4].worldMatrix = m_d3dWorldMatrix;
	 tThisWorld.atWorldMatrix[5].worldMatrix = m_d3dWorldMatrix;
	 tThisWorld.atWorldMatrix[2].worldMatrix = m_d3dWorldMatrix;

	 tThisWorld.atWorldMatrix[2].worldMatrix.r[3].m128_f32[1] += 5;
	 tThisWorld.atWorldMatrix[3].worldMatrix.r[3].m128_f32[1] += 1;
	 tThisWorld.atWorldMatrix[4].worldMatrix.r[3].m128_f32[1] += -5;
	 tThisWorld.atWorldMatrix[5].worldMatrix.r[3].m128_f32[1] += -5;
	 tThisWorld.atWorldMatrix[5].worldMatrix.r[3].m128_f32[0] += -5;

	 XMVECTOR playerGravity = pcPhysicsSystem->ZeroVector();
	 playerGravity.m128_f32[1] =-0.000001;
	 tThisWorld.atRigidBody[1].gravity = playerGravity;


}

void CAuger::Update()
{

	//Call some sort of function from the graphics system to create this matrix
	if (pcInputSystem->InputCheck(G_KEY_9) == 1)
	{
		m_bDebugMode = !m_bDebugMode;
	}
	static XMMATRIX d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();;
	//m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();
	//createDummyPlayer(&tThisWorld, m_d3dPlayerMatrix);
	//d3d_ResultMatrix = 
	if (m_bDebugMode == false)
	{
		//XMVECTOR testW = m_d3dViewMatrix.r[3];
		//m_d3dViewMatrix = pcGraphicsSystem->SetPlayerViewMatrix(m_d3dViewMatrix, m_d3dPlayerMatrix);
		//m_d3dViewMatrix.r[3] = testW;

		//d3d_ResultMatrix = XMMatrixInverse(NULL, m_d3dViewMatrix);

		d3d_ResultMatrix = pcInputSystem->WalkLookAt(d3d_ResultMatrix.r[3], m_d3dPlayerMatrix.r[3], XMVectorSet(0, 1.0f, 0, 0), d3d_ResultMatrix);

		//m_d3dViewMatrix = XMMatrixInverse(NULL, d3d_ResultMatrix);
	}
	else
	{
		d3d_ResultMatrix = pcInputSystem->DebugCamera(m_d3dCameraMatrix, m_d3dWorldMatrix);
		m_d3dViewMatrix = XMMatrixInverse(NULL, d3d_ResultMatrix);
	}
	m_d3dCameraMatrix = XMMatrixMultiply(d3d_ResultMatrix, m_d3dPlayerMatrix);

	
	

	CGraphicsSystem::TPrimalVertexBufferType tTempVertexBuffer;
	CGraphicsSystem::TPrimalPixelBufferType tTempPixelBuffer;
	tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

	

	pcGraphicsSystem->UpdateD3D();
	fShootingCoolDown -= 0.1;
	if (pcInputSystem->InputCheck(G_KEY_CAPSLOCK)==1&&fShootingCoolDown<=0) {
		if(nBulletsAvailables.size() != 0) {

			fShootingCoolDown = 100;
			int indextoBullet = *nBulletsAvailables.begin();
			nBulletsAvailables.pop_front();
			nBulletsFired.push_back(indextoBullet);
			fAliveTime.push_back(0);
			TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[indextoBullet].m_VertexData);
			MyAbb.m_IndexLocation = indextoBullet;
			tThisWorld.atAABB[indextoBullet] = MyAbb;
			pcCollisionSystem->AddAABBCollider(MyAbb, indextoBullet);
			tThisWorld.atCollisionMask[indextoBullet].m_tnCollisionMask = COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC;
			tThisWorld.atGraphicsMask[indextoBullet].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID;
			tThisWorld.atAIMask[indextoBullet].m_tnAIMask = COMPONENT_AIMASK;
			tThisWorld.atUIMask[indextoBullet].m_tnUIMask = COMPONENT_UIMASK;
			tThisWorld.atPhysicsMask[indextoBullet].m_tnPhysicsMask = COMPONENT_PHYSICSMASK | COMPONENT_RIGIDBODY;
			XMMATRIX bulletSpawnLocation = tThisWorld.atWorldMatrix[1].worldMatrix;
			pcGraphicsSystem->CreateBuffers(&tThisWorld);

	/*		tThisWorld.atCollisionMask[indextoBullet].m_tnCollisionMask = COMPONENT_AABB | COMPONENT_NONSTATIC;
			tThisWorld.atGraphicsMask[indextoBullet].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID;
			tThisWorld.atAIMask[indextoBullet].m_tnAIMask = COMPONENT_AIMASK;
			tThisWorld.atUIMask[indextoBullet].m_tnUIMask = COMPONENT_UIMASK;
			tThisWorld.atPhysicsMask[indextoBullet].m_tnPhysicsMask = COMPONENT_PHYSICSMASK | COMPONENT_RIGIDBODY;*/
			/*	DefaultPerspectiveMatrix.r[0].m128_f32[0] = 1;
	DefaultPerspectiveMatrix.r[0].m128_f32[1] = 0;
	DefaultPerspectiveMatrix.r[0].m128_f32[2] = 0;
	DefaultPerspectiveMatrix.r[0].m128_f32[3] = 0;

	DefaultPerspectiveMatrix.r[1].m128_f32[0] = 0;	
	DefaultPerspectiveMatrix.r[1].m128_f32[1] = 1;
	DefaultPerspectiveMatrix.r[1].m128_f32[2] = 0;
	DefaultPerspectiveMatrix.r[1].m128_f32[3] = 0;

	DefaultPerspectiveMatrix.r[2].m128_f32[0] = 0;
	DefaultPerspectiveMatrix.r[2].m128_f32[1] = 0;
	DefaultPerspectiveMatrix.r[2].m128_f32[2] = 1;
	DefaultPerspectiveMatrix.r[2].m128_f32[3] = 0;*/
		

			bulletSpawnLocation.r[3].m128_f32[2] += 1;
			bulletSpawnLocation.r[3].m128_f32[0] += -0.1;

			tThisWorld.atWorldMatrix[indextoBullet].worldMatrix = bulletSpawnLocation;
			
			float x = 0;

		}
	}
	tThisWorld.atWorldMatrix[nAiminglineIndex] = tThisWorld.atWorldMatrix[1];
	tThisWorld.atWorldMatrix[nAiminglineIndex].worldMatrix.r[3].m128_f32[2] += 1.5;
	tThisWorld.atWorldMatrix[nAiminglineIndex].worldMatrix.r[3].m128_f32[0] -= 0.3;

	//for (list<int>::iterator ptr = nBulletsFired.begin(); ptr != nBulletsFired.end(); ++ptr) {
	//	XMVECTOR localpos;
	//	localpos.m128_f32[0] = 0;
	//	localpos.m128_f32[1] = 0;
	//	localpos.m128_f32[2] = 0.0001;

	//	//XMMATRIX localMatrix=XMMatrixTranslationFromVector(localpos);
	//	//tThisWorld.atWorldMatrix[*ptr].worldMatrix = XMMatrixMultiply(localMatrix, tThisWorld.atWorldMatrix[*ptr].worldMatrix);
	//	tThisWorld.atRigidBody[*ptr].totalForce += localpos;
	//	tThisWorld.atWorldMatrix[*ptr].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[*ptr], tThisWorld.atWorldMatrix[*ptr].worldMatrix);
	//	
	//}
	m_nIndexToBullets = 0;
	m_nIndexToBullets2 = 0;
	if (pcInputSystem->InputCheck(G_KEY_R) == 1) {
		for (int i = 0; i < nDeadBullets.size(); ++i) {
			if (nBulletsAvailables.size() < m_nClipSize) {
				nBulletsAvailables.push_back(nDeadBullets[i]);
				nDeadBullets.erase(nDeadBullets.begin() + i);
			}
			else {
				break;
			}
		}
	}
	
	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tTempVertexBuffer.m_d3dViewMatrix = m_d3dViewMatrix;

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		{
			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dWorldMatrix;
			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(XMMatrixScaling(100, 100, 100), tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
			pcGraphicsSystem->InitPrimalShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], m_d3dCameraMatrix);

			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

		}
		if(tThisWorld.atPhysicsMask[nCurrentEntity].m_tnPhysicsMask== (COMPONENT_PHYSICSMASK | COMPONENT_RIGIDBODY)) {
			if (nBulletsFired.size()> m_nIndexToBullets &&nBulletsFired[m_nIndexToBullets] == nCurrentEntity) {
				fAliveTime[m_nIndexToBullets] += 0.1;
				if (fAliveTime[m_nIndexToBullets] > 100) {
					fAliveTime.erase(fAliveTime.begin() + m_nIndexToBullets);
					nDeadBullets.push_back(nBulletsFired[m_nIndexToBullets]);
					tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask = COMPONENT_AABB;
					tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK;
					tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask = COMPONENT_AIMASK;
					tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask = COMPONENT_UIMASK;
					tThisWorld.atPhysicsMask[nCurrentEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK;
					tThisWorld.atRigidBody[nCurrentEntity].velocity = pcPhysicsSystem->ZeroVector();
					tThisWorld.atRigidBody[nCurrentEntity].totalForce = pcPhysicsSystem->ZeroVector();
					pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
					nBulletsFired.erase(nBulletsFired.begin() + m_nIndexToBullets);
					pcGraphicsSystem->CreateBuffers(&tThisWorld);

				}
				pcPhysicsSystem->AddBulletForce(&tThisWorld.atRigidBody[nCurrentEntity]);
				m_nIndexToBullets++;
			}
			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
		}
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID))
		{
			if (nCurrentEntity == 1) 
			{
				// = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
				if (m_bDebugMode == false)
				{
					m_d3dPlayerMatrix = pcInputSystem->TrackCamera(m_d3dPlayerMatrix);

					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
					m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
				}
			
				
				//tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[0] = m_d3dPlayerMatrix.r[3].m128_f32[0];
				//tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1] = m_d3dPlayerMatrix.r[3].m128_f32[1];
				//tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[2] = m_d3dPlayerMatrix.r[3].m128_f32[2];
				/*tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[0]=  pcGraphicsSystem->GetCameraPos().m128_f32[0];
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1] = pcGraphicsSystem->GetCameraPos().m128_f32[1] - 2;
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[2] = pcGraphicsSystem->GetCameraPos().m128_f32[2] + 2;*/
			//	tThisWorld.atAABB[1] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[1].worldMatrix, tThisWorld.atAABB[1]);
			
			}
		
			tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
			tTempVertexBuffer.m_d3dViewMatrix = m_d3dViewMatrix;
			if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask != 0)
				tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
			}
			
				if ((tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC)))
				{
					vector<int> otherCollisionsIndex;
					if (pcCollisionSystem->AABBtoAABBCollisionCheck(tThisWorld.atAABB[nCurrentEntity], &otherCollisionsIndex) == true)
					{
						for (int i = 0; i < otherCollisionsIndex.size(); ++i) {
							if (tThisWorld.atRigidBody[otherCollisionsIndex[i]].NeatrlizeForce == true) {
								float x = 0;
								tThisWorld.atRigidBody[nCurrentEntity].totalForce =-tThisWorld.atRigidBody[nCurrentEntity].velocity;
								tTempVertexBuffer.m_d3dWorldMatrix =pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
							}
						}
					/*
						if (nBulletsFired.size() > m_nIndexToBullets2 &&nBulletsFired[m_nIndexToBullets2] == nCurrentEntity) {
							fAliveTime.erase(fAliveTime.begin() + m_nIndexToBullets2);
							nDeadBullets.push_back(nBulletsFired[m_nIndexToBullets2]);
							tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask = COMPONENT_AABB;
							tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask = COMPONENT_GRAPHICSMASK;
							tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask = COMPONENT_AIMASK;
							tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask = COMPONENT_UIMASK;
							tThisWorld.atPhysicsMask[nCurrentEntity].m_tnPhysicsMask = COMPONENT_PHYSICSMASK;
							tThisWorld.atRigidBody[nCurrentEntity].velocity = pcPhysicsSystem->ZeroVector();
							tThisWorld.atRigidBody[nCurrentEntity].totalForce = pcPhysicsSystem->ZeroVector();
							pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
							nBulletsFired.erase(nBulletsFired.begin() + m_nIndexToBullets2);
							pcGraphicsSystem->CreateBuffers(&tThisWorld);
							m_nIndexToBullets2++;
						}*/
						tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
					

					}
					else {

						tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;

					}
				}
			
				pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity], m_d3dCameraMatrix);

				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atSimpleMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			
		}
		pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);

		
	}


void CAuger::End()
{
	pcGraphicsSystem->CleanD3D(&tThisWorld);
	delete pcGraphicsSystem;
	delete pcInputSystem;
	delete pcCollisionSystem;
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
