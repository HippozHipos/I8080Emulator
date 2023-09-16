#pragma once
#include <memory>
#include <array>
#include "MemoryViewer.h"
#include "Platformwin/PlatformWin32.h"

class Application
{
public:
    Application();
    ~Application();

    void Run();

private:
    //SHOULD BE HANDLED BY EMULATOR
    std::array<uint8_t, 0xFFFF> test_ram{};
    // Forward declarations of helper functions
    std::unique_ptr<MemoryViewer> memview{ new MemoryViewer() };
    // Create application window
    PlatformWin32 window{ L"i8080_emulator", 100, 100, 600, 600 };
};
