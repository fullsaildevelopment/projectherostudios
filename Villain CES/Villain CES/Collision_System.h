#pragma once
#include"Collision_Component.h"
#include <list>
using namespace std;
class CCollisionSystem
{
	//panda
public:

	CCollisionSystem();
	~CCollisionSystem();
	int AddAABBCollider(TAABB m_AABB,int nIndex);
	bool RemoveAABBCollider(int nIndex);
	bool AABBtoAABBCollisionCheck(TAABB m_AABB);
	bool replaceAABB(int nIndex, TAABB m_AABB2);
private:
	bool ContainAABB(int nIndex);
	bool classify_aabb_to_aabb(TAABB aabb1, TAABB aabb2);
	
	list<TAABB> m_AAbb;


};

