#pragma once
#include "Entity.h"
#include "Graphics_System.h"
#include "DirectXTex-master/WICTextureLoader/WICTextureLoader.h"
class CUISystem
{
public:
	CUISystem();
	~CUISystem();

	void AddTextureToUI(TWorld* tThisWorld, int& nThisEntity, ID3D11Device* device, wchar_t* filepath, ID3D11ShaderResourceView* srvDiffuse = nullptr);
	void AddButtonToUI(HWND* cApplicationWindow, TWorld* tThisWorld, int& nThisEntity, int sceneIndex, bool enabled);
	void AddTextToUI(HWND* cApplicationWindow, TWorld* tThisWorld, unsigned int& nThisEntity, wchar_t* text, unsigned int& textSize, int* textColor, int justification = 0);
	void AddMaskToUI(TWorld* tThisWorld, int& nThisEntity, eUIComponent mask);
	void AdjustBoundingBox(HWND* cApplicationWindow, TWorld* tThisWorld, unsigned int& nThisEntity);
	void AddBarToUI(HWND* cApplicationWindow, TWorld* tThisWorld, int& nThisEntity, XMFLOAT4* backgroundColor, char* function, unsigned int textSize = 0, float ratio = 1);
	void UpdateText(TWorld* tThisWorld, CGraphicsSystem* pcGraphicsSystem, int& nThisEntity, std::vector<TUIVert*>* atUIVertices, wchar_t* character, int& textSize, TUIVert* UVs);
	bool CheckIfStringsAreTheSame(char* string1, int& textSize1, const char* string2);
	void CheckOptionsBars(TWorld* tThisWorld, CInputSystem* pcInputSystem, int& nThisEntity, float& m_fMasterVolume, float& m_fDialogueVolume, float& m_fMusicVolume, float& m_fSFXVolume, int& masterIndex, int& dialogueIndex, int& musicIndex, int& fxIndex);
	void UpdateFPS(TWorld* tThisWorld, CGraphicsSystem* pcGraphicsSystem, CFPS& fpsTimer, int& fpsIndex, CGraphicsSystem::TUIVertexBufferType& tUIVertexBuffer, CGraphicsSystem::TUIPixelBufferType& tUIPixelBuffer, std::vector<TUIVert*>& atUIVertices, TCamera* menuCamera);
	void ScrollText(TWorld* tThisWorld, CGraphicsSystem* pcGraphicsSystem, int& nThisEntity, std::vector<TUIVert*>* atUIVertices, double delta);
	void UpdateHUDBars(TWorld* tThisWorld, int& nThisEntity, CGraphicsSystem::TUIVertexBufferType& tUIVertexBuffer, int& screenWidth, int& PlayerStartIndex, int& GunIndexForPlayer);
	void UpdateDirtyUI(HWND* cApplicationWindow, TWorld* tThisWorld, bool fullscreen);
};
