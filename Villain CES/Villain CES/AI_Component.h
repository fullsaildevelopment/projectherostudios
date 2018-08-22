#pragma once
#include"stdafx.h"
#ifndef CES_AI_COMPONENT_H
#define CES_AI_COMPONENT_H

enum eAIComponent
{
	COMPONENT_AIMASK = 1 << 0,
	COMPONENT_FOLLOW = 1 << 1,
	COMPONENT_SHOOT =  1 << 2,
	COMPONENT_SEARCH = 1 << 3,
	COMPONENT_SPOTEDPLAYER = 1 << 4,
	COMPONENT_PATHFINDTEST  = 1<< 5,
	COMPONENT_COVERTRIGGER	= 1 << 6
};

struct TAIMask
{
	int m_tnAIMask = 0;
	int GunIndex = -1;
};
struct TAIActive {
	bool active=false;
};
struct TAIVision {
	frustum_t eyes0;
	float3 normalAtBegining[6];
	int indexoffrustum;

	float    visionRotation;
	bool	 keepRotatingRight=true;
	XMMATRIX playerLastKnownLocation;
	int		 wallIndex = -1;
	bool keepSearching = true;
	

};
struct TAIMovement {
	float strafeCoolDown = 0;
	float StrafeCoolDownValue = 100;
};
struct TAIHeath {
	float heath = 100;
};

struct TAIPathFinding {
	vector<XMVECTOR> directions;
	int index = 0;
	int startingNode;
	int Goal;
	bool testingPathFinding = true;
	bool foundDestination = false;
	int DelayMovement = 0;
	bool InterRuptPathPlanning=true;
	

};
struct Tcover {
	vector<int> CoverPositions;
	int availableCovers = 0;
};
struct TCoverTrigger {
	vector<Tcover> coverAiCanGoTo;
	bool inCover;
};

#endif
