#include <iostream>

//3rd party includes
#include "thirdparty/imgui/imgui.h"

//src includes
#include "src/PlatformWin/PlatformWin32.h" 
#include "src/MemoryViewer.h"
#include "src/dx11/dx11.h"
#include "src/imgooey/imgooey.h"
#include "Application.h"

#include "CPU.h"
#include "Opcodes.h"

#include "MEM.h"

void test()
{

	std::string bin = "00000000";

	ErrorCode e;

	Memory mem;
	CPU cpu;
	std::shared_ptr<opcode> currentOpcode = std::make_shared<LXI>(Registers::A);
	std::cout << currentOpcode->disassemble_to_string(&cpu, &mem) << '\n';
	cpu.execute_opcode(currentOpcode, &mem);
	cpu.setFlags(currentOpcode.m_reg); //there is currently no way to get the current register. this needs to be implemented for this to work

	//typedef void (opcode::* execute)(CPU*, Memory*);
}

int main()
{
	test();

	Application* app = new Application();
	app->Run();
	delete app;
}
