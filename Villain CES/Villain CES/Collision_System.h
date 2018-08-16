#pragma once
#include"Collision_Component.h"
#include <list>
#include"stdafx.h"
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
	TAABB createAABBS(std::vector<XMFLOAT3> verticies, TAABB AABBDATA);
	bool classify_aabb_to_aabb(TAABB aabb1, TAABB aabb2);


	list<TAABB> m_AAbb;
	vector<TAABB> AiFrustumCheck;
	bool aabb_to_frustum(TAABB& aabb, frustum_t& frustum);
	bool AiVisionCheck(frustum_t eyeSight, vector<int>* index);

private:
	bool ContainAABB(int nIndex);
	int classify_aabb_to_plane(TAABB& aabb, plane_t& plane);
	bool spehreCollison(sphere_t a, sphere_t b);
	int classify_sphere_to_plane(sphere_t& sphere, plane_t& plane);
	


};
