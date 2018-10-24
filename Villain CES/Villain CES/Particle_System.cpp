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

void Particle_System::CreateColorFulHealing(XMMATRIX locationofParticles, TWorld * ptWorld, int numberofblood, CGraphicsSystem * pcGraphicsSystem, CAISystem * pcAisystem)
{
	for (int i = 0; i < numberofblood; i++) {
		int cubeindex = ParticleTest(ptWorld, locationofParticles);
		ptWorld->atRigidBody[cubeindex].gravity = DirectX::XMVectorZero();
	//	ptWorld->atRigidBody[cubeindex].velocity.m128_f32[0] += pcAisystem->GiveRandomBetweenNegative1and1()*0.2;
		ptWorld->atRigidBody[cubeindex].velocity.m128_f32[1] += pcAisystem->GiveRandomNuberBetweenzeroand1()*0.07;
	//	ptWorld->atRigidBody[cubeindex].velocity.m128_f32[2] += -0.4;//pcAisystem->GiveRandomNuberBetweenzeroand1()*1;
		ptWorld->atWorldMatrix[cubeindex].worldMatrix.r[3].m128_f32[0]+= pcAisystem->GiveRandomBetweenNegative1and1()*0.2;
		ptWorld->atWorldMatrix[cubeindex].worldMatrix.r[3].m128_f32[2] += pcAisystem->GiveRandomBetweenNegative1and1()*0.2;
		ptWorld->atWorldMatrix[cubeindex].worldMatrix.r[3].m128_f32[1] += pcAisystem->GiveRandomBetweenNegative1and1()*0.2;

		ptWorld->atParticleMask[cubeindex].m_tnParticleMask = (COMPONENT_Particles | COMPONENT_Active | COMPONENT_ChangeColor);
		ptWorld->atParticle[cubeindex].AliveTime = 100;

		pcGraphicsSystem->CreateEntityBuffer(ptWorld, cubeindex);
	}
}

XMFLOAT4 Particle_System::ChangeColor(CAISystem * pcAisystem)
{
	XMFLOAT4 red;
	red.x = 1;
	red.y = 0;
	red.z = 0;
	red.w = 1;

	XMFLOAT4 blue;
	blue.x = 0;
	blue.y = 1;
	blue.z = 0;
	blue.w = 1;
	int randomNumber = pcAisystem->GiveRandomNuberBetweenzeroand1();
	switch (randomNumber)
	{
	case 0 :
		return red;
	default:
		return blue;
		break;
	}


	
}
