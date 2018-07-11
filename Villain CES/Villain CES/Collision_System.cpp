#include "stdafx.h"
#include "Collision_System.h"


CCollisionSystem::CCollisionSystem()
{
}


CCollisionSystem::~CCollisionSystem()
{
}

bool CCollisionSystem::ContainAABB(int nIndex)
{
	for (list<TAABB>::iterator ptr=m_AAbb.begin(); ptr != m_AAbb.end(); ++ptr) {
		if ( ptr->m_IndexLocation== nIndex) {
			return true;
		}
	}
	return false;
}

bool CCollisionSystem::classify_aabb_to_aabb(TAABB aabb1, TAABB aabb2)
{
	return (aabb1.m_dMinPoint.x <= aabb2.m_dMaxPoint.x&&aabb1.m_dMaxPoint.x >= aabb2.m_dMinPoint.x) &&
		(aabb1.m_dMinPoint.y <= aabb2.m_dMaxPoint.y&&aabb1.m_dMaxPoint.y >= aabb2.m_dMinPoint.y) &&
		(aabb1.m_dMinPoint.z <= aabb2.m_dMaxPoint.z&&aabb1.m_dMaxPoint.z >= aabb2.m_dMinPoint.z);
}

bool CCollisionSystem::replaceAABB(int nIndex, TAABB m_AABB2)
{
	for (list<TAABB>::iterator ptr = m_AAbb.begin(); ptr != m_AAbb.end(); ++ptr) {
		if (nIndex == ptr->m_IndexLocation) {
			ptr->m_dMaxPoint = m_AABB2.m_dMaxPoint;
			ptr->m_dMinPoint = m_AABB2.m_dMinPoint;
			return true;
		}
	}
	return false;
}

int CCollisionSystem::AddAABBCollider(TAABB m_AABB2, int nIndex)
{
	if (ContainAABB(nIndex) == false) {
		m_AABB2.m_IndexLocation = nIndex;
		m_AAbb.push_back(m_AABB2);
		
		return m_AAbb.size();
	}
}

bool CCollisionSystem::RemoveAABBCollider(int nIndex)
{
	for (list<TAABB>::iterator ptr = m_AAbb.begin(); ptr != m_AAbb.end(); ++ptr) {
		if (ptr->m_IndexLocation == nIndex) {
			m_AAbb.erase(ptr);
			return true;
		}
	}
	
	return false;
}


bool CCollisionSystem::AABBtoAABBCollisionCheck(TAABB m_AABB2)
{
	for (list<TAABB>::iterator ptr = m_AAbb.begin(); ptr != m_AAbb.end(); ++ptr) {
		if (m_AABB2.m_IndexLocation != ptr->m_IndexLocation) {
			if (classify_aabb_to_aabb(m_AABB2, *ptr) == true) {
				return true;
			}
		}
	}
	return false;
}

