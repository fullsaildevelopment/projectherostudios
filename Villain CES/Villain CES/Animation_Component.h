#pragma once
#ifndef CES_ANIMATION_COMPONENT_H
#define CES_ANIMATION_COMPONENT_H
enum eAIComponent
{
	COMPONENT_ANIMATIONMASK = 1 << 0,
	COMPONENT_1 = 1 << 1,
	COMPONENT_2 = 1 << 2
};

struct TAnimationMask
{
	int m_tnAIMask = 0;
};

//struct MyStruct
//{
//
//};

#endif