#include <iostream>

//3rd party includes
#include "thirdparty/imgui/imgui.h"

//src includes
#include "src/PlatformWin/PlatformWin32.h" 
#include "src/MemoryViewer.h"
#include "src/dx11/dx11.h"
#include "src/imgooey/imgooey.h"
#include "Application.h"

#include "CPU.h"
#include "Opcodes.h"

#include "MEM.h"

#include <string>
#include <bitset>
#include <iostream>
#include <sstream>
#include <fstream>

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

void test()
{

    std::string filelocation = "C:/Users/Alex/Desktop/test.hex";

	ErrorCode e;

	Memory mem;
	CPU cpu;
	/*std::shared_ptr<opcode> currentOpcode = std::make_shared<LXI>(Registers::A);
	std::cout << currentOpcode->disassemble_to_string(&cpu, &mem) << '\n';
	cpu.execute_opcode(currentOpcode, &mem);*/
    auto bufofshit = constructHex(filelocation.c_str());

    for(int i = 0; i <= bufofshit.size()-1; i+=2)
    {
        std::string h;
        h += bufofshit[i];
        h += bufofshit[i + 1];
        uint8_t value = std::stoi(h);
        mem.DirectWriteBytes<1, Memory::Type::RAM>((i/2), value, e);
    }

    
        uint8_t currentOp = mem.DirectReadBytes(cpu.PC, e);
        std::cout << (int) currentOp << "\n";
        cpu.execute_opcode(currentOp, &mem);
        count++;
    
    
    //std::cout << currentOpcode->disassemble_to_string(&cpu, &mem) << '\n';

	//uint8_t currentOpcode = ;
	//cpu.execute_opcode(currentOpcode, &mem);
	
	//typedef void (opcode::* execute)(CPU*, Memory*);
}

int main()
{
	test();

	Application* app = new Application();
	app->Run();
	delete app;
}
