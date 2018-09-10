#include "stdafx.h"
#include "UI_System.h"


CUISystem::CUISystem()
{
}


CUISystem::~CUISystem()
{
}

void CUISystem::AddTextureToUI(TWorld* tThisWorld, unsigned int nThisEntity, ID3D11Device* device, wchar_t* filepath)
{
	HRESULT result;

	ID3D11Resource* resource;

	result = CreateWICTextureFromFile(device, filepath, &resource, &tThisWorld->atMesh[nThisEntity].m_d3dSRVDiffuse, NULL);

	tThisWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = tThisWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask | COMPONENT_TEXTURE;
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

	/*HDC tHDC = GetDC(0);
	HGDIOBJ oldFont = SelectObject(tHDC, (HGDIOBJ)myFont);

	SIZE tempSize;
	GetTextExtentPoint32(tHDC, tThisWorld->atText[nThisEntity].textBuffer, tThisWorld->atText[nThisEntity].textSize, &tempSize);

	int height = DrawText(tHDC, tThisWorld->atText[nThisEntity].textBuffer, tThisWorld->atText[nThisEntity].textSize, &tThisWorld->atText[nThisEntity].textBoundingBox, DT_CALCRECT);

	POINT tempPoint = { tThisWorld->atButton[nThisEntity].boundingBox.left, tThisWorld->atButton[nThisEntity].boundingBox.top };
	POINT tempPoint2 = { tThisWorld->atButton[nThisEntity].boundingBox.right, tThisWorld->atButton[nThisEntity].boundingBox.bottom };

	bool worked = ClientToScreen(*cApplicationWindow, &tempPoint);
	worked = ClientToScreen(*cApplicationWindow, &tempPoint2);

	int difference = (tempSize.cx) - (tempPoint2.x - tempPoint.x);
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

	difference = (height) - (tempPoint2.y - tempPoint.y);
	if (difference > 0)
	{
		tThisWorld->atText[nThisEntity].textBoundingBox.bottom += (difference);

		tThisWorld->atButton[nThisEntity].boundingBox.bottom += (difference);
	}
	else
	{
		tThisWorld->atText[nThisEntity].textBoundingBox.bottom += (difference);

		tThisWorld->atButton[nThisEntity].boundingBox.bottom += (difference);
	}*/

	tThisWorld->atUIMask[nThisEntity].m_tnUIMask = tThisWorld->atUIMask[nThisEntity].m_tnUIMask | COMPONENT_TEXT;

	/*SelectObject(tHDC, oldFont);
	ReleaseDC(*cApplicationWindow, tHDC);*/
}

void CUISystem::AddMaskToUI(TWorld* tThisWorld, unsigned int nThisEntity, eUIComponent mask)
{
	tThisWorld->atUIMask[nThisEntity].m_tnUIMask = tThisWorld->atUIMask[nThisEntity].m_tnUIMask | mask;
}

void CUISystem::AdjustBoundingBox(HWND* cApplicationWindow, TWorld* tThisWorld, unsigned int nThisEntity)
{
	HDC tHDC = GetDC(0);
	HGDIOBJ oldFont = SelectObject(tHDC, (HGDIOBJ)myFont);

	SIZE tempSize;
	GetTextExtentPoint32(tHDC, tThisWorld->atText[nThisEntity].textBuffer, tThisWorld->atText[nThisEntity].textSize, &tempSize);

	int height = DrawText(tHDC, tThisWorld->atText[nThisEntity].textBuffer, tThisWorld->atText[nThisEntity].textSize, &tThisWorld->atText[nThisEntity].textBoundingBox, DT_CALCRECT);

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
			//tThisWorld->atButton[nThisEntity].boundingBox.right += (difference / 2);
		}
		else
		{
			tThisWorld->atText[nThisEntity].textBoundingBox.left = tempPoint.x + ((abs(difference)));
			tThisWorld->atText[nThisEntity].textBoundingBox.top = tempPoint.y;
			tThisWorld->atText[nThisEntity].textBoundingBox.right = tempPoint2.x;
			tThisWorld->atText[nThisEntity].textBoundingBox.bottom = tempPoint2.y;

			tThisWorld->atButton[nThisEntity].boundingBox.left += (abs(difference));
			//tThisWorld->atButton[nThisEntity].boundingBox.right -= (abs(difference) / 2);
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

			//tThisWorld->atButton[nThisEntity].boundingBox.left -= (difference / 2);
			tThisWorld->atButton[nThisEntity].boundingBox.right += (difference);
		}
		else
		{
			tThisWorld->atText[nThisEntity].textBoundingBox.left = tempPoint.x;
			tThisWorld->atText[nThisEntity].textBoundingBox.top = tempPoint.y;
			tThisWorld->atText[nThisEntity].textBoundingBox.right = tempPoint2.x - ((abs(difference)));
			tThisWorld->atText[nThisEntity].textBoundingBox.bottom = tempPoint2.y;

			//tThisWorld->atButton[nThisEntity].boundingBox.left += (abs(difference) / 2);
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

	SelectObject(tHDC, oldFont);
	ReleaseDC(*cApplicationWindow, tHDC);
}

void CUISystem::AddBarToUI(HWND* cApplicationWindow, TWorld* tThisWorld, unsigned int nThisEntity)
{
	RECT window;
	GetWindowRect(*cApplicationWindow, &window);

	float screenWidth = window.right - window.left;
	float screenHeight = window.bottom - window.top;

	float ratioLeftX = (screenWidth / 2) * ((tThisWorld->atLabel[nThisEntity].x - (tThisWorld->atLabel[nThisEntity].width / 2)) * .1);
	float ratioTopY = (screenHeight / 2) * ((tThisWorld->atLabel[nThisEntity].y + (tThisWorld->atLabel[nThisEntity].height / 2)) * .1);
	float ratioRightX = (screenWidth / 2) * ((tThisWorld->atLabel[nThisEntity].x + (tThisWorld->atLabel[nThisEntity].width / 2)) * .1);
	float ratioBottomY = (screenHeight / 2) * ((tThisWorld->atLabel[nThisEntity].y - (tThisWorld->atLabel[nThisEntity].height / 2)) * .1);

	//tThisWorld->atBar[nThisEntity].start = new POINT{ (screenWidth / 2) + ratioLeftX - 7 - tThisWorld->atLabel[nThisEntity].x , (screenHeight / 2) + (ratioBottomY * -1) - 17 + tThisWorld->atLabel[nThisEntity].y };
	//tThisWorld->atBar[nThisEntity].end = new POINT{ (screenWidth / 2) + ratioRightX - 7 - tThisWorld->atLabel[nThisEntity].x , (screenHeight / 2) + (ratioTopY * -1) - 17 + tThisWorld->atLabel[nThisEntity].y };
	//POINT start, end;
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

	//tThisWorld->atBar[nThisEntity].barBoundingBox.top = start.y;
	//tThisWorld->atBar[nThisEntity].barBoundingBox.bottom = end.y;
	//tThisWorld->atBar[nThisEntity].barBoundingBox.left = start.x;
	//tThisWorld->atBar[nThisEntity].barBoundingBox.right = end.x;

	tThisWorld->atBar[nThisEntity].ratio = 1;

	tThisWorld->atUIMask[nThisEntity].m_tnUIMask = tThisWorld->atUIMask[nThisEntity].m_tnUIMask | COMPONENT_BAR;
}

void CUISystem::CreateEnemyHealthBar(HWND* cApplicationWindow, TWorld * tThisWorld, unsigned int &nThisEntity, XMFLOAT4 in_EnemyPos)
{
	RECT window;
	GetWindowRect(*cApplicationWindow, &window);

	float screenWidth = window.right - window.left;
	float screenHeight = window.bottom - window.top;

	float ratioLeftX = (screenWidth / 2) * ((tThisWorld->atLabel[nThisEntity].x - (tThisWorld->atLabel[nThisEntity].width / 2)) * .1);
	float ratioTopY = (screenHeight / 2) * ((tThisWorld->atLabel[nThisEntity].y + (tThisWorld->atLabel[nThisEntity].height / 2)) * .1);
	float ratioRightX = (screenWidth / 2) * ((tThisWorld->atLabel[nThisEntity].x + (tThisWorld->atLabel[nThisEntity].width / 2)) * .1);
	float ratioBottomY = (screenHeight / 2) * ((tThisWorld->atLabel[nThisEntity].y - (tThisWorld->atLabel[nThisEntity].height / 2)) * .1);

	//tThisWorld->atBar[nThisEntity].start.x = (screenWidth / 2) * 0.1;
	//tThisWorld->atBar[nThisEntity].start.y = (screenHeight / 2) * 0.1;

	//

	//ClientToScreen(*cApplicationWindow, &tThisWorld->atBar[nThisEntity].start);
	//ClientToScreen(*cApplicationWindow, &tThisWorld->atBar[nThisEntity].end);


	//tThisWorld->atBar[nThisEntity].ratio = 1.0f / tThisWorld->atAiHeath[nThisEntity].heath;
	//tThisWorld->atBar[nThisEntity].backgroundColor = XMFLOAT4(1,0,0,1);

	tThisWorld->atUIMask[nThisEntity].m_tnUIMask = tThisWorld->atUIMask[nThisEntity].m_tnUIMask | COMPONENT_BAR;

	nThisEntity = createGSQuad(tThisWorld, tThisWorld->atBar[nThisEntity].backgroundColor);
}

void CUISystem::UpdateEnemyHealthBar(TWorld * tThisWorld, unsigned int nThisEntity, XMFLOAT4 &out_EnemyPos)
{
	
}

float CUISystem::GetEnemyHealth(TWorld * tThisWorld, unsigned int nThisEntity)
{
	return tThisWorld->atAiHeath[nThisEntity].heath;
}


