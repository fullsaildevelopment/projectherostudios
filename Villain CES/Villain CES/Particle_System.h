#pragma once
#include"stdafx.h"
#include "Entity.h"
#include"Graphics_System.h"
#include "AI_System.h"
#include"ParticleCompoent.h"
class Particle_System
{
public:

	Particle_System();
	~Particle_System();
	int CreateCube15(XMMATRIX LocationofParticle, TWorld * ptWorld);
	void CreateAlotofCubes(XMMATRIX locationofParticles, TWorld * ptWorld, int numberofblood, CGraphicsSystem* pcGraphicsSystem, CAISystem* pcAisystem,float delta,bool color);
	void CreateColorFulHealing(XMMATRIX locationofParticles, TWorld * ptWorld, int numberofblood, CGraphicsSystem* pcGraphicsSystem, CAISystem* pcAisystem,float delta);
	XMFLOAT4 ChangeColor(CAISystem * pcAisystem);
};

