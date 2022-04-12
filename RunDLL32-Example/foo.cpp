/*
    @file foo.cpp
    @author Skip Cherniss
    @github https://github.com/skip-cherniss/RunDLL32-Example

    This file contains two different functions which are exported and can be called from rundll32
        popup (Can only be dynamically loaded)
        sayHello
    This framework can be copied and tweaked to support redteam testing.
    The companion DLL, RunDLL32-StartJump, is contained in this solution
    Read the function summaries below for more detail
*/
#include <iostream>
#include  "myCode.h"
#include "foo.h"

#undef UNICODE
#define UNICODE
#include <windows.h>

// **************************************************************************************
// * rundll32.exe RunDLL32-Example.dll, sayHello test test test
// **************************************************************************************

/// <summary>
/// RunDLL32-Example.dll popup function
/// The popup function can be launched to display a message from the windows message box function
/// This function can only be loaded are called inside the same project. The function definition
/// does not support being calledby rundll32
/// 
/// Format:
/// popup <messsage to pass to popup>, #
/// 
/// Example with parameters:
/// popup Seabees CAN DO!!, 2
/// </summary>
/// <param name="lpszMsg">the message to display</param>
/// <param name="loopCount">the number of times to display the message</param>
/// <returns></returns>
int __cdecl popUp(LPWSTR lpszMsg, int loopCount)
{
    DWORD const infoBoxOptions = MB_ICONINFORMATION | MB_SETFOREGROUND;

    if (loopCount < 0 || loopCount > 5)
        loopCount = 1;

    for (int i = 0; i < loopCount; i++)
    {
        std::wstring aString = L"DLL message:";
        std::wstring aConcat = aString + std::to_wstring(i+1);
        LPCWSTR msgTitle = aConcat.c_str();

        MessageBox(0, lpszMsg , msgTitle, infoBoxOptions);
    }    

    return 0;
}

/// <summary>
/// RunDLL32-Example.dll sayHello function
/// The popup function can be launched from rundll32.exe to display a message from the cmdline
/// 
/// Format:
/// rundll32.exe RunDLL32-Example.dll, sayHello <messsage to pass to cmdline>
/// 
/// Example with parameters:
/// rundll32.exe RunDLL32-Example.dll, sayHello hello1 hello2 hello3
/// </summary>
/// <param name="hwnd"></param>
/// <param name="hinst"></param>
/// <param name="lpszCmdLine">the message to display</param>
/// <param name="nCmdShow"></param>
void CALLBACK sayHello(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
    AllocConsole();

    FILE* fDummy;
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONOUT$", "w", stdout);

    DWORD const infoBoxOptions = MB_ICONINFORMATION | MB_SETFOREGROUND;
    MessageBox(0, L"Before call...", L"DLL message:", infoBoxOptions);
    
    if (strlen(lpszCmdLine) > 0)
    {
        int argCount = 0;
        argCount = myStrings::delimiter_count(lpszCmdLine, ' ') + 1;
        std::string* a = new std::string[argCount];

        myStrings::split(lpszCmdLine, ' ', a);

        for (int i = 0; i < argCount; i++)
        {
            std::cout << "Argument #:";
            std::cout << std::to_string(i);
            std::cout << " ";
            std::cout << a[i] << std::endl;
        }
        
        std::cout << lpszCmdLine << std::endl;
    }
    else
    {
        myCode::sayHello();
    }
    //std::cout << "Hello, world!" << std::endl;
    MessageBox(0, L"After call...", L"DLL message:", infoBoxOptions);

    std::cout.clear();

    FreeConsole();

}