#include "Simulation.h"
#include "Opcodes.h"

#include "imgooey/imgooey.h"
#include "dx11/dx11.h"

//Create Reader functions to pull out all the sections of memory, e.g. if you want RAM block we can create a function to pull it all out.

Simulation::Simulation()
{
        
        // Initialize Direct3D
        dx11::Init(window.hwnd);

        // Setup Dear ImGui context
        imgooey::Init(window.hwnd);

        //OR works because we can set 32 bits of an int. e.g. 0000 -> 0010 sets 1 flag, 0011 sets different flags...etc.
        imgooey::SetFlags(ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_NavEnableKeyboard);

        memview->setup(&this->mem.m_Memory);
		memview->setup_cpu(&this->cpu);

		InitOpcodeTable();
}

Simulation::~Simulation()
{
    imgooey::Destroy();
    dx11::Destroy();
}

void Simulation::constructMemory()
{
    std::string filelocation = "C:/Users/Alex/Desktop/test.hex";
    std::string toPrintLastInstructions;

    //std::shared_ptr<opcode> currentOpcode = std::make_shared<LXI>(Registers::A);
    //std::cout << currentOpcode->disassemble_to_string(&cpu, &mem) << '\n';
    //cpu.execute_opcode(currentOpcode, &mem);
    auto bufofshit = constructHex(filelocation.c_str());

    for (int i = 0; i <= bufofshit.size() - 1; i += 2)
    {
        std::string h;
        h += bufofshit[i];
        h += bufofshit[i + 1];
        uint8_t value = std::stoi(h); 
        mem.DirectWriteBytes<1, Memory::Type::RAM>((i / 2), value, e);
    }

    for (int i = 0; i <= (bufofshit.size() / 2) - 1; i += 2)
    {
        uint8_t currentOpIndex = mem.DirectReadBytes(cpu.PC, e);
		std::shared_ptr<opcode> cur_op = opcodeLookup[currentOpIndex];
        //std::cout << (int)currentOpIndex << "\n";
		if (cur_op->m_size == 1)
			toPrintLastInstructions += " - - ";
		else if (cur_op->m_size == 2) 
		{
			std::cout << std::string((int)*mem.GetRawPtr(cpu.PC + 1, e), 1) << '\n';
			toPrintLastInstructions += std::string((int)*mem.GetRawPtr(cpu.PC + 1, e), 1) + " - ";
		}
		else
		{
			auto var = std::to_string(*mem.GetRawPtr(cpu.PC + 1, e));
			toPrintLastInstructions += std::to_string(*mem.GetRawPtr(cpu.PC + 1, e))
															+ std::to_string((int) *mem.GetRawPtr(cpu.PC + 2, e));
		}
		memview->addToOpcodesDeque(toHexStr(mem.GetLwrBnd<Memory::RAM>() + cpu.PC) + "  | " + toPrintLastInstructions + " | " + cur_op->disassemble_to_string(&cpu, &mem));
        cpu.execute_opcode(cur_op.get(), &mem);
		//the format of the string should be ADDRESS DATA OPCODE_NAME
		//ideally we have ADDRESS OPCODE DATE OPCODE_NAME
		toPrintLastInstructions.clear();
    }
}


void Simulation::Run()
{
	this->cpu.toggleStart();
    constructMemory();

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
		memview->draw_last_exec_opcodes();
		memview->draw_register_info();

        imgooey::End();
        dx11::Render();
    }

}

void Simulation::InitOpcodeTable()
{

	//std::make_shared ensure we don't instantiate new pointers
	//0-0f opcodes, 0-15 opcodes
	opcodeLookup[0x00] = std::make_shared<NOP>(); //this is essentially equivelent to: opcodeLookup[0x00] = new Nop();
	opcodeLookup[0x01] = std::make_shared<LXI>(Registers::BC);
	opcodeLookup[0x02] = std::make_shared<STAX>(Registers::BC);
	opcodeLookup[0x03] = std::make_shared<INX>(Registers::BC);
	opcodeLookup[0x04] = std::make_shared<INR>(Registers::B);
	opcodeLookup[0x05] = std::make_shared<DCR>(Registers::B);
	/*opcodeLookup[0x06] = std::make_shared<MVI>(Registers::B);
	opcodeLookup[0x07] = std::make_shared<RLC>();
	opcodeLookup[0x08] = std::make_shared<NOP>();
	opcodeLookup[0x09] = std::make_shared<DAD>(Registers::B);
	opcodeLookup[0x0A] = std::make_shared<LDAX>(Registers::B);
	opcodeLookup[0x0B] = std::make_shared<DCX>(Registers::BC);
	opcodeLookup[0x0C] = std::make_shared<INR>(Registers::C);
	opcodeLookup[0x0D] = std::make_shared<DCR>(Registers::C);
	opcodeLookup[0x0E] = std::make_shared<MVI>();
	opcodeLookup[0x0F] = std::make_shared<RRC>();*/

	//10-1F opcodes, 16-31 opcodes
	opcodeLookup[0x10] = std::make_shared<NOP>();
	opcodeLookup[0x11] = std::make_shared<LXI>(Registers::DE);
	opcodeLookup[0x12] = std::make_shared<STAX>(Registers::DE);
	opcodeLookup[0x13] = std::make_shared<INX>(Registers::DE);
	opcodeLookup[0x14] = std::make_shared<INR>(Registers::D);
	opcodeLookup[0x15] = std::make_shared<DCR>(Registers::D);
	//opcodeLookup[0x16] = std::make_shared<MVI>(Registers::D);
	/*opcodeLookup[0x17] = std::make_shared<RAL>();
	opcodeLookup[0x18] = std::make_shared<NOP>();
	opcodeLookup[0x19] = std::make_shared<DAD>();
	opcodeLookup[0x1A] = std::make_shared<LDAX>(Registers::DE);
	opcodeLookup[0x1B] = std::make_shared<DCX >();
	opcodeLookup[0x1C] = std::make_shared<INR >();
	opcodeLookup[0x1D] = std::make_shared<DCR >();
	opcodeLookup[0x1E] = std::make_shared<MVI >();
	opcodeLookup[0x1F] = std::make_shared<RAR>();

	//20-2F opcodes. 32-47 opcodes
	opcodeLookup[0x20] = std::make_shared<NOP>();
	opcodeLookup[0x21] = std::make_shared<LXI >(Registers::HL);
	opcodeLookup[0x22] = std::make_shared<NOP>();
	opcodeLookup[0x23] = std::make_shared<NOP>();
	opcodeLookup[0x24] = std::make_shared<NOP>();
	opcodeLookup[0x25] = std::make_shared<NOP>();
	opcodeLookup[0x26] = std::make_shared<NOP>();
	opcodeLookup[0x27] = std::make_shared<NOP>();
	opcodeLookup[0x28] = std::make_shared<NOP>();
	opcodeLookup[0x29] = std::make_shared<NOP>();
	opcodeLookup[0x2A] = std::make_shared<NOP>();
	opcodeLookup[0x2B] = std::make_shared<NOP>();
	opcodeLookup[0x2C] = std::make_shared<NOP>();
	opcodeLookup[0x2D] = std::make_shared<NOP>();
	opcodeLookup[0x2E] = std::make_shared<NOP>();
	opcodeLookup[0x2F] = std::make_shared<NOP>();

	//30-3F opcodes, 48-63 opcodes
	opcodeLookup[0x30] = std::make_shared<NOP>();
	opcodeLookup[0x31] = std::make_shared<NOP>();
	opcodeLookup[0x32] = std::make_shared<NOP>();
	opcodeLookup[0x33] = std::make_shared<NOP>();
	opcodeLookup[0x34] = std::make_shared<NOP>();
	opcodeLookup[0x35] = std::make_shared<NOP>();
	opcodeLookup[0x36] = std::make_shared<NOP>();
	opcodeLookup[0x37] = std::make_shared<NOP>();
	opcodeLookup[0x38] = std::make_shared<NOP>();
	opcodeLookup[0x39] = std::make_shared<NOP>();
	opcodeLookup[0x3A] = std::make_shared<NOP>();
	opcodeLookup[0x3B] = std::make_shared<NOP>();
	opcodeLookup[0x3C] = std::make_shared<NOP>();
	opcodeLookup[0x3D] = std::make_shared<NOP>();
	opcodeLookup[0x3E] = std::make_shared<NOP>();
	opcodeLookup[0x3F] = std::make_shared<NOP>();

	//40-4F opcodes 64-79 opcodes
	opcodeLookup[0x40] = std::make_shared<MOV>();
	opcodeLookup[0x41] = std::make_shared<MOV>();
	opcodeLookup[0x42] = std::make_shared<MOV>();
	opcodeLookup[0x43] = std::make_shared<MOV>();
	opcodeLookup[0x44] = std::make_shared<MOV>();
	opcodeLookup[0x45] = std::make_shared<MOV>();
	opcodeLookup[0x46] = std::make_shared<MOV>();
	opcodeLookup[0x47] = std::make_shared<MOV>();
	opcodeLookup[0x48] = std::make_shared<MOV>();
	opcodeLookup[0x49] = std::make_shared<MOV>();
	opcodeLookup[0x4A] = std::make_shared<MOV>();
	opcodeLookup[0x4B] = std::make_shared<MOV>();
	opcodeLookup[0x4C] = std::make_shared<MOV>();
	opcodeLookup[0x4D] = std::make_shared<MOV>();
	opcodeLookup[0x4E] = std::make_shared<MOV>();
	opcodeLookup[0x4F] = std::make_shared<MOV>();

	//50-5F opcodes 80-95 opcodes
	opcodeLookup[0x50] = std::make_shared<MOV>();
	opcodeLookup[0x51] = std::make_shared<MOV>();
	opcodeLookup[0x52] = std::make_shared<MOV>();
	opcodeLookup[0x53] = std::make_shared<MOV>();
	opcodeLookup[0x54] = std::make_shared<MOV>();
	opcodeLookup[0x55] = std::make_shared<MOV>();
	opcodeLookup[0x56] = std::make_shared<MOV>();
	opcodeLookup[0x57] = std::make_shared<MOV>();
	opcodeLookup[0x58] = std::make_shared<MOV>();
	opcodeLookup[0x59] = std::make_shared<MOV>();
	opcodeLookup[0x5A] = std::make_shared<MOV>();
	opcodeLookup[0x5B] = std::make_shared<MOV>();
	opcodeLookup[0x5C] = std::make_shared<MOV>();
	opcodeLookup[0x5D] = std::make_shared<MOV>();
	opcodeLookup[0x5E] = std::make_shared<MOV>();
	opcodeLookup[0x5F] = std::make_shared<MOV>();

	//60-6F opcodes 96-111 opcodes
	opcodeLookup[0x60] = std::make_shared<MOV>();
	opcodeLookup[0x61] = std::make_shared<MOV>();
	opcodeLookup[0x62] = std::make_shared<MOV>();
	opcodeLookup[0x63] = std::make_shared<MOV>();
	opcodeLookup[0x64] = std::make_shared<MOV>();
	opcodeLookup[0x65] = std::make_shared<MOV>();
	opcodeLookup[0x66] = std::make_shared<MOV>();
	opcodeLookup[0x67] = std::make_shared<MOV>();
	opcodeLookup[0x68] = std::make_shared<MOV>();
	opcodeLookup[0x69] = std::make_shared<MOV>();
	opcodeLookup[0x6A] = std::make_shared<MOV>();
	opcodeLookup[0x6B] = std::make_shared<MOV>();
	opcodeLookup[0x6C] = std::make_shared<MOV>();
	opcodeLookup[0x6D] = std::make_shared<MOV>();
	opcodeLookup[0x6E] = std::make_shared<MOV>();
	opcodeLookup[0x6F] = std::make_shared<MOV>();

	//70-7F opcodes 112-127 opcodes
	opcodeLookup[0x70] = std::make_shared<MOV>();
	opcodeLookup[0x71] = std::make_shared<MOV>();
	opcodeLookup[0x72] = std::make_shared<MOV>();
	opcodeLookup[0x73] = std::make_shared<MOV>();
	opcodeLookup[0x74] = std::make_shared<MOV>();
	opcodeLookup[0x75] = std::make_shared<MOV>();
	opcodeLookup[0x76] = std::make_shared<MOV>();
	opcodeLookup[0x77] = std::make_shared<MOV>();
	opcodeLookup[0x78] = std::make_shared<MOV>();
	opcodeLookup[0x79] = std::make_shared<MOV>();
	opcodeLookup[0x7A] = std::make_shared<MOV>();
	opcodeLookup[0x7B] = std::make_shared<MOV>();
	opcodeLookup[0x7C] = std::make_shared<MOV>();
	opcodeLookup[0x7D] = std::make_shared<MOV>();
	opcodeLookup[0x7E] = std::make_shared<MOV>();
	opcodeLookup[0x7F] = std::make_shared<MOV>();
	*/
	//80-8F opcodes 128-143 opcodes
	opcodeLookup[0x80] = std::make_shared<NOP>();
	opcodeLookup[0x81] = std::make_shared<NOP>();
	opcodeLookup[0x82] = std::make_shared<NOP>();
	opcodeLookup[0x83] = std::make_shared<NOP>();
	opcodeLookup[0x84] = std::make_shared<NOP>();
	opcodeLookup[0x85] = std::make_shared<NOP>();
	opcodeLookup[0x86] = std::make_shared<NOP>();
	opcodeLookup[0x87] = std::make_shared<NOP>();
	opcodeLookup[0x88] = std::make_shared<NOP>();
	opcodeLookup[0x89] = std::make_shared<NOP>();
	opcodeLookup[0x8A] = std::make_shared<NOP>();
	opcodeLookup[0x8B] = std::make_shared<NOP>();
	opcodeLookup[0x8C] = std::make_shared<NOP>();
	opcodeLookup[0x8D] = std::make_shared<NOP>();
	opcodeLookup[0x8E] = std::make_shared<NOP>();
	opcodeLookup[0x8F] = std::make_shared<NOP>();

	//90-9F opcodes 144-159 opcodes
	opcodeLookup[0x90] = std::make_shared<NOP>();
	opcodeLookup[0x91] = std::make_shared<NOP>();
	opcodeLookup[0x92] = std::make_shared<NOP>();
	opcodeLookup[0x93] = std::make_shared<NOP>();
	opcodeLookup[0x94] = std::make_shared<NOP>();
	opcodeLookup[0x95] = std::make_shared<NOP>();
	opcodeLookup[0x96] = std::make_shared<NOP>();
	opcodeLookup[0x97] = std::make_shared<NOP>();
	opcodeLookup[0x98] = std::make_shared<NOP>();
	opcodeLookup[0x99] = std::make_shared<NOP>();
	opcodeLookup[0x9A] = std::make_shared<NOP>();
	opcodeLookup[0x9B] = std::make_shared<NOP>();
	opcodeLookup[0x9C] = std::make_shared<NOP>();
	opcodeLookup[0x9D] = std::make_shared<NOP>();
	opcodeLookup[0x9E] = std::make_shared<NOP>();
	opcodeLookup[0x9F] = std::make_shared<NOP>();

	//A0-AF opcodes 160-175 opcodes
	opcodeLookup[0xA0] = std::make_shared<NOP>();
	opcodeLookup[0xA1] = std::make_shared<NOP>();
	opcodeLookup[0xA2] = std::make_shared<NOP>();
	opcodeLookup[0xA3] = std::make_shared<NOP>();
	opcodeLookup[0xA4] = std::make_shared<NOP>();
	opcodeLookup[0xA5] = std::make_shared<NOP>();
	opcodeLookup[0xA6] = std::make_shared<NOP>();
	opcodeLookup[0xA7] = std::make_shared<NOP>();
	opcodeLookup[0xA8] = std::make_shared<NOP>();
	opcodeLookup[0xA9] = std::make_shared<NOP>();
	opcodeLookup[0xAA] = std::make_shared<NOP>();
	opcodeLookup[0xAB] = std::make_shared<NOP>();
	opcodeLookup[0xAC] = std::make_shared<NOP>();
	opcodeLookup[0xAD] = std::make_shared<NOP>();
	opcodeLookup[0xAE] = std::make_shared<NOP>();
	opcodeLookup[0xAF] = std::make_shared<NOP>();

	//B0-BF opcodes 176-191 opcodes
	opcodeLookup[0xB0] = std::make_shared<NOP>();
	opcodeLookup[0xB1] = std::make_shared<NOP>();
	opcodeLookup[0xB2] = std::make_shared<NOP>();
	opcodeLookup[0xB3] = std::make_shared<NOP>();
	opcodeLookup[0xB4] = std::make_shared<NOP>();
	opcodeLookup[0xB5] = std::make_shared<NOP>();
	opcodeLookup[0xB6] = std::make_shared<NOP>();
	opcodeLookup[0xB7] = std::make_shared<NOP>();
	opcodeLookup[0xB8] = std::make_shared<NOP>();
	opcodeLookup[0xB9] = std::make_shared<NOP>();
	opcodeLookup[0xBA] = std::make_shared<NOP>();
	opcodeLookup[0xBB] = std::make_shared<NOP>();
	opcodeLookup[0xBC] = std::make_shared<NOP>();
	opcodeLookup[0xBD] = std::make_shared<NOP>();
	opcodeLookup[0xBE] = std::make_shared<NOP>();
	opcodeLookup[0xBF] = std::make_shared<NOP>();

	//C0-CF opcodes 192-207 opcodes 
	opcodeLookup[0xC0] = std::make_shared<NOP>();
	opcodeLookup[0xC1] = std::make_shared<NOP>();
	opcodeLookup[0xC2] = std::make_shared<NOP>();
	opcodeLookup[0xC3] = std::make_shared<NOP>();
	opcodeLookup[0xC4] = std::make_shared<NOP>();
	opcodeLookup[0xC5] = std::make_shared<NOP>();
	opcodeLookup[0xC6] = std::make_shared<NOP>();
	opcodeLookup[0xC7] = std::make_shared<NOP>();
	opcodeLookup[0xC8] = std::make_shared<NOP>();
	opcodeLookup[0xC9] = std::make_shared<NOP>();
	opcodeLookup[0xCA] = std::make_shared<NOP>();
	opcodeLookup[0xCB] = std::make_shared<NOP>();
	opcodeLookup[0xCC] = std::make_shared<NOP>();
	opcodeLookup[0xCD] = std::make_shared<NOP>();
	opcodeLookup[0xCE] = std::make_shared<NOP>();
	opcodeLookup[0xCF] = std::make_shared<NOP>();

	//D0-DF opcodes 208-223 opcodes
	opcodeLookup[0xD0] = std::make_shared<NOP>();
	opcodeLookup[0xD1] = std::make_shared<NOP>();
	opcodeLookup[0xD2] = std::make_shared<NOP>();
	opcodeLookup[0xD3] = std::make_shared<NOP>();
	opcodeLookup[0xD4] = std::make_shared<NOP>();
	opcodeLookup[0xD5] = std::make_shared<NOP>();
	opcodeLookup[0xD6] = std::make_shared<NOP>();
	opcodeLookup[0xD7] = std::make_shared<NOP>();
	opcodeLookup[0xD8] = std::make_shared<NOP>();
	opcodeLookup[0xD9] = std::make_shared<NOP>();
	opcodeLookup[0xDA] = std::make_shared<NOP>();
	opcodeLookup[0xDB] = std::make_shared<NOP>();
	opcodeLookup[0xDC] = std::make_shared<NOP>();
	opcodeLookup[0xDD] = std::make_shared<NOP>();
	opcodeLookup[0xDE] = std::make_shared<NOP>();
	opcodeLookup[0xDF] = std::make_shared<NOP>();

	//E0-EF opcodes 224-239 opcodes
	opcodeLookup[0xE0] = std::make_shared<NOP>();
	opcodeLookup[0xE1] = std::make_shared<NOP>();
	opcodeLookup[0xE2] = std::make_shared<NOP>();
	opcodeLookup[0xE3] = std::make_shared<NOP>();
	opcodeLookup[0xE4] = std::make_shared<NOP>();
	opcodeLookup[0xE5] = std::make_shared<NOP>();
	opcodeLookup[0xE6] = std::make_shared<NOP>();
	opcodeLookup[0xE7] = std::make_shared<NOP>();
	opcodeLookup[0xE8] = std::make_shared<NOP>();
	opcodeLookup[0xE9] = std::make_shared<NOP>();
	opcodeLookup[0xEA] = std::make_shared<NOP>();
	opcodeLookup[0xEB] = std::make_shared<NOP>();
	opcodeLookup[0xEC] = std::make_shared<NOP>();
	opcodeLookup[0xED] = std::make_shared<NOP>();
	opcodeLookup[0xEE] = std::make_shared<NOP>();
	opcodeLookup[0xEF] = std::make_shared<NOP>();

	//F0-FF opcodes 240-255 opcodes
	opcodeLookup[0xF0] = std::make_shared<NOP>();
	opcodeLookup[0xF1] = std::make_shared<NOP>();
	opcodeLookup[0xF2] = std::make_shared<NOP>();
	opcodeLookup[0xF3] = std::make_shared<NOP>();
	opcodeLookup[0xF4] = std::make_shared<NOP>();
	opcodeLookup[0xF5] = std::make_shared<NOP>();
	opcodeLookup[0xF6] = std::make_shared<NOP>();
	opcodeLookup[0xF7] = std::make_shared<NOP>();
	opcodeLookup[0xF8] = std::make_shared<NOP>();
	opcodeLookup[0xF9] = std::make_shared<NOP>();
	opcodeLookup[0xFA] = std::make_shared<NOP>();
	opcodeLookup[0xFB] = std::make_shared<NOP>();
	opcodeLookup[0xFC] = std::make_shared<NOP>();
	opcodeLookup[0xFD] = std::make_shared<NOP>();
	opcodeLookup[0xFE] = std::make_shared<NOP>();
	opcodeLookup[0xFF] = std::make_shared<NOP>();
}