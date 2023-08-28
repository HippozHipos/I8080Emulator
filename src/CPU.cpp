#include "CPU.h"

#include <chrono>
#include <thread>
#include "Opcodes.h"
#include "MEM.h"

void CPU::execute_opcode(std::shared_ptr<opcode> cur_op, Memory* mem)
{
	if constexpr (LimitCycles)
	{
		timer.Reset();
		cur_op->execute(this, mem);
		//1 cycle takes 500 nano seconds (1 / 2,000,000) * 1,000,000,000
		timer.Sleep((500 * cur_op->m_cycle_duration) - timer.Time());
		this->PC += cur_op->m_size; //NOt 100 percent sure this is right 
	}
	else
	{
		cur_op->execute(this, mem);
		this->PC += cur_op->m_size;
	}
}

std::string CPU::RegToStr(int r)
{
	static const char* reg[] = { "A", "B", "C", "BC", "D", "E", "DE", "H", "L", "HL", "SP" };
	return reg[r];
}

void CPU::load_register_value(Registers reg, uint8_t* value)
{
	switch (reg)
	{
	case Registers::A:
	{
		this->A = value[0];
	} break;
	case Registers::B:
	{
		this->B = value[0];
	} break;
	case Registers::C:
	{
		this->C = value[0];
	} break;
	case Registers::BC:
	{
		this->C = value[0];
		this->B = value[1];
	}
	case Registers::D:
	{
		this->D = value[0];
	} break;
	case Registers::E:
	{
		this->E = value[0];
	} break;
	case Registers::DE:
	{
		this->E = value[0];
		this->D = value[1];
	} break;
	case Registers::H:
	{
		this->H = value[0];
	} break;
	case Registers::L:
	{
		this->L = value[0];
	} break;
	case Registers::HL:
	{
		this->L = value[0];
		this->H = value[1];
	} break;
	default:
	{
		printf("INVALID REGISTER FOR fn load_register_value");
	} break;
	}
}