#pragma once
#include "Entity.h"
#include "Graphics_System.h"
#include "DirectXTex-master/WICTextureLoader/WICTextureLoader.h"
class CUISystem
{
public:
	CUISystem();
	~CUISystem();

	void DrawMenu(TWorld* ptWorld, CGraphicsSystem* pcGraphicsSystem, XMMATRIX cameraMatrix, XMMATRIX worldMatrix);
	void AddTextureToUI(TWorld* tThisWorld, unsigned int nThisEntity, ID3D11Device* device, wchar_t* filepath);
	void AddButtonToUI(HWND* cApplicationWindow, TWorld* tThisWorld, unsigned int nThisEntity, int sceneIndex, bool enabled);
	void AddTextToUI(HWND* cApplicationWindow, TWorld* tThisWorld, unsigned int nThisEntity, wchar_t* text, unsigned int textSize, int* textColor);
	void AddMaskToUI(TWorld* tThisWorld, unsigned int nThisEntity, eUIComponent mask);
	void DrawPauseMenu(HWND* cApplicationWindow, TWorld* tThisWorld, HDC* toHDC, unsigned int nCurrentEntity);

	HFONT myFont = nullptr;
};
