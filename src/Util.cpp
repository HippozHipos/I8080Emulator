#include "Util.h"
#include <format>

std::string toUpper(const std::string& input)
{
	std::string result = input;
	for (char& c : result) {
		c = std::toupper(c);
	}
	return result;
}

std::string toHexStr(uint16_t input)
{
	return "0x" + toUpper(std::format("{:x}", input));
}

int numberOfSetBits(uint32_t i)
{
	// Java: use int, and use >>> instead of >>. Or use Integer.bitCount()
	// C or C++: use uint32_t
	i = i - ((i >> 1) & 0x55555555);        // add pairs of bits
	i = (i & 0x33333333) + ((i >> 2) & 0x33333333);  // quads
	i = (i + (i >> 4)) & 0x0F0F0F0F;        // groups of 8
	return (i * 0x01010101) >> 24;          // horizontal sum of bytes
}

/*bool BeginCentered(const char* name)
{
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 pos(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
    ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_AlwaysAutoResize
        | ImGuiWindowFlags_NoSavedSettings;
    return ImGui::Begin(name, nullptr, flags);
}*/