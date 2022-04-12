// File [foo.cpp]
#include <iostream>
#include  "myCode.h"

#undef UNICODE
#define UNICODE
#include <windows.h>

// **************************************************************************************
// * rundll32.exe RunDLL32-Example.dll, sayHello test test test
// **************************************************************************************
extern "C" __declspec(dllexport) int __cdecl popUp(LPWSTR, int);

extern "C" __declspec(dllexport) void CALLBACK sayHello(HWND, HINSTANCE, LPSTR, int);

/// <summary>
/// example cmd line call rundll32.exe RunDLL32-StartJump.dll, jumpload Seabees CAN DO!!
/// </summary>
/// <param name="lpszMsg"></param>
/// <param name="loopCount"></param>
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

//extern "C"
//__declspec(dllexport)
//void CALLBACK sayHello(HWND, HINSTANCE, wchar_t const*, int)

//void CALLBACK sayHello(HWND, HINSTANCE, LPSTR, int)
void CALLBACK sayHello(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
    
    // Severity	Code Error	C4996	
    // Description          'freopen': This function or variable may be unsafe.Consider using freopen_s instead.To disable deprecation, use _CRT_SECURE_NO_WARNINGS.See online help for details.RunDLL32 - Example	
    // Project	File        D:\@Snapshot\@RedTeamPlayground\RunDLL32 - Example\RunDLL32 - Example\foo.cpp		
    // Line	23
    /*
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    */

    /*
    
     _ACRTIMP FILE* __cdecl freopen(
        _In_z_  char const* _FileName,
        _In_z_  char const* _Mode,
        _Inout_ FILE*       _Stream
        );

    _Check_return_wat_
        _ACRTIMP errno_t __cdecl freopen_s(
            _Outptr_result_maybenull_ FILE * *_Stream,
            _In_z_                    char const* _FileName,
            _In_z_                    char const* _Mode,
            _Inout_                   FILE * _OldStream
        );
    */

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

    /*
    FILE* fpin;
    FILE* fpout;

    AllocConsole();

    freopen_s(&fpin, "CONIN$", "r", stdin);
    freopen_s(&fpout, "CONOUT$", "w", stdout);
    freopen_s(&fpout, "CONOUT$", "w", stdout);
    
    DWORD const infoBoxOptions = MB_ICONINFORMATION | MB_SETFOREGROUND;
    MessageBox(0, L"Before call...", L"DLL message:", infoBoxOptions);
    myCode::sayHello();
    MessageBox(0, L"After call...", L"DLL message:", infoBoxOptions);

    std::cout.clear();
    fclose(fpin);
    fclose(fpout);
    
    FreeConsole();
    */
    // https://stackoverflow.com/questions/15543571/allocconsole-not-displaying-cout

}