#include "Auger.h"

CAuger::CAuger()
{
}

CAuger::CAuger(HWND window)
{
	pcInputSystem = new CInputSystem();
	pcInputSystem->InitializeGInput(window);
	pcGameMangerSystem = new CGameMangerSystem(window, pcInputSystem);
	window2 = window;
	pcProjectileSystem = new CProjectileSystem();
}

CAuger::~CAuger()
{
	//delete pcInputSystem;
	delete pcGameMangerSystem;
}

void CAuger::Start()
{
	//Initializes Systems
	
	
	InitializeSystems();
	
}

void CAuger::InitializeSystems()
{
	//pcGameMangerSystem->LoadLevel();
	pcGameMangerSystem->nCurrentScene = 0;
	//pcGraphicsSystem->InitD3D(cApplicationWindow);
	
	//createDebugGrid(&tThisWorld);
	

	

	// do not make things that u want to draw after this line of code or shit will  break;
	//createDebugTransformLines(&tThisWorld);
	
	//createEntity(&tThisWorld);

	/* The memember world Matrix, camera's Postion Matrix, and camera Matrix Are set the inited to the same world starting position on purpose*/

	 //m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	 //tMyCamera.d3d_Position = pcGraphicsSystem->SetDefaultViewMatrix();
  //   //m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();
	 //m_d3dCameraMatrix = pcGraphicsSystem->SetDefaultCameraMatrix();
	 //m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective(90.f);

	// tThisWorld.atWorldMatrix[1].worldMatrix = m_d3dWorldMatrix;
	 // create the AABB
	
	
	

	 
}

void CAuger::Update(bool* loopgame)
{
	

	//Call some sort of function from the graphics system to create this matrix
	 //   tCameraMode = pcInputSystem->CameraModeListen(tCameraMode);
		
//	UpdateWindow(window2);
	//createDummyPlayer(&tThisWorld, m_d3dPlayerMatrix);
	//d3d_ResultMatrix = 
	RECT rect = { 0 };
	switch (pcGameMangerSystem->nCurrentScene)
	{ 
	case -1:
		*loopgame = false;
		break;
	case 0:
		pcGameMangerSystem->InitializeTitleScreen();
		pcGameMangerSystem->nCurrentScene = 1;
		break;

	case 1:
		pcGameMangerSystem->nCurrentScene = pcGameMangerSystem->LoadTitleScreen();
		break;
	case 2:
		pcGameMangerSystem->InitializeMainMenu();
		pcGameMangerSystem->nCurrentScene = 3;

	case 3:
		pcGameMangerSystem->nCurrentScene = pcGameMangerSystem->LoadMainMenu();
		
		break;
	case 4:
		//pcGameMangerSystem->LoadLevel();
		pcGameMangerSystem->nCurrentScene = 5;
		break;
	case 5 :
		if (!pcGameMangerSystem->GamePaused && !pcGameMangerSystem->GameOver)
		{
			GetWindowRect(window2, &rect);
			//SetCursorPos((rect.right / 2.0f) + 20, (rect.bottom / 2.0f) + 64);
			// sets mouse cursor to middle of screen ZB
			SetCursorPos(((rect.right - rect.left) / 2.0f) + rect.left, ((rect.bottom - rect.top) / 2.0f) + rect.top);

		}

		break;
	case 6 :
		pcGameMangerSystem->nCurrentScene =pcGameMangerSystem->PathFindingExample();

		GetWindowRect(window2, &rect);
		SetCursorPos((rect.right / 2.0f) + 20, (rect.bottom / 2.0f) + 64);
		break;
	case 7 :
		  pcGameMangerSystem->FirstSkeltonAiTestLoad();
		  pcGameMangerSystem->nCurrentScene = 8;
		  break;
	case 8 :
		pcGameMangerSystem->nCurrentScene = pcGameMangerSystem->SpacePirateGamePlay();
	
		GetWindowRect(window2, &rect);
		SetCursorPos((rect.right / 2.0) + 20, (rect.bottom / 2.0) + 65);
			GetWindowRect(window2, &rect);
			SetCursorPos((rect.right / 2.0)-40, (rect.bottom / 2.0) + rect.top);
		break;
	case 9:
		pcGameMangerSystem->LoadMikesGraphicsSandbox();
		pcGameMangerSystem->nCurrentScene = 10;
		break;
	case 10:
		pcGameMangerSystem->nCurrentScene = pcGameMangerSystem->MikesGraphicsSandbox();
		break;
	case 11:
		pcGameMangerSystem->InitializeStory();

		pcGameMangerSystem->nCurrentScene = 12;
		break;
	case 12:
		pcGameMangerSystem->nCurrentScene = pcGameMangerSystem->LoadStory();
		break;
	case 13: 
		if (!pcGameMangerSystem->loading)
		{
			pcGameMangerSystem->LoadLevelWithMapInIt();
		}
		else
		{
			pcGameMangerSystem->nCurrentScene = pcGameMangerSystem->LoadLoadingScreen(true);

			if (pcGameMangerSystem->nCurrentScene == 14)
			{
				pcGameMangerSystem->CleanLoadingScreen();

				while (ShowCursor(false) > -1)
				{
					continue;
				}
			}
		}

		//pcGameMangerSystem->nCurrentScene = 14;
		break;
	case 14:
		if (!pcGameMangerSystem->GamePaused && !pcGameMangerSystem->GameOver && GetFocus())
		{
			GetWindowRect(window2, &rect);
			//SetCursorPos((rect.right / 2.0f) + 20, (rect.bottom / 2.0f) + 64);
			// sets mouse cursor to middle of screen ZB
			SetCursorPos(((rect.right - rect.left) / 2.0f) + rect.left, ((rect.bottom - rect.top) / 2.0f) + rect.top);
		}
		pcGameMangerSystem->nCurrentScene = pcGameMangerSystem->RealLevelUpdate();
		break;
	case 15:
		pcGameMangerSystem->LoadPathFindingTest();
		pcGameMangerSystem->nCurrentScene = 6;
		break;
	default:
		break;
	}
}
	

void CAuger::End()
{
	//delete pcInputSystem;
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