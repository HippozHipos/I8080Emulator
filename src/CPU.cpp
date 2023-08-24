#include "CPU.h"

#include <chrono>
#include <thread>
#include "Opcodes.h"
#include "MEM.h"

//turn on if game/instructions needs to be frame time limited.
constexpr bool bLimitOpcodeSpeed = false;

void CPU::execute_opcode(std::shared_ptr<opcode> cur_op, Memory* mem)
{
	if (bLimitOpcodeSpeed)
	{
		auto start_time = std::chrono::high_resolution_clock::now();

		cur_op->execute(this, mem);

		auto end_time = std::chrono::high_resolution_clock::now();

		long long time_difference = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

		//1 cycle takes 500 nano seconds (1 / 2,000,000) * 1,000,000,000

		long long sleep_time = (500 * cur_op->m_cycle_duration) - time_difference;

		this->PC += cur_op->m_size; //NOt 100 percent sure this is right but

		std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_time));
	}
	else
	{
		cur_op->execute(this, mem);
		this->PC += cur_op->m_size;
	}
}

std::string CPU::RegToStr(int r)
{
	const char* reg[] = { "A", "B", "C", "BC", "D", "E", "DE", "H", "L", "HL", "SP" };
	return reg[r];
}


void CPU::load_register_value(registers reg, uint8_t* value)
{
	switch (reg)
	{
	case registers::A:
	{
		this->A = value[0];
	}
	case registers::B:
	{
		this->B = value[0];
	}
	case registers::C:
	{
		this->C = value[0];
	}
	case registers::BC:
	{
		this->C = value[0];
		this->B = value[1];
	}
	case registers::D:
	{
		this->D = value[0];
	}
	case registers::E:
	{
		this->E = value[0];
	}
	case registers::DE:
	{
		this->E = value[0];
		this->D = value[1];
	}
	case registers::H:
	{
		this->H = value[0];
	}
	case registers::L:
	{
		this->L = value[0];
	}
	case registers::HL:
	{
		this->L = value[0];
		this->H = value[1];
	}
	default:
	{
		printf("INVALID REGISTER FOR fn load_register_value");
		break;
	}
	}
}