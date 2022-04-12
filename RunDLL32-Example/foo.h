#include <windows.h>

#undef UNICODE
#define UNICODE

extern "C" __declspec(dllexport) int __cdecl popUp(LPWSTR, int);

extern "C" __declspec(dllexport) void CALLBACK sayHello(HWND, HINSTANCE, LPSTR, int);
