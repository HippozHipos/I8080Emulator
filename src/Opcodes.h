#ifndef OPCODE_H
#define OPCODE_H

#include <string>
#include "CPU.h"
#include "MEM.h"
#include "Error.h"

struct opcode
{
	using PCType = uint16_t;
	
	std::string m_name;
	uint8_t m_size = 0;
	uint8_t m_cycle_duration = 0;

	//prints string to console, by taking the hex and figuring out which instruction it is.
	virtual std::string disassemble_to_string(CPU* cpu, Memory* mem) = 0;
	//executes the real instruction into the emulators memory
	virtual void execute(CPU* cpu, Memory* mem) = 0; 

	virtual ~opcode() = default;
	static ErrorCode LastError;

	int m_reg = -1;
};

struct NOP : public opcode
{
	NOP();
	std::string disassemble_to_string(CPU* cpu, Memory* mem) override;
	void execute(CPU* cpu, Memory* mem) override;
};

struct LXI : public opcode
{
	LXI(Registers reg);
	std::string disassemble_to_string(CPU* cpu, Memory* mem) override;
	void execute(CPU* cpu, Memory* mem) override;

};

struct LDA : public opcode
{
	LDA(Registers reg);
	std::string disassemble_to_string(CPU* cpu, Memory* mem) override;
	void execute(CPU* cpu, Memory* mem) override;

	int m_reg;
};

struct STAX : public opcode
{
	STAX(Registers reg);
	std::string disassemble_to_string(CPU* cpu, Memory* mem) override;
	void execute(CPU* cpu, Memory* mem) override;

	int m_reg;
};

struct SHLD : public opcode
{
	SHLD(Registers reg);
	std::string disassemble_to_string(CPU* cpu, Memory* mem) override;
	void execute(CPU* cpu, Memory* mem) override;

	int m_reg;
};

struct STA : public opcode
{
	STA();
	std::string disassemble_to_string(CPU* cpu, Memory* mem) override;
	void execute(CPU* cpu, Memory* mem) override;
};

struct LDAX : public opcode
{
	LDAX(Registers reg);
	std::string disassemble_to_string(CPU* cpu, Memory* mem) override;
	void execute(CPU* cpu, Memory* mem) override;

	int m_reg;
};

struct INX : public opcode
{
	INX(Registers reg);
	std::string disassemble_to_string(CPU* cpu, Memory* mem) override;
	void execute(CPU* cpu, Memory* mem) override;

	int m_reg;
};

struct ADD : public opcode
{
	ADD(Registers reg);
	std::string disassemble_to_string(CPU* cpu, Memory* mem) override;
	void execute(CPU* cpu, Memory* mem) override;

	int m_reg;
};

struct INR : public opcode
{
	INR(Registers reg);
	std::string disassemble_to_string(CPU* cpu, Memory* mem) override;
	void execute(CPU* cpu, Memory* mem) override;

	int m_reg;
};

struct DCX : public opcode
{
	DCX(Registers reg);
	std::string disassemble_to_string(CPU* cpu, Memory* mem) override;
	void execute(CPU* cpu, Memory* mem) override;

	int m_reg;
};

struct DCR : public opcode
{
	DCR(Registers reg);
	std::string disassemble_to_string(CPU* cpu, Memory* mem) override;
	void execute(CPU* cpu, Memory* mem) override;

	int m_reg;
};


#endif // OPCODE_H