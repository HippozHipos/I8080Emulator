#ifndef GUI_H
#define GUI_H

#include <array>

#include "../thirdparty/imgui/imgui.h"
#include "../thirdparty/imgui_club/imgui_memory_editor.h"

class MemoryViewer
{
public:
	MemoryViewer() = default;

public:
	void setup(std::array<uint8_t, 0xFFFF>* ram);
	//void setup(uint8_t** ram);
	void draw_memory_viewer();
	void draw_debugger();
	void draw_emulator_screen();

private:
	std::array<uint8_t, 0xFFFF>* m_Memory = nullptr;
	MemoryEditor m_CPU_Ram_Memory;
};

#endif