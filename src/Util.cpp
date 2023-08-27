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
