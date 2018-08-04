#include "stdafx.h"
#include "Auger.h"
#include"Collision_Component.h"
#include <stdint.h>


CAuger::CAuger()
{

}

CAuger::CAuger(HWND window)
{
	pcInputSystem = new CInputSystem();
	pcGameMangerSystem = new CGameMangerSystem(window,pcInputSystem);

		pcInputSystem->InitializeGInput(window);
		pcGraphicsSystem = new CGraphicsSystem();
	/*cApplicationWindow = window;
	
	pcGraphicsSystem = new CGraphicsSystem();
	pcInputSystem = new CInputSystem();
	pcCollisionSystem = new CCollisionSystem();
	pcPhysicsSystem = new CPhysicsSystem();
	pcInputSystem->InitializeGInput(window);
	pcProjectileSystem = new CProjectileSystem();
	pcAiSystem = new CAISystem();
	srand(time(NULL));*/
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
	//pcGameMangerSystem->LoadLevel();
	CurrentSpotInGame = -1;
}

void CAuger::Update(bool* loopgame)
{
	switch (CurrentSpotInGame)
	{
	case -1:
		pcGameMangerSystem->InitilizeMainMenu();
		CurrentSpotInGame = 0;
		break;
		
		// main menu
	case 0:
		CurrentSpotInGame=pcGameMangerSystem->LoadMainMenu();

		break;
	case 1:
		pcGameMangerSystem->LoadLevel();
		CurrentSpotInGame = 2;
		break;
	case 2:
		CurrentSpotInGame =pcGameMangerSystem->InGameUpdate();
	case 3:
		if(pcInputSystem->InputCheck(G_KEY_U))
		CurrentSpotInGame = 2;
	case 4:
		*loopgame = false;
	default:
		break;
	}
	
	
	
	
	
	
}


void CAuger::End()
{
	delete pcGameMangerSystem;
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
