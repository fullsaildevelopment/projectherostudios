#pragma once
#ifndef CES_AI_COMPONENT_H
#define CES_AI_COMPONENT_H
enum eAIComponent
{
	COMPONENT_AIMASK = 1 << 0,
	COMPONENT_FOLLOW = 1 << 1,
	COMPONENT_SHOOT =  1 << 2,
	COMPONENT_SEARCH = 1 << 3,
	COMPONENT_SPOTEDPLAYER = 1 << 4
};

struct TAIMask
{
	int m_tnAIMask = 0;
	int GunIndex = -1;
};
struct TAIVision {
	frustum_t eyes0;
	float    visionRotation;
	bool	 keepRotatingRight=true;
	XMMATRIX playerLastKnownLocation;
	int		 wallIndex = -1;
	bool keepSearching = true;
	

};

#endif
