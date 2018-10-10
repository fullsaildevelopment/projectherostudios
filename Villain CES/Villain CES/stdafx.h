// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Windows Header Files:


// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <d3d11.h>
#include <DirectXMath.h>
//#include "DirectXTex-master\WICTextureLoader\WICTextureLoader.h"
#include <wchar.h>
#include <fstream>
#include <vector>
#include <ctime>
#include <chrono>
#include <windows.system.diagnostics.h>
using namespace std;
using namespace DirectX;

// TODO: reference additional headers your program requires here
#define ENTITYCOUNT 1100
#define ENTITYCOUNT_MIKESGRAPHICSSANDBOX 10
#define MUSIC_ON true
#define HUD true
//Wwise Related
#ifdef AK_WIN
#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#endif
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
#include <array>
struct sphere_t { float3 center; float radius; }; //Alterative: using sphere_t = float4;

struct aabb_t { float3 center; float3 extents; }; //Alternative: aabb_t { float3 min; float3 max; };

struct plane_t { float3 normal; float offset; };  //Alterative: using plane_t = float4;

using frustum_t = std::array<plane_t, 6>;