#pragma once
#include"XTime.h"
#include"InputSystem.h"
#include<string>
struct System_Times
{
	float applicationTime;
	float sceneTime;
	System_Times()
	{
		applicationTime = 0;
		sceneTime = 0;
	 }
	float GetTime(XTime &timer,float time)
	{
		timer.Signal();
		time = timer.TotalTime();

		return time;
	}

	void StartClock(XTime &timer)
	{
		timer.Signal();
	}

	void DisplayTimes(System_Times* myTimes, CInputSystem *mySystem)
	{
		if (mySystem->InputCheck(G_KEY_T) == 1)
		{
			float convertedTime;
			std::string stTimeDisplay;

			convertedTime = myTimes->applicationTime / 60.0f;
			stTimeDisplay = "AppTime: ";
			stTimeDisplay += std::to_string(convertedTime);
			cout << stTimeDisplay <<  endl;

			convertedTime = myTimes->sceneTime / 60.0f;
			stTimeDisplay = "Scene Time: ";
			stTimeDisplay += std::to_string(convertedTime);
			cout << stTimeDisplay <<  endl;
		}
	}
};

struct Timers 
{
	XTime tAppTimer;
	XTime tSceneTimer;
};