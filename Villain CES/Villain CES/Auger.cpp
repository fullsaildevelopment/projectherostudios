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
//	createCube(&tThisWorld);
	createDebugGrid(&tThisWorld);
	createPlayerBox(&tThisWorld, pcCollisionSystem);
	createPlayerBox(&tThisWorld, pcCollisionSystem);
	
	// do not make things that u want to draw after this line of code or shit will  break;
	//createDebugTransformLines(&tThisWorld);
	pcGraphicsSystem->CreateBuffers(&tThisWorld);
	//createEntity(&tThisWorld);
	 m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	 m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
	 m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();
	 tThisWorld.atWorldMatrix[1].worldMatrix = m_d3dWorldMatrix;
	 for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++) {
		 if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask != 0) {
			 TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[nCurrentEntity].m_VertexData);
			 MyAbb.m_IndexLocation = nCurrentEntity;
			 tThisWorld.atAABB[nCurrentEntity] = MyAbb;
			 pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);
		 }
	 }

}

void CAuger::Update()
{
	//Call some sort of function from the graphics system to create this matrix
	XMMATRIX d3d_ResultMatrix;
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();
	d3d_ResultMatrix = pcInputSystem->DebugCamera(m_d3dViewMatrix, m_d3dWorldMatrix);

	m_d3dViewMatrix = XMMatrixInverse(NULL, d3d_ResultMatrix);
	CGraphicsSystem::TPrimalVertexBufferType tTempVertexBuffer;
	CGraphicsSystem::TPrimalPixelBufferType tTempPixelBuffer;
	tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
	tTempVertexBuffer.m_d3dViewMatrix = m_d3dViewMatrix;
	pcGraphicsSystem->UpdateD3D();
	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		{
			tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dWorldMatrix;
			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(XMMatrixScaling(100, 100, 100), tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
			pcGraphicsSystem->InitPrimalShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity]);

			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID))
		{
			if (nCurrentEntity == 1) 
			{
				// = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix

			
			
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[0]=  pcGraphicsSystem->GetCameraPos().m128_f32[0];
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1] = pcGraphicsSystem->GetCameraPos().m128_f32[1]-1;
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[2] = pcGraphicsSystem->GetCameraPos().m128_f32[2]+2;
				
				
				tThisWorld.atAABB[1] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[1].worldMatrix, tThisWorld.atAABB[1]);
				
			}
			if (nCurrentEntity == 2) 
			{


				tThisWorld.atWorldMatrix[2].worldMatrix = m_d3dWorldMatrix;
				tThisWorld.atWorldMatrix[2].worldMatrix.r[3].m128_f32[1] += 5;
				tThisWorld.atAABB[2] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[2].worldMatrix, tThisWorld.atAABB[2]);
				
			}
			tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;

			pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity]);

			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atSimpleMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

		}
		
		if ((tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_AABB | COMPONENT_NONSTATIC))) 
		{
			if (pcCollisionSystem->AABBtoAABBCollisionCheck(tThisWorld.atAABB[nCurrentEntity]) == true) 
			{
				float x = 0;
				tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			}
		}		
	}
	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);
}

void CAuger::End()
{
	pcGraphicsSystem->CleanD3D(&tThisWorld);
	delete pcGraphicsSystem;
	delete pcInputSystem;
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
