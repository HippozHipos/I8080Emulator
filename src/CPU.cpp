#include "CPU.h"

#include <chrono>
#include <thread>
#include "Opcodes.h"
#include "MEM.h"

CPU::CPU()
{
	start = false;
}

//assumes memory has been loaded into ram (think of disk being loaded into memory)
void CPU::startExecAt(Memory* mem) 
{

}

void CPU::execute_opcode(opcode* cur_op, Memory* mem)
{

	while (!start) {}

	//opcode* cur_op = opcodeLookup[index].get(); 
	// DO NOT USE .GET() IT USES A TEMPORARY AS A RETURN VALUE
	// THAT MEANS YOU THEN TRY TO ASSIGN A DESTROYED TEMPORARY WHEN YOU EXIT THE SCOPE OF .GET()
	//std::shared_ptr<opcode> cur_op = opcodeLookup[index];
	if constexpr (LimitCycles)
	{
		timer.Reset();
		//std::cout << cur_op->disassemble_to_string(this, mem) << "\n";
		cur_op->execute(this, mem);
		int cyclefreq = (1 / this->ProcesserFrequency) * 1000000000;
		this->PC += cur_op->m_size; //NOt 100 percent sure this is right 
		timer.Sleep((cyclefreq * cur_op->m_cycle_duration) - timer.Time());
		//add timer.accumulate, so we can see how long it takes to execute every instruction
	}
	else
	{
		cur_op->execute(this, mem);
		this->PC += cur_op->m_size;
	}
	//return cur_op;
}

std::string CPU::RegToStr(int r)
{
	static const char* reg[] = { "A", "B", "C", "BC", "D", "E", "DE", "H", "L", "HL", "SP" };
	return reg[r];
}

void CPU::toggleStart()
{

	this->start = !this->start; //add a check in CPU to check if start is toggled or not

}

void CPU::load_register_value(Registers reg, uint8_t* value)
{
	switch (reg)
	{
	case Registers::A:
	{
		this->A = value[1];
	} break;
	case Registers::B:
	{
		this->B = value[1];
	} break;
	case Registers::C:
	{
		this->C = value[1];
	} break;
	case Registers::BC:
	{
		this->C = value[1];
		this->B = value[2];
	}
	case Registers::D:
	{
		this->D = value[1];
	} break;
	case Registers::E:
	{
		this->E = value[1];
	} break;
	case Registers::DE:
	{
		this->E = value[1];
		this->D = value[2];
	} break;
	case Registers::H:
	{
		this->H = value[1];
	} break;
	case Registers::L:
	{
		this->L = value[2];
	} break;
	case Registers::HL:
	{
		this->L = value[1];
		this->H = value[2];
	} break;
	case Registers::SP:
	{
		//NOT TESTED
		//SP.low = byte 2
		*((uint8_t*)&this->SP + 1) = value[1];//get the FIRST 8 BIT (SP is 16 bit) address of the SP, add 1 (go to the next byte), derefence it, then assign
												//this->SP << value[0];
		//SP.high = byte 3
		this->SP = value[2];

	} break;
	default:
	{
		printf("INVALID REGISTER FOR fn load_register_value");
	} break;
	}
}

