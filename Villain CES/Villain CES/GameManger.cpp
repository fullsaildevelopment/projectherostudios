#include "GameManger.h"
#define AI_ON true
#define MIKES_SANDBOX_ON false
#define SKELETON_LOAD_ON false
#define MAIN_LEVEL_ON true
#define MUSIC_ON true
//Don't forget to comment out PlaySoundInBank() call in VillCES.cpp if MUSIC_ON is False - ZFB
CGameMangerSystem::CGameMangerSystem(HWND window, CInputSystem* _pcInputSystem)
{
	cApplicationWindow = window;
	pcGraphicsSystem = new CGraphicsSystem();
	pcInputSystem = _pcInputSystem;
	pcInputSystem->SendHWNDToInputSystem(cApplicationWindow);
	pcCollisionSystem = new CCollisionSystem();
	pcPhysicsSystem = new CPhysicsSystem();
	pcProjectileSystem = new CProjectileSystem();
	pcAiSystem = new CAISystem();
	pcUISystem = new CUISystem();
	pcGraphicsSystem->InitD3D(cApplicationWindow);
	tAugerTimers = new System_Times();
	tTimerInfo = new System_Times();
	aimCamera = new TCamera();
	walkCamera = new TCamera();
	debugCamera = new TCamera();
	menuCamera = new TCamera();
#if MUSIC_ON
	pcAudioSystem = new CAudioSystem();
#endif
	//srand(time(NULL));

	GetWindowRect(cApplicationWindow, &windowRect);

	screenWidth = windowRect.right - windowRect.left;
	screenHeight = windowRect.bottom - windowRect.top;
}

CGameMangerSystem::~CGameMangerSystem()
{
	pcGraphicsSystem->CleanD3D(&tThisWorld);
#if MUSIC_ON
	//Terminate Audio System Function
	pcAudioSystem->TermSoundEngine();
	delete pcAudioSystem;
#endif
	delete pcGraphicsSystem;
	delete pcInputSystem;
	delete pcCollisionSystem;
	delete pcPhysicsSystem;
	delete pcProjectileSystem;
	delete pcAiSystem;
	//delete tAugerTimers;
	delete tTimerInfo;
	delete aimCamera;
	delete debugCamera;
	delete walkCamera;
	delete menuCamera;
	delete pcUISystem;

	fontTexture->Release();
	atUIVertices.clear();
	atUIIndices.clear();
}

void CGameMangerSystem::InitializeMainMenu()
{ // Music Stuff 
#if MUSIC_ON
	AK::SoundEngine::StopAll();
	pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_MAIN_MENU_MUSIC, m_AkMainMenuMusic);
#endif
	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);
	atUIVertices.clear();
	atUIIndices.clear();

	options = false;
	credits = false;

	if (fontTexture == nullptr)
	{
		ID3D11Resource* resource;

		wchar_t filepath[] =
		{ L"UI_Textures.fbm/FontB.png" };

		CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, filepath, &resource, &fontTexture, NULL);
	}

	unsigned int nThisEntity;
	{
		wchar_t wideChar[] =
		{ L"UI_Textures.fbm/Auger_MainMenu.png" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 20, 20, 0, 0, &atUIVertices, -1, .2);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, wideChar);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 1);
	}
	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2.3, 1, .5, 0, &atUIVertices, -1, .15);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 1);
	}
	{
		wchar_t textBuffer[] =
		{ L"NEW GAME" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.2, 1, .5, 0, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		//                                                  modify this to switch testing levels in Augur.cpp

		//pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 4, true);

#if MIKES_SANDBOX_ON
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 9, true);

#endif
#if SKELETON_LOAD_ON
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 7, true);

#endif
#if MAIN_LEVEL_ON
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 13, true);

#endif

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}
	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2.4, 1, .51, -1.2, &atUIVertices, -1, .15);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 1);
	}
	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2.1, 1, .5, -2.4, &atUIVertices, -1, .15);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 1);
	}
	{
		wchar_t textBuffer[] =
		{ L"OPTIONS" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2, 1, .5, -2.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 98, true);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}
	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2.1, 1, .5, -3.6, &atUIVertices, -1, .15);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 1);
	}
	{
		wchar_t textBuffer[] =
		{ L"CREDITS" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2, 1, .5, -3.6, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 99, true);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}
	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 1.6, 1, .5, -4.8, &atUIVertices, -1, .15);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 1);
	}
	{
		wchar_t textBuffer[] =
		{ L"EXIT" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 1.5, 1, .5, -4.8, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, -1, true);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	//{
	//	wchar_t textBuffer[] =
	//	{ L"Alteris: “Dark Matter Gates possess the unique ability \nto open passages to their twins when a sufficient \ncharge is run through them both, this passage allows for \ninstantaneous transport between the two gates.”" };
	//
	//	nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 15, 6, 0, -7, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, .1);
	//	pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
	//	//pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, -1, true);
	//
	//	tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	//}

	InitializeOptionsMenu();
	InitializeCredits();

	pcGraphicsSystem->CreateBuffers(&tThisWorld);
}

int CGameMangerSystem::LoadMainMenu()
{
	fpsTimer.Xtime_Signal();

	//////////
	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();
	//////////
	m_RealTimeFov = pcInputSystem->ZoomSight(m_RealTimeFov);
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective(m_RealTimeFov);

	CGraphicsSystem::TUIVertexBufferType tTempVertexBuffer;
	CGraphicsSystem::TUIPixelBufferType tTempPixelBuffer;

	hoverPoint = { -1, -1 };
	//POINT hoverPoint;
	GetCursorPos(&hoverPoint);
	ScreenToClient(cApplicationWindow, &hoverPoint);

	//POINT
	clickPoint = { -1, -1 };
	if (pcInputSystem->InputCheck(G_BUTTON_LEFT) == 1 && mouseUp)
	{
		GetCursorPos(&startDragPoint);
		ScreenToClient(cApplicationWindow, &startDragPoint);

		GetCursorPos(&dragPoint);
		ScreenToClient(cApplicationWindow, &dragPoint);

		mouseUp = false;
		mouseDown = true;
	}
	else if (pcInputSystem->InputCheck(G_BUTTON_LEFT) == 1 && mouseDown)
	{
		GetCursorPos(&dragPoint);
		ScreenToClient(cApplicationWindow, &dragPoint);
	}
	else if (pcInputSystem->InputCheck(G_BUTTON_LEFT) == 0 && mouseDown)
	{
		GetCursorPos(&clickPoint);
		ScreenToClient(cApplicationWindow, &clickPoint);

		mouseUp = true;
		mouseDown = false;

		click = true;
	}

	pcGraphicsSystem->UpdateD3D();

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if (!options && !credits)
		{
			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL))
			{
				tTempVertexBuffer.start = -1;
				tTempVertexBuffer.end = -1;
				tTempVertexBuffer.ratio = -1;
				tTempVertexBuffer.padding = -1;

				tTempPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}

			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON))
			{
				tTempVertexBuffer.start = -1;
				tTempVertexBuffer.end = -1;
				tTempVertexBuffer.ratio = -1;

				tTempPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

				if (tThisWorld.atButton[nCurrentEntity].enabled && clickTime > 0.2)
				{
					if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
					{
						clickTime = 0;

						if (tThisWorld.atButton[nCurrentEntity].sceneIndex < 96)
						{
							atUIVertices.clear();
							atUIIndices.clear();

							return tThisWorld.atButton[nCurrentEntity].sceneIndex;
						}
						else if (tThisWorld.atButton[nCurrentEntity].sceneIndex == 98)
						{
							options = true;
						}
						else if (tThisWorld.atButton[nCurrentEntity].sceneIndex == 99)
						{
							credits = true;
						}
					}
					else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
					{
						tTempPixelBuffer.hoverColor = XMFLOAT4(1, .6, .6, 0);
					}

					pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
					pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
				}
			}
		}
		else if (options && !credits)
		{
			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON | COMPONENT_OPTIONS))
			{
				tTempVertexBuffer.start = -1;
				tTempVertexBuffer.end = -1;
				tTempVertexBuffer.ratio = -1;
				tTempVertexBuffer.padding = -1;

				tTempPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

				if (tThisWorld.atButton[nCurrentEntity].enabled && clickTime > 0.2)
				{
					if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
					{
						clickTime = 0;

						if (tThisWorld.atButton[nCurrentEntity].sceneIndex == 3)
						{
							options = false;
						}
					}
					else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
					{
						tTempPixelBuffer.hoverColor = XMFLOAT4(1, .6, .6, 0);
					}
				}

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}

			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_OPTIONS))
			{
				tTempVertexBuffer.start = -1;
				tTempVertexBuffer.end = -1;
				tTempVertexBuffer.ratio = -1;

				tTempPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}

			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BAR | COMPONENT_OPTIONS))
			{
				if (PtInRect(&tThisWorld.atBar[nCurrentEntity].barBoundingBox, clickPoint))
				{
					tThisWorld.atBar[nCurrentEntity].ratio = (clickPoint.x - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left - 5.0) / (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left - 10);
				
					if (pcUISystem->CheckIfStringsAreTheSame(tThisWorld.atBar[nCurrentEntity].valueToChange, tThisWorld.atBar[nCurrentEntity].valueToChangeSize, "Sensitivity"))
					{
						pcInputSystem->SetMouseRotationSpeed(tThisWorld.atBar[nCurrentEntity].ratio * 0.005 + 0.003);
					}
				}
				else if (PtInRect(&tThisWorld.atBar[nCurrentEntity].barBoundingBox, dragPoint))
				{
					tThisWorld.atBar[nCurrentEntity].ratio = (dragPoint.x - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left - 5.0) / (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left - 10);
				
					if (pcUISystem->CheckIfStringsAreTheSame(tThisWorld.atBar[nCurrentEntity].valueToChange, tThisWorld.atBar[nCurrentEntity].valueToChangeSize, "Sensitivity"))
					{
						pcInputSystem->SetMouseRotationSpeed(tThisWorld.atBar[nCurrentEntity].ratio * 0.005 + 0.003);
					}
				}

				if (tThisWorld.atBar[nCurrentEntity].backgroundColor.x == 1 &&
					tThisWorld.atBar[nCurrentEntity].backgroundColor.y == 0 &&
					tThisWorld.atBar[nCurrentEntity].backgroundColor.z == 0 &&
					tThisWorld.atBar[nCurrentEntity].backgroundColor.w == 1)
				{
					tTempVertexBuffer.start = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.left + 14 - (screenWidth / 2.0)) / (screenWidth / 2);
					tTempVertexBuffer.end = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right + 4 - (screenWidth / 2.0)) / (screenWidth / 2);
					tTempVertexBuffer.ratio = tThisWorld.atBar[nCurrentEntity].ratio;
				}
				else
				{
					tTempVertexBuffer.start = -1;
					tTempVertexBuffer.end = -1;
					tTempVertexBuffer.ratio = -1;
				}

				tTempPixelBuffer.hoverColor = tThisWorld.atBar[nCurrentEntity].backgroundColor;

				tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse = nullptr;

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}
		}
		else if (!options && credits)
		{
			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_CREDITS))
			{
				tTempVertexBuffer.start = -1;
				tTempVertexBuffer.end = -1;
				tTempVertexBuffer.ratio = -1;

				tTempPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}

			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON | COMPONENT_CREDITS))
			{
				tTempVertexBuffer.start = -1;
				tTempVertexBuffer.end = -1;
				tTempVertexBuffer.ratio = -1;
				tTempVertexBuffer.padding = -1;

				tTempPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

				if (tThisWorld.atButton[nCurrentEntity].enabled && clickTime > 0.2)
				{
					if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
					{
						clickTime = 0;

						if (tThisWorld.atButton[nCurrentEntity].sceneIndex == 3)
						{
							credits = false;
						}
					}
					else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
					{
						tTempPixelBuffer.hoverColor = XMFLOAT4(1, .6, .6, 0);
					}
				}

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}
		}
	}
	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);

	if (pcInputSystem->InputCheck(G_KEY_ESCAPE) == 1)
	{
		return -1;
	}

	clickTime += fpsTimer.GetDelta();

#if MIKES_SANDBOX_ON
	return 9;
#endif
	return 3;
}

void CGameMangerSystem::InitializeTitleScreen()
{
#if MUSIC_ON
	pcAudioSystem->IntiializeSystem(ErrorResult);
	pcAudioSystem->SetBanksFolderPath(AKTEXT("../Villain CES/WwiseSounds/Windows"));
	pcAudioSystem->RegisterGameObj(Listener);
	pcAudioSystem->RegisterGameObj(m_AkMainMenuMusic);
	pcAudioSystem->RegisterGameObj(m_AkHallwayBattle);
	pcAudioSystem->RegisterGameObj(m_MetalReload);
	pcAudioSystem->RegisterGameObj(m_AkMetalFired);
	pcAudioSystem->LoadBankFile(INIT_BNK, init_bnkID, ErrorResult);
	pcAudioSystem->LoadBankFile(MAINMENU_BNK, MainMenu_bnkID, ErrorResult);
	pcAudioSystem->LoadBankFile(SFX, m_SFX_bnkID, ErrorResult);
#endif 
	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);
	atUIVertices.clear();
	atUIIndices.clear();

	unsigned int nThisEntity;

	{
		wchar_t wideChar[] =
		{ L"UI_Textures.fbm/Project_Hero_Studios_Logo.png" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 20, 20, 0, 0, &atUIVertices, -1, .2);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, wideChar);
	}

	{
		wchar_t wideChar[] =
		{ L"UI_Textures.fbm/transparentSquareB.png" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 20, 20, 0, 0, &atUIVertices, -1, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, wideChar);
	}

	pcGraphicsSystem->CreateBuffers(&tThisWorld);

	fadeTime = 0;
}

int CGameMangerSystem::LoadTitleScreen()
{
	fpsTimer.Xtime_Signal();

#if MUSIC_ON
	pcAudioSystem->SetListener(Listener, 1, ErrorResult);
#endif
	//////////
	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();
	//////////

	CGraphicsSystem::TUIVertexBufferType tTempVertexBuffer;
	CGraphicsSystem::TUIPixelBufferType tTempPixelBuffer;

	pcGraphicsSystem->UpdateD3D();

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL))
		{
			tTempVertexBuffer.start = -1;
			tTempVertexBuffer.end = -1;
			tTempVertexBuffer.ratio = -1;

			if (nCurrentEntity == 0)
				tTempPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 1);
			else
			{
				float opacity = fadeTime / 5;

				if (opacity > 1)
					return 2;

				tTempPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, opacity);
			}

			pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}
	}

	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);

	for (auto i = G_KEY_UNKNOWN; i <= G_KEY_9; ++i)
	{
		if (pcInputSystem->InputCheck(i) == 1)
		{
			if (i == G_KEY_ESCAPE)
				return -1;

			return 2;
		}
	}

	if (pcInputSystem->InputCheck(G_BUTTON_LEFT))
		return 2;

	fadeTime += fpsTimer.GetDelta();

	return 1;
}

void CGameMangerSystem::InitializeEndScreen(bool playerWin)
{
	unsigned int nThisEntity;

	if (playerWin)
	{
		{
			wchar_t textBuffer[] =
			{ L"SUCCESS" };

			nThisEntity = createEntityReverse(&tThisWorld);
			CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2, 1, 0, 4.8, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, 0.1);

			pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

			pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_DEATHSCREEN);

			tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
		}

		{
			wchar_t textBuffer[] =
			{ L"You Completed The Objective" };

			nThisEntity = createEntityReverse(&tThisWorld);
			CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 6, 1, 0, 2.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, 0.1);

			pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

			pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_DEATHSCREEN);

			tThisWorld.atLabel[nThisEntity].lastUIElement = true;
			tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
		}

		{
			wchar_t textBuffer[] =
			{ L"CONTINUE" };

			nThisEntity = createEntityReverse(&tThisWorld);
			CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 1.4, 1, 0, -1.2, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);

			pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
			pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 2, true);

			pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_DEATHSCREEN);
			tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
		}
	}
	else
	{
		{
			wchar_t textBuffer[] =
			{ L"DEFEAT" };

			nThisEntity = createEntityReverse(&tThisWorld);
			CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2, 1, 0, 4.8, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, 0.1);

			pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

			pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_DEATHSCREEN);

			tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
		}

		{
			wchar_t textBuffer[] =
			{ L"You Failed To Complete The Objective" };

			nThisEntity = createEntityReverse(&tThisWorld);
			CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 6, 1, 0, 2.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, 0.1);

			pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

			pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_DEATHSCREEN);

			tThisWorld.atLabel[nThisEntity].lastUIElement = true;
			tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
		}

		{
			wchar_t textBuffer[] =
			{ L"CONTINUE" };

			nThisEntity = createEntityReverse(&tThisWorld);
			CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 1.4, 1, -1, -1.2, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);

			pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
			pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 13, true);

			pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_DEATHSCREEN);
			tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
		}

		{
			wchar_t textBuffer[] =
			{ L"EXIT" };

			nThisEntity = createEntityReverse(&tThisWorld);
			CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, .8, 1, 1, -1.2, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);

			pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
			pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 2, true);

			pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_DEATHSCREEN);
			tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
		}
	}

	pcGraphicsSystem->CreateBuffers(&tThisWorld);
	ShowCursor(true);
}

void CGameMangerSystem::InitializeOptionsMenu()
{
	unsigned int nThisEntity;

	//if (pcUISystem->myFont == nullptr)
	//{
	//	pcUISystem->myFont = CreateFontA(24, 12, 0, 0, FW_MEDIUM, false, false, false, 0, 0, 0, 0, 0, 0);
	//}

	if (fontTexture == nullptr)
	{
		ID3D11Resource* resource;

		wchar_t filepath[] =
		{ L"UI_Textures.fbm/FontB.png" };

		CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, filepath, &resource, &fontTexture, NULL);
	}

	{
		wchar_t wideChar[] =
		{ L"UI_Textures.fbm/Auger_Space.png" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 20, 20, 0, 0, &atUIVertices, -1, 0.2);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, wideChar);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 1);
	}

	{
		wchar_t textBuffer[] =
		{ L"SUBTITLES:" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.05, .8, -1.545, 6.1, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"MASTER VOLUME:" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 3, .8, -2.001, 4.9, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		char valueToChange[] =
		{ "Master Volume" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 4.8, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), valueToChange, ARRAYSIZE(valueToChange));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 4.8, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 1, 1, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
		//pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_BACKGROUND);
	}

	{
		wchar_t textBuffer[] =
		{ L"DIALOGUE VOLUME:" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 3.4, .8, -2.22, 3.7, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		char valueToChange[] =
		{ "Dialogue Volume" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 3.6, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), valueToChange, ARRAYSIZE(valueToChange));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 3.6, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 1, 1, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
		//pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_BACKGROUND);
	}

	{
		wchar_t textBuffer[] =
		{ L"MUSIC VOLUME:" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.8, .8, -1.915, 2.5, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		char valueToChange[] =
		{ "Music Volume" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 2.4, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), valueToChange, ARRAYSIZE(valueToChange));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 2.4, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 1, 1, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
		//pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_BACKGROUND);
	}

	{
		wchar_t textBuffer[] =
		{ L"FX VOLUME:" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.595, 1.3, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		char valueToChange[] =
		{ "FX Volume" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 1.2, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), valueToChange, ARRAYSIZE(valueToChange));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 1.2, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 1, 1, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
		//pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_BACKGROUND);
	}

	{
		wchar_t textBuffer[] =
		{ L"DIFFICULTY:" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.60, 0.1, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"BRIGHTNESS:" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.59, -1.1, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		char valueToChange[] =
		{ "Brightness" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, -1.2, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), valueToChange, ARRAYSIZE(valueToChange));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, -1.2, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 1, 1, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
		//pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_BACKGROUND);
	}

	{
		wchar_t textBuffer[] =
		{ L"SENSITIVITY:" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.59, -2.3, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		char valueToChange[] =
		{ "Sensitivity" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, -2.4, &atUIVertices, -1, 0.1);
		// weird little formula to convert the MouseRotationSpeed from 0.003-0.008 to 0-1
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), valueToChange, ARRAYSIZE(valueToChange), (pcInputSystem->GetMouseRotationSpeed() - .003) * 200);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, -2.4, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 1, 1, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
		//pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_BACKGROUND);
	}

	{
		wchar_t textBuffer[] =
		{ L"BACK" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 1, 1, 0, -3.6, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, nCurrentScene + 1, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}
}

void CGameMangerSystem::InitializeCredits()
{
	unsigned int nThisEntity;
	{
		wchar_t wideChar[] =
		{ L"UI_Textures.fbm/Auger_Credits.png" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 20, 20, 0, 0, &atUIVertices, -1, .2);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, wideChar);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_CREDITS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 1);
	}

	{
		wchar_t textBuffer[] =
		{ L"CREDITS" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2, 1, 0, 4.8, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_CREDITS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"BACK" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 1, 1, 0, -2.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, nCurrentScene + 1, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_CREDITS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}
}

void CGameMangerSystem::InitializePauseScreen()
{
	unsigned int nThisEntity;

	{
		wchar_t filePath[] =
		{ L"UI_Textures.fbm/transparentSquareB.png" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 20, 20, 0, 0, &atUIVertices, nThisEntity);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, filePath);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_PAUSESCREEN);
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_DEATHSCREEN);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, .2);
	}

	{
		wchar_t textBuffer[] =
		{ L"CONTINUE" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2, 1, 0, 4.8, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, nCurrentScene + 1, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_PAUSESCREEN);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"SAVE" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 1, 1, 0, 3.6, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 96, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_PAUSESCREEN);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"LOAD" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 1, 1, 0, 2.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 97, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_PAUSESCREEN);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"OPTIONS" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2, 1, 0, 1.2, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 98, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_PAUSESCREEN);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"EXIT" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 1, 1, 0, 0, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 2, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_PAUSESCREEN);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"SUBTITLES:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.05, .8, -1.545, 6.1, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"MASTER VOLUME:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 3, .8, -2.001, 4.9, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		char valueToChange[] =
		{ "Master Volume" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 4.8, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), valueToChange, ARRAYSIZE(valueToChange));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 4.8, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"DIALOGUE VOLUME:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 3.4, .8, -2.22, 3.7, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		char valueToChange[] =
		{ "Dialogue Volume" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 3.6, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), valueToChange, ARRAYSIZE(valueToChange));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 3.6, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"MUSIC VOLUME:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.8, .8, -1.915, 2.5, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		char valueToChange[] =
		{ "Music Volume" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 2.4, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), valueToChange, ARRAYSIZE(valueToChange));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 2.4, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"FX VOLUME:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.595, 1.3, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		char valueToChange[] =
		{ "FX Volume" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 1.2, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), valueToChange, ARRAYSIZE(valueToChange));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 1.2, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"DIFFICULTY:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.60, 0.1, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"BRIGHTNESS:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.59, -1.1, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		char valueToChange[] =
		{ "Brightness" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, -1.2, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), valueToChange, ARRAYSIZE(valueToChange));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, -1.2, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"SENSITIVITY:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.59, -2.3, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		char valueToChange[] =
		{ "Sensitivity" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, -2.4, &atUIVertices, -1, 0.1);
		// weird little formula to convert the MouseRotationSpeed from 0.003-0.008 to 0-1
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), valueToChange, ARRAYSIZE(valueToChange), (pcInputSystem->GetMouseRotationSpeed() - .003) * 200);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, -2.4, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
		//pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_BACKGROUND);
	}

	{
		wchar_t textBuffer[] =
		{ L"BACK" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 1, 1, 0, -3.6, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, nCurrentScene + 1, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	pcGraphicsSystem->CreateBuffers(&tThisWorld);
}

void CGameMangerSystem::InitializeHUD()
{
	atUIVertices.clear();
	atUIIndices.clear();

	unsigned int nThisEntity;

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, .35, .15, .1, -.4, &atUIVertices, nThisEntity, .1);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(.5, .5, .5, 1);
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, .35, .15, -.54, -.4, &atUIVertices, nThisEntity, .1);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(.5, .5, .5, 1);
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, .1, .45, -.22, 0.05, &atUIVertices, nThisEntity, .1);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(.5, .5, .5, 1);
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, .1, .45, -.22, -.85, &atUIVertices, nThisEntity, .1);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(.5, .5, .5, 1);
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, 2, 6, -8, &atUIVertices, nThisEntity, .1);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(.5, .5, .5, 1);
	}

	{
		wchar_t textBuffer[] =
		{ L"3" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, .75, 1, 6.625, -7.5, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 3, 1, -7, -8, &atUIVertices, nThisEntity, .1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 3, 1, -7, -8, &atUIVertices, nThisEntity, .15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 3, 1, -7, -9.1, &atUIVertices, nThisEntity, .1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 1, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 3, 1, -7, -9.1, &atUIVertices, nThisEntity, .15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
	}

	{
		wchar_t textBuffer[] =
		{ L"000" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 1, 1, -6.625, 7.5, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_FPS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	/*{
		wchar_t textBuffer[] =
		{ L"3" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, .4, 1, 9.2, 8.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}*/

	{
		wchar_t filePath[] =
		{ L"UI_Textures.fbm/objectiveCircle.png" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, .5, 1, 4.7, 8.5, &atUIVertices, nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, filePath);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"Kill The Enemies" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 4, 1, 7, 8.5, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}
}

void CGameMangerSystem::LoadPathFindingTest()
{
	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);

	pcAiSystem->SetNumberOfAI(2);
	//tTimerInfo->StartClock(tAugerTimers->tSceneTimer);

	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();
	debugCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	walkCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	aimCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();

	tCameraMode.bDebugMode = false;
	tCameraMode.bAimMode = true;
	tCameraMode.bWalkMode = false;
	tCameraMode.bSwitch = false;
	bMoving = false;
	m_d3dPlayerMatrix = pcGraphicsSystem->SetDefaultWorldPosition();

	//m_d3dPlayerMatrix = XMMatrixMultiply(m_d3dPlayerMatrix, XMMatrixScaling(.01, .01, .01));

	m_d3dPlayerMatrix.r[3].m128_f32[2] -= 10;
	ImporterData tempImport;
	tempImport = pcGraphicsSystem->ReadMesh("meshData_Pirate.txt");

	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		PlayerStartIndex = createClayton(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);

	}

	tThisWorld.atClayton[PlayerStartIndex].health = 100;
	XMMATRIX wall = m_d3dWorldMatrix;
	wall.r[3].m128_f32[1] += -1;

	CreateWall(&tThisWorld, wall);
	wall.r[3].m128_f32[0] -= 10;
	wall.r[3].m128_f32[1] -= 1;
	wall.r[3].m128_f32[2] -= 1;

	CreateDoorWay(&tThisWorld, wall);
	wall.r[3].m128_f32[0] -= 22;
	CreateDoorWay(&tThisWorld, wall);

	wall = m_d3dWorldMatrix;
	wall.r[3].m128_f32[1] += -1;
	wall.r[3].m128_f32[0] += -22;
	CreateWall(&tThisWorld, wall);
	XMMATRIX celling = m_d3dWorldMatrix;
	celling.r[3].m128_f32[1] += 10;
	CreateCelling(&tThisWorld, celling);
	XMMATRIX AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[2] += -5;
	AILocation.r[3].m128_f32[0] += -5;
	//CreateSimpleGunAi(&tThisWorld, AILocation);



	AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[0] += -1;
	AILocation.r[3].m128_f32[2] += -2;



	AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[0] += -3;
	AILocation.r[3].m128_f32[2] += -5;

	//CreateSimpleGunAi(&tThisWorld, AILocation);
	XMMATRIX groundSpawnPoint;
	groundSpawnPoint = m_d3dWorldMatrix;
	groundSpawnPoint.r[3].m128_f32[1] -= 2;
	CreateGround(&tThisWorld, groundSpawnPoint);
	groundSpawnPoint.r[3].m128_f32[2] -= -20;
	CreateGround(&tThisWorld, groundSpawnPoint);

	tempImport = pcGraphicsSystem->ReadMesh("meshData_Example_Objects.txt");

	for (int meshIndex = 1; meshIndex < tempImport.meshCount; meshIndex++)
	{
		//int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	}

	tempImport = pcGraphicsSystem->ReadMesh("meshData_ScifiRoom.txt");

	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		//int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	}

#pragma region 
	AILocation.r[3].m128_f32[0] -= 7;
	int nodeLocation = CreateNodePoint(&tThisWorld, AILocation);
	//	tThisWorld.atSimpleMesh[nodeLocation].m_nColor= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT3 nodePosition;
	nodePosition.x = AILocation.r[3].m128_f32[0];
	nodePosition.y = AILocation.r[3].m128_f32[1];
	nodePosition.z = AILocation.r[3].m128_f32[2];

	pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	XMMATRIX part2ofNodeMovement = AILocation;
	part2ofNodeMovement.r[3].m128_f32[2] -= -10;
	int nodelocation5 = CreateNodePoint(&tThisWorld, part2ofNodeMovement);
	nodePosition.x = part2ofNodeMovement.r[3].m128_f32[0];
	nodePosition.y = part2ofNodeMovement.r[3].m128_f32[1];
	nodePosition.z = part2ofNodeMovement.r[3].m128_f32[2];

	pcAiSystem->AddNodeToPathFinding(nodelocation5, nodePosition, 1);
	part2ofNodeMovement.r[3].m128_f32[0] -= 6;

	int nodelocation4 = CreateNodePoint(&tThisWorld, part2ofNodeMovement);
	nodePosition.x = part2ofNodeMovement.r[3].m128_f32[0];
	nodePosition.y = part2ofNodeMovement.r[3].m128_f32[1];
	nodePosition.z = part2ofNodeMovement.r[3].m128_f32[2];

	pcAiSystem->AddNodeToPathFinding(nodelocation4, nodePosition, 1);
	AILocation.r[3].m128_f32[0] += 4;
	AILocation.r[3].m128_f32[2] += -4;

	int nodeLocation2 = CreateNodePoint(&tThisWorld, AILocation);

	nodePosition.x = AILocation.r[3].m128_f32[0];
	nodePosition.y = AILocation.r[3].m128_f32[1];
	nodePosition.z = AILocation.r[3].m128_f32[2];

	pcAiSystem->AddNodeToPathFinding(nodeLocation2, nodePosition, 1);
	XMMATRIX pathfindAI = AILocation;
	pathfindAI.r[3].m128_f32[0] += 3;
	nodePosition.x = pathfindAI.r[3].m128_f32[0];
	nodePosition.y = pathfindAI.r[3].m128_f32[1];
	nodePosition.z = pathfindAI.r[3].m128_f32[2];
	int aiPathIndex = CreateTestAIPathFinding(&tThisWorld, pathfindAI);
	//	tThisWorld.atSimpleMesh[aiPathIndex].m_nColor= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	int nodelocation3 = CreateNodePoint(&tThisWorld, pathfindAI);
	tThisWorld.atPathPlanining[aiPathIndex].startingNode = nodelocation3;
	tThisWorld.atPathPlanining[aiPathIndex].Goal = nodelocation4;
	pcAiSystem->AddNodeToPathFinding(nodelocation3, nodePosition, 1);
	vector<int> edges;
	//	edges.push_back(nodelocation4);
	edges.push_back(nodeLocation2);
	edges.push_back(nodelocation3);
	edges.push_back(nodelocation5);

	pcAiSystem->AddEdgestoNode(nodeLocation, edges);
	edges.clear();
	edges.push_back(nodeLocation);
	edges.push_back(nodelocation3);
	pcAiSystem->AddEdgestoNode(nodeLocation2, edges);
	edges.clear();
	//	edges.push_back(nodeLocation);
	edges.push_back(nodeLocation2);
	pcAiSystem->AddEdgestoNode(nodelocation3, edges);
	edges.clear();
	edges.push_back(nodelocation4);
	pcAiSystem->AddEdgestoNode(nodelocation5, edges);
#pragma endregion Nodes & Edges


	if (pcCollisionSystem->m_AAbb.size() != 0) {
		float x = 0;
	}
	XMFLOAT4 blue;
	blue.y = 0;
	blue.z = 1;
	blue.w = 1;
	blue.x = 0;
	tThisWorld.atSimpleMesh[nodelocation4].m_nColor = blue;
	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{

		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask > 1)
		{
			if (tThisWorld.atSimpleMesh[nCurrentEntity].m_nVertexCount > tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount)
			{
				TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[nCurrentEntity].m_VertexData, tThisWorld.atAABB[nCurrentEntity]);
				MyAbb.m_IndexLocation = nCurrentEntity;
				tThisWorld.atAABB[nCurrentEntity] = MyAbb;
				pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);
				if (nCurrentEntity == PlayerStartIndex) {
					pcCollisionSystem->AddAiVisioNCheck(MyAbb, nCurrentEntity);

				}

			}

		}
		if (tThisWorld.atMesh[nCurrentEntity].m_nVertexCount > tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount)
		{
			TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atMesh[nCurrentEntity].m_VertexData, tThisWorld.atAABB[nCurrentEntity]);
			MyAbb.m_IndexLocation = nCurrentEntity;
			tThisWorld.atAABB[nCurrentEntity] = MyAbb;
			pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);
		}
	}


	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask > 1)
		{
			tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
		}
	}


	pcGraphicsSystem->CreateBuffers(&tThisWorld);
}

int CGameMangerSystem::PathFindingExample()
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(-1); //Important!
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective(m_RealTimeFov);

	//	tTimerInfo->applicationTime = tTimerInfo->GetTime(tAugerTimers->tAppTimer, tTimerInfo->applicationTime);
	//tTimerInfo->sceneTime = tTimerInfo->GetTime(tAugerTimers->tSceneTimer, tTimerInfo->sceneTime);

	tCameraMode = pcInputSystem->CameraModeListen(tCameraMode);

	static XMMATRIX m_d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	static XMMATRIX m_d3dOffsetMatrix = pcGraphicsSystem->SetDefaultOffset();

	if (pcInputSystem->InputCheck(G_KEY_P))
	{
		return 3;
	}
	// ui stuff
	if (GamePaused == true) {
		if (DrawUI == true) {
			XMMATRIX UiPos = tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix;
			//UiPos.r[3].m128_f32[1] -= 1;
			XMVECTOR foward;
			foward.m128_f32[0] = 0;
			foward.m128_f32[1] = 1;
			foward.m128_f32[2] = 2;
			//	foward.m128_f32[0] = 1;


			XMMATRIX localMatrix2 = XMMatrixTranslationFromVector(foward);

			UiPos = XMMatrixMultiply(localMatrix2, UiPos);
			DrawUI = false;
			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] -= 1;
			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] += 1;
			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] += 1;
			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] += 1;
			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[0] += 1;
			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[0] += 0.8;
			UiPos.r[3].m128_f32[1] -= 0.3;

			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] -= 0.7;

			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] -= 0.7;

			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[0] -= 1;

			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
		}
	}

	if (tCameraMode.bWalkMode == true)
	{
		if (tCameraMode.bSwitch == true)
		{
			m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
			tCameraMode.bSwitch = false;
		}
		m_d3d_ResultMatrix = pcInputSystem->WalkCameraControls(XMVectorSet(0, 1.0f, 0, 0), m_d3d_ResultMatrix, bMoving);

		walkCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);
		walkCamera->d3d_Position = XMMatrixMultiply(m_d3dOffsetMatrix, walkCamera->d3d_Position);






	}
	else if (tCameraMode.bAimMode == true)
	{
		if (tCameraMode.bSwitch == true)
		{
			m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
			//CameraNewPosition = pcInputSystem->CameraBehaviorLerp(m_d3d_ResultMatrix, m_d3dPlayerMatrix);
			//m_d3dPlayerMatrix = pcInputSystem->AimMode(m_d3dPlayerMatrix);

			CameraNewPosition = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);

			CameraNewPosition = XMMatrixMultiply(m_d3dOffsetMatrix, CameraNewPosition);
			aimCamera->d3d_Position = pcInputSystem->CameraBehaviorLerp(walkCamera->d3d_Position, CameraNewPosition, scale);
			scale += 0.001;
			if (scale > 1) {
				tCameraMode.bSwitch = false;
				scale = 0;

			}

		}
		else {
			m_RealTimeFov = pcInputSystem->ZoomSight(m_RealTimeFov);




			//			m_d3dPlayerMatrix = pcInputSystem->AimMode(m_d3dPlayerMatrix);

			aimCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);

			aimCamera->d3d_Position = XMMatrixMultiply(m_d3dOffsetMatrix, aimCamera->d3d_Position);
		}

	}
	else
	{

		if (tCameraMode.bSwitch == true)
		{
			m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
			tCameraMode.bSwitch = false;
		}
		m_d3d_ResultMatrix = pcInputSystem->DebugCamera(m_d3d_ResultMatrix, m_d3dWorldMatrix);

		debugCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dWorldMatrix);
	}

	CGraphicsSystem::TPrimalVertexBufferType tTempVertexBuffer;
	CGraphicsSystem::TPrimalPixelBufferType tTempPixelBuffer;
	CGraphicsSystem::TMyVertexBufferType tMyVertexBufferTemp;
	tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);



	pcGraphicsSystem->UpdateD3D();
#pragma region Input Garbage
	// togle the modes that you are in
	if (pcInputSystem->InputCheck(G_BUTTON_MIDDLE)) {
		tThisWorld.atClip[GunIndexForPlayer].GunMode = !tThisWorld.atClip[GunIndexForPlayer].GunMode;
	}
	// shoot a bullet
	if (pcInputSystem->InputCheck(G_BUTTON_LEFT) == 1 && tThisWorld.atClip[GunIndexForPlayer].GunMode == true) {

		tThisWorld.atClip[GunIndexForPlayer].tryToShoot = true;


	}
	// shoot a ray
	else if (pcInputSystem->InputCheck(G_BUTTON_LEFT) == 1 && tThisWorld.atClip[GunIndexForPlayer].GunMode == false) {
		tThisWorld.atClip[GunIndexForPlayer].tryToShoot = true;

	}
	// turn the ray off
	else if (tThisWorld.atClip[GunIndexForPlayer].GunMode == false)
	{
		tThisWorld.atClip[GunIndexForPlayer].tryToShoot = false;

	}

	// reload
	if (pcInputSystem->InputCheck(G_KEY_R) == 1) {

		tThisWorld.atClip[GunIndexForPlayer].tryToReload = true;

	}

#pragma endregion

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		{

			pcGraphicsSystem->InitPrimalShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, debugCamera->d3d_Position, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], debugCamera->d3d_Position);

			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

		}
		// ai code would run here
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			if (tCameraMode.bWalkMode == true)
			{
				tMyVertexBufferTemp.m_d3dViewMatrix = walkCamera->d3d_Position;
				tTempVertexBuffer.m_d3dViewMatrix = walkCamera->d3d_Position;
			}
			else if (tCameraMode.bAimMode == true)
			{
				tTempVertexBuffer.m_d3dViewMatrix = aimCamera->d3d_Position;
				tMyVertexBufferTemp.m_d3dViewMatrix = aimCamera->d3d_Position;

			}
			else if (tCameraMode.bDebugMode == true)
			{
				tTempVertexBuffer.m_d3dWorldMatrix = m_d3dWorldMatrix;
				tTempVertexBuffer.m_d3dViewMatrix = debugCamera->d3d_Position;
				tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;

			}
			else {
				tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
				tTempVertexBuffer.m_d3dViewMatrix = m_d3dViewMatrix;
				tMyVertexBufferTemp.m_d3dViewMatrix = m_d3dViewMatrix;
			}

			if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK))
			{
				if (tCameraMode.bWalkMode == true)
				{
					m_d3dPlayerMatrix = pcInputSystem->CharacterMovement(m_d3dPlayerMatrix);
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
					m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
				}
				else if (tCameraMode.bAimMode == true)
				{
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
					m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
				}
			}
			if (tCameraMode.bWalkMode == true)
			{
				pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], walkCamera->d3d_Position);
			}
			else if (tCameraMode.bAimMode == true)
			{
				pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], aimCamera->d3d_Position);
			}
			else
			{
				pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);
			}


			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

		if (pcAiSystem->GetNumberOfAI() <= 0)
		{
			return -1;
		}
		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_FOLLOW) || tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_SHOOT | COMPONENT_AIMASK | COMPONENT_FOLLOW) || (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST) == tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask)
		{
#if AI_ON				
			if (tThisWorld.atActiveAI[nCurrentEntity].active == true) {
				pcAiSystem->FollowObject(tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
				pcAiSystem->ShootGun(&tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex]);
			}
#endif // AI_ON
		}
		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_HEALINGAI)) {

		}

		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SEARCH) || tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SPOTEDPLAYER)) {



			if (tThisWorld.atAIVision[nCurrentEntity].keepSearching == true) {
				if (tThisWorld.atAIVision[nCurrentEntity].visionRotation < 7 && tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true) {
					tThisWorld.atAIVision[nCurrentEntity].visionRotation += 0.001;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcAiSystem->LookBackLeftToRight(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix,
						true);
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true) {
					tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight = false;
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].visionRotation > -7 && tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == false) {
					tThisWorld.atAIVision[nCurrentEntity].visionRotation -= 0.001;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcAiSystem->LookBackLeftToRight(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix,
						false);
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == false) {
					tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight = true;
				}
			}
			else if (tThisWorld.atAIVision[nCurrentEntity].keepSearching == false) {
				pcAiSystem->LookAtObject(tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
			}

			float4x4 AiFrustum;
			AiFrustum.row1.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[0];
			AiFrustum.row1.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[1];
			AiFrustum.row1.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[2];
			AiFrustum.row1.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[3];

			AiFrustum.row2.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[0];
			AiFrustum.row2.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[1];
			AiFrustum.row2.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[2];
			AiFrustum.row2.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[3];

			AiFrustum.row3.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[0];
			AiFrustum.row3.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[1];
			AiFrustum.row3.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[2];
			AiFrustum.row3.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[3];

			AiFrustum.row4.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[0];
			AiFrustum.row4.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1];
			AiFrustum.row4.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[2];
			AiFrustum.row4.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[3];
			pcAiSystem->UpdateFrustum(tThisWorld.atAIVision[nCurrentEntity].eyes0, AiFrustum, 70, 1, 0.1, 20);
			//	 pcAiSystem->calculate_frustum(&tThisWorld,tThisWorld.atAIVision[nCurrentEntity].eyes0, AiFrustum,70,1,0.1,20, nCurrentEntity, -2.1, 1.4, 19.6);
			vector<int> indicies;
			if (pcCollisionSystem->AiVisionCheck(tThisWorld.atAIVision[nCurrentEntity].eyes0, &indicies) == true)
			{
				float x = 0;
				bool danger = false;
				for (int i = 0; i < indicies.size(); ++i) {
					if (PlayerStartIndex == indicies[i])
					{
						tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
						tThisWorld.atAIVision[nCurrentEntity].keepSearching = false;
						danger = true;
						if (tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].nBulletsAvailables.size() <= 0) {
							tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].tryToReload = true;

						}
						tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].tryToShoot = true;

						pcAiSystem->AddAiInCombat(nCurrentEntity);
						//		tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask = COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST;
						/*	if (tThisWorld.atPathPlanining[nCurrentEntity].foundDestination == true) {
						int previousgoal = tThisWorld.atPathPlanining[nCurrentEntity].Goal;
						int previousStartPosition = tThisWorld.atPathPlanining[nCurrentEntity].startingNode;
						tThisWorld.atPathPlanining[nCurrentEntity].Goal = previousStartPosition;
						tThisWorld.atPathPlanining[nCurrentEntity].startingNode = previousgoal;
						tThisWorld.atPathPlanining[nCurrentEntity].foundDestination = false;
						tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding = true;

						}*/

					}
					else if (tThisWorld.atProjectiles[indicies[i]].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
						danger = true;
						tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

					}
					else if (danger == false) {
						tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
					}
				}
			}



			/*	if (cloasestPlayer < closestWall) {
			XMFLOAT4 red;
			red.y = 0;
			red.z = 0;
			red.w = 1;
			red.x = 1;
			tThisWorld.atSimpleMesh[8].m_nColor = red;
			spotedplayer = true;
			tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask = COMPONENT_AIMASK | COMPONENT_SPOTEDPLAYER;
			tThisWorld.atAIVision[nCurrentEntity].playerLastKnownLocation = tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix;
			tThisWorld.atAIVision[nCurrentEntity].keepSearching = false;
			}
			else {
			spotedplayer = false;
			tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask = COMPONENT_AIMASK | COMPONENT_SEARCH;
			}*/

		}
		//else if(tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask==())
		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_PATHFINDTEST) || (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST) == tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask) {

			if (tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding == true) {

				pcAiSystem->FindBestPath(tThisWorld.atPathPlanining[nCurrentEntity].startingNode,
					tThisWorld.atPathPlanining[nCurrentEntity].Goal,
					&tThisWorld.atPathPlanining[nCurrentEntity].directions);

				tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding = false;
			}
			else {
				if (tThisWorld.atPathPlanining[nCurrentEntity].DelayMovement <= 0) {
					pcAiSystem->PathPlaningMovement(&tThisWorld.atPathPlanining[nCurrentEntity], &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
				}
				else {
					tThisWorld.atPathPlanining[nCurrentEntity].DelayMovement -= 0.001;
				}
			}

		}
		if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_CLIP))
		{
			if (tThisWorld.atClip[nCurrentEntity].GunMode == false && tThisWorld.atClip[nCurrentEntity].tryToShoot == true)
			{
				if (tThisWorld.atClip[nCurrentEntity].maderay == false)
				{
					XMMATRIX gun = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
					gun.r[3].m128_f32[0] += 1;
					rayindex = CreateRayBullet(&tThisWorld, gun, 10, PlayerStartIndex, -0.6, -0.1, 10.7);
					pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, rayindex);
					tThisWorld.atClip[nCurrentEntity].maderay = true;
				}

			}
			else if (tThisWorld.atClip[nCurrentEntity].GunMode == false && tThisWorld.atClip[nCurrentEntity].tryToShoot == false && tThisWorld.atClip[nCurrentEntity].maderay == true)
			{
				pcGraphicsSystem->CleanD3DObject(&tThisWorld, rayindex);
				rayindex = -1;
				tThisWorld.atClip[nCurrentEntity].maderay = false;

			}
			else
			{
				if (nCurrentEntity == 10 && tThisWorld.atClip[nCurrentEntity].tryToShoot == true && tThisWorld.atClip[nCurrentEntity].fShootingCoolDown >= 0) {
					float x = 0;
				}
				if (tThisWorld.atClip[nCurrentEntity].tryToShoot == true && tThisWorld.atClip[nCurrentEntity].nBulletsAvailables.size() > 0 && tThisWorld.atClip[nCurrentEntity].fShootingCoolDown <= 0)
				{


					XMVECTOR foward;
					foward.m128_f32[0] = 0;
					foward.m128_f32[1] = 0;
					foward.m128_f32[2] = 1;



					XMMATRIX localMatrix2 = XMMatrixTranslationFromVector(foward);
					XMMATRIX gunMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
					gunMatrix = XMMatrixMultiply(localMatrix2, gunMatrix);

					int newbullet = CreateBullet(&tThisWorld, gunMatrix,
						tThisWorld.atClip[nCurrentEntity].currentMaterial, 0); // Last zero is bulletType - 0 for friend, 1 for enemy
					tThisWorld.atClip[newbullet].gunIndex = nCurrentEntity;
					tThisWorld.atSimpleMesh[newbullet].m_nColor = tThisWorld.atClip[nCurrentEntity].colorofBullets;
					tThisWorld.atClip[newbullet].indexInclip = pcProjectileSystem->CreateBulletProjectile(newbullet, &tThisWorld.atClip[nCurrentEntity]);
					tThisWorld.atAABB[newbullet] = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[newbullet].m_VertexData, tThisWorld.atAABB[newbullet]);
					tThisWorld.atAABB[newbullet].m_IndexLocation = newbullet;

					pcCollisionSystem->AddAABBCollider(tThisWorld.atAABB[newbullet], newbullet);
					pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, newbullet);

					tThisWorld.atClip[nCurrentEntity].tryToShoot = false;

				}
				else if (tThisWorld.atClip[nCurrentEntity].tryToShoot == true) {
					tThisWorld.atClip[nCurrentEntity].tryToShoot = false;
				}
				if (tThisWorld.atClip[nCurrentEntity].tryToReload == true) {
					pcProjectileSystem->Reload(&tThisWorld.atClip[nCurrentEntity]);
					tThisWorld.atClip[nCurrentEntity].tryToReload = false;

				}
				//if (tThisWorld.atClip[nCurrentEntity].nBulletsFired.size() != 0) {
				//	for (int i = 0; i < tThisWorld.atClip[nCurrentEntity].nBulletsFired.size(); ++i) {
				//	/*	if (tThisWorld.atClip[nCurrentEntity].fAliveTime[i] < tThisWorld.atClip[nCurrentEntity].FValueOfCoolDown) {
				//			pcPhysicsSystem->AddBulletForce(&tThisWorld.atRigidBody[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]], tThisWorld.atClip[nCurrentEntity].bulletSpeed);
				//			tThisWorld.atClip[nCurrentEntity].fAliveTime[i] += 0.1;

				//		}*/
				//		else {
				//		/*	tThisWorld.atClip[nCurrentEntity].fAliveTime.erase(tThisWorld.atClip[nCurrentEntity].fAliveTime.begin() + i);
				//			pcCollisionSystem->RemoveAABBCollider(tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]);
				//			pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]);
				//			tThisWorld.atClip[nCurrentEntity].nBulletsFired.erase(tThisWorld.atClip[nCurrentEntity].nBulletsFired.begin() + i);
				//			for (int i = 0; i < tThisWorld.atClip[nCurrentEntity].fAliveTime.size(); ++i) {
				//				if (tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]].indexInclip != 0) {
				//					tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]].indexInclip -= 1;
				//				}

				//			}*/
				//		}
				//	}

				//}
				if (tThisWorld.atClip[nCurrentEntity].fShootingCoolDown > 0)
					tThisWorld.atClip[nCurrentEntity].fShootingCoolDown -= 0.1;
			}


		}


		if (nCurrentEntity == PlayerStartIndex) {
			float x = 0;
		}
		tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, true);

		if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_RAYGUN)) {
			float CloseEstObject = 10000000000000000000.0f;
			float* distanceCalucaltion = new float();
			for (list<TAABB>::iterator ptr = pcCollisionSystem->m_AAbb.begin(); ptr != pcCollisionSystem->m_AAbb.end(); ++ptr) {

				if (ptr->m_IndexLocation != PlayerStartIndex)
					if (pcCollisionSystem->IsLineInBox(XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[0], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix)
						, XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[1], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix),
						tThisWorld.atWorldMatrix[ptr->m_IndexLocation].worldMatrix, *ptr, distanceCalucaltion) == true && *distanceCalucaltion < CloseEstObject) {
						CloseEstObject = *distanceCalucaltion;
						tThisWorld.atClip[GunIndexForPlayer].currentMaterial = 0;

						tThisWorld.atClip[GunIndexForPlayer].colorofBullets = tThisWorld.atSimpleMesh[ptr->m_IndexLocation].m_nColor;
					}
			}
			delete distanceCalucaltion;
		}


		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
			COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC) || tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
				COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)) {
			tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
		}


		if ((tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_TRIGGER) || tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)))
		{

			vector<int> otherCollisionsIndex;
			if (pcCollisionSystem->AABBtoAABBCollisionCheck(tThisWorld.atAABB[nCurrentEntity], &otherCollisionsIndex) == true)
			{
				for (int i = 0; i < otherCollisionsIndex.size(); ++i) {
					if (tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == true && tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask !=
						(COMPONENT_COLLISIONMASK |
							COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)) {
						if (nCurrentEntity == 8)
						{
							float x = 0;
						}

						tThisWorld.atRigidBody[nCurrentEntity].totalForce = -tThisWorld.atRigidBody[nCurrentEntity].velocity;


						tTempVertexBuffer.m_d3dWorldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcCollisionSystem->WalkingThrewObjectCheck(tTempVertexBuffer.m_d3dWorldMatrix, tThisWorld.atAABB[otherCollisionsIndex[i]], tThisWorld.atAABB[nCurrentEntity]);
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tTempVertexBuffer.m_d3dWorldMatrix;

					}
					if (tThisWorld.atRigidBody[nCurrentEntity].ground == false && tThisWorld.atRigidBody[nCurrentEntity].wall == false &&
						tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)
						&& (tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_STATIC)
							|| tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
								COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))) {
						tTempVertexBuffer.m_d3dWorldMatrix = pcCollisionSystem->WalkingThrewObjectCheck(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[otherCollisionsIndex[i]], tThisWorld.atAABB[nCurrentEntity]);
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tTempVertexBuffer.m_d3dWorldMatrix;
						if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK)) {
							m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
						}

					}
					if (tThisWorld.atRigidBody[nCurrentEntity].ground == false && tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == false && tThisWorld.atRigidBody[nCurrentEntity].wall == false &&
						tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)
						&& (tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_STATIC)) || tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
								COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))
					{

						if (tThisWorld.atAIMask[otherCollisionsIndex[i]].m_tnAIMask >1) {
							if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
								if (tThisWorld.atAiHeath[otherCollisionsIndex[i]].heath <= 0) {
									pcAiSystem->SetNumberOfAI(pcAiSystem->GetNumberOfAI() - 1);
									if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1) {
									/*	tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.erase
										(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.begin()
											+ tThisWorld.atClip[nCurrentEntity].indexInclip);

										pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
										tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.erase(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.begin()
											+ tThisWorld.atClip[nCurrentEntity].indexInclip);

*/
										pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);

									}
									pcCollisionSystem->RemoveAABBCollider(otherCollisionsIndex[i]);
									pcGraphicsSystem->CleanD3DObject(&tThisWorld, otherCollisionsIndex[i]);
									pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atAIMask[otherCollisionsIndex[i]].GunIndex);
								}
								else {
									/*tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.erase
									(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.begin()
										+ tThisWorld.atClip[nCurrentEntity].indexInclip);

									pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
									tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.erase(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.begin()
										+ tThisWorld.atClip[nCurrentEntity].indexInclip);*/


									pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);
									tThisWorld.atAiHeath[otherCollisionsIndex[i]].heath -= playerDamage;
									if (tThisWorld.atAiHeath[otherCollisionsIndex[i]].heath <= 0) {
										pcAiSystem->SetNumberOfAI(pcAiSystem->GetNumberOfAI() - 1);
										pcCollisionSystem->RemoveAABBCollider(otherCollisionsIndex[i]);
										pcGraphicsSystem->CleanD3DObject(&tThisWorld, otherCollisionsIndex[i]);
										pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atAIMask[otherCollisionsIndex[i]].GunIndex);
									}
								}
							}
						}
						else if (tThisWorld.atInputMask[otherCollisionsIndex[i]].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK)) {
							if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
								if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1) {
									/*tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.erase
									(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.begin()
										+ tThisWorld.atClip[nCurrentEntity].indexInclip);

									pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
									tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.erase(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.begin()
										+ tThisWorld.atClip[nCurrentEntity].indexInclip);
*/
									//pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atAIMask[nCurrentEntity].GunIndex);
									pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);

								}
								tThisWorld.atClayton[otherCollisionsIndex[i]].health -= pirateDamage;
							}


						}
						if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
							if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1) {
								//tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.erase
								//(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.begin()
								//	+ tThisWorld.atClip[nCurrentEntity].indexInclip);

								//pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
								//tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.erase(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.begin()
								//	+ tThisWorld.atClip[nCurrentEntity].indexInclip);

								////pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atAIMask[nCurrentEntity].GunIndex);
								//pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);

							}
						}

					}
					if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK) && tThisWorld.atAIMask[otherCollisionsIndex[i]].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_COVERTRIGGER)) {

						pcAiSystem->MoveAiToCoverLocation(tThisWorld.atCoverTrigger[otherCollisionsIndex[i]], &tThisWorld);
					}
				}
				if (tThisWorld.atClayton[PlayerStartIndex].health <= 0)
				{
					return -1;
				}
				//		tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);


			}
			else {

				//tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;

			}
		}
		else {

			//	tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;

		}
		tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;

		if (tThisWorld.atParentWorldMatrix[nCurrentEntity] != -1)
		{

			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(pcGraphicsSystem->SetDefaultWorldPosition(),
				tThisWorld.atWorldMatrix[tThisWorld.atParentWorldMatrix[nCurrentEntity]].worldMatrix);

			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(tThisWorld.atOffSetMatrix[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);

			if (nCurrentEntity == frustumIndex) {
				XMMATRIX empty;
				empty = pcGraphicsSystem->SetDefaultWorldPosition();
				XMMATRIX rotation;
				XMMATRIX d3dTmpViewM;
				rotation = XMMatrixRotationY(-0.3);
				empty.r[1] = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1];
				empty.r[0] = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0];
				d3dTmpViewM = empty;
				XMVECTOR d3d_newX, d3d_newY, d3d_existingZ;
				d3dTmpViewM = XMMatrixMultiply(rotation, d3dTmpViewM);
				d3d_existingZ = d3dTmpViewM.r[2];
				d3d_newX = XMVector3Cross(empty.r[1], d3d_existingZ);
				d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

				d3d_newX = XMVector3Normalize(d3d_newX);
				d3d_newY = XMVector3Normalize(d3d_newY);

				d3d_existingZ = XMVector3Normalize(d3d_existingZ);

				d3dTmpViewM.r[0] = d3d_newX;
				d3dTmpViewM.r[1] = d3d_newY;
				d3dTmpViewM.r[2] = d3d_existingZ;
				empty = d3dTmpViewM;
				empty.r[3].m128_f32[0] = 3;
				empty.r[3].m128_f32[1] = 0;
				empty.r[3].m128_f32[2] = 1;


				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = empty;
			}
		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID))
		{
			if (tCameraMode.bWalkMode == true)
			{
				pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity], walkCamera->d3d_Position);

			}
			else if (tCameraMode.bAimMode == true)
			{
				pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity], aimCamera->d3d_Position);

			}
			else
			{
				pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity], debugCamera->d3d_Position);
			}
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atSimpleMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}
		if (tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix.r[3].m128_f32[1] < -10) {
			return 7;
		}

	}

	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);
	zValue += 0.001;
	return 6;
}

void CGameMangerSystem::FirstSkeltonAiTestLoad()
{
	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);

	pcAiSystem->SetNumberOfAI(1);
	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
	walkCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	aimCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	debugCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();

	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();

	tCameraMode.bDebugMode = false;
	tCameraMode.bAimMode = true;
	tCameraMode.bWalkMode = false;
	tCameraMode.bSwitch = false;
	bMoving = false;
	m_d3dPlayerMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	//m_d3dPlayerMatrix = XMMatrixMultiply(m_d3dPlayerMatrix, XMMatrixScaling(.01, .01, .01));
	m_d3dPlayerMatrix = m_d3dWorldMatrix;
	m_d3dPlayerMatrix.r[3].m128_f32[2] -= 10;
	ImporterData tempImport;
	tempImport = pcGraphicsSystem->ReadMesh("meshData_Pirate.txt");

	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		PlayerStartIndex = createClayton(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);

	}
	XMMATRIX AISowrdGuy = m_d3dPlayerMatrix;
	int swordGuy = CreateFriendlySwordsMan(&tThisWorld, AISowrdGuy, PlayerStartIndex);
	tThisWorld.atSimpleMesh[swordGuy].m_nColor = XMFLOAT4(0, 0, 0, 1);

	//AimingLine(&tThisWorld, m_d3dWorldMatrix, PlayerStartIndex, -1, 1, 10.5);

	GunIndexForPlayer = CreateGun(&tThisWorld, m_d3dWorldMatrix, PlayerStartIndex, -1, 1, 10.5, 3, 100);
	tThisWorld.atClip[GunIndexForPlayer].bulletSpeed = 0.001;
	//tThisWorld.atClip[GunIndexForPlayer].

	tThisWorld.atClayton[PlayerStartIndex].health = 100000;
	XMMATRIX wall = m_d3dWorldMatrix;
	wall.r[3].m128_f32[1] += -1;

	int wallidex = CreateWall(&tThisWorld, wall);
	wall.r[3].m128_f32[0] -= 10;
	wall.r[3].m128_f32[1] -= 1;
	wall.r[3].m128_f32[2] -= 1;
	tThisWorld.atSimpleMesh[wallidex].m_nColor = XMFLOAT4(1, 0, 0, 1);

	int door1Index = CreateDoorWay(&tThisWorld, wall);
	tThisWorld.atSimpleMesh[door1Index].m_nColor = XMFLOAT4(1, 0, 0, 1);

	wall.r[3].m128_f32[0] -= 22;
	int door2Index = CreateDoorWay(&tThisWorld, wall);
	tThisWorld.atSimpleMesh[door2Index].m_nColor = XMFLOAT4(1, 0, 0, 1);


	wall = m_d3dWorldMatrix;
	wall.r[3].m128_f32[1] += -1;
	wall.r[3].m128_f32[0] += -22;
	int wall2index = CreateWall(&tThisWorld, wall);
	tThisWorld.atSimpleMesh[wall2index].m_nColor = XMFLOAT4(0, 1, 0, 1);

	XMMATRIX celling = m_d3dWorldMatrix;
	celling.r[3].m128_f32[1] += 10;
	CreateCelling(&tThisWorld, celling);
	XMMATRIX AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[2] += -5;
	AILocation.r[3].m128_f32[0] += -5;
	//CreateSimpleGunAi(&tThisWorld, AILocation);



	AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[0] += -13;
	AILocation.r[3].m128_f32[2] -= -15;
	XMMATRIX CoverLocation = AILocation;
	AILocation = XMMatrixLookAtLH(AILocation.r[3],
		m_d3dPlayerMatrix.r[3], XMVectorSet(0, 1, 0, 0));
	AILocation = XMMatrixInverse(NULL, AILocation);

	CoverLocation.r[3].m128_f32[2] += -1;
	vector<int> coverPosition;
	int cover1 = CreateCover(&tThisWorld, CoverLocation, coverPosition);
	CoverLocation.r[3].m128_f32[0] += 10;
	int cover2 = CreateCover(&tThisWorld, CoverLocation, coverPosition);
	tThisWorld.atSimpleMesh[cover1].m_nColor = XMFLOAT4(0, 0, 1, 1);
	tThisWorld.atSimpleMesh[cover2].m_nColor = XMFLOAT4(0, 0, 1, 1);

	XMMATRIX coverTriggerMatrix = CoverLocation;
	coverTriggerMatrix.r[3].m128_f32[2] -= 14;
	coverTriggerMatrix.r[3].m128_f32[1] -= 1;
	coverTriggerMatrix.r[3].m128_f32[0] -= 8;

	vector<int> coverIndexs;
	coverIndexs.push_back(cover2);

	//int coverTrigerIndex=CreateCoverTriggerZone(&tThisWorld, coverTriggerMatrix);
	coverTriggerMatrix.r[3].m128_f32[2] += 10;
	coverTriggerMatrix.r[3].m128_f32[0] += -3;

	int coverTrigerIndex2 = CreateCoverTriggerZone(&tThisWorld, coverTriggerMatrix);
	coverTriggerMatrix.r[3].m128_f32[0] += 10;

	int coverTrigerIndex3 = CreateCoverTriggerZone(&tThisWorld, coverTriggerMatrix);


	//tThisWorld.atSimpleMesh[coverTrigerIndex].m_nColor = XMFLOAT4(0, 0, 1, 1);
	tThisWorld.atSimpleMesh[coverTrigerIndex2].m_nColor = XMFLOAT4(0, 0, 1, 1);
	tThisWorld.atSimpleMesh[coverTrigerIndex3].m_nColor = XMFLOAT4(0, 0, 1, 1);




	XMMATRIX nodeLocation = CoverLocation;
	nodeLocation.r[3].m128_f32[0] += 3;
	nodeLocation.r[3].m128_f32[1] -= 1;
	nodeLocation.r[3].m128_f32[2] += 2;

	//pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	int nodeindex = CreateNodePoint(&tThisWorld, nodeLocation);
	XMFLOAT3 nodePosition;
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex, nodePosition, 1);
	nodeLocation = AILocation;
	nodeLocation.r[3].m128_f32[2] += 2;
	int nodeindex2 = CreateNodePoint(&tThisWorld, AILocation);
	tThisWorld.atCover[cover1].CoverPositions.push_back(nodeindex);
	tThisWorld.atCover[cover2].CoverPositions.push_back(nodeindex2);
	/*tThisWorld.atCoverTrigger[coverTrigerIndex2].coverAiCanGoTo.push_back(tThisWorld.atCover[cover1]);
	tThisWorld.atCoverTrigger[coverTrigerIndex].coverAiCanGoTo.push_back(tThisWorld.atCover[cover2]);
	tThisWorld.atCoverTrigger[coverTrigerIndex3].coverAiCanGoTo.push_back(tThisWorld.atCover[cover2]);
	*/
	AILocation.r[3].m128_f32[1] -= 1;
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex2, nodePosition, 1);
	AILocation.r[3].m128_f32[2] -= 4;
	AILocation.r[3].m128_f32[0] -= 3;
	nodePosition.x = AILocation.r[3].m128_f32[0];
	nodePosition.y = AILocation.r[3].m128_f32[1];
	nodePosition.z = AILocation.r[3].m128_f32[2];
	int nodeindex3 = CreateNodePoint(&tThisWorld, AILocation);
	pcAiSystem->AddNodeToPathFinding(nodeindex3, nodePosition, 1);
	tThisWorld.atCoverTrigger[coverTrigerIndex2].coverAiCanGoTo.push_back(tThisWorld.atCover[cover1]);
	//tThisWorld.atCoverTrigger[coverTrigerIndex].coverAiCanGoTo.push_back(tThisWorld.atCover[cover2]);
	tThisWorld.atCoverTrigger[coverTrigerIndex3].coverAiCanGoTo.push_back(tThisWorld.atCover[cover2]);


	vector<int> edges;
	edges.push_back(nodeindex);
	pcAiSystem->AddEdgestoNode(nodeindex2, edges);
	pcAiSystem->AddEdgestoNode(nodeindex3, edges);

	edges.clear();
	edges.push_back(nodeindex2);
	pcAiSystem->AddEdgestoNode(nodeindex, edges);
	edges.clear();
	edges.push_back(nodeindex2);
	pcAiSystem->AddEdgestoNode(nodeindex3, edges);



	int spacePirate = CreateSpacePirate(&tThisWorld, AILocation);
	tThisWorld.atAiHeath[spacePirate].heath = 100;

	//tThisWorld.atPathPlanining[spacePirate].Goal = nodeindex;
	tThisWorld.atPathPlanining[spacePirate].startingNode = nodeindex3;
	tThisWorld.atPathPlanining[spacePirate].Goal = nodeindex2;


	int GunINdexai = CreateGun(&tThisWorld, m_d3dWorldMatrix, spacePirate, -1.1, 0.5, 11.5, 10, 70);
	tThisWorld.atAIMask[spacePirate].GunIndex = GunINdexai;

	tThisWorld.atClip[GunINdexai].bulletSpeed = 0.0001;//Frame Dependent
	std::array<plane_t, 6> planes;
	float4x4 AiFrustum;

	AiFrustum.row1.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[0];
	AiFrustum.row1.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[1];
	AiFrustum.row1.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[2];
	AiFrustum.row1.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[3];

	AiFrustum.row2.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[0];
	AiFrustum.row2.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[1];
	AiFrustum.row2.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[2];
	AiFrustum.row2.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[3];

	AiFrustum.row3.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[0];
	AiFrustum.row3.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[1];
	AiFrustum.row3.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[2];
	AiFrustum.row3.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[3];

	AiFrustum.row4.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[0];
	AiFrustum.row4.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[1];
	AiFrustum.row4.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[2];
	AiFrustum.row4.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[3];

	frustumIndex = pcAiSystem->calculate_frustum(&tThisWorld, planes, AiFrustum, 70, 1, 0.1, 20, spacePirate, -2.1, 1.4, 19.6);
	tThisWorld.atWorldMatrix[frustumIndex].worldMatrix = AILocation;

	tThisWorld.atAIVision[spacePirate].eyes0 = planes;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[0] = planes[0].normal;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[1] = planes[1].normal;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[2] = planes[2].normal;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[3] = planes[3].normal;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[4] = planes[4].normal;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[5] = planes[5].normal;
	AILocation = m_d3dWorldMatrix;
	AILocation.r[3].m128_f32[0] += -3;
	AILocation.r[3].m128_f32[2] += -5;

	//CreateSimpleGunAi(&tThisWorld, AILocation);
	XMMATRIX groundSpawnPoint;
	groundSpawnPoint = m_d3dWorldMatrix;
	groundSpawnPoint.r[3].m128_f32[1] -= 2;
	int groundindex = CreateGround(&tThisWorld, groundSpawnPoint);
	groundSpawnPoint.r[3].m128_f32[2] -= -20;
	int groundindex2 = CreateGround(&tThisWorld, groundSpawnPoint);
	tThisWorld.atSimpleMesh[groundindex].m_nColor = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	tThisWorld.atSimpleMesh[groundindex2].m_nColor = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	tThisWorld.atAIVision[spacePirate].indexLookingAt = PlayerStartIndex;

	tempImport = pcGraphicsSystem->ReadMesh("meshData_Example_Objects.txt");

	for (int meshIndex = 1; meshIndex < tempImport.meshCount; meshIndex++)
	{
		//int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	}

	tempImport = pcGraphicsSystem->ReadMesh("meshData_ScifiRoom.txt");

	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		//int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	}
	//AILocation.r[3].m128_f32[0] -= 7;
	//int nodeLocation = CreateNodePoint(&tThisWorld, AILocation);
	//tThisWorld.atSimpleMesh[nodeLocation].m_nColor = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	//XMFLOAT3 nodePosition;
	//nodePosition.x = AILocation.r[3].m128_f32[0];
	//nodePosition.y = AILocation.r[3].m128_f32[1];
	//nodePosition.z = AILocation.r[3].m128_f32[2];

	//pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	//XMMATRIX part2ofNodeMovement = AILocation;
	//part2ofNodeMovement.r[3].m128_f32[2] -= -10;
	//int nodelocation4 = CreateNodePoint(&tThisWorld, part2ofNodeMovement);
	//nodePosition.x = part2ofNodeMovement.r[3].m128_f32[0];
	//nodePosition.y = part2ofNodeMovement.r[3].m128_f32[1];
	//nodePosition.z = part2ofNodeMovement.r[3].m128_f32[2];

	//pcAiSystem->AddNodeToPathFinding(nodelocation4, nodePosition, 1);
	//AILocation.r[3].m128_f32[0] += 4;
	//AILocation.r[3].m128_f32[2] += -4;

	//int nodeLocation2 = CreateNodePoint(&tThisWorld, AILocation);

	//nodePosition.x = AILocation.r[3].m128_f32[0];
	//nodePosition.y = AILocation.r[3].m128_f32[1];
	//nodePosition.z = AILocation.r[3].m128_f32[2];

	//pcAiSystem->AddNodeToPathFinding(nodeLocation2, nodePosition, 1);
	//XMMATRIX pathfindAI = AILocation;
	//pathfindAI.r[3].m128_f32[0] += 3;
	//nodePosition.x = pathfindAI.r[3].m128_f32[0];
	//nodePosition.y = pathfindAI.r[3].m128_f32[1];
	//nodePosition.z = pathfindAI.r[3].m128_f32[2];
	//int aiPathIndex = CreateTestAIPathFinding(&tThisWorld, pathfindAI);
	//tThisWorld.atSimpleMesh[aiPathIndex].m_nColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	//int nodelocation3 = CreateNodePoint(&tThisWorld, pathfindAI);
	//tThisWorld.atPathPlanining[aiPathIndex].startingNode = nodelocation3;
	//tThisWorld.atPathPlanining[aiPathIndex].Goal = nodeLocation;
	//pcAiSystem->AddNodeToPathFinding(nodelocation3, nodePosition, 1);
	//vector<int> edges;
	//edges.push_back(nodelocation4);
	//edges.push_back(nodeLocation2);
	//edges.push_back(nodelocation3);
	//pcAiSystem->AddEdgestoNode(nodeLocation, edges);
	//edges.clear();
	//edges.push_back(nodeLocation);
	//edges.push_back(nodelocation3);
	//pcAiSystem->AddEdgestoNode(nodeLocation2, edges);
	//edges.clear();
	////	edges.push_back(nodeLocation);
	//edges.push_back(nodeLocation2);
	//pcAiSystem->AddEdgestoNode(nodelocation3, edges);



	if (pcCollisionSystem->m_AAbb.size() != 0) {
		float x = 0;
	}

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{

		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask > 1)
		{

			if (tThisWorld.atSimpleMesh[nCurrentEntity].m_nVertexCount > tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount)
			{

				TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[nCurrentEntity].m_VertexData, tThisWorld.atAABB[nCurrentEntity]);
				MyAbb.m_IndexLocation = nCurrentEntity;
				tThisWorld.atAABB[nCurrentEntity] = MyAbb;
				pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);
				if (nCurrentEntity == door1Index || nCurrentEntity == door2Index || swordGuy == nCurrentEntity) {
					pcCollisionSystem->AddAiVisioNCheck(MyAbb, nCurrentEntity);
				}

			}
			if (tThisWorld.atMesh[nCurrentEntity].m_nVertexCount > tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount)
			{
				TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atMesh[nCurrentEntity].m_VertexData, tThisWorld.atAABB[nCurrentEntity]);
				MyAbb.m_IndexLocation = nCurrentEntity;
				tThisWorld.atAABB[nCurrentEntity] = MyAbb;
				pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);
				if (nCurrentEntity == PlayerStartIndex) {
					pcCollisionSystem->AddAiVisioNCheck(MyAbb, nCurrentEntity);

				}
			}
		}
	}

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask > 1)
		{

			tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
		}
	}


	pcGraphicsSystem->CreateBuffers(&tThisWorld);

}

int CGameMangerSystem::SpacePirateGamePlay()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(-1); //Important!
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective(m_RealTimeFov);

	//tTimerInfo->applicationTime = tTimerInfo->GetTime(tAugerTimers->tAppTimer, tTimerInfo->applicationTime);
	//tTimerInfo->sceneTime = tTimerInfo->GetTime(tAugerTimers->tSceneTimer, tTimerInfo->sceneTime);

	tCameraMode = pcInputSystem->CameraModeListen(tCameraMode);

	static XMMATRIX m_d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	static XMMATRIX m_d3dOffsetMatrix = pcGraphicsSystem->SetDefaultOffset();

	if (pcInputSystem->InputCheck(G_KEY_P))
	{
		return 3;
	}
	// ui stuff
	if (GamePaused == true) {
		if (DrawUI == true) {
			XMMATRIX UiPos = tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix;
			//UiPos.r[3].m128_f32[1] -= 1;
			XMVECTOR foward;
			foward.m128_f32[0] = 0;
			foward.m128_f32[1] = 1;
			foward.m128_f32[2] = 2;
			//	foward.m128_f32[0] = 1;


			XMMATRIX localMatrix2 = XMMatrixTranslationFromVector(foward);

			UiPos = XMMatrixMultiply(localMatrix2, UiPos);
			DrawUI = false;
			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] -= 1;
			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] += 1;
			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] += 1;
			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] += 1;
			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[0] += 1;
			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[0] += 0.8;
			UiPos.r[3].m128_f32[1] -= 0.3;

			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] -= 0.7;

			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[1] -= 0.7;

			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
			UiPos.r[3].m128_f32[0] -= 1;

			UIIndex.push_back(CreateTemptUIBox(&tThisWorld, UiPos));
			pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, UIIndex.back());
		}
	}

	if (tCameraMode.bWalkMode == true)
	{
		if (tCameraMode.bSwitch == true)
		{
			m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
			tCameraMode.bSwitch = false;
		}
		m_d3d_ResultMatrix = pcInputSystem->WalkCameraControls(XMVectorSet(0, 1.0f, 0, 0), m_d3d_ResultMatrix, bMoving);

		walkCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);
		walkCamera->d3d_Position = XMMatrixMultiply(m_d3dOffsetMatrix, walkCamera->d3d_Position);






	}
	else if (tCameraMode.bAimMode == true)
	{
		if (tCameraMode.bSwitch == true)
		{
			m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
			//CameraNewPosition = pcInputSystem->CameraBehaviorLerp(m_d3d_ResultMatrix, m_d3dPlayerMatrix);
			//			m_d3dPlayerMatrix = pcInputSystem->AimMode(m_d3dPlayerMatrix);

			CameraNewPosition = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);

			CameraNewPosition = XMMatrixMultiply(m_d3dOffsetMatrix, CameraNewPosition);
			aimCamera->d3d_Position = pcInputSystem->CameraBehaviorLerp(walkCamera->d3d_Position, CameraNewPosition, scale);
			scale += 0.001;
			if (scale > 1) {
				tCameraMode.bSwitch = false;
				scale = 0;

			}

		}
		else {
			m_RealTimeFov = pcInputSystem->ZoomSight(m_RealTimeFov);


			//			m_d3dPlayerMatrix = pcInputSystem->AimMode(m_d3dPlayerMatrix);

			aimCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);

			aimCamera->d3d_Position = XMMatrixMultiply(m_d3dOffsetMatrix, aimCamera->d3d_Position);
		}

	}
	else
	{

		if (tCameraMode.bSwitch == true)
		{
			m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
			tCameraMode.bSwitch = false;
		}
		m_d3d_ResultMatrix = pcInputSystem->DebugCamera(m_d3d_ResultMatrix, m_d3dWorldMatrix);

		debugCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dWorldMatrix);
	}

	CGraphicsSystem::TPrimalVertexBufferType tTempVertexBuffer;
	CGraphicsSystem::TPrimalPixelBufferType tTempPixelBuffer;
	CGraphicsSystem::TMyVertexBufferType tMyVertexBufferTemp;
	tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);



	pcGraphicsSystem->UpdateD3D();
#pragma region Input Garbage
	// togle the modes that you are in
	if (pcInputSystem->InputCheck(G_BUTTON_MIDDLE)) {
		tThisWorld.atClip[GunIndexForPlayer].GunMode = !tThisWorld.atClip[GunIndexForPlayer].GunMode;
	}
	// shoot a bullet
	if (pcInputSystem->InputCheck(G_BUTTON_LEFT) == 1 && tThisWorld.atClip[GunIndexForPlayer].GunMode == true) {

		tThisWorld.atClip[GunIndexForPlayer].tryToShoot = true;


	}
	// shoot a ray
	else if (pcInputSystem->InputCheck(G_BUTTON_LEFT) == 1 && tThisWorld.atClip[GunIndexForPlayer].GunMode == false) {
		tThisWorld.atClip[GunIndexForPlayer].tryToShoot = true;

	}
	// turn the ray off
	else if (tThisWorld.atClip[GunIndexForPlayer].GunMode == false)
	{
		tThisWorld.atClip[GunIndexForPlayer].tryToShoot = false;

	}

	// reload
	if (pcInputSystem->InputCheck(G_KEY_R) == 1) {

		tThisWorld.atClip[GunIndexForPlayer].tryToReload = true;

	}

#pragma endregion

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		{
			if (nCurrentEntity == rayindex) {
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix;
			}
			pcGraphicsSystem->InitPrimalShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tMyVertexBufferTemp.m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], tMyVertexBufferTemp.m_d3dViewMatrix);

			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

		}
		// ai code would run here
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			if (tCameraMode.bWalkMode == true)
			{
				tMyVertexBufferTemp.m_d3dViewMatrix = walkCamera->d3d_Position;
				tTempVertexBuffer.m_d3dViewMatrix = walkCamera->d3d_Position;
			}
			else if (tCameraMode.bAimMode == true)
			{
				tTempVertexBuffer.m_d3dViewMatrix = aimCamera->d3d_Position;
				tMyVertexBufferTemp.m_d3dViewMatrix = aimCamera->d3d_Position;

			}
			else if (tCameraMode.bDebugMode == true)
			{
				tTempVertexBuffer.m_d3dWorldMatrix = m_d3dWorldMatrix;
				tTempVertexBuffer.m_d3dViewMatrix = debugCamera->d3d_Position;
				tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;

			}
			else {
				tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
				tTempVertexBuffer.m_d3dViewMatrix = m_d3dViewMatrix;
				tMyVertexBufferTemp.m_d3dViewMatrix = m_d3dViewMatrix;
			}

			if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK))
			{
				if (tCameraMode.bWalkMode == true)
				{
					m_d3dPlayerMatrix = pcInputSystem->CharacterMovement(m_d3dPlayerMatrix);
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
					m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
				}
				else if (tCameraMode.bAimMode == true)
				{
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
					m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
				}
			}
			if (tCameraMode.bWalkMode == true)
			{
				pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], walkCamera->d3d_Position);
			}
			else if (tCameraMode.bAimMode == true)
			{
				pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], aimCamera->d3d_Position);
			}
			else
			{
				pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);
			}


			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

		if (pcAiSystem->GetNumberOfAI() <= 0)
		{
			return -1;
		}
		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_FOLLOW) || tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_SHOOT | COMPONENT_AIMASK | COMPONENT_FOLLOW) || (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST) == tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask)
		{
#if AI_ON				
			if (tThisWorld.atActiveAI[nCurrentEntity].active == true) {
				pcAiSystem->LookAtObject(tThisWorld.atWorldMatrix[tThisWorld.atAIVision[nCurrentEntity].indexLookingAt].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
				pcAiSystem->ShootGun(&tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex]);
			}
#endif // AI_ON
		}

		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SEARCH) || tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SPOTEDPLAYER) || (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST) == tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask) {



			if (tThisWorld.atAIVision[nCurrentEntity].keepSearching == true) {
				if (tThisWorld.atAIVision[nCurrentEntity].visionRotation < 7 && tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true) {
					tThisWorld.atAIVision[nCurrentEntity].visionRotation += 0.001;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcAiSystem->LookBackLeftToRight(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix,
						true);
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true) {
					tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight = false;
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].visionRotation > -7 && tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == false) {
					tThisWorld.atAIVision[nCurrentEntity].visionRotation -= 0.001;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcAiSystem->LookBackLeftToRight(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix,
						false);
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == false) {
					tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight = true;
				}
			}
			else if (tThisWorld.atAIVision[nCurrentEntity].keepSearching == false) {
				pcAiSystem->LookAtObject(tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
			}

			float4x4 AiFrustum;
			AiFrustum.row1.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[0];
			AiFrustum.row1.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[1];
			AiFrustum.row1.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[2];
			AiFrustum.row1.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[3];

			AiFrustum.row2.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[0];
			AiFrustum.row2.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[1];
			AiFrustum.row2.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[2];
			AiFrustum.row2.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[3];

			AiFrustum.row3.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[0];
			AiFrustum.row3.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[1];
			AiFrustum.row3.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[2];
			AiFrustum.row3.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[3];

			AiFrustum.row4.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[0];
			AiFrustum.row4.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1];
			AiFrustum.row4.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[2];
			AiFrustum.row4.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[3];
			pcAiSystem->UpdateFrustum(tThisWorld.atAIVision[nCurrentEntity].eyes0, AiFrustum, 70, 1, 0.1, 20);
			//	 pcAiSystem->calculate_frustum(&tThisWorld,tThisWorld.atAIVision[nCurrentEntity].eyes0, AiFrustum,70,1,0.1,20, nCurrentEntity, -2.1, 1.4, 19.6);
			vector<int> indicies;
			if (pcCollisionSystem->AiVisionCheck(tThisWorld.atAIVision[nCurrentEntity].eyes0, &indicies) == true) {
				float x = 0;
				bool danger = false;
				for (int i = 0; i < indicies.size(); ++i)
				{
					if (PlayerStartIndex == indicies[i])
					{
						tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
						tThisWorld.atAIVision[nCurrentEntity].keepSearching = false;
						tThisWorld.atActiveAI[nCurrentEntity].active = true;
						danger = true;
						if (tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].nBulletsAvailables.size() <= 0)
						{
							tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].tryToReload = true;

						}
						tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].tryToShoot = true;

						pcAiSystem->AddAiInCombat(nCurrentEntity);
						/*	if (tThisWorld.atPathPlanining[nCurrentEntity].foundDestination == true) {
						int previousgoal = tThisWorld.atPathPlanining[nCurrentEntity].Goal;
						int previousStartPosition = tThisWorld.atPathPlanining[nCurrentEntity].startingNode;
						tThisWorld.atPathPlanining[nCurrentEntity].Goal = previousStartPosition;
						tThisWorld.atPathPlanining[nCurrentEntity].startingNode = previousgoal;
						tThisWorld.atPathPlanining[nCurrentEntity].foundDestination = false;
						tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding = true;

						}*/

					}
					else if (tThisWorld.atProjectiles[indicies[i]].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL))
					{
						danger = true;
						tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
					}
					else if (danger == false)
					{
						tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
					}
					else if (tThisWorld.atAIMask[indicies[i]].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_FIGHTINGAI))
					{
						float x = 0;
					}
				}
			}



			/*	if (cloasestPlayer < closestWall) {
			XMFLOAT4 red;
			red.y = 0;
			red.z = 0;
			red.w = 1;
			red.x = 1;
			tThisWorld.atSimpleMesh[8].m_nColor = red;
			spotedplayer = true;
			tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask = COMPONENT_AIMASK | COMPONENT_SPOTEDPLAYER;
			tThisWorld.atAIVision[nCurrentEntity].playerLastKnownLocation = tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix;
			tThisWorld.atAIVision[nCurrentEntity].keepSearching = false;
			}
			else {
			spotedplayer = false;
			tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask = COMPONENT_AIMASK | COMPONENT_SEARCH;
			}*/

		}
		//else if(tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask==())
		if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_PATHFINDTEST) || (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST) == tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask) {
			if (tThisWorld.atActiveAI[nCurrentEntity].active == true) {
				if (tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding == true) {

					pcAiSystem->FindBestPath(tThisWorld.atPathPlanining[nCurrentEntity].startingNode,
						tThisWorld.atPathPlanining[nCurrentEntity].Goal,
						&tThisWorld.atPathPlanining[nCurrentEntity].directions);

					tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding = false;
				}
				else {
					if (tThisWorld.atPathPlanining[nCurrentEntity].DelayMovement <= 0) {
						pcAiSystem->PathPlaningMovement(&tThisWorld.atPathPlanining[nCurrentEntity], &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
					}
					else {
						tThisWorld.atPathPlanining[nCurrentEntity].DelayMovement -= 0.001;
					}
				}
			}
		}

		if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_CLIP))
		{
			if (tThisWorld.atClip[nCurrentEntity].GunMode == false && tThisWorld.atClip[nCurrentEntity].tryToShoot == true)
			{
				if (tThisWorld.atClip[nCurrentEntity].maderay == false)
				{
					XMMATRIX gun = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
					gun.r[3].m128_f32[0] += 1;
					rayindex = CreateRayBullet(&tThisWorld, gun, 10, GunIndexForPlayer, -0.6, 0.6, 10.7);
					pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, rayindex);
					tThisWorld.atClip[nCurrentEntity].maderay = true;
				}

			}
			else if (tThisWorld.atClip[nCurrentEntity].GunMode == false && tThisWorld.atClip[nCurrentEntity].tryToShoot == false && tThisWorld.atClip[nCurrentEntity].maderay == true)
			{
				pcGraphicsSystem->CleanD3DObject(&tThisWorld, rayindex);
				rayindex = -1;
				tThisWorld.atClip[nCurrentEntity].maderay = false;

			}

			else
			{
				if (nCurrentEntity == 10 && tThisWorld.atClip[nCurrentEntity].tryToShoot == true && tThisWorld.atClip[nCurrentEntity].fShootingCoolDown >= 0) {
					float x = 0;
				}
				if (tThisWorld.atClip[nCurrentEntity].tryToShoot == true && tThisWorld.atClip[nCurrentEntity].nBulletsAvailables.size() > 0 && tThisWorld.atClip[nCurrentEntity].fShootingCoolDown <= 0)
				{


					XMVECTOR foward;
					foward.m128_f32[0] = 0;
					foward.m128_f32[1] = 0;
					foward.m128_f32[2] = 1;



					XMMATRIX localMatrix2 = XMMatrixTranslationFromVector(foward);
					XMMATRIX gunMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
					gunMatrix = XMMatrixMultiply(localMatrix2, gunMatrix);


					int newbullet = CreateBullet(&tThisWorld, gunMatrix,
						tThisWorld.atClip[nCurrentEntity].currentMaterial, 0); // Last zero is bulletType - 0 for friend, 1 for enemy
					tThisWorld.atClip[newbullet].gunIndex = nCurrentEntity;
					tThisWorld.atSimpleMesh[newbullet].m_nColor = tThisWorld.atClip[nCurrentEntity].colorofBullets;
					tThisWorld.atClip[newbullet].indexInclip = pcProjectileSystem->CreateBulletProjectile(newbullet, &tThisWorld.atClip[nCurrentEntity]);
					tThisWorld.atAABB[newbullet] = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[newbullet].m_VertexData, tThisWorld.atAABB[newbullet]);
					tThisWorld.atAABB[newbullet].m_IndexLocation = newbullet;

					pcCollisionSystem->AddAABBCollider(tThisWorld.atAABB[newbullet], newbullet);
					pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, newbullet);
					tThisWorld.atClip[nCurrentEntity].tryToShoot = false;


				}
				else if (tThisWorld.atClip[nCurrentEntity].tryToShoot == true) {
					tThisWorld.atClip[nCurrentEntity].tryToShoot = false;
				}

				if (tThisWorld.atClip[nCurrentEntity].tryToReload == true) {
					pcProjectileSystem->Reload(&tThisWorld.atClip[nCurrentEntity]);
					tThisWorld.atClip[nCurrentEntity].tryToReload = false;

				}
				//if (tThisWorld.atClip[nCurrentEntity].nBulletsFired.size() != 0) {
				//	for (int i = 0; i < tThisWorld.atClip[nCurrentEntity].nBulletsFired.size(); ++i) {
				//		if (tThisWorld.atClip[nCurrentEntity].fAliveTime[i] < tThisWorld.atClip[nCurrentEntity].FValueOfCoolDown) {
				//		/*	pcPhysicsSystem->AddBulletForce(&tThisWorld.atRigidBody[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]], tThisWorld.atClip[nCurrentEntity].bulletSpeed);
				//			tThisWorld.atClip[nCurrentEntity].fAliveTime[i] += 0.1;*/

				//		}
				//		else {
				//			/*tThisWorld.atClip[nCurrentEntity].fAliveTime.erase(tThisWorld.atClip[nCurrentEntity].fAliveTime.begin() + i);
				//			pcCollisionSystem->RemoveAABBCollider(tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]);
				//			pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]);
				//			tThisWorld.atClip[nCurrentEntity].nBulletsFired.erase(tThisWorld.atClip[nCurrentEntity].nBulletsFired.begin() + i);
				//			for (int i = 0; i < tThisWorld.atClip[nCurrentEntity].fAliveTime.size(); ++i) {
				//				if (tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]].indexInclip != 0) {
				//					tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].nBulletsFired[i]].indexInclip -= 1;
				//				}

				//			}
				//		}*/
				//	}

				//}
				//if (tThisWorld.atClip[nCurrentEntity].fShootingCoolDown > 0)
					tThisWorld.atClip[nCurrentEntity].fShootingCoolDown -= 0.1;
			}


		}


		if (nCurrentEntity == PlayerStartIndex) {
			float x = 0;
		}
		tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, true);

		if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_RAYGUN)) {
			float CloseEstObject = 10000000000000000000.0f;
			float* distanceCalucaltion = new float();
			for (list<TAABB>::iterator ptr = pcCollisionSystem->m_AAbb.begin(); ptr != pcCollisionSystem->m_AAbb.end(); ++ptr) {

				if (ptr->m_IndexLocation != PlayerStartIndex && ptr->m_IndexLocation != GunIndexForPlayer)
					if (pcCollisionSystem->intersectRayAABox2(XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[0], tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix)
						, XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[1], tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix), *ptr
					) == true) {
						//CloseEstObject = *distanceCalucaltion;
						tThisWorld.atClip[GunIndexForPlayer].currentMaterial = 0;
						cout << "turtle" << '/n';
						tThisWorld.atClip[GunIndexForPlayer].colorofBullets = tThisWorld.atSimpleMesh[ptr->m_IndexLocation].m_nColor;
					}
			}
			delete distanceCalucaltion;
		}


		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
			COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC) || tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
				COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)) {
			tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
		}


		if ((tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_TRIGGER) || tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)))
		{

			vector<int> otherCollisionsIndex;
			if (pcCollisionSystem->AABBtoAABBCollisionCheck(tThisWorld.atAABB[nCurrentEntity], &otherCollisionsIndex) == true)
			{
				for (int i = 0; i < otherCollisionsIndex.size(); ++i) {
					if (tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == true && tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask !=
						(COMPONENT_COLLISIONMASK |
							COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)) {
						if (nCurrentEntity == 8)
						{
							float x = 0;
						}

						tThisWorld.atRigidBody[nCurrentEntity].totalForce = -tThisWorld.atRigidBody[nCurrentEntity].velocity;


						tTempVertexBuffer.m_d3dWorldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcCollisionSystem->WalkingThrewObjectCheck(tTempVertexBuffer.m_d3dWorldMatrix, tThisWorld.atAABB[otherCollisionsIndex[i]], tThisWorld.atAABB[nCurrentEntity]);
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tTempVertexBuffer.m_d3dWorldMatrix;

					}
					if (tThisWorld.atRigidBody[nCurrentEntity].ground == false && tThisWorld.atRigidBody[nCurrentEntity].wall == false &&
						tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)
						&& (tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_STATIC) || tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
								COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))) {
						tTempVertexBuffer.m_d3dWorldMatrix = pcCollisionSystem->WalkingThrewObjectCheck(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[otherCollisionsIndex[i]], tThisWorld.atAABB[nCurrentEntity]);
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tTempVertexBuffer.m_d3dWorldMatrix;
						if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK)) {
							m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
						}

					}
					if (tThisWorld.atRigidBody[nCurrentEntity].ground == false && tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == false && tThisWorld.atRigidBody[nCurrentEntity].wall == false &&
						tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)
						&& (tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
							COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_STATIC)) || tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK |
								COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))
					{

						if (tThisWorld.atAIMask[otherCollisionsIndex[i]].m_tnAIMask >1) {
							if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
								if (tThisWorld.atAiHeath[otherCollisionsIndex[i]].heath <= 0) {
									pcAiSystem->SetNumberOfAI(pcAiSystem->GetNumberOfAI() - 1);
									if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1) {
									/*	tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.erase
										(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.begin()
											+ tThisWorld.atClip[nCurrentEntity].indexInclip);

										pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
										tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.erase(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.begin()
											+ tThisWorld.atClip[nCurrentEntity].indexInclip);
*/

										pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);

									}
									pcCollisionSystem->RemoveAABBCollider(otherCollisionsIndex[i]);
									pcGraphicsSystem->CleanD3DObject(&tThisWorld, otherCollisionsIndex[i]);
									pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atAIMask[otherCollisionsIndex[i]].GunIndex);
								}
								else {
								/*	tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.erase
									(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.begin()
										+ tThisWorld.atClip[nCurrentEntity].indexInclip);

									pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
									tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.erase(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.begin()
										+ tThisWorld.atClip[nCurrentEntity].indexInclip);*/


									pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);
									tThisWorld.atAiHeath[otherCollisionsIndex[i]].heath -= playerDamage;
									if (tThisWorld.atAiHeath[otherCollisionsIndex[i]].heath <= 0) {
										pcAiSystem->SetNumberOfAI(pcAiSystem->GetNumberOfAI() - 1);
										pcCollisionSystem->RemoveAABBCollider(otherCollisionsIndex[i]);
										pcGraphicsSystem->CleanD3DObject(&tThisWorld, otherCollisionsIndex[i]);
										pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atAIMask[otherCollisionsIndex[i]].GunIndex);
									}
								}
							}
						}
						else if (tThisWorld.atInputMask[otherCollisionsIndex[i]].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK)) {
							if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
								if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1) {
									//tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.erase
									//(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.begin()
									//	+ tThisWorld.atClip[nCurrentEntity].indexInclip);

									//pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
									//tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.erase(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.begin()
									//	+ tThisWorld.atClip[nCurrentEntity].indexInclip);

									////pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atAIMask[nCurrentEntity].GunIndex);
									//pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);

								}
								tThisWorld.atClayton[otherCollisionsIndex[i]].health -= pirateDamage;
							}


						}
						//if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL)) {
						//	if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1) {
						//		tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.erase
						//		(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].fAliveTime.begin()
						//			+ tThisWorld.atClip[nCurrentEntity].indexInclip);

						//		pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
						//		tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.erase(tThisWorld.atClip[tThisWorld.atClip[nCurrentEntity].gunIndex].nBulletsFired.begin()
						//			+ tThisWorld.atClip[nCurrentEntity].indexInclip);

						//		//pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atAIMask[nCurrentEntity].GunIndex);
						//		pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);

						//	}
						//}

					}
					if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK) && tThisWorld.atAIMask[otherCollisionsIndex[i]].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_COVERTRIGGER)) {

						pcAiSystem->MoveAiToCoverLocation(tThisWorld.atCoverTrigger[otherCollisionsIndex[i]], &tThisWorld);
					}
				}
				if (tThisWorld.atClayton[PlayerStartIndex].health <= 0)
				{
					return -1;
				}
				//		tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);


			}
			else
			{

				//tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;

			}
		}
		else {

			//	tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;

		}
		tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;

		if (tThisWorld.atParentWorldMatrix[nCurrentEntity] != -1)
		{
			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(pcGraphicsSystem->SetDefaultWorldPosition(),
				tThisWorld.atWorldMatrix[tThisWorld.atParentWorldMatrix[nCurrentEntity]].worldMatrix);

			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(tThisWorld.atOffSetMatrix[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);

			if (nCurrentEntity == rayindex) {
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix;
			}
			if (nCurrentEntity == frustumIndex) {
				XMMATRIX empty;
				empty = pcGraphicsSystem->SetDefaultWorldPosition();
				XMMATRIX rotation;
				XMMATRIX d3dTmpViewM;
				rotation = XMMatrixRotationY(-0.3);
				empty.r[1] = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1];
				empty.r[0] = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0];
				d3dTmpViewM = empty;
				XMVECTOR d3d_newX, d3d_newY, d3d_existingZ;
				d3dTmpViewM = XMMatrixMultiply(rotation, d3dTmpViewM);
				d3d_existingZ = d3dTmpViewM.r[2];
				d3d_newX = XMVector3Cross(empty.r[1], d3d_existingZ);
				d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

				d3d_newX = XMVector3Normalize(d3d_newX);
				d3d_newY = XMVector3Normalize(d3d_newY);

				d3d_existingZ = XMVector3Normalize(d3d_existingZ);

				d3dTmpViewM.r[0] = d3d_newX;
				d3dTmpViewM.r[1] = d3d_newY;
				d3dTmpViewM.r[2] = d3d_existingZ;
				empty = d3dTmpViewM;
				empty.r[3].m128_f32[0] = 3;
				empty.r[3].m128_f32[1] = 0;
				empty.r[3].m128_f32[2] = 1;


				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = empty;
			}
		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID))
		{
			if (tCameraMode.bWalkMode == true)
			{
				pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity], walkCamera->d3d_Position);

			}
			else if (tCameraMode.bAimMode == true)
			{
				pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity], aimCamera->d3d_Position);

			}
			else
			{
				pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity], debugCamera->d3d_Position);
			}
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atSimpleMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}
		if (tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix.r[3].m128_f32[1] < -10) {
			return 7;
		}

	}



	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);
	zValue += 0.001;
	return 8;


}

void CGameMangerSystem::LoadMikesGraphicsSandbox()
{
	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);
	for (int i = 0; i < matOpt.numberOfMaterials; i++)
	{
		matOpt.SRVArrayOfMaterials[i]->Release();
	}
	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();//Call some sort of function from the graphics system to create this matrix
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();

	debugCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	tCameraMode.bDebugMode = true;
	tCameraMode.bAimMode = false;
	tCameraMode.bWalkMode = false;
	tCameraMode.bSwitch = false;

	ImporterData tempImport;
	//tempImport = pcGraphicsSystem->ReadMesh("meshData_ScifiRoom.txt");

	//for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	//{
	//	int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	//}

#pragma region Create Skybox
	ID3D11Resource * spaceMap[6];

	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceLightBlue/left.png", &spaceMap[0], NULL, NULL);
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceLightBlue/right.png", &spaceMap[1], NULL, NULL);
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceLightBlue/top.png", &spaceMap[2], NULL, NULL);

	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceLightBlue/bot.png", &spaceMap[3], NULL, NULL);
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceLightBlue/front.png", &spaceMap[4], NULL, NULL);
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceLightBlue/back.png", &spaceMap[5], NULL, NULL);

	ID3D11ShaderResourceView * tempSrv = pcGraphicsSystem->TexturesToCubeMap(pcGraphicsSystem->m_pd3dDeviceContext, spaceMap);
	renderToTexturePassIndex = CreateSkybox(&tThisWorld, tempSrv);

#pragma endregion

	//tempImport = pcGraphicsSystem->ReadMesh("meshData_Satellite.txt");
	//matOpt = pcGraphicsSystem->CreateTexturesFromFile(tempImport.vtMaterials, tempImport.meshCount);
	//for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	//{
	//	int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], matOpt, meshIndex);
	//}

	tempImport = pcGraphicsSystem->ReadMesh("meshData_NoBrewery7.txt");
	matOpt = pcGraphicsSystem->CreateTexturesFromFile(tempImport.vtMaterials, tempImport.meshCount);
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], matOpt, meshIndex);
	}

	//tempImport = pcGraphicsSystem->ReadMesh("meshData_LaserFlintlock.txt");
	//matOpt =  pcGraphicsSystem->CreateTexturesFromFile(tempImport.vtMaterials, tempImport.meshCount);
	//for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	//{
	//	int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], matOpt, meshIndex);
	//}

	//tempImport = pcGraphicsSystem->ReadMesh("meshData_PirateMoveForward.txt");
	//matOpt = pcGraphicsSystem->CreateTexturesFromFile(tempImport.vtMaterials, tempImport.meshCount);
	//for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	//{
	//	int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], matOpt, meshIndex);
	//}

	//Create Clayton Animated
	tempImport = pcGraphicsSystem->ReadMesh("meshData_PirateMoveForward.txt");
	matOpt = pcGraphicsSystem->CreateTexturesFromFile(tempImport.vtMaterials, tempImport.meshCount);

	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		int myMesh = createClaytonAnim(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], matOpt, tempImport.vtAnimations[meshIndex], meshIndex);
	}

	//createGSQuad(&tThisWorld, XMFLOAT4(1, 0, 0, 1));
	//createGSQuad(&tThisWorld, XMFLOAT4(1, 1, 1, 1));

	pcGraphicsSystem->CreateBuffers(&tThisWorld);
	/*
	loop through all entities
	current entities System_Times StartClock

	*/

}

int CGameMangerSystem::MikesGraphicsSandbox()
{
#pragma region Camera
	static XMMATRIX m_d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	static XMMATRIX m_d3dOffsetMatrix = pcGraphicsSystem->SetDefaultOffset();
	tCameraMode = pcInputSystem->CameraModeListen(tCameraMode);
	if (pcInputSystem->InputCheck(G_KEY_P))
	{
		return 3;
	}

	if (pcInputSystem->InputCheck(G_KEY_Y))
	{
		if (Health > 0)
		{
			Health -= .1f;
		}
	}

	if (tCameraMode.bSwitch == true)
	{
		m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
		tCameraMode.bSwitch = false;
	}
	m_d3d_ResultMatrix = pcInputSystem->DebugCamera(m_d3d_ResultMatrix, m_d3dWorldMatrix);

	debugCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dWorldMatrix);

#pragma endregion

#pragma region Buffer Declarations
	CGraphicsSystem::TPrimalVertexBufferType tTempVertexBuffer;
	CGraphicsSystem::TPrimalPixelBufferType tTempPixelBuffer;
	CGraphicsSystem::TMyVertexBufferType tMyVertexBufferTemp;
	CGraphicsSystem::TAnimatedVertexBufferType tAnimVertexBuffer;
	tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

#pragma endregion

#pragma region Render To Texture Pass
	pcGraphicsSystem->UpdateD3D();
	pcGraphicsSystem->UpdateD3D_RenderToTexture();

	for (int nCurrentEntity = 0; nCurrentEntity <= renderToTexturePassIndex; nCurrentEntity++)
	{
		tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			tTempVertexBuffer.m_d3dWorldMatrix = m_d3dWorldMatrix;
			tTempVertexBuffer.m_d3dViewMatrix = debugCamera->d3d_Position;
			tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;

			pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_SKYBOX | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;
			tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
			pcGraphicsSystem->InitSkyboxShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}
	}

	pcGraphicsSystem->m_pd3dDevice->CreateShaderResourceView(pcGraphicsSystem->m_pd3dOutsideGlassRenderToTexture, NULL, &pcGraphicsSystem->m_pd3dOutsideGlassSRV);
	pcGraphicsSystem->m_pd3dDeviceContext->GenerateMips(pcGraphicsSystem->m_pd3dOutsideGlassSRV);

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			if (tThisWorld.atShaderID[nCurrentEntity].m_nShaderID == 10)
			{
				tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse = pcGraphicsSystem->m_pd3dOutsideGlassSRV;
			}
		}
	}

#pragma endregion

	pcGraphicsSystem->UpdateD3D();
	tThisWorld.atAnimation[0].tTimer.StartClock(tThisWorld.atAnimation[0].tTimer.tSceneTimer);
	//Start Time

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tAnimVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tAnimVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		{
			tTempVertexBuffer.m_d3dWorldMatrix = m_d3dWorldMatrix;
			tTempVertexBuffer.m_d3dViewMatrix = debugCamera->d3d_Position;
			tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;
			pcGraphicsSystem->InitQuadShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], debugCamera->d3d_Position, tThisWorld.atBar[nCurrentEntity].backgroundColor, Health);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			tTempVertexBuffer.m_d3dWorldMatrix = m_d3dWorldMatrix;
			tTempVertexBuffer.m_d3dViewMatrix = debugCamera->d3d_Position;
			tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;

			pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_ANIMATION | COMPONENT_SHADERID))
		{
			tAnimVertexBuffer.m_d3dWorldMatrix = m_d3dWorldMatrix;
			tAnimVertexBuffer.m_d3dViewMatrix = debugCamera->d3d_Position;
			tAnimVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
			XMFLOAT4X4 * tweenJoints = pcAnimationSystem->PlayAnimation(tThisWorld.atAnimationVariant[nCurrentEntity], tThisWorld.atAnimation[nCurrentEntity], tThisWorld.atAnimation[0].tTimer.localTime);
			memcpy(&tAnimVertexBuffer.m_d3dJointsForVS, &tweenJoints, sizeof(tweenJoints));
			pcGraphicsSystem->InitAnimShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tAnimVertexBuffer, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_SKYBOX | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;

			tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
			pcGraphicsSystem->InitSkyboxShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}
	}

	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);

	tThisWorld.atAnimation[0].tTimer.GetLocalTime(tThisWorld.atAnimation[0].tTimer.tSceneTimer, tThisWorld.atAnimation[0].tTimer.localTime);
	tThisWorld.atAnimation[0].tTimer.DisplayTimes(pcInputSystem);


	//End Time
	return 10;
}

void CGameMangerSystem::LoadLevelWithMapInIt()
{
	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);
	//Stops Main Menu Music 
#if MUSIC_ON
	AK::SoundEngine::StopAll();
	pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_HALLWAY_MUSIC, m_AkHallwayBattle);
#endif
	while (ShowCursor(false) > -1)
	{
		continue;
	}

	InitializeHUD();
	InitializePauseScreen();
	GameOver = false;
	GamePaused = false;
	endInit = false;
	pauseInit = false;
	mouseDown = false;
	mouseUp = true;
	click = false;

	pcAiSystem->SetNumberOfAI(3);
	//	tTimerInfo->StartClock(tAugerTimers->tSceneTimer);
	ImporterData tempImport;
	TMaterialOptimized matOpt;

	ImporterData gunImport;

#pragma region Create Skybox
	ID3D11Resource * spaceMap[6];

	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceLightBlue/left.png", &spaceMap[0], NULL, NULL);
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceLightBlue/right.png", &spaceMap[1], NULL, NULL);
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceLightBlue/top.png", &spaceMap[2], NULL, NULL);

	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceLightBlue/bot.png", &spaceMap[3], NULL, NULL);
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceLightBlue/front.png", &spaceMap[4], NULL, NULL);
	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceLightBlue/back.png", &spaceMap[5], NULL, NULL);

	ID3D11ShaderResourceView * tempSrv = pcGraphicsSystem->TexturesToCubeMap(pcGraphicsSystem->m_pd3dDeviceContext, spaceMap);
	renderToTexturePassIndex = CreateSkybox(&tThisWorld, tempSrv);

#pragma endregion

	tempImport = pcGraphicsSystem->ReadMesh("meshData_NoBrewery.txt");
	matOpt = pcGraphicsSystem->CreateTexturesFromFile(tempImport.vtMaterials, tempImport.meshCount);
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], matOpt, meshIndex);
	}

	//Current World Matrix Init
	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
																   //Current View matrix Init
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();
	//Init Walk Camera to Default Position
	walkCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	//Init Aim Camera to Default Position
	aimCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	//Init Debug Camera to Default Position
	debugCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	// Init Projection Matrix with 90.0f
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective(m_RealTimeFov);

	//Init Player matrix
	m_d3dPlayerMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	//Set Camera Modes to Default 
	tCameraMode.bWalkMode = false;
	tCameraMode.bDebugMode = false;
	tCameraMode.bAimMode = true;
	tCameraMode.bSwitch = false;

	bMoving = false;
	//Init FOV default
	m_RealTimeFov = 90.0f;
	//m_d3dPlayerMatrix = XMMatrixMultiply(m_d3dPlayerMatrix, XMMatrixScaling(.01, .01, .01));



	m_d3dPlayerMatrix.r[3].m128_f32[2] -= 10;
	m_d3dPlayerMatrix.r[3].m128_f32[0] -= 5;
	m_d3dPlayerMatrix.r[3].m128_f32[1] += 0.2;


	//Clayton Import Data - ZFB
	tempImport = pcGraphicsSystem->ReadMesh("meshData_Pirate.txt");

	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		PlayerStartIndex = createClayton(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	}

	GunIndexForPlayer = CreateGun(&tThisWorld, m_d3dWorldMatrix, PlayerStartIndex, -1, 1, 10.5, 3, 130);
	tThisWorld.atClip[GunIndexForPlayer].bulletSpeed = 0.01;
	tThisWorld.atClayton[PlayerStartIndex].health = 100;
	XMMATRIX AILocation = pcGraphicsSystem->SetDefaultWorldPosition();
	AILocation.r[3].m128_f32[2] -= 60;
	AILocation.r[3].m128_f32[0] -= 8;
	AILocation.r[3].m128_f32[1] += 0.2;
	pcAiSystem->LookAtObject(AILocation, &m_d3dPlayerMatrix);
	XMMATRIX nodeLocation = pcGraphicsSystem->SetDefaultWorldPosition();
	nodeLocation.r[3].m128_f32[2] -= 60;
	nodeLocation.r[3].m128_f32[0] -= 8;
	nodeLocation.r[3].m128_f32[1] += -1;

	int nodeindex = CreateNodePoint(&tThisWorld, nodeLocation);
	XMFLOAT3 nodePosition;
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	vector<int> edges;

	pcAiSystem->AddNodeToPathFinding(nodeindex, nodePosition, 1);
	nodeLocation.r[3].m128_f32[2] += 3;
	//nodeLocation.r[3].m128_f32[0] += 5;
	int nodeindex2 = CreateNodePoint(&tThisWorld, nodeLocation);

	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex2, nodePosition, 1);
	edges.push_back(nodeindex2);
	pcAiSystem->AddEdgestoNode(nodeindex, edges);
	edges.clear();
	edges.push_back(nodeindex);
	pcAiSystem->AddEdgestoNode(nodeindex2, edges);

	tempImport = pcGraphicsSystem->ReadMesh("meshData_Scyllian.txt");
	gunImport = pcGraphicsSystem->ReadMesh("meshData_LaserFlintlockTextured.txt");
	int spacePirate;
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; ++meshIndex)
	{
		spacePirate = CreateScyllian(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex], AILocation);
	}

	//spacePirate = CreateSpacePirate(&tThisWorld, AILocation);
	tThisWorld.atAiHeath[spacePirate].heath = 100;
	createGSQuad(&tThisWorld, XMFLOAT4(1, 0, 0, 1), spacePirate);
	createGSQuad(&tThisWorld, XMFLOAT4(0, 0, 0, 1), spacePirate);

	tThisWorld.atPathPlanining[spacePirate].Goal = nodeindex2;
	tThisWorld.atPathPlanining[spacePirate].startingNode = nodeindex;



	int GunINdexai;
	//int GunINdexai = CreateGun(&tThisWorld, m_d3dWorldMatrix, spacePirate, -1.1, 0.5, 12.5, 10, 30);
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; ++meshIndex)
	{
		GunINdexai = CreateScyllianGun(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, m_d3dWorldMatrix, spacePirate, -0.5, 1, 11.5, 10, 30, gunImport.vtMeshes[meshIndex], gunImport.vtMaterials[meshIndex]);
	}
	tThisWorld.atAIMask[spacePirate].GunIndex = GunINdexai;

	tThisWorld.atClip[GunINdexai].bulletSpeed = 0.01;//Frame Dependent
	std::array<plane_t, 6> planes;
	float4x4 AiFrustum;

	AiFrustum.row1.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[0];
	AiFrustum.row1.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[1];
	AiFrustum.row1.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[2];
	AiFrustum.row1.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[3];

	AiFrustum.row2.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[0];
	AiFrustum.row2.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[1];
	AiFrustum.row2.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[2];
	AiFrustum.row2.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[3];

	AiFrustum.row3.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[0];
	AiFrustum.row3.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[1];
	AiFrustum.row3.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[2];
	AiFrustum.row3.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[3];

	AiFrustum.row4.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[0];
	AiFrustum.row4.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[1];
	AiFrustum.row4.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[2];
	AiFrustum.row4.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[3];

	frustumIndex = pcAiSystem->calculate_frustum(&tThisWorld, planes, AiFrustum, 70, 1, 0.1, 20, spacePirate, -2.1, 1.4, 19.6);
	tThisWorld.atWorldMatrix[frustumIndex].worldMatrix = AILocation;

	tThisWorld.atAIVision[spacePirate].eyes0 = planes;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[0] = planes[0].normal;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[1] = planes[1].normal;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[2] = planes[2].normal;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[3] = planes[3].normal;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[4] = planes[4].normal;
	tThisWorld.atAIVision[spacePirate].normalAtBegining[5] = planes[5].normal;
	AILocation.r[3].m128_f32[0] += 3;

	int spacePirate2;
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; ++meshIndex)
	{
		spacePirate2 = CreateScyllian(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex], AILocation);
	}

	//spacePirate2 = CreateSpacePirate(&tThisWorld, AILocation);
	tThisWorld.atActiveAI[spacePirate].NoctifyOtherAi.push_back(spacePirate2);
	tThisWorld.atActiveAI[spacePirate2].NoctifyOtherAi.push_back(spacePirate);

	tThisWorld.atAiHeath[spacePirate2].heath = 100;
	createGSQuad(&tThisWorld, XMFLOAT4(1, 0, 0, 1), spacePirate2);
	createGSQuad(&tThisWorld, XMFLOAT4(0, 0, 0, 1), spacePirate2);

	//tThisWorld.atPathPlanining[spacePirate].Goal = nodeindex;
	/*tThisWorld.atPathPlanining[spacePirate].startingNode = nodeindex3;
	tThisWorld.atPathPlanining[spacePirate].Goal = nodeindex2;*/


	//int GunINdexai2 = CreateGun(&tThisWorld, m_d3dWorldMatrix, spacePirate2, -1.1, 0.5, 11.5, 10,130);
	int GunINdexai2;
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; ++meshIndex)
	{
		GunINdexai2 = CreateScyllianGun(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, m_d3dWorldMatrix, spacePirate2, -0.5, 1, 11.5, 10, 30, gunImport.vtMeshes[meshIndex], gunImport.vtMaterials[meshIndex]);
	}
	tThisWorld.atAIMask[spacePirate2].GunIndex = GunINdexai2;
	tThisWorld.atAIVision[spacePirate2].keepRotatingRight = false;

	tThisWorld.atClip[GunINdexai2].bulletSpeed = 0.0001;//Frame Dependent
	std::array<plane_t, 6> planes2;
	AiFrustum;

	AiFrustum.row1.x = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[0].m128_f32[0];
	AiFrustum.row1.y = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[0].m128_f32[1];
	AiFrustum.row1.z = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[0].m128_f32[2];
	AiFrustum.row1.w = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[0].m128_f32[3];

	AiFrustum.row2.x = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[1].m128_f32[0];
	AiFrustum.row2.y = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[1].m128_f32[1];
	AiFrustum.row2.z = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[1].m128_f32[2];
	AiFrustum.row2.w = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[1].m128_f32[3];

	AiFrustum.row3.x = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[2].m128_f32[0];
	AiFrustum.row3.y = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[2].m128_f32[1];
	AiFrustum.row3.z = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[2].m128_f32[2];
	AiFrustum.row3.w = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[2].m128_f32[3];

	AiFrustum.row4.x = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[3].m128_f32[0];
	AiFrustum.row4.y = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[3].m128_f32[1];
	AiFrustum.row4.z = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[3].m128_f32[2];
	AiFrustum.row4.w = tThisWorld.atWorldMatrix[spacePirate2].worldMatrix.r[3].m128_f32[3];

	frustumIndex = pcAiSystem->calculate_frustum(&tThisWorld, planes2, AiFrustum, 70, 1, 0.1, 20, spacePirate2, -2.1, 1.4, 19.6);
	tThisWorld.atWorldMatrix[frustumIndex].worldMatrix = AILocation;

	tThisWorld.atAIVision[spacePirate2].eyes0 = planes2;
	tThisWorld.atAIVision[spacePirate2].normalAtBegining[0] = planes2[0].normal;
	tThisWorld.atAIVision[spacePirate2].normalAtBegining[1] = planes2[1].normal;
	tThisWorld.atAIVision[spacePirate2].normalAtBegining[2] = planes2[2].normal;
	tThisWorld.atAIVision[spacePirate2].normalAtBegining[3] = planes2[3].normal;
	tThisWorld.atAIVision[spacePirate2].normalAtBegining[4] = planes2[4].normal;
	tThisWorld.atAIVision[spacePirate2].normalAtBegining[5] = planes2[5].normal;
	nodeLocation = AILocation;
	nodeLocation.r[3].m128_f32[1] += -1;

	int nodeindex3 = CreateNodePoint(&tThisWorld, nodeLocation);
	nodePosition;
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];


	pcAiSystem->AddNodeToPathFinding(nodeindex3, nodePosition, 1);
	nodeLocation.r[3].m128_f32[2] += 3;
	//nodeLocation.r[3].m128_f32[0] += 5;
	int nodeindex4 = CreateNodePoint(&tThisWorld, nodeLocation);
	edges.clear();
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex4, nodePosition, 1);
	edges.push_back(nodeindex4);
	pcAiSystem->AddEdgestoNode(nodeindex3, edges);
	edges.clear();
	edges.push_back(nodeindex3);
	pcAiSystem->AddEdgestoNode(nodeindex4, edges);

	tThisWorld.atPathPlanining[spacePirate2].Goal = nodeindex3;
	tThisWorld.atPathPlanining[spacePirate2].startingNode = nodeindex4;


	//int nodeindex2 = CreateNodePoint(&tThisWorld, nodeLocation);

	//nodePosition.x = nodeLocation.r[3].m128_f32[0];
	//nodePosition.y = nodeLocation.r[3].m128_f32[1];
	//nodePosition.z = nodeLocation.r[3].m128_f32[2];
	//pcAiSystem->AddNodeToPathFinding(nodeindex2, nodePosition, 1);
	//edges.push_back(nodeindex2);
	//pcAiSystem->AddEdgestoNode(nodeindex, edges);
	//edges.clear();
	//edges.push_back(nodeindex);
	//pcAiSystem->AddEdgestoNode(nodeindex2, edges);
	AILocation.r[3].m128_f32[2] -= 20;
	AILocation.r[3].m128_f32[0] -= 10;

	int spacePirate3;
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; ++meshIndex)
	{
		spacePirate3 = CreateScyllian(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex], AILocation);
	}

	//spacePirate3 = CreateSpacePirate(&tThisWorld, AILocation);
	tThisWorld.atAiHeath[spacePirate3].heath = 100;
	createGSQuad(&tThisWorld, XMFLOAT4(1, 0, 0, 1), spacePirate3);
	createGSQuad(&tThisWorld, XMFLOAT4(0, 0, 0, 1), spacePirate3);

	//tThisWorld.atPathPlanining[spacePirate].Goal = nodeindex;
	/*tThisWorld.atPathPlanining[spacePirate].startingNode = nodeindex3;
	tThisWorld.atPathPlanining[spacePirate].Goal = nodeindex2;*/


	//int GunINdexai3 = CreateGun(&tThisWorld, m_d3dWorldMatrix, spacePirate3, -1.1, 0.5, 12.5, 10, 70);
	int GunINdexai3;
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; ++meshIndex)
	{
		GunINdexai3 = CreateScyllianGun(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, m_d3dWorldMatrix, spacePirate3, -0.5, 1, 11.5, 10, 30, gunImport.vtMeshes[meshIndex], gunImport.vtMaterials[meshIndex]);
	}
	tThisWorld.atAIMask[spacePirate3].GunIndex = GunINdexai3;
	tThisWorld.atAIVision[spacePirate3].keepRotatingRight = false;

	tThisWorld.atClip[GunINdexai3].bulletSpeed = 0.0001;//Frame Dependent
	std::array<plane_t, 6> planes3;
	AiFrustum;

	AiFrustum.row1.x = tThisWorld.atWorldMatrix[spacePirate3].worldMatrix.r[0].m128_f32[0];
	AiFrustum.row1.y = tThisWorld.atWorldMatrix[spacePirate3].worldMatrix.r[0].m128_f32[1];
	AiFrustum.row1.z = tThisWorld.atWorldMatrix[spacePirate3].worldMatrix.r[0].m128_f32[2];
	AiFrustum.row1.w = tThisWorld.atWorldMatrix[spacePirate3].worldMatrix.r[0].m128_f32[3];

	AiFrustum.row2.x = tThisWorld.atWorldMatrix[spacePirate3].worldMatrix.r[1].m128_f32[0];
	AiFrustum.row2.y = tThisWorld.atWorldMatrix[spacePirate3].worldMatrix.r[1].m128_f32[1];
	AiFrustum.row2.z = tThisWorld.atWorldMatrix[spacePirate3].worldMatrix.r[1].m128_f32[2];
	AiFrustum.row2.w = tThisWorld.atWorldMatrix[spacePirate3].worldMatrix.r[1].m128_f32[3];

	AiFrustum.row3.x = tThisWorld.atWorldMatrix[spacePirate3].worldMatrix.r[2].m128_f32[0];
	AiFrustum.row3.y = tThisWorld.atWorldMatrix[spacePirate3].worldMatrix.r[2].m128_f32[1];
	AiFrustum.row3.z = tThisWorld.atWorldMatrix[spacePirate3].worldMatrix.r[2].m128_f32[2];
	AiFrustum.row3.w = tThisWorld.atWorldMatrix[spacePirate3].worldMatrix.r[2].m128_f32[3];

	AiFrustum.row4.x = tThisWorld.atWorldMatrix[spacePirate3].worldMatrix.r[3].m128_f32[0];
	AiFrustum.row4.y = tThisWorld.atWorldMatrix[spacePirate3].worldMatrix.r[3].m128_f32[1];
	AiFrustum.row4.z = tThisWorld.atWorldMatrix[spacePirate3].worldMatrix.r[3].m128_f32[2];
	AiFrustum.row4.w = tThisWorld.atWorldMatrix[spacePirate3].worldMatrix.r[3].m128_f32[3];

	frustumIndex = pcAiSystem->calculate_frustum(&tThisWorld, planes3, AiFrustum, 70, 1, 0.1, 20, spacePirate3, -2.1, 1.4, 19.6);
	tThisWorld.atWorldMatrix[frustumIndex].worldMatrix = AILocation;

	tThisWorld.atAIVision[spacePirate3].eyes0 = planes3;
	tThisWorld.atAIVision[spacePirate3].normalAtBegining[0] = planes3[0].normal;
	tThisWorld.atAIVision[spacePirate3].normalAtBegining[1] = planes3[1].normal;
	tThisWorld.atAIVision[spacePirate3].normalAtBegining[2] = planes3[2].normal;
	tThisWorld.atAIVision[spacePirate3].normalAtBegining[3] = planes3[3].normal;
	tThisWorld.atAIVision[spacePirate3].normalAtBegining[4] = planes3[4].normal;
	tThisWorld.atAIVision[spacePirate3].normalAtBegining[5] = planes3[5].normal;
	nodeLocation = AILocation;
	nodeLocation.r[3].m128_f32[1] += -1;

	int nodeindex5 = CreateNodePoint(&tThisWorld, nodeLocation);
	nodePosition;
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];


	pcAiSystem->AddNodeToPathFinding(nodeindex5, nodePosition, 1);
	nodeLocation.r[3].m128_f32[2] += 3;
	//nodeLocation.r[3].m128_f32[0] += 5;
	int nodeindex6 = CreateNodePoint(&tThisWorld, nodeLocation);
	edges.clear();
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex6, nodePosition, 1);
	edges.push_back(nodeindex6);
	pcAiSystem->AddEdgestoNode(nodeindex5, edges);
	edges.clear();
	edges.push_back(nodeindex5);
	pcAiSystem->AddEdgestoNode(nodeindex6, edges);

	tThisWorld.atPathPlanining[spacePirate3].Goal = nodeindex5;
	tThisWorld.atPathPlanining[spacePirate3].startingNode = nodeindex6;


	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask > 1)
		{

			if (tThisWorld.atSimpleMesh[nCurrentEntity].m_nVertexCount > tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount)
			{

				TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[nCurrentEntity].m_VertexData, tThisWorld.atAABB[nCurrentEntity]);
				MyAbb.m_IndexLocation = nCurrentEntity;
				tThisWorld.atAABB[nCurrentEntity] = MyAbb;
				pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);
				/*if (nCurrentEntity == door1Index || nCurrentEntity == door2Index || swordGuy == nCurrentEntity) {
				pcCollisionSystem->AddAiVisioNCheck(MyAbb, nCurrentEntity);
				}*/

			}
			if (tThisWorld.atMesh[nCurrentEntity].m_nVertexCount > tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount)
			{
				TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atMesh[nCurrentEntity].m_VertexData, tThisWorld.atAABB[nCurrentEntity]);
				MyAbb.m_IndexLocation = nCurrentEntity;
				tThisWorld.atAABB[nCurrentEntity] = MyAbb;
				pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);
				if (nCurrentEntity == PlayerStartIndex) {
					pcCollisionSystem->AddAiVisioNCheck(MyAbb, nCurrentEntity);

				}
			}
		}
	}

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask > 1)
		{
			tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
		}
	}


	pcGraphicsSystem->CreateBuffers(&tThisWorld);

	fpsTimer.Init_FPSReader();
}

int CGameMangerSystem::RealLevelUpdate()
{
	fpsTimer.Xtime_Signal();

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(-1); //Important!
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective(m_RealTimeFov);

	// checks the camera bool variables & store mouse position at the beginning  

	tCameraMode = pcInputSystem->CameraModeListen(tCameraMode);
	pcInputSystem->GetMousePosition();

	static XMMATRIX m_d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	static XMMATRIX m_d3dOffsetMatrix = pcGraphicsSystem->SetDefaultOffset();

	CGraphicsSystem::TPrimalVertexBufferType tTempVertexBuffer;
	CGraphicsSystem::TPrimalPixelBufferType tTempPixelBuffer;
	CGraphicsSystem::TMyVertexBufferType tMyVertexBufferTemp;
	CGraphicsSystem::TUIVertexBufferType tUIVertexBuffer;
	CGraphicsSystem::TUIPixelBufferType tUIPixelBuffer;

	hoverPoint = { -1, -1 };
	//POINT hoverPoint;
	GetCursorPos(&hoverPoint);
	ScreenToClient(cApplicationWindow, &hoverPoint);

	//POINT 
	clickPoint = { -1, -1 };
	if (pcInputSystem->InputCheck(G_BUTTON_LEFT) == 1 && mouseUp)
	{

		GetCursorPos(&startDragPoint);
		ScreenToClient(cApplicationWindow, &startDragPoint);

		GetCursorPos(&dragPoint);
		ScreenToClient(cApplicationWindow, &dragPoint);

		mouseUp = false;
		mouseDown = true;
	}
	else if (pcInputSystem->InputCheck(G_BUTTON_LEFT) == 1 && mouseDown)
	{
		GetCursorPos(&dragPoint);
		ScreenToClient(cApplicationWindow, &dragPoint);
	}
	else if (pcInputSystem->InputCheck(G_BUTTON_LEFT) == 0 && mouseDown)
	{
		GetCursorPos(&clickPoint);
		ScreenToClient(cApplicationWindow, &clickPoint);

		mouseUp = true;
		mouseDown = false;

		click = true;

		startDragPoint = { -1, -1 };
		dragPoint = { -1, -1 };
	}

	if (pcInputSystem->InputCheck(G_KEY_P) && !GameOver)
	{
		GamePaused = true;
		if (!pauseInit)
		{
			pauseInit = true;
			ShowCursor(true);
		}
	}
	if (pcInputSystem->InputCheck(G_KEY_U) && !options && !GameOver)
	{
		GamePaused = false;
		if (pauseInit)
		{
			pauseInit = false;
			ShowCursor(false);
		}
	}

	//Camera Functions here will move to a input system function when all behaviors are finalized - ZFB
	if (GamePaused == false && GameOver == false)
	{

		// Walk mode not needed in demo at the moment - ZFB
		if (tCameraMode.bWalkMode == true)
		{

			if (tCameraMode.bSwitch == true)
			{
				m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
				tCameraMode.bSwitch = false;
			}



			m_d3d_ResultMatrix = pcInputSystem->WalkCameraControls(XMVectorSet(0, 1.0f, 0, 0), m_d3d_ResultMatrix, bMoving);

			walkCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);
			walkCamera->d3d_Position = XMMatrixMultiply(m_d3dOffsetMatrix, walkCamera->d3d_Position);

			tMyVertexBufferTemp.m_d3dViewMatrix = walkCamera->d3d_Position;
			tTempVertexBuffer.m_d3dViewMatrix = walkCamera->d3d_Position;
		}
		//Aim Mode Functions are Here - ZFB 
		else if (tCameraMode.bAimMode == true)
		{
			m_d3dOffsetMatrix = pcGraphicsSystem->ResetAimModeCameraOffset();
			if (tCameraMode.bSwitch == true)
			{
				//m_d3dOffsetMatrix = pcGraphicsSystem->ResetAimModeCameraOffset();
				m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
				//CameraNewPosition = pcInputSystem->CameraBehaviorLerp(m_d3d_ResultMatrix, m_d3dPlayerMatrix);
				//CameraNewPosition = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dPlayerMatrix);

				//CameraNewPosition = XMMatrixMultiply(m_d3dOffsetMatrix, CameraNewPosition);
				//aimCamera->d3d_Position = pcInputSystem->CameraBehaviorLerp(walkCamera->d3d_Position, CameraNewPosition, scale);
				//scale += 0.001;
				//if (scale > 1) {
				tCameraMode.bSwitch = false;
				scale = 0;



			}

			m_RealTimeFov = pcInputSystem->ZoomSight(m_RealTimeFov);
			// Camera rotation Done here
			aimCamera->d3d_Position = pcInputSystem->AimMode(aimCamera, m_d3d_ResultMatrix);
			//Does Character Rotation and Movement
			m_d3dPlayerMatrix = pcInputSystem->CharacterMovement(m_d3dPlayerMatrix);

			aimCamera->d3d_Position = XMMatrixMultiply(aimCamera->d3d_Position, m_d3dPlayerMatrix);
			// for shoulder offset 
			aimCamera->d3d_Position = XMMatrixMultiply(m_d3dOffsetMatrix, aimCamera->d3d_Position);



			tMyVertexBufferTemp.m_d3dViewMatrix = aimCamera->d3d_Position;
			tTempVertexBuffer.m_d3dViewMatrix = aimCamera->d3d_Position;
		}
		//This is Debug Camera Function here - ZFB
		else
		{
			if (tCameraMode.bSwitch == true)
			{
				m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
				tCameraMode.bSwitch = false;
			}
			m_d3d_ResultMatrix = pcInputSystem->DebugCamera(m_d3d_ResultMatrix, m_d3dWorldMatrix);

			debugCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dWorldMatrix);
			tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;
			tTempVertexBuffer.m_d3dViewMatrix = debugCamera->d3d_Position;
		}

	}


	tTempPixelBuffer.m_d3dCollisionColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

#pragma region Render To Texture Pass
	pcGraphicsSystem->UpdateD3D();
	pcGraphicsSystem->UpdateD3D_RenderToTexture();

	for (int nCurrentEntity = 0; nCurrentEntity <= renderToTexturePassIndex; nCurrentEntity++)
	{
		tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			tTempVertexBuffer.m_d3dWorldMatrix = m_d3dWorldMatrix;
			if (tCameraMode.bWalkMode == true)
			{
				tMyVertexBufferTemp.m_d3dViewMatrix = walkCamera->d3d_Position;
				pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], walkCamera->d3d_Position);
			}
			else if (tCameraMode.bAimMode == true)
			{
				tMyVertexBufferTemp.m_d3dViewMatrix = aimCamera->d3d_Position;
				pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], aimCamera->d3d_Position);
			}
			else if (tCameraMode.bDebugMode == true)
			{
				tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;
				pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);
			}
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_SKYBOX | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
			if (tCameraMode.bWalkMode == true)
			{
				tMyVertexBufferTemp.m_d3dViewMatrix = walkCamera->d3d_Position;
				pcGraphicsSystem->InitSkyboxShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], walkCamera->d3d_Position);
			}
			else if (tCameraMode.bAimMode == true)
			{
				tMyVertexBufferTemp.m_d3dViewMatrix = aimCamera->d3d_Position;
				pcGraphicsSystem->InitSkyboxShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], aimCamera->d3d_Position);
			}
			else if (tCameraMode.bDebugMode == true)
			{
				tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;
				pcGraphicsSystem->InitSkyboxShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);
			}
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}
	}

	pcGraphicsSystem->m_pd3dDevice->CreateShaderResourceView(pcGraphicsSystem->m_pd3dOutsideGlassRenderToTexture, NULL, &pcGraphicsSystem->m_pd3dOutsideGlassSRV);
	pcGraphicsSystem->m_pd3dDeviceContext->GenerateMips(pcGraphicsSystem->m_pd3dOutsideGlassSRV);

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			if (tThisWorld.atShaderID[nCurrentEntity].m_nShaderID == 10)
			{
				tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse = pcGraphicsSystem->m_pd3dOutsideGlassSRV;
			}
		}
	}

#pragma endregion

	pcGraphicsSystem->UpdateD3D();

#pragma region Input Garbage
	// toggle the modes that you are in
	if (GamePaused == false && GameOver == false)
	{
		if (pcInputSystem->InputCheck(G_BUTTON_MIDDLE))
		{
			tThisWorld.atClip[GunIndexForPlayer].GunMode = !tThisWorld.atClip[GunIndexForPlayer].GunMode;
		}
		// shoot a bullet
		if (pcInputSystem->InputCheck(G_BUTTON_LEFT) == 1
			&& tThisWorld.atClip[GunIndexForPlayer].GunMode == true)
		{
			tThisWorld.atClip[GunIndexForPlayer].tryToShoot = true;
		}
		// shoot a ray
		else if (pcInputSystem->InputCheck(G_BUTTON_LEFT) == 1
			&& tThisWorld.atClip[GunIndexForPlayer].GunMode == false)
		{
			tThisWorld.atClip[GunIndexForPlayer].tryToShoot = true;
		}
		// turn the ray off
		else if (tThisWorld.atClip[GunIndexForPlayer].GunMode == false)
		{
			tThisWorld.atClip[GunIndexForPlayer].tryToShoot = false;
		}

		// reload
		if (pcInputSystem->InputCheck(G_KEY_R) == 1)
		{
			tThisWorld.atClip[GunIndexForPlayer].tryToReload = true;
		}
	}

#pragma endregion

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_SKYBOX | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
			if (tCameraMode.bWalkMode == true)
			{
				tMyVertexBufferTemp.m_d3dViewMatrix = walkCamera->d3d_Position;
				pcGraphicsSystem->InitSkyboxShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], walkCamera->d3d_Position);
			}
			else if (tCameraMode.bAimMode == true)
			{
				tMyVertexBufferTemp.m_d3dViewMatrix = aimCamera->d3d_Position;
				pcGraphicsSystem->InitSkyboxShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], aimCamera->d3d_Position);
			}
			else if (tCameraMode.bDebugMode == true)
			{
				tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;
				pcGraphicsSystem->InitSkyboxShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);
			}
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		{
			if (tThisWorld.atBar[nCurrentEntity].entityToFollow != -1)
			{
				if (tThisWorld.atActiveAI[tThisWorld.atBar[nCurrentEntity].entityToFollow].active == true)
				{
					unsigned int targetEntity = tThisWorld.atBar[nCurrentEntity].entityToFollow;

					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tThisWorld.atWorldMatrix[targetEntity].worldMatrix;

					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1] += 2;

					if (tThisWorld.atBar[nCurrentEntity].backgroundColor.x == 0
						&& tThisWorld.atBar[nCurrentEntity].backgroundColor.y == 0
						&& tThisWorld.atBar[nCurrentEntity].backgroundColor.z == 0)
					{
						if (tCameraMode.bWalkMode == true)
						{
							pcGraphicsSystem->InitQuadShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], walkCamera->d3d_Position, tThisWorld.atBar[nCurrentEntity].backgroundColor, 1.0f);
						}
						else if (tCameraMode.bAimMode == true)
						{
							pcGraphicsSystem->InitQuadShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], aimCamera->d3d_Position, tThisWorld.atBar[nCurrentEntity].backgroundColor, 1.0f);
						}
						else if (tCameraMode.bDebugMode == true)
						{
							pcGraphicsSystem->InitQuadShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], debugCamera->d3d_Position, tThisWorld.atBar[nCurrentEntity].backgroundColor, 1.0f);
						}
					}
					else
					{
						if (tCameraMode.bWalkMode == true)
						{
							pcGraphicsSystem->InitQuadShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], walkCamera->d3d_Position, tThisWorld.atBar[nCurrentEntity].backgroundColor, tThisWorld.atAiHeath[targetEntity].heath * .01);
						}
						else if (tCameraMode.bAimMode == true)
						{
							pcGraphicsSystem->InitQuadShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], aimCamera->d3d_Position, tThisWorld.atBar[nCurrentEntity].backgroundColor, tThisWorld.atAiHeath[targetEntity].heath * .01);
						}
						else if (tCameraMode.bDebugMode == true)
						{
							pcGraphicsSystem->InitQuadShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], debugCamera->d3d_Position, tThisWorld.atBar[nCurrentEntity].backgroundColor, tThisWorld.atAiHeath[targetEntity].heath * .01);
						}
					}

					pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
				}
			}
			else
			{
				if (nCurrentEntity == rayindex)
				{
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix;
				}
				pcGraphicsSystem->InitPrimalShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, debugCamera->d3d_Position, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], debugCamera->d3d_Position);
			
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}


		}
		// ai code would run here
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{

			/*if (nCurrentEntity == 2 || nCurrentEntity == 3) {
			continue;
			}*/
			if (tCameraMode.bWalkMode == true)
			{
				tMyVertexBufferTemp.m_d3dViewMatrix = walkCamera->d3d_Position;
				tTempVertexBuffer.m_d3dViewMatrix = walkCamera->d3d_Position;
			}
			/*else if (tCameraMode.bAimMode == true)
			{
			tTempVertexBuffer.m_d3dViewMatrix = aimCamera->d3d_Position;
			tMyVertexBufferTemp.m_d3dViewMatrix = aimCamera->d3d_Position;
			}*/
			else if (tCameraMode.bDebugMode == true)
			{
				tTempVertexBuffer.m_d3dWorldMatrix = m_d3dWorldMatrix;
				tTempVertexBuffer.m_d3dViewMatrix = debugCamera->d3d_Position;
				tMyVertexBufferTemp.m_d3dViewMatrix = debugCamera->d3d_Position;
			}
			else
			{
				tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
				tTempVertexBuffer.m_d3dViewMatrix = m_d3dViewMatrix;
				tMyVertexBufferTemp.m_d3dViewMatrix = m_d3dViewMatrix;
			}
			//Clayton input with Camera variables here
			if (GamePaused == false && GameOver == false) {
				if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK))
				{
					if (tCameraMode.bWalkMode == true)
					{
						m_d3dPlayerMatrix = pcInputSystem->CharacterMovement(m_d3dPlayerMatrix);
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
						m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
					}
					else if (tCameraMode.bAimMode == true)
					{
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
						m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
					}
				}
			}
			if (tCameraMode.bWalkMode == true)
			{
				pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], walkCamera->d3d_Position);
			}
			//Calls Shader Function on Aim Camera If aim mode is active
			else if (tCameraMode.bAimMode == true)
			{
				pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], aimCamera->d3d_Position);
			}
			else
			{
				pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);
			}

			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

		if (pcAiSystem->GetNumberOfAI() <= 0 && !endInit)
		{
			GameOver = true;
			InitializeEndScreen(true);
			endInit = true;
		}
		if (GamePaused == false && GameOver == false)
		{
			if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_FOLLOW)
				|| tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_SHOOT | COMPONENT_AIMASK | COMPONENT_FOLLOW)
				|| tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST))
			{
#if AI_ON				
				if (tThisWorld.atActiveAI[nCurrentEntity].active == true)
				{
					pcAiSystem->LookAtObject(tThisWorld.atWorldMatrix[tThisWorld.atAIVision[nCurrentEntity].indexLookingAt].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
					pcAiSystem->ShootGun(&tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex]);
				}
#endif // AI_ON
			}
		}
		if (GamePaused == false && GameOver == false)
		{
			if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SEARCH)
				|| tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SPOTEDPLAYER)
				|| tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST))
			{
#if AI_ON
				if (tThisWorld.atAIVision[nCurrentEntity].keepSearching == true)
				{
					if (tThisWorld.atAIVision[nCurrentEntity].visionRotation < 7
						&& tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true)
					{
						tThisWorld.atAIVision[nCurrentEntity].visionRotation += 0.01;
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcAiSystem->LookBackLeftToRight(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix,
							true);
					}
					else if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true)
					{
						tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight = false;
					}
					else if (tThisWorld.atAIVision[nCurrentEntity].visionRotation > -7
						&& tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == false)
					{
						tThisWorld.atAIVision[nCurrentEntity].visionRotation -= 0.01;
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcAiSystem->LookBackLeftToRight(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix,
							false);
					}
					else if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == false)
					{
						tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight = true;
					}
				}
				else if (tThisWorld.atAIVision[nCurrentEntity].keepSearching == false)
				{
					pcAiSystem->LookAtObject(tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
				}
#endif

				float4x4 AiFrustum;
				AiFrustum.row1.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[0];
				AiFrustum.row1.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[1];
				AiFrustum.row1.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[2];
				AiFrustum.row1.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0].m128_f32[3];

				AiFrustum.row2.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[0];
				AiFrustum.row2.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[1];
				AiFrustum.row2.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[2];
				AiFrustum.row2.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1].m128_f32[3];

				AiFrustum.row3.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[0];
				AiFrustum.row3.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[1];
				AiFrustum.row3.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[2];
				AiFrustum.row3.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[2].m128_f32[3];

				AiFrustum.row4.x = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[0];
				AiFrustum.row4.y = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1];
				AiFrustum.row4.z = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[2];
				AiFrustum.row4.w = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[3];
				pcAiSystem->UpdateFrustum(tThisWorld.atAIVision[nCurrentEntity].eyes0, AiFrustum, 70, 1, 0.1, 20);
				//	 pcAiSystem->calculate_frustum(&tThisWorld,tThisWorld.atAIVision[nCurrentEntity].eyes0, AiFrustum,70,1,0.1,20, nCurrentEntity, -2.1, 1.4, 19.6);
				vector<int> indicies;
				if (pcCollisionSystem->AiVisionCheck(tThisWorld.atAIVision[nCurrentEntity].eyes0, &indicies) == true)
				{
					bool danger = false;
					for (int i = 0; i < indicies.size(); ++i)
					{
						if (PlayerStartIndex == indicies[i])
						{
							tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
							tThisWorld.atAIVision[nCurrentEntity].keepSearching = false;
							tThisWorld.atActiveAI[nCurrentEntity].active = true;
							danger = true;
							if (tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].nBulletsAvailables.size() <= 0)
							{
								tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].tryToReload = true;

							}
							for (int CurrentAIINdex = 0; CurrentAIINdex < tThisWorld.atActiveAI[nCurrentEntity].NoctifyOtherAi.size(); ++CurrentAIINdex)
							{

								tThisWorld.atSimpleMesh[CurrentAIINdex].m_nColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
								tThisWorld.atAIVision[CurrentAIINdex].keepSearching = false;
								tThisWorld.atActiveAI[CurrentAIINdex].active = true;

								if (tThisWorld.atClip[tThisWorld.atAIMask[CurrentAIINdex].GunIndex].nBulletsAvailables.size() <= 0)
								{
									tThisWorld.atClip[tThisWorld.atAIMask[CurrentAIINdex].GunIndex].tryToReload = true;

								}
							}
#if AI_ON
							tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].tryToShoot = true;

#endif
						}//
						else if (tThisWorld.atProjectiles[indicies[i]].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL))
						{
							danger = true;
							tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
						}
						else if (danger == false)
						{
							tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
						}
						else if (tThisWorld.atAIMask[indicies[i]].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_FIGHTINGAI))
						{
						}
					}
				}
			}
		}
		if (GamePaused == false && GameOver == false)
		{
			if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_PATHFINDTEST)
				|| tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST))
			{
				if (tThisWorld.atActiveAI[nCurrentEntity].active == true)
				{
					if (tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding == true)
					{
						pcAiSystem->FindBestPath(tThisWorld.atPathPlanining[nCurrentEntity].startingNode,
							tThisWorld.atPathPlanining[nCurrentEntity].Goal,
							&tThisWorld.atPathPlanining[nCurrentEntity].directions);
						tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding = false;
					}
					else
					{
						if (tThisWorld.atPathPlanining[nCurrentEntity].DelayMovement <= 0)
						{
							pcAiSystem->PathPlaningMovement(&tThisWorld.atPathPlanining[nCurrentEntity], &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
						}
						else
						{
							tThisWorld.atPathPlanining[nCurrentEntity].DelayMovement -= 0.001;
						}
					}
				}
			}
		}
		if (GamePaused == false && GameOver == false)
		{
			if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_CLIP))
			{
				if (tThisWorld.atClip[nCurrentEntity].GunMode == false
					&& tThisWorld.atClip[nCurrentEntity].tryToShoot == true)
				{
					if (tThisWorld.atClip[nCurrentEntity].maderay == false)
					{
						XMMATRIX gun = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
						gun.r[3].m128_f32[0] += 1;
						rayindex = CreateRayBullet(&tThisWorld, gun, 10, GunIndexForPlayer, -0.6, 0.6, 10.7);
						pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, rayindex);
						tThisWorld.atClip[nCurrentEntity].maderay = true;
					}
				}
				else if (tThisWorld.atClip[nCurrentEntity].GunMode == false
					&& tThisWorld.atClip[nCurrentEntity].tryToShoot == false
					&& tThisWorld.atClip[nCurrentEntity].maderay == true)
				{
					pcGraphicsSystem->CleanD3DObject(&tThisWorld, rayindex);
					rayindex = -1;
					tThisWorld.atClip[nCurrentEntity].maderay = false;
				}
				else
				{
					// Metal Fired Sound in Here -ZFB
					if (tThisWorld.atClip[nCurrentEntity].tryToShoot == true
						&& tThisWorld.atClip[nCurrentEntity].nBulletsAvailables.size() > 0
						&& tThisWorld.atClip[nCurrentEntity].fShootingCoolDown <= 0)
					{
						if (nCurrentEntity == GunIndexForPlayer)
						{
							wchar_t* textBuffer = new wchar_t[1];

							textBuffer[0] = (tThisWorld.atClip[nCurrentEntity].nBulletsAvailables.size() - 1);

							pcUISystem->UpdateText(&tThisWorld, 1094, &atUIVertices, textBuffer, 1, atUIVertices.at(tThisWorld.atLabel[1094].vIndex));

							pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, 1094);

							delete[] textBuffer;
						}

						XMVECTOR foward;
						/*foward.m128_f32[0] = 0;
						foward.m128_f32[1] = 0;
						foward.m128_f32[2] = 1;*/

						XMMATRIX localMatrix2;
						XMMATRIX gunMatrix;

						int bulletType = -1;
						if (nCurrentEntity == GunIndexForPlayer)
						{
							foward.m128_f32[0] = 0;
							foward.m128_f32[1] = 0;
							foward.m128_f32[2] = 1;

							localMatrix2 = XMMatrixTranslationFromVector(foward);
							gunMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
							gunMatrix = XMMatrixMultiply(localMatrix2, gunMatrix);

							bulletType = 0;
						}
						else
						{
							foward.m128_f32[0] = 0;
							foward.m128_f32[1] = 0;
							foward.m128_f32[2] = 1;

							localMatrix2 = XMMatrixTranslationFromVector(foward);
							gunMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
							gunMatrix = XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(90)), gunMatrix);
							gunMatrix = XMMatrixMultiply(localMatrix2, gunMatrix);

							bulletType = 1;
						}
#if MUSIC_ON
						pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_METAL_FIRED, m_AkMetalFired);
#endif
						int newbullet = CreateBullet(&tThisWorld, gunMatrix, tThisWorld.atClip[nCurrentEntity].currentMaterial, bulletType);
						tThisWorld.atClip[newbullet].gunIndex = nCurrentEntity;
						tThisWorld.atSimpleMesh[newbullet].m_nColor = tThisWorld.atClip[nCurrentEntity].colorofBullets;
						tThisWorld.atClip[newbullet].indexInclip = pcProjectileSystem->CreateBulletProjectile(newbullet, &tThisWorld.atClip[nCurrentEntity]);
						tThisWorld.atAABB[newbullet] = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[newbullet].m_VertexData, tThisWorld.atAABB[newbullet]);
						tThisWorld.atAABB[newbullet].m_IndexLocation = newbullet;

						pcCollisionSystem->AddAABBCollider(tThisWorld.atAABB[newbullet], newbullet);
						pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, newbullet);

					}
					else if (tThisWorld.atClip[nCurrentEntity].tryToShoot == true)
					{
						tThisWorld.atClip[nCurrentEntity].tryToShoot = false;
					}
					if (tThisWorld.atClip[nCurrentEntity].tryToReload == true)
					{
						//Reload Metal Sound - ZFB
#if MUSIC_ON
						if (tThisWorld.atClip[nCurrentEntity].nBulletsAvailables.size() < 3)
						{
							pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_METAL_RELOAD, m_MetalReload);
						}
#endif
						pcProjectileSystem->Reload(&tThisWorld.atClip[nCurrentEntity]);
						tThisWorld.atClip[nCurrentEntity].tryToReload = false;

						if (nCurrentEntity == GunIndexForPlayer)
						{
							wchar_t* textBuffer = new wchar_t[1];

							textBuffer[0] = (tThisWorld.atClip[nCurrentEntity].nBulletsAvailables.size());

							pcUISystem->UpdateText(&tThisWorld, 1094, &atUIVertices, textBuffer, 1, atUIVertices.at(tThisWorld.atLabel[1094].vIndex));

							pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, 1094);

							delete[] textBuffer;
						}
					}
					
					if (tThisWorld.atClip[nCurrentEntity].fShootingCoolDown > 0)
					{
						tThisWorld.atClip[nCurrentEntity].fShootingCoolDown -= fpsTimer.GetDelta() * 50;
					}

				}
			}
		}
		if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_FRIENDLY) ||
			tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_ENEMY))
		{
			//ADD FORCE TO EVERY BULLET
			pcPhysicsSystem->AddBulletForce(&tThisWorld.atRigidBody[nCurrentEntity], fpsTimer.GetDelta() * 2);
		}
		if (GamePaused == false && GameOver == false)
		{
			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, true);
		}
		if (GamePaused == false && GameOver == false)
		{
			if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_RAYGUN))
			{
				float CloseEstObject = 10000000000000000000.0f;
				float* distanceCalucaltion = new float();
				for (list<TAABB>::iterator ptr = pcCollisionSystem->m_AAbb.begin(); ptr != pcCollisionSystem->m_AAbb.end(); ++ptr)
				{

					if (ptr->m_IndexLocation != PlayerStartIndex && ptr->m_IndexLocation != GunIndexForPlayer)
					{
						if (pcCollisionSystem->intersectRayAABox2(
							XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[0],
								tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix),
							XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[1],
								tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix), *ptr
						) == true)
						{
							//CloseEstObject = *distanceCalucaltion;
							tThisWorld.atClip[GunIndexForPlayer].currentMaterial = 0;
							cout << "turtle" << '/n';
							tThisWorld.atClip[GunIndexForPlayer].colorofBullets = tThisWorld.atSimpleMesh[ptr->m_IndexLocation].m_nColor;
						}

					}
				}
				delete distanceCalucaltion;
			}
		}
		if (GamePaused == false && GameOver == false)
		{
			if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)
				|| tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))
			{
				tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
			}


			if ((tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_TRIGGER) || tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)))
			{
				vector<int> otherCollisionsIndex;
				if (pcCollisionSystem->AABBtoAABBCollisionCheck(tThisWorld.atAABB[nCurrentEntity], &otherCollisionsIndex) == true)
				{

					for (int i = 0; i < otherCollisionsIndex.size(); ++i)
					{
						if (tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == true
							&& tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))
						{
							//SHOOTING A WALL OR ANYTHING ELSE
							if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_FRIENDLY) ||
								tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_ENEMY))
							{
								pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);



								pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);
							}
						}
						if (tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == true
							&& tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask != (COMPONENT_COLLISIONMASK | COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))
						{
						

							tThisWorld.atRigidBody[nCurrentEntity].totalForce = -tThisWorld.atRigidBody[nCurrentEntity].velocity;
							tTempVertexBuffer.m_d3dWorldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
							tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcCollisionSystem->WalkingThrewObjectCheck(tTempVertexBuffer.m_d3dWorldMatrix, tThisWorld.atAABB[otherCollisionsIndex[i]], tThisWorld.atAABB[nCurrentEntity]);
							tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tTempVertexBuffer.m_d3dWorldMatrix;
						}
						if (tThisWorld.atRigidBody[nCurrentEntity].ground == false
							&& tThisWorld.atRigidBody[nCurrentEntity].wall == false
							&& tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)
							&& (tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_STATIC)
								|| (tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))))
						{
							tTempVertexBuffer.m_d3dWorldMatrix = pcCollisionSystem->WalkingThrewObjectCheck(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[otherCollisionsIndex[i]], tThisWorld.atAABB[nCurrentEntity]);
							tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tTempVertexBuffer.m_d3dWorldMatrix;
							if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK))
							{
								m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
							}

						}
						if (tThisWorld.atRigidBody[nCurrentEntity].ground == false
							&& tThisWorld.atRigidBody[otherCollisionsIndex[i]].ground == false
							&& tThisWorld.atRigidBody[nCurrentEntity].wall == false
							&& tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)
							&& (tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_STATIC))
							|| tThisWorld.atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))
						{

							if (tThisWorld.atAIMask[otherCollisionsIndex[i]].m_tnAIMask > 1) 
							{
								// SHOOTING AN AI ONLY
								if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_FRIENDLY))
								{
									if (tThisWorld.atAiHeath[otherCollisionsIndex[i]].heath <= 0)
									{
										pcAiSystem->SetNumberOfAI(pcAiSystem->GetNumberOfAI() - 1);
										if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1)
										{
											

											pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
										


											pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);


										}
										pcCollisionSystem->RemoveAABBCollider(otherCollisionsIndex[i]);
										pcGraphicsSystem->CleanD3DObject(&tThisWorld, otherCollisionsIndex[i]);
										pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atAIMask[otherCollisionsIndex[i]].GunIndex);
									
									}
									else
									{
										
										

											pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
										
										


										pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);
										tThisWorld.atAiHeath[otherCollisionsIndex[i]].heath -= playerDamage;
										tThisWorld.atActiveAI[otherCollisionsIndex[i]].active = true;
										if (tThisWorld.atAiHeath[otherCollisionsIndex[i]].heath <= 0)
										{
											pcAiSystem->SetNumberOfAI(pcAiSystem->GetNumberOfAI() - 1);
											pcCollisionSystem->RemoveAABBCollider(otherCollisionsIndex[i]);
											pcGraphicsSystem->CleanD3DObject(&tThisWorld, otherCollisionsIndex[i]);
											pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atAIMask[otherCollisionsIndex[i]].GunIndex);

											pcGraphicsSystem->CleanD3DObject(&tThisWorld, otherCollisionsIndex[i] + 1);
											pcGraphicsSystem->CleanD3DObject(&tThisWorld, otherCollisionsIndex[i] + 2);
										}
									
									}
								}
							}
							else if (tThisWorld.atInputMask[otherCollisionsIndex[i]].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK))
							{
								//SHOOTING THE PLAYER ONLY
								if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_ENEMY))
								{
									if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1)
									{
											

											pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
										
										
										//pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atAIMask[nCurrentEntity].GunIndex);
										pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);

									}
									tThisWorld.atClayton[otherCollisionsIndex[i]].health -= pirateDamage;
									if (tThisWorld.atClayton[otherCollisionsIndex[i]].health <= 0)
									{
										GameOver = true;
										InitializeEndScreen(false);
									}
								}


							}
							//Wasn't being hit at all so it's been commented.
							/*if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL))
							{
								if (tThisWorld.atClip[nCurrentEntity].gunIndex != -1)
								{
								

									pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
									

									//pcGraphicsSystem->CleanD3DObject(&tThisWorld, tThisWorld.atAIMask[nCurrentEntity].GunIndex);
									pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);

								}
							}*/

						}
						if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK) && tThisWorld.atAIMask[otherCollisionsIndex[i]].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_COVERTRIGGER))
						{
							pcAiSystem->MoveAiToCoverLocation(tThisWorld.atCoverTrigger[otherCollisionsIndex[i]], &tThisWorld);
						}
					}
					if (tThisWorld.atClayton[PlayerStartIndex].health <= 0)
					{
						GameOver = true;
						InitializeEndScreen(false);
					}

				}
			}
		}
		if (tThisWorld.atParentWorldMatrix[nCurrentEntity] != -1)
		{
			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(pcGraphicsSystem->SetDefaultWorldPosition(),
				tThisWorld.atWorldMatrix[tThisWorld.atParentWorldMatrix[nCurrentEntity]].worldMatrix);

			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(tThisWorld.atOffSetMatrix[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);

			if (nCurrentEntity != GunIndexForPlayer)
			{
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(-90)), tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
			}

			if (nCurrentEntity == rayindex)
			{
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix;
			}
			if (nCurrentEntity == frustumIndex)
			{
				XMMATRIX empty;
				empty = pcGraphicsSystem->SetDefaultWorldPosition();
				XMMATRIX rotation;
				XMMATRIX d3dTmpViewM;
				rotation = XMMatrixRotationY(-0.3);
				empty.r[1] = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[1];
				empty.r[0] = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[0];
				d3dTmpViewM = empty;
				XMVECTOR d3d_newX, d3d_newY, d3d_existingZ;
				d3dTmpViewM = XMMatrixMultiply(rotation, d3dTmpViewM);
				d3d_existingZ = d3dTmpViewM.r[2];
				d3d_newX = XMVector3Cross(empty.r[1], d3d_existingZ);
				d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

				d3d_newX = XMVector3Normalize(d3d_newX);
				d3d_newY = XMVector3Normalize(d3d_newY);

				d3d_existingZ = XMVector3Normalize(d3d_existingZ);

				d3dTmpViewM.r[0] = d3d_newX;
				d3dTmpViewM.r[1] = d3d_newY;
				d3dTmpViewM.r[2] = d3d_existingZ;
				empty = d3dTmpViewM;
				empty.r[3].m128_f32[0] = 3;
				empty.r[3].m128_f32[1] = 0;
				empty.r[3].m128_f32[2] = 1;


				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = empty;
			}
		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID))
		{
			if (tCameraMode.bWalkMode == true)
			{
				pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity], walkCamera->d3d_Position);
			}
			else if (tCameraMode.bAimMode == true)
			{
				pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity], aimCamera->d3d_Position);
			}
			else
			{
				pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity], debugCamera->d3d_Position);
			}
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atSimpleMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_CLIP))
			{
				if (tCameraMode.bWalkMode == true)
				{
					pcGraphicsSystem->InitPrimalShaderData3(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], walkCamera->d3d_Position);
				}
				else if (tCameraMode.bAimMode == true)
				{
					pcGraphicsSystem->InitPrimalShaderData3(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], aimCamera->d3d_Position);
				}
				else
				{
					pcGraphicsSystem->InitPrimalShaderData3(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);
				}
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}
		}

		if (tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix.r[3].m128_f32[1] < -30)
		{
			tThisWorld.atClayton[PlayerStartIndex].health *= 0;
		}


		if (GamePaused)
		{
			if (!options)
			{
				if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON | COMPONENT_PAUSESCREEN))
				{
					tUIVertexBuffer.start = -1;
					tUIVertexBuffer.end = -1;
					tUIVertexBuffer.ratio = -1;

					tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

					if (tThisWorld.atButton[nCurrentEntity].enabled && clickTime > .2)
					{
						if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
						{
							clickTime = 0;

							if (tThisWorld.atButton[nCurrentEntity].sceneIndex == 98)
							{
								options = true;
							}
							else if (tThisWorld.atButton[nCurrentEntity].sceneIndex == nCurrentScene)
							{
								GamePaused = false;
								if (pauseInit)
								{
									pauseInit = false;
									ShowCursor(false);
								}
							}
							else if (tThisWorld.atButton[nCurrentEntity].sceneIndex == 96 || tThisWorld.atButton[nCurrentEntity].sceneIndex == 97)
							{

							}
							else
							{
								return tThisWorld.atButton[nCurrentEntity].sceneIndex;
							}

						}
						else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
						{
							tUIPixelBuffer.hoverColor = XMFLOAT4(.6, .6, .6, 0);
						}
					}

					pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
					pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
				}
			}
			else if (options)
			{
				if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON | COMPONENT_OPTIONS))
				{
					tUIVertexBuffer.start = -1;
					tUIVertexBuffer.end = -1;
					tUIVertexBuffer.ratio = -1;

					tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

					if (tThisWorld.atButton[nCurrentEntity].enabled && clickTime > .2)
					{
						if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
						{
							clickTime = 0;

							if (tThisWorld.atButton[nCurrentEntity].sceneIndex == nCurrentScene)
								options = false;
						}
						else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
						{
							tUIPixelBuffer.hoverColor = XMFLOAT4(.6, .6, .6, 0);
						}
					}

					pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
					pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

				}

				if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BAR | COMPONENT_OPTIONS))
				{
					if (PtInRect(&tThisWorld.atBar[nCurrentEntity].barBoundingBox, clickPoint) && clickTime > .2)
					{
						// bar manipulation with mouse click try and use for enemy health bar - ZB                   
						tThisWorld.atBar[nCurrentEntity].ratio = (clickPoint.x - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left - 5.0) / (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left - 10);

						if (pcUISystem->CheckIfStringsAreTheSame(tThisWorld.atBar[nCurrentEntity].valueToChange, tThisWorld.atBar[nCurrentEntity].valueToChangeSize, "Sensitivity"))
						{
							// weird little formula to convert the ratio from 0-1 to 0.003-0.008
							pcInputSystem->SetMouseRotationSpeed(tThisWorld.atBar[nCurrentEntity].ratio * 0.005 + 0.003);
						}
					}
					else if (PtInRect(&tThisWorld.atBar[nCurrentEntity].barBoundingBox, dragPoint) && clickTime > .2)
					{
						// bar manipulation with mouse click try and use for enemy health bar - ZB                   
						tThisWorld.atBar[nCurrentEntity].ratio = (dragPoint.x - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left - 5.0) / (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left - 10);
					
						if (pcUISystem->CheckIfStringsAreTheSame(tThisWorld.atBar[nCurrentEntity].valueToChange, tThisWorld.atBar[nCurrentEntity].valueToChangeSize, "Sensitivity"))
						{
							// weird little formula to convert the ratio from 0-1 to 0.003-0.008
							pcInputSystem->SetMouseRotationSpeed(tThisWorld.atBar[nCurrentEntity].ratio * 0.005 + 0.003);
						}
					}

					if (tThisWorld.atBar[nCurrentEntity].backgroundColor.x == 1 &&
						tThisWorld.atBar[nCurrentEntity].backgroundColor.y == 0 &&
						tThisWorld.atBar[nCurrentEntity].backgroundColor.z == 0 &&
						tThisWorld.atBar[nCurrentEntity].backgroundColor.w == 1)
					{
						tUIVertexBuffer.start = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.left + 14 - (screenWidth / 2.0)) / (screenWidth / 2);
						tUIVertexBuffer.end = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right + 4 - (screenWidth / 2.0)) / (screenWidth / 2);
						tUIVertexBuffer.ratio = tThisWorld.atBar[nCurrentEntity].ratio;
					}
					else
					{
						tUIVertexBuffer.start = -1;
						tUIVertexBuffer.end = -1;
						tUIVertexBuffer.ratio = -1;
					}

					tUIPixelBuffer.hoverColor = tThisWorld.atBar[nCurrentEntity].backgroundColor;

					if (tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse != nullptr)
						tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse = nullptr;

					pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
					pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
				}
			}
		}
		else
		{
			if (GameOver)
			{
				if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON | COMPONENT_DEATHSCREEN))
				{
					tUIVertexBuffer.start = -1;
					tUIVertexBuffer.end = -1;
					tUIVertexBuffer.ratio = -1;

					tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

					if (tThisWorld.atButton[nCurrentEntity].enabled && clickTime > .2)
					{
						if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
						{
							clickTime = 0;

							if (tThisWorld.atButton[nCurrentEntity].sceneIndex < 96)
								return tThisWorld.atButton[nCurrentEntity].sceneIndex;
						}
						else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
						{
							tUIPixelBuffer.hoverColor = XMFLOAT4(.6, .6, .6, 0);
						}
					}

					pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
					pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
				}

				if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_DEATHSCREEN))
				{
					tUIVertexBuffer.start = -1;
					tUIVertexBuffer.end = -1;
					tUIVertexBuffer.ratio = -1;

					tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

					pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
					pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
				}
			}
		}

		if (GamePaused || GameOver || options)
		{
			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_PAUSESCREEN | COMPONENT_DEATHSCREEN | COMPONENT_OPTIONS))
			{
				tUIVertexBuffer.start = -1;
				tUIVertexBuffer.end = -1;
				tUIVertexBuffer.ratio = -1;

				tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}
		}

		if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_HUD))
		{
			tUIVertexBuffer.start = -1;
			tUIVertexBuffer.end = -1;
			tUIVertexBuffer.ratio = -1;

			tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

			pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

		}

		if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BAR | COMPONENT_HUD))
		{
			if (tThisWorld.atBar[nCurrentEntity].backgroundColor.x == 0 &&
				tThisWorld.atBar[nCurrentEntity].backgroundColor.y == 0 &&
				tThisWorld.atBar[nCurrentEntity].backgroundColor.z == 1 &&
				tThisWorld.atBar[nCurrentEntity].backgroundColor.w == 1)
			{
				tUIVertexBuffer.start = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.left + 14 - (screenWidth / 2.0)) / (screenWidth / 2);
				tUIVertexBuffer.end = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right + 4 - (screenWidth / 2.0)) / (screenWidth / 2);
				tUIVertexBuffer.ratio = (100 - tThisWorld.atClip[GunIndexForPlayer].fShootingCoolDown) * .01;
			}
			else if (tThisWorld.atBar[nCurrentEntity].backgroundColor.x == 1 &&
					 tThisWorld.atBar[nCurrentEntity].backgroundColor.y == 0 &&
					 tThisWorld.atBar[nCurrentEntity].backgroundColor.z == 0 &&
					 tThisWorld.atBar[nCurrentEntity].backgroundColor.w == 1)
			{
				tUIVertexBuffer.start = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.left + 14 - (screenWidth / 2.0)) / (screenWidth / 2);
				tUIVertexBuffer.end = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right + 4 - (screenWidth / 2.0)) / (screenWidth / 2);
				tUIVertexBuffer.ratio = tThisWorld.atClayton[PlayerStartIndex].health * .01;
			}
			else
			{
				tUIVertexBuffer.start = -1;
				tUIVertexBuffer.end = -1;
				tUIVertexBuffer.ratio = -1;
			}

			tUIPixelBuffer.hoverColor = tThisWorld.atBar[nCurrentEntity].backgroundColor;

			if (tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse != nullptr)
				tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse = nullptr;

			pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}
	}

	if (pcInputSystem->InputCheck(G_KEY_V))
	{
		GameOver = true;
		InitializeEndScreen(true);
	}

	if (pcInputSystem->InputCheck(G_KEY_B))
	{
		tThisWorld.atClayton[PlayerStartIndex].health = 0;
		GameOver = true;
		InitializeEndScreen(false);
	}

	clickTime += fpsTimer.GetDelta();
	//double samples = 0;
	//try
	//{
	//	clickTimer.Throttle(64);
	//	samples = clickTimer.SamplesPerSecond();
	//	if (samples > 64)
	//	{
	//		throw samples;
	//	}
	//}
	//catch(double e)
	//{
	//	if (samples > 64)
	//	{
	//		std::cout << "Exception Occurred:\t" << samples << " samples per second\n";
	//	}
	//}

	float fps = fpsTimer.UpdateFrameTime();

	tUIVertexBuffer.start = -1;
	tUIVertexBuffer.end = -1;
	tUIVertexBuffer.ratio = -1;

	tUIPixelBuffer.hoverColor = tThisWorld.atLabel[1089].color;

	wchar_t* textBuffer = new wchar_t[3];

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

	pcUISystem->UpdateText(&tThisWorld, 1089, &atUIVertices, textBuffer, 3, atUIVertices.at(tThisWorld.atLabel[1089].vIndex));

	pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, 1089);

	delete[] textBuffer;

	pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[1089], menuCamera->d3d_Position);
	pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[1089].m_nIndexCount, tThisWorld.atGraphicsMask[1089].m_tnGraphicsMask, tThisWorld.atShaderID[1089].m_nShaderID);

	fpsTimer.Throttle(60);

	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);
	zValue += 0.001;

	return 14;
}
