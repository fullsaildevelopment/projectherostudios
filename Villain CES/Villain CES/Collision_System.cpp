#include "stdafx.h"
#include "Collision_System.h"


CCollisionSystem::CCollisionSystem()
{
}


CCollisionSystem::~CCollisionSystem()
{
}

/*
* ContainAABB():  Determins if the list of aabbs contains the aabb index being passed in
*
* Ins:                  nIndex
*                    
*
* Outs:      
*
* Returns:              bool
*
* Mod. Date:              07/11/2018
* Mod. Initials:          AP
*/
bool CCollisionSystem::ContainAABB(int nIndex)
{
	for (list<TAABB>::iterator ptr=m_AAbb.begin(); ptr != m_AAbb.end(); ++ptr) {
		if ( ptr->m_IndexLocation== nIndex) {
			return true;
		}
	}
	return false;
}
/*
* classify_aabb_to_aabb():  Determines if the two AABB are coliding or not
*
* Ins:                  two AABBs
*
*
* Outs:
*
* Returns:              bool
*
* Mod. Date:              07/11/2018
* Mod. Initials:          AP
*/

bool CCollisionSystem::classify_aabb_to_aabb(TAABB aabb1, TAABB aabb2)
{

	if (aabb1.m_dMinPoint.x <= aabb2.m_dMaxPoint.x) {
		float x = 0;
	}
	if (aabb1.m_dMaxPoint.x >= aabb2.m_dMinPoint.x)
	{
		float x = 0;
	}
	if (aabb1.m_dMinPoint.y <= aabb2.m_dMaxPoint.y) {
		float x = 0;
	}
	if (aabb1.m_dMaxPoint.y >= aabb2.m_dMinPoint.y) {
		float x = 0;
	}
	if (aabb1.m_dMinPoint.z <= aabb2.m_dMaxPoint.z) {
		float x = 0;
	}
	if (aabb1.m_dMaxPoint.z >= aabb2.m_dMinPoint.z) {
		float x = 0;
	}
	return (aabb1.m_dMinPoint.x <= aabb2.m_dMaxPoint.x&&aabb1.m_dMaxPoint.x >= aabb2.m_dMinPoint.x) &&
	(aabb1.m_dMinPoint.y <= aabb2.m_dMaxPoint.y&&aabb1.m_dMaxPoint.y >= aabb2.m_dMinPoint.y) &&
	(aabb1.m_dMinPoint.z <= aabb2.m_dMaxPoint.z&&aabb1.m_dMaxPoint.z >= aabb2.m_dMinPoint.z);
}
/*
* replaceAABB():  finds an AABB if able in the list and then replaces the data with the new AABB
*
* Ins:                  Index to AABB in list
*						AABB to replace it with
*
*
* Outs:
*
* Returns:              bool
*
* Mod. Date:              07/11/2018
* Mod. Initials:          AP
*/
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
TAABB CCollisionSystem::updateAABB(XMMATRIX worldMatrix, TAABB aabb)
{
	XMVECTOR max;
	max.m128_f32[0] = aabb.m_dMaxPointOrginal.x;
	max.m128_f32[1] = aabb.m_dMaxPointOrginal.y;
	max.m128_f32[2] = aabb.m_dMaxPointOrginal.z;
	max.m128_f32[3] = 1;
	max = XMVector4Transform(max, worldMatrix);

	XMVECTOR min;
	min.m128_f32[0] = aabb.m_dMinPointOrginal.x;
	min.m128_f32[1] = aabb.m_dMinPointOrginal.y;
	min.m128_f32[2] = aabb.m_dMinPointOrginal.z;
	min.m128_f32[3] = 1;
	min = XMVector4Transform(min, worldMatrix);
	TAABB newaabb;
	newaabb = aabb;
	newaabb.m_dMaxPoint.x = max.m128_f32[0];
	newaabb.m_dMaxPoint.y = max.m128_f32[1];
	newaabb.m_dMaxPoint.z = max.m128_f32[2];

	newaabb.m_dMinPoint.x = min.m128_f32[0];
	newaabb.m_dMinPoint.y = min.m128_f32[1];
	newaabb.m_dMinPoint.z = min.m128_f32[2];
	newaabb.m_dMinPointOrginal = aabb.m_dMinPointOrginal;
	newaabb.m_dMaxPointOrginal = aabb.m_dMaxPointOrginal;

	replaceAABB(aabb.m_IndexLocation, newaabb);
	return newaabb;
}
TAABB CCollisionSystem::createAABBS(std::vector<XMFLOAT3> verticies)
{
	int size = verticies.size();
	if (size == 0) { return TAABB(); }
	TAABB aabb;
	aabb.m_dMaxPoint.x = verticies[0].x;
	aabb.m_dMaxPoint.y = verticies[0].y;
	aabb.m_dMaxPoint.z = verticies[0].z;
	aabb.m_dMaxPointOrginal.x = verticies[0].x;
	aabb.m_dMaxPointOrginal.y = verticies[0].y;
	aabb.m_dMaxPointOrginal.z = verticies[0].z;
	aabb.m_dMinPointOrginal.x = verticies[0].x;
	aabb.m_dMinPointOrginal.y = verticies[0].y;
	aabb.m_dMinPointOrginal.z = verticies[0].z;



	aabb.m_dMinPoint.x = verticies[0].x;
	aabb.m_dMinPoint.y = verticies[0].y;
	aabb.m_dMinPoint.z = verticies[0].z;

	for (int i = 1; i < size ; ++i) {
		if (verticies[i].x > aabb.m_dMaxPoint.x) {
			aabb.m_dMaxPoint.x = verticies[i].x;
			aabb.m_dMaxPointOrginal.x = aabb.m_dMaxPoint.x;

		}
		if (verticies[i].x < aabb.m_dMinPoint.x) {
			aabb.m_dMinPoint.x = verticies[i].x;
			aabb.m_dMinPointOrginal.x = aabb.m_dMinPoint.x;

		}

		if (verticies[i].y > aabb.m_dMaxPoint.y) {
			aabb.m_dMaxPoint.y = verticies[i].y;
			aabb.m_dMaxPointOrginal.y = aabb.m_dMaxPoint.y;

		}
		if (verticies[i].y < aabb.m_dMinPoint.y) {
			aabb.m_dMinPoint.y = verticies[i].y;
			aabb.m_dMinPointOrginal.y = aabb.m_dMinPoint.y;

		}

		if (verticies[i].z > aabb.m_dMaxPoint.z) {
			aabb.m_dMaxPoint.z = verticies[i].z;
			aabb.m_dMaxPointOrginal.z = aabb.m_dMaxPoint.z;

		}
		if (verticies[i].z < aabb.m_dMinPoint.z) {
			aabb.m_dMinPoint.z = verticies[i].z;
			aabb.m_dMinPointOrginal.z = aabb.m_dMinPoint.z;

		}
	}
	return aabb;
}

/*
* AddAABBCollider():  Adds an AABB to the list if it is not already in the list
*
* Ins:                  Index to AABB in list
*						AABB to add
*
*
* Outs:
*
* Returns:              bool
*
* Mod. Date:              07/11/2018
* Mod. Initials:          AP
*/
int CCollisionSystem::AddAABBCollider(TAABB m_AABB2, int nIndex)
{
	if (ContainAABB(nIndex) == false) {
		m_AABB2.m_IndexLocation = nIndex;
		m_AAbb.push_back(m_AABB2);
		
		return m_AAbb.size();
	}
}
/*
* RemoveAABBCollider():  Adds an AABB to the list if it is not already in the list
*
* Ins:                  Index to AABB in list
*						AABB to add
*
*
* Outs:
*
* Returns:              bool
*
* Mod. Date:              07/11/2018
* Mod. Initials:          AP
*/
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


bool CCollisionSystem::AABBtoAABBCollisionCheck(TAABB m_AABB2, vector<int>* m_OtherColision)
{
	for (list<TAABB>::iterator ptr = m_AAbb.begin(); ptr != m_AAbb.end(); ++ptr) {
		if (m_AABB2.m_IndexLocation != ptr->m_IndexLocation) {
			if (classify_aabb_to_aabb(m_AABB2, *ptr) == true) {
				m_OtherColision->push_back(ptr->m_IndexLocation);
				
			}
		}
	}
	if (m_OtherColision->size() != 0) {
		return true;
	}
	
	return false;
}

