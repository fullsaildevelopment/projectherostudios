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
	CurrentSpotInGame = 0;
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
	switch (CurrentSpotInGame)
	{ 
	case -1:
		*loopgame = false;
		break;
	case 0:
		pcGameMangerSystem->InitializeTitleScreen();
		CurrentSpotInGame = 1;
		break;

	case 1:
		CurrentSpotInGame = pcGameMangerSystem->LoadTitleScreen();
		break;
	case 2:
		pcGameMangerSystem->InitializeMainMenu();
		CurrentSpotInGame = 3;

	case 3:
		CurrentSpotInGame = pcGameMangerSystem->LoadMainMenu();
		
		break;
	case 4:
		pcGameMangerSystem->LoadLevel();

		CurrentSpotInGame = 5;
		break;
	case 5 :
		CurrentSpotInGame = pcGameMangerSystem->InGameUpdate();
		if (!pcGameMangerSystem->GamePaused && !pcGameMangerSystem->GameOver)
		{
			GetWindowRect(window2, &rect);
			//SetCursorPos((rect.right / 2.0f) + 20, (rect.bottom / 2.0f) + 64);
			// sets mouse cursor to middle of screen ZB
			SetCursorPos(((rect.right - rect.left) / 2.0f) + rect.left, ((rect.bottom - rect.top) / 2.0f) + rect.top);

		}

		break;
	case 6 :
		CurrentSpotInGame=pcGameMangerSystem->PathFindingExample();

		GetWindowRect(window2, &rect);
		SetCursorPos((rect.right / 2.0f) + 20, (rect.bottom / 2.0f) + 64);
		break;
	case 7 :
		  pcGameMangerSystem->FirstSkeltonAiTestLoad();
		  CurrentSpotInGame = 8;
		  break;
	case 8 :
		CurrentSpotInGame = pcGameMangerSystem->SpacePirateGamePlay();
	
		GetWindowRect(window2, &rect);
		SetCursorPos((rect.right / 2.0) + 20, (rect.bottom / 2.0) + 65);
			GetWindowRect(window2, &rect);
			SetCursorPos((rect.right / 2.0)-40, (rect.bottom / 2.0) + rect.top);
		break;
	case 9:
		pcGameMangerSystem->LoadMikesGraphicsSandbox();
		CurrentSpotInGame = 10;
		break;
	case 10:
		CurrentSpotInGame = pcGameMangerSystem->MikesGraphicsSandbox();
		break;
	case 13: 
		pcGameMangerSystem->LoadLevelWithMapInIt();
		CurrentSpotInGame = 14;
		break;
	case 14:
		CurrentSpotInGame = pcGameMangerSystem->RealLevelUpdate();
		if (!pcGameMangerSystem->GamePaused && !pcGameMangerSystem->GameOver)
		{
			GetWindowRect(window2, &rect);
			//SetCursorPos((rect.right / 2.0f) + 20, (rect.bottom / 2.0f) + 64);
			// sets mouse cursor to middle of screen ZB
			SetCursorPos(((rect.right - rect.left) / 2.0f) + rect.left, ((rect.bottom - rect.top) / 2.0f) + rect.top);

		}

		break;
	case 15:
		pcGameMangerSystem->LoadPathFindingTest();
		CurrentSpotInGame = 6;
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