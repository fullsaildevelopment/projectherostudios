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

	tThisWorld->atBar[nThisEntity].ratio = 1;

	tThisWorld->atUIMask[nThisEntity].m_tnUIMask = tThisWorld->atUIMask[nThisEntity].m_tnUIMask | COMPONENT_BAR;
}

void CUISystem::GetUVsForCharacter(wchar_t* character, XMFLOAT2* UVs)
{
	delete[] UVs;

	UVs = new XMFLOAT2[4];

	switch (*character)
	{
	case ' ':
	{
		UVs[0] = XMFLOAT2{ 0 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 10 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 0 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 10 / 950.0f, 14 / 20.0f };
	}
	break;
	case ':':
	{
		UVs[0] = XMFLOAT2{ 260 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 270 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 260 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 270 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'A':
	{
		UVs[0] = XMFLOAT2{ 330 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 340 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 330 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 340 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'B':
	{
		UVs[0] = XMFLOAT2{ 340 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 350 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 340 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 350 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'C':
	{
		UVs[0] = XMFLOAT2{ 350 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 360 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 350 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 360 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'D':
	{
		UVs[0] = XMFLOAT2{ 360 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 370 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 360 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 370 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'E':
	{
		UVs[0] = XMFLOAT2{ 370 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 380 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 370 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 380 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'F':
	{
		UVs[0] = XMFLOAT2{ 380 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 390 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 380 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 390 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'G':
	{
		UVs[0] = XMFLOAT2{ 390 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 400 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 390 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 400 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'H':
	{
		UVs[0] = XMFLOAT2{ 400 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 410 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 400 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 410 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'I':
	{
		UVs[0] = XMFLOAT2{ 410 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 420 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 410 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 420 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'J':
	{
		UVs[0] = XMFLOAT2{ 420 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 430 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 420 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 430 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'K':
	{
		UVs[0] = XMFLOAT2{ 430 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 440 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 430 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 440 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'L':
	{
		UVs[0] = XMFLOAT2{ 440 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 450 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 440 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 450 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'M':
	{
		UVs[0] = XMFLOAT2{ 450 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 460 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 450 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 460 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'N':
	{
		UVs[0] = XMFLOAT2{ 460 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 470 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 460 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 470 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'O':
	{
		UVs[0] = XMFLOAT2{ 470 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 480 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 470 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 480 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'P':
	{
		UVs[0] = XMFLOAT2{ 480 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 490 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 480 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 490 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'Q':
	{
		UVs[0] = XMFLOAT2{ 490 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 500 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 490 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 500 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'R':
	{
		UVs[0] = XMFLOAT2{ 500 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 510 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 500 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 510 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'S':
	{
		UVs[0] = XMFLOAT2{ 510 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 520 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 510 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 520 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'T':
	{
		UVs[0] = XMFLOAT2{ 520 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 530 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 520 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 530 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'U':
	{
		UVs[0] = XMFLOAT2{ 530 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 540 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 530 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 540 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'V':
	{
		UVs[0] = XMFLOAT2{ 540 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 550 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 540 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 550 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'W':
	{
		UVs[0] = XMFLOAT2{ 550 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 560 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 550 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 560 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'X':
	{
		UVs[0] = XMFLOAT2{ 560 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 570 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 560 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 570 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'Y':
	{
		UVs[0] = XMFLOAT2{ 570 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 580 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 570 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 580 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'Z':
	{
		UVs[0] = XMFLOAT2{ 580 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 590 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 580 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 590 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'a':
	{
		UVs[0] = XMFLOAT2{ 650 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 660 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 650 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 660 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'b':
	{
		UVs[0] = XMFLOAT2{ 660 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 670 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 660 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 670 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'c':
	{
		UVs[0] = XMFLOAT2{ 670 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 680 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 670 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 680 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'd':
	{
		UVs[0] = XMFLOAT2{ 680 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 690 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 680 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 690 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'e':
	{
		UVs[0] = XMFLOAT2{ 690 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 700 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 690 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 700 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'f':
	{
		UVs[0] = XMFLOAT2{ 700 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 710 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 700 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 710 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'g':
	{
		UVs[0] = XMFLOAT2{ 710 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 720 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 710 / 950.0f, 16 / 20.0f };
		UVs[3] = XMFLOAT2{ 720 / 950.0f, 16 / 20.0f };
	}
	break;
	case 'h':
	{
		UVs[0] = XMFLOAT2{ 720 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 730 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 720 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 730 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'i':
	{
		UVs[0] = XMFLOAT2{ 730 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 740 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 730 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 740 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'j':
	{
		UVs[0] = XMFLOAT2{ 740 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 750 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 740 / 950.0f, 16 / 20.0f };
		UVs[3] = XMFLOAT2{ 750 / 950.0f, 16 / 20.0f };
	}
	break;
	case 'k':
	{
		UVs[0] = XMFLOAT2{ 750 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 760 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 750 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 760 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'l':
	{
		UVs[0] = XMFLOAT2{ 760 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 770 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 760 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 770 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'm':
	{
		UVs[0] = XMFLOAT2{ 770 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 780 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 770 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 780 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'n':
	{
		UVs[0] = XMFLOAT2{ 780 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 790 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 780 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 790 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'o':
	{
		UVs[0] = XMFLOAT2{ 790 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 800 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 790 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 800 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'p':
	{
		UVs[0] = XMFLOAT2{ 800 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 810 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 800 / 950.0f, 16 / 20.0f };
		UVs[3] = XMFLOAT2{ 810 / 950.0f, 16 / 20.0f };
	}
	break;
	case 'q':
	{
		UVs[0] = XMFLOAT2{ 810 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 820 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 810 / 950.0f, 16 / 20.0f };
		UVs[3] = XMFLOAT2{ 820 / 950.0f, 16 / 20.0f };
	}
	break;
	case 'r':
	{
		UVs[0] = XMFLOAT2{ 820 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 830 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 820 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 830 / 950.0f, 14 / 20.0f };
	}
	break;
	case 's':
	{
		UVs[0] = XMFLOAT2{ 830 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 840 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 830 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 840 / 950.0f, 14 / 20.0f };
	}
	break;
	case 't':
	{
		UVs[0] = XMFLOAT2{ 840 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 850 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 840 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 850 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'u':
	{
		UVs[0] = XMFLOAT2{ 850 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 860 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 850 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 860 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'v':
	{
		UVs[0] = XMFLOAT2{ 860 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 870 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 860 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 870 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'w':
	{
		UVs[0] = XMFLOAT2{ 870 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 880 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 870 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 880 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'x':
	{
		UVs[0] = XMFLOAT2{ 880 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 890 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 880 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 890 / 950.0f, 14 / 20.0f };
	}
	break;
	case 'y':
	{
		UVs[0] = XMFLOAT2{ 890 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 900 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 890 / 950.0f, 16 / 20.0f };
		UVs[3] = XMFLOAT2{ 900 / 950.0f, 16 / 20.0f };
	}
	break;
	case 'z':
	{
		UVs[0] = XMFLOAT2{ 900 / 950.0f, 4 / 20.0f };
		UVs[1] = XMFLOAT2{ 910 / 950.0f, 4 / 20.0f };
		UVs[2] = XMFLOAT2{ 900 / 950.0f, 14 / 20.0f };
		UVs[3] = XMFLOAT2{ 910 / 950.0f, 14 / 20.0f };
	}
	break;
	default:
		UVs[0] = XMFLOAT2{ -1, -1 };
		UVs[1] = XMFLOAT2{ -1, -1 };
		UVs[2] = XMFLOAT2{ -1, -1 };
		UVs[3] = XMFLOAT2{ -1, -1 };
		break;
	}
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


