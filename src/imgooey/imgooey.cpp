#include "imgooey.h"
#include "../../thirdparty/imgui/imgui.h"
#include "../../thirdparty/imgui/backends/imgui_impl_dx11.h"
#include "../../thirdparty/imgui/backends/imgui_impl_win32.h"
#include "../dx11/dx11.h"


void imgooey::CheckVer()
{
    IMGUI_CHECKVERSION();
}

void imgooey::Init(HWND window) 
{
        ImGui::CreateContext();
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // Setup Platform/Renderer backends
        ImGui_ImplWin32_Init(window);
        ImGui_ImplDX11_Init(dx11::g_pd3dDevice, dx11::g_pd3dDeviceContext);
}

void imgooey::SetFlags(int flags)
{
    ImGuiIO & io = ImGui::GetIO(); 
    (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags = flags;
}

void imgooey::Begin()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void imgooey::End() 
{
    ImGui::Render();
}

void imgooey::Destroy()
{
	// Cleanup
	ImGui::DestroyContext();
	ImGui_ImplDX11_Shutdown();
}
