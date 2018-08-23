#pragma once

#include"AK/SoundEngine/Common/AkMemoryMgr.h"
#include"AK/SoundEngine/Common/AkModule.h"
#include"AK/SoundEngine/Common/IAkStreamMgr.h"
#include"AK/SoundEngine/Platforms/Windows/AkWinSoundEngine.h"
#include"AK/SoundEngine/Common/AkStreamMgrModule.h"
#include"AK/Tools/Common/AkPlatformFuncs.h"
#include"AK/MusicEngine/Common/AkMusicEngine.h"
#include"AK/SoundEngine/Common/AkSoundEngine.h"
#ifndef AK_OPTIMIZED
// Only needed for debug mode
#include <AK/Comm/AkCommunication.h>
#endif //AK_OPTIMIZED
using namespace AK;
class GameSound
{
public:
	GameSound();
	~GameSound();
	void IntiializeSystem();
	void PlaySoundInBank(GameSound* mysound);
private:

};

GameSound::GameSound()
{
}

GameSound::~GameSound()
{
}

inline void GameSound::IntiializeSystem()
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
	AK::StreamMgr::SetFileLocationResolver();
	// AkFilePackageLowLevelIOBlocking.h find this file 
	
	

#pragma endregion Stream Manager & Device Init

#pragma region
	AkInitSettings mySoundEngine;
	AkPlatformInitSettings myPlatform;
	AK::SoundEngine::GetDefaultInitSettings(mySoundEngine);
	AK::SoundEngine::GetDefaultPlatformInitSettings(myPlatform);
	if(AK::SoundEngine::Init(&mySoundEngine, &myPlatform) != AK_Success)
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
#endif // !AK_OPTIMIZED

#pragma endregion Communication Between Wwise App & C++ Project
}

inline void GameSound::PlaySoundInBank(GameSound* mysound)
{
	AK::SoundEngine::RenderAudio();
}


