#pragma once
#include"ProjectileSystem.h"

#include"Entity.h"
#include"stdafx.h"
#include"PriorityQueue.h"
#include<map>
#include "platform.h"
using namespace fullsail_ai;

class CAISystem
{
public:
	CAISystem();
	~CAISystem();


	plane_t calculate_plane(float3 a, float3 b, float3 c) {
		plane_t tempt;
		XMVECTOR aV;
		aV.m128_f32[0] = a.x;
		aV.m128_f32[1] = a.y;
		aV.m128_f32[2] = a.z;
		XMVECTOR bV;
		bV.m128_f32[0] = b.x;
		bV.m128_f32[1] = b.y;
		bV.m128_f32[2] = b.z;
		XMVECTOR cV;
		cV.m128_f32[0] = c.x;
		cV.m128_f32[1] = c.y;
		cV.m128_f32[2] = c.z;


		XMVECTOR bvMinusAV;
		bvMinusAV = bV - aV;

		XMVECTOR normalV = XMVector3Cross(bV - aV, cV - bV);
		normalV = XMVector3Normalize(normalV);
	
		tempt.normal.x = normalV.m128_f32[0];
		tempt.normal.y = normalV.m128_f32[1];
		tempt.normal.z = normalV.m128_f32[2];
		tempt.offset = (float)(tempt.normal.x*aV.m128_f32[0]) + (float)(tempt.normal.y*aV.m128_f32[1]) + (float)(tempt.normal.z*aV.m128_f32[2]);
	

		//tempt.normal = render->normalize(tempt.normal);

		//tempt.offset = render->dot(tempt.normal, a);
		
		return tempt;
	}
public:
	int calculate_frustum(TWorld * ptWorld,frustum_t& frustum, float4x4 camera_transform, float fov, float aspect_ratio, float near_offset, float far_offset, int parentWorldMatrixIndex, float xoffset, float yoffset, float zoffset);
	void UpdateFrustum(frustum_t& frustum, float4x4 camera_transform, float fov, float aspect_ratio, float near_offset, float far_offset);
	//void FollowAndShoot(XMMATRIX playerMatrix, Clips* AiGun);
	void FollowObject(XMMATRIX thingToFollow, XMMATRIX* AIMatrix);
	void ShootGun(Clips* AIGun);
	void SetNumberOfAI(int aiCount);
	//bool LookForPlayer(XMVECTOR start,XMVECTOR end,XMMATRIX boxWorldMaxtrix,TAABB abbcolider,float* distance, CCollisionSystem* pcCollisionPointer);
	XMMATRIX LookBackLeftToRight(XMMATRIX worldMatrix,bool leftorRight);
	
	int GetNumberOfAI();
	void FindBestPath(int start, int end, vector<XMVECTOR>* directions);
	void AddNodeToPathFinding(int index, XMFLOAT3 pos, float weight);
	void AddEdgestoNode(int nodeyouAreChanging, vector<int> edges);
	void PathPlaningMovement(TAIPathFinding* path, XMMATRIX* worldMatrix);
	void LookAtObject(XMMATRIX thingToLookAt, XMMATRIX* AIMatrix);
	void Strafe(XMMATRIX* AiMatrix);
private:
	
	int numberofAI;
	struct tiledata {
		XMFLOAT3 pos;
	};
	struct SearchNode
	{
		float weight;
		tiledata* tile;
		vector<SearchNode*> edges;
	};

	struct PlannerNode
	{

		SearchNode* state = nullptr;
		PlannerNode *parent = nullptr;
		float heuristicCost;
		float givenCost;
		float finalCost;


	};
	map<int, SearchNode*>Nodes;
	float CalcualteDistance(tiledata* _search, tiledata * goal);
	map<SearchNode*, PlannerNode*> visited;
	PriorityQueue<PlannerNode*> open;
	float GetFinalCost(PlannerNode* finalCOst) {
		return finalCOst->givenCost + finalCOst->heuristicCost*finalCOst->state->weight;
	}
	float calculateTest(PlannerNode* _givencost, SearchNode* _weight) {
		return _givencost->givenCost + _weight->weight;
	}
	void MakeDirections(vector<XMVECTOR> *directions, PlannerNode* current);
};

