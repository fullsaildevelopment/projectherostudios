#include "stdafx.h"
#include "UI_System.h"


CUISystem::CUISystem()
{
}


CUISystem::~CUISystem()
{
}

void CUISystem::AddTextureToUI(TWorld* tThisWorld, unsigned int nThisEntity, ID3D11Device* device, wchar_t* filepath, ID3D11ShaderResourceView* srvDiffuse)
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

void CUISystem::AddButtonToUI(HWND* cApplicationWindow, TWorld* tThisWorld, unsigned int nThisEntity, int sceneIndex, bool enabled)
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

	tThisWorld->atButton[nThisEntity].sceneIndex = sceneIndex;

	tThisWorld->atButton[nThisEntity].enabled = enabled;

	tThisWorld->atUIMask[nThisEntity].m_tnUIMask = tThisWorld->atUIMask[nThisEntity].m_tnUIMask | COMPONENT_BUTTON;
}

void CUISystem::AddTextToUI(HWND* cApplicationWindow, TWorld* tThisWorld, unsigned int nThisEntity, wchar_t* text, unsigned int textSize, int* textColor, int justification)
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

void CUISystem::AddMaskToUI(TWorld* tThisWorld, unsigned int nThisEntity, eUIComponent mask)
{
	tThisWorld->atUIMask[nThisEntity].m_tnUIMask = tThisWorld->atUIMask[nThisEntity].m_tnUIMask | mask;
}

void CUISystem::AdjustBoundingBox(HWND* cApplicationWindow, TWorld* tThisWorld, unsigned int nThisEntity)
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

void CUISystem::AddBarToUI(HWND* cApplicationWindow, TWorld* tThisWorld, unsigned int nThisEntity, XMFLOAT4* backgroundColor, char* function, unsigned int textSize, float ratio)
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

	tThisWorld->atBar[nThisEntity].ratio = ratio;

	if (backgroundColor != nullptr)
		tThisWorld->atBar[nThisEntity].backgroundColor = *backgroundColor;

	tThisWorld->atBar[nThisEntity].valueToChangeSize = textSize;

	tThisWorld->atUIMask[nThisEntity].m_tnUIMask = tThisWorld->atUIMask[nThisEntity].m_tnUIMask | COMPONENT_BAR;
}

void CUISystem::UpdateText(TWorld* tThisWorld, unsigned int nThisEntity, std::vector<TUIVert*>* atUIVertices, wchar_t* character, int textSize, TUIVert* UVs)
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

}

bool CUISystem::CheckIfStringsAreTheSame(char* string1, unsigned int textSize1, const char* string2)
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