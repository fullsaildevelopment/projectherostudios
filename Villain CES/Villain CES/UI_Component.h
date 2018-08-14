#pragma once
#ifndef CES_UI_COMPONENT_H
#define CES_UI_COMPONENT_H
enum eUIComponent
{
	COMPONENT_UIMASK = 1 << 0,
	COMPONENT_BAR = 1 << 1,
	COMPONENT_TEXT = 1 << 2,
	COMPONENT_LABEL = 1 << 3
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
};

struct TText
{
	/*
	TODO:
		Fill out Text Struct
	*/
};

struct TLabel
{
	/*
	TODO:
		Fill out Label Struct
	*/
};
#endif