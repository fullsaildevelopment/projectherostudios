#pragma once
#include"XTime.h"
#include"InputSystem.h"
#include<string>
struct System_Times
{
	XTime tAppTimer;
	XTime tSceneTimer;
	float globalTime;
	float localTime;
	System_Times()
	{
		globalTime = 0;
		localTime = 0;
	 }
	float GetGlobalTime(XTime &timer,float &time)
	{
		timer.Signal();
		time = timer.TotalTime();

		return time;
	}

	float GetLocalTime(XTime &timer, float &time)
	{
		timer.Signal();
		time += timer.SmoothDelta();

		return time;
	}

	void StartClock(XTime &timer)
	{
		timer.Signal();
	}

	void DisplayTimes(CInputSystem *mySystem)
	{		

		float convertedTime;
		std::string stTimeDisplay;

		convertedTime = globalTime;
		stTimeDisplay = "AppTime: ";
		stTimeDisplay += std::to_string(convertedTime);
		cout << stTimeDisplay << endl;

		convertedTime = localTime;
		stTimeDisplay = "Scene Time: ";
		stTimeDisplay += std::to_string(convertedTime);
		cout << stTimeDisplay << endl;
		
	}
};