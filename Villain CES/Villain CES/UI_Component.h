#pragma once
#ifndef CES_UI_COMPONENT_H
#define CES_UI_COMPONENT_H
enum eUIComponent
{
	COMPONENT_UIMASK	= 1 << 0,
	COMPONENT_BAR		= 1 << 1,
	COMPONENT_TEXT		= 1 << 2,
	COMPONENT_LABEL		= 1 << 3,
	COMPONENT_BUTTON	= 1 << 4
};

struct TUIMask
{
	int m_tnUIMask = 0;
};

struct TBar
{
	 /*
	 TODO:
		 Fill out Bar Struct
	 */
	XMFLOAT4 backgroundColor;
};

struct TText
{
	char * textBuffer = nullptr;

	/*
	TODO:
		Fill out Text Struct
	*/
};

struct TLabel
{
	float width, height, x, y;
	/*
	TODO:
		Fill out Label Struct
	*/
};

struct TButton
{
	RECT boundingBox;
	int sceneIndex;
};
#endif