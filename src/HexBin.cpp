#include "HexBin.h"
#include <bitset>
#include <istream>
#include <sstream>
#include <fstream>
#include <ostream>

std::string hexBytetoBin(const std::string& hex)
{
    unsigned int num = std::stoul(hex, nullptr, 16);
    return std::bitset<8>(num).to_string();
}

std::string toBin(const std::string& hex)
{
    if (hex.size() < 2 || hex.size() % 2 != 0)
        return "-1";
    std::string out;
    for (int i = 0; i < hex.size(); i += 2)
    {
        std::string byte;
        std::memcpy(&byte[0], &hex[i], 2);
        out += hexBytetoBin(byte);
    }
    return out;
}
//gets rid of the colons
std::string deweirdify(const std::string& hex)
{
    std::string out;
    for (char c : hex)
        if (c != ':' && c != '\n')
            out += c;
    return out;
}

std::string constructBin(const char* filename)
{
    std::ifstream stream{ filename };
    if (!stream.is_open())
        return "-1";
    std::stringstream filecontentsbuf;
    filecontentsbuf << stream.rdbuf();
    std::string contents = deweirdify(filecontentsbuf.str());
    return toBin(contents);
}

std::string constructHex(const char* filename)
{
    std::ifstream stream{ filename };
    if (!stream.is_open())
        return "-1";
    std::stringstream filecontentsbuf;
    filecontentsbuf << stream.rdbuf();
    std::string contents = deweirdify(filecontentsbuf.str());
    return contents;

}


std::string string_to_hex(const std::string& input)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(input.length() * 2);
    for (unsigned char c : input)
    {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}
