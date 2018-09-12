#pragma once
#include"AudioSystem.h"
CAudioSystem::CAudioSystem()
{
	m_LowIOHook = NULL;
	m_LowIOHook = new CAkFilePackageLowLevelIOBlocking();
}

CAudioSystem::~CAudioSystem()
{
	//Has to deallocate be this order or at least tutorial said to
//#ifndef AK_OPTIMIZED
//	AK::Comm::Term();
//#endif //!AK_OPTIMIZED

}
void CAudioSystem::IntiializeSystem(AKRESULT &out_ErrorCheck)
{
#pragma region
	AkMemSettings myMemoryManager;
	myMemoryManager.uMaxNumPools = 20;
	int test = 0;
	out_ErrorCheck = AK::MemoryMgr::Init(&myMemoryManager);
	if (out_ErrorCheck != AK_Success)
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

	out_ErrorCheck =  m_LowIOHook->Init(myDevice);




#pragma endregion Stream Manager & Device Init

#pragma region
	AkInitSettings mySoundEngine;
	AkPlatformInitSettings myPlatform;
	AK::SoundEngine::GetDefaultInitSettings(mySoundEngine);
	AK::SoundEngine::GetDefaultPlatformInitSettings(myPlatform);
	out_ErrorCheck = AK::SoundEngine::Init(&mySoundEngine, &myPlatform);
	if (out_ErrorCheck != AK_Success)
	{
		test = -3;
	}

#pragma endregion Sound System & Platform Init

#pragma region
	AkMusicSettings myMusicEngine;
	AK::MusicEngine::GetDefaultInitSettings(myMusicEngine);
	out_ErrorCheck = AK::MusicEngine::Init(&myMusicEngine);
	if (out_ErrorCheck != AK_Success)
	{
		test = -4;
	}

#pragma endregion Music Engine Init

//#pragma region
//#ifndef AK_OPTIMIZED
//	AkCommSettings myCommunicator;
//	AK::Comm::GetDefaultInitSettings(myCommunicator);
//	if (AK::Comm::Init(myCommunicator) != AK_Success)
//	{
//		test = -5;
//	}
//#endif // AK_OPTIMIZED
//
//#pragma endregion Communication Between Wwise App & C++ Project
}

void CAudioSystem::TermSoundEngine()
{
	ClearAllActiveBnks();
	ClearActiveGameObjs();
	AK::MusicEngine::Term();

	AK::SoundEngine::Term();

	m_LowIOHook->Term();
	delete m_LowIOHook;
	m_LowIOHook = NULL;
	if (AK::IAkStreamMgr::Get())
	{
		// main problem here is it crashes on exit with a unknown memory leak
		AK::IAkStreamMgr::Get()->Destroy();
	}

	AK::MemoryMgr::Term();
}

void CAudioSystem::ClearActiveGameObjs()
{
	AK::SoundEngine::UnregisterAllGameObj();
}

void CAudioSystem::RegisterGameObj(AkGameObjectID in_gameObj)
{
	AK::SoundEngine::RegisterGameObj(in_gameObj);
}

void CAudioSystem::UnRegisterGameObj(AkGameObjectID in_gameObj)
{
	AK::SoundEngine::UnregisterGameObj(in_gameObj);
}

void CAudioSystem::LoadBankFile(const wchar_t* inBankname,AkBankID &bnkId, AKRESULT &out_ErrorCheck)
{
	out_ErrorCheck = AK::SoundEngine::LoadBank(inBankname, AK_DEFAULT_POOL_ID, bnkId);
}

void CAudioSystem::UnloadBankFile(const wchar_t * inBankname, AkBankID & bnkId, AKRESULT &out_ErrorCheck)
{
	out_ErrorCheck = AK::SoundEngine::UnloadBank(inBankname,NULL, NULL);
}

void CAudioSystem::SetListener(AkGameObjectID in_ListenerID, AkUInt32 _instanceNumb, AKRESULT &out_ErrorCheck)
{
	out_ErrorCheck = AK::SoundEngine::SetDefaultListeners(&in_ListenerID,  _instanceNumb);
}


void CAudioSystem::ClearAllActiveBnks()
{
	AK::SoundEngine::ClearBanks();
}


void CAudioSystem::PlaySoundInBank()
{
	AK::SoundEngine::RenderAudio();
}
// Fin
void CAudioSystem::SendSoundsToEngine(const AkUniqueID uID, AkGameObjectID obj_ID)
{
	AK::SoundEngine::PostEvent(uID, obj_ID);
}

void CAudioSystem::SetBanksFolderPath(const AkOSChar * _inFilePath)
{
	m_LowIOHook->SetBasePath(_inFilePath);
}
//Find by File name 
void CAudioSystem::SendSoundsToEngine(const wchar_t * soundName, AkGameObjectID  obj_ID)
{
	AK::SoundEngine::PostEvent(soundName, obj_ID);
}
