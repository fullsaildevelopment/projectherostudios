#pragma once
//SDK files from Libraries.
#include"AK/SoundEngine/Common/AkMemoryMgr.h"
#include"AK/SoundEngine/Common/AkModule.h"
#include"AK/SoundEngine/Common/IAkStreamMgr.h"
#include"AK/SoundEngine/Platforms/Windows/AkWinSoundEngine.h"
#include"AK/SoundEngine/Common/AkStreamMgrModule.h"
#include"AK/Tools/Common/AkPlatformFuncs.h"
#include"AK/MusicEngine/Common/AkMusicEngine.h"
#include"AK/SoundEngine/Common/AkSoundEngine.h"
//File Package
#include"Win32/AkDefaultIOHookBlocking.h"
#include"Common/AkFilePackageLowLevelIO.h"
#include"../Villain CES/Common/AkDefaultLowLevelIODispatcher.h"
#include"../Villain CES/Common/AkFileLocationBase.h"
#include"../Villain CES/Common/AkFilePackage.h"
#include"../Villain CES/Common/AkFilePackageLowLevelIO.h"
#include"../Villain CES/Common/AkFilePackageLUT.h"

#ifndef AK_OPTIMIZED
// Only needed for debug mode
#include <AK/Comm/AkCommunication.h>
#endif //AK_OPTIMIZED
namespace AK
{
#ifdef WIN32
	void * AllocHook(size_t in_size)
	{
		return malloc(in_size);
	}
	void FreeHook(void * in_ptr)
	{
		free(in_ptr);
	}
	// Note: VirtualAllocHook() may be used by I/O pools of the default implementation
	// of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
	// - refer to the Windows SDK documentation for more details). This is NOT mandatory;
	// you may implement it with a simple malloc().
	void * VirtualAllocHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwAllocationType,
		DWORD in_dwProtect
	)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}
	void VirtualFreeHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwFreeType
	)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
#endif
}
using namespace AK;
class GameSound 
{
public:
	GameSound();
	~GameSound();
	void IntiializeSystem();
	void PlaySoundInBank(GameSound* mySound);
	void SendSoundsToEngine(AkUniqueID &uID, AkGameObjectID &obj_ID);
	void SendSoundsToEngine(const char* soundName, AkGameObjectID &obj_ID);

private:
	CAkDefaultIOHookBlocking* m_LowIOHook;
};

GameSound::GameSound()
{
}

GameSound::~GameSound()
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
	m_LowIOHook->Init(myDevice);
	
	
	

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
#endif // AK_OPTIMIZED

#pragma endregion Communication Between Wwise App & C++ Project
}



inline void GameSound::PlaySoundInBank(GameSound* mysound)
{
	AK::SoundEngine::RenderAudio();
}

inline void GameSound::SendSoundsToEngine(AkUniqueID &uID, AkGameObjectID &obj_ID)
{
	AK::SoundEngine::PostEvent(uID, obj_ID);
}

inline void GameSound::SendSoundsToEngine(const char * soundName, AkGameObjectID & obj_ID)
{
	AK::SoundEngine::PostEvent(soundName, obj_ID);
}


