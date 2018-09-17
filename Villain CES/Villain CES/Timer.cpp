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

void CFPS::UpdateFrameTime()
{
	double tmp_dTime = m_frameRate->TotalTime();
	if (m_Init == true)
	{
		m_framesPassed = 0;
		m_startTime = tmp_dTime;
		m_Init = false;
		return;
	}

	m_framesPassed++;

	if (tmp_dTime - m_startTime > 0.25 && m_framesPassed > 10)
	{
		m_fpsCounter = (double)m_framesPassed / (tmp_dTime - m_startTime);
		m_startTime = tmp_dTime;
		m_framesPassed = 0;
	}
	std::cout << "FPS:" << m_fpsCounter << endl;
}

void CFPS::Xtime_Signal()
{
	m_frameRate->Signal();
}

