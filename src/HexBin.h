#pragma once

#include <string>

//helper function => converts hex bytes (2 decimals) to a binary string.
std::string hexBytetoBin(const std::string& hex);
//helper function => converts hex file into binary string.
std::string toBin(const std::string& hex);
//removes : and \n from the binary or hex strings.
std::string deweirdify(const std::string& hex);
//
std::string constructBin(const char* filename);
std::string constructHex(const char* filename);
std::string string_to_hex(const std::string& input);