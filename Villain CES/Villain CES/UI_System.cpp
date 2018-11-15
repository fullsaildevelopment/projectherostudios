#include "stdafx.h"
#include "UI_System.h"


CUISystem::CUISystem()
{
}


CUISystem::~CUISystem()
{
}

void CUISystem::AddTextureToUI(TWorld* tThisWorld, int& nThisEntity, ID3D11Device* device, wchar_t* filepath, ID3D11ShaderResourceView* srvDiffuse)
{
	if (srvDiffuse == nullptr)
	{
		HRESULT result;

		ID3D11Resource* resource;

		result = CreateWICTextureFromFile(device, filepath, &resource, &tThisWorld->atMesh[nThisEntity].m_d3dSRVDiffuse, NULL);

		tThisWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = tThisWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask | COMPONENT_TEXTURE;
	}
	else
	{
		tThisWorld->atMesh[nThisEntity].m_d3dSRVDiffuse = srvDiffuse;
		tThisWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = tThisWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask | COMPONENT_TEXTURE;
	}
}

void CUISystem::AddButtonToUI(HWND* cApplicationWindow, TWorld* tThisWorld, int& nThisEntity, int sceneIndex, bool enabled)
{
	RECT window;
	GetWindowRect(*cApplicationWindow, &window);

	float screenWidth = window.right - window.left;
	float screenHeight = window.bottom - window.top;

	float ratioLeftX = (screenWidth / 2) * ((tThisWorld->atLabel[nThisEntity].x - (tThisWorld->atLabel[nThisEntity].width / 2)) * .1);
	float ratioTopY = (screenHeight / 2) * ((tThisWorld->atLabel[nThisEntity].y + (tThisWorld->atLabel[nThisEntity].height / 2)) * .1);
	float ratioRightX = (screenWidth / 2) * ((tThisWorld->atLabel[nThisEntity].x + (tThisWorld->atLabel[nThisEntity].width / 2)) * .1);
	float ratioBottomY = (screenHeight / 2) * ((tThisWorld->atLabel[nThisEntity].y - (tThisWorld->atLabel[nThisEntity].height / 2)) * .1);

	tThisWorld->atButton[nThisEntity].boundingBox.bottom = (screenHeight / 2) + (ratioBottomY * -1) + tThisWorld->atLabel[nThisEntity].y;
	tThisWorld->atButton[nThisEntity].boundingBox.top = (screenHeight / 2) + (ratioTopY * -1) + tThisWorld->atLabel[nThisEntity].y;
	tThisWorld->atButton[nThisEntity].boundingBox.left = (screenWidth / 2) + ratioLeftX - 9;
	tThisWorld->atButton[nThisEntity].boundingBox.right = (screenWidth / 2) + ratioRightX - 9;

	tThisWorld->atButton[nThisEntity].sceneIndex = sceneIndex;

	tThisWorld->atButton[nThisEntity].enabled = enabled;

	tThisWorld->atUIMask[nThisEntity].m_tnUIMask = tThisWorld->atUIMask[nThisEntity].m_tnUIMask | COMPONENT_BUTTON;
}

void CUISystem::AddTextToUI(HWND* cApplicationWindow, TWorld* tThisWorld, unsigned int& nThisEntity, wchar_t* text, unsigned int& textSize, int* textColor, int justification)
{
	tThisWorld->atText[nThisEntity].textBuffer = new wchar_t[textSize]; 

	int lines = 1;
	for (int i = 0; i < textSize; ++i)
	{
		if (text[i] == '\n')
			++lines;

		tThisWorld->atText[nThisEntity].textBuffer[i] = text[i];
	}

	tThisWorld->atText[nThisEntity].textSize = textSize - 1;
	
	tThisWorld->atText[nThisEntity].textColor[0] = textColor[0];
	tThisWorld->atText[nThisEntity].textColor[1] = textColor[1];
	tThisWorld->atText[nThisEntity].textColor[2] = textColor[2];

	tThisWorld->atText[nThisEntity].justification = justification;

	AdjustBoundingBox(cApplicationWindow, tThisWorld, nThisEntity);

	tThisWorld->atUIMask[nThisEntity].m_tnUIMask = tThisWorld->atUIMask[nThisEntity].m_tnUIMask | COMPONENT_TEXT;
}

void CUISystem::AddMaskToUI(TWorld* tThisWorld, int& nThisEntity, eUIComponent mask)
{
	tThisWorld->atUIMask[nThisEntity].m_tnUIMask = tThisWorld->atUIMask[nThisEntity].m_tnUIMask | mask;
}

void CUISystem::AdjustBoundingBox(HWND* cApplicationWindow, TWorld* tThisWorld, unsigned int& nThisEntity)
{
	HDC tHDC = GetDC(0);
	//HGDIOBJ oldFont = SelectObject(tHDC, (HGDIOBJ)myFont);

	SIZE tempSize;
	GetTextExtentPoint32(tHDC, tThisWorld->atText[nThisEntity].textBuffer, tThisWorld->atText[nThisEntity].textSize, &tempSize);

	int height = DrawText(tHDC, tThisWorld->atText[nThisEntity].textBuffer, tThisWorld->atText[nThisEntity].textSize, &tThisWorld->atText[nThisEntity].textBoundingBox, DT_CALCRECT);

	/*if (tThisWorld->atButton[nThisEntity].boundingBox.left == 0)
	{
		RECT window;
		GetWindowRect(*cApplicationWindow, &window);

		float screenWidth = window.right - window.left;
		float screenHeight = window.bottom - window.top;

		float ratioLeftX = (screenWidth / 2) * ((tThisWorld->atLabel[nThisEntity].x - (tThisWorld->atLabel[nThisEntity].width / 2)) * .1);
		float ratioTopY = (screenHeight / 2) * ((tThisWorld->atLabel[nThisEntity].y + (tThisWorld->atLabel[nThisEntity].height / 2)) * .1);
		float ratioRightX = (screenWidth / 2) * ((tThisWorld->atLabel[nThisEntity].x + (tThisWorld->atLabel[nThisEntity].width / 2)) * .1);
		float ratioBottomY = (screenHeight / 2) * ((tThisWorld->atLabel[nThisEntity].y - (tThisWorld->atLabel[nThisEntity].height / 2)) * .1);

		tThisWorld->atButton[nThisEntity].boundingBox.bottom = (screenHeight / 2) + (ratioBottomY * -1) - 17 + tThisWorld->atLabel[nThisEntity].y;
		tThisWorld->atButton[nThisEntity].boundingBox.top = (screenHeight / 2) + (ratioTopY * -1) - 17 + tThisWorld->atLabel[nThisEntity].y;
		tThisWorld->atButton[nThisEntity].boundingBox.left = (screenWidth / 2) + ratioLeftX - 9;
		tThisWorld->atButton[nThisEntity].boundingBox.right = (screenWidth / 2) + ratioRightX - 9;
	}*/

	POINT tempPoint = { tThisWorld->atButton[nThisEntity].boundingBox.left, tThisWorld->atButton[nThisEntity].boundingBox.top };
	POINT tempPoint2 = { tThisWorld->atButton[nThisEntity].boundingBox.right, tThisWorld->atButton[nThisEntity].boundingBox.bottom };

	ClientToScreen(*cApplicationWindow, &tempPoint);
	ClientToScreen(*cApplicationWindow, &tempPoint2);

	int difference = (tempSize.cx) - (tempPoint2.x - tempPoint.x);
	
	if (tThisWorld->atText[nThisEntity].justification == 0)
	{
		if (difference > 0)
		{
			tThisWorld->atText[nThisEntity].textBoundingBox.left = tempPoint.x - (difference / 2);
			tThisWorld->atText[nThisEntity].textBoundingBox.top = tempPoint.y;
			tThisWorld->atText[nThisEntity].textBoundingBox.right = tempPoint2.x + (difference / 2);
			tThisWorld->atText[nThisEntity].textBoundingBox.bottom = tempPoint2.y;

			tThisWorld->atButton[nThisEntity].boundingBox.left -= (difference / 2);
			tThisWorld->atButton[nThisEntity].boundingBox.right += (difference / 2);
		}
		else
		{
			tThisWorld->atText[nThisEntity].textBoundingBox.left = tempPoint.x + ((abs(difference)) / 2);
			tThisWorld->atText[nThisEntity].textBoundingBox.top = tempPoint.y;
			tThisWorld->atText[nThisEntity].textBoundingBox.right = tempPoint2.x - ((abs(difference)) / 2);
			tThisWorld->atText[nThisEntity].textBoundingBox.bottom = tempPoint2.y;

			tThisWorld->atButton[nThisEntity].boundingBox.left += (abs(difference) / 2);
			tThisWorld->atButton[nThisEntity].boundingBox.right -= (abs(difference) / 2);
		}
	}
	else if (tThisWorld->atText[nThisEntity].justification == 1)
	{
		if (difference > 0)
		{
			tThisWorld->atText[nThisEntity].textBoundingBox.left = tempPoint.x - (difference);
			tThisWorld->atText[nThisEntity].textBoundingBox.top = tempPoint.y;
			tThisWorld->atText[nThisEntity].textBoundingBox.right = tempPoint2.x;
			tThisWorld->atText[nThisEntity].textBoundingBox.bottom = tempPoint2.y;

			tThisWorld->atButton[nThisEntity].boundingBox.left -= (difference);
		}
		else
		{
			tThisWorld->atText[nThisEntity].textBoundingBox.left = tempPoint.x + ((abs(difference)));
			tThisWorld->atText[nThisEntity].textBoundingBox.top = tempPoint.y;
			tThisWorld->atText[nThisEntity].textBoundingBox.right = tempPoint2.x;
			tThisWorld->atText[nThisEntity].textBoundingBox.bottom = tempPoint2.y;

			tThisWorld->atButton[nThisEntity].boundingBox.left += (abs(difference));
		}
	}
	else if (tThisWorld->atText[nThisEntity].justification == -1)
	{
		if (difference > 0)
		{
			tThisWorld->atText[nThisEntity].textBoundingBox.left = tempPoint.x;
			tThisWorld->atText[nThisEntity].textBoundingBox.top = tempPoint.y;
			tThisWorld->atText[nThisEntity].textBoundingBox.right = tempPoint2.x + (difference);
			tThisWorld->atText[nThisEntity].textBoundingBox.bottom = tempPoint2.y;

			tThisWorld->atButton[nThisEntity].boundingBox.right += (difference);
		}
		else
		{
			tThisWorld->atText[nThisEntity].textBoundingBox.left = tempPoint.x;
			tThisWorld->atText[nThisEntity].textBoundingBox.top = tempPoint.y;
			tThisWorld->atText[nThisEntity].textBoundingBox.right = tempPoint2.x - ((abs(difference)));
			tThisWorld->atText[nThisEntity].textBoundingBox.bottom = tempPoint2.y;

			tThisWorld->atButton[nThisEntity].boundingBox.right -= (abs(difference));
		}
	}

	difference = (height)-(tempPoint2.y - tempPoint.y);
	if (difference > 0)
	{
		tThisWorld->atText[nThisEntity].textBoundingBox.bottom += (difference);

		tThisWorld->atButton[nThisEntity].boundingBox.bottom += (difference);
	}
	else
	{
		tThisWorld->atText[nThisEntity].textBoundingBox.bottom += (difference);

		tThisWorld->atButton[nThisEntity].boundingBox.bottom += (difference);
	}

	//tThisWorld->atText[nThisEntity].textColor[0] = textColor[0];
	//tThisWorld->atText[nThisEntity].textColor[1] = textColor[1];
	//tThisWorld->atText[nThisEntity].textColor[2] = textColor[2];

	//tThisWorld->atUIMask[nThisEntity].m_tnUIMask = tThisWorld->atUIMask[nThisEntity].m_tnUIMask | COMPONENT_TEXT;

	//SelectObject(tHDC, oldFont);
	ReleaseDC(*cApplicationWindow, tHDC);
}

void CUISystem::AddBarToUI(HWND* cApplicationWindow, TWorld* tThisWorld, int& nThisEntity, XMFLOAT4* backgroundColor, char* function, unsigned int textSize, float ratio)
{
	if (function != nullptr)
	{
		tThisWorld->atBar[nThisEntity].valueToChange = new char[textSize];

		for (int i = 0; i < textSize; ++i)
		{
			tThisWorld->atBar[nThisEntity].valueToChange[i] = function[i];
		}
	}

	RECT window;
	GetWindowRect(*cApplicationWindow, &window);

	float screenWidth = window.right - window.left;
	float screenHeight = window.bottom - window.top;

	float ratioLeftX = (screenWidth / 2) * ((tThisWorld->atLabel[nThisEntity].x - (tThisWorld->atLabel[nThisEntity].width / 2)) * .1);
	float ratioTopY = (screenHeight / 2) * ((tThisWorld->atLabel[nThisEntity].y + (tThisWorld->atLabel[nThisEntity].height / 2)) * .1);
	float ratioRightX = (screenWidth / 2) * ((tThisWorld->atLabel[nThisEntity].x + (tThisWorld->atLabel[nThisEntity].width / 2)) * .1);
	float ratioBottomY = (screenHeight / 2) * ((tThisWorld->atLabel[nThisEntity].y - (tThisWorld->atLabel[nThisEntity].height / 2)) * .1);

	tThisWorld->atBar[nThisEntity].start.y = (screenHeight / 2) + (ratioTopY * -1) + tThisWorld->atLabel[nThisEntity].y;
	tThisWorld->atBar[nThisEntity].end.y = (screenHeight / 2) + (ratioBottomY * -1) + tThisWorld->atLabel[nThisEntity].y;
	tThisWorld->atBar[nThisEntity].start.x = (screenWidth / 2) + ratioLeftX;
	tThisWorld->atBar[nThisEntity].end.x = (screenWidth / 2) + ratioRightX;

	tThisWorld->atBar[nThisEntity].barBoundingBox.top = (screenHeight / 2) + (ratioTopY * -1) + tThisWorld->atLabel[nThisEntity].y;
	tThisWorld->atBar[nThisEntity].barBoundingBox.bottom = (screenHeight / 2) + (ratioBottomY * -1) + tThisWorld->atLabel[nThisEntity].y;
	tThisWorld->atBar[nThisEntity].barBoundingBox.left = (screenWidth / 2) + ratioLeftX - 5;
	tThisWorld->atBar[nThisEntity].barBoundingBox.right = (screenWidth / 2) + ratioRightX + 6;

	ClientToScreen(*cApplicationWindow, &tThisWorld->atBar[nThisEntity].start);
	ClientToScreen(*cApplicationWindow, &tThisWorld->atBar[nThisEntity].end);

	tThisWorld->atBar[nThisEntity].ratio = ratio;

	if (backgroundColor != nullptr)
		tThisWorld->atBar[nThisEntity].backgroundColor = *backgroundColor;

	tThisWorld->atBar[nThisEntity].valueToChangeSize = textSize;

	tThisWorld->atUIMask[nThisEntity].m_tnUIMask = tThisWorld->atUIMask[nThisEntity].m_tnUIMask | COMPONENT_BAR;
}

void CUISystem::UpdateText(TWorld* tThisWorld, CGraphicsSystem* pcGraphicsSystem, int& nThisEntity, std::vector<TUIVert*>* atUIVertices, wchar_t* character, int& textSize, TUIVert* UVs)
{
	XMFLOAT2* tempfloats = new XMFLOAT2[4];
	
	int countIndex = 0;
	for (int loopIndex = 0; loopIndex < textSize; ++loopIndex)
	{
		GetUVsForCharacter(&character[loopIndex], tempfloats);

		atUIVertices->at(tThisWorld->atLabel[nThisEntity].vIndex)[countIndex].m_d3dfUVs = tempfloats[0];
		atUIVertices->at(tThisWorld->atLabel[nThisEntity].vIndex)[countIndex + 1].m_d3dfUVs = tempfloats[1];
		atUIVertices->at(tThisWorld->atLabel[nThisEntity].vIndex)[countIndex + 2].m_d3dfUVs = tempfloats[2];
		atUIVertices->at(tThisWorld->atLabel[nThisEntity].vIndex)[countIndex + 3].m_d3dfUVs = tempfloats[3];

		countIndex += 4;
	}

	tThisWorld->atMesh[nThisEntity].m_d3dVertexData.pSysMem = &atUIVertices->at(tThisWorld->atLabel[nThisEntity].vIndex)[0];

	tThisWorld->atMesh[nThisEntity].m_VertexData.clear();
	for (int i = 0; i < tThisWorld->atMesh[nThisEntity].m_nVertexCount; ++i)
	{
		tThisWorld->atMesh[nThisEntity].m_VertexData.push_back(atUIVertices->at(tThisWorld->atLabel[nThisEntity].vIndex)[i].m_d3dfPosition);
	}

	pcGraphicsSystem->CreateEntityBuffer(tThisWorld, nThisEntity);
}

bool CUISystem::CheckIfStringsAreTheSame(char* string1, int& textSize1, const char* string2)
{
	if (string1 == nullptr)
	{
		return false;
	}
	else
	{
		for (int i = 0; i < textSize1; ++i)
		{
			if (string1[i] == string2[i])
			{
				continue;
			}
			else
			{
				return false;
			}
		}

		return true;
	}
}

void CUISystem::CheckOptionsBars(TWorld* tThisWorld, CGraphicsSystem* pcGraphicsSystem, CInputSystem* pcInputSystem, int& nThisEntity, float& m_fMasterVolume, float& m_fMusicVolume, float& m_fSFXVolume, int& masterIndex, int& musicIndex, int& fxIndex, float& currentAmbience, int& brightnessIndex)
{
	if (this->CheckIfStringsAreTheSame(tThisWorld->atBar[nThisEntity].valueToChange, tThisWorld->atBar[nThisEntity].valueToChangeSize, "Sensitivity"))
	{
		pcInputSystem->SetMouseRotationSpeed(tThisWorld->atBar[nThisEntity].ratio * 0.005 + 0.003);
	}
	else if (this->CheckIfStringsAreTheSame(tThisWorld->atBar[nThisEntity].valueToChange, tThisWorld->atBar[nThisEntity].valueToChangeSize, "Master Volume"))
	{
		m_fMasterVolume = tThisWorld->atBar[nThisEntity].ratio * 100;

		/*if (m_fMasterVolume < m_fDialogueVolume)
		{
			m_fDialogueVolume = m_fMasterVolume;
			tThisWorld->atBar[dialogueIndex].ratio = m_fMasterVolume * .01;
		}*/

		if (m_fMasterVolume < m_fMusicVolume)
		{
			m_fMusicVolume = m_fMasterVolume;
			tThisWorld->atBar[musicIndex].ratio = m_fMasterVolume * .01;
		}

		if (m_fMasterVolume < m_fSFXVolume)
		{
			m_fSFXVolume = m_fMasterVolume;
			tThisWorld->atBar[fxIndex].ratio = m_fMasterVolume * .01;
		}
	}
	else if (this->CheckIfStringsAreTheSame(tThisWorld->atBar[nThisEntity].valueToChange, tThisWorld->atBar[nThisEntity].valueToChangeSize, "Music Volume"))
	{
		m_fMusicVolume = tThisWorld->atBar[nThisEntity].ratio * 100;

		if (m_fMusicVolume > m_fMasterVolume)
		{
			m_fMusicVolume = m_fMasterVolume;
			tThisWorld->atBar[musicIndex].ratio = m_fMasterVolume * .01;
		}
	}
	else if (this->CheckIfStringsAreTheSame(tThisWorld->atBar[nThisEntity].valueToChange, tThisWorld->atBar[nThisEntity].valueToChangeSize, "FX Volume"))
	{
		m_fSFXVolume = tThisWorld->atBar[nThisEntity].ratio * 100;

		if (m_fSFXVolume > m_fMasterVolume)
		{
			m_fSFXVolume = m_fMasterVolume;
			tThisWorld->atBar[fxIndex].ratio = m_fMasterVolume * .01;
		}
	}
	else if (this->CheckIfStringsAreTheSame(tThisWorld->atBar[nThisEntity].valueToChange, tThisWorld->atBar[nThisEntity].valueToChangeSize, "Brightness"))
	{
		currentAmbience = (.6 - .3) * tThisWorld->atBar[nThisEntity].ratio + .3;

		pcGraphicsSystem->UpdateAmbience(currentAmbience);
	}
}

void CUISystem::UpdateFPS(TWorld* tThisWorld, CGraphicsSystem* pcGraphicsSystem, CFPS& fpsTimer, int& fpsIndex, CGraphicsSystem::TUIVertexBufferType& tUIVertexBuffer, CGraphicsSystem::TUIPixelBufferType& tUIPixelBuffer, std::vector<TUIVert*>& atUIVertices, TCamera* menuCamera)
{
	float fps = fpsTimer.UpdateFrameTime();

#if HUD
		tUIVertexBuffer.start = -1;
		tUIVertexBuffer.end = -1;
		tUIVertexBuffer.ratio = -1;

		tUIPixelBuffer.hoverColor = tThisWorld->atLabel[fpsIndex].color;

		int textSize = 3;
		wchar_t* textBuffer = new wchar_t[textSize];

		if (fps < 10)
		{
			textBuffer[0] = 0;
			textBuffer[1] = 0;
			textBuffer[2] = (int)fps % 10;
		}
		else if (fps < 100)
		{
			textBuffer[0] = 0;
			textBuffer[1] = (int)fps / 10;
			textBuffer[2] = (int)fps % 10;
		}
		else
		{
			textBuffer[0] = (int)fps / 100;
			textBuffer[1] = ((int)fps / 10) - ((int)fps / 100 * 10);
			textBuffer[2] = (int)fps % 10;
		}

		this->UpdateText(tThisWorld, pcGraphicsSystem, fpsIndex, &atUIVertices, textBuffer, textSize, atUIVertices.at(tThisWorld->atLabel[fpsIndex].vIndex));

		delete[] textBuffer;

		pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld->atMesh[fpsIndex], menuCamera->d3d_Position);
		pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld->atMesh[fpsIndex].m_nIndexCount, tThisWorld->atGraphicsMask[fpsIndex].m_tnGraphicsMask, tThisWorld->atShaderID[fpsIndex].m_nShaderID);
#endif

	fpsTimer.Throttle(60);
}

void CUISystem::ScrollText(TWorld* tThisWorld, CGraphicsSystem* pcGraphicsSystem, int& nThisEntity, std::vector<TUIVert*>* atUIVertices, double delta)
{
	XMFLOAT3* tempPos = new XMFLOAT3[tThisWorld->atMesh[nThisEntity].m_VertexData.size()];

	int counter = 0;
	for (int i = 0; i < tThisWorld->atMesh[nThisEntity].m_VertexData.size(); ++i)
	{
		tempPos[i] = atUIVertices->at(tThisWorld->atLabel[nThisEntity].vIndex)[i].m_d3dfPosition;

		tempPos[i].y += delta;

		atUIVertices->at(tThisWorld->atLabel[nThisEntity].vIndex)[i].m_d3dfPosition.y = tempPos[i].y;

		++counter;
	}

	tThisWorld->atMesh[nThisEntity].m_d3dVertexData.pSysMem = &atUIVertices->at(tThisWorld->atLabel[nThisEntity].vIndex)[0];

	tThisWorld->atMesh[nThisEntity].m_VertexData.clear();
	for (int i = 0; i < counter; ++i)
	{
		tThisWorld->atMesh[nThisEntity].m_VertexData.push_back(atUIVertices->at(tThisWorld->atLabel[nThisEntity].vIndex)[i].m_d3dfPosition);
	}

	pcGraphicsSystem->CreateEntityBuffer(tThisWorld, nThisEntity);
}

void CUISystem::UpdateHUDBars(TWorld * tThisWorld, int & nThisEntity, CGraphicsSystem::TUIVertexBufferType& tUIVertexBuffer, int& screenWidth, int& PlayerStartIndex, int& GunIndexForPlayer, int& CaelisIndex)
{
	if (this->CheckIfStringsAreTheSame(tThisWorld->atBar[nThisEntity].valueToChange, tThisWorld->atBar[nThisEntity].valueToChangeSize, "Health"))
	{
		tUIVertexBuffer.start = (tThisWorld->atBar[nThisEntity].barBoundingBox.left - (screenWidth * .5)) / (screenWidth * .5);
		tUIVertexBuffer.end = (tThisWorld->atBar[nThisEntity].barBoundingBox.right - (screenWidth * .5)) / (screenWidth * .5);
		tUIVertexBuffer.ratio = tThisWorld->atClayton[PlayerStartIndex].health * .01;

		if (tUIVertexBuffer.ratio > .6)
		{
			tThisWorld->atBar[nThisEntity].backgroundColor = XMFLOAT4(0, 1, 0, 1);
		}
		else if (tUIVertexBuffer.ratio > .3)
		{
			tThisWorld->atBar[nThisEntity].backgroundColor = XMFLOAT4(1, .5, 0, 1);
		}
		else
		{
			tThisWorld->atBar[nThisEntity].backgroundColor = XMFLOAT4(1, 0, 0, 1);
		}
	}
	else if (this->CheckIfStringsAreTheSame(tThisWorld->atBar[nThisEntity].valueToChange, tThisWorld->atBar[nThisEntity].valueToChangeSize, "ShootingCooldown"))
	{
		tUIVertexBuffer.start = (tThisWorld->atBar[nThisEntity].barBoundingBox.left - (screenWidth * .5)) / (screenWidth * .5);
		tUIVertexBuffer.end = (tThisWorld->atBar[nThisEntity].barBoundingBox.right - (screenWidth * .5)) / (screenWidth * .5);
		tUIVertexBuffer.ratio = (tThisWorld->atClip[GunIndexForPlayer].fShootingCoolDown) / tThisWorld->atClip[GunIndexForPlayer].FValueOfCoolDown;
	}
	else if (this->CheckIfStringsAreTheSame(tThisWorld->atBar[nThisEntity].valueToChange, tThisWorld->atBar[nThisEntity].valueToChangeSize, "CaelisAbilityCooldown"))
	{
		tUIVertexBuffer.start = (tThisWorld->atBar[nThisEntity].barBoundingBox.left - (screenWidth * .5)) / (screenWidth * .5);
		tUIVertexBuffer.end = (tThisWorld->atBar[nThisEntity].barBoundingBox.right - (screenWidth * .5)) / (screenWidth * .5);
		tUIVertexBuffer.ratio = (100 - tThisWorld->atCaelis[CaelisIndex].m_tfSpecialCooldown) * .01;
	}
	else
	{
		tUIVertexBuffer.start = -1;
		tUIVertexBuffer.end = -1;
		tUIVertexBuffer.ratio = -1;
	}
}

void CUISystem::UpdateDirtyUI(HWND * cApplicationWindow, TWorld * tThisWorld, bool fullscreen)
{
	RECT window;
	GetWindowRect(*cApplicationWindow, &window);

	float screenWidth = window.right - window.left;
	float screenHeight = window.bottom - window.top;

	for (unsigned int nThisEntity = 0; nThisEntity < ENTITYCOUNT; ++nThisEntity)
	{
		float ratioLeftX = (screenWidth / 2) * ((tThisWorld->atLabel[nThisEntity].x - (tThisWorld->atLabel[nThisEntity].width / 2)) * .1);
		float ratioTopY = (screenHeight / 2) * ((tThisWorld->atLabel[nThisEntity].y + (tThisWorld->atLabel[nThisEntity].height / 2)) * .1);
		float ratioRightX = (screenWidth / 2) * ((tThisWorld->atLabel[nThisEntity].x + (tThisWorld->atLabel[nThisEntity].width / 2)) * .1);
		float ratioBottomY = (screenHeight / 2) * ((tThisWorld->atLabel[nThisEntity].y - (tThisWorld->atLabel[nThisEntity].height / 2)) * .1);

		if (tThisWorld->atUIMask[nThisEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON) ||
			tThisWorld->atUIMask[nThisEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON | COMPONENT_PAUSESCREEN) ||
			tThisWorld->atUIMask[nThisEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON | COMPONENT_OPTIONS))
		{
			if (fullscreen)
			{
				tThisWorld->atButton[nThisEntity].boundingBox.bottom = (screenHeight / 2) + (ratioBottomY * -1);
				tThisWorld->atButton[nThisEntity].boundingBox.top = (screenHeight / 2) + (ratioTopY * -1);
				tThisWorld->atButton[nThisEntity].boundingBox.left = (screenWidth / 2) + ratioLeftX;
				tThisWorld->atButton[nThisEntity].boundingBox.right = (screenWidth / 2) + ratioRightX;
			}
			else
			{
				tThisWorld->atButton[nThisEntity].boundingBox.bottom = (screenHeight / 2) + (ratioBottomY * -1) - 17 + tThisWorld->atLabel[nThisEntity].y;
				tThisWorld->atButton[nThisEntity].boundingBox.top = (screenHeight / 2) + (ratioTopY * -1) - 17 + tThisWorld->atLabel[nThisEntity].y;
				tThisWorld->atButton[nThisEntity].boundingBox.left = (screenWidth / 2) + ratioLeftX - 9;
				tThisWorld->atButton[nThisEntity].boundingBox.right = (screenWidth / 2) + ratioRightX - 9;
			}
		}
		else if (tThisWorld->atUIMask[nThisEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BAR) ||
				 tThisWorld->atUIMask[nThisEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BAR | COMPONENT_OPTIONS) ||
				 tThisWorld->atUIMask[nThisEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BAR | COMPONENT_HUD))
		{
			if (fullscreen)
			{
				tThisWorld->atBar[nThisEntity].start.y = (screenHeight / 2) + (ratioTopY * -1);
				tThisWorld->atBar[nThisEntity].end.y = (screenHeight / 2) + (ratioBottomY * -1);
				tThisWorld->atBar[nThisEntity].start.x = (screenWidth / 2) + ratioLeftX - 9;
				tThisWorld->atBar[nThisEntity].end.x = (screenWidth / 2) + ratioRightX - 9;

				tThisWorld->atBar[nThisEntity].barBoundingBox.top = (screenHeight / 2) + (ratioTopY * -1);
				tThisWorld->atBar[nThisEntity].barBoundingBox.bottom = (screenHeight / 2) + (ratioBottomY * -1);
				tThisWorld->atBar[nThisEntity].barBoundingBox.left = (screenWidth / 2) + ratioLeftX - 14;
				tThisWorld->atBar[nThisEntity].barBoundingBox.right = (screenWidth / 2) + ratioRightX - 4;

				ClientToScreen(*cApplicationWindow, &tThisWorld->atBar[nThisEntity].start);
				ClientToScreen(*cApplicationWindow, &tThisWorld->atBar[nThisEntity].end);
			}
			else
			{
				tThisWorld->atBar[nThisEntity].start.y = (screenHeight / 2) + (ratioTopY * -1) - 17 + tThisWorld->atLabel[nThisEntity].y;
				tThisWorld->atBar[nThisEntity].end.y = (screenHeight / 2) + (ratioBottomY * -1) - 17 + tThisWorld->atLabel[nThisEntity].y;
				tThisWorld->atBar[nThisEntity].start.x = (screenWidth / 2) + ratioLeftX - 9;
				tThisWorld->atBar[nThisEntity].end.x = (screenWidth / 2) + ratioRightX - 9;

				tThisWorld->atBar[nThisEntity].barBoundingBox.top = (screenHeight / 2) + (ratioTopY * -1) - 17 + tThisWorld->atLabel[nThisEntity].y;
				tThisWorld->atBar[nThisEntity].barBoundingBox.bottom = (screenHeight / 2) + (ratioBottomY * -1) - 17 + tThisWorld->atLabel[nThisEntity].y;
				tThisWorld->atBar[nThisEntity].barBoundingBox.left = (screenWidth / 2) + ratioLeftX - 14;
				tThisWorld->atBar[nThisEntity].barBoundingBox.right = (screenWidth / 2) + ratioRightX - 4;

				ClientToScreen(*cApplicationWindow, &tThisWorld->atBar[nThisEntity].start);
				ClientToScreen(*cApplicationWindow, &tThisWorld->atBar[nThisEntity].end);
			}
		}
	}
}
