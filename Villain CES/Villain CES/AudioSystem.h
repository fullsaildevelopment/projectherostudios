#pragma once
//SDK files from Libraries.
#include<malloc.h>
#include"AK/SoundEngine/Common/AkMemoryMgr.h"
#include"AK/SoundEngine/Common/AkModule.h"
#include"AK/SoundEngine/Common/AkTypes.h"
#include"AK/SoundEngine/Common/IAkStreamMgr.h"
#include"AK/SoundEngine/Platforms/Windows/AkWinSoundEngine.h"
#include"AK/SoundEngine/Common/AkStreamMgrModule.h"
#include"AK/MusicEngine/Common/AkMusicEngine.h"
#include"AK/SoundEngine/Common/AkSoundEngine.h"
#include"AK/SpatialAudio/Common/AkSpatialAudio.h"
//File Package
#include"Win32/AkDefaultIOHookBlocking.h"
#include"Common/AkFilePackageLowLevelIO.h"
#include"Common/AkDefaultLowLevelIODispatcher.h"
#include"Common/AkFileLocationBase.h"
#include"Common/AkFilePackage.h"
#include"Win32/AkFilePackageLowLevelIOBlocking.h"
#include"Common/AkFilePackageLUT.h"

#ifndef AK_OPTIMIZED
// Only needed for debug mode
#include <AK/Comm/AkCommunication.h>
#endif //AK_OPTIMIZED
namespace AK
{
	// reason these are inline that way they will not defined in multiple .obj files for our .exe
#ifdef WIN32
	inline void * AllocHook(size_t in_size)
	{
		return malloc(in_size);
	}
	inline void FreeHook(void * in_ptr)
	{
		free(in_ptr);
	}
	// Note: VirtualAllocHook() may be used by I/O pools of the default implementation
	// of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
	// - refer to the Windows SDK documentation for more details). This is NOT mandatory;
	// you may implement it with a simple malloc().
	inline void * VirtualAllocHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwAllocationType,
		DWORD in_dwProtect
	)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}
	inline void VirtualFreeHook(
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
using namespace AKPLATFORM;
class CAudioSystem 
{
public:
	CAudioSystem();
	~CAudioSystem();
	void IntiializeSystem();
	void PlaySoundInBank(CAudioSystem* mySound);
	void SendSoundsToEngine(AkUniqueID &uID, AkGameObjectID &obj_ID);
	void SendSoundsToEngine(const char* soundName, AkGameObjectID &obj_ID);

private:
	CAkFilePackageLowLevelIOBlocking * m_LowIOHook;
};




