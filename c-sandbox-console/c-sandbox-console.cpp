// c-sandbox-console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

int main()
{
    std::cout << "Hello World!\n";

	STARTUPINFO startInfo = { 0 };

	PROCESS_INFORMATION processInfo = { 0 };

	memset(&startInfo, 0, sizeof(startInfo));
	memset(&processInfo, 0, sizeof(processInfo));

	/*
	LPCSTR is a pointer to a const string (LP means Long Pointer)
	LPCTSTR is a pointer to a const TCHAR string, (TCHAR being either a wide char or char depending on whether UNICODE is defined in your project)
	LPTSTR is a pointer to a (non-const) TCHAR string
	LP == Long Pointer. Just think pointer or char*

	C = Const, in this case, I think they mean the character string is a const, not the pointer being const.
	STR is string
	the T is for a wide character or char (TCHAR) depending on compiler options.


	#ifdef UNICODE
	#define CreateProcess  CreateProcessW
	#else
	#define CreateProcess  CreateProcessA
	#endif // !UNICODE

	CreateProcessA(
		_In_opt_ LPCSTR lpApplicationName,
		_Inout_opt_ LPSTR lpCommandLine,
		_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
		_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
		_In_ BOOL bInheritHandles,
		_In_ DWORD dwCreationFlags,
		_In_opt_ LPVOID lpEnvironment,
		_In_opt_ LPCSTR lpCurrentDirectory,
		_In_ LPSTARTUPINFOA lpStartupInfo,
		_Out_ LPPROCESS_INFORMATION lpProcessInformation
		);

	*/

	std::string stringtoconvert;

	std::wstring temp = std::wstring(stringtoconvert.begin(), stringtoconvert.end());
	LPWSTR lpwstr = (LPWSTR)"";
	LPCWSTR lpcwstr = temp.c_str();

	//BOOL bSuccess = CreateProcess(
	//	TEXT("C:\\Windows\\System32\\notepad.exe"),
	//	lpwstr,
	//	NULL,
	//	NULL,
	//	FALSE,
	//	NULL,
	//	NULL,
	//	NULL,
	//	&startInfo,
	//	&processInfo);


	BOOL bSuccess = CreateProcess(
		TEXT("C:\\Windows\\System32\\rundll32.exe "),
		lpwstr,
		NULL,
		NULL,
		FALSE,
		NULL,
		NULL,
		NULL,
		&startInfo,
		&processInfo);

	//wait till the proc ends
	WaitForSingleObject(processInfo.hProcess, INFINITE);

	//close all
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	getchar();
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
