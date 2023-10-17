#pragma once

#include <array>
#include <cstdint>
#include <limits>
#include <string>
#include <unordered_map>
#include <memory>

#include "Timer.h"
#include "Error.h"
//#include "Opcodes.h"

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
	//make private and accesible with the public methods from opcodes. Leave public for now for testing?
public:
	// Main Registers
	//Registers B, C, D, E, H, L are 8 bits, but can be combined into 3 16 bit registers
	uint8_t A = 0; //Accumulator

	//The A register is used to store the result of operations, whilst the other 6 registers are often used in 16-bit pairs B-C D-E H-L

	//union acts as an XOR for the variables in it's scope.
	//the struct acts as an AND for the variables internally
	union
	{
		struct
		{
			uint8_t B : 8;// : 8 specifies that it occupies 8 bits
			uint8_t C : 8;
		};
		//we can't assign inside the struct, because of the unions property, only one can be assigned at a time (XOR)
		uint16_t BC = 0; //no need for : 16, because we are assigning it 0 (=0) where the compiler will determine that it is 16 bits.
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
			//the : x specifies the number of occupying bits the value will use
			// if x is 3, it will use 3 bits, x=1, 1 bit.
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

	FLAGS flags;



public:
	CPU();
	void load_register_value(Registers reg, uint8_t* value);
	void execute_opcode(opcode* cur_op, Memory* mem);
	void startExecAt(Memory* mem);
	void toggleStart();

public:
	static std::string RegToStr(int r);

public:
	//turn on if game/instructions needs to be frame time limited.
	static constexpr bool LimitCycles = true;

private:
	void InitOpcodeTable();

private:
	ErrorCode cpu_LastError;
	Timer timer;
	static bool start;
	//this is done to avoid using a constructor/destructor, and thus we just call the functions themselves. 
	//if opcode* is used, we would need to use a constructor and destructor to avoid memory leaks
	//std::shared_ptr<opcode> opcodeLookup[256]; //this equiveleant to opcode* opcodeLookup[255]
};
//this is just the table for all the opcodes