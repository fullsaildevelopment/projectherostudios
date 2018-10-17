#pragma once
#include"Timer.h"

CFPS::CFPS()
{
	m_frameRate = new XTime();
}

CFPS::~CFPS()
{
	delete m_frameRate;
	m_framesPassed = 0;
	m_startTime = 0;
	m_fpsCounter = 0;
}

bool CFPS::Init_FPSReader()
{
	m_Init = true;
	return m_Init;
}

float CFPS::UpdateFrameTime()
{
	double tmp_dTime = m_frameRate->TotalTime();
	if (m_Init == true)
	{
		m_framesPassed = 0;
		m_startTime = tmp_dTime;
		m_Init = false;
		return 0;
	}

	m_framesPassed++;

	if (tmp_dTime - m_startTime > 0.25 && m_framesPassed > 10)
	{
		m_fpsCounter = (double)m_framesPassed / (tmp_dTime - m_startTime);
		m_startTime = tmp_dTime;
		m_framesPassed = 0;
	}
//	std::cout << "FPS:" << m_fpsCounter << endl;
	return m_fpsCounter;
}

void CFPS::Xtime_Signal()
{
	m_frameRate->Signal();
}

double CFPS::GetDelta()
{
	return m_frameRate->SmoothDelta();
}

void CFPS::Restart()
{
	m_frameRate->Restart();
}

void CFPS::Throttle(double targetHz)
{
	m_frameRate->Throttle(targetHz);
}
void CFPS::StableFrameRate()
{
	if (m_fpsCounter >= 60)
	{
		m_frameRate->Throttle(60);
	}
	else if (m_fpsCounter >= 30)
	{
		m_frameRate->Throttle(30);
	}
	else if (m_fpsCounter >= 15)
	{
		m_frameRate->Throttle(15);
	}
	else if (m_fpsCounter >= 7)
	{
		m_frameRate->Throttle(7);
	}
}

double CFPS::GetTotalTime()
{
	return m_frameRate->TotalTimeExact() - m_startTime;
}

