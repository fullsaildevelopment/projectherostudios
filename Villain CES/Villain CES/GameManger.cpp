#include "GameManger.h"
#define AI_ON true
#define MIKES_SANDBOX_ON false
#define SKELETON_LOAD_ON false
#define MAIN_LEVEL_ON true
#define INPUT_ABSTRACTED_ON true

#define NUMBER_OF_AI 5
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
	pcParticleSystem = new Particle_System();
#if MUSIC_ON
	pcAudioSystem = new CAudioSystem();
	
#endif

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

int CGameMangerSystem::LoadMainMenu()
{
	fpsTimer.Xtime_Signal();
#if MUSIC_ON
	if (pcInputSystem->InputCheck(G_KEY_F9) == 1)
	{
		pcAudioSystem->m_fMusicVolume += 1.0f;

	}
	else if (pcInputSystem->InputCheck(G_KEY_F8) == 1)
	{
		pcAudioSystem->m_fMusicVolume -= 1.0f;
	}
	pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::MUSIC_VOLUME, pcAudioSystem->m_fMusicVolume);
#endif

	//////////
	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective();
	//////////
	m_RealTimeFov = pcInputSystem->ZoomSight(m_RealTimeFov);
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective(m_RealTimeFov);

	CGraphicsSystem::TUIVertexBufferType tUIVertexBuffer;
	CGraphicsSystem::TUIPixelBufferType tUIPixelBuffer;

	hoverPoint = { -1, -1 };
	GetCursorPos(&hoverPoint);
	ScreenToClient(cApplicationWindow, &hoverPoint);

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
			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_LOADING))
			{
				tUIVertexBuffer.start = -1;
				tUIVertexBuffer.end = -1;
				tUIVertexBuffer.ratio = -1;
				tUIVertexBuffer.padding = -1;
				float opacity;

				if (fadeOut)
				{
					opacity = fadeTime;

					if (opacity > 1)
					{
						fadeTime = 0;
						fadeOut = false;
						fadeIn = true;

						return 11;
					}

					tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, opacity);
				}
				else
				{
					tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;
				}

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}

			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL))
			{
				tUIVertexBuffer.start = -1;
				tUIVertexBuffer.end = -1;
				tUIVertexBuffer.ratio = -1;
				tUIVertexBuffer.padding = -1;

				tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}

			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON))
			{
				tUIVertexBuffer.start = -1;
				tUIVertexBuffer.end = -1;
				tUIVertexBuffer.ratio = -1;

				tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

				if (tThisWorld.atButton[nCurrentEntity].enabled && clickTime > TIMEUNTILCLICK)
				{
					if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
					{
#if MUSIC_ON
						pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_MENU_CLICK, pcAudioSystem->m_MenuClick);
						pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);
#endif
						clickTime = 0;

						if (tThisWorld.atButton[nCurrentEntity].sceneIndex == -1)
						{
							atUIVertices.clear();
							atUIIndices.clear();

							clickPoint = { -1, -1 };
							dragPoint = { -1, -1 };

							return tThisWorld.atButton[nCurrentEntity].sceneIndex;
						}
						else if (tThisWorld.atButton[nCurrentEntity].sceneIndex < SAVE_INDEX)
						{
							atUIVertices.clear();
							atUIIndices.clear();

							clickPoint = { -1, -1 };
							dragPoint = { -1, -1 };

							fadeOut = true;
						}
						else if (tThisWorld.atButton[nCurrentEntity].sceneIndex == OPTIONS_INDEX)
						{
							clickPoint = { -1, -1 };
							dragPoint = { -1, -1 };

							options = true;
						}
						else if (tThisWorld.atButton[nCurrentEntity].sceneIndex == CREDITS_INDEX)
						{
							clickPoint = { -1, -1 };
							dragPoint = { -1, -1 };

							credits = true;
						}
					}
					else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
					{
						tUIPixelBuffer.hoverColor = XMFLOAT4(.6, .6, .6, 0);
						if (tThisWorld.atButton[nCurrentEntity].hovered == false)
						{
#if MUSIC_ON
							pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_HOVERSOUND, pcAudioSystem->m_HoverSound);
							pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);
#endif
						}
						tThisWorld.atButton[nCurrentEntity].hovered = true;

					}
					else
					{
						tThisWorld.atButton[nCurrentEntity].hovered = false;
					}

				}
					pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
					pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}
		}
		else if (options && !credits)
		{
			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON | COMPONENT_OPTIONS))
			{
				tUIVertexBuffer.start = -1;
				tUIVertexBuffer.end = -1;
				tUIVertexBuffer.ratio = -1;
				tUIVertexBuffer.padding = -1;

				tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

				if (tThisWorld.atButton[nCurrentEntity].enabled && clickTime > TIMEUNTILCLICK)
				{
					if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
					{
						clickTime = 0;
						//Click sound for menus here - ZFB
#if MUSIC_ON
						pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_MENU_CLICK, pcAudioSystem->m_MenuClick);
						pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);
#endif
						if (tThisWorld.atButton[nCurrentEntity].sceneIndex == 3)
						{
							clickPoint = { -1, -1 };
							dragPoint = { -1, -1 };

							options = false;
						}
					}
					else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
					{
						tUIPixelBuffer.hoverColor = XMFLOAT4(.6, .6, .6, 0);
						if (tThisWorld.atButton[nCurrentEntity].hovered == false)
						{
#if MUSIC_ON
							pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_HOVERSOUND, pcAudioSystem->m_HoverSound);
							pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);
#endif
						}
						tThisWorld.atButton[nCurrentEntity].hovered = true;

					}
					else
					{
						tThisWorld.atButton[nCurrentEntity].hovered = false;
					}
					
				}

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}

			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_OPTIONS))
			{
				tUIVertexBuffer.start = -1;
				tUIVertexBuffer.end = -1;
				tUIVertexBuffer.ratio = -1;

				tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}

			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BAR | COMPONENT_OPTIONS))
			{
				if (PtInRect(&tThisWorld.atBar[nCurrentEntity].barBoundingBox, clickPoint))
				{
					tThisWorld.atBar[nCurrentEntity].ratio = (clickPoint.x - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left + 0.0) / (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left);
				
					pcUISystem->CheckOptionsBars(&tThisWorld, pcGraphicsSystem, pcInputSystem, nCurrentEntity, pcAudioSystem->m_fMasterVolume, pcAudioSystem->m_fMusicVolume, pcAudioSystem->m_fSFXVolume, masterIndex, musicIndex, fxIndex, pcGraphicsSystem->m_CurrentAmbience, brightnessIndex);
#if MUSIC_ON
					if (click == true && fxIndex == nCurrentEntity)
					{
						pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_S_FEEDBACK, pcAudioSystem->m_FeedBack);
						pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);

					}
#endif
				}
				else if (PtInRect(&tThisWorld.atBar[nCurrentEntity].barBoundingBox, dragPoint))
				{
					tThisWorld.atBar[nCurrentEntity].ratio = (dragPoint.x - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left + 0.0) / (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left);

					pcUISystem->CheckOptionsBars(&tThisWorld, pcGraphicsSystem, pcInputSystem, nCurrentEntity, pcAudioSystem->m_fMasterVolume, pcAudioSystem->m_fMusicVolume, pcAudioSystem->m_fSFXVolume, masterIndex, musicIndex, fxIndex, pcGraphicsSystem->m_CurrentAmbience, brightnessIndex);
				}

				if (tThisWorld.atBar[nCurrentEntity].backgroundColor.x == 1 &&
					tThisWorld.atBar[nCurrentEntity].backgroundColor.y == 0 &&
					tThisWorld.atBar[nCurrentEntity].backgroundColor.z == 0 &&
					tThisWorld.atBar[nCurrentEntity].backgroundColor.w == 1)
				{
					tUIVertexBuffer.start = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.left - (screenWidth * .5)) / (screenWidth * .5);
					tUIVertexBuffer.end = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right - (screenWidth * .5)) / (screenWidth * .5);
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
		else if (!options && credits)
		{
			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_CREDITS))
			{
				tUIVertexBuffer.start = -1;
				tUIVertexBuffer.end = -1;
				tUIVertexBuffer.ratio = -1;

				tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}

			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON | COMPONENT_CREDITS))
			{
				tUIVertexBuffer.start = -1;
				tUIVertexBuffer.end = -1;
				tUIVertexBuffer.ratio = -1;
				tUIVertexBuffer.padding = -1;

				tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

				if (tThisWorld.atButton[nCurrentEntity].enabled && clickTime > TIMEUNTILCLICK)
				{
					if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
					{
						clickTime = 0;
						//Click soud for menus here - ZFB
#if MUSIC_ON
						pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_MENU_CLICK, pcAudioSystem->m_MenuClick);
						pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);
#endif
						if (tThisWorld.atButton[nCurrentEntity].sceneIndex == 3)
						{
							clickPoint = { -1, -1 };
							dragPoint = { -1, -1 };

							credits = false;
						}
					}
					else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
					{
						tUIPixelBuffer.hoverColor = XMFLOAT4(.6, .6, .6, 0);
						if (tThisWorld.atButton[nCurrentEntity].hovered == false)
						{
#if MUSIC_ON
							pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_HOVERSOUND, pcAudioSystem->m_HoverSound);
							pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);
#endif
						}
						tThisWorld.atButton[nCurrentEntity].hovered = true;

					}
					else
					{
						tThisWorld.atButton[nCurrentEntity].hovered = false;
					}
					
				}

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
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

	if (fadeOut)
	{
		fadeTime += fpsTimer.GetDelta();
	}

#if MIKES_SANDBOX_ON
	return 9;
#endif
	return 3;
}

void CGameMangerSystem::InitializeMainMenu()
{
	// Music Stuff 
#if MUSIC_ON
	AK::SoundEngine::StopAll();
	if (soundOff == true)
	{
		pcAudioSystem->UnRegisterGameObj(pcAudioSystem->m_AkHallwayBattle);
		pcAudioSystem->UnRegisterGameObj(pcAudioSystem->m_MetalReload);
		pcAudioSystem->UnRegisterGameObj(pcAudioSystem->m_AkMetalFired);
		pcAudioSystem->UnRegisterGameObj(pcAudioSystem->m_Laser_Fire);
		pcAudioSystem->UnRegisterGameObj(pcAudioSystem->m_Human_Hurt);
		pcAudioSystem->UnRegisterGameObj(pcAudioSystem->m_Scylian_Hurt);
		pcAudioSystem->UnRegisterGameObj(pcAudioSystem->m_GunEmpty);
		pcAudioSystem->UnRegisterGameObj(pcAudioSystem->m_Extract);
		pcAudioSystem->UnRegisterGameObj(pcAudioSystem->m_Heal);
		pcAudioSystem->RegisterGameObj(pcAudioSystem->m_AkMainMenuMusic);
		soundOff = false;
	}
	else
	{
		pcAudioSystem->RegisterGameObj(pcAudioSystem->m_HoverSound);
		pcAudioSystem->RegisterGameObj(pcAudioSystem->m_FeedBack);
		pcAudioSystem->RegisterGameObj(pcAudioSystem->m_AkMainMenuMusic);
	}
	pcAudioSystem->m_playOnce1 = false;
	pcAudioSystem->m_playOnce2 = false;
	pcAudioSystem->m_playOnce3 = false;
	pcAudioSystem->m_playOnce4 = false;
	pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_MAIN_MENU_MUSIC, pcAudioSystem->m_AkMainMenuMusic);
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
		{ L"UI_Textures.fbm/FontB_Updated.png" };

		CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, filepath, &resource, &fontTexture, NULL);
	}

	int nThisEntity;
	{
		wchar_t wideChar[] =
		{ L"UI_Textures.fbm/Auger_MainMenu.png" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 20, 20, 0, 0, &atUIVertices, -1, .2);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, wideChar);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 1);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2.4, 1, .51, 0, &atUIVertices, -1, .15);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 1);
	}

	{
		wchar_t textBuffer[] =
		{ L"NEW GAME" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 2.2, 1, .6, 0, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 24, -1, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

#if MIKES_SANDBOX_ON
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 9, true);

#endif
#if SKELETON_LOAD_ON
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 7, true);

#endif
#if MAIN_LEVEL_ON
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 11, true);

#endif

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	//{
	//	nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2.4, 1, .51, -1.2, &atUIVertices, -1, .15);
	//
	//	tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 1);
	//}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2.4, 1, .51, -1.2, &atUIVertices, -1, .15);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 1);
	}

	{
		wchar_t textBuffer[] =
		{ L"OPTIONS" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 2, 1, .6, -1.2, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 24, -1, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, OPTIONS_INDEX, true);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2.4, 1, .51, -2.4, &atUIVertices, -1, .15);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 1);
	}

	{
		wchar_t textBuffer[] =
		{ L"CREDITS" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 2, 1, .6, -2.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 24, -1, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, CREDITS_INDEX, true);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2.4, 1, .51, -3.6, &atUIVertices, -1, .15);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 1);
	}

	{
		wchar_t textBuffer[] =
		{ L"EXIT" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 1.2, 1, .65, -3.6, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 24, -1, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, -1, true);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"Press 'NEW GAME' to start" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 5, 1, 7.2, -8.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 12, -1, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		wchar_t wideChar[] =
		{ L"UI_Textures.fbm/transparentSquareB.png" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 20, 20, 0, 0, &atUIVertices, -1, .05);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, wideChar);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_LOADING);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	InitializeOptionsMenu();
	InitializeCredits();

	pcGraphicsSystem->CreateBuffers(&tThisWorld);

	fadeTime = 0;
}

void CGameMangerSystem::CleanStory()
{
	pcGraphicsSystem->CleanD3DObject(&tThisWorld, loadingImageIndex);
	pcGraphicsSystem->CleanD3DObject(&tThisWorld, loadingTextIndex + 1);
	pcGraphicsSystem->CleanD3DObject(&tThisWorld, loadingTextIndex--);
	pcGraphicsSystem->CleanD3DObject(&tThisWorld, loadingTextIndex--);
	pcGraphicsSystem->CleanD3DObject(&tThisWorld, loadingTextIndex--);
	pcGraphicsSystem->CleanD3DObject(&tThisWorld, loadingTextIndex--);
}

int CGameMangerSystem::LoadStory()
{
	fpsTimer.Xtime_Signal();

	CGraphicsSystem::TUIVertexBufferType tUIVertexBuffer;
	CGraphicsSystem::TUIPixelBufferType tUIPixelBuffer;

	pcGraphicsSystem->UpdateD3D();

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BACKGROUND | COMPONENT_LOADING))
		{
			tUIVertexBuffer.start = -1;
			tUIVertexBuffer.end = -1;
			tUIVertexBuffer.ratio = -1;
			tUIVertexBuffer.padding = -1;

			tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

			pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

		if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL) && nCurrentEntity == loadingTextIndex)
		{
			tUIVertexBuffer.start = -1;
			tUIVertexBuffer.end = -1;
			tUIVertexBuffer.ratio = -1;
			tUIVertexBuffer.padding = -1;

			tUIPixelBuffer.hoverColor = tThisWorld.atLabel[loadingTextIndex].color;

			//double scrollSpeed = .05;
			//pcUISystem->ScrollText(&tThisWorld, pcGraphicsSystem, loadingTextIndex, &atUIVertices, scrollSpeed * fpsTimer.GetDelta());

			pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[loadingTextIndex], menuCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[loadingTextIndex].m_nIndexCount, tThisWorld.atGraphicsMask[loadingTextIndex].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

		if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_LOADING))
		{
			tUIVertexBuffer.start = -1;
			tUIVertexBuffer.end = -1;
			tUIVertexBuffer.ratio = -1;
			tUIVertexBuffer.padding = -1;

			float opacity;
			if (fadeIn && fadeOut)
			{
				opacity = 1 - (fadeTime);

				fadeTime = (opacity + 1) * .2;

				fadeIn = false;
			}
			else if (fadeIn)
			{
				opacity = 1 - (fadeTime);

				if (opacity < 0)
				{
					fadeIn = false;

					fadeTime = 0;
				}

				tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, opacity);
			}
			else if (fadeOut || timeOutTime > 10)
			{
				opacity = fadeTime * 2;

				if (opacity > 1 || timeOutTime > 10)
				{
					fadeTime = 0;
					blinkTime = 0;
					timeOutTime = 0;

					loading = false;
					fadeOut = false;
					fadeIn = true;

					if (loadingImage >= 3)
					{
						loadingImage = 0;
#if MUSIC_ON
						AK::SoundEngine::StopAll();
						CleanStory();
						pcAudioSystem->UnRegisterGameObj(pcAudioSystem->m_Story1);
						pcAudioSystem->UnRegisterGameObj(pcAudioSystem->m_Story2);
						pcAudioSystem->UnRegisterGameObj(pcAudioSystem->m_Story3);
						pcAudioSystem->UnRegisterGameObj(pcAudioSystem->m_Story4);
						pcAudioSystem->UnloadBankFile(STORY_BNK, pcAudioSystem->m_Story_bnkID, ErrorResult);
#endif
						return 13;
					}
					else
					{
						++loadingImage;
						++loadingTextIndex;

						switch (loadingImage)
						{
						case 1:
						{
							wchar_t filePath[] =
							{ L"UI_Textures.fbm/IntroImage_02.png" };

							pcUISystem->AddTextureToUI(&tThisWorld, loadingImageIndex, pcGraphicsSystem->m_pd3dDevice, filePath);
						}
						break;
						case 2:
						{
							wchar_t filePath[] =
							{ L"UI_Textures.fbm/IntroImage_03.png" };

							pcUISystem->AddTextureToUI(&tThisWorld, loadingImageIndex, pcGraphicsSystem->m_pd3dDevice, filePath);
						}
						break;
						case 3:
						{
							wchar_t filePath[] =
							{ L"UI_Textures.fbm/IntroImage_04.png" };

							pcUISystem->AddTextureToUI(&tThisWorld, loadingImageIndex, pcGraphicsSystem->m_pd3dDevice, filePath);
						}
						break;
						default:
							break;
						}
					}
				}

				tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, opacity);
			}
			else
			{
				tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 0);
			}
#if MUSIC_ON
			if (loadingImage == 0 && pcAudioSystem->m_playOnce1 == false)
			{

				AK::SoundEngine::StopAll();
				pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_STORY_1, pcAudioSystem->m_Story1);
				pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);
				pcAudioSystem->m_playOnce1 = true;
			}
			else if (loadingImage == 1 && pcAudioSystem->m_playOnce2 == false)
			{
				AK::SoundEngine::StopAll();
				pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_STORY_2, pcAudioSystem->m_Story2);
				pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);
				pcAudioSystem->m_playOnce2 = true;

			}
			else if (loadingImage == 2 && pcAudioSystem->m_playOnce3 == false)
			{
				AK::SoundEngine::StopAll();
				pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_STORY_3, pcAudioSystem->m_Story3);
				pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);

				pcAudioSystem->m_playOnce3 = true;

			}
			else if (loadingImage == 3 && pcAudioSystem->m_playOnce4 == false)
			{
				AK::SoundEngine::StopAll();
				pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_STORY_4, pcAudioSystem->m_Story4);
				pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);
				pcAudioSystem->m_playOnce4 = true;

			}
#endif

			pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

		/*if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON | COMPONENT_LOADING))
		{
			if (blinkTime < 1)
			{
				if (tThisWorld.atButton[nCurrentEntity].enabled == false)
				{
					tUIVertexBuffer.start = -1;
					tUIVertexBuffer.end = -1;
					tUIVertexBuffer.ratio = -1;
					tUIVertexBuffer.padding = -1;

					tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

					pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
					pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
				}
			}
			else if (blinkTime > 1.8)
			{
				blinkTime = 0;
			}
		}*/
	}
	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);

	for (auto i = G_KEY_UNKNOWN; i <= G_KEY_9; ++i)
	{
		if (pcInputSystem->InputCheck(i) == 1 && !fadeOut)
		{
			fadeOut = true;
		}
	}

	if (pcInputSystem->InputCheck(G_BUTTON_LEFT) && !fadeOut)
	{
		clickTime = 0;

		fadeOut = true;
	}

	if (fadeOut || fadeIn)
	{
		fadeTime += fpsTimer.GetDelta();
	}

	clickTime += fpsTimer.GetDelta();
	blinkTime += fpsTimer.GetDelta();
	timeOutTime += fpsTimer.GetDelta();

	return 12;
}

void CGameMangerSystem::InitializeStory()
{
#if MUSIC_ON
	pcAudioSystem->LoadBankFile(STORY_BNK, pcAudioSystem->m_Story_bnkID, ErrorResult);
	pcAudioSystem->RegisterGameObj(pcAudioSystem->m_Story1);
	pcAudioSystem->RegisterGameObj(pcAudioSystem->m_Story2);
	pcAudioSystem->RegisterGameObj(pcAudioSystem->m_Story3);
	pcAudioSystem->RegisterGameObj(pcAudioSystem->m_Story4);
#endif
	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);
	atUIVertices.clear();
	atUIIndices.clear();

	int nThisEntity;

	{
		wchar_t wideChar[] =
		{ L"UI_Textures.fbm/IntroImage_01.png" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 20, 20, 0, 0, &atUIVertices, -1, .2);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, wideChar);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_LOADING);
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_BACKGROUND);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 1);

		loadingImageIndex = nThisEntity;
	}

	/*{
		wchar_t wideChar[] =
		{ L"UI_Textures.fbm/IntroImage_02.png" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 20, 20, 0, 0, &atUIVertices, -1, .2);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, wideChar);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_LOADING);
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_BACKGROUND);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 1);
	}

	{
		wchar_t wideChar[] =
		{ L"UI_Textures.fbm/IntroImage_03.png" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 20, 20, 0, 0, &atUIVertices, -1, .2);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, wideChar);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_LOADING);
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_BACKGROUND);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 1);
	}

	{
		wchar_t wideChar[] =
		{ L"UI_Textures.fbm/IntroImage_04.png" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 20, 20, 0, 0, &atUIVertices, -1, .2);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, wideChar);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_LOADING);
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_BACKGROUND);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 1);
	}*/

	/*{
		wchar_t textBuffer[] =
		{ L"Press Any Key To Skip" };

		nThisEntity = CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 5, 1, 0, -8.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), -1, .15);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, -1, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_LOADING);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 1, 1, 0);
	}*/

	{
		wchar_t textBuffer[] =
		{ L"Voice over the radio: \"Clayton.\" Clayton: \"Yes General K?\" General K: \"Commercial Station 1 in Rannia's Orbit is under attack. All surviving civilians and personnel are holed up in the station's safe room, but it won't be long until the attackers break in.\"" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 15, 3, 0, -8, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 10, -1, .15);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);

		loadingTextIndex = nThisEntity;
	}

	{
		wchar_t textBuffer[] =
		{ L"General K: \"The culprits are a notorious band of Human and Scyllian pirates known as the Gorgonate. They often take contracts, but their motives are unclear as of right now.\"" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 15, 2, 0, -8, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 10, -1, .15);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"General K: \"You are the only one who can make it in time. It's in your hands now. There's no time to get you better equipped, but I had my men prep your ship for you, while you were in my office.\"" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 15, 2, 0, -8, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 10, -1, .15);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"General K: \"All you'll have are the guns you brought with you, including your material gun. There should be a lot of resources for you to scan once you get there, so make use of them how you see fit.\"" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 15, 2, 0, -8, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 10, -1, .15);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		wchar_t wideChar[] =
		{ L"UI_Textures.fbm/transparentSquareB.png" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 20, 20, 0, 0, &atUIVertices, -1, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, wideChar);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_LOADING);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	pcGraphicsSystem->CreateBuffers(&tThisWorld);

	//fadeTime = 0;
}

void CGameMangerSystem::CleanLoadingScreen()
{
	pcGraphicsSystem->CleanD3DObject(&tThisWorld, 12);
	pcGraphicsSystem->CleanD3DObject(&tThisWorld, 13);
	pcGraphicsSystem->CleanD3DObject(&tThisWorld, 14);
}

int CGameMangerSystem::LoadLoadingScreen(bool _continue)
{

	if (_continue)
	{
		fpsTimer.Xtime_Signal();
	}

	CGraphicsSystem::TUIVertexBufferType tUIVertexBuffer;
	CGraphicsSystem::TUIPixelBufferType tUIPixelBuffer;

	pcGraphicsSystem->UpdateD3D();

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BACKGROUND | COMPONENT_LOADING))
		{
			tUIVertexBuffer.start = -1;
			tUIVertexBuffer.end = -1;
			tUIVertexBuffer.ratio = -1;
			tUIVertexBuffer.padding = -1;

			tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

			pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

		if (_continue)
		{
			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_LOADING))
			{
				tUIVertexBuffer.start = -1;
				tUIVertexBuffer.end = -1;
				tUIVertexBuffer.ratio = -1;
				tUIVertexBuffer.padding = -1;

				float opacity;

				if (fadeOut)
				{
					opacity = fadeTime / .5;
					
					if (opacity > 1)
					{
						fadeTime = 0;
						blinkTime = 0;

						loading = false;
						fadeOut = false;
						return 14;
					}

					tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, opacity);
				}
				else
				{
					tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, 0);
				}

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}

			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BUTTON | COMPONENT_LOADING))
			{
				if (blinkTime < 1)
				{
					if (tThisWorld.atButton[nCurrentEntity].enabled == false)
					{
						tUIVertexBuffer.start = -1;
						tUIVertexBuffer.end = -1;
						tUIVertexBuffer.ratio = -1;
						tUIVertexBuffer.padding = -1;

						tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

						pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
						pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
					}
				}
				else if (blinkTime > 1.8)
				{
					blinkTime = 0;
				}
			}
		}
	}
	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);

	if (_continue)
	{
		for (auto i = G_KEY_UNKNOWN; i <= G_KEY_9; ++i)
		{
			if (pcInputSystem->InputCheck(i) == 1)
			{
				fadeOut = true;
			}
		}

		if (pcInputSystem->InputCheck(G_BUTTON_LEFT))
		{
			clickTime = 0;

			fadeOut = true;
		}

		if (fadeOut)
		{
			fadeTime += fpsTimer.GetDelta();
		}

		clickTime += fpsTimer.GetDelta();
		blinkTime += fpsTimer.GetDelta();
	}

	return 13;
}

void CGameMangerSystem::InitializeLoadingScreen()
{
	int nThisEntity;

	{
		wchar_t wideChar[] =
		{ L"UI_Textures.fbm/Keyboard_Layout.png" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 20, 20, 0, 0, &atUIVertices, -1, .2);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, wideChar);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_LOADING);
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_BACKGROUND);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 1);
	}

	{
		wchar_t textBuffer[] =
		{ L"Press Any Key To Continue" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 5, 1, 0, -8.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 12, -1, .15);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, -1, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_LOADING);
	
		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		wchar_t wideChar[] =
		{ L"UI_Textures.fbm/transparentSquareB.png" };
	
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 20, 20, 0, 0, &atUIVertices, -1, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, wideChar);
	
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_LOADING);
	
		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	pcGraphicsSystem->CreateBuffers(&tThisWorld);

	fadeTime = 0;
}

int CGameMangerSystem::LoadTitleScreen()
{
	fpsTimer.Xtime_Signal();


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
				float opacity;

				if (fadeIn)
				{
					opacity = 1 - (fadeTime);

					if (opacity < 0)
					{
						fadeIn = false;

						fadeTime = 0;
					}
				}
				else if (fadeOut)
				{
					if (fadeTime > .51)
					{
						fadeTime = fadeTime * .1;
					}

					opacity = fadeTime / .5;
					
					if (opacity > 1)
					{
						fadeTime = 0;

						fadeIn = true;
						fadeOut = false;
						return 2;
					}
				}
				else
				{
					opacity = fadeTime / 5;

					if (opacity > 1)
					{
						fadeTime = 0;

						fadeIn = true;
						fadeOut = false;
						return 2;
					}
				}

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

			fadeOut = true;
		}
	}

	if (pcInputSystem->InputCheck(G_BUTTON_LEFT))
	{
		clickTime = 0;

		fadeOut = true;
	}

	fadeTime += fpsTimer.GetDelta();
	clickTime += fpsTimer.GetDelta();

	return 1;
}

void CGameMangerSystem::InitializeTitleScreen()
{
#if MUSIC_ON
	pcAudioSystem->IntiializeSystem(ErrorResult);
	pcAudioSystem->SetBanksFolderPath(AKTEXT("../Villain CES/WwiseSounds/Windows"));
	pcAudioSystem->RegisterGameObj(pcAudioSystem->Listener);
	pcAudioSystem->RegisterGameObj(pcAudioSystem->m_MenuClick);
	pcAudioSystem->SetListener(pcAudioSystem->Listener, 1, ErrorResult);

	pcAudioSystem->LoadBankFile(INIT_BNK, pcAudioSystem->init_bnkID, ErrorResult);
	pcAudioSystem->LoadBankFile(MAINMENU_BNK, pcAudioSystem->MainMenu_bnkID, ErrorResult);
	pcAudioSystem->LoadBankFile(SFX, pcAudioSystem->m_SFX_bnkID, ErrorResult);
#endif 


	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);
	atUIVertices.clear();
	atUIIndices.clear();

	int nThisEntity;

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

void CGameMangerSystem::InitializeEndScreen(bool playerWin)
{
	
	int nThisEntity;

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

			endScreenBackgroundIndex = nThisEntity;
		}

		{
			nThisEntity = createEntityReverse(&tThisWorld);
			CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, 1, 0, 4.8, &atUIVertices, nThisEntity, .15);

			tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(.7, .7, .7, .5);
		}

		{
			wchar_t textBuffer[] =
			{ L"You Completed The Objective" };

			nThisEntity = createEntityReverse(&tThisWorld);
			CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 6, 1, 0, 2.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, 0.1);

			pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

			pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_DEATHSCREEN);

			tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
		}

		{
			nThisEntity = createEntityReverse(&tThisWorld);
			CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 6, 1, 0, 2.4, &atUIVertices, nThisEntity, .15);

			tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(.7, .7, .7, .5);
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

		{
			nThisEntity = createEntityReverse(&tThisWorld);
			CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 1.4, 1, 0, -1.2, &atUIVertices, nThisEntity, .15);

			tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(.7, .7, .7, .5);
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
		
			endScreenBackgroundIndex = nThisEntity;
		}

		{
			nThisEntity = createEntityReverse(&tThisWorld);
			CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, 1, 0, 4.8, &atUIVertices, nThisEntity, .15);

			tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(.7, .7, .7, .5);
		}

		{
			wchar_t textBuffer[] =
			{ L"You Failed To Complete The Objective" };

			nThisEntity = createEntityReverse(&tThisWorld);
			CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 6, 1, 0, 2.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, 0.1);

			pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

			pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_DEATHSCREEN);

			tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
		}

		{
			nThisEntity = createEntityReverse(&tThisWorld);
			CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 6, 1, 0, 2.4, &atUIVertices, nThisEntity, .15);

			tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(.7, .7, .7, .5);
		}

		{
			wchar_t textBuffer[] =
			{ L"CONTINUE" };

			nThisEntity = createEntityReverse(&tThisWorld);
			CreateUILabelForText(&tThisWorld, menuCamera->d3d_Position, 1.4, 1, -1, -1.2, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), nThisEntity, .1);

			pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
			pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 16, true);

			pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_DEATHSCREEN);
			tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
		}

		{
			nThisEntity = createEntityReverse(&tThisWorld);
			CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 1.4, 1, -1, -1.2, &atUIVertices, nThisEntity, .15);

			tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(.7, .7, .7, .5);
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

		{
			nThisEntity = createEntityReverse(&tThisWorld);
			CreateUILabel(&tThisWorld, menuCamera->d3d_Position, .8, 1, 1, -1.2, &atUIVertices, nThisEntity, .15);

			tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(.7, .7, .7, .5);
		}
	}

	pcGraphicsSystem->CreateBuffers(&tThisWorld);
	ShowCursor(true);
}

void CGameMangerSystem::InitializeOptionsMenu()
{
	signed int nThisEntity;

	if (fontTexture == nullptr)
	{
		ID3D11Resource* resource;

		wchar_t filepath[] =
		{ L"UI_Textures.fbm/FontB_Updated.png" };

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

	/*{
		wchar_t textBuffer[] =
		{ L"SUBTITLES:" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 2.05, .8, -1.22, 6.1, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}*/

	{
		wchar_t textBuffer[] =
		{ L"MASTER VOLUME:" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 3, .8, -1.4, 3.7, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		char valueToChange[] =
		{ "Master Volume" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 3.6, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), valueToChange, ARRAYSIZE(valueToChange));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
#if MUSIC_ON
		tThisWorld.atBar[nThisEntity].ratio = pcAudioSystem->m_fMasterVolume * .01;
#endif
		masterIndex = nThisEntity;
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 3.6, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 1, 1, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	/*{
		wchar_t textBuffer[] =
		{ L"DIALOGUE VOLUME:" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 3.4, .8, -2.385, 3.7, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, -1, 0.1);
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

		tThisWorld.atBar[nThisEntity].ratio = pcAudioSystem->m_fDialogueVolume * .01;
		dialogueIndex = nThisEntity;
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 3.6, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 1, 1, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}*/

	{
		wchar_t textBuffer[] =
		{ L"MUSIC VOLUME:" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 2.8, .8, -1.35, 2.5, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, -1, 0.1);
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
#if MUSIC_ON
		tThisWorld.atBar[nThisEntity].ratio = pcAudioSystem->m_fMusicVolume * .01;
#endif // MUSIC_ON

		musicIndex = nThisEntity;
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 2.4, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 1, 1, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"FX VOLUME:" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.18, 1.3, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, -1, 0.1);
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
#if MUSIC_ON
		tThisWorld.atBar[nThisEntity].ratio = pcAudioSystem->m_fSFXVolume * .01;
#endif
		fxIndex = nThisEntity;
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 1.2, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 1, 1, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	/*{
		wchar_t textBuffer[] =
		{ L"DIFFICULTY:" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.34, 1.3, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}*/

	{
		wchar_t textBuffer[] =
		{ L"BRIGHTNESS:" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.35, .1, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		char valueToChange[] =
		{ "Brightness" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, -0, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), valueToChange, ARRAYSIZE(valueToChange));

		if (pcGraphicsSystem->m_CurrentAmbience != -1)
		{
			tThisWorld.atBar[nThisEntity].ratio = pcGraphicsSystem->m_CurrentAmbience;
		}
		else
		{
			tThisWorld.atBar[nThisEntity].ratio = .5;
		}

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		brightnessIndex = nThisEntity;
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, -0, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 1, 1, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"SENSITIVITY:" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.51, -1.1, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		char valueToChange[] =
		{ "Sensitivity" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, -1.2, &atUIVertices, -1, 0.1);
		// weird little formula to convert the MouseRotationSpeed from 0.003-0.008 to 0-1
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), valueToChange, ARRAYSIZE(valueToChange), (pcInputSystem->GetMouseRotationSpeed() - .003) * 200);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, -1.2, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 1, 1, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"BACK" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 1.2, 1, 0.1, -2.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 24, -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, nCurrentScene + 1, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"Press 'BACK' to return to the main menu" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 4, 2, 7.2, -8.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 12, -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}
}

void CGameMangerSystem::InitializeCredits()
{
	int nThisEntity;
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

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 1.75, 1, 0, 4.8, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 20, -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_CREDITS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	/*{
		wchar_t textBuffer[] =
		{ L"Annemarie Soto - \nArthur Parker - AI Programmer, Gameplay Programmer\nJonah Rogers - UI Programmer, Gameplay Programmer\nMicheal Manahan - Lead Developer, Graphics Programmer\nZayne Bakir - Audio SDK Integration, Input System Integration, Gameplay Programmer\nWalker Hoisington - Game Designer, Artist, Sound Designer, Voice Actor" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 10, 6, 0, -8, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, nThisEntity, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_CREDITS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}*/
	{
		wchar_t textBuffer[] =
		{ L"Annemarie Soto - 3D Artist" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 10, 1, 0, 2.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, nThisEntity, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_CREDITS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}
	{
		wchar_t textBuffer[] =
		{ L"Arthur Parker - AI Programmer, Gameplay Programmer" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 10, 1, 0, 1.2, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, nThisEntity, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_CREDITS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}
	{
		wchar_t textBuffer[] =
		{ L"Jonah Rogers - UI Programmer, Gameplay Programmer" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 10, 1, 0, 0, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, nThisEntity, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_CREDITS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}
	{
		wchar_t textBuffer[] =
		{ L"Michael Manahan - Lead Developer, Graphics Programmer" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 10, 1, 0, -1.2, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, nThisEntity, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_CREDITS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}
	{
		wchar_t textBuffer[] =
		{ L"Zayn Bakir - Audio SDK Integration, Input System Integration, Gameplay Programmer" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 10, 2, 0, -3, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, nThisEntity, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_CREDITS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}
	{
		wchar_t textBuffer[] =
		{ L"Walker Hoisington - Game Designer, 3D Artist, 2D Artist, Sound Designer, Voice Actor" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 10, 2, 0, -5.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, nThisEntity, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_CREDITS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"BACK" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 1, 1, -9, -9, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 20, -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, nCurrentScene + 1, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_CREDITS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"Press 'BACK' to return to the main menu" };

		nThisEntity = CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 4, 2, 7.2, -8.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 12, -1, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_CREDITS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);
	}
}

void CGameMangerSystem::InitializePauseScreen()
{
	int nThisEntity;

	{
		wchar_t filePath[] =
		{ L"UI_Textures.fbm/transparentSquareB.png" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 20, 20, 0, 0, &atUIVertices, nThisEntity);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, filePath);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_PAUSESCREEN);
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_DEATHSCREEN);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0.1, 0.1, 0.1, .3);
	}

	{
		wchar_t textBuffer[] =
		{ L"CONTINUE" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 2.3, 1, 0, 2.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 20, nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, nCurrentScene + 1, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_PAUSESCREEN);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}
	
	/*{
		wchar_t textBuffer[] =
		{ L"SAVE" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 1.2, 1, -0.05, 3.6, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 20, nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, SAVE_INDEX, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_PAUSESCREEN);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"LOAD" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 1.2, 1, -0.05, 2.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 20, nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, LOAD_INDEX, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_PAUSESCREEN);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}*/

	{
		wchar_t textBuffer[] =
		{ L"OPTIONS" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 2, 1, 0, 1.2, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 20, nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, OPTIONS_INDEX, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_PAUSESCREEN);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"EXIT" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 1.2, 1, -0.05, 0, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 20, nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 2, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_PAUSESCREEN);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"Press 'U' or 'CONTINUE' to unpause" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 4, 2, 7.2, -8.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 12, nThisEntity, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, -1, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_PAUSESCREEN);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2.4, 4, -.3, 1.2, &atUIVertices, nThisEntity, .15);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_PAUSESCREEN);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(.7, .7, .7, .5);
	}

	/*{
		wchar_t textBuffer[] =
		{ L"SUBTITLES:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 2.05, .8, -1.22, 6.1, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}*/

	{
		wchar_t textBuffer[] =
		{ L"MASTER VOLUME:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 3, .8, -1.4, 3.7, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, nThisEntity, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		char valueToChange[] =
		{ "Master Volume" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 3.6, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), valueToChange, ARRAYSIZE(valueToChange));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atBar[nThisEntity].ratio = pcAudioSystem->m_fMasterVolume * .01;
		masterIndex = nThisEntity;
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 3.6, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"DIALOGUE VOLUME:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 3.4, .8, 20, 3.7, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		char valueToChange[] =
		{ "Dialogue Volume" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, 20, 3.6, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), valueToChange, ARRAYSIZE(valueToChange));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atBar[nThisEntity].ratio = pcAudioSystem->m_fDialogueVolume * .01;
		dialogueIndex = nThisEntity;
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, 20, 3.6, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"MUSIC VOLUME:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 2.8, .8, -1.35, 2.5, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, nThisEntity, .1);
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

		tThisWorld.atBar[nThisEntity].ratio = pcAudioSystem->m_fMusicVolume * .01;
		musicIndex = nThisEntity;
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
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.18, 1.3, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, nThisEntity, .1);
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

		tThisWorld.atBar[nThisEntity].ratio = pcAudioSystem->m_fSFXVolume * .01;
		fxIndex = nThisEntity;
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 1.2, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	/*{
		wchar_t textBuffer[] =
		{ L"DIFFICULTY:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.34, 1.3, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}*/

	{
		wchar_t textBuffer[] =
		{ L"BRIGHTNESS:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.35, .1, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		char valueToChange[] =
		{ "Brightness" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 0, &atUIVertices, -1, 0.1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), valueToChange, ARRAYSIZE(valueToChange));

		tThisWorld.atBar[nThisEntity].ratio = .5;

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		brightnessIndex = nThisEntity;	
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, 0, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"SENSITIVITY:" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 2.15, .8, -1.51, -1.1, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, nThisEntity, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, 0, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		char valueToChange[] =
		{ "Sensitivity" };

		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, -1.2, &atUIVertices, -1, 0.1);
		// weird little formula to convert the MouseRotationSpeed from 0.003-0.008 to 0-1
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), valueToChange, ARRAYSIZE(valueToChange), (pcInputSystem->GetMouseRotationSpeed() - .003) * 200);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		nThisEntity = CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, .5, .5, -1.2, &atUIVertices, -1, 0.15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);
	}

	{
		wchar_t textBuffer[] =
		{ L"BACK" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 1.2, 1, 0.1, -2.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 24, nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, nCurrentScene + 1, true);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		wchar_t textBuffer[] =
		{ L"Press 'U' to unpause or 'BACK'\nto return to the pause menu" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 4, 2.5, 7.2, -8.4, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 12, nThisEntity, 0.1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);
		pcUISystem->AddButtonToUI(&cApplicationWindow, &tThisWorld, nThisEntity, -1, false);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 6, 8, -.3, .6, &atUIVertices, nThisEntity, .15);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_OPTIONS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(.7, .7, .7, .5);
	}

	pcGraphicsSystem->CreateBuffers(&tThisWorld);
}

void CGameMangerSystem::CleanEndScreen(bool playerWin)
{
	if (playerWin)
	{
		pcGraphicsSystem->CleanD3DObject(&tThisWorld, endScreenBackgroundIndex--);
		pcGraphicsSystem->CleanD3DObject(&tThisWorld, endScreenBackgroundIndex--);
		pcGraphicsSystem->CleanD3DObject(&tThisWorld, endScreenBackgroundIndex--);
		pcGraphicsSystem->CleanD3DObject(&tThisWorld, endScreenBackgroundIndex--);
		pcGraphicsSystem->CleanD3DObject(&tThisWorld, endScreenBackgroundIndex--);
		pcGraphicsSystem->CleanD3DObject(&tThisWorld, endScreenBackgroundIndex--);
	}
	else
	{
		pcGraphicsSystem->CleanD3DObject(&tThisWorld, endScreenBackgroundIndex--);
		pcGraphicsSystem->CleanD3DObject(&tThisWorld, endScreenBackgroundIndex--);
		pcGraphicsSystem->CleanD3DObject(&tThisWorld, endScreenBackgroundIndex--);
		pcGraphicsSystem->CleanD3DObject(&tThisWorld, endScreenBackgroundIndex--);
		pcGraphicsSystem->CleanD3DObject(&tThisWorld, endScreenBackgroundIndex--);
		pcGraphicsSystem->CleanD3DObject(&tThisWorld, endScreenBackgroundIndex--);
		pcGraphicsSystem->CleanD3DObject(&tThisWorld, endScreenBackgroundIndex--);
		pcGraphicsSystem->CleanD3DObject(&tThisWorld, endScreenBackgroundIndex--);
	}
}

void CGameMangerSystem::InitializeHUD()
{
	atUIVertices.clear();
	atUIIndices.clear();

	int nThisEntity;

	{
		wchar_t filePath[] =
		{ L"UI_Textures.fbm/Auger_Hitmarker.png" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, .375, .75, -.22, -.4, &atUIVertices, nThisEntity, .05);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, filePath);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(1, 0, 0, 0);

		hitmarkerIndex = nThisEntity;
	}

	{
		wchar_t filePath[] =
		{ L"UI_Textures.fbm/Auger_Reticle.png" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, .75, 1.5, -.22, -.4, &atUIVertices, nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, filePath);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		wchar_t filePath[] =
		{ L"UI_Textures.fbm/transparentSquareB.png" };

		char valueToChange[] =
		{ "ShootingCooldown" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, 2, 6, -8, &atUIVertices, nThisEntity, .05);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, filePath);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, .2), valueToChange, ARRAYSIZE(valueToChange));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
	}

	{
		wchar_t textBuffer[] =
		{ L"00" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, .5, 1, 6.55, -7.5, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 24, nThisEntity, .075);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);

		ammoIndex = nThisEntity;
	}

	{
		wchar_t filePath[] =
		{ L"UI_Textures.fbm/Material_Gun_Paint.png" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 2, 2, 6, -8, &atUIVertices, nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, filePath);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 1);

		gunImageIndex = nThisEntity;
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 3, 1, -7, -8, &atUIVertices, nThisEntity, .05);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 1, 1, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_BACKGROUND);
	}

	{
		char valueToChange[] =
		{ "Health" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 3, 1, -7, -8, &atUIVertices, nThisEntity, .1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(1, 0, 0, 1), valueToChange, ARRAYSIZE(valueToChange));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 3, 1, -7, -8, &atUIVertices, nThisEntity, .15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
	}

	{
		wchar_t filePath[] =
		{ L"UI_Textures.fbm/HeartIcon.png" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, .5, 1, -8.8, -8, &atUIVertices, nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, filePath);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

	{
		char valueToChange[] =
		{ "CaelisAbilityCooldown" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 3, 1, -7, -9.1, &atUIVertices, nThisEntity, .1);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 1, 1), valueToChange, ARRAYSIZE(valueToChange));

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
	}

	{
		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, 3, 1, -7, -9.1, &atUIVertices, nThisEntity, .15);
		pcUISystem->AddBarToUI(&cApplicationWindow, &tThisWorld, nThisEntity, &XMFLOAT4(0, 0, 0, 1), nullptr);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
	}

	{
		wchar_t filePath[] =
		{ L"UI_Textures.fbm/HealthIcon.png" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, .5, 1, -8.8, -9.1, &atUIVertices, nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, filePath);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);
	}

#if FPS
	{
		wchar_t textBuffer[] =
		{ L"000" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 1, 1, -6.625, 7.5, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 20, nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, nullptr, fontTexture);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);
		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_FPS);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);

		fpsIndex = nThisEntity;
	}
#endif

	{
		wchar_t filePath[] =
		{ L"UI_Textures.fbm/objectiveCircle.png" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabel(&tThisWorld, menuCamera->d3d_Position, .5, 1, 4.7, 8.5, &atUIVertices, nThisEntity, .1);
		pcUISystem->AddTextureToUI(&tThisWorld, nThisEntity, pcGraphicsSystem->m_pd3dDevice, filePath);

		pcUISystem->AddMaskToUI(&tThisWorld, nThisEntity, COMPONENT_HUD);

		tThisWorld.atLabel[nThisEntity].color = XMFLOAT4(0, 0, 0, 0);

		objLogoIndex = nThisEntity;
	}

	{
		wchar_t textBuffer[] =
		{ L"Kill The Enemies" };

		nThisEntity = createEntityReverse(&tThisWorld);
		CreateUILabelForText2(&tThisWorld, menuCamera->d3d_Position, 4, 1, 7, 8.5, &atUIVertices, &atUIIndices, textBuffer, ARRAYSIZE(textBuffer), &windowRect, 16, nThisEntity, .1);
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
	bNoMoving = false;
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

	tThisWorld.atCaelis[CaelisIndex].m_tfSpecialCooldown=0;

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
		m_d3d_ResultMatrix = pcInputSystem->WalkCameraControls(XMVectorSet(0, 1.0f, 0, 0), m_d3d_ResultMatrix, bNoMoving, 1);

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
		m_d3d_ResultMatrix = pcInputSystem->DebugCamera(m_d3d_ResultMatrix, m_d3dWorldMatrix, 1);

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
					//m_d3dPlayerMatrix = pcInputSystem->CharacterMovement(m_d3dPlayerMatrix, fpsTimer.GetDelta(), pcAudioSystem, tThisWorld.atClayton[PlayerStartIndex], tThisWorld.atRigidBody[PlayerStartIndex].velocity, bNoMoving);
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
					m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
				}
				else if (tCameraMode.bAimMode == true)
				{
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix =pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
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

						pcAiSystem->AddShootingActiveAI(nCurrentEntity);
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
					pcAiSystem->PathPlaningMovement(&tThisWorld.atPathPlanining[nCurrentEntity], &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, fpsTimer.GetDelta());
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

					int newbullet = CreateBullet(&tThisWorld, gunMatrix,tThisWorld.atClip[nCurrentEntity].currentMaterial, 0); // Last zero is bulletType - 0 for friend, 1 for enemy

					tThisWorld.atClip[newbullet].gunIndex = nCurrentEntity;
					tThisWorld.atSimpleMesh[newbullet].m_nColor = tThisWorld.atClip[nCurrentEntity].colorofBullets;
					tThisWorld.atClip[newbullet].indexInclip = pcProjectileSystem->CreateBulletProjectile(newbullet, &tThisWorld.atClip[nCurrentEntity]);
					tThisWorld.atAABB[newbullet] = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[newbullet].m_VertexData, tThisWorld.atAABB[newbullet]);
					tThisWorld.atAABB[newbullet].m_IndexLocation = newbullet;

					pcCollisionSystem->AddAABBCollider(tThisWorld.atAABB[newbullet], newbullet);
					pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, newbullet);

					tThisWorld.atClip[nCurrentEntity].tryToShoot = false;

				}
				else if (tThisWorld.atClip[nCurrentEntity].tryToShoot == true) 
				{
					tThisWorld.atClip[nCurrentEntity].tryToShoot = false;
				}
				if (tThisWorld.atClip[nCurrentEntity].tryToReload == true) 
				{
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
		tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix =pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);

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


						tTempVertexBuffer.m_d3dWorldMatrix =pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
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

					//	pcAiSystem->MoveAiToCoverLocation(tThisWorld.atCoverTrigger[otherCollisionsIndex[i]], &tThisWorld);
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
	bNoMoving = false;
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
 nodeLocation.r[3].m128_f32[1] -= 0;
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
		m_d3d_ResultMatrix = pcInputSystem->WalkCameraControls(XMVectorSet(0, 1.0f, 0, 0), m_d3d_ResultMatrix, bNoMoving, 1);

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
		m_d3d_ResultMatrix = pcInputSystem->DebugCamera(m_d3d_ResultMatrix, m_d3dWorldMatrix, 1);

		debugCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dWorldMatrix);
	}

	CGraphicsSystem::TPrimalVertexBufferType tTempVertexBuffer;
	CGraphicsSystem::TPrimalPixelBufferType tTempPixelBuffer;
	CGraphicsSystem::TMyVertexBufferType tMyVertexBufferTemp;
	tMyVertexBufferTemp.m_d3dColor = XMFLOAT4(0, 0, 0, 1);

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
					//m_d3dPlayerMatrix = pcInputSystem->CharacterMovement(m_d3dPlayerMatrix, fpsTimer.GetDelta(), pcAudioSystem, tThisWorld.atClayton[PlayerStartIndex], tThisWorld.atRigidBody[PlayerStartIndex].velocity, bNoMoving);
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix =pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
					m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
				}
				else if (tCameraMode.bAimMode == true)
				{
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix =pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
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

						pcAiSystem->AddShootingActiveAI(nCurrentEntity);
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
						pcAiSystem->PathPlaningMovement(&tThisWorld.atPathPlanining[nCurrentEntity], &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, fpsTimer.GetDelta());
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
		tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix =pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);

		if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_RAYGUN)) {
			float CloseEstObject = 10000000000000000000.0f;
			float* distanceCalucaltion = new float();
			for (list<TAABB>::iterator ptr = pcCollisionSystem->m_AAbb.begin(); ptr != pcCollisionSystem->m_AAbb.end(); ++ptr) {

				if (ptr->m_IndexLocation != PlayerStartIndex && ptr->m_IndexLocation != GunIndexForPlayer)
					if (pcCollisionSystem->intersectRayAABox2(XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[0], tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix)
						, XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[1], tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix), *ptr
					) == true) {
						
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


						tTempVertexBuffer.m_d3dWorldMatrix =pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
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

					//	pcAiSystem->MoveAiToCoverLocation(tThisWorld.atCoverTrigger[otherCollisionsIndex[i]], &tThisWorld);
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

	tempImport = pcGraphicsSystem->ReadMesh("meshData_DemoDoors.txt");
	matOpt = pcGraphicsSystem->CreateTexturesFromFile(tempImport.vtMaterials, tempImport.meshCount);
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		int myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], matOpt, tempImport.vtMaterials[meshIndex],meshIndex);
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
	tempImport = pcGraphicsSystem->ReadMesh2("meshData_PirateStrafeRight.txt", CLAYTON, nullptr, 3);
	tempImport = pcGraphicsSystem->ReadMesh2("meshData_PirateStrafeLeft.txt", CLAYTON, &tempImport, 3);
	tempImport = pcGraphicsSystem->ReadMesh2("meshData_PirateMoveBackward.txt", CLAYTON, &tempImport, 3);
	tempImport = pcGraphicsSystem->ReadMesh2("meshData_PirateMoveForward.txt", CLAYTON, &tempImport, 3);
	tempImport = pcGraphicsSystem->ReadMesh2("meshData_PirateIdle.txt", CLAYTON, &tempImport, 3);
	//tempImport = pcGraphicsSystem->ReadMesh2("MoveForward_Clayton_output_animation.mesh", 1);
	//tempImport = pcGraphicsSystem->ReadMesh2("Run_Mage_output_animation.mesh");
	matOpt = pcGraphicsSystem->CreateTexturesFromFile(tempImport.vtMaterials, 1);

	int myMesh;
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		myMesh = createClaytonAnim(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], matOpt, tempImport.vtMaterials[meshIndex],tempImport.vtAnimations, meshIndex, tempImport.animationCount);
	}

	tThisWorld.atWorldMatrix[myMesh].worldMatrix.r[3].m128_f32[0] = -4;
	tThisWorld.atWorldMatrix[myMesh].worldMatrix.r[3].m128_f32[1] = -1.2;
	tThisWorld.atWorldMatrix[myMesh].worldMatrix.r[3].m128_f32[2] = 10.8f;

	XMMATRIX AILocation = pcGraphicsSystem->SetDefaultWorldPosition();
	AILocation.r[3].m128_f32[2] = 10.8;
	AILocation.r[3].m128_f32[0] = 0;
	AILocation.r[3].m128_f32[1] = -1.2;
	/*XMMATRIX CoverLocation = AILocation;
	vector<int> coverPosition;
	CoverLocation.r[3].m128_f32[2] += 4;
	int cover1 = CreateCover(&tThisWorld, CoverLocation, coverPosition);
	XMMATRIX nodeLocation = CoverLocation;
	nodeLocation.r[3].m128_f32[0] += 0;
	nodeLocation.r[3].m128_f32[1] -= 0;
	nodeLocation.r[3].m128_f32[2] += -2;*/

	tempImport = pcGraphicsSystem->ReadMesh2("meshData_SnakeIdle.txt");
	//gunImport = pcGraphicsSystem->ReadMesh("meshData_LaserFlintlockTextured.txt");

	matOpt = pcGraphicsSystem->CreateTexturesFromFile(tempImport.vtMaterials, 1);

	int spacePirate;
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; ++meshIndex)
	{
		spacePirate = CreateScyllianAnim(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], matOpt, AILocation, tempImport.vtAnimations, tempImport.vtMaterials[meshIndex], meshIndex, tempImport.animationCount);
	}

	/*enemyToCopyFrom.atMesh = tThisWorld.atMesh[spacePirate];
	enemyToCopyFrom.atRigidBody = tThisWorld.atRigidBody[spacePirate];
	enemyToCopyFrom.atShaderID = tThisWorld.atShaderID[spacePirate];
	enemyToCopyFrom.atGraphicsMask = tThisWorld.atGraphicsMask[spacePirate];
	enemyToCopyFrom.atCollisionMask = tThisWorld.atCollisionMask[spacePirate];
	enemyToCopyFrom.atPhysicsMask = tThisWorld.atPhysicsMask[spacePirate];
	enemyToCopyFrom.atAIMask = tThisWorld.atAIMask[spacePirate];*/

	tThisWorld.atOffSetMatrix[spacePirate] = AILocation;

	tThisWorld.atAiHeath[spacePirate].heath = 100;
	//createGSQuad(&tThisWorld, XMFLOAT4(1, 0, 0, 1) myMesh);
	//createGSQuad(&tThisWorld, XMFLOAT4(1, 1, 1, 1) myMesh);
	pcGraphicsSystem->SetLights();
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
	TLights* setLights = pcGraphicsSystem->GetLights();
	tCameraMode = pcInputSystem->CameraModeListen(tCameraMode);

	if (tCameraMode.bSwitch == true)
	{
		m_d3d_ResultMatrix = pcInputSystem->CameraOrientationReset(m_d3d_ResultMatrix);
		tCameraMode.bSwitch = false;
	}
	m_d3d_ResultMatrix = pcInputSystem->DebugCamera(m_d3d_ResultMatrix, m_d3dWorldMatrix, .02);

	debugCamera->d3d_Position = XMMatrixMultiply(m_d3d_ResultMatrix, m_d3dWorldMatrix);

#pragma endregion

#pragma region Buffer Declarations
	CGraphicsSystem::TPrimalVertexBufferType tTempVertexBuffer;
	CGraphicsSystem::TPrimalPixelBufferType tTempPixelBuffer;
	CGraphicsSystem::TMyVertexBufferType tMyVertexBufferTemp;
	CGraphicsSystem::TAnimatedVertexBufferType tAnimVertexBuffer;
	CGraphicsSystem::TLightBufferType tLightBuffer;
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
	tThisWorld.atAnimation[0].tTimer.StartClock(tThisWorld.atAnimation[ClaytonIndex].tTimer.tSceneTimer);
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

			/*TLights testLight;
			testLight.m_d3dLightColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1);
			testLight.m_d3dLightPosition = XMFLOAT4(-4, -1.0f, 12.8f, 1);
			testLight.enabled = true;
			testLight.m_lightType = 1;
			testLight.m_Direction = XMFLOAT4(0, -2, -5.0f, 1);
			testLight.m_padding = XMFLOAT2(0, 0);
			tLightBuffer.m_allLights = testLight;*/

			for (int currentLightIndex = 0; currentLightIndex < MAX_LIGHTS; currentLightIndex++)
			{
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_d3dLightPosition, &setLights[currentLightIndex].m_d3dLightPosition, sizeof(setLights[currentLightIndex].m_d3dLightPosition));
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_d3dLightColor, &setLights[currentLightIndex].m_d3dLightColor, sizeof(setLights[currentLightIndex].m_d3dLightColor));
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_lightType, &setLights[currentLightIndex].m_lightType, sizeof(setLights[currentLightIndex].m_lightType));
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_Direction, &setLights[currentLightIndex].m_Direction, sizeof(setLights[currentLightIndex].m_Direction));
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].enabled, &setLights[currentLightIndex].enabled, sizeof(setLights[currentLightIndex].enabled));

			}
			tLightBuffer.m_Proprties = tThisWorld.atMaterial[nCurrentEntity];

			pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], tLightBuffer, debugCamera->d3d_Position);
			pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		}

		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_ANIMATION | COMPONENT_SHADERID))
		{
			tThisWorld.atAnimation[nCurrentEntity].tTimer.GetLocalTime(tThisWorld.atAnimation[nCurrentEntity].tTimer.tSceneTimer, tThisWorld.atAnimation[nCurrentEntity].tTimer.localTime);

			tAnimVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			tAnimVertexBuffer.m_d3dViewMatrix = debugCamera->d3d_Position;
			tAnimVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;

			bool scale = false;

			if (nCurrentEntity == 904)
			{
				scale = true;
			}

			XMFLOAT4X4 * tweenJoints = pcAnimationSystem->PlayAnimation(tThisWorld.atAnimationVariant[nCurrentEntity], tThisWorld.atAnimation[nCurrentEntity], tThisWorld.atAnimation[nCurrentEntity].tTimer.localTime, scale);

			for (int i = 0; i < MAX_JOINTS; ++i)
			{
				memcpy(&tAnimVertexBuffer.m_d3dJointsForVS[i], &tweenJoints[i], sizeof(tweenJoints[i]));
			}

			/*TLights* Lights_To_Send = pcGraphicsSystem->GetLights();
			for  (int k = 0; k < MAX_LIGHTS; k++)
			{
				memcpy(&tLightBuffer.m_allLights[k], &Lights_To_Send[k], sizeof(Lights_To_Send[k]));
			}*/
			/*TLights testLight;
			testLight.m_d3dLightColor = XMFLOAT4(1, 0, 0, 1);
			testLight.m_d3dLightPosition = XMFLOAT4(-4, -1.0f, 12.8f, 1);
			testLight.enabled = true;
			testLight.m_lightType = 1;
			testLight.m_Direction = XMFLOAT4(0, -2.0f, -5.0f, 1);
			testLight.m_padding = XMFLOAT2(0, 0);
			tLightBuffer.m_allLights = testLight;*/
			
			for ( int currentLightIndex = 0; currentLightIndex < MAX_LIGHTS; currentLightIndex++)
			{
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_d3dLightPosition,&setLights[currentLightIndex].m_d3dLightPosition, sizeof(setLights[currentLightIndex].m_d3dLightPosition));
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_d3dLightColor, &setLights[currentLightIndex].m_d3dLightColor, sizeof(setLights[currentLightIndex].m_d3dLightColor));
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_lightType, &setLights[currentLightIndex].m_lightType, sizeof(setLights[currentLightIndex].m_lightType));
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_Direction, &setLights[currentLightIndex].m_Direction, sizeof(setLights[currentLightIndex].m_Direction));
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].enabled ,&setLights[currentLightIndex].enabled, sizeof(setLights[currentLightIndex].enabled));
				
			}
			tLightBuffer.m_Proprties = tThisWorld.atMaterial[nCurrentEntity];
			
			pcGraphicsSystem->InitAnimShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tAnimVertexBuffer, tThisWorld.atMesh[nCurrentEntity], tLightBuffer , debugCamera->d3d_Position);
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

	//tThisWorld.atAnimation[904].tTimer.GetLocalTime(tThisWorld.atAnimation[904].tTimer.tSceneTimer, tThisWorld.atAnimation[904].tTimer.localTime);
	//tThisWorld.atAnimation[904].tTimer.DisplayTimes(pcInputSystem);

	if (pcInputSystem->InputCheck(G_KEY_I))
	{
		if (tThisWorld.atAnimationVariant[904].tClaytonAnim.animType != 1)
		{
			tThisWorld.atAnimation[904].tTimer.localTime = 0;

			tThisWorld.atAnimationVariant[904].tClaytonAnim.currentFrame = 0;
			tThisWorld.atAnimationVariant[904].tClaytonAnim.nextFrame = 1;
		}

		tThisWorld.atAnimationVariant[904].tClaytonAnim.animType = 1;
	}
	else if(pcInputSystem->InputCheck(G_KEY_K))
	{
		if (tThisWorld.atAnimationVariant[904].tClaytonAnim.animType != 2)
		{
			tThisWorld.atAnimation[904].tTimer.localTime = 0;

			tThisWorld.atAnimationVariant[904].tClaytonAnim.currentFrame = 0;
			tThisWorld.atAnimationVariant[904].tClaytonAnim.nextFrame = 1;
		}

		tThisWorld.atAnimationVariant[904].tClaytonAnim.animType = 2;
	}
	else if (pcInputSystem->InputCheck(G_KEY_J))
	{
		if (tThisWorld.atAnimationVariant[904].tClaytonAnim.animType != 4)
		{
			tThisWorld.atAnimation[904].tTimer.localTime = 0;

			tThisWorld.atAnimationVariant[904].tClaytonAnim.currentFrame = 0;
			tThisWorld.atAnimationVariant[904].tClaytonAnim.nextFrame = 1;
		}

		tThisWorld.atAnimationVariant[904].tClaytonAnim.animType = 4;
	}
	else if (pcInputSystem->InputCheck(G_KEY_L))
	{
		if (tThisWorld.atAnimationVariant[904].tClaytonAnim.animType != 3)
		{
			tThisWorld.atAnimation[904].tTimer.localTime = 0;

			tThisWorld.atAnimationVariant[904].tClaytonAnim.currentFrame = 0;
			tThisWorld.atAnimationVariant[904].tClaytonAnim.nextFrame = 1;
		}

		tThisWorld.atAnimationVariant[904].tClaytonAnim.animType = 3;
	}
	else if(tThisWorld.atAnimationVariant[904].tClaytonAnim.animType != 0)
	{
		tThisWorld.atAnimationVariant[904].tClaytonAnim.animType = 0;

		tThisWorld.atAnimation[904].tTimer.localTime = 0;

		tThisWorld.atAnimationVariant[904].tClaytonAnim.currentFrame = 0;
		tThisWorld.atAnimationVariant[904].tClaytonAnim.nextFrame = 1;
	}

	//if (pcInputSystem->InputCheck(G_KEY_M) == 1 && !buttonPressed)
	//{
	//	if (tThisWorld.atAnimation[904].m_tAnim.m_vtKeyFrames[tThisWorld.atAnimationVariant[904].tClaytonAnim.nextFrame].dTime == 0 && tThisWorld.atAnimation[904].m_tAnim.m_vtKeyFrames[tThisWorld.atAnimationVariant[904].tClaytonAnim.currentFrame].dTime != 0)
	//	{
	//		tThisWorld.atAnimationVariant[904].tClaytonAnim.currentFrame = 0;
	//		tThisWorld.atAnimationVariant[904].tClaytonAnim.nextFrame = 1;
	//
	//		tThisWorld.atAnimation[904].tTimer.localTime = tThisWorld.atAnimation[904].m_tAnim.m_vtKeyFrames[tThisWorld.atAnimationVariant[904].tClaytonAnim.currentFrame].dTime;
	//	}
	//	else
	//	{
	//		tThisWorld.atAnimation[904].tTimer.localTime = tThisWorld.atAnimation[904].m_tAnim.m_vtKeyFrames[tThisWorld.atAnimationVariant[904].tClaytonAnim.nextFrame].dTime - .001;
	//	}
	//
	//	buttonPressed = true;
	//}
	//else if (pcInputSystem->InputCheck(G_KEY_M) == 0 && buttonPressed)
	//{
	//	buttonPressed = false;
	//}

	//End Time
	return 10;
}

void CGameMangerSystem::LoadLevelWithMapInIt()
{
	pcCollisionSystem->m_AAbb.clear();
	pcCollisionSystem->AiFrustumCheck.clear();
	pcAiSystem->CLeanPathPlaning();
	pcAiSystem->ClearShootingActiveAI();
	pcGraphicsSystem->CleanD3DLevel(&tThisWorld);
	//Stops Main Menu Music 
#if MUSIC_ON
	AK::SoundEngine::StopAll();
	if (soundOff == false)
	{
		pcAudioSystem->UnRegisterGameObj(pcAudioSystem->m_AkMainMenuMusic);
		pcAudioSystem->RegisterGameObj(pcAudioSystem->m_AkHallwayBattle);
		pcAudioSystem->RegisterGameObj(pcAudioSystem->m_MetalReload);
		pcAudioSystem->RegisterGameObj(pcAudioSystem->m_AkMetalFired);
		pcAudioSystem->RegisterGameObj(pcAudioSystem->m_Laser_Fire);
		pcAudioSystem->RegisterGameObj(pcAudioSystem->m_Human_Hurt);
		pcAudioSystem->RegisterGameObj(pcAudioSystem->m_Scylian_Hurt);
		pcAudioSystem->RegisterGameObj(pcAudioSystem->m_GunEmpty);
		/*pcAudioSystem->RegisterGameObj(pcAudioSystem->m_WalkSound);*/
		pcAudioSystem->RegisterGameObj(pcAudioSystem->m_Extract);
		pcAudioSystem->RegisterGameObj(pcAudioSystem->m_Heal);
		/*pcAudioSystem->RegisterGameObj(pcAudioSystem->m_Caelis_Walk);
		pcAudioSystem->RegisterGameObj(pcAudioSystem->m_Caelis_Fire);
		pcAudioSystem->RegisterGameObj(pcAudioSystem->m_Caelis_Reload);*/

		soundOff = true;
	}


	pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_HALLWAY_MUSIC, pcAudioSystem->m_AkHallwayBattle);
	pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::MUSIC_VOLUME, pcAudioSystem->m_fMusicVolume);
#endif

	pcGraphicsSystem->SetLights();
	InitializeHUD();
	InitializePauseScreen();
	GameOver = false;
	GamePaused = false;
	endInit = false;
	pauseInit = false;
	mouseDown = false;
	mouseUp = true;
	click = false;
	makeBeamBuffer = true;
	InitializeLoadingScreen();
	LoadLoadingScreen(false);

	pcAiSystem->SetNumberOfAI(NUMBER_OF_AI);
	//	tTimerInfo->StartClock(tAugerTimers->tSceneTimer);
	ImporterData tempImport;
	TMaterialOptimized matOpt;

	ImporterData gunImport;

	CreateWICTextureFromFile(pcGraphicsSystem->m_pd3dDevice, L"Cubemap_SpaceLightBlue/left.png", NULL, &particleSRV, NULL);

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
	//
	bulletMesh = pcGraphicsSystem->ReadMesh("meshData_ProjectileMatGun.txt");
	matOpt = pcGraphicsSystem->CreateTexturesFromFile(bulletMesh.vtMaterials, bulletMesh.meshCount);
	int BulletTexted;
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		BulletTexted = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, bulletMesh.vtMeshes[meshIndex], matOpt, meshIndex);
	}

	int tempBullet;
	for (int meshIndex = 0; meshIndex < bulletMesh.meshCount; ++meshIndex)
	{
		tempBullet = CreateBulletMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, m_d3dWorldMatrix, 0, 0, bulletMesh.vtMeshes[meshIndex], bulletMesh.vtMaterials[meshIndex]);
	}

	bulletToCopyFrom.atMesh = tThisWorld.atMesh[tempBullet];
	bulletToCopyFrom.atShaderID = tThisWorld.atShaderID[tempBullet];
	bulletToCopyFrom.atRigidBody = tThisWorld.atRigidBody[tempBullet];
	bulletToCopyFrom.atGraphicsMask = tThisWorld.atGraphicsMask[tempBullet];
	bulletToCopyFrom.atCollisionMask = tThisWorld.atCollisionMask[tempBullet];
	bulletToCopyFrom.atProjectiles = tThisWorld.atProjectiles[tempBullet];

	pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, tempBullet);
	pcGraphicsSystem->CleanD3DObject(&tThisWorld, tempBullet);

	tempImport = pcGraphicsSystem->ReadMesh("meshData_OctoberDemoUpdated.txt");
	matOpt = pcGraphicsSystem->CreateTexturesFromFile(tempImport.vtMaterials, tempImport.meshCount);
	int myMesh;
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		tempImport.vtMaterials[meshIndex].lambert;
		myMesh = createMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], matOpt, tempImport.vtMaterials[meshIndex],  meshIndex, tempImport.vtMaterials[meshIndex].lambert);
	}

#pragma region Matrix Init
	//Current World Matrix Init
	m_d3dWorldMatrix = pcGraphicsSystem->SetDefaultWorldPosition();//Call some sort of function from the graphics system to create this matrix
																   //Current View matrix Init
	m_d3dViewMatrix = pcGraphicsSystem->SetDefaultViewMatrix();
	//Init Walk Camera to Default Position
	walkCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	walkCamera->fPitch = 0;
	walkCamera->fRoll = 0;
	walkCamera->fYaw = 0;
	//Init Aim Camera to Default Position
	aimCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	aimCamera->fPitch = 0;
	aimCamera->fRoll = 0;
	aimCamera->fYaw = 0;
	//Init Debug Camera to Default Position
	debugCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
	debugCamera->fPitch = 0;
	debugCamera->fRoll = 0;
	debugCamera->fYaw = 0;
	// Init Projection Matrix with 90.0f
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective(m_RealTimeFov);

	//Init Player matrix
	m_d3dPlayerMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	//Set Camera Modes to Default 
	tCameraMode.bWalkMode = false;
	tCameraMode.bDebugMode = false;
	tCameraMode.bAimMode = true;
	tCameraMode.bSwitch = false;

	bNoMoving = false;
	//Init FOV default
	m_RealTimeFov = 90.0f;
	//m_d3dPlayerMatrix = XMMatrixMultiply(m_d3dPlayerMatrix, XMMatrixScaling(.01, .01, .01));



	m_d3dPlayerMatrix.r[3].m128_f32[2] += 20;
	m_d3dPlayerMatrix.r[3].m128_f32[0] -= 5;
	m_d3dPlayerMatrix.r[3].m128_f32[1] -= 1;

#pragma endregion

	//Clayton Import Data - ZFB
	//tempImport = pcGraphicsSystem->ReadMesh("meshData_Pirate.txt");
	tempImport = pcGraphicsSystem->ReadMesh2("meshData_PirateStrafeRight.txt", CLAYTON, nullptr, 3);
	tempImport = pcGraphicsSystem->ReadMesh2("meshData_PirateStrafeLeft.txt", CLAYTON, &tempImport, 3);
	tempImport = pcGraphicsSystem->ReadMesh2("meshData_PirateMoveBackward.txt", CLAYTON, &tempImport, 3);
	tempImport = pcGraphicsSystem->ReadMesh2("meshData_PirateMoveForward.txt", CLAYTON, &tempImport, 3);
	tempImport = pcGraphicsSystem->ReadMesh2("meshData_PirateIdle.txt", CLAYTON, &tempImport, 3);

	matOpt = pcGraphicsSystem->CreateTexturesFromFile(tempImport.vtMaterials, 1);

	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		//ClaytonIndex = createClayton(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
		ClaytonIndex = createClaytonAnim(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], matOpt, tempImport.vtMaterials[meshIndex], tempImport.vtAnimations, meshIndex, tempImport.animationCount);
	}
	PlayerStartIndex = ClaytonIndex;
	//pcInputSystem->m_Companion1 = ClaytonIndex;
	m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix = XMMatrixRotationRollPitchYaw(0, XMConvertToRadians(180), 0);

	m_d3dPlayerMatrix.r[3].m128_f32[0] = tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix.r[3].m128_f32[0] = -4;
	m_d3dPlayerMatrix.r[3].m128_f32[1] = tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix.r[3].m128_f32[1] = -1;
	m_d3dPlayerMatrix.r[3].m128_f32[2] = tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix.r[3].m128_f32[2] = 5.3;

#if CAELIS
	//Put Caelis Import Data here 
	tempImport = pcGraphicsSystem->ReadMesh("meshData_CaelisWingsBack.txt");
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	{
		CaelisIndex = CreateCaelis(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	}
	// CreateHealingAI(&tThisWorld);
	pcInputSystem->m_Companion1 = CaelisIndex;

	m_d3dCaelisMatrix = tThisWorld.atWorldMatrix[CaelisIndex].worldMatrix;
#endif

	m_d3dClaytonMatrix = tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix;

	//PlayerStartIndex = CaelisIndex;

	// Put Seth Import Data here

	std::array<plane_t, 6> cPlanes;
	float4x4 ClaytonFrustum;

	ClaytonFrustum.row1.x = aimCamera->d3d_Position.r[0].m128_f32[0];
	ClaytonFrustum.row1.y = aimCamera->d3d_Position.r[0].m128_f32[1];
	ClaytonFrustum.row1.z = aimCamera->d3d_Position.r[0].m128_f32[2];
	ClaytonFrustum.row1.w = aimCamera->d3d_Position.r[0].m128_f32[3];

	ClaytonFrustum.row2.x = aimCamera->d3d_Position.r[1].m128_f32[0];
	ClaytonFrustum.row2.y = aimCamera->d3d_Position.r[1].m128_f32[1];
	ClaytonFrustum.row2.z = aimCamera->d3d_Position.r[1].m128_f32[2];
	ClaytonFrustum.row2.w = aimCamera->d3d_Position.r[1].m128_f32[3];

	ClaytonFrustum.row3.x = aimCamera->d3d_Position.r[2].m128_f32[0];
	ClaytonFrustum.row3.y = aimCamera->d3d_Position.r[2].m128_f32[1];
	ClaytonFrustum.row3.z = aimCamera->d3d_Position.r[2].m128_f32[2];
	ClaytonFrustum.row3.w = aimCamera->d3d_Position.r[2].m128_f32[3];

	ClaytonFrustum.row4.x = aimCamera->d3d_Position.r[3].m128_f32[0];
	ClaytonFrustum.row4.y = aimCamera->d3d_Position.r[3].m128_f32[1];
	ClaytonFrustum.row4.z = aimCamera->d3d_Position.r[3].m128_f32[2];
	ClaytonFrustum.row4.w = aimCamera->d3d_Position.r[3].m128_f32[3];

	claytonFrustumIndex = pcAiSystem->calculate_frustum(&tThisWorld, cPlanes, ClaytonFrustum, 60, 1, 0.1, 150, PlayerStartIndex, 0, 0, 0);

	tThisWorld.atWorldMatrix[claytonFrustumIndex].worldMatrix = aimCamera->d3d_Position;

	tThisWorld.atClaytonVision.eyes0 = cPlanes;
	tThisWorld.atClaytonVision.normalAtBegining[0] = cPlanes[0].normal;
	tThisWorld.atClaytonVision.normalAtBegining[1] = cPlanes[1].normal;
	tThisWorld.atClaytonVision.normalAtBegining[2] = cPlanes[2].normal;
	tThisWorld.atClaytonVision.normalAtBegining[3] = cPlanes[3].normal;
	tThisWorld.atClaytonVision.normalAtBegining[4] = cPlanes[4].normal;
	tThisWorld.atClaytonVision.normalAtBegining[5] = cPlanes[5].normal;

	gunImport = pcGraphicsSystem->ReadMesh("meshData_MaterialGun.txt");

	for (int meshIndex = 0; meshIndex < gunImport.meshCount; ++meshIndex)
	{
		GunIndexForClayton = CreateClaytonGun(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix, ClaytonIndex, .2, 1.35, .9, 8, 100, gunImport.vtMeshes[meshIndex], gunImport.vtMaterials[meshIndex]);
	}
	GunIndexForPlayer = GunIndexForClayton;

	tThisWorld.atAABB[GunIndexForClayton] = pcCollisionSystem->createAABBS(tThisWorld.atMesh[GunIndexForClayton].m_VertexData, tThisWorld.atAABB[GunIndexForClayton]);
	tThisWorld.atAABB[GunIndexForClayton].m_IndexLocation = GunIndexForClayton;
	pcCollisionSystem->AddAABBCollider(tThisWorld.atAABB[GunIndexForClayton], GunIndexForClayton);

#if CAELIS
	gunImport = pcGraphicsSystem->ReadMesh("meshData_CaelisGun.txt");

	for (int meshIndex = 0; meshIndex < gunImport.meshCount; ++meshIndex)
	{
		GunIndexForCaelis = CreateCaelisGun(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, m_d3dWorldMatrix, CaelisIndex, -.7, 1, 10.4, 10, 30, gunImport.vtMeshes[meshIndex], gunImport.vtMaterials[meshIndex]);
	}

	tThisWorld.atAABB[GunIndexForCaelis] = pcCollisionSystem->createAABBS(tThisWorld.atMesh[GunIndexForCaelis].m_VertexData, tThisWorld.atAABB[GunIndexForCaelis]);
	tThisWorld.atAABB[GunIndexForCaelis].m_IndexLocation = GunIndexForCaelis;
	pcCollisionSystem->AddAABBCollider(tThisWorld.atAABB[GunIndexForCaelis], GunIndexForCaelis);
#endif

	//for (int meshIndex = 0; meshIndex < tempImport.meshCount; meshIndex++)
	//{
	//	caelisIndex = createClayton(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex]);
	//}
	//tThisWorld.atAABB[caelisIndex] = pcCollisionSystem->createAABBS(tThisWorld.atMesh[caelisIndex].m_VertexData, tThisWorld.atAABB[caelisIndex]);
	//tThisWorld.atAABB[caelisIndex].m_IndexLocation = caelisIndex;
	//pcCollisionSystem->AddAABBCollider(tThisWorld.atAABB[caelisIndex], caelisIndex);
	//
	//tThisWorld.atWorldMatrix[caelisIndex].worldMatrix = XMMatrixIdentity();
	//tThisWorld.atWorldMatrix[caelisIndex].worldMatrix.r[3].m128_f32[0] = 0;
	//tThisWorld.atWorldMatrix[caelisIndex].worldMatrix.r[3].m128_f32[1] = -1.2;
	//tThisWorld.atWorldMatrix[caelisIndex].worldMatrix.r[3].m128_f32[2] = 10.8;
	//
	//tThisWorld.atRigidBody[caelisIndex].gravity.m128_f32[1] = 0;
	//
	//tThisWorld.atInputMask[caelisIndex].m_tnInputMask = (COMPONENT_INPUTMASK | COMPONENT_CAELIS);

#pragma region AI and Gun INIT
//GunIndexForPlayer = CreateGun(&tThisWorld, m_d3dWorldMatrix, PlayerStartIndex, -1, 1, 10.5, 3, 130);
	tThisWorld.atClip[GunIndexForPlayer].bulletSpeed = 10;
	tThisWorld.atClayton[PlayerStartIndex].health = 100;


	//tThisWorld.atWorldMatrix[94].worldMatrix.r[3].m128_f32[2] += 10;



	XMMATRIX AILocation = pcGraphicsSystem->SetDefaultWorldPosition();
	AILocation.r[3].m128_f32[2] -= 60;
	AILocation.r[3].m128_f32[0] -= 8;
	AILocation.r[3].m128_f32[1] -= 1;
	XMMATRIX CoverLocation = AILocation;
	vector<int> coverPosition;
	CoverLocation.r[3].m128_f32[2] += 4;
	int cover1 = CreateCover(&tThisWorld, CoverLocation, coverPosition);
	XMMATRIX nodeLocation = CoverLocation;
	nodeLocation.r[3].m128_f32[0] += 0;
	nodeLocation.r[3].m128_f32[1] -= 0;
	nodeLocation.r[3].m128_f32[2] += -2;

	//pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	int nodeindex = CreateNodePoint(&tThisWorld, nodeLocation);
	XMFLOAT3 nodePosition;
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex, nodePosition, 1);
	nodeLocation = AILocation;
	int nodeindex2 = CreateNodePoint(&tThisWorld, nodeLocation);

	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex2, nodePosition, 1);
	tThisWorld.atCover[cover1].CoverPositions.push_back(nodeindex2);
	pcAiSystem->LookAtObject(AILocation, &m_d3dPlayerMatrix);
	XMMATRIX TriggerZone = m_d3dPlayerMatrix;
	TriggerZone.r[3].m128_f32[2] -= 40;
	TriggerZone.r[3].m128_f32[0] -= 6;
	int CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone);

	vector<int> edges;
	edges.push_back(nodeindex);
	pcAiSystem->AddEdgestoNode(nodeindex2, edges);
	//pcAiSystem->AddEdgestoNode(nodeindex3, edges);
	edges.clear();
	edges.push_back(nodeindex2);
	pcAiSystem->AddEdgestoNode(nodeindex, edges);
	edges.clear();
#pragma endregion

	tempImport = pcGraphicsSystem->ReadMesh2("meshData_SnakeDeath1.txt", SCYLLIAN);
	tempImport = pcGraphicsSystem->ReadMesh2("meshData_SnakeGetHit.txt", SCYLLIAN, &tempImport);
	tempImport = pcGraphicsSystem->ReadMesh2("meshData_SnakeAttack1.txt", SCYLLIAN, &tempImport);
	tempImport = pcGraphicsSystem->ReadMesh2("meshData_SnakeIdle.txt", SCYLLIAN, &tempImport);

	matOpt = pcGraphicsSystem->CreateTexturesFromFile(tempImport.vtMaterials, 1);

	int spacePirate;
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; ++meshIndex)
	{
		//spacePirate = CreateScyllian(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex], AILocation);
		spacePirate = CreateScyllianAnim(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], matOpt, AILocation, tempImport.vtAnimations, tempImport.vtMaterials[meshIndex], meshIndex, tempImport.animationCount);
	}
	pcAiSystem->allAI.push_back(spacePirate);
	enemyToCopyFrom.atMesh = tThisWorld.atMesh[spacePirate];
	enemyToCopyFrom.atRigidBody = tThisWorld.atRigidBody[spacePirate];
	enemyToCopyFrom.atShaderID = tThisWorld.atShaderID[spacePirate];
	enemyToCopyFrom.atGraphicsMask = tThisWorld.atGraphicsMask[spacePirate];
	enemyToCopyFrom.atCollisionMask = tThisWorld.atCollisionMask[spacePirate];
	enemyToCopyFrom.atPhysicsMask = tThisWorld.atPhysicsMask[spacePirate];
	enemyToCopyFrom.atAIMask = tThisWorld.atAIMask[spacePirate];

	enemyToCopyFrom.atAnimation = tThisWorld.atAnimation[spacePirate];
	enemyToCopyFrom.atAnimationVariant = tThisWorld.atAnimationVariant[spacePirate];

	tThisWorld.atOffSetMatrix[spacePirate] = AILocation;

	//spacePirate = CreateSpacePirate(&tThisWorld, AILocation);
	tThisWorld.atAiHeath[spacePirate].heath = 100;
	createGSQuad(&tThisWorld, XMFLOAT4(1, 0, 0, 1), spacePirate);
	createGSQuad(&tThisWorld, XMFLOAT4(0, 0, 0, 1), spacePirate);


	gunImport = pcGraphicsSystem->ReadMesh("meshData_LaserFlintlockTextured.txt");

	int GunINdexai;
	//int GunINdexai = CreateGun(&tThisWorld, m_d3dWorldMatrix, spacePirate, -1.1, 0.5, 12.5, 10, 30);
	for (int meshIndex = 0; meshIndex < gunImport.meshCount; ++meshIndex)
	{
		GunINdexai = CreateScyllianGun(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, m_d3dWorldMatrix, spacePirate, -1, 1, 11.5, 10, 200, gunImport.vtMeshes[meshIndex], gunImport.vtMaterials[meshIndex]);
	}

#pragma region More AI Init
	tThisWorld.atAIMask[spacePirate].GunIndex = GunINdexai;
	tThisWorld.atClip[GunINdexai].gunHolder = spacePirate;


	tThisWorld.atClip[GunINdexai].bulletSpeed = 0.1;//Frame Dependent
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
	tThisWorld.atPathPlanining[spacePirate].startingNode = nodeindex2;
	tThisWorld.atPathPlanining[spacePirate].Goal = nodeindex2;



	tThisWorld.atCoverTrigger[CoverTriggerIndex].AItoMove.push_back(spacePirate);

	tThisWorld.atCoverTrigger[CoverTriggerIndex].coverAiCanGoTo.push_back(tThisWorld.atCover[cover1]);





	AILocation = pcGraphicsSystem->SetDefaultWorldPosition();
	AILocation.r[3].m128_f32[2] -= 60;
	AILocation.r[3].m128_f32[0] -= 4;
	AILocation.r[3].m128_f32[1] -= 1;
	CoverLocation = AILocation;
	coverPosition.clear();
	CoverLocation.r[3].m128_f32[2] += 4;
	cover1 = CreateCover(&tThisWorld, CoverLocation, coverPosition);
	nodeLocation = CoverLocation;
	nodeLocation.r[3].m128_f32[0] += 0;
	nodeLocation.r[3].m128_f32[1] -= 0;
	nodeLocation.r[3].m128_f32[2] += -2;

	//pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	nodeindex = CreateNodePoint(&tThisWorld, nodeLocation);
	nodePosition;
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex, nodePosition, 1);
	nodeLocation = AILocation;
	nodeindex2 = CreateNodePoint(&tThisWorld, nodeLocation);

	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex2, nodePosition, 1);
	tThisWorld.atCover[cover1].CoverPositions.push_back(nodeindex2);
	pcAiSystem->LookAtObject(AILocation, &m_d3dPlayerMatrix);
	TriggerZone = m_d3dPlayerMatrix;
	TriggerZone.r[3].m128_f32[2] -= 40;
	TriggerZone.r[3].m128_f32[0] -= 2;

	// CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone);

	edges.clear();
	edges.push_back(nodeindex);
	pcAiSystem->AddEdgestoNode(nodeindex2, edges);
	//pcAiSystem->AddEdgestoNode(nodeindex3, edges);
	edges.clear();
	edges.push_back(nodeindex2);
	pcAiSystem->AddEdgestoNode(nodeindex, edges);
	edges.clear();
#pragma endregion

	tempImport = pcGraphicsSystem->ReadMesh("meshData_Scyllian.txt");
	//gunImport = pcGraphicsSystem->ReadMesh("meshData_LaserFlintlockTextured.txt");
	int previousai = spacePirate;
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; ++meshIndex)
	{
		spacePirate = CreateScyllian(&tThisWorld, AILocation, enemyToCopyFrom);
		//spacePirate = CreateScyllian(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex], AILocation);
	}
	pcAiSystem->allAI.push_back(spacePirate);
	tThisWorld.atActiveAI[previousai].NoctifyOtherAi.push_back(spacePirate);
	tThisWorld.atActiveAI[spacePirate].NoctifyOtherAi.push_back(previousai);

	tThisWorld.atOffSetMatrix[spacePirate] = AILocation;

	//spacePirate = CreateSpacePirate(&tThisWorld, AILocation);
	tThisWorld.atAiHeath[spacePirate].heath = 100;
	createGSQuad(&tThisWorld, XMFLOAT4(1, 0, 0, 1), spacePirate);
	createGSQuad(&tThisWorld, XMFLOAT4(0, 0, 0, 1), spacePirate);
	XMMATRIX VisionTriggerMatrix= m_d3dClaytonMatrix;
	VisionTriggerMatrix .r[3].m128_f32[0]+= 7.3;
	VisionTriggerMatrix.r[3].m128_f32[2] -= 11;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);
	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);
	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);
	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);
	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);
	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);
	VisionTriggerMatrix.r[3].m128_f32[2] += 6;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);
	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);
	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);


	// right side of first door
	VisionTriggerMatrix = m_d3dClaytonMatrix;
	VisionTriggerMatrix.r[3].m128_f32[0] += -11;
	VisionTriggerMatrix.r[3].m128_f32[2] -= 11;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);
	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);
	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);
	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);
	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);
	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);
	VisionTriggerMatrix.r[3].m128_f32[2] += 6;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);
	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);
	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] += 1;

	CreateTriggerForAiVision(&tThisWorld, VisionTriggerMatrix);

	VisionTriggerMatrix = m_d3dClaytonMatrix;
	VisionTriggerMatrix.r[3].m128_f32[0] += 10;
	VisionTriggerMatrix.r[3].m128_f32[2] -= 60;




	//int GunINdexai = CreateGun(&tThisWorld, m_d3dWorldMatrix, spacePirate, -1.1, 0.5, 12.5, 10, 30);
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; ++meshIndex)
	{
		GunINdexai = CreateScyllianGun(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, m_d3dWorldMatrix, spacePirate, -1, 1.2, 11.5, 10, 100, gunImport.vtMeshes[meshIndex], gunImport.vtMaterials[meshIndex]);
	}
#pragma region MORE AI Init
	tThisWorld.atAIMask[spacePirate].GunIndex = GunINdexai;

	tThisWorld.atClip[GunINdexai].bulletSpeed = 0.1;//Frame Dependent
	tThisWorld.atClip[GunINdexai].gunHolder = spacePirate;


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
	tThisWorld.atPathPlanining[spacePirate].startingNode = nodeindex2;
	tThisWorld.atPathPlanining[spacePirate].Goal = nodeindex2;



	tThisWorld.atCoverTrigger[CoverTriggerIndex].AItoMove.push_back(spacePirate);

	tThisWorld.atCoverTrigger[CoverTriggerIndex].coverAiCanGoTo.push_back(tThisWorld.atCover[cover1]);





	// ai in the back left
	

	AILocation = pcGraphicsSystem->SetDefaultWorldPosition();
	AILocation.r[3].m128_f32[2] -= 80;
	AILocation.r[3].m128_f32[0] += 4;
	AILocation.r[3].m128_f32[1] -= 1;
	XMMATRIX AiLookPosition = AILocation;
	AiLookPosition.r[3].m128_f32[0] -= 12;
	AiLookPosition.r[3].m128_f32[2] += 10;

	CreateNodePoint(&tThisWorld, AiLookPosition);
	CoverLocation = AILocation;
	coverPosition.clear();
	CoverLocation.r[3].m128_f32[2] += 4;
	cover1 = CreateCover(&tThisWorld, CoverLocation, coverPosition);
	nodeLocation = CoverLocation;
	nodeLocation.r[3].m128_f32[0] += 0;
	nodeLocation.r[3].m128_f32[1] -= 0;
	nodeLocation.r[3].m128_f32[2] += -2;

	//pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	nodeindex = CreateNodePoint(&tThisWorld, nodeLocation);
	nodePosition;
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex, nodePosition, 1);
	nodeLocation = AILocation;
	nodeindex2 = CreateNodePoint(&tThisWorld, nodeLocation);

	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex2, nodePosition, 1);
	tThisWorld.atCover[cover1].CoverPositions.push_back(nodeindex);
	pcAiSystem->LookAtObject(AILocation, &m_d3dPlayerMatrix);
	TriggerZone = m_d3dPlayerMatrix;
	TriggerZone.r[3].m128_f32[2] -= 80;
	TriggerZone.r[3].m128_f32[0] -= 2;

	//CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone);

	edges.clear();
	edges.push_back(nodeindex);
	pcAiSystem->AddEdgestoNode(nodeindex2, edges);
	//pcAiSystem->AddEdgestoNode(nodeindex3, edges);
	edges.clear();
	edges.push_back(nodeindex2);
	pcAiSystem->AddEdgestoNode(nodeindex, edges);
	edges.clear();
#pragma endregion

	tempImport = pcGraphicsSystem->ReadMesh("meshData_Scyllian.txt");
	//gunImport = pcGraphicsSystem->ReadMesh("meshData_LaserFlintlockTextured.txt");
	//AILocation.r[3].m128_f32[0] += 7;

	for (int meshIndex = 0; meshIndex < gunImport.meshCount; ++meshIndex)
	{
		spacePirate = CreateScyllian(&tThisWorld, AILocation, enemyToCopyFrom);
		createGSQuad(&tThisWorld, XMFLOAT4(1, 0, 0, 1), spacePirate);
		createGSQuad(&tThisWorld, XMFLOAT4(0, 0, 0, 1), spacePirate);
		int CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		//spacePirate = CreateScyllian(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex], AILocation);
	}
	pcAiSystem->allAI.push_back(spacePirate);

	tThisWorld.atOffSetMatrix[spacePirate] = AILocation;

	pcAiSystem->LookAtObject(AiLookPosition, &tThisWorld.atWorldMatrix[spacePirate].worldMatrix);
	//spacePirate = CreateSpacePirate(&tThisWorld, AILocation);
	tThisWorld.atAiHeath[spacePirate].heath = 100;
	







	//int GunINdexai = CreateGun(&tThisWorld, m_d3dWorldMatrix, spacePirate, -1.1, 0.5, 12.5, 10, 30);
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; ++meshIndex)
	{
		GunINdexai = CreateScyllianGun(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, m_d3dWorldMatrix, spacePirate, -1, 1, 11.5, 10, 200, gunImport.vtMeshes[meshIndex], gunImport.vtMaterials[meshIndex]);
	}
#pragma region MORE AI Init
	tThisWorld.atAIMask[spacePirate].GunIndex = GunINdexai;
	tThisWorld.atClip[GunINdexai].gunHolder = spacePirate;


	tThisWorld.atClip[GunINdexai].bulletSpeed = 0.1;//Frame Dependent


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
	//AILocation.r[3].m128_f32[0] += 8;
	tThisWorld.atPathPlanining[spacePirate].startingNode = nodeindex2;


	tThisWorld.atAIVision[spacePirate].keepSearching = false;
	TriggerZone = m_d3dPlayerMatrix;
	TriggerZone.r[3].m128_f32[2] -= 90;
	TriggerZone.r[3].m128_f32[0] -= 2;

	//	CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone);

	CoverLocation = AILocation;
	coverPosition.clear();
	CoverLocation.r[3].m128_f32[2] -= 4;
	cover1 = CreateCover(&tThisWorld, CoverLocation, coverPosition);
	nodeLocation = CoverLocation;
	nodeLocation.r[3].m128_f32[0] += 0;
	nodeLocation.r[3].m128_f32[1] -= 0;
	nodeLocation.r[3].m128_f32[2] += -2;

	//pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	int nodeindexBackleft = CreateNodePoint(&tThisWorld, nodeLocation);
	nodePosition;
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindexBackleft, nodePosition, 1);
	nodeLocation = AILocation;
	int backleftFrontnodeindex2 = CreateNodePoint(&tThisWorld, nodeLocation);

	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(backleftFrontnodeindex2, nodePosition, 1);
	tThisWorld.atCover[cover1].CoverPositions.push_back(backleftFrontnodeindex2);
	//	pcAiSystem->LookAtObject(AILocation, &m_d3dPlayerMatrix);


	//CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone);

	edges.clear();
	edges.push_back(nodeindexBackleft);
	edges.push_back(backleftFrontnodeindex2);

	pcAiSystem->AddEdgestoNode(nodeindex2, edges);
	//pcAiSystem->AddEdgestoNode(nodeindex3, edges);
	edges.clear();
	edges.push_back(nodeindexBackleft);
	edges.push_back(backleftFrontnodeindex2);
	pcAiSystem->AddEdgestoNode(nodeindex, edges);
	edges.clear();
	edges.push_back(nodeindexBackleft);
	pcAiSystem->AddEdgestoNode(backleftFrontnodeindex2, edges);

	//tThisWorld.atCoverTrigger[CoverTriggerIndex].AItoMove.push_back(spacePirate);

	//tThisWorld.atCoverTrigger[CoverTriggerIndex].coverAiCanGoTo.push_back(tThisWorld.atCover[cover1]);
	tThisWorld.atPathPlanining[spacePirate].Goal = nodeindexBackleft;
	//tThisWorld.atCoverTrigger[CoverTriggerIndex].AItoMove.push_back(spacePirate);

	//tThisWorld.atCoverTrigger[CoverTriggerIndex].coverAiCanGoTo.push_back(tThisWorld.atCover[cover1]);
#pragma endregion

	// ai in back right


	AILocation = pcGraphicsSystem->SetDefaultWorldPosition();
	AILocation.r[3].m128_f32[2] -= 80;
	AILocation.r[3].m128_f32[0] -= 16;
	AILocation.r[3].m128_f32[1] -= 1;
	AiLookPosition = AILocation;
	AiLookPosition.r[3].m128_f32[0] += 12;
	AiLookPosition.r[3].m128_f32[2] += 10;

	CreateNodePoint(&tThisWorld, AiLookPosition);
	CoverLocation = AILocation;
	coverPosition.clear();
	CoverLocation.r[3].m128_f32[2] += 4;
	cover1 = CreateCover(&tThisWorld, CoverLocation, coverPosition);
	nodeLocation = CoverLocation;
	nodeLocation.r[3].m128_f32[0] += 0;
	nodeLocation.r[3].m128_f32[1] -= 0;
	nodeLocation.r[3].m128_f32[2] += -2;

	//pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	nodeindex = CreateNodePoint(&tThisWorld, nodeLocation);
	nodePosition;
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex, nodePosition, 1);
	nodeLocation = AILocation;
	nodeindex2 = CreateNodePoint(&tThisWorld, nodeLocation);

	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex2, nodePosition, 1);
	tThisWorld.atCover[cover1].CoverPositions.push_back(nodeindex);
	//pcAiSystem->LookAtObject(AILocation, &m_d3dPlayerMatrix);


	//CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone);

	edges.clear();
	edges.push_back(nodeindex);
	pcAiSystem->AddEdgestoNode(nodeindex2, edges);
	//pcAiSystem->AddEdgestoNode(nodeindex3, edges);
	edges.clear();
	edges.push_back(nodeindex2);
	pcAiSystem->AddEdgestoNode(nodeindex, edges);
	edges.clear();
#pragma endregion

	tempImport = pcGraphicsSystem->ReadMesh("meshData_Scyllian.txt");
	//gunImport = pcGraphicsSystem->ReadMesh("meshData_LaserFlintlockTextured.txt");
	//AILocation.r[3].m128_f32[0] += 7;
	VisionTriggerMatrix = m_d3dClaytonMatrix;
	VisionTriggerMatrix.r[3].m128_f32[0] += -9;
	VisionTriggerMatrix.r[3].m128_f32[2] -= 60;
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; ++meshIndex)
	{
		spacePirate = CreateScyllian(&tThisWorld, AILocation, enemyToCopyFrom);
		createGSQuad(&tThisWorld, XMFLOAT4(1, 0, 0, 1), spacePirate);
		createGSQuad(&tThisWorld, XMFLOAT4(0, 0, 0, 1), spacePirate);
		int CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		CollisionofVisionIndex = CreateTriggerForAiVisionWithListofEnemeisthatcantsee(&tThisWorld, VisionTriggerMatrix);	VisionTriggerMatrix.r[3].m128_f32[2] -= 1;
		tThisWorld.atAIVision[CollisionofVisionIndex].AIToTurnVisionOf.push_back(spacePirate);
		//spacePirate = CreateScyllian(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex], AILocation);
	}
	pcAiSystem->allAI.push_back(spacePirate);

	tThisWorld.atOffSetMatrix[spacePirate] = AILocation;

	pcAiSystem->LookAtObject(AiLookPosition, &tThisWorld.atWorldMatrix[spacePirate].worldMatrix);
	//spacePirate = CreateSpacePirate(&tThisWorld, AILocation);
	tThisWorld.atAiHeath[spacePirate].heath = 100;






	//int GunINdexai = CreateGun(&tThisWorld, m_d3dWorldMatrix, spacePirate, -1.1, 0.5, 12.5, 10, 30);
	for (int meshIndex = 0; meshIndex < tempImport.meshCount; ++meshIndex)
	{
		GunINdexai = CreateScyllianGun(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, m_d3dWorldMatrix, spacePirate, -1, 1.5, 12, 10, 200, gunImport.vtMeshes[meshIndex], gunImport.vtMaterials[meshIndex]);
	}
#pragma region MORE AI Init
	tThisWorld.atAIMask[spacePirate].GunIndex = GunINdexai;

	tThisWorld.atClip[GunINdexai].bulletSpeed = 0.1;//Frame Dependent
	tThisWorld.atClip[GunINdexai].gunHolder = spacePirate;



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
	//AILocation.r[3].m128_f32[0] += 8;
	tThisWorld.atPathPlanining[spacePirate].startingNode = nodeindex2;
	tThisWorld.atPathPlanining[spacePirate].Goal = nodeindex;

	tThisWorld.atAIVision[spacePirate].keepSearching = false;
	//	tThisWorld.atCoverTrigger[CoverTriggerIndex].AItoMove.push_back(spacePirate);
	//
	//	tThisWorld.atCoverTrigger[CoverTriggerIndex].coverAiCanGoTo.push_back(tThisWorld.atCover[cover1]);
	//#pragma endregion
		// This is my AIMovementTrigger for backleft ai and back right
	TriggerZone = m_d3dPlayerMatrix;
	TriggerZone.r[3].m128_f32[2] -= 90;
	TriggerZone.r[3].m128_f32[0] -= 2;

	//CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone);

	CoverLocation = AILocation;
	coverPosition.clear();
	CoverLocation.r[3].m128_f32[2] -= 4;
	cover1 = CreateCover(&tThisWorld, CoverLocation, coverPosition);
	nodeLocation = CoverLocation;
	nodeLocation.r[3].m128_f32[0] += 0;
	nodeLocation.r[3].m128_f32[1] -= 0;
	nodeLocation.r[3].m128_f32[2] += -2;

	//pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	int nodeindexBackRight = CreateNodePoint(&tThisWorld, nodeLocation);
	nodePosition;
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindexBackRight, nodePosition, 1);
	nodeLocation = AILocation;
	int backrightFrontnodeindex2 = CreateNodePoint(&tThisWorld, nodeLocation);

	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(backrightFrontnodeindex2, nodePosition, 1);
	tThisWorld.atCover[cover1].CoverPositions.push_back(nodeindexBackRight);
	//	pcAiSystem->LookAtObject(AILocation, &m_d3dPlayerMatrix);


		//CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone);

	edges.clear();
	edges.push_back(nodeindexBackRight);
	edges.push_back(backrightFrontnodeindex2);

	pcAiSystem->AddEdgestoNode(nodeindex2, edges);
	//pcAiSystem->AddEdgestoNode(nodeindex3, edges);
	edges.clear();
	edges.push_back(nodeindexBackRight);
	edges.push_back(backrightFrontnodeindex2);
	pcAiSystem->AddEdgestoNode(nodeindex, edges);
	edges.clear();
	edges.push_back(nodeindexBackRight);
	pcAiSystem->AddEdgestoNode(backrightFrontnodeindex2, edges);

	/*tThisWorld.atCoverTrigger[CoverTriggerIndex].AItoMove.push_back(spacePirate);

	tThisWorld.atCoverTrigger[CoverTriggerIndex].coverAiCanGoTo.push_back(tThisWorld.atCover[cover1]);*/
	tThisWorld.atPathPlanining[spacePirate].Goal = nodeindexBackRight;

	// ai next to tree
	AILocation = pcGraphicsSystem->SetDefaultWorldPosition();
	AILocation.r[3].m128_f32[2] -= 100;
	AILocation.r[3].m128_f32[0] -= 10;
	AILocation.r[3].m128_f32[1] -= 1;
	AiLookPosition = AILocation;
	AiLookPosition.r[3].m128_f32[0] += 12;
	AiLookPosition.r[3].m128_f32[2] += 10;
	//pcAiSystem->LookAtObject(AiLookPosition, &tThisWorld.atWorldMatrix[spacePirate].worldMatrix);


	CreateNodePoint(&tThisWorld, AiLookPosition);
	CoverLocation = AILocation;
	coverPosition.clear();
	CoverLocation.r[3].m128_f32[2] += 4;
	cover1 = CreateCover(&tThisWorld, CoverLocation, coverPosition);
	nodeLocation = CoverLocation;
	nodeLocation.r[3].m128_f32[0] += 0;
	nodeLocation.r[3].m128_f32[1] -= 0;
	nodeLocation.r[3].m128_f32[2] += -2;

	//pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	nodeindex = CreateNodePoint(&tThisWorld, nodeLocation);
	nodePosition;
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex, nodePosition, 1);
	nodeLocation = AILocation;
	nodeindex2 = CreateNodePoint(&tThisWorld, nodeLocation);

	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindex2, nodePosition, 1);
	tThisWorld.atCover[cover1].CoverPositions.push_back(nodeindex);
	//pcAiSystem->LookAtObject(AILocation, &m_d3dPlayerMatrix);


	//CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone);

	edges.clear();
	edges.push_back(nodeindex);
	pcAiSystem->AddEdgestoNode(nodeindex2, edges);
	//pcAiSystem->AddEdgestoNode(nodeindex3, edges);
	edges.clear();
	edges.push_back(nodeindex2);
	pcAiSystem->AddEdgestoNode(nodeindex, edges);
	edges.clear();
#pragma endregion

	tempImport = pcGraphicsSystem->ReadMesh("meshData_Scyllian.txt");
	//gunImport = pcGraphicsSystem->ReadMesh("meshData_LaserFlintlockTextured.txt");
	//AILocation.r[3].m128_f32[0] += 7;

	for (int meshIndex = 0; meshIndex < tempImport.meshCount; ++meshIndex)
	{
		spacePirate = CreateScyllian(&tThisWorld, AILocation, enemyToCopyFrom);
		//spacePirate = CreateScyllian(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex], AILocation);
	}
	pcAiSystem->allAI.push_back(spacePirate);

	tThisWorld.atOffSetMatrix[spacePirate] = AILocation;

	pcAiSystem->LookAtObject(AiLookPosition, &tThisWorld.atWorldMatrix[spacePirate].worldMatrix);
	//spacePirate = CreateSpacePirate(&tThisWorld, AILocation);
	tThisWorld.atAiHeath[spacePirate].heath = 100;
	createGSQuad(&tThisWorld, XMFLOAT4(1, 0, 0, 1), spacePirate);
	createGSQuad(&tThisWorld, XMFLOAT4(0, 0, 0, 1), spacePirate);





	//int GunINdexai = CreateGun(&tThisWorld, m_d3dWorldMatrix, spacePirate, -1.1, 0.5, 12.5, 10, 30);
	for (int meshIndex = 0; meshIndex < gunImport.meshCount; ++meshIndex)
	{
		GunINdexai = CreateScyllianGun(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, m_d3dWorldMatrix, spacePirate, -1.5, 1, 11.5, 10, 200, gunImport.vtMeshes[meshIndex], gunImport.vtMaterials[meshIndex]);
	}
#pragma region MORE AI Init
	tThisWorld.atAIMask[spacePirate].GunIndex = GunINdexai;

	tThisWorld.atClip[GunINdexai].bulletSpeed = 0.1;//Frame Dependent
	tThisWorld.atClip[GunINdexai].gunHolder = spacePirate;

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
	//AILocation.r[3].m128_f32[0] += 8;
	tThisWorld.atPathPlanining[spacePirate].startingNode = nodeindex2;
	tThisWorld.atPathPlanining[spacePirate].Goal = nodeindex;

	tThisWorld.atAIVision[spacePirate].keepSearching = false;
	//	tThisWorld.atCoverTrigger[CoverTriggerIndex].AItoMove.push_back(spacePirate);
	//
	//	tThisWorld.atCoverTrigger[CoverTriggerIndex].coverAiCanGoTo.push_back(tThisWorld.atCover[cover1]);
	//#pragma endregion
	// This is my AIMovementTrigger for backleft ai and back right
	TriggerZone = m_d3dPlayerMatrix;
	TriggerZone.r[3].m128_f32[2] -= 90;
	TriggerZone.r[3].m128_f32[0] -= 2;

	//	CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone);

	CoverLocation = AILocation;
	coverPosition.clear();
	CoverLocation.r[3].m128_f32[2] -= 4;
	cover1 = CreateCover(&tThisWorld, CoverLocation, coverPosition);
	nodeLocation = CoverLocation;
	nodeLocation.r[3].m128_f32[0] += 3.5f;
	nodeLocation.r[3].m128_f32[1] -= 0;
	nodeLocation.r[3].m128_f32[2] += 5;

	//pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	nodeindexBackRight = CreateNodePoint(&tThisWorld, nodeLocation);
	nodePosition;
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindexBackRight, nodePosition, 1);
	nodeLocation = AILocation;
	backrightFrontnodeindex2 = CreateNodePoint(&tThisWorld, nodeLocation);

	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(backrightFrontnodeindex2, nodePosition, 1);
	tThisWorld.atCover[cover1].CoverPositions.push_back(nodeindexBackRight);



	TriggerZone = m_d3dPlayerMatrix;
	TriggerZone.r[3].m128_f32[2] -= 110;
	TriggerZone.r[3].m128_f32[0] += 6;

	//CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone,2);



	CoverLocation = AILocation;
	coverPosition.clear();
	CoverLocation.r[3].m128_f32[2] -= 4;
	cover1 = CreateCover(&tThisWorld, CoverLocation, coverPosition);
	nodeLocation = CoverLocation;
	nodeLocation.r[3].m128_f32[0] += 3.5f;
	nodeLocation.r[3].m128_f32[1] -= 0;
	nodeLocation.r[3].m128_f32[2] += 5;

	//pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	nodeindexBackRight = CreateNodePoint(&tThisWorld, nodeLocation);
	nodePosition;
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(nodeindexBackRight, nodePosition, 1);
	nodeLocation = AILocation;
	backrightFrontnodeindex2 = CreateNodePoint(&tThisWorld, nodeLocation);

	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(backrightFrontnodeindex2, nodePosition, 1);
	tThisWorld.atCover[cover1].CoverPositions.push_back(nodeindexBackRight);
	//	pcAiSystem->LookAtObject(AILocation, &m_d3dPlayerMatrix);


	//CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone);

	edges.clear();
	edges.push_back(nodeindexBackRight);
	edges.push_back(backrightFrontnodeindex2);

	pcAiSystem->AddEdgestoNode(nodeindex2, edges);
	//pcAiSystem->AddEdgestoNode(nodeindex3, edges);
	edges.clear();
	edges.push_back(nodeindexBackRight);
	edges.push_back(backrightFrontnodeindex2);
	pcAiSystem->AddEdgestoNode(nodeindex, edges);
	edges.clear();
	edges.push_back(nodeindexBackRight);
	pcAiSystem->AddEdgestoNode(backrightFrontnodeindex2, edges);
	edges.clear();
	edges.push_back(backrightFrontnodeindex2);
	pcAiSystem->AddEdgestoNode(nodeindexBackRight, edges);


	//tThisWorld.atCoverTrigger[CoverTriggerIndex].AItoMove.push_back(spacePirate);

	//tThisWorld.atCoverTrigger[CoverTriggerIndex].coverAiCanGoTo.push_back(tThisWorld.atCover[cover1]);
	tThisWorld.atPathPlanining[spacePirate].Goal = nodeindexBackRight;


	TriggerZone = m_d3dPlayerMatrix;
	TriggerZone.r[3].m128_f32[2] -= 100;
	TriggerZone.r[3].m128_f32[0] += 3;

	//int coverIndexHorzontal = CreateCoverTriggerZone(&tThisWorld, TriggerZone);


	CoverLocation = AILocation;
	coverPosition.clear();
	CoverLocation.r[3].m128_f32[2] -= 4;
	cover1 = CreateCover(&tThisWorld, CoverLocation, coverPosition);
	nodeLocation = CoverLocation;
	nodeLocation.r[3].m128_f32[0] += 6;
	nodeLocation.r[3].m128_f32[1] -= 0;
	nodeLocation.r[3].m128_f32[2] += 5;

	//pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	int noderighttree = CreateNodePoint(&tThisWorld, nodeLocation);
	nodePosition;
	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(noderighttree, nodePosition, 1);
	nodeLocation = AILocation;
	int noderighttree2 = CreateNodePoint(&tThisWorld, nodeLocation);

	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	pcAiSystem->AddNodeToPathFinding(noderighttree2, nodePosition, 1);
	tThisWorld.atCover[cover1].CoverPositions.push_back(noderighttree);

	edges.clear();
	edges.push_back(noderighttree);
	edges.push_back(noderighttree2);

	pcAiSystem->AddEdgestoNode(backrightFrontnodeindex2, edges);
	edges.clear();
	edges.push_back(noderighttree);
	edges.push_back(noderighttree2);
	pcAiSystem->AddEdgestoNode(nodeindexBackRight, edges);
	edges.clear();
	edges.push_back(backrightFrontnodeindex2);
	edges.push_back(nodeindexBackRight);
	pcAiSystem->AddEdgestoNode(noderighttree2, edges);
	edges.clear();
	edges.push_back(backrightFrontnodeindex2);
	edges.push_back(nodeindexBackRight);
	pcAiSystem->AddEdgestoNode(noderighttree, edges);

	/*tThisWorld.atCoverTrigger[coverIndexHorzontal].AItoMove.push_back(spacePirate);

	tThisWorld.atCoverTrigger[coverIndexHorzontal].coverAiCanGoTo.push_back(tThisWorld.atCover[cover1]);*/

	//
	//	AILocation = pcGraphicsSystem->SetDefaultWorldPosition();
	//	AILocation.r[3].m128_f32[2] -= 106;
	//	AILocation.r[3].m128_f32[0] -= 13;
	//	AILocation.r[3].m128_f32[1] -= 1;
	//	AiLookPosition = AILocation;
	//	AiLookPosition.r[3].m128_f32[0] -= 12;
	//	AiLookPosition.r[3].m128_f32[2] += 10;
	//
	//	CreateNodePoint(&tThisWorld, AiLookPosition);
	//
	//	coverPosition.clear();
	//
	//
	//	pcAiSystem->LookAtObject(AILocation, &m_d3dPlayerMatrix);
	//
	//
	//	//CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone);
	//
	//
	//#pragma endregion
	//
	//	tempImport = pcGraphicsSystem->ReadMesh("meshData_Scyllian.txt");
	//	gunImport = pcGraphicsSystem->ReadMesh("meshData_LaserFlintlockTextured.txt");
	//	//AILocation.r[3].m128_f32[0] += 7;
	//
	//	for (int meshIndex = 0; meshIndex < tempImport.meshCount; ++meshIndex)
	//	{
	//		spacePirate = CreateScyllian(&tThisWorld, AILocation, enemyToCopyFrom);
	//		//spacePirate = CreateScyllian(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex], AILocation);
	//	}
	//	pcAiSystem->LookAtObject(AiLookPosition, &tThisWorld.atWorldMatrix[spacePirate].worldMatrix);
	//	//spacePirate = CreateSpacePirate(&tThisWorld, AILocation);
	//	tThisWorld.atAiHeath[spacePirate].heath = 100;
	//	createGSQuad(&tThisWorld, XMFLOAT4(1, 0, 0, 1), spacePirate);
	//	createGSQuad(&tThisWorld, XMFLOAT4(0, 0, 0, 1), spacePirate);
	//
	//
	//
	//
	//
	//	//int GunINdexai = CreateGun(&tThisWorld, m_d3dWorldMatrix, spacePirate, -1.1, 0.5, 12.5, 10, 30);
	//	for (int meshIndex = 0; meshIndex < tempImport.meshCount; ++meshIndex)
	//	{
	//	 GunINdexai = CreateScyllianGun(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, m_d3dWorldMatrix, spacePirate, -1.5, 1, 11.5, 10, 200, gunImport.vtMeshes[meshIndex], gunImport.vtMaterials[meshIndex]);
	//	}
	//#pragma region MORE AI Init
	//	tThisWorld.atAIMask[spacePirate].GunIndex = GunINdexai;
	//
	//	tThisWorld.atClip[GunINdexai].bulletSpeed = 0.01;//Frame Dependent
	//
	//
	//	AiFrustum.row1.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[0];
	//	AiFrustum.row1.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[1];
	//	AiFrustum.row1.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[2];
	//	AiFrustum.row1.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[3];
	//
	//	AiFrustum.row2.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[0];
	//	AiFrustum.row2.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[1];
	//	AiFrustum.row2.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[2];
	//	AiFrustum.row2.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[3];
	//
	//	AiFrustum.row3.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[0];
	//	AiFrustum.row3.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[1];
	//	AiFrustum.row3.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[2];
	//	AiFrustum.row3.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[3];
	//
	//	AiFrustum.row4.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[0];
	//	AiFrustum.row4.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[1];
	//	AiFrustum.row4.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[2];
	//	AiFrustum.row4.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[3];
	//
	//	frustumIndex = pcAiSystem->calculate_frustum(&tThisWorld, planes, AiFrustum, 70, 1, 0.1, 20, spacePirate, -2.1, 1.4, 19.6);
	//	tThisWorld.atWorldMatrix[frustumIndex].worldMatrix = AILocation;
	//
	//	tThisWorld.atAIVision[spacePirate].eyes0 = planes;
	//	tThisWorld.atAIVision[spacePirate].normalAtBegining[0] = planes[0].normal;
	//	tThisWorld.atAIVision[spacePirate].normalAtBegining[1] = planes[1].normal;
	//	tThisWorld.atAIVision[spacePirate].normalAtBegining[2] = planes[2].normal;
	//	tThisWorld.atAIVision[spacePirate].normalAtBegining[3] = planes[3].normal;
	//	tThisWorld.atAIVision[spacePirate].normalAtBegining[4] = planes[4].normal;
	//	tThisWorld.atAIVision[spacePirate].normalAtBegining[5] = planes[5].normal;
	//	//AILocation.r[3].m128_f32[0] += 8;
	//	tThisWorld.atPathPlanining[spacePirate].startingNode = nodeindex2;
	//	tThisWorld.atPathPlanining[spacePirate].Goal = nodeindex;
	//
	//	tThisWorld.atAIVision[spacePirate].keepSearching = false;
	//	//	tThisWorld.atCoverTrigger[CoverTriggerIndex].AItoMove.push_back(spacePirate);
	//	//
	//	//	tThisWorld.atCoverTrigger[CoverTriggerIndex].coverAiCanGoTo.push_back(tThisWorld.atCover[cover1]);
	//	//#pragma endregion
	//	// This is my AIMovementTrigger for backleft ai and back right
	//	TriggerZone = AILocation;
	//	TriggerZone.r[3].m128_f32[2] += 10;
	//
	//	CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone);
	//
	//	CoverLocation = AILocation;
	//	coverPosition.clear();
	//	CoverLocation.r[3].m128_f32[2] += 4;
	//	cover1 = CreateCover(&tThisWorld, CoverLocation, coverPosition);
	//	nodeLocation = CoverLocation;
	//	nodeLocation.r[3].m128_f32[0] += 0;
	// nodeLocation.r[3].m128_f32[1] -= 0;
	////	nodeLocation.r[3].m128_f32[2] += -2;
	//
	//	//pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	//	 nodeindexBackRight = CreateNodePoint(&tThisWorld, nodeLocation);
	//	nodePosition;
	//	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	//	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	//	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	//	pcAiSystem->AddNodeToPathFinding(nodeindexBackRight, nodePosition, 1);
	//	nodeLocation = AILocation;
	//	 backrightFrontnodeindex2 = CreateNodePoint(&tThisWorld, nodeLocation);
	//
	//	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	//	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	//	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	//	pcAiSystem->AddNodeToPathFinding(backrightFrontnodeindex2, nodePosition, 1);
	//	tThisWorld.atCover[cover1].CoverPositions.push_back(nodeindexBackRight);
	//	tThisWorld.atPathPlanining[spacePirate].startingNode = backrightFrontnodeindex2;
	//
	//	//	pcAiSystem->LookAtObject(AILocation, &m_d3dPlayerMatrix);
	//
	//
	//	//CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone);
	//
	//	edges.clear();
	//	edges.push_back(nodeindexBackRight);
	//	pcAiSystem->AddEdgestoNode(backrightFrontnodeindex2, edges);
	//	//pcAiSystem->AddEdgestoNode(nodeindex3, edges);
	//	edges.clear();
	//	edges.push_back(backrightFrontnodeindex2);
	//	pcAiSystem->AddEdgestoNode(nodeindexBackRight, edges);
	//
	//	tThisWorld.atCoverTrigger[CoverTriggerIndex].AItoMove.push_back(spacePirate);
	//
	//	tThisWorld.atCoverTrigger[CoverTriggerIndex].coverAiCanGoTo.push_back(tThisWorld.atCover[cover1]);
	//	tThisWorld.atPathPlanining[spacePirate].Goal = nodeindexBackRight;
	//
	//
	//
	//
	//	AILocation = pcGraphicsSystem->SetDefaultWorldPosition();
	//	AILocation.r[3].m128_f32[2] -= 106;
	//	AILocation.r[3].m128_f32[0] -= 3;
	//	AILocation.r[3].m128_f32[1] -= 1;
	//	AiLookPosition = AILocation;
	//	AiLookPosition.r[3].m128_f32[0] -= 12;
	//	AiLookPosition.r[3].m128_f32[2] += 10;
	//
	//	CreateNodePoint(&tThisWorld, AiLookPosition);
	//
	//	coverPosition.clear();
	//
	//
	//	pcAiSystem->LookAtObject(AILocation, &m_d3dPlayerMatrix);
	//
	//
	//	//CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone);
	//
	//
	//#pragma endregion
	//
	//	tempImport = pcGraphicsSystem->ReadMesh("meshData_Scyllian.txt");
	//	gunImport = pcGraphicsSystem->ReadMesh("meshData_LaserFlintlockTextured.txt");
	//	//AILocation.r[3].m128_f32[0] += 7;
	//
	//	for (int meshIndex = 0; meshIndex < tempImport.meshCount; ++meshIndex)
	//	{
	//		spacePirate = CreateScyllian(&tThisWorld, AILocation, enemyToCopyFrom);
	//		//spacePirate = CreateScyllian(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex], AILocation);
	//	}
	//	pcAiSystem->LookAtObject(AiLookPosition, &tThisWorld.atWorldMatrix[spacePirate].worldMatrix);
	//	//spacePirate = CreateSpacePirate(&tThisWorld, AILocation);
	//	tThisWorld.atAiHeath[spacePirate].heath = 100;
	//	createGSQuad(&tThisWorld, XMFLOAT4(1, 0, 0, 1), spacePirate);
	//	createGSQuad(&tThisWorld, XMFLOAT4(0, 0, 0, 1), spacePirate);
	//
	//
	//
	//
	//
	//	//int GunINdexai = CreateGun(&tThisWorld, m_d3dWorldMatrix, spacePirate, -1.1, 0.5, 12.5, 10, 30);
	//	for (int meshIndex = 0; meshIndex < tempImport.meshCount; ++meshIndex)
	//	{
	//	 GunINdexai = CreateScyllianGun(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, m_d3dWorldMatrix, spacePirate, -1.5, 1, 11.5, 10, 200, gunImport.vtMeshes[meshIndex], gunImport.vtMaterials[meshIndex]);
	//	}
	//#pragma region MORE AI Init
	//	tThisWorld.atAIMask[spacePirate].GunIndex = GunINdexai;
	//
	//	tThisWorld.atClip[GunINdexai].bulletSpeed = 0.01;//Frame Dependent
	//
	//
	//	AiFrustum.row1.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[0];
	//	AiFrustum.row1.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[1];
	//	AiFrustum.row1.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[2];
	//	AiFrustum.row1.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[3];
	//
	//	AiFrustum.row2.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[0];
	//	AiFrustum.row2.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[1];
	//	AiFrustum.row2.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[2];
	//	AiFrustum.row2.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[3];
	//
	//	AiFrustum.row3.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[0];
	//	AiFrustum.row3.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[1];
	//	AiFrustum.row3.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[2];
	//	AiFrustum.row3.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[3];
	//
	//	AiFrustum.row4.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[0];
	//	AiFrustum.row4.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[1];
	//	AiFrustum.row4.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[2];
	//	AiFrustum.row4.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[3];
	//
	//	frustumIndex = pcAiSystem->calculate_frustum(&tThisWorld, planes, AiFrustum, 70, 1, 0.1, 20, spacePirate, -2.1, 1.4, 19.6);
	//	tThisWorld.atWorldMatrix[frustumIndex].worldMatrix = AILocation;
	//
	//	tThisWorld.atAIVision[spacePirate].eyes0 = planes;
	//	tThisWorld.atAIVision[spacePirate].normalAtBegining[0] = planes[0].normal;
	//	tThisWorld.atAIVision[spacePirate].normalAtBegining[1] = planes[1].normal;
	//	tThisWorld.atAIVision[spacePirate].normalAtBegining[2] = planes[2].normal;
	//	tThisWorld.atAIVision[spacePirate].normalAtBegining[3] = planes[3].normal;
	//	tThisWorld.atAIVision[spacePirate].normalAtBegining[4] = planes[4].normal;
	//	tThisWorld.atAIVision[spacePirate].normalAtBegining[5] = planes[5].normal;
	//	//AILocation.r[3].m128_f32[0] += 8;
	//	tThisWorld.atPathPlanining[spacePirate].startingNode = nodeindex2;
	//	tThisWorld.atPathPlanining[spacePirate].Goal = nodeindex;
	//
	//	tThisWorld.atAIVision[spacePirate].keepSearching = false;
	//	//	tThisWorld.atCoverTrigger[CoverTriggerIndex].AItoMove.push_back(spacePirate);
	//	//
	//	//	tThisWorld.atCoverTrigger[CoverTriggerIndex].coverAiCanGoTo.push_back(tThisWorld.atCover[cover1]);
	//	//#pragma endregion
	//	// This is my AIMovementTrigger for backleft ai and back right
	//	TriggerZone = AILocation;
	//	TriggerZone.r[3].m128_f32[2] += 10;
	//
	//	CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone);
	//
	//	CoverLocation = AILocation;
	//	coverPosition.clear();
	//	CoverLocation.r[3].m128_f32[2] += 4;
	//	cover1 = CreateCover(&tThisWorld, CoverLocation, coverPosition);
	//	nodeLocation = CoverLocation;
	//	nodeLocation.r[3].m128_f32[0] += 0;
	// nodeLocation.r[3].m128_f32[1] -= 0;
	//	//	nodeLocation.r[3].m128_f32[2] += -2;
	//
	//	//pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	//	nodeindexBackRight = CreateNodePoint(&tThisWorld, nodeLocation);
	//	nodePosition;
	//	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	//	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	//	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	//	pcAiSystem->AddNodeToPathFinding(nodeindexBackRight, nodePosition, 1);
	//	nodeLocation = AILocation;
	//	backrightFrontnodeindex2 = CreateNodePoint(&tThisWorld, nodeLocation);
	//
	//	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	//	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	//	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	//	pcAiSystem->AddNodeToPathFinding(backrightFrontnodeindex2, nodePosition, 1);
	//	tThisWorld.atCover[cover1].CoverPositions.push_back(nodeindexBackRight);
	//	tThisWorld.atPathPlanining[spacePirate].startingNode = backrightFrontnodeindex2;
	//
	//	//	pcAiSystem->LookAtObject(AILocation, &m_d3dPlayerMatrix);
	//
	//
	//	//CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone);
	//
	//	edges.clear();
	//	edges.push_back(nodeindexBackRight);
	//	pcAiSystem->AddEdgestoNode(backrightFrontnodeindex2, edges);
	//	//pcAiSystem->AddEdgestoNode(nodeindex3, edges);
	//	edges.clear();
	//	edges.push_back(backrightFrontnodeindex2);
	//	pcAiSystem->AddEdgestoNode(nodeindexBackRight, edges);
	//
	//	tThisWorld.atCoverTrigger[CoverTriggerIndex].AItoMove.push_back(spacePirate);
	//
	//	tThisWorld.atCoverTrigger[CoverTriggerIndex].coverAiCanGoTo.push_back(tThisWorld.atCover[cover1]);
	//	tThisWorld.atPathPlanining[spacePirate].Goal = nodeindexBackRight;
	//
	//
	//
	//	AILocation = pcGraphicsSystem->SetDefaultWorldPosition();
	//	AILocation.r[3].m128_f32[2] -= 135;
	//	AILocation.r[3].m128_f32[0] -= 9;
	//	AILocation.r[3].m128_f32[1] -= 1;
	//	AiLookPosition = AILocation;
	//	AiLookPosition.r[3].m128_f32[0] -= 12;
	//	AiLookPosition.r[3].m128_f32[2] += 10;
	//
	//	CreateNodePoint(&tThisWorld, AiLookPosition);
	//
	//	coverPosition.clear();
	//
	//
	//	pcAiSystem->LookAtObject(AILocation, &m_d3dPlayerMatrix);
	//
	//
	//	//CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone);
	//
	//
	//#pragma endregion
	//
	//	tempImport = pcGraphicsSystem->ReadMesh("meshData_Scyllian.txt");
	//	gunImport = pcGraphicsSystem->ReadMesh("meshData_LaserFlintlockTextured.txt");
	//	//AILocation.r[3].m128_f32[0] += 7;
	//
	//	for (int meshIndex = 0; meshIndex < tempImport.meshCount; ++meshIndex)
	//	{
	//		spacePirate = CreateScyllian(&tThisWorld, AILocation, enemyToCopyFrom);
	//		//spacePirate = CreateScyllian(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, tempImport.vtMeshes[meshIndex], tempImport.vtMaterials[meshIndex], AILocation);
	//	}
	//	pcAiSystem->LookAtObject(AiLookPosition, &tThisWorld.atWorldMatrix[spacePirate].worldMatrix);
	//	//spacePirate = CreateSpacePirate(&tThisWorld, AILocation);
	//	tThisWorld.atAiHeath[spacePirate].heath = 100;
	//	createGSQuad(&tThisWorld, XMFLOAT4(1, 0, 0, 1), spacePirate);
	//	createGSQuad(&tThisWorld, XMFLOAT4(0, 0, 0, 1), spacePirate);
	//
	//
	//
	//
	//
	//	//int GunINdexai = CreateGun(&tThisWorld, m_d3dWorldMatrix, spacePirate, -1.1, 0.5, 12.5, 10, 30);
	//	for (int meshIndex = 0; meshIndex < tempImport.meshCount; ++meshIndex)
	//	{
	//	 GunINdexai = CreateScyllianGun(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, m_d3dWorldMatrix, spacePirate, -1.5, 1, 11.5, 10, 200, gunImport.vtMeshes[meshIndex], gunImport.vtMaterials[meshIndex]);
	//	}
	//#pragma region MORE AI Init
	//	tThisWorld.atAIMask[spacePirate].GunIndex = GunINdexai;
	//
	//	tThisWorld.atClip[GunINdexai].bulletSpeed = 0.01;//Frame Dependent
	//
	//
	//	AiFrustum.row1.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[0];
	//	AiFrustum.row1.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[1];
	//	AiFrustum.row1.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[2];
	//	AiFrustum.row1.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[0].m128_f32[3];
	//
	//	AiFrustum.row2.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[0];
	//	AiFrustum.row2.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[1];
	//	AiFrustum.row2.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[2];
	//	AiFrustum.row2.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[1].m128_f32[3];
	//
	//	AiFrustum.row3.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[0];
	//	AiFrustum.row3.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[1];
	//	AiFrustum.row3.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[2];
	//	AiFrustum.row3.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[2].m128_f32[3];
	//
	//	AiFrustum.row4.x = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[0];
	//	AiFrustum.row4.y = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[1];
	//	AiFrustum.row4.z = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[2];
	//	AiFrustum.row4.w = tThisWorld.atWorldMatrix[spacePirate].worldMatrix.r[3].m128_f32[3];
	//
	//	frustumIndex = pcAiSystem->calculate_frustum(&tThisWorld, planes, AiFrustum, 70, 1, 0.1, 20, spacePirate, -2.1, 1.4, 19.6);
	//	tThisWorld.atWorldMatrix[frustumIndex].worldMatrix = AILocation;
	//
	//	tThisWorld.atAIVision[spacePirate].eyes0 = planes;
	//	tThisWorld.atAIVision[spacePirate].normalAtBegining[0] = planes[0].normal;
	//	tThisWorld.atAIVision[spacePirate].normalAtBegining[1] = planes[1].normal;
	//	tThisWorld.atAIVision[spacePirate].normalAtBegining[2] = planes[2].normal;
	//	tThisWorld.atAIVision[spacePirate].normalAtBegining[3] = planes[3].normal;
	//	tThisWorld.atAIVision[spacePirate].normalAtBegining[4] = planes[4].normal;
	//	tThisWorld.atAIVision[spacePirate].normalAtBegining[5] = planes[5].normal;
	//	//AILocation.r[3].m128_f32[0] += 8;
	//	tThisWorld.atPathPlanining[spacePirate].startingNode = nodeindex2;
	//	tThisWorld.atPathPlanining[spacePirate].Goal = nodeindex;
	//
	//	tThisWorld.atAIVision[spacePirate].keepSearching = false;
	//	//	tThisWorld.atCoverTrigger[CoverTriggerIndex].AItoMove.push_back(spacePirate);
	//	//
	//	//	tThisWorld.atCoverTrigger[CoverTriggerIndex].coverAiCanGoTo.push_back(tThisWorld.atCover[cover1]);
	//	//#pragma endregion
	//	// This is my AIMovementTrigger for backleft ai and back right
	//	TriggerZone = AILocation;
	//	TriggerZone.r[3].m128_f32[2] += 10;
	//
	//	CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone);
	//
	//	CoverLocation = AILocation;
	//	coverPosition.clear();
	//	CoverLocation.r[3].m128_f32[2] += 4;
	//	cover1 = CreateCover(&tThisWorld, CoverLocation, coverPosition);
	//	nodeLocation = CoverLocation;
	//	nodeLocation.r[3].m128_f32[0] += 0;
	// nodeLocation.r[3].m128_f32[1] -= 0;
	//		nodeLocation.r[3].m128_f32[2] += -20;
	//
	//	//pcAiSystem->AddNodeToPathFinding(nodeLocation, nodePosition, 1);
	//	nodeindexBackRight = CreateNodePoint(&tThisWorld, nodeLocation);
	//	nodePosition;
	//	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	//	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	//	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	//	pcAiSystem->AddNodeToPathFinding(nodeindexBackRight, nodePosition, 1);
	//	nodeLocation = AILocation;
	//	backrightFrontnodeindex2 = CreateNodePoint(&tThisWorld, nodeLocation);
	//
	//	nodePosition.x = nodeLocation.r[3].m128_f32[0];
	//	nodePosition.y = nodeLocation.r[3].m128_f32[1];
	//	nodePosition.z = nodeLocation.r[3].m128_f32[2];
	//	pcAiSystem->AddNodeToPathFinding(backrightFrontnodeindex2, nodePosition, 1);
	//	tThisWorld.atCover[cover1].CoverPositions.push_back(nodeindexBackRight);
	//	tThisWorld.atPathPlanining[spacePirate].startingNode = backrightFrontnodeindex2;
	//
	//	//	pcAiSystem->LookAtObject(AILocation, &m_d3dPlayerMatrix);
	//
	//
	//	//CoverTriggerIndex = CreateCoverTriggerZone(&tThisWorld, TriggerZone);
	//
	//	edges.clear();
	//	edges.push_back(nodeindexBackRight);
	//	pcAiSystem->AddEdgestoNode(backrightFrontnodeindex2, edges);
	//	//pcAiSystem->AddEdgestoNode(nodeindex3, edges);
	//	edges.clear();
	//	edges.push_back(backrightFrontnodeindex2);
	//	pcAiSystem->AddEdgestoNode(nodeindexBackRight, edges);
	//
	//	tThisWorld.atCoverTrigger[CoverTriggerIndex].AItoMove.push_back(spacePirate);
	//
	//	tThisWorld.atCoverTrigger[CoverTriggerIndex].coverAiCanGoTo.push_back(tThisWorld.atCover[cover1]);
	//	tThisWorld.atPathPlanining[spacePirate].Goal = nodeindexBackRight;

	XMMATRIX rectangle = m_d3dPlayerMatrix;
	rectangle.r[3].m128_f32[2] -= 53.5f;
	rectangle.r[3].m128_f32[1] -= 1.0f;
	rectangle.r[3].m128_f32[0] -= 1.0f;
	int cylinder = CreateCylinder(&tThisWorld, rectangle);
	tThisWorld.atCollisionMask[218].m_tnCollisionMask = 1;
	tThisWorld.atCollisionMask[233].m_tnCollisionMask = 1;

	tThisWorld.atCollisionMask[118].m_tnCollisionMask = 1;

#pragma region COLLISION INIT
	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{

		if (cylinder == nCurrentEntity) {
			float x = 0;
		}
		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask > 1 && nCurrentEntity != 210&& nCurrentEntity != 215 && nCurrentEntity != 218 && nCurrentEntity != 118 && nCurrentEntity != 233 && nCurrentEntity != 133 && nCurrentEntity != 33)// && nCurrentEntity != 18)
		{

			if (tThisWorld.atSimpleMesh[nCurrentEntity].m_nVertexCount > tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount)
			{
			/*	if (nCurrentEntity == 218) {
					TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[nCurrentEntity].m_VertexData, tThisWorld.atAABB[nCurrentEntity]);
					MyAbb.m_IndexLocation = nCurrentEntity;
					tThisWorld.atAABB[nCurrentEntity] = MyAbb;
					pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);

				}*/
				TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[nCurrentEntity].m_VertexData, tThisWorld.atAABB[nCurrentEntity]);
				
				MyAbb.m_IndexLocation = nCurrentEntity;
				MyAbb.locationinArray = pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);

				tThisWorld.atAABB[nCurrentEntity] = MyAbb;


				/*if (nCurrentEntity == door1Index || nCurrentEntity == door2Index || swordGuy == nCurrentEntity) {
				pcCollisionSystem->AddAiVisioNCheck(MyAbb, nCurrentEntity);
				}*/

			}

			if (tThisWorld.atMesh[nCurrentEntity].m_nVertexCount > tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount)
			{

				TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atMesh[nCurrentEntity].m_VertexData, tThisWorld.atAABB[nCurrentEntity]);
				MyAbb.m_IndexLocation = nCurrentEntity;
				MyAbb.locationinArray = pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);

				if (nCurrentEntity == ClaytonIndex)
				{
					MyAbb.m_dMaxPoint.x *= 0.01f;
					MyAbb.m_dMaxPoint.y *= 0.01f;
					MyAbb.m_dMaxPoint.z *= 0.01f;

					MyAbb.m_dMinPoint.x *= 0.01f;
					MyAbb.m_dMinPoint.y *= 0.01f;
					MyAbb.m_dMinPoint.z *= 0.01f;

					MyAbb.m_dMinPointOrginal.x *= 0.01f;
					MyAbb.m_dMinPointOrginal.y *= 0.01f;
					MyAbb.m_dMinPointOrginal.z *= 0.01f;

					MyAbb.m_dMaxPointOrginal.x *= 0.01f;
					MyAbb.m_dMaxPointOrginal.y *= 0.01f;
					MyAbb.m_dMaxPointOrginal.z *= 0.01f;
				}

				tThisWorld.atAABB[nCurrentEntity] = MyAbb;
				pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);
				
				if (tThisWorld.atShaderID[nCurrentEntity].m_nShaderID == 20) {
					vector<int> DoorPices;
					DoorPices.push_back(nCurrentEntity - 1);
					DoorPices.push_back(nCurrentEntity + 1);
					DoorPices.push_back(nCurrentEntity + 2);
					DoorPices.push_back(nCurrentEntity + 3);
					//	DoorPices.push_back(nCurrentEntity + 4);

					tThisWorld.atAABB[nCurrentEntity].doorPeices = DoorPices;
					float x = 0;
				}
				if (tThisWorld.atShaderID[nCurrentEntity].m_nShaderID == 30) {
					vector<int> DoorPices;
					//	tThisWorld.atWorldMatrix[nCurrentEntity - 4].worldMatrix.r[3].m128_f32[2] += 5;
						/*tThisWorld.atShaderID[nCurrentEntity - 1].m_nShaderID = 34;
						tThisWorld.atShaderID[nCurrentEntity - 2].m_nShaderID = 32;
						tThisWorld.atShaderID[nCurrentEntity - 3].m_nShaderID = 31;
						tThisWorld.atShaderID[nCurrentEntity - 4].m_nShaderID = 33;*/



					DoorPices.push_back(nCurrentEntity - 4);
					DoorPices.push_back(nCurrentEntity - 1);
					DoorPices.push_back(nCurrentEntity - 2);
					DoorPices.push_back(nCurrentEntity - 3);
					//	DoorPices.push_back(nCurrentEntity + 4);

					tThisWorld.atAABB[nCurrentEntity].doorPeices = DoorPices;
					float x = 0;
				}
				if (tThisWorld.atShaderID[nCurrentEntity].m_nShaderID == 40) {
					vector<int> DoorPices;
					DoorPices.push_back(nCurrentEntity + 1);
					DoorPices.push_back(nCurrentEntity + 2);
					DoorPices.push_back(nCurrentEntity + 3);
					DoorPices.push_back(nCurrentEntity + 4);
					tThisWorld.atAABB[nCurrentEntity].doorPeices = DoorPices;

					float x = 0;
				}
				if (nCurrentEntity == PlayerStartIndex) {
					pcCollisionSystem->AddAiVisioNCheck(MyAbb, nCurrentEntity);

				}
			}
		}

		if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_CLIP) && nCurrentEntity != 218)
		{
			if (tThisWorld.atMesh[nCurrentEntity].m_nVertexCount > tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount)
			{
				TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atMesh[nCurrentEntity].m_VertexData, tThisWorld.atAABB[nCurrentEntity]);
				MyAbb.m_IndexLocation = nCurrentEntity;
				MyAbb.locationinArray = pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);

				tThisWorld.atAABB[nCurrentEntity] = MyAbb;
				pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);
			}
		}
	}
	tThisWorld.atAABB[208].m_dMaxPoint.x *= 2;
	tThisWorld.atAABB[208].m_dMaxPoint.y *= 2;
	tThisWorld.atAABB[208].m_dMaxPoint.z *= 2;
	tThisWorld.atAABB[208].m_dMinPoint.z *= -2;
	tThisWorld.atAABB[208].m_dMinPoint.x *= -2;
	tThisWorld.atAABB[208].m_dMinPoint.y *= -2;
	pcCollisionSystem->replaceAABB(208, tThisWorld.atAABB[208]);




	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if (tThisWorld.atCollisionMask[nCurrentEntity].m_tnCollisionMask > 1)
		{
			if (nCurrentEntity == 94) {
				float x = 0;
			}
			tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
		}
	}
	pcAiSystem->SetCanWeChooseShooter(true);

#pragma endregion

#if EXTRACTIONBEAM
	TPrimalVert x;
	x.m_d3dfColor = XMFLOAT4(1, 1, 0, 1);
	x.m_d3dfPosition = XMFLOAT3(0, 0, 0);
	atBeamVerts.push_back(x);
	x.m_d3dfPosition = XMFLOAT3(10, 0, 0);
	atBeamVerts.push_back(x);
	x.m_d3dfPosition = XMFLOAT3(0, 0, 0);
	atBeamVerts.push_back(x);
	x.m_d3dfPosition = XMFLOAT3(10, 0, 0);
	atBeamVerts.push_back(x);
	x.m_d3dfPosition = XMFLOAT3(0, 0, 0);
	atBeamVerts.push_back(x);
	x.m_d3dfPosition = XMFLOAT3(10, 0, 0);
	atBeamVerts.push_back(x);

	ExtractionBeamIndex = CreateExtractionBeam(&tThisWorld, m_d3dWorldMatrix, PlayerStartIndex, atBeamVerts);
#endif
	pcGraphicsSystem->CreateBuffers(&tThisWorld);

	int textSize = 2;
	wchar_t* textBuffer = new wchar_t[textSize];

	if (tThisWorld.atClip[GunIndexForPlayer].nBulletsAvailables.size() <= 9)
	{
		textBuffer[0] = 0;
		textBuffer[1] = tThisWorld.atClip[GunIndexForPlayer].nBulletsAvailables.size();
	}
#if CAELIS
	else
	{
		textBuffer[0] = tThisWorld.atClip[GunIndexForPlayer].nBulletsAvailables.size() * .1;
		textBuffer[1] = tThisWorld.atClip[GunIndexForPlayer].nBulletsAvailables.size() % 10;
	}
#endif

	pcUISystem->UpdateText(&tThisWorld, pcGraphicsSystem, ammoIndex, &atUIVertices, textBuffer, textSize, atUIVertices.at(tThisWorld.atLabel[ammoIndex].vIndex));
	
	delete[] textBuffer;

	moveTime = 0;
	bNoMoving = true;
	loading = true;
	fpsTimer.Restart();
	fpsTimer.Init_FPSReader();
}

#pragma region Door Stuff
enum
{
	DoorPiece1_FRAME = 20,
	DoorPiece1_UP,
	DoorPiece1_DOWN,
	DoorPiece1_LEFT,
	DoorPiece1_RIGHT,
	DoorPiece2_FRAME = 30,
	DoorPiece2_UP,
	DoorPiece2_DOWN,
	DoorPiece2_LEFT,
	DoorPiece2_RIGHT,
	DoorPiece3_FRAME = 40,
	DoorPiece3_UP,
	DoorPiece3_DOWN,
	DoorPiece3_LEFT,
	DoorPiece3_RIGHT,
	DoorPiece4_FRAME = 50,
	DoorPiece4_UP,
	DoorPiece4_DOWN,
	DoorPiece4_LEFT,
	DoorPiece4_RIGHT,
	DoorPiece5_FRAME = 60,
	DoorPiece5_UP,
	DoorPiece5_DOWN,
	DoorPiece5_LEFT,
	DoorPiece5_RIGHT,
	DoorPiece6_FRAME = 70,
	DoorPiece6_UP,
	DoorPiece6_DOWN,
	DoorPiece6_LEFT,
	DoorPiece6_RIGHT,
};
int doors[6] = { -2, -2, -2, -2, -2, -2 };

XMMATRIX DoorEventListener(int shaderID, bool open)
{
	switch (shaderID)
	{
	case 0:
	{
		if (doors[0] == 1)
		{
			doors[0] = 2;

		}
		else if (doors[0] == -1)
		{
			doors[0] = -2;
		}
	}
	case DoorPiece1_FRAME:
	{
		if (open)
		{
			if (doors[0] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 0, 0);
			}
			else if (doors[0] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 0, 0);
			}
		}
		else
		{
			if (doors[0] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 0, 0);
			}
			else if (doors[0] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 0, 0);
			}
		}
		break;
	}
	case DoorPiece1_UP:
	{
		if (open)
		{
			if (doors[0] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 1, 0);
			}
			else if (doors[0] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, -1, 0);
			}
		}
		else
		{
			if (doors[0] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 1, 0);
			}
			else if (doors[0] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, -1, 0);
			}
		}
		break;
	}
	case DoorPiece1_DOWN:
	{
		if (open)
		{
			if (doors[0] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, -1, 0);
			}
			else if (doors[0] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 1, 0);
			}
		}
		else
		{
			if (doors[0] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, -1, 0);
			}
			else if (doors[0] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 1, 0);
			}
		}
		break;
	}
	case DoorPiece1_LEFT:
	{
		if (open)
		{
			if (doors[0] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(-1, 0, 0);
			}
			else if (doors[0] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(1, 0, 0);
			}
		}
		else
		{
			if (doors[0] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(-1, 0, 0);
			}
			else if (doors[0] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(1, 0, 0);
			}
		}
		break;
	}
	case DoorPiece1_RIGHT:
	{

		if (open)
		{
			if (doors[0] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(1, 0, 0);
			}
			else if (doors[0] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(-1, 0, 0);
			}
		}
		else
		{
			if (doors[0] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(1, 0, 0);
			}
			else if (doors[0] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(-1, 0, 0);
			}
		}
		break;
	}
	case DoorPiece2_FRAME:
	{
		if (open)
		{
			if (doors[1] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 0, 0);
			}
			else if (doors[1] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 0, 0);
			}
		}
		else
		{
			if (doors[1] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 0, 0);
			}
			else if (doors[1] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 0, 0);
			}
		}
		break;
	}
	case DoorPiece2_UP:
	{
		if (open)
		{
			if (doors[1] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 1, 0);
			}
			else if (doors[1] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, -1, 0);
			}
		}
		else
		{
			if (doors[1] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 1, 0);
			}
			else if (doors[1] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, -1, 0);
			}
		}
		break;
	}
	case DoorPiece2_DOWN:
	{
		if (open)
		{
			if (doors[1] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, -1, 0);
			}
			else if (doors[1] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 1, 0);
			}
		}
		else
		{
			if (doors[1] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, -1, 0);
			}
			else if (doors[1] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 1, 0);
			}
		}
		break;
	}
	case DoorPiece2_LEFT:
	{
		if (open)
		{
			if (doors[1] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(-1, 0, 0);
			}
			else if (doors[1] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(1, 0, 0);
			}
		}
		else
		{
			if (doors[1] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(-1, 0, 0);
			}
			else if (doors[1] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(1, 0, 0);
			}
		}
		break;
	}
	case DoorPiece2_RIGHT:
	{

		if (open)
		{
			if (doors[1] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(1, 0, 0);
			}
			else if (doors[1] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(-1, 0, 0);
			}
		}
		else
		{
			if (doors[1] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(1, 0, 0);
			}
			else if (doors[1] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(-1, 0, 0);
			}
		}
		break;
	}
	case DoorPiece3_FRAME:
	{
		if (open)
		{
			if (doors[2] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 0, 0);
			}
			else if (doors[2] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 0, 0);
			}
		}
		else
		{
			if (doors[2] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 0, 0);
			}
			else if (doors[2] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 0, 0);
			}
		}
		break;
	}
	case DoorPiece3_UP:
	{
		if (open)
		{
			if (doors[2] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 1, 0);
			}
			else if (doors[2] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, -1, 0);
			}
		}
		else
		{
			if (doors[2] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 1, 0);
			}
			else if (doors[2] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, -1, 0);
			}
		}
		break;
	}
	case DoorPiece3_DOWN:
	{
		if (open)
		{
			if (doors[2] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, -1, 0);
			}
			else if (doors[2] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 1, 0);
			}
		}
		else
		{
			if (doors[2] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, -1, 0);
			}
			else if (doors[2] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 1, 0);
			}
		}
		break;
	}
	case DoorPiece3_LEFT:
	{
		if (open)
		{
			if (doors[2] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(-1, 0, 0);
			}
			else if (doors[2] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(1, 0, 0);
			}
		}
		else
		{
			if (doors[2] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(-1, 0, 0);
			}
			else if (doors[2] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(1, 0, 0);
			}
		}
		break;
	}
	case DoorPiece3_RIGHT:
	{

		if (open)
		{
			if (doors[2] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(1, 0, 0);
			}
			else if (doors[2] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(-1, 0, 0);
			}
		}
		else
		{
			if (doors[2] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(1, 0, 0);
			}
			else if (doors[2] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(-1, 0, 0);
			}
		}
		break;
	}
	case DoorPiece4_FRAME:
	{
		if (open)
		{
			if (doors[3] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 0, 0);
			}
			else if (doors[3] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 0, 0);
			}
		}
		else
		{
			if (doors[3] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 0, 0);
			}
			else if (doors[3] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 0, 0);
			}
		}
		break;
	}
	case DoorPiece4_UP:
	{
		if (open)
		{
			if (doors[3] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 1, 0);
			}
			else if (doors[3] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, -1, 0);
			}
		}
		else
		{
			if (doors[3] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 1, 0);
			}
			else if (doors[3] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, -1, 0);
			}
		}
		break;
	}
	case DoorPiece4_DOWN:
	{
		if (open)
		{
			if (doors[3] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, -1, 0);
			}
			else if (doors[3] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 1, 0);
			}
		}
		else
		{
			if (doors[3] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, -1, 0);
			}
			else if (doors[3] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 1, 0);
			}
		}
		break;
	}
	case DoorPiece4_LEFT:
	{
		if (open)
		{
			if (doors[3] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(-1, 0, 0);
			}
			else if (doors[3] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(1, 0, 0);
			}
		}
		else
		{
			if (doors[3] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(-1, 0, 0);
			}
			else if (doors[3] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(1, 0, 0);
			}
		}
		break;
	}
	case DoorPiece4_RIGHT:
	{

		if (open)
		{
			if (doors[3] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(1, 0, 0);
			}
			else if (doors[3] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(-1, 0, 0);
			}
		}
		else
		{
			if (doors[3] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(1, 0, 0);
			}
			else if (doors[3] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(-1, 0, 0);
			}
		}
		break;
	}
	case DoorPiece5_FRAME:
	{
		if (open)
		{
			if (doors[4] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 0, 0);
			}
			else if (doors[4] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 0, 0);
			}
		}
		else
		{
			if (doors[4] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 0, 0);
			}
			else if (doors[4] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 0, 0);
			}
		}
		break;
	}
	case DoorPiece5_UP:
	{
		if (open)
		{
			if (doors[4] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 1, 0);
			}
			else if (doors[4] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, -1, 0);
			}
		}
		else
		{
			if (doors[4] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 1, 0);
			}
			else if (doors[4] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, -1, 0);
			}
		}
		break;
	}
	case DoorPiece5_DOWN:
	{
		if (open)
		{
			if (doors[4] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, -1, 0);
			}
			else if (doors[4] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 1, 0);
			}
		}
		else
		{
			if (doors[4] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, -1, 0);
			}
			else if (doors[4] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 1, 0);
			}
		}
		break;
	}
	case DoorPiece5_LEFT:
	{
		if (open)
		{
			if (doors[4] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(-1, 0, 0);
			}
			else if (doors[4] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(1, 0, 0);
			}
		}
		else
		{
			if (doors[4] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(-1, 0, 0);
			}
			else if (doors[4] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(1, 0, 0);
			}
		}
		break;
	}
	case DoorPiece5_RIGHT:
	{

		if (open)
		{
			if (doors[4] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(1, 0, 0);
			}
			else if (doors[4] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(-1, 0, 0);
			}
		}
		else
		{
			if (doors[4] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(1, 0, 0);
			}
			else if (doors[4] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(-1, 0, 0);
			}
		}
		break;
	}
	case DoorPiece6_FRAME:
	{
		if (open)
		{
			if (doors[5] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 0, 0);
			}
			else if (doors[5] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 0, 0);
			}
		}
		else
		{
			if (doors[5] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 0, 0);
			}
			else if (doors[5] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 0, 0);
			}
		}
		break;
	}
	case DoorPiece6_UP:
	{
		if (open)
		{
			if (doors[5] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 1, 0);
			}
			else if (doors[5] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, -1, 0);
			}
		}
		else
		{
			if (doors[5] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, 1, 0);
			}
			else if (doors[5] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, -1, 0);
			}
		}
		break;
	}
	case DoorPiece6_DOWN:
	{
		if (open)
		{
			if (doors[5] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, -1, 0);
			}
			else if (doors[5] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 1, 0);
			}
		}
		else
		{
			if (doors[5] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(0, -1, 0);
			}
			else if (doors[5] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(0, 1, 0);
			}
		}
		break;
	}
	case DoorPiece6_LEFT:
	{
		if (open)
		{
			if (doors[5] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(-1, 0, 0);
			}
			else if (doors[5] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(1, 0, 0);
			}
		}
		else
		{
			if (doors[5] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(-1, 0, 0);
			}
			else if (doors[5] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(1, 0, 0);
			}
		}
		break;
	}
	case DoorPiece6_RIGHT:
	{

		if (open)
		{
			if (doors[5] == 1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(1, 0, 0);
			}
			else if (doors[5] == -1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(-1, 0, 0);
			}
		}
		else
		{
			if (doors[5] == -1)
			{
				//doors[4] = 2;
				return XMMatrixTranslation(1, 0, 0);
			}
			else if (doors[5] == 1)
			{
				//doors[4] = -2;
				return XMMatrixTranslation(-1, 0, 0);
			}
		}
		break;
	}
	default:
	{
		return XMMatrixIdentity();
		break;
	}
	}
	return XMMatrixIdentity();
}

void DoorEventChanger(int shaderID)
{
	if (shaderID >= 20)
	{
		if (shaderID < 25)
		{
			if (doors[0] == -2)
			{
				doors[0] = 1;
			}
			else if (doors[0] == 2)
			{
				doors[0] = -1;
			}
		}
		else if (shaderID < 35)
		{
			if (doors[1] == -2)
			{
				doors[1] = 1;
			}
			else if (doors[1] == 2)
			{
				doors[1] = -1;
			}
		}
		else if (shaderID < 45)
		{
			if (doors[2] == -2)
			{
				doors[2] = 1;
			}
			else if (doors[2] == 2)
			{
				doors[2] = -1;
			}
		}
		else if (shaderID < 55)
		{
			if (doors[3] == -2)
			{
				doors[3] = 1;
			}
			else if (doors[3] == 2)
			{
				doors[3] = -1;
			}
		}
		else if (shaderID < 65)
		{
			if (doors[4] == -2)
			{
				doors[4] = 1;
			}
			else if (doors[4] == 2)
			{
				doors[4] = -1;
			}
		}
		else if (shaderID < 75)
		{
			if (doors[5] == -2)
			{
				doors[5] = 1;
			}
			else if (doors[5] == 2)
			{
				doors[5] = -1;
			}
		}
	}
}

#pragma endregion

int CGameMangerSystem::RealLevelUpdate()
{
	TLights* setLights = pcGraphicsSystem->GetLights();

#if MUSIC_ON
	pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::MUSIC_VOLUME, pcAudioSystem->m_fMusicVolume);
#endif
	if (tThisWorld.atClayton[PlayerStartIndex].health <= 0 && !endInit)
	{
		GameOver = true;
		InitializeEndScreen(false);
		endInit = true;
	}
	fpsTimer.Xtime_Signal();

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(-1); //Important!
	m_d3dProjectionMatrix = pcGraphicsSystem->SetDefaultPerspective(m_RealTimeFov);

	// checks the camera bool variables & store mouse position at the beginning  

	tCameraMode = pcInputSystem->CameraModeListen(tCameraMode);

#if CAELIS
	if ((pcInputSystem->InputCheck(G_KEY_0) == 1 || pcInputSystem->InputCheck(G_KEY_5) == 1) && pcInputSystem->m_buttonPressed != true)
	{
		PlayerStartIndex = pcInputSystem->CharacterSwitch(PlayerStartIndex);
		pcInputSystem->m_buttonPressed = true;
		if (PlayerStartIndex == ClaytonIndex)
		{
			m_d3dClaytonMatrix = tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix;
			m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix;

			GunIndexForPlayer = GunIndexForClayton;

			wchar_t filePath[] =
			{ L"UI_Textures.fbm/Material_Gun_Paint.png" };

			pcUISystem->AddTextureToUI(&tThisWorld, gunImageIndex, pcGraphicsSystem->m_pd3dDevice, filePath);
		}
		else if (PlayerStartIndex == CaelisIndex)
		{
			m_d3dCaelisMatrix = tThisWorld.atWorldMatrix[CaelisIndex].worldMatrix;
			m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[CaelisIndex].worldMatrix;

			GunIndexForPlayer = GunIndexForCaelis;

			wchar_t filePath[] =
			{ L"UI_Textures.fbm/CaelisGun_Paint.png" };

			pcUISystem->AddTextureToUI(&tThisWorld, gunImageIndex, pcGraphicsSystem->m_pd3dDevice, filePath);
		}

		int textSize = 2;
		wchar_t* textBuffer = new wchar_t[textSize];

		if (tThisWorld.atClip[GunIndexForPlayer].nBulletsAvailables.size() <= 9)
		{
			textBuffer[0] = 0;
			textBuffer[1] = tThisWorld.atClip[GunIndexForPlayer].nBulletsAvailables.size();
		}
		else
		{
			textBuffer[0] = tThisWorld.atClip[GunIndexForPlayer].nBulletsAvailables.size() * .1;
			textBuffer[1] = tThisWorld.atClip[GunIndexForPlayer].nBulletsAvailables.size() % 10;
		}

		pcUISystem->UpdateText(&tThisWorld, pcGraphicsSystem, ammoIndex, &atUIVertices, textBuffer, textSize, atUIVertices.at(tThisWorld.atLabel[ammoIndex].vIndex));

		delete[] textBuffer;
	}
	else if (pcInputSystem->InputCheck(G_KEY_0) == 0 && pcInputSystem->InputCheck(G_KEY_5) == 0 && pcInputSystem->m_buttonPressed == true)
	{
		pcInputSystem->m_buttonPressed = false;
	}
#endif

	pcInputSystem->GetMousePosition();

	static XMMATRIX m_d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();
	static XMMATRIX m_d3dOffsetMatrix = pcGraphicsSystem->SetDefaultOffset();

	CGraphicsSystem::TPrimalVertexBufferType tTempVertexBuffer;
	CGraphicsSystem::TPrimalPixelBufferType tTempPixelBuffer;
	CGraphicsSystem::TMyVertexBufferType tMyVertexBufferTemp;
	CGraphicsSystem::TAnimatedVertexBufferType tAnimVertexBuffer;
	CGraphicsSystem::TUIVertexBufferType tUIVertexBuffer;
	CGraphicsSystem::TUIPixelBufferType tUIPixelBuffer;
	CGraphicsSystem::TLightBufferType tLightBuffer;

	tMyVertexBufferTemp.m_d3dColor = XMFLOAT4(0, 0, 0, 1);


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
			tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
			for (int currentLightIndex = 0; currentLightIndex < MAX_LIGHTS; currentLightIndex++)
			{
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_d3dLightPosition, &setLights[currentLightIndex].m_d3dLightPosition, sizeof(setLights[currentLightIndex].m_d3dLightPosition));
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_d3dLightColor, &setLights[currentLightIndex].m_d3dLightColor, sizeof(setLights[currentLightIndex].m_d3dLightColor));
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_lightType, &setLights[currentLightIndex].m_lightType, sizeof(setLights[currentLightIndex].m_lightType));
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_Direction, &setLights[currentLightIndex].m_Direction, sizeof(setLights[currentLightIndex].m_Direction));
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].enabled, &setLights[currentLightIndex].enabled, sizeof(setLights[currentLightIndex].enabled));

			}
			tLightBuffer.m_Proprties = tThisWorld.atMaterial[nCurrentEntity];
			
			if (tCameraMode.bWalkMode == true)
			{
				tMyVertexBufferTemp.m_d3dViewMatrix = walkCamera->d3d_Position;
				pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], walkCamera->d3d_Position);
			}
			else if (tCameraMode.bAimMode == true)
			{
				tMyVertexBufferTemp.m_d3dViewMatrix = aimCamera->d3d_Position;
				
				pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity],tLightBuffer ,aimCamera->d3d_Position);
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

	//This abstraction is 12 lines formatted and 1 line unformatted. Not abstracted is 169 lines.
#if INPUT_ABSTRACTED_ON
	double delta = fpsTimer.GetDelta();
	bool healing;
	healing = false;
	pcInputSystem->gameManagerCodeAbstracted(pcInputSystem->InputCheck(G_BUTTON_LEFT), pcInputSystem->InputCheck(G_BUTTON_MIDDLE), pcInputSystem->InputCheck(G_KEY_P), pcInputSystem->InputCheck(G_KEY_U), pcInputSystem->InputCheck(G_KEY_R), pcInputSystem->InputCheck(G_KEY_E), 
		cApplicationWindow, pcGraphicsSystem->ResetAimModeCameraOffset(),
		tThisWorld.atClip[GunIndexForPlayer].GunMode, tThisWorld.atClip[GunIndexForPlayer].tryToShoot, tThisWorld.atClip[GunIndexForPlayer].tryToReload,
		mouseUp, mouseDown, click,
		GamePaused, GameOver, pauseInit, options,
		bNoMoving,
		m_RealTimeFov,
		startDragPoint, dragPoint, hoverPoint, clickPoint,
		tCameraMode,
		walkCamera, aimCamera, debugCamera,
		m_d3d_ResultMatrix, m_d3dClaytonMatrix, m_d3dOffsetMatrix, m_d3dWorldMatrix,
		tMyVertexBufferTemp.m_d3dViewMatrix, tTempVertexBuffer.m_d3dViewMatrix,
		tTempPixelBuffer.m_d3dCollisionColor, delta, pcAudioSystem, tThisWorld.atClayton[PlayerStartIndex], tThisWorld.atRigidBody[PlayerStartIndex].velocity, m_d3dCaelisMatrix, PlayerStartIndex,
		CaelisIndex, ClaytonIndex,tThisWorld.atCaelis[CaelisIndex], pcAudioSystem, &healing, forward, backward, left, right);

	if (healing == true) 
	{
		pcParticleSystem->CreateColorFulHealing(m_d3dClaytonMatrix, &tThisWorld, 50, pcGraphicsSystem, pcAiSystem,fpsTimer.GetDelta());
		healing = false;
	}
	
	pcGraphicsSystem->UpdateAnimationInfo(tThisWorld.atAnimation[ClaytonIndex], tThisWorld.atAnimationVariant[ClaytonIndex], forward, backward, left, right, CLAYTON);

#endif // INPUT_ABSTRACTED_ON
	pcGraphicsSystem->UpdateD3D();

#if !INPUT_ABSTRACTED_ON

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

#endif // !INPUT_ABSTRACTED_ON

	tThisWorld.atAnimation[ClaytonIndex].tTimer.StartClock(tThisWorld.atAnimation[ClaytonIndex].tTimer.tSceneTimer);
	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		tMyVertexBufferTemp.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		tTempVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		if (tThisWorld.atAABB[nCurrentEntity].disabledabb == true&tThisWorld.atAABB[nCurrentEntity].TimeColiderIsDIsabled>0) {
			tThisWorld.atAABB[nCurrentEntity].TimeColiderIsDIsabled -= fpsTimer.GetDelta() + 10;
		}
		else if (nCurrentEntity==216&& tThisWorld.atAABB[nCurrentEntity].disabledabb == true) {
			tThisWorld.atAABB[nCurrentEntity].disabledabb = false;
			pcCollisionSystem->replaceAABB(nCurrentEntity, tThisWorld.atAABB[nCurrentEntity]);
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
		//Extraction Beam & related functions are here - ZFB
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID) && tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask != (COMPONENT_UIMASK | COMPONENT_NOSHOW))
		{
			if (tThisWorld.atBar[nCurrentEntity].entityToFollow != -1 || pcCollisionSystem->aabb_to_frustum(tThisWorld.atAABB[nCurrentEntity], tThisWorld.atClaytonVision.eyes0))
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

				//	if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask != (COMPONENT_UIMASK | COMPONENT_NOSHOW))
				{
					pcGraphicsSystem->InitPrimalShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, debugCamera->d3d_Position, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], debugCamera->d3d_Position);
					pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[nCurrentEntity].m_nVertexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
				}
			}

#if EXTRACTIONBEAM
			//Extraction Beam & related functions are here - ZFB
			if ((pcInputSystem->InputCheck(G_KEY_Q) == 1
				&& tCameraMode.bAimMode == true
				&& nCurrentEntity == ExtractionBeamIndex))
			{
				//Get Gun Matrix position 
				atBeamVerts.clear();
				tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData.clear();

				XMVECTOR unProjectedFar = XMVectorSet(0, 0, 1, 1);

				XMMATRIX t1 = XMMatrixMultiply(XMMatrixTranslation(0, 0, 150), aimCamera->d3d_Position);

				unProjectedFar = XMVector3Transform(unProjectedFar, t1);

				XMVECTOR startPoint = XMVectorSet(0, 0, 0, 1);
				startPoint = XMVector3Transform(startPoint, tThisWorld.atWorldMatrix[GunIndexForClayton].worldMatrix);
				XMFLOAT3 Point1, Point2;
				XMStoreFloat3(&Point1, startPoint);
				XMStoreFloat3(&Point2, unProjectedFar);

				//std::cout << "Start Point:\t" << 
				//	Point1.x << ", " << 
				//	Point1.y << ", " << 
				//	Point1.z << "\n";

				//std::cout << "End Point:\t" << 
				//	Point2.x << ", " << 
				//	Point2.y << ", " << 
				//	Point2.z << "\n";

				tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData.push_back(startPoint);
				tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData.push_back(unProjectedFar);

				uvScroll += .01f;

				pcGraphicsSystem->StoreBeamPoints(Point1, Point2, atBeamVerts, uvScroll);
				pcGraphicsSystem->UpdateLineVTBuffer(&tThisWorld.atDebugMesh[ExtractionBeamIndex].m_d3dVertexBufferDesc, tThisWorld.atDebugMesh[ExtractionBeamIndex].m_pd3dVertexBuffer, atBeamVerts);

				pcGraphicsSystem->InitLineShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[nCurrentEntity], aimCamera->d3d_Position, atBeamVerts, particleSRV);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[ExtractionBeamIndex].m_nVertexCount, tThisWorld.atGraphicsMask[ExtractionBeamIndex].m_tnGraphicsMask, tThisWorld.atShaderID[ExtractionBeamIndex].m_nShaderID);
#if MUSIC_ON
				if (pcInputSystem->InputCheck(G_KEY_Q) == 1 && pcAudioSystem->m_BeamSoundOn == false)
				{
					pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_EXTRACT, pcAudioSystem->m_Extract);
					pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);

					pcAudioSystem->m_BeamSoundOn = true;

				}

#endif
			}
			else if (nCurrentEntity == ExtractionBeamIndex)
			{
				atBeamVerts.clear();
				tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData.clear();

				XMVECTOR startPoint = XMVectorSet(0, 0, 0, 1);
				XMVECTOR endPoint;
				startPoint = XMVector3Transform(startPoint, tThisWorld.atWorldMatrix[GunIndexForClayton].worldMatrix);
				endPoint = startPoint;

				XMVECTOR BeamDirection = endPoint - startPoint;


				XMFLOAT4 Points;
				XMFLOAT3 Point1, Point2;;
				XMStoreFloat3(&Point1, startPoint);
				XMStoreFloat3(&Point2, endPoint);

				tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData.push_back(startPoint);
				tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData.push_back(endPoint);

				//Stores points into vector of primal vert types
				pcGraphicsSystem->StoreBeamPoints(Point1, Point2, atBeamVerts, uvScroll);
				pcGraphicsSystem->UpdateLineVTBuffer(&tThisWorld.atDebugMesh[ExtractionBeamIndex].m_d3dVertexBufferDesc, tThisWorld.atDebugMesh[ExtractionBeamIndex].m_pd3dVertexBuffer, atBeamVerts);

				pcGraphicsSystem->InitLineShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atWorldMatrix[ExtractionBeamIndex].worldMatrix, m_d3dViewMatrix, m_d3dProjectionMatrix, tThisWorld.atDebugMesh[ExtractionBeamIndex], aimCamera->d3d_Position, atBeamVerts, particleSRV);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atDebugMesh[ExtractionBeamIndex].m_nVertexCount, tThisWorld.atGraphicsMask[ExtractionBeamIndex].m_tnGraphicsMask, tThisWorld.atShaderID[ExtractionBeamIndex].m_nShaderID);
			}

#if MUSIC_ON

			if (pcInputSystem->InputCheck(G_KEY_Q) == 0 && pcAudioSystem->m_BeamSoundOn == true)
			{
				pcAudioSystem->m_BeamSoundOn = false;
				}
#endif
#endif
			}

		if ((tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID)
			|| tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_ANIMATION | COMPONENT_SHADERID))
			&& tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK))
		{

			/*for (int currentLightIndex = 0; currentLightIndex < MAX_LIGHTS; currentLightIndex++)
			{
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_d3dLightPosition, &setLights[currentLightIndex].m_d3dLightPosition, sizeof(setLights[currentLightIndex].m_d3dLightPosition));
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_d3dLightColor, &setLights[currentLightIndex].m_d3dLightColor, sizeof(setLights[currentLightIndex].m_d3dLightColor));
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_lightType, &setLights[currentLightIndex].m_lightType, sizeof(setLights[currentLightIndex].m_lightType));
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_Direction, &setLights[currentLightIndex].m_Direction, sizeof(setLights[currentLightIndex].m_Direction));
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].enabled, &setLights[currentLightIndex].enabled, sizeof(setLights[currentLightIndex].enabled));

			}*/
			for (int currentLightIndex = 0; currentLightIndex < MAX_LIGHTS; currentLightIndex++)
			{
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_d3dLightPosition, &setLights[currentLightIndex].m_d3dLightPosition, sizeof(setLights[currentLightIndex].m_d3dLightPosition));
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_d3dLightColor, &setLights[currentLightIndex].m_d3dLightColor, sizeof(setLights[currentLightIndex].m_d3dLightColor));
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_lightType, &setLights[currentLightIndex].m_lightType, sizeof(setLights[currentLightIndex].m_lightType));
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_Direction, &setLights[currentLightIndex].m_Direction, sizeof(setLights[currentLightIndex].m_Direction));
				memcpy(&tLightBuffer.m_allLights[currentLightIndex].enabled, &setLights[currentLightIndex].enabled, sizeof(setLights[currentLightIndex].enabled));

			}
			tLightBuffer.m_Proprties = tThisWorld.atMaterial[nCurrentEntity];

			if (tCameraMode.bWalkMode == true)
			{
				tMyVertexBufferTemp.m_d3dViewMatrix = walkCamera->d3d_Position;
				tTempVertexBuffer.m_d3dViewMatrix = walkCamera->d3d_Position;
			}
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
			if (GamePaused == false && GameOver == false)
			{
				if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK)
					&& PlayerStartIndex == ClaytonIndex)
				{
					tAnimVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
					tAnimVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;

					if (tCameraMode.bWalkMode == true)
					{
						/*m_d3dPlayerMatrix = pcInputSystem->CharacterMovement(m_d3dPlayerMatrix, fpsTimer.GetDelta(), pcAudioSystem, tThisWorld.atClayton[PlayerStartIndex], tThisWorld.atRigidBody[PlayerStartIndex].velocity, bNoMoving);
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix =pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
						m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;

						XMMATRIX claytonFrustumMatrix = walkCamera->d3d_Position;
						claytonFrustumMatrix = XMMatrixMultiply(XMMatrixTranslation(0, 0, -15), claytonFrustumMatrix);

						float4x4 ClaytonFrustum;
						ClaytonFrustum.row1.x = claytonFrustumMatrix.r[0].m128_f32[0];
						ClaytonFrustum.row1.y = claytonFrustumMatrix.r[0].m128_f32[1];
						ClaytonFrustum.row1.z = claytonFrustumMatrix.r[0].m128_f32[2];
						ClaytonFrustum.row1.w = claytonFrustumMatrix.r[0].m128_f32[3];

						ClaytonFrustum.row2.x = claytonFrustumMatrix.r[1].m128_f32[0];
						ClaytonFrustum.row2.y = claytonFrustumMatrix.r[1].m128_f32[1];
						ClaytonFrustum.row2.z = claytonFrustumMatrix.r[1].m128_f32[2];
						ClaytonFrustum.row2.w = claytonFrustumMatrix.r[1].m128_f32[3];

						ClaytonFrustum.row3.x = claytonFrustumMatrix.r[2].m128_f32[0];
						ClaytonFrustum.row3.y = claytonFrustumMatrix.r[2].m128_f32[1];
						ClaytonFrustum.row3.z = claytonFrustumMatrix.r[2].m128_f32[2];
						ClaytonFrustum.row3.w = claytonFrustumMatrix.r[2].m128_f32[3];

						ClaytonFrustum.row4.x = claytonFrustumMatrix.r[3].m128_f32[0];
						ClaytonFrustum.row4.y = claytonFrustumMatrix.r[3].m128_f32[1];
						ClaytonFrustum.row4.z = claytonFrustumMatrix.r[3].m128_f32[2];
						ClaytonFrustum.row4.w = claytonFrustumMatrix.r[3].m128_f32[3];

						pcAiSystem->UpdateFrustum(tThisWorld.atClaytonVision.eyes0, ClaytonFrustum, 60, 1, 0.1, 150);

						tThisWorld.atWorldMatrix[claytonFrustumIndex].worldMatrix = claytonFrustumMatrix;*/
					}
					else if (tCameraMode.bAimMode == true)
					{
						tAnimVertexBuffer.m_d3dViewMatrix = aimCamera->d3d_Position;

						//m_d3dPlayerMatrix = pcInputSystem->CharacterMovement(m_d3dPlayerMatrix, fpsTimer.GetDelta(), pcAudioSystem, tThisWorld.atClayton[PlayerStartIndex], tThisWorld.atRigidBody[PlayerStartIndex].velocity, bNoMoving);
						tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix = m_d3dClaytonMatrix;
						tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[ClaytonIndex], tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix, false);
						m_d3dClaytonMatrix = tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix;
						m_d3dPlayerMatrix = m_d3dClaytonMatrix;
						tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix;
						tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix;

						/*	XMVECTOR temp = XMVectorZero();

							temp = XMVector3Transform(temp, tThisWorld.atWorldMatrix[CaelisIndex].worldMatrix);
							std::cout << "\nCaelis Vector:\t";
							for (int i = 0; i < 3; i++)
							{
								std::cout << temp.m128_f32[i] << ", ";

							}
							temp = XMVector3Transform(temp, tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix);

							std::cout << "\nClayton Vector:\t";
							for (int i = 0; i < 3; i++)
							{
								std::cout << temp.m128_f32[i] << ", ";

							}*/
						XMMATRIX claytonFrustumMatrix = aimCamera->d3d_Position;
						claytonFrustumMatrix = XMMatrixMultiply(XMMatrixTranslation(0, 0, -15), claytonFrustumMatrix);

						float4x4 ClaytonFrustum;
						ClaytonFrustum.row1.x = claytonFrustumMatrix.r[0].m128_f32[0];
						ClaytonFrustum.row1.y = claytonFrustumMatrix.r[0].m128_f32[1];
						ClaytonFrustum.row1.z = claytonFrustumMatrix.r[0].m128_f32[2];
						ClaytonFrustum.row1.w = claytonFrustumMatrix.r[0].m128_f32[3];

						ClaytonFrustum.row2.x = claytonFrustumMatrix.r[1].m128_f32[0];
						ClaytonFrustum.row2.y = claytonFrustumMatrix.r[1].m128_f32[1];
						ClaytonFrustum.row2.z = claytonFrustumMatrix.r[1].m128_f32[2];
						ClaytonFrustum.row2.w = claytonFrustumMatrix.r[1].m128_f32[3];

						ClaytonFrustum.row3.x = claytonFrustumMatrix.r[2].m128_f32[0];
						ClaytonFrustum.row3.y = claytonFrustumMatrix.r[2].m128_f32[1];
						ClaytonFrustum.row3.z = claytonFrustumMatrix.r[2].m128_f32[2];
						ClaytonFrustum.row3.w = claytonFrustumMatrix.r[2].m128_f32[3];

						ClaytonFrustum.row4.x = claytonFrustumMatrix.r[3].m128_f32[0];
						ClaytonFrustum.row4.y = claytonFrustumMatrix.r[3].m128_f32[1];
						ClaytonFrustum.row4.z = claytonFrustumMatrix.r[3].m128_f32[2];
						ClaytonFrustum.row4.w = claytonFrustumMatrix.r[3].m128_f32[3];

						pcAiSystem->UpdateFrustum(tThisWorld.atClaytonVision.eyes0, ClaytonFrustum, 60, 1, 0.1, 150);

						tThisWorld.atWorldMatrix[claytonFrustumIndex].worldMatrix = claytonFrustumMatrix;
					}
					else
					{
						tAnimVertexBuffer.m_d3dViewMatrix = debugCamera->d3d_Position;

						XMMATRIX claytonFrustumMatrix = debugCamera->d3d_Position;
						claytonFrustumMatrix = XMMatrixMultiply(XMMatrixTranslation(0, 0, -15), claytonFrustumMatrix);

						float4x4 ClaytonFrustum;
						ClaytonFrustum.row1.x = claytonFrustumMatrix.r[0].m128_f32[0];
						ClaytonFrustum.row1.y = claytonFrustumMatrix.r[0].m128_f32[1];
						ClaytonFrustum.row1.z = claytonFrustumMatrix.r[0].m128_f32[2];
						ClaytonFrustum.row1.w = claytonFrustumMatrix.r[0].m128_f32[3];

						ClaytonFrustum.row2.x = claytonFrustumMatrix.r[1].m128_f32[0];
						ClaytonFrustum.row2.y = claytonFrustumMatrix.r[1].m128_f32[1];
						ClaytonFrustum.row2.z = claytonFrustumMatrix.r[1].m128_f32[2];
						ClaytonFrustum.row2.w = claytonFrustumMatrix.r[1].m128_f32[3];

						ClaytonFrustum.row3.x = claytonFrustumMatrix.r[2].m128_f32[0];
						ClaytonFrustum.row3.y = claytonFrustumMatrix.r[2].m128_f32[1];
						ClaytonFrustum.row3.z = claytonFrustumMatrix.r[2].m128_f32[2];
						ClaytonFrustum.row3.w = claytonFrustumMatrix.r[2].m128_f32[3];

						ClaytonFrustum.row4.x = claytonFrustumMatrix.r[3].m128_f32[0];
						ClaytonFrustum.row4.y = claytonFrustumMatrix.r[3].m128_f32[1];
						ClaytonFrustum.row4.z = claytonFrustumMatrix.r[3].m128_f32[2];
						ClaytonFrustum.row4.w = claytonFrustumMatrix.r[3].m128_f32[3];

						pcAiSystem->UpdateFrustum(tThisWorld.atClaytonVision.eyes0, ClaytonFrustum, 60, 1, 0.1, 150);

						tThisWorld.atWorldMatrix[claytonFrustumIndex].worldMatrix = claytonFrustumMatrix;
					}

#if JUMPING
					if (tThisWorld.atClayton[nCurrentEntity].jumpTime <= 0)
					{
						tThisWorld.atClayton[nCurrentEntity].jumpCooldown -= fpsTimer.GetDelta();

						if (tThisWorld.atClayton[nCurrentEntity].jumpCooldown <= 0)
						{
							tThisWorld.atClayton[nCurrentEntity].jumpTime = .5;
						}
					}
#endif

					XMFLOAT4X4 * tweenJoints = pcAnimationSystem->PlayAnimation(tThisWorld.atAnimationVariant[nCurrentEntity], tThisWorld.atAnimation[nCurrentEntity], tThisWorld.atAnimation[nCurrentEntity].tTimer.localTime, true);

					for (int i = 0; i < MAX_JOINTS; ++i)
					{
						memcpy(&tAnimVertexBuffer.m_d3dJointsForVS[i], &tweenJoints[i], sizeof(tweenJoints[i]));
					}

					//memcpy(&tThisWorld.atOffSetMatrix[GunIndexForClayton], &tweenJoints[testnumber], sizeof(XMMATRIX));

					if (tCameraMode.bAimMode == true)
					{
						pcGraphicsSystem->InitAnimShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tAnimVertexBuffer, tThisWorld.atMesh[nCurrentEntity],tLightBuffer ,aimCamera->d3d_Position);
					}
					else if (tCameraMode.bDebugMode == true)
					{
						pcGraphicsSystem->InitAnimShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tAnimVertexBuffer, tThisWorld.atMesh[nCurrentEntity], tLightBuffer, debugCamera->d3d_Position);
					}

					tThisWorld.atAnimation[nCurrentEntity].tTimer.GetLocalTime(tThisWorld.atAnimation[nCurrentEntity].tTimer.tSceneTimer, tThisWorld.atAnimation[nCurrentEntity].tTimer.localTime, 2);

					if (bNoMoving)
					{
						tThisWorld.atAnimation[nCurrentEntity].tTimer.localTime = 0;
					}
				}
#if CAELIS
				else if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CAELIS | COMPONENT_INPUTMASK)
					&& PlayerStartIndex == CaelisIndex)
				{    // This pair of if checks set the Camera Matricies
					if (tCameraMode.bWalkMode == true)
					{
						/*m_d3dPlayerMatrix = pcInputSystem->CharacterMovement(m_d3dPlayerMatrix, fpsTimer.GetDelta(), pcAudioSystem, tThisWorld.atClayton[PlayerStartIndex], tThisWorld.atRigidBody[PlayerStartIndex].velocity, bNoMoving);
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix =pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
						m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;

						XMMATRIX claytonFrustumMatrix = walkCamera->d3d_Position;
						claytonFrustumMatrix = XMMatrixMultiply(XMMatrixTranslation(0, 0, -15), claytonFrustumMatrix);

						float4x4 ClaytonFrustum;
						ClaytonFrustum.row1.x = claytonFrustumMatrix.r[0].m128_f32[0];
						ClaytonFrustum.row1.y = claytonFrustumMatrix.r[0].m128_f32[1];
						ClaytonFrustum.row1.z = claytonFrustumMatrix.r[0].m128_f32[2];
						ClaytonFrustum.row1.w = claytonFrustumMatrix.r[0].m128_f32[3];

						ClaytonFrustum.row2.x = claytonFrustumMatrix.r[1].m128_f32[0];
						ClaytonFrustum.row2.y = claytonFrustumMatrix.r[1].m128_f32[1];
						ClaytonFrustum.row2.z = claytonFrustumMatrix.r[1].m128_f32[2];
						ClaytonFrustum.row2.w = claytonFrustumMatrix.r[1].m128_f32[3];

						ClaytonFrustum.row3.x = claytonFrustumMatrix.r[2].m128_f32[0];
						ClaytonFrustum.row3.y = claytonFrustumMatrix.r[2].m128_f32[1];
						ClaytonFrustum.row3.z = claytonFrustumMatrix.r[2].m128_f32[2];
						ClaytonFrustum.row3.w = claytonFrustumMatrix.r[2].m128_f32[3];

						ClaytonFrustum.row4.x = claytonFrustumMatrix.r[3].m128_f32[0];
						ClaytonFrustum.row4.y = claytonFrustumMatrix.r[3].m128_f32[1];
						ClaytonFrustum.row4.z = claytonFrustumMatrix.r[3].m128_f32[2];
						ClaytonFrustum.row4.w = claytonFrustumMatrix.r[3].m128_f32[3];

						pcAiSystem->UpdateFrustum(tThisWorld.atClaytonVision.eyes0, ClaytonFrustum, 60, 1, 0.1, 150);

						tThisWorld.atWorldMatrix[claytonFrustumIndex].worldMatrix = claytonFrustumMatrix;*/
					}
					else if (tCameraMode.bAimMode == true)
					{
						//m_d3dPlayerMatrix = pcInputSystem->CharacterMovement(m_d3dPlayerMatrix, fpsTimer.GetDelta(), pcAudioSystem, tThisWorld.atClayton[PlayerStartIndex], tThisWorld.atRigidBody[PlayerStartIndex].velocity, bNoMoving);
						tThisWorld.atWorldMatrix[CaelisIndex].worldMatrix = m_d3dCaelisMatrix;
						tThisWorld.atWorldMatrix[CaelisIndex].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
						m_d3dCaelisMatrix = tThisWorld.atWorldMatrix[CaelisIndex].worldMatrix;
						m_d3dPlayerMatrix = m_d3dCaelisMatrix;

						tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[CaelisIndex].worldMatrix;
						tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[CaelisIndex].worldMatrix;
						/*XMVECTOR temp = XMVectorZero();

						temp = XMVector3Transform(temp, tThisWorld.atWorldMatrix[CaelisIndex].worldMatrix);
						std::cout << "\nCaelis Vector:\t";
						for (int i = 0; i < 3; i++)
						{
							std::cout << temp.m128_f32[i] << ", ";

						}
						temp = XMVector3Transform(temp, tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix);

						std::cout << "\nClayton Vector:\t";
						for (int i = 0; i < 3; i++)
						{
							std::cout << temp.m128_f32[i] << ", ";

						}*/

						XMMATRIX claytonFrustumMatrix = aimCamera->d3d_Position;
						claytonFrustumMatrix = XMMatrixMultiply(XMMatrixTranslation(0, 0, -15), claytonFrustumMatrix);

						float4x4 ClaytonFrustum;
						ClaytonFrustum.row1.x = claytonFrustumMatrix.r[0].m128_f32[0];
						ClaytonFrustum.row1.y = claytonFrustumMatrix.r[0].m128_f32[1];
						ClaytonFrustum.row1.z = claytonFrustumMatrix.r[0].m128_f32[2];
						ClaytonFrustum.row1.w = claytonFrustumMatrix.r[0].m128_f32[3];

						ClaytonFrustum.row2.x = claytonFrustumMatrix.r[1].m128_f32[0];
						ClaytonFrustum.row2.y = claytonFrustumMatrix.r[1].m128_f32[1];
						ClaytonFrustum.row2.z = claytonFrustumMatrix.r[1].m128_f32[2];
						ClaytonFrustum.row2.w = claytonFrustumMatrix.r[1].m128_f32[3];

						ClaytonFrustum.row3.x = claytonFrustumMatrix.r[2].m128_f32[0];
						ClaytonFrustum.row3.y = claytonFrustumMatrix.r[2].m128_f32[1];
						ClaytonFrustum.row3.z = claytonFrustumMatrix.r[2].m128_f32[2];
						ClaytonFrustum.row3.w = claytonFrustumMatrix.r[2].m128_f32[3];

						ClaytonFrustum.row4.x = claytonFrustumMatrix.r[3].m128_f32[0];
						ClaytonFrustum.row4.y = claytonFrustumMatrix.r[3].m128_f32[1];
						ClaytonFrustum.row4.z = claytonFrustumMatrix.r[3].m128_f32[2];
						ClaytonFrustum.row4.w = claytonFrustumMatrix.r[3].m128_f32[3];

						pcAiSystem->UpdateFrustum(tThisWorld.atClaytonVision.eyes0, ClaytonFrustum, 60, 1, 0.1, 150);

						tThisWorld.atWorldMatrix[claytonFrustumIndex].worldMatrix = claytonFrustumMatrix;
					}
					else
					{
						XMMATRIX caelisFrustumMatrix = debugCamera->d3d_Position;
						caelisFrustumMatrix = XMMatrixMultiply(XMMatrixTranslation(0, 0, -15), caelisFrustumMatrix);

						float4x4 CaelisFrustum;
						CaelisFrustum.row1.x = caelisFrustumMatrix.r[0].m128_f32[0];
						CaelisFrustum.row1.y = caelisFrustumMatrix.r[0].m128_f32[1];
						CaelisFrustum.row1.z = caelisFrustumMatrix.r[0].m128_f32[2];
						CaelisFrustum.row1.w = caelisFrustumMatrix.r[0].m128_f32[3];

						CaelisFrustum.row2.x = caelisFrustumMatrix.r[1].m128_f32[0];
						CaelisFrustum.row2.y = caelisFrustumMatrix.r[1].m128_f32[1];
						CaelisFrustum.row2.z = caelisFrustumMatrix.r[1].m128_f32[2];
						CaelisFrustum.row2.w = caelisFrustumMatrix.r[1].m128_f32[3];

						CaelisFrustum.row3.x = caelisFrustumMatrix.r[2].m128_f32[0];
						CaelisFrustum.row3.y = caelisFrustumMatrix.r[2].m128_f32[1];
						CaelisFrustum.row3.z = caelisFrustumMatrix.r[2].m128_f32[2];
						CaelisFrustum.row3.w = caelisFrustumMatrix.r[2].m128_f32[3];

						CaelisFrustum.row4.x = caelisFrustumMatrix.r[3].m128_f32[0];
						CaelisFrustum.row4.y = caelisFrustumMatrix.r[3].m128_f32[1];
						CaelisFrustum.row4.z = caelisFrustumMatrix.r[3].m128_f32[2];
						CaelisFrustum.row4.w = caelisFrustumMatrix.r[3].m128_f32[3];

						pcAiSystem->UpdateFrustum(tThisWorld.atClaytonVision.eyes0, CaelisFrustum, 60, 1, 0.1, 150);

						tThisWorld.atWorldMatrix[claytonFrustumIndex].worldMatrix = caelisFrustumMatrix;
				}
#endif
#if JUMPING
					if (tThisWorld.atClayton[nCurrentEntity].jumpTime <= 0)
					{
						tThisWorld.atClayton[nCurrentEntity].jumpCooldown -= fpsTimer.GetDelta();

						if (tThisWorld.atClayton[nCurrentEntity].jumpCooldown <= 0)
						{
							tThisWorld.atClayton[nCurrentEntity].jumpTime = 1;
						}
					}
			}
#endif
				//Don't forget to uncomment follow code
				else if ((tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CAELIS | COMPONENT_INPUTMASK) && PlayerStartIndex != CaelisIndex) ||
					tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK) && PlayerStartIndex != ClaytonIndex)
				{
					float distanceFromUser = pcAiSystem->CalculateDistanceMatrix(tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix, tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);

					double tempDelta = fpsTimer.GetDelta();
					double scaleBy = 0.3;

					pcAiSystem->LookAtObject(tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);

					if (distanceFromUser >= 5)
					{
						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, XMMatrixTranslation((tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix.r[3].m128_f32[0] - tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[0]) * tempDelta * scaleBy, (tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix.r[3].m128_f32[1] - tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1]) * tempDelta * scaleBy, (tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix.r[3].m128_f32[2] - tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[2]) * tempDelta * scaleBy));

						/*if (tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1] < -.5)
						{
							tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1] = -.5;
						}*/
					}

					tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
				}
		}

			if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask != (COMPONENT_AIMASK))
			{
				tAnimVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
				for (int currentLightIndex = 0; currentLightIndex < MAX_LIGHTS; currentLightIndex++)
				{
					memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_d3dLightPosition, &setLights[currentLightIndex].m_d3dLightPosition, sizeof(setLights[currentLightIndex].m_d3dLightPosition));
					memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_d3dLightColor, &setLights[currentLightIndex].m_d3dLightColor, sizeof(setLights[currentLightIndex].m_d3dLightColor));
					memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_lightType, &setLights[currentLightIndex].m_lightType, sizeof(setLights[currentLightIndex].m_lightType));
					memcpy(&tLightBuffer.m_allLights[currentLightIndex].m_Direction, &setLights[currentLightIndex].m_Direction, sizeof(setLights[currentLightIndex].m_Direction));
					memcpy(&tLightBuffer.m_allLights[currentLightIndex].enabled, &setLights[currentLightIndex].enabled, sizeof(setLights[currentLightIndex].enabled));

				}
				tLightBuffer.m_Proprties = tThisWorld.atMaterial[nCurrentEntity];

				XMFLOAT4X4 * tweenJoints = pcAnimationSystem->PlayAnimation(tThisWorld.atAnimationVariant[nCurrentEntity], tThisWorld.atAnimation[nCurrentEntity], tThisWorld.atAnimation[nCurrentEntity].tTimer.localTime, false);

				for (int i = 0; i < MAX_JOINTS; ++i)
				{
					memcpy(&tAnimVertexBuffer.m_d3dJointsForVS[i], &tweenJoints[i], sizeof(tweenJoints[i]));
				}

				memcpy(&tThisWorld.atOffSetMatrix[tThisWorld.atAIMask[nCurrentEntity].GunIndex], &tweenJoints[50], sizeof(XMMATRIX));

				if (tCameraMode.bAimMode == true)
				{
					pcGraphicsSystem->InitAnimShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tAnimVertexBuffer, tThisWorld.atMesh[nCurrentEntity], tLightBuffer, aimCamera->d3d_Position);

				}
				else if (tCameraMode.bDebugMode == true)
				{
					pcGraphicsSystem->InitAnimShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tAnimVertexBuffer, tThisWorld.atMesh[nCurrentEntity], tLightBuffer, debugCamera->d3d_Position);

				}

				tThisWorld.atAnimation[nCurrentEntity].tTimer.GetLocalTime(tThisWorld.atAnimation[nCurrentEntity].tTimer.tSceneTimer, tThisWorld.atAnimation[nCurrentEntity].tTimer.localTime, 1);

				if (tThisWorld.atAnimationVariant[nCurrentEntity].tClaytonAnim.animationFinished == true)
				{
					if (tThisWorld.atAnimationVariant[nCurrentEntity].tClaytonAnim.animType == 3)
					{
						tThisWorld.atAnimation[nCurrentEntity].tTimer.localTime = 0;

						tThisWorld.atAnimationVariant[nCurrentEntity].tClaytonAnim.currentFrame = 0;
						tThisWorld.atAnimationVariant[nCurrentEntity].tClaytonAnim.nextFrame = 1;

						tThisWorld.atAnimationVariant[nCurrentEntity].tClaytonAnim.animType = 0;

						tThisWorld.atAnimationVariant[nCurrentEntity].tClaytonAnim.animationFinished = false;
						pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
						tThisWorld.atAABB[nCurrentEntity].m_IndexLocation = -1;
						tThisWorld.atAABB[nCurrentEntity].m_dMaxPoint = XMFLOAT3(0, 0, 0);
						tThisWorld.atAABB[nCurrentEntity].m_dMaxPointOrginal = XMFLOAT3(0, 0, 0);
						tThisWorld.atAABB[nCurrentEntity].m_dMinPoint = XMFLOAT3(0, 0, 0);
						tThisWorld.atAABB[nCurrentEntity].m_dMinPointOrginal = XMFLOAT3(0, 0, 0);

						tThisWorld.atAiHeath[nCurrentEntity].heath = 0;
						tThisWorld.atActiveAI[nCurrentEntity].active = false;
						tThisWorld.atAIVision[nCurrentEntity].stopSearching = true;

						tThisWorld.atClip[nCurrentEntity].tryToShoot = false;
						tThisWorld.atClip[nCurrentEntity].tryToReload = false;

						tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1] -= 10;

					}
					else
					{
						tThisWorld.atAnimation[nCurrentEntity].tTimer.localTime = 0;

						tThisWorld.atAnimationVariant[nCurrentEntity].tClaytonAnim.currentFrame = 0;
						tThisWorld.atAnimationVariant[nCurrentEntity].tClaytonAnim.nextFrame = 1;

						tThisWorld.atAnimationVariant[nCurrentEntity].tClaytonAnim.animType = 0;

						tThisWorld.atAnimationVariant[nCurrentEntity].tClaytonAnim.animationFinished = false;
					}
				}

				if (bNoMoving)
				{
					tThisWorld.atAnimation[nCurrentEntity].tTimer.localTime = 0;
				}

				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}
			else
			{
				if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_ENEMY))
				{
					// changes the color of the enemy bullets
					tMyVertexBufferTemp.m_d3dColor = XMFLOAT4(1, 0, 0, 1);
				}

				if (tCameraMode.bWalkMode == true)
				{
					pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], walkCamera->d3d_Position);
				}
				//Calls Shader Function on Aim Camera If aim mode is active
				else if (tCameraMode.bAimMode == true && nCurrentEntity != ClaytonIndex)
				{
					pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], tLightBuffer, aimCamera->d3d_Position);
				}
				else
				{
					if (nCurrentEntity != ClaytonIndex)
					{
						pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], tLightBuffer, debugCamera->d3d_Position);

					}
				}
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

				if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_ENEMY))
				{
					//changes the color back
					tMyVertexBufferTemp.m_d3dColor = XMFLOAT4(0, 0, 0, 0);
				}
			}

		}

#pragma region Probably need to delete sometime soon
		//if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_ANIMATION | COMPONENT_SHADERID))
		//{
		//	
		//	tAnimVertexBuffer.m_d3dViewMatrix = debugCamera->d3d_Position;
		//	tAnimVertexBuffer.m_d3dProjectionMatrix = m_d3dProjectionMatrix;
		//	if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK)
		//		&& PlayerStartIndex == ClaytonIndex)
		//	{
		//		if (tCameraMode.bWalkMode == true)
		//		{
		//			/*m_d3dPlayerMatrix = pcInputSystem->CharacterMovement(m_d3dPlayerMatrix, fpsTimer.GetDelta(), pcAudioSystem, tThisWorld.atClayton[PlayerStartIndex], tThisWorld.atRigidBody[PlayerStartIndex].velocity, bNoMoving);
		//			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = m_d3dPlayerMatrix;
		//			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix =pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
		//			m_d3dPlayerMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
		//			XMMATRIX claytonFrustumMatrix = walkCamera->d3d_Position;
		//			claytonFrustumMatrix = XMMatrixMultiply(XMMatrixTranslation(0, 0, -15), claytonFrustumMatrix);
		//			float4x4 ClaytonFrustum;
		//			ClaytonFrustum.row1.x = claytonFrustumMatrix.r[0].m128_f32[0];
		//			ClaytonFrustum.row1.y = claytonFrustumMatrix.r[0].m128_f32[1];
		//			ClaytonFrustum.row1.z = claytonFrustumMatrix.r[0].m128_f32[2];
		//			ClaytonFrustum.row1.w = claytonFrustumMatrix.r[0].m128_f32[3];
		//			ClaytonFrustum.row2.x = claytonFrustumMatrix.r[1].m128_f32[0];
		//			ClaytonFrustum.row2.y = claytonFrustumMatrix.r[1].m128_f32[1];
		//			ClaytonFrustum.row2.z = claytonFrustumMatrix.r[1].m128_f32[2];
		//			ClaytonFrustum.row2.w = claytonFrustumMatrix.r[1].m128_f32[3];
		//			ClaytonFrustum.row3.x = claytonFrustumMatrix.r[2].m128_f32[0];
		//			ClaytonFrustum.row3.y = claytonFrustumMatrix.r[2].m128_f32[1];
		//			ClaytonFrustum.row3.z = claytonFrustumMatrix.r[2].m128_f32[2];
		//			ClaytonFrustum.row3.w = claytonFrustumMatrix.r[2].m128_f32[3];
		//			ClaytonFrustum.row4.x = claytonFrustumMatrix.r[3].m128_f32[0];
		//			ClaytonFrustum.row4.y = claytonFrustumMatrix.r[3].m128_f32[1];
		//			ClaytonFrustum.row4.z = claytonFrustumMatrix.r[3].m128_f32[2];
		//			ClaytonFrustum.row4.w = claytonFrustumMatrix.r[3].m128_f32[3];
		//			pcAiSystem->UpdateFrustum(tThisWorld.atClaytonVision.eyes0, ClaytonFrustum, 60, 1, 0.1, 150);
		//		tThisWorld.atWorldMatrix[claytonFrustumIndex].worldMatrix = claytonFrustumMatrix;*/
		//		}
		//		else if (tCameraMode.bAimMode == true)
		//		{
		//			//m_d3dPlayerMatrix = pcInputSystem->CharacterMovement(m_d3dPlayerMatrix, fpsTimer.GetDelta(), pcAudioSystem, tThisWorld.atClayton[PlayerStartIndex], tThisWorld.atRigidBody[PlayerStartIndex].velocity, bNoMoving);
		//			tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix = m_d3dClaytonMatrix;
		//			tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[ClaytonIndex], tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix, false);
		//			m_d3dClaytonMatrix = tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix;
		//			m_d3dPlayerMatrix = m_d3dClaytonMatrix;
		//			tAnimVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix;
		//			tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix;
		//			tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix;
		//			tAnimVertexBuffer.m_d3dViewMatrix = aimCamera->d3d_Position;
		//			XMFLOAT4X4 * tweenJoints = pcAnimationSystem->PlayAnimation(tThisWorld.atAnimationVariant[nCurrentEntity], tThisWorld.atAnimation[nCurrentEntity], tThisWorld.atAnimation[ClaytonIndex].tTimer.localTime, true);
		//			for (int i = 0; i < 63; ++i)
		//			{
		//				memcpy(&tAnimVertexBuffer.m_d3dJointsForVS[i], &tweenJoints[i], sizeof(tweenJoints[i]));
		//			}
		//			/*	XMVECTOR temp = XMVectorZero();
		//				temp = XMVector3Transform(temp, tThisWorld.atWorldMatrix[CaelisIndex].worldMatrix);
		//				std::cout << "\nCaelis Vector:\t";
		//				for (int i = 0; i < 3; i++)
		//				{
		//					std::cout << temp.m128_f32[i] << ", ";
		//				}
		//				temp = XMVector3Transform(temp, tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix);
		//				std::cout << "\nClayton Vector:\t";
		//				for (int i = 0; i < 3; i++)
		//				{
		//					std::cout << temp.m128_f32[i] << ", ";
					/*TLights* Lights_To_Send = pcGraphicsSystem->GetLights();
					for (int k = 0; k < MAX_LIGHTS; k++)
					{
						memcpy(&tLightBuffer.m_allLights[k], &Lights_To_Send[k], sizeof(Lights_To_Send[k]));
					}
					tLightBuffer.m_Proprties = tThisWorld.atMaterial[nCurrentEntity];*/
					/*TLights testLight;
					testLight.m_d3dLightColor = XMFLOAT4(1, 0, 0, 0);
					testLight.m_d3dLightPosition = XMFLOAT4(-4, 4, 10.8f, 1);
					testLight.enabled = true;
					testLight.m_lightType = 1;
					testLight.m_Direction = XMFLOAT4(0, 0, 0, 1);
					testLight.m_padding = XMFLOAT2(0, 0);

					tLightBuffer.m_allLights = testLight;
					tLightBuffer.m_Proprties = tThisWorld.atMaterial[nCurrentEntity];
					tLightBuffer.lightEyePos = XMFLOAT4(-4, 4, 10.8f, 1);
					tLightBuffer.Ambience = XMFLOAT4(0.5f, 0.5f, 0.5f, 1);*/
		//			XMMATRIX claytonFrustumMatrix = aimCamera->d3d_Position;
		//			claytonFrustumMatrix = XMMatrixMultiply(XMMatrixTranslation(0, 0, -15), claytonFrustumMatrix);
		//			float4x4 ClaytonFrustum;
		//			ClaytonFrustum.row1.x = claytonFrustumMatrix.r[0].m128_f32[0];
		//			ClaytonFrustum.row1.y = claytonFrustumMatrix.r[0].m128_f32[1];
		//			ClaytonFrustum.row1.z = claytonFrustumMatrix.r[0].m128_f32[2];
		//			ClaytonFrustum.row1.w = claytonFrustumMatrix.r[0].m128_f32[3];
		////			ClaytonFrustum.row2.x = claytonFrustumMatrix.r[1].m128_f32[0];
		////			ClaytonFrustum.row2.y = claytonFrustumMatrix.r[1].m128_f32[1];
		////			ClaytonFrustum.row2.z = claytonFrustumMatrix.r[1].m128_f32[2];
		////			ClaytonFrustum.row2.w = claytonFrustumMatrix.r[1].m128_f32[3];
		////			ClaytonFrustum.row3.x = claytonFrustumMatrix.r[2].m128_f32[0];
		////			ClaytonFrustum.row3.y = claytonFrustumMatrix.r[2].m128_f32[1];
		////			ClaytonFrustum.row3.z = claytonFrustumMatrix.r[2].m128_f32[2];
		////			ClaytonFrustum.row3.w = claytonFrustumMatrix.r[2].m128_f32[3];
		////			ClaytonFrustum.row4.x = claytonFrustumMatrix.r[3].m128_f32[0];
		////			ClaytonFrustum.row4.y = claytonFrustumMatrix.r[3].m128_f32[1];
		////			ClaytonFrustum.row4.z = claytonFrustumMatrix.r[3].m128_f32[2];
		////			ClaytonFrustum.row4.w = claytonFrustumMatrix.r[3].m128_f32[3];
		////			pcAiSystem->UpdateFrustum(tThisWorld.atClaytonVision.eyes0, ClaytonFrustum, 60, 1, 0.1, 150);
		//			tThisWorld.atWorldMatrix[claytonFrustumIndex].worldMatrix = claytonFrustumMatrix;
		//			pcGraphicsSystem->InitAnimShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tAnimVertexBuffer, tThisWorld.atMesh[nCurrentEntity], aimCamera->d3d_Position);
		////		}
		////		else
		////		{
		////			XMMATRIX claytonFrustumMatrix = debugCamera->d3d_Position;
		////			claytonFrustumMatrix = XMMatrixMultiply(XMMatrixTranslation(0, 0, -15), claytonFrustumMatrix);
		////			XMFLOAT4X4 * tweenJoints = pcAnimationSystem->PlayAnimation(tThisWorld.atAnimationVariant[nCurrentEntity], tThisWorld.atAnimation[nCurrentEntity], tThisWorld.atAnimation[ClaytonIndex].tTimer.localTime, true);
		////			for (int i = 0; i < 63; ++i)
		////			{
		////				memcpy(&tAnimVertexBuffer.m_d3dJointsForVS[i], &tweenJoints[i], sizeof(tweenJoints[i]));
		////			}
		////			tAnimVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix;
		////			tTempVertexBuffer.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix;
		////			tMyVertexBufferTemp.m_d3dWorldMatrix = tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix;
		////			tAnimVertexBuffer.m_d3dViewMatrix = debugCamera->d3d_Position;
		//			/*TLights* Lights_To_Send = pcGraphicsSystem->GetLights();
		//			for (int k = 0; k < MAX_LIGHTS; k++)
		//			{
		//				memcpy(&tLightBuffer.m_allLights[k], &Lights_To_Send[k], sizeof(Lights_To_Send[k]));
		//			}
		//			tLightBuffer.m_Proprties = tThisWorld.atMaterial[nCurrentEntity];*/
		//			/*TLights testLight;
		//			testLight.m_d3dLightColor = XMFLOAT4(1, 0, 0, 0);
		//			testLight.m_d3dLightPosition = XMFLOAT4(-4, 4, 10.8f, 1);
		//			testLight.enabled = true;
		//			testLight.m_lightType = 1;
		//			testLight.m_Direction = XMFLOAT4(0, 0, 0, 1);
		//			testLight.m_padding = XMFLOAT2(0, 0);*/
		//			//tLightBuffer.m_allLights = testLight;
		//			tLightBuffer.m_Proprties = tThisWorld.atMaterial[nCurrentEntity];
		//			tLightBuffer.lightEyePos = XMFLOAT4(-4, 4, 10.8f, 1);
		//			tLightBuffer.Ambience = XMFLOAT4(0.5f, 0.5f, 0.5f, 1);
		//			float4x4 ClaytonFrustum;
		//			ClaytonFrustum.row1.x = claytonFrustumMatrix.r[0].m128_f32[0];
		//			ClaytonFrustum.row1.y = claytonFrustumMatrix.r[0].m128_f32[1];
		//			ClaytonFrustum.row1.z = claytonFrustumMatrix.r[0].m128_f32[2];
		//			ClaytonFrustum.row1.w = claytonFrustumMatrix.r[0].m128_f32[3];
		////			ClaytonFrustum.row2.x = claytonFrustumMatrix.r[1].m128_f32[0];
		////			ClaytonFrustum.row2.y = claytonFrustumMatrix.r[1].m128_f32[1];
		////			ClaytonFrustum.row2.z = claytonFrustumMatrix.r[1].m128_f32[2];
		////			ClaytonFrustum.row2.w = claytonFrustumMatrix.r[1].m128_f32[3];
		////			ClaytonFrustum.row3.x = claytonFrustumMatrix.r[2].m128_f32[0];
		////			ClaytonFrustum.row3.y = claytonFrustumMatrix.r[2].m128_f32[1];
		////			ClaytonFrustum.row3.z = claytonFrustumMatrix.r[2].m128_f32[2];
		////			ClaytonFrustum.row3.w = claytonFrustumMatrix.r[2].m128_f32[3];
		////			ClaytonFrustum.row4.x = claytonFrustumMatrix.r[3].m128_f32[0];
		////			ClaytonFrustum.row4.y = claytonFrustumMatrix.r[3].m128_f32[1];
		////			ClaytonFrustum.row4.z = claytonFrustumMatrix.r[3].m128_f32[2];
		////			ClaytonFrustum.row4.w = claytonFrustumMatrix.r[3].m128_f32[3];
		////			pcAiSystem->UpdateFrustum(tThisWorld.atClaytonVision.eyes0, ClaytonFrustum, 60, 1, 0.1, 150);
		//			tThisWorld.atWorldMatrix[claytonFrustumIndex].worldMatrix = claytonFrustumMatrix;
		//			pcGraphicsSystem->InitAnimShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tAnimVertexBuffer, tThisWorld.atMesh[nCurrentEntity], tLightBuffer, debugCamera->d3d_Position);
		//		}
		//		tThisWorld.atAnimation[ClaytonIndex].tTimer.GetLocalTime(tThisWorld.atAnimation[ClaytonIndex].tTimer.tSceneTimer, tThisWorld.atAnimation[ClaytonIndex].tTimer.localTime);
		//		tThisWorld.atAnimation[ClaytonIndex].tTimer.DisplayTimes(pcInputSystem);
		//	}
		//	pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		//}
#pragma endregion
		// ai code would run here
		if (pcAiSystem->GetNumberOfAI() <= 0 && !endInit)
		{
			GameOver = true;
			InitializeEndScreen(true);
			endInit = true;
		}
		if (pcAiSystem->ShootingActiveAI.size()>=1&& tThisWorld.atActiveAI[tThisWorld.atClip[ pcAiSystem->GetActiveShooter()].gunHolder].active == false) {
			pcAiSystem->SetActiveShooter(tThisWorld.atAIMask[pcAiSystem->ChooseRandomSHooter()].GunIndex);
			tThisWorld.atClip[pcAiSystem->GetActiveShooter()].fShootingCoolDown = 200;
			tThisWorld.atClip[pcAiSystem->GetActiveShooter()].tryToShoot = true;
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
					//	pcAiSystem->ShootGun(&tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex]);
					pcAiSystem->AddShootingActiveAI(nCurrentEntity);
					if (pcAiSystem->GetCanWechooseShooter() == true) 
					{
						//	tThisWorld.atClip[tThisWorld.atAIMask[pcAiSystem->ChooseRandomSHooter()].GunIndex].fsReloadingCoolDown=100;
						pcAiSystem->SetActiveShooter(tThisWorld.atAIMask[pcAiSystem->ChooseRandomSHooter()].GunIndex);
						tThisWorld.atClip[pcAiSystem->GetActiveShooter()].fShootingCoolDown = 200;
						tThisWorld.atClip[pcAiSystem->GetActiveShooter()].tryToShoot = true;
					}
					else {
						if (
							pcAiSystem->ActiveShooterCheck(tThisWorld.atAIMask[nCurrentEntity].GunIndex, tThisWorld.atClip[pcAiSystem->GetActiveShooter()].fShootingCoolDown
							) == true) 
						{
							tThisWorld.atClip[pcAiSystem->GetActiveShooter()].tryToShoot = true;
						}
					}
				}
#endif // AI_ON
				/*	return sqrtf(
		((_search->pos.x - goal->pos.x)*(_search->pos.x - goal->pos.x)) +
		((_search->pos.y - goal->pos.y)*(_search->pos.y - goal->pos.y)) +
		((_search->pos.z - goal->pos.z)*(_search->pos.z - goal->pos.z))
	);*/
				if (tThisWorld.atActiveAI[nCurrentEntity].active == true && pcAiSystem->CalculateDistanceMatrix(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atWorldMatrix[tThisWorld.atAIVision[nCurrentEntity].indexLookingAt].worldMatrix) > 30) {
					//tThisWorld.atActiveAI[nCurrentEntity].active = false;
					//tThisWorld.atAIVision[nCurrentEntity].keepSearching = true;
				}
			}
		}

		if (GamePaused == false && GameOver == false)
		{
			if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SEARCH)
				|| tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SPOTEDPLAYER)
				|| tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST))
			{

				if (tThisWorld.atAIVision[nCurrentEntity].keepSearching == true)
				{
					if (tThisWorld.atAIVision[nCurrentEntity].visionRotation < 7
						&& tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true)
					{

						tThisWorld.atAIVision[nCurrentEntity].visionRotation += 0.1;
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


						tThisWorld.atAIVision[nCurrentEntity].visionRotation -= 0.1;
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
					//	pcAiSystem->LookAtObject(tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
				}

				pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);

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
				if (pcCollisionSystem->AiVisionCheck(tThisWorld.atAIVision[nCurrentEntity].eyes0, &indicies) == true && tThisWorld.atAIVision[nCurrentEntity].stopSearching == false)
				{
					bool danger = false;
					for (int i = 0; i < indicies.size(); ++i)
					{
						if (PlayerStartIndex == indicies[i])
						{
							tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
							tThisWorld.atAIVision[nCurrentEntity].keepSearching = false;
							tThisWorld.atActiveAI[nCurrentEntity].active = true;
							tThisWorld.atAIVision[nCurrentEntity].indexLookingAt = PlayerStartIndex;
							danger = true;
							pcAiSystem->AddShootingActiveAI(nCurrentEntity);
							if (tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].nBulletsAvailables.size() <= 0)
							{
								tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].tryToReload = true;

							}
							for (int CurrentAIINdex = 0; CurrentAIINdex < tThisWorld.atActiveAI[nCurrentEntity].NoctifyOtherAi.size(); ++CurrentAIINdex)
							{

								tThisWorld.atSimpleMesh[CurrentAIINdex].m_nColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
								tThisWorld.atAIVision[CurrentAIINdex].keepSearching = false;
								tThisWorld.atActiveAI[CurrentAIINdex].active = true;
								tThisWorld.atAIVision[CurrentAIINdex].indexLookingAt = PlayerStartIndex;
								if (tThisWorld.atClip[tThisWorld.atAIMask[CurrentAIINdex].GunIndex].nBulletsAvailables.size() <= 0)
								{
									tThisWorld.atClip[tThisWorld.atAIMask[CurrentAIINdex].GunIndex].tryToReload = true;

								}
							}
#if AI_ON
							//	tThisWorld.atClip[tThisWorld.atAIMask[nCurrentEntity].GunIndex].tryToShoot = true;

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
							XMVECTOR playerGravity;
							playerGravity.m128_f32[1] = -0.01f;
							playerGravity.m128_f32[0] = 0;
							playerGravity.m128_f32[2] = 0;
							playerGravity.m128_f32[3] = 0;
							//	tThisWorld.atRigidBody[nCurrentEntity].gravity = playerGravity;
								/*cout << tThisWorld.atPathPlanining[nCurrentEntity].startingNode<<"start"<<std::endl;
								cout << tThisWorld.atPathPlanining[nCurrentEntity].Goal<<"goal"<<std::endl;

								cout << "AITRYINGTOMOVE" << nCurrentEntity << std::endl;
								cout << tThisWorld.atPathPlanining[nCurrentEntity].pauseMovement;*/
							if (tThisWorld.atPathPlanining[nCurrentEntity].startingNode != tThisWorld.atPathPlanining[nCurrentEntity].Goal&&tThisWorld.atPathPlanining[nCurrentEntity].pauseMovement == false)
								pcAiSystem->PathPlaningMovement(&tThisWorld.atPathPlanining[nCurrentEntity], &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, fpsTimer.GetDelta());
							else {
								//	cout << " penguin " << std::endl;
							}
						}
						else
						{
							tThisWorld.atPathPlanining[nCurrentEntity].DelayMovement -= 0.1f;
						}
					}
				}
			}
		}
		//Gun Clips here for player - ZFB
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
							int textSize = 2;
							wchar_t* textBuffer = new wchar_t[textSize];

							if (tThisWorld.atClip[nCurrentEntity].nBulletsAvailables.size() - 1 <= 9)
							{
								textBuffer[0] = 0;
								textBuffer[1] = (tThisWorld.atClip[nCurrentEntity].nBulletsAvailables.size() - 1);
							}
							else
							{
								textBuffer[0] = (int)(((tThisWorld.atClip[nCurrentEntity].nBulletsAvailables.size() - 1) * .1) + 1);
								textBuffer[1] = (tThisWorld.atClip[nCurrentEntity].nBulletsAvailables.size() - 1) % 10;
							}

							pcUISystem->UpdateText(&tThisWorld, pcGraphicsSystem, ammoIndex, &atUIVertices, textBuffer, textSize, atUIVertices.at(tThisWorld.atLabel[ammoIndex].vIndex));

							delete[] textBuffer;
						}

						XMVECTOR foward;
						/*foward.m128_f32[0] = 0;
						foward.m128_f32[1] = 0;
						foward.m128_f32[2] = 1;*/

						XMMATRIX localMatrix2;
						XMMATRIX gunMatrix;

						int bulletType = -1;
						// Metal Fired Sound in Here -ZFB
						if (nCurrentEntity == GunIndexForClayton)
						{
							pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);

							foward.m128_f32[0] = 0;
							foward.m128_f32[1] = 0;
							foward.m128_f32[2] = 1;

							localMatrix2 = XMMatrixTranslationFromVector(foward);
							gunMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
							gunMatrix = XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(-90)), gunMatrix);
							gunMatrix = XMMatrixMultiply(localMatrix2, gunMatrix);
#if MUSIC_ON
							pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_METAL_FIRED, pcAudioSystem->m_AkMetalFired);
							pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);
#endif

							bulletType = 0;
#pragma region Mesh Bullet For Player
							int newbullet = -1;
							for (int meshIndex = 0; meshIndex < bulletMesh.meshCount; ++meshIndex)
							{
								newbullet = CreateBulletMesh(&tThisWorld, gunMatrix, bulletToCopyFrom);
						//		pcParticleSystem->CreateAlotofCubes(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, &tThisWorld, 100, pcGraphicsSystem, pcAiSystem, fpsTimer.GetDelta());
							
								//tThisWorld.atWorldMatrix[box].
								if (materialGunProjectileSRV != nullptr)
								{
									tThisWorld.atMesh[newbullet].m_d3dSRVDiffuse = materialGunProjectileSRV;
								}
							//	pcParticleSystem->CreateAlotofCubes(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, &tThisWorld, 100, pcGraphicsSystem, pcAiSystem);

								//newbullet = CreateBulletMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, gunMatrix, tThisWorld.atClip[nCurrentEntity].currentMaterial, bulletType, bulletMesh.vtMeshes[meshIndex], bulletMesh.vtMaterials[meshIndex]);
							}
							tThisWorld.atClip[newbullet].gunIndex = nCurrentEntity;
							tThisWorld.atSimpleMesh[newbullet].m_nColor = tThisWorld.atClip[nCurrentEntity].colorofBullets;
							tThisWorld.atClip[newbullet].indexInclip = pcProjectileSystem->CreateBulletProjectile(newbullet, &tThisWorld.atClip[nCurrentEntity]);
							tThisWorld.atAABB[newbullet] = pcCollisionSystem->createAABBS(tThisWorld.atMesh[newbullet].m_VertexData, tThisWorld.atAABB[newbullet]);
							tThisWorld.atAABB[newbullet].m_IndexLocation = newbullet;

							tThisWorld.atClip[newbullet].maxLifeTime = 2.4;
							tThisWorld.atAABB[newbullet].locationinArray = pcCollisionSystem->AddAABBCollider(tThisWorld.atAABB[newbullet], newbullet);
							pcCollisionSystem->AddAABBCollider(tThisWorld.atAABB[newbullet], newbullet);
							pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, newbullet);
						

							//TMaterialOptimized matOpt;
							/*
							Notes on Material gun Concept:
							1. save srv value or material to keep a constant texture and then in extract method you can update the value
							2. create the bullet
							*/
							/*int newbullet;
							matOpt = pcGraphicsSystem->CreateTexturesFromFile(bulletMesh.vtMaterials, bulletMesh.meshCount);
							for (int meshIndex = 0; meshIndex < bulletMesh.meshCount; meshIndex++)
							{
								 newbullet = CreateMaterialBullet(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, bulletMesh.vtMeshes[meshIndex], matOpt,gunMatrix, meshIndex, bulletType);
							}
							tThisWorld.atClip[newbullet].gunIndex = nCurrentEntity;
							tThisWorld.atClip[newbullet].indexInclip = pcProjectileSystem->CreateBulletProjectile(newbullet, &tThisWorld.atClip[nCurrentEntity]);
							tThisWorld.atAABB[newbullet] = pcCollisionSystem->createAABBS(tThisWorld.atMesh[newbullet].m_VertexData, tThisWorld.atAABB[newbullet]);
							tThisWorld.atAABB[newbullet].m_IndexLocation = newbullet;
							pcCollisionSystem->AddAABBCollider(tThisWorld.atAABB[newbullet], newbullet);
							pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, newbullet);*/
#pragma endregion        
						}
#if CAELIS
						else if (nCurrentEntity == GunIndexForCaelis)
						{
							pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);

							foward.m128_f32[0] = 0;
							foward.m128_f32[1] = 0;
							foward.m128_f32[2] = 1;

							localMatrix2 = XMMatrixTranslationFromVector(foward);
							gunMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
							gunMatrix = XMMatrixMultiply(XMMatrixRotationX(XMConvertToRadians(90)), gunMatrix);
							gunMatrix = XMMatrixMultiply(localMatrix2, gunMatrix);

							bulletType = 0;

							int newbullet = -1;
							for (int meshIndex = 0; meshIndex < bulletMesh.meshCount; ++meshIndex)
							{
								newbullet = CreateBulletMesh(&tThisWorld, gunMatrix, bulletToCopyFrom);
								if (materialGunProjectileSRV != nullptr)
								{
									tThisWorld.atMesh[newbullet].m_d3dSRVDiffuse = materialGunProjectileSRV;
								}
							}
							tThisWorld.atClip[newbullet].gunIndex = nCurrentEntity;
							tThisWorld.atSimpleMesh[newbullet].m_nColor = tThisWorld.atClip[nCurrentEntity].colorofBullets;
							tThisWorld.atClip[newbullet].indexInclip = pcProjectileSystem->CreateBulletProjectile(newbullet, &tThisWorld.atClip[nCurrentEntity]);
							tThisWorld.atAABB[newbullet] = pcCollisionSystem->createAABBS(tThisWorld.atMesh[newbullet].m_VertexData, tThisWorld.atAABB[newbullet]);
							tThisWorld.atAABB[newbullet].m_IndexLocation = newbullet;

							tThisWorld.atClip[newbullet].maxLifeTime = 2.4;

							tThisWorld.atAABB[newbullet].locationinArray=pcCollisionSystem->AddAABBCollider(tThisWorld.atAABB[newbullet], newbullet);
							pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[newbullet].worldMatrix, tThisWorld.atAABB[newbullet]);
							pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, newbullet);
						}
#endif
						else
						{
							if (tThisWorld.atAnimationVariant[tThisWorld.atParentWorldMatrix[nCurrentEntity]].tClaytonAnim.animType != 1)
							{
								tThisWorld.atAnimation[tThisWorld.atParentWorldMatrix[nCurrentEntity]].tTimer.localTime = 0;

								tThisWorld.atAnimationVariant[tThisWorld.atParentWorldMatrix[nCurrentEntity]].tClaytonAnim.currentFrame = 0;
								tThisWorld.atAnimationVariant[tThisWorld.atParentWorldMatrix[nCurrentEntity]].tClaytonAnim.nextFrame = 1;

								tThisWorld.atAnimationVariant[tThisWorld.atParentWorldMatrix[nCurrentEntity]].tClaytonAnim.animType = 1;

								tThisWorld.atAnimationVariant[tThisWorld.atParentWorldMatrix[nCurrentEntity]].tClaytonAnim.animationFinished = false;
							}

							pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);

							foward.m128_f32[0] = 0;
							foward.m128_f32[1] = 0;
							foward.m128_f32[2] = 1;

							localMatrix2 = XMMatrixTranslationFromVector(foward);
							gunMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;
							gunMatrix = XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(90)), gunMatrix);
							gunMatrix = XMMatrixMultiply(localMatrix2, gunMatrix);
							//Laser Fire sound is here - ZFB
#if MUSIC_ON
							pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_LASER_FIRE, pcAudioSystem->m_Laser_Fire);
							pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);
#endif
							bulletType = 1;
							int newbullet;
							for (int meshIndex = 0; meshIndex < bulletMesh.meshCount; ++meshIndex)
							{
								newbullet = CreateBulletMesh(&tThisWorld, gunMatrix, bulletToCopyFrom, bulletType);
								//newbullet = CreateBulletMesh(&tThisWorld, pcGraphicsSystem->m_pd3dDevice, gunMatrix, tThisWorld.atClip[nCurrentEntity].currentMaterial, bulletType, bulletMesh.vtMeshes[meshIndex], bulletMesh.vtMaterials[meshIndex]);
								tThisWorld.atPathPlanining[tThisWorld.atClip[nCurrentEntity].gunHolder].pauseMovement = true;
								cout << tThisWorld.atPathPlanining[tThisWorld.atClip[nCurrentEntity].gunHolder].movementPausedTimer << std::endl;
								tThisWorld.atPathPlanining[tThisWorld.atClip[nCurrentEntity].gunHolder].movementPausedTimer = 100;

							}
							//newbullet = CreateBullet(&tThisWorld, gunMatrix, tThisWorld.atClip[nCurrentEntity].currentMaterial, bulletType);
							tThisWorld.atClip[newbullet].gunIndex = nCurrentEntity;
							tThisWorld.atSimpleMesh[newbullet].m_nColor = tThisWorld.atClip[nCurrentEntity].colorofBullets;
							tThisWorld.atClip[newbullet].indexInclip = pcProjectileSystem->CreateBulletProjectile(newbullet, &tThisWorld.atClip[nCurrentEntity]);
							tThisWorld.atAABB[newbullet] = pcCollisionSystem->createAABBS(tThisWorld.atSimpleMesh[newbullet].m_VertexData, tThisWorld.atAABB[newbullet]);
							tThisWorld.atAABB[newbullet].m_IndexLocation = newbullet;

							tThisWorld.atClip[newbullet].maxLifeTime = 2.4;
							tThisWorld.atAABB[newbullet].locationinArray = pcCollisionSystem->AddAABBCollider(tThisWorld.atAABB[newbullet], newbullet);
							pcCollisionSystem->AddAABBCollider(tThisWorld.atAABB[newbullet], newbullet);
							pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, newbullet);
						}
					}
					else if (tThisWorld.atClip[nCurrentEntity].tryToShoot == true)
					{
						tThisWorld.atClip[nCurrentEntity].tryToShoot = false;
#if MUSIC_ON
						if (tThisWorld.atClip[GunIndexForPlayer].nBulletsAvailables.size() == 0 && pcInputSystem->InputCheck(G_BUTTON_LEFT) == 1)
						{
							//Remember to ask chris about wwise property to check when sound is done playing
							pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_AMMODEPLETED, pcAudioSystem->m_GunEmpty);
							pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);
							tThisWorld.atClip[GunIndexForPlayer].empty = false;

							if (tThisWorld.atClip[GunIndexForPlayer].fShootingCoolDown <= 0)
							{
								tThisWorld.atLabel[gunImageIndex].color = XMFLOAT4(.5, 0, 0, 1);
							}
						}
						/*else if(tThisWorld.atClip[GunIndexForPlayer].empty == false)
						{
							tThisWorld.atClip[GunIndexForPlayer].empty = true;
						}*/
#endif
					}
					if (tThisWorld.atClip[nCurrentEntity].tryToReload == true)
					{
						//Reload Metal Sound - ZFB
#if MUSIC_ON
						if (tThisWorld.atClip[GunIndexForPlayer].nBulletsAvailables.size() < 8)
						{
							pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_METAL_RELOAD, pcAudioSystem->m_MetalReload);
							pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);
						}

#endif
						pcProjectileSystem->Reload(&tThisWorld.atClip[nCurrentEntity]);
						tThisWorld.atClip[nCurrentEntity].tryToReload = false;

						if (nCurrentEntity == GunIndexForPlayer)
						{
							int textSize = 2;
							wchar_t* textBuffer = new wchar_t[textSize];

							if (tThisWorld.atClip[nCurrentEntity].nBulletsAvailables.size() <= 9)
							{
								textBuffer[0] = 0;
								textBuffer[1] = tThisWorld.atClip[nCurrentEntity].nBulletsAvailables.size();
							}
							else
							{
								textBuffer[0] = tThisWorld.atClip[nCurrentEntity].nBulletsAvailables.size() * .1;
								textBuffer[1] = tThisWorld.atClip[nCurrentEntity].nBulletsAvailables.size() % 10;
							}

							pcUISystem->UpdateText(&tThisWorld, pcGraphicsSystem, ammoIndex, &atUIVertices, textBuffer, textSize, atUIVertices.at(tThisWorld.atLabel[ammoIndex].vIndex));

							delete[] textBuffer;
						}
					}



				}
				if (tThisWorld.atClip[nCurrentEntity].fShootingCoolDown > 0)
				{
					tThisWorld.atClip[nCurrentEntity].fShootingCoolDown -= fpsTimer.GetDelta() * 100;
				}

				if (tThisWorld.atPathPlanining[tThisWorld.atClip[nCurrentEntity].gunHolder].movementPausedTimer > 0) {
					tThisWorld.atPathPlanining[tThisWorld.atClip[nCurrentEntity].gunHolder].movementPausedTimer -= fpsTimer.GetDelta() * 100;
				}
				else {
					tThisWorld.atPathPlanining[tThisWorld.atClip[nCurrentEntity].gunHolder].pauseMovement = false;
				}
			}
		}
		if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_FRIENDLY) ||
			tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_ENEMY))
		{
			//ADD FORCE TO EVERY BULLET
		/*	pcPhysicsSystem->AddBulletForce(&tThisWorld.atRigidBody[nCurrentEntity], fpsTimer.GetDelta() * 1.5);

			tThisWorld.atClip[nCurrentEntity].lifeTime += fpsTimer.GetDelta();*/

			if (tThisWorld.atClip[nCurrentEntity].lifeTime > tThisWorld.atClip[nCurrentEntity].maxLifeTime)
			{
				pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);

				pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);
			}
		}
		if (GamePaused == false && GameOver == false&&tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask<=1)
		{
			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcPhysicsSystem->ResolveForces(&tThisWorld.atRigidBody[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, true);
		}
		if (GamePaused == false && GameOver == false)
		{

			// bullet check 
			if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_RAYGUN))
			{
				float CloseEstObject = 10000000000000000000.0f;
				float distanceCalucaltion = 0;
				XMVECTOR gunVector = XMVector3Transform(XMVectorZero(), tThisWorld.atWorldMatrix[GunIndexForClayton].worldMatrix);
				XMVECTOR intersectVector = XMVectorZero();
				XMVECTOR diff = XMVectorZero();

				//ptr is the collided entity index compared to current entit index. - ZFB
				for (list<TAABB>::iterator ptr = pcCollisionSystem->m_AAbb.begin(); ptr != pcCollisionSystem->m_AAbb.end(); ++ptr)
				{
					if (ptr->m_IndexLocation != PlayerStartIndex && ptr->m_IndexLocation != GunIndexForClayton)
					{
						if (pcCollisionSystem->intersectRayAABox2(
							XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[0],
								tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix),
							XMVector3Transform(tThisWorld.atDebugMesh[nCurrentEntity].m_VertexData[1],
								tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix), *ptr
						) == true)
						{
							if (tThisWorld.atMesh[ptr->m_IndexLocation].m_VertexData.size())
							{
								intersectVector = XMLoadFloat3(&(tThisWorld.atMesh[ptr->m_IndexLocation].m_VertexData[0]));
								intersectVector = XMVector3Transform(intersectVector, tThisWorld.atWorldMatrix[ptr->m_IndexLocation].worldMatrix);
								std::cout << "Gun Vector:\t";
								for (int i = 0; i < 3; i++)
								{
									std::cout << gunVector.m128_f32[i] << ", ";
								}
								std::cout << "\n";

								std::cout << "Intersect Vector:\t";
								for (int i = 0; i < 3; i++)
								{
									std::cout << intersectVector.m128_f32[i] << ", ";
								}
								std::cout << "\n";

								distanceCalucaltion = sqrt((gunVector.m128_f32[0] - intersectVector.m128_f32[0]) * (gunVector.m128_f32[0] - intersectVector.m128_f32[0]) +
									(gunVector.m128_f32[1] - intersectVector.m128_f32[1]) * (gunVector.m128_f32[1] - intersectVector.m128_f32[1]) +
									(gunVector.m128_f32[2] - intersectVector.m128_f32[2]) * (gunVector.m128_f32[2] - intersectVector.m128_f32[2]));
								std::cout << "Intersected Entity #:\t" << ptr->m_IndexLocation << "\n\tDistance From Gun:\t" << distanceCalucaltion << "\n";
								if (CloseEstObject > distanceCalucaltion)
								{
									CloseEstObject = distanceCalucaltion;
									materialGunProjectileSRV = tThisWorld.atMesh[ptr->m_IndexLocation].m_d3dSRVDiffuse;
									//Make Load Sound
								}
							}
						}

					}
				}
			}
		}

		if (GamePaused == false && GameOver == false)
		{

			if (nCurrentEntity == 962)
			{
				float x = 0;
			}
			if (tThisWorld.atAABB[nCurrentEntity].theeadmade == false
				&& (nCurrentEntity == ClaytonIndex || nCurrentEntity == CaelisIndex
					|| (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask > 1 && tThisWorld.atActiveAI[nCurrentEntity].active == true)
					|| tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask > 1))
			{
				/*thread newthread(&CCollisionSystem::TestThreading, pcCollisionSystem, &tThisWorld, nCurrentEntity, pcGraphicsSystem, &tTempVertexBuffer, &m_d3dPlayerMatrix, pcPhysicsSystem);
				newthread.detach();*/
				XMMATRIX(*doorEventListenerPointer)(int, bool) = DoorEventListener;
				void(*doorEventChangerPointer)(int) = DoorEventChanger;
				cout << nCurrentEntity << " active collision" << endl;

				workers.push_back(thread(&CCollisionSystem::TestThreading, pcCollisionSystem,
					&tThisWorld, nCurrentEntity, pcGraphicsSystem,
					&tTempVertexBuffer, std::ref(tMyVertexBufferTemp.m_d3dWorldMatrix), &m_d3dPlayerMatrix, pcPhysicsSystem, pcAiSystem,
					PlayerStartIndex, std::ref(playerDamage), std::ref(pirateDamage),
					std::ref(prevHealth), std::ref(fallingHealth), std::ref(lerpTime),
					pcAudioSystem->m_fMasterVolume, pcAudioSystem->m_fSFXVolume, pcAudioSystem->m_fMusicVolume, pcAudioSystem,
					doorEventListenerPointer, doorEventChangerPointer, std::ref(hitmarkerTime), &m_d3dClaytonMatrix, &m_d3dCaelisMatrix,  pcParticleSystem,fpsTimer.GetDelta(),
					&tThisWorld.atAnimation[nCurrentEntity], &tThisWorld.atAnimationVariant[nCurrentEntity]));


				//	tThisWorld.atAABB[nCurrentEntity].myThread = workers.begin() + workers.size() - 1;
			}
			for (std::thread &t : workers)
			{
				if (t.joinable())
				{
					t.join();
					workers.erase(workers.begin());
				}
			}
		}

		if (tThisWorld.atParentWorldMatrix[nCurrentEntity] != -1)
		{
			if (nCurrentEntity != GunIndexForPlayer && nCurrentEntity != 914)
			{
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(XMMatrixIdentity(),
					tThisWorld.atWorldMatrix[tThisWorld.atParentWorldMatrix[nCurrentEntity]].worldMatrix);

				//tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(pcGraphicsSystem->SetDefaultWorldPosition(),
				//	tThisWorld.atAnimation->m_tAnim[tThisWorld.atAnimationVariant[nCurrentEntity].tClaytonAnim.animType].m_vtKeyFrames[tThisWorld.atAnimationVariant[nCurrentEntity].tClaytonAnim.currentFrame].m_vd3dJointMatrices[50]);
				//tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(tThisWorld.atOffSetMatrix[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
			}
			else if (nCurrentEntity == 914)
			{
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(pcGraphicsSystem->SetDefaultWorldPosition(),
					tThisWorld.atWorldMatrix[tThisWorld.atParentWorldMatrix[nCurrentEntity]].worldMatrix);

				//tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(tThisWorld.atOffSetMatrix[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
			}
			else
			{
				XMMATRIX CamandPlayer;
				CamandPlayer.r[0] = aimCamera->d3d_Position.r[0];
				CamandPlayer.r[1] = aimCamera->d3d_Position.r[1];
				CamandPlayer.r[2] = aimCamera->d3d_Position.r[2];

				CamandPlayer.r[3] = tThisWorld.atWorldMatrix[tThisWorld.atParentWorldMatrix[nCurrentEntity]].worldMatrix.r[3];
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(XMMatrixIdentity(),
					CamandPlayer);

				//tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(XMMatrixIdentity(), tThisWorld.atWorldMatrix[tThisWorld.atParentWorldMatrix[nCurrentEntity]].worldMatrix);

				//tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(XMMatrixIdentity(),
				//	tThisWorld.atWorldMatrix[tThisWorld.atParentWorldMatrix[nCurrentEntity]].worldMatrix);
				/*tThisWorld.atOffSetMatrix[nCurrentEntity].r[3].m128_f32[0] *= .01;
				tThisWorld.atOffSetMatrix[nCurrentEntity].r[3].m128_f32[1] *= .01;
				tThisWorld.atOffSetMatrix[nCurrentEntity].r[3].m128_f32[2] *= .01;
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(tThisWorld.atOffSetMatrix[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);*/

				/*tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(XMMatrixRotationX(XMConvertToRadians(40)), tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(65)), tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);

				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(XMMatrixTranslation(1, 0, 0), tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);*/
			}

			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(tThisWorld.atOffSetMatrix[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);

			if (nCurrentEntity == GunIndexForClayton)
			{
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(89.4)), tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(XMMatrixRotationZ(XMConvertToRadians(1.2)), tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);

				//tThisWorld.atOffSetMatrix[nCurrentEntity] =

				tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
			}
#if CAELIS
			else if (nCurrentEntity == GunIndexForCaelis)
			{
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(XMMatrixRotationX(XMConvertToRadians(-90)), tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
				//tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(XMMatrixRotationZ(XMConvertToRadians(-90)), tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);

				tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
			}
#endif
			else
			{
				pcAiSystem->FollowObject(tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix, &tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);

				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(-90)), tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);

				float distanceToPlayer = pcAiSystem->CalculateDistanceMatrix(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atWorldMatrix[ClaytonIndex].worldMatrix);
				if (distanceToPlayer > 10)
				{
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(XMMatrixRotationZ(XMConvertToRadians(2.5)), tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
				}
				else
				{
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(XMMatrixRotationZ(XMConvertToRadians(8 - distanceToPlayer / 2)), tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
				}

				tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
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
		if (tThisWorld.atAABB[nCurrentEntity].TimeColiderIsDIsabled > 0 && tThisWorld.atAABB[nCurrentEntity].disabledabb == true) {
			tThisWorld.atAABB[nCurrentEntity].TimeColiderIsDIsabled -= fpsTimer.GetDelta() + 1;
			cout << "disabled timer" << tThisWorld.atAABB[nCurrentEntity].TimeColiderIsDIsabled;

		}
		else {
			tThisWorld.atAABB[nCurrentEntity].disabledabb = false;
			pcCollisionSystem->replaceAABB(nCurrentEntity, tThisWorld.atAABB[nCurrentEntity]);
		}
		if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID))
		{
			tTempPixelBuffer.m_d3dCollisionColor = tThisWorld.atSimpleMesh[nCurrentEntity].m_nColor;
			if (tThisWorld.atParticleMask[nCurrentEntity].m_tnParticleMask == (COMPONENT_Particles | COMPONENT_Active)) {
				if (tThisWorld.atParticle[nCurrentEntity].AliveTime <= 0) {
					destroyEntity(&tThisWorld, nCurrentEntity);
				}
				else {
					tThisWorld.atParticle[nCurrentEntity].AliveTime -= fpsTimer.GetDelta() + 1;
				}
			}
			if (tThisWorld.atParticleMask[nCurrentEntity].m_tnParticleMask == (COMPONENT_Particles | COMPONENT_Active | COMPONENT_ChangeColor)) {
				tTempPixelBuffer.m_d3dCollisionColor = pcParticleSystem->ChangeColor(pcAiSystem);
				if (tThisWorld.atParticle[nCurrentEntity].AliveTime <= 0) {
					destroyEntity(&tThisWorld, nCurrentEntity);
				}
				else {
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[2] += tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix.r[3].m128_f32[2]
						- tThisWorld.atOffSetMatrix[nCurrentEntity].r[3].m128_f32[2];

					tThisWorld.atOffSetMatrix[nCurrentEntity].r[3].m128_f32[2] += tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix.r[3].m128_f32[2]
						- tThisWorld.atOffSetMatrix[nCurrentEntity].r[3].m128_f32[2];
					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1] += tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix.r[3].m128_f32[1]
						- tThisWorld.atOffSetMatrix[nCurrentEntity].r[3].m128_f32[1];
					tThisWorld.atOffSetMatrix[nCurrentEntity].r[3].m128_f32[1] += tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix.r[3].m128_f32[1]
						- tThisWorld.atOffSetMatrix[nCurrentEntity].r[3].m128_f32[1];

					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[0] += tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix.r[3].m128_f32[0]
						- tThisWorld.atOffSetMatrix[nCurrentEntity].r[3].m128_f32[0];
					tThisWorld.atOffSetMatrix[nCurrentEntity].r[3].m128_f32[0] += tThisWorld.atWorldMatrix[PlayerStartIndex].worldMatrix.r[3].m128_f32[0]
						- tThisWorld.atOffSetMatrix[nCurrentEntity].r[3].m128_f32[0];
				//	tThisWorld.atOffSetMatrix[nCurrentEntity] = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix;

					tThisWorld.atParticle[nCurrentEntity].AliveTime -= fpsTimer.GetDelta() + 1;
				}
			}
		
		//	if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask != (COMPONENT_UIMASK | COMPONENT_NOSHOW))
			{
				//if (pcCollisionSystem->aabb_to_frustum(tThisWorld.atAABB[nCurrentEntity], tThisWorld.atClaytonVision.eyes0))
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
			}


			//if (pcCollisionSystem->aabb_to_frustum(tThisWorld.atAABB[nCurrentEntity], tThisWorld.atClaytonVision.eyes0))
			//{
			//	if (tCameraMode.bWalkMode == true)
			//	{
			//		pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity], walkCamera->d3d_Position);
			//	}
			//	//Calls Shader Function on Aim Camera If aim mode is active
			//	else if (tCameraMode.bAimMode == true)
			//	{
			//		pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer , tThisWorld.atSimpleMesh[nCurrentEntity],aimCamera->d3d_Position);
			//	}
			//	else
			//	{
			//		pcGraphicsSystem->InitPrimalShaderData2(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atSimpleMesh[nCurrentEntity], debugCamera->d3d_Position);
			//	}
			//	pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atSimpleMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			//}
		}


		//if (tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		//{
		//	/*if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_CLIP))
		//	{
		//		if (tCameraMode.bWalkMode == true)
		//		{
		//			pcGraphicsSystem->InitPrimalShaderData3(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], walkCamera->d3d_Position);
		//		}
		//		else if (tCameraMode.bAimMode == true)
		//		{
		//			pcGraphicsSystem->InitPrimalShaderData3(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], aimCamera->d3d_Position);
		//		}
		//		else
		//		{
		//			pcGraphicsSystem->InitPrimalShaderData3(pcGraphicsSystem->m_pd3dDeviceContext, tTempVertexBuffer, tTempPixelBuffer, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);
		//		}
		//		pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		//	}
		//	else if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_FRIENDLY) ||
		//			 tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_ENEMY))
		//	{
		//		if (tCameraMode.bWalkMode == true)
		//		{
		//			pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], walkCamera->d3d_Position);
		//		}
		//		else if (tCameraMode.bAimMode == true)
		//		{
		//			pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], aimCamera->d3d_Position);
		//		}
		//		else
		//		{
		//			pcGraphicsSystem->InitMyShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tMyVertexBufferTemp, tThisWorld.atMesh[nCurrentEntity], debugCamera->d3d_Position);
		//		}
		//		pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
		//	}*/
		//}

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

					if (tThisWorld.atButton[nCurrentEntity].enabled && clickTime > TIMEUNTILCLICK)
					{
						if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
						{
							clickTime = 0;
							//Click soud for menus here - ZFB
#if MUSIC_ON
							pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_MENU_CLICK, pcAudioSystem->m_MenuClick);
							pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);
#endif
							if (tThisWorld.atButton[nCurrentEntity].sceneIndex == OPTIONS_INDEX)
							{
								clickPoint = { -1, -1 };
								dragPoint = { -1, -1 };

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
							else if (tThisWorld.atButton[nCurrentEntity].sceneIndex == SAVE_INDEX || tThisWorld.atButton[nCurrentEntity].sceneIndex == LOAD_INDEX)
							{
								clickPoint = { -1, -1 };
								dragPoint = { -1, -1 };
							}
							else
							{
								return tThisWorld.atButton[nCurrentEntity].sceneIndex;
							}

						}
						//Here is color change for Icon 
						else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
						{
							tUIPixelBuffer.hoverColor = XMFLOAT4(.3, .3, .3, 0);
							if (tThisWorld.atButton[nCurrentEntity].hovered == false)
							{
#if MUSIC_ON
								pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_HOVERSOUND, pcAudioSystem->m_HoverSound);
								pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);
#endif
							}
							tThisWorld.atButton[nCurrentEntity].hovered = true;

						}
						else
						{
							tThisWorld.atButton[nCurrentEntity].hovered = false;
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

					if (tThisWorld.atButton[nCurrentEntity].enabled && clickTime > TIMEUNTILCLICK)
					{
						if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
						{
							clickTime = 0;
							//Click soud for menus here - ZFB
#if MUSIC_ON
							pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_MENU_CLICK, pcAudioSystem->m_MenuClick);
							pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);
#endif
							if (tThisWorld.atButton[nCurrentEntity].sceneIndex == nCurrentScene)
							{
								clickPoint = { -1, -1 };
								dragPoint = { -1, -1 };

								options = false;
							}
						}
						else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
						{
							tUIPixelBuffer.hoverColor = XMFLOAT4(.3, .3, .3, 0);
							if (tThisWorld.atButton[nCurrentEntity].hovered == false)
							{
#if MUSIC_ON
								pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_HOVERSOUND, pcAudioSystem->m_HoverSound);
								pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);
#endif
							}
							tThisWorld.atButton[nCurrentEntity].hovered = true;

						}
						else
						{
							tThisWorld.atButton[nCurrentEntity].hovered = false;
						}

					}

					pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
					pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);

				}

				if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BAR | COMPONENT_OPTIONS))
				{
					if (PtInRect(&tThisWorld.atBar[nCurrentEntity].barBoundingBox, clickPoint))
					{
						tThisWorld.atBar[nCurrentEntity].ratio = (clickPoint.x - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left + 0.0) / (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left);

						pcUISystem->CheckOptionsBars(&tThisWorld, pcGraphicsSystem, pcInputSystem, nCurrentEntity, pcAudioSystem->m_fMasterVolume, pcAudioSystem->m_fMusicVolume, pcAudioSystem->m_fSFXVolume, masterIndex, musicIndex, fxIndex, pcGraphicsSystem->m_CurrentAmbience, brightnessIndex);
#if MUSIC_ON
						if ( click == true && fxIndex == nCurrentEntity)
						{
							pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_S_FEEDBACK, pcAudioSystem->m_FeedBack);
							pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);

						}
#endif
						
					}
					else if (PtInRect(&tThisWorld.atBar[nCurrentEntity].barBoundingBox, dragPoint))
					{
              
						tThisWorld.atBar[nCurrentEntity].ratio = (dragPoint.x - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left + 0.0) / (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left);

						pcUISystem->CheckOptionsBars(&tThisWorld, pcGraphicsSystem, pcInputSystem, nCurrentEntity, pcAudioSystem->m_fMasterVolume, pcAudioSystem->m_fMusicVolume, pcAudioSystem->m_fSFXVolume, masterIndex, musicIndex, fxIndex, pcGraphicsSystem->m_CurrentAmbience, brightnessIndex);
					}

					if (tThisWorld.atBar[nCurrentEntity].backgroundColor.x == 1 &&
						tThisWorld.atBar[nCurrentEntity].backgroundColor.y == 0 &&
						tThisWorld.atBar[nCurrentEntity].backgroundColor.z == 0 &&
						tThisWorld.atBar[nCurrentEntity].backgroundColor.w == 1)
					{
						tUIVertexBuffer.start = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.left - (screenWidth * .5)) / (screenWidth * .5);
						tUIVertexBuffer.end = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right - (screenWidth * .5)) / (screenWidth * .5);
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

					if (tThisWorld.atButton[nCurrentEntity].enabled && clickTime > TIMEUNTILCLICK)
					{
						if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, clickPoint))
						{
							clickTime = 0;
							//Click soud for menus here - ZFB
#if MUSIC_ON
							pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_MENU_CLICK, pcAudioSystem->m_MenuClick);
							pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);
#endif

							if (tThisWorld.atButton[nCurrentEntity].sceneIndex < SAVE_INDEX)
							{
								clickPoint = { -1, -1 };
								dragPoint = { -1, -1 };

								return tThisWorld.atButton[nCurrentEntity].sceneIndex;
							}
						}
						else if (PtInRect(&tThisWorld.atButton[nCurrentEntity].boundingBox, hoverPoint))
						{
							tUIPixelBuffer.hoverColor = XMFLOAT4(.3, .3, .3, 0);
							if (tThisWorld.atButton[nCurrentEntity].hovered == false)
							{
#if MUSIC_ON
								pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_HOVERSOUND, pcAudioSystem->m_HoverSound);
								pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, pcAudioSystem->m_fSFXVolume);
#endif
							}
							tThisWorld.atButton[nCurrentEntity].hovered = true;

						}
						else
						{
							tThisWorld.atButton[nCurrentEntity].hovered = false;
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

			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL))
			{
				tUIVertexBuffer.start = -1;
				tUIVertexBuffer.end = -1;
				tUIVertexBuffer.ratio = -1;

				tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

				if (tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse != nullptr)
				{
					tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse = nullptr;
				}

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}

			if (GamePaused && !options)
			{
				if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_PAUSESCREEN))
				{
					tUIVertexBuffer.start = -1;
					tUIVertexBuffer.end = -1;
					tUIVertexBuffer.ratio = -1;

					tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

					if (tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse != nullptr)
					{
						tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse = nullptr;
					}

					pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
					pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
				}
			}
			else if (GamePaused && options)
			{
				if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_OPTIONS))
				{
					tUIVertexBuffer.start = -1;
					tUIVertexBuffer.end = -1;
					tUIVertexBuffer.ratio = -1;

					tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

					if (tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse != nullptr)
					{
						tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse = nullptr;
					}

					pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
					pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
				}
			}
		}


#if HUD
		if (tCameraMode.bDebugMode == false)
		{
			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_HUD))
			{
				tUIVertexBuffer.start = -1;
				tUIVertexBuffer.end = -1;
				tUIVertexBuffer.ratio = -1;

				if (nCurrentEntity == objLogoIndex)
				{
					float degrees = XMConvertToDegrees(fadeTime * .025);
					float opacity = (cos(degrees) * .5) - .5;

					tUIPixelBuffer.hoverColor = XMFLOAT4(0, 0, 0, opacity);
				}
				else if (nCurrentEntity == hitmarkerIndex)
				{
					float opacity = 0 - hitmarkerTime;

					tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;

					tUIPixelBuffer.hoverColor.w = opacity;

					if (hitmarkerTime >= 1)
					{
						continue;
					}
				}
				else
				{
					tUIPixelBuffer.hoverColor = tThisWorld.atLabel[nCurrentEntity].color;
				}

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}

			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BAR | COMPONENT_HUD))
			{
				pcUISystem->UpdateHUDBars(&tThisWorld, nCurrentEntity, tUIVertexBuffer, screenWidth, PlayerStartIndex, GunIndexForPlayer, CaelisIndex);

				tUIPixelBuffer.hoverColor = tThisWorld.atBar[nCurrentEntity].backgroundColor;

				if (tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse != nullptr)
				{
					tThisWorld.atMesh[nCurrentEntity].m_d3dSRVDiffuse = nullptr;
				}

				pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
				pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
			}

			if (tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK | COMPONENT_LABEL | COMPONENT_BAR | COMPONENT_BACKGROUND | COMPONENT_HUD))
			{
				if (prevHealth != 0)
				{
					float barWidth = tThisWorld.atBar[nCurrentEntity].barBoundingBox.right - tThisWorld.atBar[nCurrentEntity].barBoundingBox.left;

					float damageRatio = 1 - (lerpTime * .5);
					fallingHealth = prevHealth - ((prevHealth - tThisWorld.atClayton[PlayerStartIndex].health) * abs(damageRatio - 1));

					if (damageRatio < 0)
					{
						prevHealth = 0;
						fallingHealth = 0;
					}

					tUIVertexBuffer.start = (tThisWorld.atBar[nCurrentEntity - 1].barBoundingBox.left - (screenWidth * .5) + (barWidth * ((tThisWorld.atClayton[PlayerStartIndex].health) * .01))) / (screenWidth * .5);
					tUIVertexBuffer.end = (tThisWorld.atBar[nCurrentEntity].barBoundingBox.right - (screenWidth * .5) - (barWidth * ((100 - prevHealth) * .01))) / (screenWidth * .5);
					tUIVertexBuffer.ratio = damageRatio;

					tUIPixelBuffer.hoverColor = tThisWorld.atBar[nCurrentEntity - 1].backgroundColor;

					tUIPixelBuffer.hoverColor.x /= 1.4;
					tUIPixelBuffer.hoverColor.y /= 1.4;
					tUIPixelBuffer.hoverColor.z /= 1.4;

					pcGraphicsSystem->InitUIShaderData(pcGraphicsSystem->m_pd3dDeviceContext, tUIVertexBuffer, tUIPixelBuffer, tThisWorld.atMesh[nCurrentEntity], menuCamera->d3d_Position);
					pcGraphicsSystem->ExecutePipeline(pcGraphicsSystem->m_pd3dDeviceContext, tThisWorld.atMesh[nCurrentEntity].m_nIndexCount, tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask, tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
				}
			}
		}
#endif
	}
	

	pcGraphicsSystem->m_pd3dOutsideGlassSRV->Release();

	delta = fpsTimer.GetDelta();

	fadeTime += delta;
	clickTime += delta;
	lerpTime += delta;
	hitmarkerTime += delta;
	tThisWorld.atCaelis[CaelisIndex].m_tfSpecialCooldown -= delta * 10;
	moveTime += delta;

	if (tThisWorld.atLabel[gunImageIndex].color.x != 0)
	{
		++outOfAmmoFrames;

		if (outOfAmmoFrames > 5)
		{
			tThisWorld.atLabel[gunImageIndex].color = XMFLOAT4(0, 0, 0, 1);
			outOfAmmoFrames = 0;
		}
	}

	if (moveTime > 1 && bNoMoving)
	{
		bNoMoving = false;
	}

#if FPS
	pcUISystem->UpdateFPS(&tThisWorld, pcGraphicsSystem, fpsTimer, fpsIndex, tUIVertexBuffer, tUIPixelBuffer, atUIVertices, menuCamera);
#endif

	pcGraphicsSystem->m_pd3dSwapchain->Present(0, 0);
	zValue += 0.001;
	return 14;
}

int CGameMangerSystem::ResetLevel()
{
	GamePaused = false;
	GameOver = false;

	ShowCursor(false);

	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		if ((tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID) || tThisWorld.atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_ANIMATION | COMPONENT_SHADERID)) && tThisWorld.atUIMask[nCurrentEntity].m_tnUIMask == (COMPONENT_UIMASK))
		{
			//Clayton input with Camera variables here
			if (GamePaused == false && GameOver == false)
			{
				if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK))
				{
					PlayerStartIndex = nCurrentEntity;

					GunIndexForPlayer = GunIndexForClayton;

					wchar_t filePath[] =
					{ L"UI_Textures.fbm/Material_Gun_Paint.png" };

					pcUISystem->AddTextureToUI(&tThisWorld, gunImageIndex, pcGraphicsSystem->m_pd3dDevice, filePath);

					tCameraMode.bAimMode = true;
					tCameraMode.bDebugMode = false;
					tCameraMode.bWalkMode = false;
					tCameraMode.bSwitch = true;

					m_d3dClaytonMatrix = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixRotationRollPitchYaw(0, XMConvertToRadians(180), 0);

					m_d3dClaytonMatrix.r[3].m128_f32[0] = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[0] = -4;
					m_d3dClaytonMatrix.r[3].m128_f32[1] = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1] = -1.2;
					m_d3dClaytonMatrix.r[3].m128_f32[2] = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[2] = 5.3;

					debugCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
					debugCamera->fPitch = 0;
					debugCamera->fRoll = 0;
					debugCamera->fYaw = 0;
					walkCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
					walkCamera->fPitch = 0;
					walkCamera->fRoll = 0;
					walkCamera->fYaw = 0;
					aimCamera->d3d_Position = pcGraphicsSystem->SetDefaultCameraMatrix();
					aimCamera->fPitch = 0;
					aimCamera->fRoll = 0;
					aimCamera->fYaw = 0;

					tThisWorld.atRigidBody[nCurrentEntity].velocity = XMVectorSet(0, 0, 0, 0);

					tThisWorld.atClayton[nCurrentEntity].health = 100;
					tThisWorld.atCaelis[nCurrentEntity].m_tfSpecialCooldown = 0;

#if JUMPING
					tThisWorld.atClayton[nCurrentEntity].jumpCooldown = 0;
					tThisWorld.atClayton[nCurrentEntity].jumpTime = 1;
#endif

					tThisWorld.atAnimation[nCurrentEntity].tTimer.localTime = 0;
					tThisWorld.atAnimationVariant[nCurrentEntity].tClaytonAnim.animType = 0;
					tThisWorld.atAnimationVariant[nCurrentEntity].tClaytonAnim.currentFrame = 0;
					tThisWorld.atAnimationVariant[nCurrentEntity].tClaytonAnim.nextFrame = 1;

					pcProjectileSystem->Reload(&tThisWorld.atClip[GunIndexForClayton]);

					int textSize = 2;
					wchar_t* textBuffer = new wchar_t[textSize];

					if (tThisWorld.atClip[GunIndexForPlayer].nBulletsAvailables.size() <= 9)
					{
						textBuffer[0] = 0;
						textBuffer[1] = tThisWorld.atClip[GunIndexForPlayer].nBulletsAvailables.size();
					}
					else
					{
						textBuffer[0] = tThisWorld.atClip[GunIndexForPlayer].nBulletsAvailables.size() * .1;
						textBuffer[1] = tThisWorld.atClip[GunIndexForPlayer].nBulletsAvailables.size() % 10;
					}

					pcUISystem->UpdateText(&tThisWorld, pcGraphicsSystem, ammoIndex, &atUIVertices, textBuffer, textSize, atUIVertices.at(tThisWorld.atLabel[ammoIndex].vIndex));

					delete[] textBuffer;

					XMMATRIX claytonFrustumMatrix = aimCamera->d3d_Position;
					claytonFrustumMatrix = XMMatrixMultiply(XMMatrixTranslation(0, 0, -15), claytonFrustumMatrix);

					float4x4 ClaytonFrustum;
					ClaytonFrustum.row1.x = claytonFrustumMatrix.r[0].m128_f32[0];
					ClaytonFrustum.row1.y = claytonFrustumMatrix.r[0].m128_f32[1];
					ClaytonFrustum.row1.z = claytonFrustumMatrix.r[0].m128_f32[2];
					ClaytonFrustum.row1.w = claytonFrustumMatrix.r[0].m128_f32[3];

					ClaytonFrustum.row2.x = claytonFrustumMatrix.r[1].m128_f32[0];
					ClaytonFrustum.row2.y = claytonFrustumMatrix.r[1].m128_f32[1];
					ClaytonFrustum.row2.z = claytonFrustumMatrix.r[1].m128_f32[2];
					ClaytonFrustum.row2.w = claytonFrustumMatrix.r[1].m128_f32[3];

					ClaytonFrustum.row3.x = claytonFrustumMatrix.r[2].m128_f32[0];
					ClaytonFrustum.row3.y = claytonFrustumMatrix.r[2].m128_f32[1];
					ClaytonFrustum.row3.z = claytonFrustumMatrix.r[2].m128_f32[2];
					ClaytonFrustum.row3.w = claytonFrustumMatrix.r[2].m128_f32[3];

					ClaytonFrustum.row4.x = claytonFrustumMatrix.r[3].m128_f32[0];
					ClaytonFrustum.row4.y = claytonFrustumMatrix.r[3].m128_f32[1];
					ClaytonFrustum.row4.z = claytonFrustumMatrix.r[3].m128_f32[2];
					ClaytonFrustum.row4.w = claytonFrustumMatrix.r[3].m128_f32[3];

					pcAiSystem->UpdateFrustum(tThisWorld.atClaytonVision.eyes0, ClaytonFrustum, 60, 1, 0.1, 150);

					tThisWorld.atWorldMatrix[claytonFrustumIndex].worldMatrix = claytonFrustumMatrix;
				}
#if CAELIS
				else if (tThisWorld.atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_INPUTMASK | COMPONENT_CAELIS))
				{
					m_d3dCaelisMatrix.r[3].m128_f32[0] = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[0] = 4;
					m_d3dCaelisMatrix.r[3].m128_f32[1] = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[1] = 0;
					m_d3dCaelisMatrix.r[3].m128_f32[2] = tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix.r[3].m128_f32[2] = 0;

					pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);

					pcProjectileSystem->Reload(&tThisWorld.atClip[GunIndexForCaelis]);

					int textSize = 2;
					wchar_t* textBuffer = new wchar_t[textSize];

					if (tThisWorld.atClip[GunIndexForPlayer].nBulletsAvailables.size() <= 9)
					{
						textBuffer[0] = 0;
						textBuffer[1] = tThisWorld.atClip[GunIndexForPlayer].nBulletsAvailables.size();
					}
					else
					{
						textBuffer[0] = tThisWorld.atClip[GunIndexForPlayer].nBulletsAvailables.size() * .1;
						textBuffer[1] = tThisWorld.atClip[GunIndexForPlayer].nBulletsAvailables.size() % 10;
					}

					pcUISystem->UpdateText(&tThisWorld, pcGraphicsSystem, ammoIndex, &atUIVertices, textBuffer, textSize, atUIVertices.at(tThisWorld.atLabel[ammoIndex].vIndex));

					delete[] textBuffer;
				}
#endif
				else if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_FOLLOW) ||
					tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_SHOOT | COMPONENT_AIMASK | COMPONENT_FOLLOW) ||
					tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST) ||
					tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SEARCH) ||
					tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SPOTEDPLAYER))
				{
					tThisWorld.atAiHeath[nCurrentEntity].heath = 100;

					tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask = (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST);

					tThisWorld.atAIVision[nCurrentEntity].keepSearching = false;
					tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight = true;
					tThisWorld.atAIVision[nCurrentEntity].stopSearching = false;
					tThisWorld.atPathPlanining[nCurrentEntity].InterRuptPathPlanning = true;
					tThisWorld.atActiveAI[nCurrentEntity].active = false;

				//	tThisWorld.atRigidBody[nCurrentEntity].gravity.m128_f32[1] = 0;
					tThisWorld.atRigidBody[nCurrentEntity].velocity = XMVectorSet(0, 0, 0, 0);

					tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = tThisWorld.atOffSetMatrix[nCurrentEntity];

					pcAiSystem->SetCanWeChooseShooter(true);
					pcAiSystem->ClearShootingActiveAI();

					pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);
					TAABB MyAbb = pcCollisionSystem->createAABBS(tThisWorld.atMesh[nCurrentEntity].m_VertexData, tThisWorld.atAABB[nCurrentEntity]);
					MyAbb.m_IndexLocation = nCurrentEntity;
					MyAbb.locationinArray = pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity);
				


						tThisWorld.atAABB[nCurrentEntity] = MyAbb;
						
					
					tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
				//	pcCollisionSystem->AddAABBCollider(MyAbb, nCurrentEntity); orld.atAABB[nCurrentEntity]);
				}
			}
		}

		if (GamePaused == false && GameOver == false)
		{
			if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SEARCH)
				|| tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SPOTEDPLAYER)
				|| tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST))
			{
			//	if (tThisWorld.atAIVision[nCurrentEntity].keepSearching == false)
				{
					tThisWorld.atAIVision[nCurrentEntity].keepSearching = false;
				}

				if (tThisWorld.atAIVision[nCurrentEntity].keepSearching == true)
				{
					if (tThisWorld.atAIVision[nCurrentEntity].visionRotation < 7
						&& tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true)
					{
						//tThisWorld.atAIVision[nCurrentEntity].visionRotation += 0.01;
						//tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcAiSystem->LookBackLeftToRight(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, true);
					}
					else if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == true)
					{
						tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight = false;
					}
					else if (tThisWorld.atAIVision[nCurrentEntity].visionRotation > -7
						&& tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == false)
					{
						//tThisWorld.atAIVision[nCurrentEntity].visionRotation -= 0.01;
						//tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = pcAiSystem->LookBackLeftToRight(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, false);
					}
					else if (tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight == false)
					{
						tThisWorld.atAIVision[nCurrentEntity].keepRotatingRight = true;
					}
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
			}
		}
		if (GamePaused == false && GameOver == false)
		{
			if (tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_PATHFINDTEST)
				|| tThisWorld.atAIMask[nCurrentEntity].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_SEARCH | COMPONENT_PATHFINDTEST))
			{
				if (tThisWorld.atActiveAI[nCurrentEntity].active == true)
				{
					tThisWorld.atActiveAI[nCurrentEntity].active = false;

				//	if (tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding == true)
					{
						tThisWorld.atPathPlanining[nCurrentEntity].testingPathFinding = true;
					}
					//else
					{
						if (tThisWorld.atPathPlanining[nCurrentEntity].DelayMovement <= 0)
						{
							tThisWorld.atPathPlanining[nCurrentEntity].DelayMovement = 0;
						}
					}
				}

			/*	if (tThisWorld.atAABB[nCurrentEntity].m_IndexLocation == -1)
				{
					tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->createAABBS(tThisWorld.atMesh[nCurrentEntity].m_VertexData, tThisWorld.atAABB[nCurrentEntity]);
					tThisWorld.atAABB[nCurrentEntity].m_IndexLocation = nCurrentEntity;

					pcCollisionSystem->AddAABBCollider(tThisWorld.atAABB[nCurrentEntity], nCurrentEntity);
					pcGraphicsSystem->CreateEntityBuffer(&tThisWorld, nCurrentEntity);
				}*/
			}
		}

		if (GamePaused == false && GameOver == false)
		{
			if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_CLIP))
			{
				tThisWorld.atClip[nCurrentEntity].tryToShoot = false;
				tThisWorld.atClip[nCurrentEntity].tryToReload = false;

				tThisWorld.atClip[nCurrentEntity].fShootingCoolDown = 0;
			}
		}

		if (tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_FRIENDLY) ||
			tThisWorld.atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_ENEMY))
		{
			pcCollisionSystem->RemoveAABBCollider(nCurrentEntity);

			pcGraphicsSystem->CleanD3DObject(&tThisWorld, nCurrentEntity);
		}

		if (tThisWorld.atParentWorldMatrix[nCurrentEntity] != -1)
		{
			if (nCurrentEntity != GunIndexForPlayer)
			{
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(pcGraphicsSystem->SetDefaultWorldPosition(),
					tThisWorld.atWorldMatrix[tThisWorld.atParentWorldMatrix[nCurrentEntity]].worldMatrix);
			}
			else
			{
				XMMATRIX CamandPlayer;
				CamandPlayer.r[0] = aimCamera->d3d_Position.r[0];
				CamandPlayer.r[1] = aimCamera->d3d_Position.r[1];
				CamandPlayer.r[2] = aimCamera->d3d_Position.r[2];

				CamandPlayer.r[3] = tThisWorld.atWorldMatrix[tThisWorld.atParentWorldMatrix[nCurrentEntity]].worldMatrix.r[3];
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(pcGraphicsSystem->SetDefaultWorldPosition(),
					CamandPlayer);
			}

			tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(tThisWorld.atOffSetMatrix[nCurrentEntity], tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);

			if (nCurrentEntity != GunIndexForPlayer)
			{
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(-90)), tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);
			}
			else
			{
				tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix = XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(90)), tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix);

				tThisWorld.atAABB[nCurrentEntity] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[nCurrentEntity].worldMatrix, tThisWorld.atAABB[nCurrentEntity]);
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

		if (endInit)
		{
			CleanEndScreen(false);

			pcAiSystem->SetNumberOfAI(NUMBER_OF_AI);

			endInit = false;
		}

		if (nCurrentEntity != 909)
		{
			if (tThisWorld.atShaderID[nCurrentEntity].m_nShaderID % 10 == 0)
			{
				for (int doorindex = 0; doorindex < tThisWorld.atAABB[nCurrentEntity].doorPeices.size(); ++doorindex)
				{
					DoorEventChanger(tThisWorld.atShaderID[nCurrentEntity].m_nShaderID);
					while (tThisWorld.atWorldMatrix[tThisWorld.atAABB[nCurrentEntity].doorPeices[doorindex]].worldMatrix.r[3].m128_f32[0] != 0 ||
						tThisWorld.atWorldMatrix[tThisWorld.atAABB[nCurrentEntity].doorPeices[doorindex]].worldMatrix.r[3].m128_f32[1] != 0 ||
						tThisWorld.atWorldMatrix[tThisWorld.atAABB[nCurrentEntity].doorPeices[doorindex]].worldMatrix.r[3].m128_f32[2] != 0)
					{
						tThisWorld.atWorldMatrix[tThisWorld.atAABB[nCurrentEntity].doorPeices[doorindex]].worldMatrix = XMMatrixMultiply(DoorEventListener(tThisWorld.atShaderID[tThisWorld.atAABB[nCurrentEntity].doorPeices[doorindex]].m_nShaderID, false), tThisWorld.atWorldMatrix[tThisWorld.atAABB[nCurrentEntity].doorPeices[doorindex]].worldMatrix);
					}
					tThisWorld.atAABB[tThisWorld.atAABB[nCurrentEntity].doorPeices[doorindex]] = pcCollisionSystem->updateAABB(tThisWorld.atWorldMatrix[tThisWorld.atAABB[nCurrentEntity].doorPeices[doorindex]].worldMatrix, tThisWorld.atAABB[tThisWorld.atAABB[nCurrentEntity].doorPeices[doorindex]]);
				}
			}
		}
	}

	return 14;
}

/*
If door[i] == 1
	open door
If door[i] == -1
	close door
If door[i] == 0
	Something is wrong
If door[i] == 2
	door is open, do nothing
If door[i] == -2
	door is closed, do nothing
*/