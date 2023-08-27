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
	Memory mem;
	ErrorCode e;
	mem.DirectWriteByte < Memory::Type::RAM>(0x200, 1, e);
	mem.DirectWriteByte< Memory::Type::RAM>(0x201, 2, e);
	std::cout << mem.DirectReadBytes<Memory::Type::RAM, 2>(0x200, e) << '\n';
	PrintDebugMsg(e);
	CPU cpu;
	std::shared_ptr<opcode> currentOpcode = std::make_shared<LXI>(Registers::A);
	std::cout << currentOpcode->to_string(&cpu, &mem) << '\n';
	cpu.execute_opcode(currentOpcode, &mem);
}

int main()
{
	test();

	Application* app = new Application();
	app->Run();
	delete app;
}


