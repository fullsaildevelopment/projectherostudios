#pragma once
#include"ProjectileSystem.h"

#include"Entity.h"
#include"stdafx.h"


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
	//void FollowAndShoot(XMMATRIX playerMatrix, Clips* AiGun);
	void FollowObject(XMMATRIX thingToFollow, XMMATRIX* AIMatrix);
	void ShootGun(Clips* AIGun);
	void SetNumberOfAI(int aiCount);
	//bool LookForPlayer(XMVECTOR start,XMVECTOR end,XMMATRIX boxWorldMaxtrix,TAABB abbcolider,float* distance, CCollisionSystem* pcCollisionPointer);
	XMMATRIX LookBackLeftToRight(XMMATRIX worldMatrix,bool leftorRight);
	
	int GetNumberOfAI();
private:
	int numberofAI;
};

