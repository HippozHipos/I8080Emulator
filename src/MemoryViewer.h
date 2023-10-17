#ifndef GUI_H
#define GUI_H

#include <array>

#include "../thirdparty/imgui/imgui.h"
#include "../thirdparty/imgui_club/imgui_memory_editor.h"
#include "CPU.h"
#include <deque>
#include <string>

using std::deque;
using std::string;

class MemoryViewer
{
public:
	MemoryViewer() = default;

public:
	void setup(std::array<uint8_t, 0xFFFF>* ram);
	void setup_cpu(CPU* cpu);
	//void setup(uint8_t** ram);
	void draw_memory_viewer();
	void draw_debugger();
	void draw_register_info();
	void draw_last_exec_opcodes();

	void draw_cpu_ctrl_buttons();
	void draw_startstop_button();

	void draw_emulator_screen();

	void addToOpcodesDeque(string opcode);
	void getshittyfuckingregisters();

private:
	std::array<uint8_t, 0xFFFF>* m_Memory = nullptr;
	deque<string> last128opcodes;
	std::shared_ptr<CPU> cpu;
	MemoryEditor m_CPU_Ram_Memory;
};

#endif