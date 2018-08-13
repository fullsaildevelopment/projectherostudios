#pragma once
#ifndef CES_ANIMATION_COMPONENT_H
#define CES_ANIMATION_COMPONENT_H
enum eAIComponent
{
	COMPONENT_ANIMATIONMASK = 1 << 0,
	COMPONENT_CLAYTONANIM = 1 << 1,
	COMPONENT_HPIRATEANIM = 1 << 2,
	COMPONENT_SPIRATEANIM = 1 << 3,
};

struct TAnimationMask
{
	int m_tnAIMask = 0;
};

struct TClaytonAnim
{
	bool forward;
	int animType;//Idling, Walking, Running, Shooting, etc.
	int currentFrame;
	int nextFrame;
};

#endif