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
	void AddBarToUI(HWND* cApplicationWindow, TWorld* tThisWorld, unsigned int nThisEntity, XMFLOAT4* backgroundColor, char* function, unsigned int textSize = 0, float ratio = 1);
	void UpdateText(TWorld* tThisWorld, unsigned int nThisEntity, std::vector<TUIVert*>* atUIVertices, wchar_t* character, int textSize, TUIVert* UVs);
	bool CheckIfStringsAreTheSame(char* string1, unsigned int textSize1, const char* string2);
	void CheckOptionsBars(TWorld* tThisWorld, CInputSystem* pcInputSystem, unsigned int nThisEntity, char* valueToChange, unsigned int valueToChangeSize, float& m_fMasterVolume, float& m_fMusicVolume, float& m_fSFXVolume, int& masterIndex, int& musicIndex, int& fxIndex);
};
