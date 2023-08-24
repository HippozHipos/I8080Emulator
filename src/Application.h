#pragma once

class Application
{
public:
    Application()
    {

        // Initialize Direct3D
        dx11::Init(window.hwnd);

        // Setup Dear ImGui context
        imgooey::Init(window.hwnd);

        //OR works because we can set 32 bits of an int. e.g. 0000 -> 0010 sets 1 flag, 0011 sets different flags...etc.
        imgooey::SetFlags(ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_NavEnableKeyboard);

        memview->setup(&test_ram);
    }

    ~Application()
    {
        imgooey::Destroy();
        dx11::Destroy();
    }

public:
    void Run()
    {
        // Main loop
        bool done = false;
        while (!done)
        {
            // Poll and handle messages (inputs, window resize, etc.)
            // See the WndProc() function below for our to dispatch events to the Win32 backend.
            pw32_HandleMessages(done);
            if (done)
                break;
            dx11::ClearScreen(0.45f, 0.55f, 0.60f, 1.00f);
            imgooey::Begin();

            // Draw Here
            memview->draw_memory_viewer();

            imgooey::End();
            dx11::Render();
        }

    }

private:
    //SHOULD BE HANDLED BY EMULATOR
    std::array<uint8_t, 0xFFFF> test_ram{};
    // Forward declarations of helper functions
    std::unique_ptr<MemoryViewer> memview{ new MemoryViewer() };
    // Create application window
    PlatformWin32 window{ L"i8080_emulator", 100, 100, 600, 600 };
};