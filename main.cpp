#include <iostream>

//3rd party includes
#include "thirdparty/imgui/imgui.h"

//src includes
#include "src/win32/PlatformWin32.h" 
#include "src/MemoryViewer.h"
#include "src/dx11/dx11.h"
#include "src/imgooey/imgooey.h"
#include "Application.h"

#include "CPU.h"
#include "Opcodes.h"

#include "Error.h"

#include "MEM.h"

void test()
{	
	Memory mem;
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


