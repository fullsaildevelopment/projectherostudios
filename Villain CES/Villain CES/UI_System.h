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
	void AddButtonToUI(TWorld* tThisWorld, unsigned int nThisEntity, int sceneIndex, HWND cApplicationWindow);
};
