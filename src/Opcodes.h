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
	uint8_t m_size;
	uint8_t m_cycle_duration;

	virtual std::string to_string(CPU* cpu, Memory* mem) = 0;
	virtual void execute(CPU* cpu, Memory* mem) = 0;

	virtual ~opcode() = default;
	static ErrorCode LastError;
};

struct NOP : public opcode
{
	NOP();
	std::string to_string(CPU* cpu, Memory* mem) override;
	void execute(CPU* cpu, Memory* mem) override;
};

struct LXI : public opcode
{
	LXI(Registers reg);
	std::string to_string(CPU* cpu, Memory* mem) override;
	void execute(CPU* cpu, Memory* mem) override;

	int m_reg;
};

struct LDA : public opcode
{
	LDA(Registers reg);
	std::string to_string(CPU* cpu, Memory* mem) override;
	void execute(CPU* cpu, Memory* mem) override;

	int m_reg;
};

struct STAX : public opcode
{
	STAX(Registers reg);
	std::string to_string(CPU* cpu, Memory* mem) override;
	void execute(CPU* cpu, Memory* mem) override;

	int m_reg;
};

struct SHLD : public opcode
{
	SHLD(Registers reg);
	std::string to_string(CPU* cpu, Memory* mem) override;
	void execute(CPU* cpu, Memory* mem) override;

	int m_reg;
};

struct STA : public opcode
{
	STA();
	std::string to_string(CPU* cpu, Memory* mem) override;
	void execute(CPU* cpu, Memory* mem) override;
};

struct LDAX : public opcode
{
	LDAX(Registers reg);
	std::string to_string(CPU* cpu, Memory* mem) override;
	void execute(CPU* cpu, Memory* mem) override;

	int m_reg;
};

struct INX : public opcode
{
	INX(Registers reg);
	std::string to_string(CPU* cpu, Memory* mem) override;
	void execute(CPU* cpu, Memory* mem) override;

	int m_reg;
};

#endif // OPCODE_H