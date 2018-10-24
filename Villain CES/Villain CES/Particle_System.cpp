#include "Particle_System.h"



Particle_System::Particle_System()
{
}


Particle_System::~Particle_System()
{
}

int Particle_System::CreateCube15(XMMATRIX LocationofParticle, TWorld * ptWorld)
{
	return ParticleTest(ptWorld, LocationofParticle);
}

void Particle_System::CreateAlotofCubes(XMMATRIX locationofParticles, TWorld * ptWorld, int numberofblood, CGraphicsSystem* pcGraphicsSystem, CAISystem* pcAisystem)
{
	for (int i = 0; i < numberofblood; i++) {
		int cubeindex =ParticleTest(ptWorld, locationofParticles);
		ptWorld->atRigidBody[cubeindex].velocity.m128_f32[0] += pcAisystem->GiveRandomBetweenNegative1and1()*0.2;
		ptWorld->atRigidBody[cubeindex].velocity.m128_f32[1] += pcAisystem->GiveRandomBetweenNegative1and1()*0.2;
		ptWorld->atRigidBody[cubeindex].velocity.m128_f32[2] += -0.4;//pcAisystem->GiveRandomNuberBetweenzeroand1()*1;
		ptWorld->atParticle[cubeindex].AliveTime = 10;

		pcGraphicsSystem->CreateEntityBuffer(ptWorld, cubeindex);
	}
}
