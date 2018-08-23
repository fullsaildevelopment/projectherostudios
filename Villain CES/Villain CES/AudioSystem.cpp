#pragma once
#include"AudioSystem.h"
CAudioSystem::CAudioSystem()
{
	m_LowIOHook = new CAkFilePackageLowLevelIOBlocking();
}

CAudioSystem::~CAudioSystem()
{
	//Has to deallocate be this order or at least tutorial said to
#ifndef AK_OPTIMIZED
	AK::Comm::Term();
#endif //!AK_OPTIMIZED

	AK::MusicEngine::Term();

	AK::SoundEngine::Term();
	m_LowIOHook->Term();

	if (AK::IAkStreamMgr::Get())
	{
		AK::IAkStreamMgr::Get()->Destroy();
	}

	AK::MemoryMgr::Term();
}
void CAudioSystem::IntiializeSystem()
{
#pragma region
	AkMemSettings myMemoryManager;
	myMemoryManager.uMaxNumPools = 20;
	int test = 0;
	if (AK::MemoryMgr::Init(&myMemoryManager) != AK_Success)
	{
		// Memeory Manager failed to create
		test = -1;
	}
#pragma endregion Memory Manager Init

#pragma region
	AkStreamMgrSettings myStream;
	AK::StreamMgr::GetDefaultSettings(myStream);
	if (!AK::StreamMgr::Create(myStream))
	{
		test = -2;
	}


	AkDeviceSettings myDevice;
	AK::StreamMgr::GetDefaultDeviceSettings(myDevice);

	//Schedule type Blocking
	myDevice.uSchedulerTypeFlags = AK_SCHEDULER_BLOCKING;

	m_LowIOHook->Init(myDevice);




#pragma endregion Stream Manager & Device Init

#pragma region
	AkInitSettings mySoundEngine;
	AkPlatformInitSettings myPlatform;
	AK::SoundEngine::GetDefaultInitSettings(mySoundEngine);
	AK::SoundEngine::GetDefaultPlatformInitSettings(myPlatform);
	if (AK::SoundEngine::Init(&mySoundEngine, &myPlatform) != AK_Success)
	{
		test = -3;
	}



#pragma endregion Sound System & Platform Init

#pragma region
	AkMusicSettings myMusicEngine;
	AK::MusicEngine::GetDefaultInitSettings(myMusicEngine);

	if (AK::MusicEngine::Init(&myMusicEngine) != AK_Success)
	{
		test = -4;
	}

#pragma endregion Music Engine Init

#pragma region
#ifndef AK_OPTIMIZED
	AkCommSettings myCommunicator;
	AK::Comm::GetDefaultInitSettings(myCommunicator);
	if (AK::Comm::Init(myCommunicator) != AK_Success)
	{
		test = -5;
	}
#endif // AK_OPTIMIZED

#pragma endregion Communication Between Wwise App & C++ Project
}



void CAudioSystem::PlaySoundInBank(CAudioSystem* mysound)
{
	AK::SoundEngine::RenderAudio();
}
// Fin
void CAudioSystem::SendSoundsToEngine(AkUniqueID &uID, AkGameObjectID &obj_ID)
{
	AK::SoundEngine::PostEvent(uID, obj_ID);
}
//Find by File name 
void CAudioSystem::SendSoundsToEngine(const char * soundName, AkGameObjectID & obj_ID)
{
	AK::SoundEngine::PostEvent(soundName, obj_ID);
}
