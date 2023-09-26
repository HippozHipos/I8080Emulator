#include "PlatformWin32.h" 
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "../src/dx11/dx11.h"
#include <d3d11.h>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main Simulation, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main Simulation, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your Simulation based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (dx11::g_pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
        {
            dx11::CleanupRenderTarget();
            dx11::g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            dx11::CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT Simulation menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

PlatformWin32::PlatformWin32(const std::wstring& name, int x, int y, int w, int h)
{

	WNDCLASSEXW wc = 
	{
		sizeof(wc),
		CS_CLASSDC, 
		WndProc, 
		0L, 
		0L, 
		GetModuleHandle(nullptr),
		nullptr,
		nullptr, 
		nullptr, 
		nullptr,
		classname.c_str(),
		nullptr 
	};

	::RegisterClassExW(&wc);

	this->hwnd = ::CreateWindowW(classname.c_str(), name.c_str(), WS_OVERLAPPEDWINDOW, x, y, w, h, nullptr, nullptr, wc.hInstance, nullptr);
    
    this->instance = GetModuleHandle(nullptr);

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    //::UpdateWindow(hwnd);
}

PlatformWin32::~PlatformWin32()
{
    ::DestroyWindow(hwnd);
    //::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    ::UnregisterClassW(this->classname.c_str(), this->instance);
	ImGui_ImplWin32_Shutdown();
}

void pw32_HandleMessages (bool done)
{
    MSG msg;
    while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        if (msg.message == WM_QUIT)
            done = true;
    }
}