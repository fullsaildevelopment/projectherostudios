#pragma once
#include "Entity.h"
#include "Graphics_System.h"
#include "DirectXTex-master/WICTextureLoader/WICTextureLoader.h"
class CUISystem
{
public:
	CUISystem();
	~CUISystem();

	void AddTextureToUI(TWorld* tThisWorld, unsigned int nThisEntity, ID3D11Device* device, wchar_t* filepath, ID3D11ShaderResourceView* srvDiffuse = nullptr);
	void AddButtonToUI(HWND* cApplicationWindow, TWorld* tThisWorld, unsigned int nThisEntity, int sceneIndex, bool enabled);
	void AddTextToUI(HWND* cApplicationWindow, TWorld* tThisWorld, unsigned int nThisEntity, wchar_t* text, unsigned int textSize, int* textColor, int justification = 0);
	void AddMaskToUI(TWorld* tThisWorld, unsigned int nThisEntity, eUIComponent mask);
	void AdjustBoundingBox(HWND* cApplicationWindow, TWorld* tThisWorld, unsigned int nThisEntity);
	void AddBarToUI(HWND* cApplicationWindow, TWorld* tThisWorld, unsigned int nThisEntity, XMFLOAT4* backgroundColor);
	void UpdateText(TWorld* tThisWorld, unsigned int nThisEntity, std::vector<TUIVert*>* atUIVertices, wchar_t* character, int textSize, TUIVert* UVs);
	void CreateEnemyHealthBar(HWND* cApplicationWindow, TWorld* tThisWorld, unsigned int &nThisEntity, XMFLOAT4 in_EnemyPos);
	void UpdateEnemyHealthBar(TWorld* tThisWorld, unsigned int nThisEntity, XMFLOAT4 &out_EnemyPos);
	float GetEnemyHealth(TWorld* tThisWorld, unsigned int nThisEntity);
	//HFONT myFont = nullptr;
};
