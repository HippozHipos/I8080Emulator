#include "opcodes.h"
#include "Util.h"
#include <format>

ErrorCode opcode::LastError;

NOP::NOP()
{
	m_name = "NOP"; m_size = 1; m_cycle_duration = 4; 
}

std::string NOP::dissamble_opcode_to_string(CPU* cpu, Memory* mem)
{
	return "NOP";
}

void NOP::execute(CPU* cpu, Memory* mem)
{
	return;
}

LXI::LXI(registers reg)
{
	m_name = "LXI"; m_size = 2; m_cycle_duration = 10; m_reg = reg;
}

std::string LXI::dissamble_opcode_to_string(CPU* cpu, Memory* mem)
{
	uint16_t num = mem->JoinBytes(cpu->PC);
	return "LXI " + CPU::RegToStr(m_reg) + ", " + toUpper(std::format("{:x}", num));
}

void LXI::execute(CPU* cpu, Memory* mem)
{
    LastError.Clear();
	cpu->load_register_value(static_cast<registers>(m_reg), mem->GetRawPtr(cpu->PC, LastError));
}

LDA::LDA(registers reg)
{ 
	m_name = "LDA"; m_size = NULL; m_cycle_duration = 4; m_reg = reg; 
}

std::string LDA::dissamble_opcode_to_string(CPU* cpu, Memory* mem)
{
    uint16_t num = mem->JoinBytes(cpu->PC);
	return "LDA " + CPU::RegToStr(m_reg) + ", " + toUpper(std::format("{:x}", num));
}

void LDA::execute(CPU* cpu, Memory* mem)
{
    LastError.Clear();
    cpu->A = mem->IndirectReadByte(cpu->PC, LastError);
}


STAX::STAX(registers reg)
{
    m_name = "STAX";
    m_size = 0;
    m_cycle_duration = 7;
    m_reg = reg;
}

std::string STAX::dissamble_opcode_to_string(CPU* cpu, Memory* mem)
{
    return "STAX " + CPU::RegToStr(m_reg);
}

void STAX::execute(CPU* cpu, Memory* mem)
{
    LastError.Clear();
    switch (m_reg)
    {
    case registers::BC:
        mem->DirectWriteByte(cpu->BC, cpu->A, LastError);
        break;
    case registers::DE:
        mem->DirectWriteByte(cpu->DE, cpu->A, LastError);
        break;
    default:
        printf("STAX Register was incorrect >:(\n");
        break;
    }
}

SHLD::SHLD(registers reg)
{
    m_name = "SHLD";
    m_size = 2;
    m_cycle_duration = 16;
    m_reg = reg;
}

std::string SHLD::dissamble_opcode_to_string(CPU* cpu, Memory* mem)
{
    uint16_t num = mem->JoinBytes(cpu->PC);
    return "SHLD " + CPU::RegToStr(m_reg) + ", " + toUpper(std::format("{:x}", num));
}

void SHLD::execute(CPU* cpu, Memory* mem)
{
    LastError.Clear();
    uint16_t memory_location = mem->JoinBytes(cpu->PC);
    mem->DirectWriteByte(memory_location, cpu->L, LastError);
    mem->DirectWriteByte(memory_location + 1, cpu->L, LastError);
}

STA::STA()
{
    m_name = "STA";
    m_size = 2;
    m_cycle_duration = 13;
}

std::string STA::dissamble_opcode_to_string(CPU* cpu, Memory* mem)
{
    uint16_t num = mem->JoinBytes(cpu->PC);
    return "STA " + toUpper(std::format("{:x}", num));
}

void STA::execute(CPU* cpu, Memory* mem)
{
    LastError.Clear();
    uint16_t memory_location = mem->JoinBytes(cpu->PC);
    mem->DirectWriteByte(memory_location, cpu->A, LastError);
}

LDAX::LDAX(registers reg)
{
    m_name = "LDAX";
    m_size = 0;
    m_cycle_duration = 2;
    m_reg = reg;
}

std::string LDAX::dissamble_opcode_to_string(CPU* cpu, Memory* mem)
{
    return "LDAX " + CPU::RegToStr(m_reg);
}

void LDAX::execute(CPU* cpu, Memory* mem)
{
    switch (m_reg)
    {
    case registers::BC:
        cpu->A = cpu->BC;
        break;
    case registers::DE:
        cpu->A = cpu->DE;
        break;
    }
}

INX::INX(registers reg)
{
    m_name = "INX";
    m_size = 0;
    m_cycle_duration = 5;
    m_reg = reg;
}

std::string INX::dissamble_opcode_to_string(CPU* cpu, Memory* mem)
{
    return "INX " + CPU::RegToStr(m_reg);
}

void INX::execute(CPU* cpu, Memory* mem)
{
    switch (m_reg)
    {
    case registers::BC:
        cpu->BC++;
        break;
    case registers::DE:
        cpu->DE++;
        break;
    case registers::HL:
        cpu->HL++;
        break;
    case registers::SP:
        cpu->SP++;
        break;
    default:
        printf("INX wrong register >:(\n");
        break;
    }
}