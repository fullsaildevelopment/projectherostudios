#pragma once
#include"ProjectileSystem.h"
#include <array>
#include"stdafx.h"
#include"Entity.h"
#include <math.h>
struct float3
{
	union
	{

		struct
		{
			float x;
			float y;
			float z;
		};
	};
	float3 operator-(float3 subtractme) {
		float3 newfloat;
		newfloat.x = x - subtractme.x;
		newfloat.y = y - subtractme.y;
		newfloat.z = z - subtractme.z;
		return newfloat;
	}
	float3 operator+(float3 subtractme) {
		float3 newfloat;
		newfloat.x = x + subtractme.x;
		newfloat.y = y + subtractme.y;
		newfloat.z = z + subtractme.z;
		return newfloat;
	}
	float3 operator*(float scaler) {
		float3 newfloat3;
		newfloat3.x = x * scaler;
		newfloat3.y = y * scaler;
		newfloat3.z = z * scaler;

		return newfloat3;
	}
	float3 operator*(float3 mutplyme) {
		float3 newfloat3;
		newfloat3.x = x * mutplyme.x;
		newfloat3.y = y * mutplyme.y;
		newfloat3.z = z * mutplyme.z;
		return newfloat3;
	}
	float& operator[](int i) { return (&x)[i]; }
	float operator[](int i)const { return (&x)[i]; }
};

struct float4
{
	union
	{
		float3 xyz;

		struct
		{

		};

		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
	};

	float& operator[](int i) { return (&x)[i]; }
	float operator[](int i)const { return (&x)[i]; }
	float4 operator+(float4 addme) {
		float4 newfloat4;
		newfloat4.x = addme.x + x;
		newfloat4.y = addme.y + y;
		newfloat4.z = addme.z + z;
		newfloat4.w = addme.w + w;
		return newfloat4;
	}
	float4 operator*(float scaler) {
		float4 newfloat4;
		newfloat4.x = x * scaler;
		newfloat4.y = y * scaler;
		newfloat4.z = z * scaler;
		newfloat4.w = w * scaler;
		return newfloat4;
	}
};
struct float4x4 {
	float4 row1;
	float4 row2;
	float4 row3;
	float4 row4;
};
struct sphere_t { float3 center; float radius; }; //Alterative: using sphere_t = float4;

struct aabb_t { float3 center; float3 extents; }; //Alternative: aabb_t { float3 min; float3 max; };

struct plane_t { float3 normal; float offset; };  //Alterative: using plane_t = float4;

using frustum_t = std::array<plane_t, 6>;
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
		aV.m128_f32[2] = a.x;
		XMVECTOR bV;
		bV.m128_f32[0] = b.x;
		bV.m128_f32[1] = b.y;
		bV.m128_f32[2] = b.z;
		XMVECTOR cV;
		cV.m128_f32[0] = c.x;
		cV.m128_f32[1] = c.y;
		cV.m128_f32[2] = c.z;




		XMVECTOR normalV = XMVector3Cross(bV - aV, cV - bV);
		normalV = XMVector3Normalize(normalV);
		normalV = XMVector3Dot(normalV, aV);
		tempt.normal.x = normalV.m128_f32[0];
		tempt.normal.y = normalV.m128_f32[1];
		tempt.normal.z = normalV.m128_f32[2];

		//tempt.normal = render->normalize(tempt.normal);

		//tempt.offset = render->dot(tempt.normal, a);
		float highesty = a.y;

		float largestx = a.x;
		if (b.x > largestx) {
			largestx = b.x;
		}
		if (c.x > largestx) {
			largestx = c.x;
		}
		if (b.y > highesty) {
			highesty = b.x;
		}
		if (c.y > highesty) {
			highesty = c.x;
		}
		float3 pos;
		pos.x = largestx;
		pos.y = highesty;
		pos.z = a.z;
		//render->add_debug_line(tempt.normal,c,a);
		return tempt;
	}
public:
	int calculate_frustum(TWorld * ptWorld,frustum_t& frustum, float4x4 camera_transform, float fov, float aspect_ratio, float near_offset, float far_offset, int parentWorldMatrixIndex, float xoffset, float yoffset, float zoffset);
	//void FollowAndShoot(XMMATRIX playerMatrix, Clips* AiGun);
	void FollowObject(XMMATRIX thingToFollow, XMMATRIX* AIMatrix);
	void ShootGun(Clips* AIGun);
	void SetNumberOfAI(int aiCount);
	bool LookForPlayer(XMVECTOR start,XMVECTOR end,XMMATRIX boxWorldMaxtrix,TAABB abbcolider,float* distance, CCollisionSystem* pcCollisionPointer);
	XMMATRIX LookBackLeftToRight(XMMATRIX worldMatrix,bool leftorRight);
	
	int GetNumberOfAI();
private:
	int numberofAI;
};

