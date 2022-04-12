#include <iostream>
#include <windows.h>
#include "jump.h"
#include <string>
#include <filesystem>
#include <codecvt>

#undef UNICODE
#define UNICODE

// demonstrating function with two parameters
typedef int(__cdecl* POPUP)(LPWSTR,int);

void CALLBACK jumpload(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
	HINSTANCE hinstLib;
	POPUP procPopUp;
	BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;
	
	AllocConsole();

	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);

	// Get a handle to the DLL module.
	// This could also be passed in on the command line
	hinstLib = LoadLibrary(TEXT("RunDLL32-Example.dll"));

	// If the handle is valid, try to get the function address.

	if (hinstLib != NULL)
	{
		// This is the function we are looking for
		// The name of the function could also be passed in on the command line
		procPopUp = (POPUP)GetProcAddress(hinstLib, "popUp");

		// If the function address is valid, call the function.

		if (NULL != procPopUp)
		{
			fRunTimeLinkSuccess = TRUE;

			std::wstring cmdLine = L"Messaage Follows:";
			cmdLine += std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(lpszCmdLine);
			wchar_t* wcs = const_cast<wchar_t*>(cmdLine.c_str());

			(procPopUp)(wcs,2);
		}
		// Free the DLL module.
		fFreeResult = FreeLibrary(hinstLib);
	}

	// If unable to call the DLL function, use an alternative.
	if (!fRunTimeLinkSuccess)
		std::cout << "Message printed from executable" << std::endl;


	std::cout.clear();
	FreeConsole();
}

/// <summary>
/// This example is called from rundll32, creates another process rundll32 and starts a different dll
/// </summary>
/// <param name="hwnd"></param>
/// <param name="hinst"></param>
/// <param name="lpszCmdLine"></param>
/// <param name="nCmdShow"></param>
void CALLBACK jump(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
	AllocConsole();

	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);

	
	STARTUPINFO startInfo = { 0 };

	PROCESS_INFORMATION processInfo = { 0 };

	memset(&startInfo, 0, sizeof(startInfo));
	memset(&processInfo, 0, sizeof(processInfo));

	// >rundll32.exe RunDLL32-Example.dll, sayHello "test test test"
	/*
		NULL,   // No module name (use command line)
		argv[1],        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory
		&si,            // Pointer to STARTUPINFO structure
		&pi )           // Pointer to PROCESS_INFORMATION structure
	*/

	// Completely works - This will not require passing in parameters, this is hard coded
	// Sourced From https://social.msdn.microsoft.com/Forums/en-US/dd6770ed-b53e-45bc-8943-e3fea4c3fc61/how-to-wait-for-child-process-when-createprocess-creates-2-process-viz-run32dll-process-which?forum=vcgeneral
	//wchar_t appSt[] = L"rundll32 \"D:\\@Snapshot\\@RedTeamPlayground\\RunDLL32-Example\\Debug\\RunDLL32-Example.dll\" sayHello";	
	
	// Completely works
	// must use starting directory, this will not require passing in parameters this is hard coded
	// TEXT("D:\\@Snapshot\\@RedTeamPlayground\\RunDLL32-Example\\Debug")
	//wchar_t appSt[] = L"rundll32 RunDLL32-Example.dll sayHello hello1 hello2 hello3";
			
	std::wstring cmdLine = L"rundll32 ";
	cmdLine += std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(lpszCmdLine);

	// https://cboard.cprogramming.com/cplusplus-programming/78614-std-wstring-wchar_t-*.html
	wchar_t* wcs =  const_cast<wchar_t*>(cmdLine.c_str());
		
	// rundll32.exe RunDLL32-StartJump.dll, jump RunDLL32-Example.dll sayHello hello1 hello2 hello3
	// rundll32.exe RunDLL32-StartJump.dll, jump RunDLL32-Example.dll sayHello
	
	BOOL bSuccess = CreateProcess(
		NULL,
		wcs,
		NULL,						// Process handle not inheritable
		NULL,						// Thread handle not inheritable
		FALSE,						// Set handle inheritance to FALSE
		0,							// No creation flags						
		NULL,						// Use parent's environment block
		TEXT("D:\\@Snapshot\\@RedTeamPlayground\\RunDLL32-Example\\Debug"),						// Starting directory, when null, use parent
		&startInfo,					// Pointer to STARTUPINFO structure
		&processInfo);				// Pointer to PROCESS_INFORMATION structure
	
	
	if (bSuccess == false)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		//return;
		std::cout << lpszCmdLine << std::endl;
		
		/*TCHAR NPath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, NPath);
		*/

		WCHAR buffer[MAX_PATH];
		
		GetModuleFileName( NULL, buffer, MAX_PATH);

		std::cout << "Current working directory: " << buffer << std::endl;

	}
	else
	{
		//wait till the proc ends
		WaitForSingleObject(processInfo.hProcess, 10000);

		//close all
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
		
	}

	std::cout << lpszCmdLine << std::endl;

	std::string response;

	std::cout << "Would you like to play a game?" << std::endl;
	std::cin >> response;

	/*std::string response;

	std::cout << "Would you like to play a game?" << std::endl;
	std::cin >> response;*/

	std::cout.clear();

	FreeConsole();
}