#pragma once
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN 
#endif
#include <Windows.h> 
#include <string> 

#ifndef UNICODE
    #define UNICODE
#endif

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void pw32_HandleMessages(bool);

struct PlatformWin32
{
    PlatformWin32(const std::wstring& name, int x, int y, int w, int h);
    PlatformWin32(const PlatformWin32&) = delete;
    ~PlatformWin32();

    PlatformWin32 operator=(const PlatformWin32&) = delete;

    std::wstring classname = L"i8080_Emulator";

    HINSTANCE instance;
    HWND hwnd;
};
