#include "MemoryViewer.h"

#include "../thirdparty/imgui/imgui.h"


void MemoryViewer::setup(std::array<uint8_t, 0xFFFF>* memoryToView)
{
	auto style = &ImGui::GetStyle();

	style->AntiAliasedFill = 0;
	style->AntiAliasedLines = 0;
	style->AntiAliasedLinesUseTex = 0;

	//style->FramePadding = ImVec2(20, 20);

	style->WindowMinSize = { 24, 24 };
	style->WindowPadding = { 0, 0 };
	style->WindowRounding = 3;
	style->WindowBorderSize = 0;

	style->PopupRounding = 3.f;
	style->PopupBorderSize = 0.f;

	style->ScrollbarSize = 7;

	style->GrabMinSize = 1;

	style->Colors[ImGuiCol_WindowBg] = ImColor{ 35, 35, 35 };

	ImGui::GetIO().FontDefault = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 14);

	m_Memory = memoryToView;
}

void MemoryViewer::draw_memory_viewer()
{
	m_CPU_Ram_Memory.DrawWindow("RAM", m_Memory->data(), 0xFFFF);
}

void MemoryViewer::draw_debugger()
{
}

void MemoryViewer::draw_emulator_screen()
{
}
