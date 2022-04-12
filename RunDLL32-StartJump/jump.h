#include <windows.h>

#undef UNICODE
#define UNICODE

extern "C" __declspec(dllexport) void CALLBACK jump(HWND, HINSTANCE, LPSTR, int);

extern "C" __declspec(dllexport) void CALLBACK jumpload(HWND, HINSTANCE, LPSTR, int);
