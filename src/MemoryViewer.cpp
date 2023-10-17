#include "MemoryViewer.h"

#include <memory>
#include "../thirdparty/imgui/imgui.h"

bool CPU::start;

//incase we switch back to std::array<uint8_t, 0xFFFF>* memoryToView
//then just going to keep this placeholder: void MemoryViewer::setup(std::array<uint8_t, 0xFFFF>* memoryToView)
void MemoryViewer::setup(std::array<uint8_t, 0xFFFF>* memoryToView)
{
	auto style = &ImGui::GetStyle();

	style->AntiAliasedFill = 0;
	style->AntiAliasedLines = 0;
	style->AntiAliasedLinesUseTex = 0;

	//style->FramePadding = ImVec2(20, 20);

	style->WindowMinSize = { 24, 24 };
	style->WindowPadding = { 0, 0 };
	style->WindowRounding = 3;
	style->WindowBorderSize = 0;

	style->PopupRounding = 3.f;
	style->PopupBorderSize = 0.f;

	style->ScrollbarSize = 7;

	style->GrabMinSize = 1;

	style->Colors[ImGuiCol_WindowBg] = ImColor{ 35, 35, 35 };

	ImGui::GetIO().FontDefault = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 14);

	m_Memory = memoryToView;
}

void MemoryViewer::setup_cpu(CPU* cpu)
{
	this->cpu = std::make_shared<CPU>(*cpu);
}

void MemoryViewer::draw_memory_viewer()
{
	m_CPU_Ram_Memory.DrawWindow("RAM", m_Memory->data(), 0xFFFF);
}

void MemoryViewer::draw_register_info()
{
	std::string instr_info = "Current intruction: " + std::to_string(cpu->PC) + "\n Next Byte:\n"; //need to get cpu->get_next_instruction()
	std::string reg_info = "Registers: \n\nA:" + std::to_string(cpu->A) + "\nB: " + std::to_string(cpu->B) 
                                                + "\nC: " + std::to_string(cpu->C) + "\nD: " + std::to_string(cpu->D) + "\nE: " 
                                                + std::to_string(cpu->E) + "\nH: " + std::to_string(cpu->H) + "\nL: " + std::to_string(cpu->L);
    std::string comb_reg_info = "\nBC: " + std::to_string(cpu->BC) + "\nDE: " + std::to_string(cpu->DE) + "\nHL: " + std::to_string(cpu->HL);
    std::string extra_regs_info = "\nPC: " + std::to_string(cpu->PC) + "\nSP: " + std::to_string(cpu->SP) + "\n";

	auto to_print = instr_info + reg_info + comb_reg_info + extra_regs_info;

	ImGui::Begin("CPU Info");

	ImGui::Text(to_print.c_str());

    draw_cpu_ctrl_buttons();
    draw_startstop_button();

	ImGui::End();
}

void MemoryViewer::draw_last_exec_opcodes()
{
	ImGui::Begin("Last Instructions");
	for (size_t i = 0; i < last128opcodes.size(); i++)
	{
		ImGui::Text(last128opcodes.at(i).c_str());
	}
	
	ImGui::End();
}

/*
static void ShowExampleMenuFile()
{
    //IMGUI_DEMO_MARKER("Examples/Menu");
    ImGui::MenuItem("(demo menu)", NULL, false, false);
    if (ImGui::MenuItem("New")) {}
    if (ImGui::MenuItem("Open", "Ctrl+O")) {}
    if (ImGui::BeginMenu("Open Recent"))
    {
        ImGui::MenuItem("fish_hat.c");
        ImGui::MenuItem("fish_hat.inl");
        ImGui::MenuItem("fish_hat.h");
        if (ImGui::BeginMenu("More.."))
        {
            ImGui::MenuItem("Hello");
            ImGui::MenuItem("Sailor");
            if (ImGui::BeginMenu("Recurse.."))
            {
                ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Save", "Ctrl+S")) {}
    if (ImGui::MenuItem("Save As..")) {}

    ImGui::Separator();
    //IMGUI_DEMO_MARKER("Examples/Menu/Options");
    if (ImGui::BeginMenu("Options"))
    {
        static bool enabled = true;
        ImGui::MenuItem("Enabled", "", &enabled);
        ImGui::BeginChild("child", ImVec2(0, 60), true);
        for (int i = 0; i < 10; i++)
            ImGui::Text("Scrolling Text %d", i);
        ImGui::EndChild();
        static float f = 0.5f;
        static int n = 0;
        ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
        ImGui::InputFloat("Input", &f, 0.1f);
        ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
        ImGui::EndMenu();
    }

    //IMGUI_DEMO_MARKER("Examples/Menu/Colors");
    if (ImGui::BeginMenu("Colors"))
    {
        float sz = ImGui::GetTextLineHeight();
        for (int i = 0; i < ImGuiCol_COUNT; i++)
        {
            const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
            ImVec2 p = ImGui::GetCursorScreenPos();
            ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
            ImGui::Dummy(ImVec2(sz, sz));
            ImGui::SameLine();
            ImGui::MenuItem(name);
        }
        ImGui::EndMenu();
    }

    // Here we demonstrate appending again to the "Options" menu (which we already created above)
    // Of course in this demo it is a little bit silly that this function calls BeginMenu("Options") twice.
    // In a real code-base using it would make senses to use this feature from very different code locations.
    if (ImGui::BeginMenu("Options")) // <-- Append!
    {
        //IMGUI_DEMO_MARKER("Examples/Menu/Append to an existing menu");
        static bool b = true;
        ImGui::Checkbox("SomeOption", &b);
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Disabled", false)) // Disabled
    {
        IM_ASSERT(0);
    }
    if (ImGui::MenuItem("Checked", NULL, true)) {}
    ImGui::Separator();
    if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}
*/

/*
void MemoryViewer::draw_cpu_ctrl_buttons()
{
    if (ImGui::Button("Set CPU Clock in MHZ"))
    {
        if (Button(element, kElementWidth, kElementHeight, window))
        {
            ImGui::OpenPopup(element.m_Name);
        }
        if (ImGui::BeginPopupModal(element.m_Name, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            const float popup_width = 100.f;
            const float popup_height = 70.f;
            constexpr float button_width = 190.f * 0.7f;
            constexpr float button_height = 60.f * 0.7f;

            ImGui::BeginChild("popup", ImVec2(popup_width, popup_height));
            ElementPopup(element);
            ImGui::EndChild();

            char buf[255]{};
            buf[255] = { '\0' };
            if (ImGui::InputText("Input Clock speed", buf, 255))
            {

            }

            if (ImGui::Button("Ok", ImVec2(button_width, button_height)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
}
*/
void MemoryViewer::draw_cpu_ctrl_buttons()
{
    if (ImGui::Button("Set CPU Clock in MHz"))
    {
        ImGui::OpenPopup("CPU Speed");
    }

    if (ImGui::BeginPopupModal("CPU Speed", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        // Center-align the text
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Enter CPU clock speed in MHZ").x) * 0.5f);
        ImGui::Text("Enter CPU clock speed in MHZ");
        ImGui::Separator();

        static char inputBuffer[32] = "Enter Speed"; // Initial placeholder text
        static bool isEditing = false;

        if (!isEditing && strcmp(inputBuffer, "Enter Speed") == 0)
        {
            // Clear the initial placeholder text when user starts typing
            inputBuffer[0] = '\0';
        }

        // ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("SpeedInput").x * 0.5f);
        isEditing = ImGui::InputText("##SpeedInput", inputBuffer, sizeof(inputBuffer) / sizeof(inputBuffer[0]),
            ImGuiInputTextFlags_EnterReturnsTrue);

        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            // Handle the OK button click, e.g., store the inputBuffer content
            // and close the popup
            // Example: speedValue = inputBuffer;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            // Handle the Cancel button click if needed and close the popup
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void MemoryViewer::draw_startstop_button()
{
    //TODO: Add display to tell if the program is currently paused or not.
    if (ImGui::Button("Start/Stop"))
    {
        cpu->toggleStart(); //add a check in CPU to check if start is toggled or not
    }
    /* 
    Find out if this is possible in imgui implicitly:
      static bool isOn = true;
          if(isOn == false)
          {
             toggleButtonText->Text = "Off";
          }
          else
          {
             toggleButtonText->Text = "On";
          }
          isOn = !isOn; //toggle the flag too
    */
}


void MemoryViewer::draw_debugger()
{
}

void MemoryViewer::draw_emulator_screen()
{
}

void MemoryViewer::addToOpcodesDeque(string opcode)
{
	if (last128opcodes.size() == 128) last128opcodes.pop_front();
	last128opcodes.emplace_back(opcode);
}
