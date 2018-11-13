#pragma once
#ifndef CES_ANIMATION_COMPONENT_H
#define CES_ANIMATION_COMPONENT_H
enum eAnimationComponent
{
	COMPONENT_ANIMATIONMASK = 1 << 0,
	COMPONENT_CLAYTONANIM = 1 << 1,
	COMPONENT_CAELISANIM = 1 << 2,
	COMPONENT_SETHANIM = 1 << 3,
	COMPONENT_HPIRATEANIM = 1 << 4,
	COMPONENT_SPIRATEANIM = 1 << 5,
};

struct TAnimationMask
{
	int m_tnAnimationMask = 0;
};

/*
	Order of animations:

	0. Idling
	1. Walking
	2. Walk backward
	3. Walk left
	4. Walk right
	5. Shooting
	6. Taking damage
	7. Dying
*/


struct TAnimationVariant
{
	//union
	//{
		struct TClaytonAnim
		{
			bool forward = true;
			int animType = 0;//Idling, Walking, Running, Shooting, etc.
			int currentFrame = 0;
			int nextFrame = 1;

			bool playingAnimation = false;
		}tClaytonAnim;

		//	struct TCaelisAnim
		//	{
		//		bool forward = true;
		//		int animType = 0;//Idling, Walking, Running, Shooting, etc.
		//		int currentFrame = 0;
		//		int nextFrame = 1;
		//	}tCaelisAnim;

		//	struct TSethAnim
		//	{
		//		bool forward = true;
		//		int animType = 0;//Idling, Walking, Running, Shooting, etc.
		//		int currentFrame = 0;
		//		int nextFrame = 1;
		//	}tSethAnim;

		//	struct THumanPirateAnim
		//	{
		//		bool forward = true;
		//		int animType = 0;//Idling, Walking, Running, Shooting, etc.
		//		int currentFrame = 0;
		//		int nextFrame = 1;
		//	}tHumanPirateAnim;

		//struct TScyllianPirateAnim
		//{
		//	bool forward = true;
		//	int animType = 0;//Idling, Walking, Running, Shooting, etc.
		//	int currentFrame = 0;
		//	int nextFrame = 1;

		//	bool playingAnimation = false;
		//}tScyllianPirateAnim;

	//};
};




#endif