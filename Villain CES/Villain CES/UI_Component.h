#pragma once
#ifndef CES_UI_COMPONENT_H
#define CES_UI_COMPONENT_H
enum eUIComponent
{
	COMPONENT_UIMASK		= 1 << 0,
	COMPONENT_BAR			= 1 << 1,
	COMPONENT_TEXT			= 1 << 2,
	COMPONENT_LABEL			= 1 << 3,
	COMPONENT_BUTTON		= 1 << 4,
	COMPONENT_PAUSESCREEN	= 1 << 5,
	COMPONENT_OPTIONS		= 1 << 6,
	COMPONENT_DEATHSCREEN	= 1 << 7,
	COMPONENT_BACKGROUND	= 1 << 8,
	COMPONENT_HUD			= 1 << 9
};

struct TUIMask
{
	int m_tnUIMask = 0;
};

struct TBar
{
	POINT start, end;
	RECT barBoundingBox;
	float ratio;
	 
	XMFLOAT4 backgroundColor;
};

struct TText
{
	wchar_t * textBuffer = nullptr;
	unsigned int textSize = 0;
	RECT textBoundingBox;
	int textColor[3] = { 0, 0, 0 };
	int justification = 0;
	
};

struct TLabel
{
	float width, height, x, y;
	bool addText = false;
	bool addTexture = false;
	bool lastUIElement = false;
};

struct TButton
{
	RECT boundingBox;
	int sceneIndex;
	bool enabled;
};
#endif