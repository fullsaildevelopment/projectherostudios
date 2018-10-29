#pragma once
#include"Collision_Component.h"
#include"Particle_System.h"
#include <list>
#include"stdafx.h"
#include"Entity.h"
#include"Graphics_System.h"
#include"Physics_System.h"
#include"AudioSystem.h"
#include"AI_System.h"
#include <thread>
#include<mutex>


using namespace std;
//File header :
/***
* Filename:          Collision_System.h
* Date:              07/11/2018
* Mod. Date:         07/23/2018
* Mod. Initials:    AP
* Author:            Arthur E. Parker
* Purpose:           Controls Collision in Game
*            
**/
class CCollisionSystem
{
	
public:

	CCollisionSystem();
	~CCollisionSystem();
	int AddAABBCollider(TAABB m_AABB,int nIndex);
	int AddAiVisioNCheck(TAABB m_AABB, int nIndex);
	bool RemoveAABBCollider(int nIndex);
	bool AABBtoAABBCollisionCheck(TAABB m_AABB, vector<int>* m_OtherColision);
	bool replaceAABB(int nIndex, TAABB m_AABB2);
	TAABB updateAABB(XMMATRIX worldMatrix, TAABB aabb);
	TAABB createAABBS(std::vector<XMFLOAT3> verticies,TAABB AABBDATA);
	bool classify_aabb_to_aabb(TAABB aabb1, TAABB aabb2);
	bool IsLineInBox(XMVECTOR startPoint, XMVECTOR endPoint, XMMATRIX worldMatrix, TAABB boxclide,float* distance);
	bool intersectRayAABox2(XMVECTOR startPoint, XMVECTOR endPoint, TAABB boxclide);
	XMMATRIX WalkingThrewObjectCheck(XMMATRIX worldPos,TAABB otherCollision,TAABB currentCollision);
	list<TAABB> m_AAbb;
	vector<TAABB> AiFrustumCheck;
	bool aabb_to_frustum(TAABB& aabb, frustum_t& frustum);
	bool AiVisionCheck(frustum_t eyeSight, vector<int>* index);
	void TestThreading(TWorld * ptWorld, int nCurrentEntity, CGraphicsSystem* pcGraphicsSystem, CGraphicsSystem::TPrimalVertexBufferType* tTempVertexBuffer, XMMATRIX& tMyVertexBufferWorldMatrix, XMMATRIX* m_d3dPlayerMatrix, CPhysicsSystem* pcPhysicsSystem, CAISystem* pcAiSystem, int PlayerStartIndex, float& playerDamage, float& pirateDamage, float& prevHealth, float& fallingHealth, float& lerpTime, float in_MasterVolume, float in_SFXVolume, float in_MusicVolume, CAudioSystem* pcAudioSystem, XMMATRIX(*doorEventListener)(int, bool), void(*doorEventChanger)(int), float &hitmarkerTime, XMMATRIX* m_d3dClaytonMatrix, XMMATRIX* m_d3dCaelisMatrix,Particle_System * pcParticleSystem, float delta);
	
private:
	bool ContainAABB(int nIndex);
	int classify_aabb_to_plane(TAABB& aabb, plane_t& plane);
	bool spehreCollison(sphere_t a, sphere_t b);
	int classify_sphere_to_plane(sphere_t& sphere, plane_t& plane);
	mutex mylock;
};

