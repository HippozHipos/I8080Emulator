#include "opcodes.h"
#include "Util.h"

ErrorCode opcode::LastError;

NOP::NOP()
{
	m_name = "NOP"; m_size = 1; m_cycle_duration = 4; 
}

std::string NOP::to_string(CPU* cpu, Memory* mem)
{
	return "NOP";
}

void NOP::execute(CPU* cpu, Memory* mem)
{
	return;
}

LXI::LXI(Registers reg)
{
	m_name = "LXI"; m_size = 2; m_cycle_duration = 10; m_reg = (int)reg;
}

std::string LXI::to_string(CPU* cpu, Memory* mem)
{
	return "LXI " + CPU::RegToStr(m_reg) + ", " + toHexStr(mem->JoinBytes(cpu->PC));
}

void LXI::execute(CPU* cpu, Memory* mem)
{
    LastError.Clear();
	cpu->load_register_value(static_cast<Registers>(m_reg), mem->GetRawPtr(cpu->PC, LastError));
}

LDA::LDA(Registers reg)
{ 
	m_name = "LDA"; m_size = NULL; m_cycle_duration = 4; m_reg = (int)reg; 
}

std::string LDA::to_string(CPU* cpu, Memory* mem)
{
    return "LDA " + CPU::RegToStr(m_reg) + ", " + toHexStr(mem->JoinBytes(cpu->PC));
}

void LDA::execute(CPU* cpu, Memory* mem)
{
    LastError.Clear();
    cpu->A = mem->IndirectReadByte(cpu->PC, LastError);
}


STAX::STAX(Registers reg)
{
    m_name = "STAX";
    m_size = 0;
    m_cycle_duration = 7;
    m_reg = (int)reg;
}

std::string STAX::to_string(CPU* cpu, Memory* mem)
{
    return "STAX " + CPU::RegToStr(m_reg);
}

void STAX::execute(CPU* cpu, Memory* mem)
{
    LastError.Clear();
    switch (m_reg)
    {
    case (int)Registers::BC:
        mem->DirectWriteByte(cpu->BC, cpu->A, LastError);
        break;
    case (int)Registers::DE:
        mem->DirectWriteByte(cpu->DE, cpu->A, LastError);
        break;
    default:
        printf("STAX Register was incorrect >:(\n");
        break;
    }
}

SHLD::SHLD(Registers reg)
{
    m_name = "SHLD";
    m_size = 2;
    m_cycle_duration = 16;
    m_reg = (int)reg;
}

std::string SHLD::to_string(CPU* cpu, Memory* mem)
{
    return "SHLD " + CPU::RegToStr(m_reg) + ", " + toHexStr(mem->JoinBytes(cpu->PC));
}

void SHLD::execute(CPU* cpu, Memory* mem)
{
    LastError.Clear();
    uint16_t adr = mem->JoinBytes(cpu->PC);
    mem->DirectWriteByte(adr, cpu->L, LastError);
    mem->DirectWriteByte(adr + 1, cpu->L, LastError);
}

STA::STA()
{
    m_name = "STA";
    m_size = 2;
    m_cycle_duration = 13;
}

std::string STA::to_string(CPU* cpu, Memory* mem)
{
    return "STA " + toHexStr(mem->JoinBytes(cpu->PC));
}

void STA::execute(CPU* cpu, Memory* mem)
{
    LastError.Clear();
    uint16_t adr = mem->JoinBytes(cpu->PC);
    mem->DirectWriteByte(adr, cpu->A, LastError);
}

LDAX::LDAX(Registers reg)
{
    m_name = "LDAX";
    m_size = 0;
    m_cycle_duration = 2;
    m_reg = (int)reg;
}

std::string LDAX::to_string(CPU* cpu, Memory* mem)
{
    return "LDAX " + CPU::RegToStr(m_reg);
}

void LDAX::execute(CPU* cpu, Memory* mem)
{
    switch (m_reg)
    {
    case (int)Registers::BC:
        cpu->A = cpu->BC;
        break;
    case (int)Registers::DE:
        cpu->A = cpu->DE;
        break;
    }
}

INX::INX(Registers reg)
{
    m_name = "INX";
    m_size = 0;
    m_cycle_duration = 5;
    m_reg = (int)reg;
}

std::string INX::to_string(CPU* cpu, Memory* mem)
{
    return "INX " + CPU::RegToStr(m_reg);
}

void INX::execute(CPU* cpu, Memory* mem)
{
    switch (m_reg)
    {
    case (int)Registers::BC:
        cpu->BC++;
        break;
    case (int)Registers::DE:
        cpu->DE++;
        break;
    case (int)Registers::HL:
        cpu->HL++;
        break;
    case (int)Registers::SP:
        cpu->SP++;
        break;
    default:
        printf("INX wrong register >:(\n");
        break;
    }
}