#include "MEM.h"

Memory::Memory()
{
	m_Memory = new std::array<uint8_t, TotalAdrSpaceUprBnd>();
}

Memory::~Memory()
{
	delete m_Memory;
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
		SetDebugMsg(e, 
			"Invalid argument to IndirectReadBytes. Offset outside of total addressable range");
		return false;
	}
	return true;
}

void Memory::Reset()
{
	for (auto& i : *m_Memory) { i = 0x00; }
	//memset() is the equivelant alternative for this
}
