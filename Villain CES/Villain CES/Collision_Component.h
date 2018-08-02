#pragma once
#ifndef CES_COLLISION_COMPONENT_H
#define CES_COLLISION_COMPONENT_H
enum eCollision_Component
{
	COMPONENT_COLLISIONMASK = 1 << 0,
	COMPONENT_AABB			= 1 << 1,
	COMPONENT_OBB			= 1 << 2,
	COMPONENT_SPHERE		= 1 << 3,
	COMPONENT_STATIC		= 1 << 4,
	COMPONENT_NONSTATIC	    = 1 << 6,
	COMPONENT_TRIGGER		= 1 << 7,
	COMPONENT_NONTRIGGER	= 1 << 8,
	COMPONENT_LINE			= 1 << 9


}; 

struct TCollisionMask
{
	int m_tnCollisionMask = 0;
};

struct TAABB
{
	XMFLOAT3 m_dMinPoint;
	XMFLOAT3 m_dMaxPoint;
	XMFLOAT3 m_dMinPointOrginal;
	XMFLOAT3 m_dMaxPointOrginal;
	int		 m_IndexLocation;
	
	


};

struct TOBB
{
	/*
		Fill out this structure
	*/
};

struct TSphere
{
	/*
		Fill out this structure
	*/
};
#endif
