#pragma once
#include <memory>
#include <array>
#include "MemoryViewer.h"
#include "MEM.h"
#include "Platformwin/PlatformWin32.h"
#include "HexBin.h"
#include "CPU.h"
#include "Error.h"
#include "BUS.h"

//Simulation includes all the GUI and the machine we will be simulation. 
//It contains the interface that the user will be interacting/viewing with.
//Two objects of the simulation class will and should never be able to interact with each other.

class Simulation
{
public:
    Simulation();
    ~Simulation();
    void constructMemory();
    void InitOpcodeTable();

    void Run();

private:
    //SHOULD BE HANDLED BY EMULATOR
    //std::array<uint8_t, 0xFFFF> test_ram{};
    Memory mem      ;
    CPU cpu         ;
    //BUS bus         ;
    ErrorCode e     ;
    // Forward declarations of helper functions
    std::unique_ptr<MemoryViewer> memview{ new MemoryViewer() };
    // Create Simulation window
    PlatformWin32 window{ L"i8080_emulator", 100, 100, 600, 600 };
    std::shared_ptr<opcode> opcodeLookup[256]; //this equiveleant to opcode* opcodeLookup[255]
};
