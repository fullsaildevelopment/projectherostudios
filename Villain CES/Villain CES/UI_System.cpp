#include "stdafx.h"
#include "UI_System.h"


CUISystem::CUISystem()
{
}


CUISystem::~CUISystem()
{
}

void CUISystem::DrawMenu(TWorld* ptWorld, CGraphicsSystem* pcGraphicsSystem, XMMATRIX cameraMatrix, XMMATRIX worldMatrix)
{
	
}

void CUISystem::AddTextureToUI(TWorld* tThisWorld, unsigned int nThisEntity, ID3D11Device* device, wchar_t* filepath)
{
	HRESULT result;

	ID3D11Resource* resource;

	result = CreateWICTextureFromFile(device, filepath, &resource, &tThisWorld->atMesh[nThisEntity].m_d3dSRVDiffuse, NULL);

	tThisWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask = tThisWorld->atGraphicsMask[nThisEntity].m_tnGraphicsMask | COMPONENT_TEXTURE;
}

void CUISystem::AddButtonToUI(TWorld* tThisWorld, unsigned int nThisEntity, int sceneIndex, HWND cApplicationWindow)
{
	RECT window;
	GetWindowRect(cApplicationWindow, &window);

	float screenWidth = window.right - window.left;
	float screenHeight = window.bottom - window.top;

	float ratioLeftX = (screenWidth / 2) * ((tThisWorld->atLabel[nThisEntity].x - (tThisWorld->atLabel[nThisEntity].width / 2)) * .1);
	float ratioTopY = (screenHeight / 2) * ((tThisWorld->atLabel[nThisEntity].y + (tThisWorld->atLabel[nThisEntity].height / 2)) * .1);
	float ratioRightX = (screenWidth / 2) * ((tThisWorld->atLabel[nThisEntity].x + (tThisWorld->atLabel[nThisEntity].width / 2)) * .1);
	float ratioBottomY = (screenHeight / 2) * ((tThisWorld->atLabel[nThisEntity].y - (tThisWorld->atLabel[nThisEntity].height / 2)) * .1);

	tThisWorld->atButton[nThisEntity].boundingBox.bottom = (screenHeight / 2) + (ratioBottomY * -1) - 17 + tThisWorld->atLabel[nThisEntity].y;
	tThisWorld->atButton[nThisEntity].boundingBox.top = (screenHeight / 2) + (ratioTopY * -1) - 17 + tThisWorld->atLabel[nThisEntity].y;
	tThisWorld->atButton[nThisEntity].boundingBox.left = (screenWidth / 2) + ratioLeftX - 7 - tThisWorld->atLabel[nThisEntity].x;
	tThisWorld->atButton[nThisEntity].boundingBox.right = (screenWidth / 2) + ratioRightX - 7 - tThisWorld->atLabel[nThisEntity].x;

	tThisWorld->atButton[nThisEntity].sceneIndex = sceneIndex;

	tThisWorld->atUIMask[nThisEntity].m_tnUIMask = tThisWorld->atUIMask[nThisEntity].m_tnUIMask | COMPONENT_BUTTON;
}

void CUISystem::AddTextToUI(TWorld* tThisWorld, unsigned int nThisEntity, HWND cApplicationWindow, wchar_t* text, unsigned int textSize, int* textColor)
{
	tThisWorld->atText[nThisEntity].textBuffer = new wchar_t[textSize]; 

	for (int i = 0; i < textSize; ++i)
	{
		tThisWorld->atText[nThisEntity].textBuffer[i] = text[i];
	}

	tThisWorld->atText[nThisEntity].textSize = textSize - 1;

	HDC tHDC = GetDC(0);

	SIZE tempSize;
	GetTextExtentPoint32(tHDC, tThisWorld->atText[nThisEntity].textBuffer, tThisWorld->atText[nThisEntity].textSize, &tempSize);

	ReleaseDC(cApplicationWindow, tHDC);

	POINT tempPoint = { tThisWorld->atButton[nThisEntity].boundingBox.left, tThisWorld->atButton[nThisEntity].boundingBox.top };
	POINT tempPoint2 = { tThisWorld->atButton[nThisEntity].boundingBox.right, tThisWorld->atButton[nThisEntity].boundingBox.bottom };

	bool worked = ClientToScreen(cApplicationWindow, &tempPoint);
	worked = ClientToScreen(cApplicationWindow, &tempPoint2);

	int difference = (tempSize.cx * 3) - (tempPoint2.x - tempPoint.x);
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
		tThisWorld->atText[nThisEntity].textBoundingBox.left = tempPoint.x + (abs(difference) / 2);
		tThisWorld->atText[nThisEntity].textBoundingBox.top = tempPoint.y;
		tThisWorld->atText[nThisEntity].textBoundingBox.right = tempPoint2.x - (abs(difference) / 2);;
		tThisWorld->atText[nThisEntity].textBoundingBox.bottom = tempPoint2.y;

		tThisWorld->atButton[nThisEntity].boundingBox.left += (abs(difference) / 2);
		tThisWorld->atButton[nThisEntity].boundingBox.right -= (abs(difference) / 2);
	}

	tThisWorld->atText[nThisEntity].textColor[0] = textColor[0];
	tThisWorld->atText[nThisEntity].textColor[1] = textColor[1];
	tThisWorld->atText[nThisEntity].textColor[2] = textColor[2];

	tThisWorld->atUIMask[nThisEntity].m_tnUIMask = tThisWorld->atUIMask[nThisEntity].m_tnUIMask | COMPONENT_TEXT;
}