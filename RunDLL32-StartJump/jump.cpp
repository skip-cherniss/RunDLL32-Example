/*
	@file jump.cpp
	@author Skip Cherniss
	@github https://github.com/skip-cherniss/RunDLL32-Example

	This file contains two different functions which are exported and can be called from rundll32
		jumpload
		jump
	This framework can be copied and tweaked to support redteam testing.
	The companion DLL, RunDLL32-Example, is contained in this solution
	Read the function summaries below for more detail
*/

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

/// <summary>
/// RunDLL32-StartJump.dll jumpload function
/// The jumpload function can be launched from rundll32.exe to load a different dll into this process
/// The second dll is hardcoded to RunDLL32-Example in this routine. 
/// You can remove the hard coding and parameterize the dll
/// 
/// Format:
/// rundll32.exe RunDLL32-StartJump.dll, jumpload <messsage to pass to RunDLL32-Example.dll popup>
/// 
/// Example with parameters:
/// rundll32.exe RunDLL32-StartJump.dll, jumpload Seabees CAN DO!!
/// 
/// Example without parameters:
/// rundll32.exe RunDLL32-StartJump.dll, jump RunDLL32-Example.dll sayHello
/// </summary>
/// <param name="hwnd"></param>
/// <param name="hinst"></param>
/// <param name="lpszCmdLine">The command line parameter. Everything after "rundll32.exe RunDLL32-StartJump.dll, jump"</param>
/// <param name="nCmdShow"></param>
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
/// RunDLL32-StartJump.dll jump function
/// The jump function can be launched from rundll32.exe to start a different dll with rundll32.exe
/// Format:
/// rundll32.exe RunDLL32-StartJump.dll, jump <name of second DLL> <function> <paramter list separated by spaces>
/// 
/// Example with parameters:
/// rundll32.exe RunDLL32-StartJump.dll, jump RunDLL32-Example.dll sayHello hello1 hello2 hello3
/// 
/// Example without parameters:
/// rundll32.exe RunDLL32-StartJump.dll, jump RunDLL32-Example.dll sayHello
/// 
/// </summary>
/// <param name="hwnd"></param>
/// <param name="hinst"></param>
/// <param name="lpszCmdLine">The command line parameter. Everything after "rundll32.exe RunDLL32-StartJump.dll, jump"</param>
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
				
	std::wstring cmdLine = L"rundll32 ";
	cmdLine += std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(lpszCmdLine);

	// It is necessary to change the data type from string to wchar_t when passing this value to CreateProcess
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

	// Use the following for testing the jump framework
	// When you are certain this is linking properly to your secondary dll, remove the following lines
	std::cout << lpszCmdLine << std::endl;

	std::string response;

	std::cout << "Would you like to play a game?" << std::endl;
	std::cin >> response;


	// Keep the following lines
	std::cout.clear();
	FreeConsole();
}