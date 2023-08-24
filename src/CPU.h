#pragma once

#include <array>
#include <cstdint>
#include <limits>
#include <string>
#include <unordered_map>
#include <memory>

#include "Timer.h"

//forward declaration
struct opcode;
class Memory;

enum class Registers : int
{
	A = 0,
	B = 1,
	C = 2,
	BC = 3,
	D = 4,
	E = 5,
	DE = 6,
	H = 7,
	L = 8,
	HL = 9,
	SP = 10,
};

class CPU
{
public:
	// Main Registers
	//Registers B, C, D, E, H, L are 8 bits, but can be combined into 3 16 bit registers
	uint8_t A; //Accumulator

	union
	{
		struct
		{
			uint8_t B : 8;
			uint8_t C : 8;
		};
		uint16_t BC = 0;
	};

	union
	{
		struct
		{
			uint8_t D : 8;
			uint8_t E : 8;
		};
		uint16_t DE = 0;
	};

	union
	{
		struct
		{
			uint8_t H : 8;
			uint8_t L : 8;
		};
		uint16_t HL = 0;
	};

	union FLAGS
	{
		struct
		{
			uint8_t S : 1; // Sign Flag set if the result is negative
			uint8_t Z : 1; // Zero Flag set if the result is zero.
			uint8_t AC : 1; // Half-Carry Flag used for binary-coded decimal arithmetic
			uint8_t P : 1; // Parity Flag set if the number of 1 bits in the result is even
			uint8_t C : 1; // Carry Flag set if the last addition operation resulted in a carry or if the last subtraction operation required a borrow
			uint8_t RESERVED : 3; // Unused
		};

		uint8_t value = 0;
	};

	// Program Counter
	uint16_t PC = 0; // Program Counter

	// Index Register
	uint16_t SP = 0; // Stack Pointer

	static constexpr int ProcesserFrequency = 2000000; // 2_000_000 or 2 Mhz

public:
	void load_register_value(Registers reg, uint8_t* value);
	void execute_opcode(std::shared_ptr<opcode> cur_op, Memory* mem);

public:
	static std::string RegToStr(int r);

public:
	//turn on if game/instructions needs to be frame time limited.
	static constexpr bool LimitCycles = true;

private:
	Timer timer;
};
