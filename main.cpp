#include <iostream>

//3rd party includes
#include "thirdparty/imgui/imgui.h"

//src includes
#include "src/PlatformWin/PlatformWin32.h" 
#include "src/MemoryViewer.h"
#include "src/dx11/dx11.h"
#include "src/imgooey/imgooey.h"
#include "Simulation.h"

#include "CPU.h"
#include "Opcodes.h"

#include "MEM.h"

#include <string>
#include <bitset>
#include <iostream>
#include <sstream>
#include <fstream>


int main()
{
	Simulation* sim = new Simulation();
	sim->Run();
	delete sim;
}
