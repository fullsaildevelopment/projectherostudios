// Villain CES.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Villain CES.h"
#include"Timer.h"

#define MAX_LOADSTRING 100
#define FarPlane 100000
#define FOV 60
// Global Variables:
HINSTANCE cTheWindowInstance;                                // current instance
HWND cTheWindow;
WCHAR achTitleBarText[MAX_LOADSTRING];                  // The title bar text
WCHAR achWindowClassName[MAX_LOADSTRING];            // the main window class name
CAuger* pcAugerInstance;
CFPS fpsTimer;
bool loopGame;
//Also need this for memory leak code stuff

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, achTitleBarText, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_VILLAINCES, achWindowClassName, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
#ifndef NDEBUG
	AllocConsole();
	FILE* new_std_in_out;
	freopen_s(&new_std_in_out, "CONOUT$", "w", stdout);
	freopen_s(&new_std_in_out, "CONIN$", "r", stdin);
	std::cout << "Hello world!\n";
#endif
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
	int tmpDbgFlag;
	float fps = 0.0f;
	// Set the debug-heap flag to keep freed blocks in the
	// heap's linked list - This will allow us to catch any
	// inadvertent use of freed memory
	tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpDbgFlag);
	_CrtSetBreakAlloc(-1); //Important!
	HACCEL cAcceleratorTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VILLAINCES));
	MSG cMessage;
	pcAugerInstance = new CAuger(cTheWindow);
	pcAugerInstance->Start();
	fpsTimer.Init_FPSReader();

	loopGame = true;
	while (loopGame)
	{
		//Begin timestamp for beginning of frame
		fpsTimer.Xtime_Signal();
		// Process all messages, stop on WM_QUIT
		if (PeekMessage(&cMessage, NULL, 0, 0, PM_REMOVE))
		{
			// WM_QUIT does not need to be
			// translated or dispatched
			if (cMessage.message == WM_QUIT)
				break;
			// Translates messages and sends them
			// to WndProc
			TranslateMessage(&cMessage);
			DispatchMessage(&cMessage);
		}
		else
		{
			pcAugerInstance->Update(&loopGame);
			fpsTimer.UpdateFrameTime();		
#if MUSIC_ON
			CAudioSystem::PlaySoundInBank();
#endif
		}
	}

	pcAugerInstance->End();
	delete pcAugerInstance;

#ifndef NDEBUG
	FreeConsole();
#endif
	return (int)cMessage.wParam;
}

#pragma region Windows Stuff
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW cWindowSettings;

	cWindowSettings.cbSize = sizeof(WNDCLASSEX);

	cWindowSettings.style = CS_HREDRAW | CS_VREDRAW;
	cWindowSettings.lpfnWndProc = WndProc;
	cWindowSettings.cbClsExtra = 0;
	cWindowSettings.cbWndExtra = 0;
	cWindowSettings.hInstance = hInstance;
	cWindowSettings.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VILLAINCES));
	cWindowSettings.hCursor = LoadCursor(nullptr, IDC_ARROW);
	cWindowSettings.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	cWindowSettings.lpszMenuName = NULL;
	cWindowSettings.lpszClassName = achWindowClassName;
	cWindowSettings.hIconSm = LoadIcon(cWindowSettings.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&cWindowSettings);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	cTheWindowInstance = hInstance; // Store instance handle in our global variable

	cTheWindow = CreateWindowW(achWindowClassName, achTitleBarText, WS_OVERLAPPEDWINDOW^ WS_THICKFRAME^WS_MAXIMIZEBOX,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!cTheWindow)
	{
		return FALSE;
	}

	ShowWindow(cTheWindow, nCmdShow);
	UpdateWindow(cTheWindow);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND cTheWindow, UINT nWindowMessage, WPARAM cWindowParameters, LPARAM cExtraWindowParameters)
{
	switch (nWindowMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(cTheWindow, nWindowMessage, cWindowParameters, cExtraWindowParameters);
	}
	return 0;
}

#pragma endregion