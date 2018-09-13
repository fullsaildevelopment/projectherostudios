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
#include"AK/Tools/Win32/AkPlatformFuncs.h"
#include"WwiseSounds/Wwise_IDs.h"
//File Package
#include"Win32/AkDefaultIOHookBlocking.h"
#include"Common/AkFilePackageLowLevelIO.h"
#include"Common/AkDefaultLowLevelIODispatcher.h"
#include"Common/AkFileLocationBase.h"
#include"Common/AkFilePackage.h"
#include"Win32/AkFilePackageLowLevelIOBlocking.h"
#include"Common/AkFilePackageLUT.h"

#define INIT_BNK L"Init.bnk"
#define FOOTSTEP_BNK L"Bus3d_Demo.bnk"
//#define AK_OPTIMIZED
//#ifndef AK_OPTIMIZED
//// Only needed for debug mode
//#include <AK/Comm/AkCommunication.h>
//#endif //AK_OPTIMIZED
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
	void IntiializeSystem(AKRESULT &out_ErrorCheck);
	void TermSoundEngine();

	static void PlaySoundInBank();
	void SendSoundsToEngine(const AkUniqueID uID, AkGameObjectID obj_ID);
	void SetBanksFolderPath(const AkOSChar* _inFilePath);
	void SendSoundsToEngine(const wchar_t* soundName, AkGameObjectID obj_ID);
	void ClearActiveGameObjs();
	void RegisterGameObj(AkGameObjectID in_gameObj);
	void UnRegisterGameObj(AkGameObjectID in_gameObj);
	void LoadBankFile(const wchar_t* inBankname, AkBankID &bnkId, AKRESULT &out_ErrorCheck);
	void UnloadBankFile(const wchar_t* inBankname, AkBankID &bnkId, AKRESULT &out_ErrorCheck);
	void SetListener(AkGameObjectID in_ListenerID, AkUInt32 _instanceNumb, AKRESULT &out_ErrorCheck);
	void ClearAllActiveBnks();
private:
	CAkFilePackageLowLevelIOBlocking * m_LowIOHook;
};
// C:\Program Files %28x86%29\Audiokinetic\Wwise 2018.1.0.6714\SDK\x64_vc150\Debug\lib



