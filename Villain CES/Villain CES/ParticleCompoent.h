#pragma once
#include"stdafx.h"

enum eParticleComponent
{
	COMPONENT_Particles = 1 << 0,
	COMPONENT_Active = 1 << 1,

};
struct TParticleMask
{
	int m_tnParticleMask = 0;
	
};
struct Particle {
	XMFLOAT4 color;
	float AliveTime;

};
