#include "MEM.h"

Memory::Memory()
{
	//m_Memory = new uint8_t[TotalAdrSpaceUprBnd];
	//m_Memory = std::array(TotalAdrSpaceUprBnd);
}

Memory::~Memory()
{
	//delete m_Memory;
}

void Memory::CheckMemUpper(uint16_t max, uint16_t value, ErrorCode& e)
{
	if (value > max)
		e.flag |= (int)ErrorFlags::MEM_OUT_OF_BOUND;
}

void Memory::CheckMemLower(uint16_t min, uint16_t value, ErrorCode& e)
{
	if (value < min)
		e.flag |= (int)ErrorFlags::MEM_OUT_OF_BOUND;
}

bool Memory::AdressableRangeCheck(int p, ErrorCode& e)
{
	if (p > TotalAdrSpaceUprBnd)
	{
		e.flag |= (int)ErrorFlags::MEM_OUT_OF_BOUND;
		SetDebugECMsg(e, 
			"Invalid argument to IndirectReadBytes. Offset outside of total addressable range");
		return false;
	}
	return true;
}

void Memory::Reset()
{
	for (uint16_t i = 0; i < TotalAdrSpaceUprBnd; i++) { i = 0x00; }
	//memset() is the equivelant alternative for this
}

/*
void stackPush(uint16_t value)
{
	e8080.SP -= 2;
	writeByte((value >> 8) & 0xFF, e8080.SP + 1);
	writeByte((value >> 0) & 0xFF, e8080.SP + 0);
}

uint16_t stackPop()
{
	uint8_t lo = readByte(e8080.SP + 0);
	uint8_t hi = readByte(e8080.SP + 1);
	e8080.SP += 2;
	return hi << 8 | lo;
}
*/