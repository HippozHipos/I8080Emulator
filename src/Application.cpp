#include "Application.h"

#include "imgooey/imgooey.h"
#include "dx11/dx11.h"

Application::Application()
{

        // Initialize Direct3D
        dx11::Init(window.hwnd);

        // Setup Dear ImGui context
        imgooey::Init(window.hwnd);

        //OR works because we can set 32 bits of an int. e.g. 0000 -> 0010 sets 1 flag, 0011 sets different flags...etc.
        imgooey::SetFlags(ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_NavEnableKeyboard);

        memview->setup(&this->test_ram);
}

Application::~Application()
{
    imgooey::Destroy();
    dx11::Destroy();
}

void Application::Run()
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