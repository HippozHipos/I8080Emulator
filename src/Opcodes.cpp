#include "opcodes.h"
#include "Util.h"

ErrorCode opcode::LastError;

/*
static void setFlags(CPU* cpu, uint16_t reg, uint16_t reg2 = 0)
{
    cpu->flags.Z = (reg == 0) & 1;  // Set Z flag to 1 if A is zero, else 0
    cpu->flags.S = (reg > 0) & 1;   // Set S flag to 1 if A is positive, else 0
    cpu->flags.C = (((uint16_t)reg + reg2) > 0xff) & 1; //set C flag to 1 if the result of the operation is going to be too big to fit in the register
    cpu->flags.P = ((std::popcount(reg) % 2) == 0) & 1; //set the Parity flag to 1 if number of bits is even, 0 if odd
    //cpu->flags.AC = ac_flagADD(cpu->A, cpu->B);
}
*/
/*

Add a function called dissasemble_opcode_data(), it will make your life easier, as we just need to print that out next.


*/


static void setZ(CPU* cpu, uint16_t a) 
{
    cpu->flags.Z = (a == 0) & 1;  // Set Z flag to 1 if reg is zero, else 0
}
static void setS(CPU* cpu, uint16_t a)
{   //0x80 is 128 which is the halfway point for a 8bit number, which means it acts as 0.
    cpu->flags.S = (a > 0x80) & 1;   // Set S flag to 1 if A is positive, else 0
}
static void setC(CPU* cpu, uint16_t reg, uint16_t reg2 = 0)
{
    cpu->flags.C = (((uint16_t)reg + reg2) > 0xff) & 1; //set C flag to 1 if the result of the operation is going to be too big to fit in the register
}
static void setP(CPU* cpu, uint16_t reg)
{
    cpu->flags.P = ((__popcnt(reg) % 2) == 0) & 1; //set the Parity flag to 1 if number of bits is even, 0 if odd
}

static uint8_t ac_flagADD(uint8_t a, uint8_t b)
{
    uint8_t lowNibbleA = a & 0x0F;
    uint8_t lowNibbleB = b & 0x0F;
    uint8_t carry = (lowNibbleA + lowNibbleB) > 0x0F;
    return carry;
}

NOP::NOP()
{
	m_name = "NOP"; m_size = 1; m_cycle_duration = 4; 
}

std::string NOP::disassemble_to_string(CPU* cpu, Memory* mem)
{
	return "NOP";
}

void NOP::execute(CPU* cpu, Memory* mem)
{
	return;
}

LXI::LXI(Registers reg)
{
	m_name = "LXI"; m_size = 3; m_cycle_duration = 10; m_reg = (int)reg;
}

std::string LXI::disassemble_to_string(CPU* cpu, Memory* mem)
{
	return "LXI " + CPU::RegToStr(m_reg) + ", " + toHexStr(mem->JoinBytes(cpu->PC));
}

void LXI::execute(CPU* cpu, Memory* mem)
{
    LastError.Clear();
    //check we only work on 16 bit registers
    if (m_reg != (int)Registers::BC && m_reg != (int)Registers::DE && m_reg != (int)Registers::SP && m_reg != (int)Registers::HL)
    {
        LastError.flag |= (int)ErrorFlags::INVALID_ARGUMENT;
        return;
    }
    cpu->load_register_value(static_cast<Registers>(m_reg), mem->GetRawPtr(cpu->PC, LastError));
}

LDA::LDA(Registers reg)
{ 
	m_name = "LDA"; m_size = 3; m_cycle_duration = 13; m_reg = (int)reg; 
}

std::string LDA::disassemble_to_string(CPU* cpu, Memory* mem)
{
    return "LDA " + CPU::RegToStr(m_reg) + ", " + toHexStr(mem->JoinBytes(cpu->PC));
}

void LDA::execute(CPU* cpu, Memory* mem)
{
    LastError.Clear();
    cpu->A = mem->IndirectReadBytes(cpu->PC, LastError);
}

STAX::STAX(Registers reg)
{
    m_name = "STAX";
    m_size = 0;
    m_cycle_duration = 7;
    m_reg = (int)reg;
}

std::string STAX::disassemble_to_string(CPU* cpu, Memory* mem)
{
    return "STAX " + CPU::RegToStr(m_reg);
}

void STAX::execute(CPU* cpu, Memory* mem)
{
    LastError.Clear();
    switch (m_reg)
    {
    case (int)Registers::BC:
        //mem->DirectWriteBytes(cpu->BC, cpu->A, LastError);
        cpu->BC = cpu->A;
        break;
    case (int)Registers::DE:
        //mem->DirectWriteBytes(cpu->DE, cpu->A, LastError);
        cpu->DE = cpu->A;
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

std::string SHLD::disassemble_to_string(CPU* cpu, Memory* mem)
{
    return "SHLD " + CPU::RegToStr(m_reg) + ", " + toHexStr(mem->JoinBytes(cpu->PC));
}

void SHLD::execute(CPU* cpu, Memory* mem)
{
    LastError.Clear();
    uint16_t adr = mem->JoinBytes(cpu->PC);
    mem->DirectWriteBytes(adr, cpu->L, LastError);
    mem->DirectWriteBytes(adr + 1, cpu->L, LastError);
}

STA::STA()
{
    m_name = "STA";
    m_size = 2;
    m_cycle_duration = 13;
}

std::string STA::disassemble_to_string(CPU* cpu, Memory* mem)
{
    return "STA " + toHexStr(mem->JoinBytes(cpu->PC));
}

void STA::execute(CPU* cpu, Memory* mem)
{
    LastError.Clear();
    uint16_t adr = mem->JoinBytes(cpu->PC);
    mem->DirectWriteBytes(adr, cpu->A, LastError);
}

LDAX::LDAX(Registers reg)
{
    m_name = "LDAX";
    m_size = 0;
    m_cycle_duration = 2;
    m_reg = (int)reg;
}

std::string LDAX::disassemble_to_string(CPU* cpu, Memory* mem)
{
    return "LDAX " + CPU::RegToStr(m_reg); //needs to be completed
}

void LDAX::execute(CPU* cpu, Memory* mem)
{
    switch (m_reg)
    {
    case (int)Registers::BC:
        cpu->A = (uint8_t)cpu->BC;
        break;
    case (int)Registers::DE:
        cpu->A = (uint8_t)cpu->DE;
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

std::string INX::disassemble_to_string(CPU* cpu, Memory* mem)
{
    return "INX " + CPU::RegToStr(m_reg); //needs to be completed
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

DCX::DCX(Registers reg)
{
    m_name = "DCX";
    m_size = 0;
    m_cycle_duration = 5;
    m_reg = (int)reg;
}

std::string DCX::disassemble_to_string(CPU* cpu, Memory* mem)
{
    return "INX " + CPU::RegToStr(m_reg); //needs to be completed
}

void DCX::execute(CPU* cpu, Memory* mem)
{
    switch (m_reg)
    {
    case (int)Registers::BC:
        cpu->BC--;
        break;
    case (int)Registers::DE:
        cpu->DE--;
        break;
    case (int)Registers::HL:
        cpu->HL--;
        break;
    case (int)Registers::SP:
        cpu->SP--;
        break;
    default:
        printf("DCX wrong register >:(\n");
        break;
    }
}

//0x80 - 0x87
ADD::ADD(Registers reg)
{
    m_name = "ADD";

    if (reg == Registers::HL) //on page 50 of the i8080 manual, ADD M holds a special case for the reg_pair HL.
        m_cycle_duration = 7;

    m_size = 1;
    m_cycle_duration = 4;
    m_reg = (int)reg;
}

std::string ADD::disassemble_to_string(CPU* cpu, Memory* mem)
{
    return std::string(); //needs to be completed
}

void ADD::execute(CPU* cpu, Memory* mem)
{
    switch (m_reg)
    {
    case (int)Registers::B:
        //if the result of the operation is going to be too big to fit in the register

        cpu->A += cpu->B;
        cpu->flags.Z = (cpu->A == 0) & 1;  // Set Z flag to 1 if A is zero, else 0
        cpu->flags.S = (cpu->A > 0) & 1;   // Set S flag to 1 if A is positive, else 0
        cpu->flags.C = (  ((uint16_t)cpu->A + cpu->B) > 0xff) & 1; //set C flag to 1 if the result of the operation is going to be too big to fit in the register
        cpu->flags.P = (  (__popcnt(cpu->A) % 2) == 0) & 1; //set the Parity flag to 1 if number of bits is even, 0 if odd
        //cpu->flags.AC = ac_flagADD(cpu->A, cpu->B);
        break;
    case (int)Registers::C:
        cpu->A += cpu->C;
        break;
    case (int)Registers::D:
        cpu->A += cpu->D;
        break;
    case (int)Registers::E:
        cpu->A += cpu->E;
        break;
    case (int)Registers::H:
        cpu->A += cpu->H;
        break;
    case (int)Registers::L:
        cpu->A += cpu->L;
        break;
    case (int)Registers::HL: // M just means the register pair HL
        cpu->A += cpu->HL;
        break;
    case (int)Registers::A:
        cpu->A += cpu->A;
    default:
        printf("ADD wrong registers >:(\n");
        break;
    }
}

INR::INR(Registers reg)
{
    m_name = "INR";
    m_size = 1;
    m_cycle_duration = 5;
    m_reg = (int)reg;
}

std::string INR::disassemble_to_string(CPU* cpu, Memory* mem)
{
    return std::string(); //needs to be completed
}

void INR::execute(CPU* cpu, Memory* mem)
{
    switch (m_reg)
    {
    case (int)Registers::B:
        cpu->B++;
        break;
    case (int)Registers::C:
        cpu->C++;
        break;
    case (int)Registers::D:
        cpu->D++;
        break;
    case (int)Registers::E:
        cpu->E++;
        break;
    case (int)Registers::H:
        cpu->H++;
        break;
    case (int)Registers::L:
        cpu->L++;
        break;
    case (int)Registers::HL:
        cpu->HL++;
        break;
    case (int)Registers::A:
        cpu->A++;
        break;
    default:
        printf("INR wrong register >:(\n"); // how you would manage to do this for INR is beyond me
        break;
    }
}

DCR::DCR(Registers reg)
{
}

std::string DCR::disassemble_to_string(CPU* cpu, Memory* mem)
{
    return std::string();
}

void DCR::execute(CPU* cpu, Memory* mem)
{
}
