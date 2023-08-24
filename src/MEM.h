#pragma once

#include <array>
#include <string>
#include "Error.h"
#include <iostream>
#include <bitset>

#include "util.h"

#ifndef I8080_32BIT_MEM
	//#define I8080_32BIT_MEM
#endif

template<uint8_t P> constexpr bool IsByteAligned()
{
	return 8 % P == 0;
}

class Memory
{
public:
	Memory();
	~Memory();

public:
	enum Type : uint8_t
	{
		RAM, 
		ROM,
		STACK,
		VRAM,
		ALL
	};

public:
	static constexpr Type DefaultType = Type::ALL;
#ifdef I8080_32BIT_MEM
	using Ptr = uint32_t;
	static constexpr uint8_t DefaultBytes = 4;
#else
	using Ptr = uint16_t;
	static constexpr uint8_t DefaultBytes = 2;
#endif

public:
#ifdef I8080_32BIT_MEM
	static constexpr int TotalAdrSpaceLwrBnd = 0x000000;
	static constexpr int TotalAdrSpaceUprBnd = 0xFFFFFF;

	static constexpr int ROMLwrBnd = 0x000000;
	static constexpr int ROMUprBnd = 0x199998, RAMLwrBnd = 0x200000;

	static constexpr int StackLwrBnd = 0x230000;

	static constexpr int RAMUprBnd = 0x239998, VRAMLwrBnd = 0x240000, StackUprBnd = 0x239998;
	static constexpr int VRAMUprBnd = 0x400000, UnusedLwrBnd = 0x400000;

	static constexpr int UnusedUprBnd = TotalAdrSpaceUprBnd;
#else
	static constexpr int TotalAdrSpaceLwrBnd = 0x0000;
	static constexpr int TotalAdrSpaceUprBnd = 0xFFFF;

	static constexpr int ROMLwrBnd = 0x0000;
	static constexpr int ROMUprBnd = 0x1998, RAMLwrBnd = 0x2000;

	static constexpr int StackLwrBnd = 0x2300;

	static constexpr int RAMUprBnd = 0x2398, VRAMLwrBnd = 0x2400, StackUprBnd = 0x2398;
	static constexpr int VRAMUprBnd = 0x4000, UnusedLwrBnd = 0x4000;

	static constexpr int UnusedUprBnd = TotalAdrSpaceUprBnd;
#endif

public:
	template<Type PtrLoc = DefaultType>
	uint8_t DirectReadByte(Ptr offset, ErrorCode& e)
	{
		if (!AdressableRangeCheck(GetLwrBnd<PtrLoc>() + offset, e))
		{
			e.flag |= (int)ErrorFlags::INVALID_READ | (int)ErrorFlags::INVALID_ARGUMENT;
			SetMemSectionFlag<PtrLoc>(e);
			return 0;
		}
		CheckMemUpper(GetUprBnd<PtrLoc>(), GetLwrBnd<PtrLoc>() + offset, e);
		if (e.flag & (int)ErrorFlags::MEM_OUT_OF_BOUND)
		{
			e.flag |= (int)ErrorFlags::INVALID_READ;
			SetMemSectionFlag<PtrLoc>(e);
			SetDebugMsg(e, "Memory read through DirectReadByte out of bound.\n"
				"Read attepmpt at offset " + toHexStr(offset) +
				".\nNote tht valid memory locations for " + GetName<PtrLoc>() + " are 0 to: " +
				toHexStr(GetUprBnd<PtrLoc>() - GetLwrBnd<PtrLoc>()));
			return 0;
		}
		SetDebugMsg(e, "Operation succeeded");
		return m_Memory->at(GetLwrBnd<PtrLoc>() + offset);
	}

	template<Type DataLoc = DefaultType>
	uint8_t* GetRawPtr(Ptr offset, ErrorCode& e)
	{
		if (!AdressableRangeCheck(GetLwrBnd<DataLoc>() + offset, e))
		{
			e.flag |= (int)ErrorFlags::INVALID_READ | (int)ErrorFlags::INVALID_ARGUMENT;
			SetMemSectionFlag<DataLoc>(e);
			return 0;
		}
		CheckMemUpper(GetUprBnd<DataLoc>(), GetLwrBnd<DataLoc>() + offset, e);
		if (e.flag & (int)ErrorFlags::MEM_OUT_OF_BOUND)
		{
			e.flag |= (int)ErrorFlags::INVALID_READ;
			SetMemSectionFlag<DataLoc>(e);
			SetDebugMsg(e, "Memory read through DirectReadByte out of bound.\n"
				"Read attepmpt at offset " + toHexStr(offset) +
				".\nNote tht valid memory locations for " + GetName<DataLoc>() + " are 0 to: " +
				toHexStr(GetUprBnd<DataLoc>() - GetLwrBnd<DataLoc>()));
			return 0;
		}
		SetDebugMsg(e, "Operation succeeded");
		return m_Memory->data() + GetLwrBnd<DataLoc>() + offset;
	}

	template<Type PtrLoc = DefaultType, Type DataLoc = DefaultType, uint8_t Bytes = DefaultBytes>
	uint8_t IndirectReadByte(Ptr offset, ErrorCode& e)
	{
		if (!AdressableRangeCheck(GetLwrBnd<PtrLoc>() + offset + (Bytes - 1), e))
		{
			e.flag |= (int)ErrorFlags::INVALID_READ | (int)ErrorFlags::INVALID_ARGUMENT;
			SetMemSectionFlag<PtrLoc>(e);
			return 0;
		}

		//2 is the upper limit for now since we are doing 16 bits rn, if we later expand to 32 bits, can set to 4
#ifdef I8080_32BIT_MEM
		if constexpr (!IsByteAligned<Bytes>() || Bytes > 4 || Bytes < 1)
#else
		if constexpr (!IsByteAligned<Bytes>() || Bytes > 2 || Bytes < 1)
#endif
		{
			e.flag |= (int)ErrorFlags::INVALID_READ | (int)ErrorFlags::INVALID_ARGUMENT;
			SetMemSectionFlag<PtrLoc>(e);
			SetDebugMsg(e, "Invalid argument to IndirectReadBytes. Bytes cannot be " + 
				std::to_string(Bytes));
			return 0;
		}

		CheckMemUpper(GetUprBnd<PtrLoc>(), GetLwrBnd<PtrLoc>() + offset + (Bytes - 1), e);
		if (e.flag & (int)ErrorFlags::MEM_OUT_OF_BOUND)
		{
			e.flag |= (int)ErrorFlags::INVALID_READ;
			SetMemSectionFlag<PtrLoc>(e);
			SetDebugMsg(e, "Memory read through IndirectReadBytes out of bound.\n"
				"Reading " + std::to_string(Bytes) + " bytes from offset " + toHexStr(offset) +
				".\nNote tht valid memory locations for " + GetName<PtrLoc>() + " are 0 to: " +
				toHexStr(GetUprBnd<DataLoc>() - GetLwrBnd<DataLoc>()));
			return 0;
		}
		Ptr index = JoinBytes<PtrLoc, Bytes>(offset);
		CheckMemUpper(GetUprBnd<PtrLoc>(), index, e);
		if (e.flag & (int)ErrorFlags::MEM_OUT_OF_BOUND)
		{
			e.flag |= (int)ErrorFlags::INVALID_READ;
			SetMemSectionFlag<DataLoc>(e);
			SetDebugMsg(e, "Memory read through IndirectReadBytes out of bound.\n" 
				"Reading " + std::to_string(Bytes) + " bytes of combined address from offset " + toHexStr(offset) +
				" succeeded\n. The memory location is " + toHexStr(index) +
				". However, this memory location is invalid for " + GetName<DataLoc>() +
				".\nValid locations are: 0" +  + " to " + 
				toHexStr(GetUprBnd<DataLoc>() - GetLwrBnd<DataLoc>()));
			return 0;
		}

		SetDebugMsg(e, "Operation succeeded");
		return m_Memory->at(GetLwrBnd<DataLoc>() + index);
	}

	template<Type DataLoc = DefaultType>
	void DirectWriteByte(Ptr offset, uint8_t data, ErrorCode& e)
	{
		if (!AdressableRangeCheck(GetLwrBnd<DataLoc>() + offset, e))
		{
			e.flag |= (int)ErrorFlags::INVALID_READ | (int)ErrorFlags::INVALID_ARGUMENT;
			SetMemSectionFlag<DataLoc>(e);
			return;
		}
		CheckMemUpper(GetUprBnd<DataLoc>(), GetLwrBnd<DataLoc>() + offset, e);
		if (e.flag & (int)ErrorFlags::MEM_OUT_OF_BOUND)
		{
			e.flag |= (int)ErrorFlags::INVALID_WRITE;
			SetMemSectionFlag<DataLoc>(e);
			SetDebugMsg(e, "Memory write through DirectWriteByte out of bound.\n" 
				"Write attepmpt at offset " + toHexStr(offset) +
				".\nNote tht valid memory locations for " + GetName<DataLoc>() + " are 0 to: " +
				toHexStr(GetUprBnd<DataLoc>() - GetLwrBnd<DataLoc>()));
			return;
		}
		SetDebugMsg(e, "Operation succeeded");
		m_Memory->data()[GetLwrBnd<DataLoc>() + offset] = data;
	}

	template<Type DataLoc = DefaultType, uint8_t Bytes = DefaultBytes>
	Ptr JoinBytes(Ptr offset)
	{
		Ptr index = 0;
		uint8_t i = Bytes;
		Ptr totalOffset = GetLwrBnd<DataLoc>() + offset;
		do
		{
			if constexpr (Bytes == 1)
			{
				index = m_Memory->at(totalOffset);
				break;
			}
			index = (index << (8 * (Bytes / 2))) | m_Memory->at(totalOffset + i - 1);
			i--;
		} while (i > 0);
		return index;
	}

	void	Reset();

private:
	void CheckMemUpper(uint16_t min, uint16_t value, ErrorCode& e);
	void CheckMemLower(uint16_t max, uint16_t value, ErrorCode& e);
	
private:
	bool AdressableRangeCheck(int p, ErrorCode& e);

private:
	template<Type T> constexpr uint16_t GetLwrBnd()
	{
		if constexpr (T == Type::RAM)		return RAMLwrBnd;
		if constexpr (T == Type::ROM)		return ROMLwrBnd;
		if constexpr (T == Type::STACK)		return StackLwrBnd;
		if constexpr (T == Type::VRAM)		return VRAMLwrBnd;
		if constexpr (T == Type::ALL)		return TotalAdrSpaceLwrBnd;
	}

	template<Type T> constexpr uint16_t GetUprBnd()
	{
		if constexpr (T == Type::RAM)		return RAMUprBnd;
		if constexpr (T == Type::ROM)		return ROMUprBnd;
		if constexpr (T == Type::STACK)		return StackUprBnd;
		if constexpr (T == Type::VRAM)		return VRAMUprBnd;
		if constexpr (T == Type::ALL)		return TotalAdrSpaceUprBnd;
	}


	template<Type T> constexpr const char* GetName()
	{
		if constexpr (T == Type::RAM)		return "RAM";
		if constexpr (T == Type::ROM)		return "ROM";
		if constexpr (T == Type::STACK)		return "STACK";
		if constexpr (T == Type::VRAM)		return "VRAM";
		if constexpr (T == Type::ALL)		return "ALL";
	}

	template<Type T> void SetMemSectionFlag(ErrorCode& e)
	{
		if (T == Type::RAM)		e.flag |= (int)ErrorFlags::MEMORY_SECTION_RAM;
		if (T == Type::ROM)		e.flag |= (int)ErrorFlags::MEMORY_SECTION_ROM;
		if (T == Type::STACK)	e.flag |= (int)ErrorFlags::MEMORY_SECTION_STACK;
		if (T == Type::VRAM)	e.flag |= (int)ErrorFlags::MEMORY_SECTION_VRAM;
		if (T == Type::ALL)		e.flag |= (int)ErrorFlags::MEMORY_SECTION_ALL;
	}

private:
	std::array<uint8_t, TotalAdrSpaceUprBnd>* m_Memory;
};

