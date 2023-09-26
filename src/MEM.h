#pragma once

#include <array>
#include <string>
#include "Error.h"
#include <iostream>
#include <bitset>

#include "util.h"

template<uint8_t P> constexpr bool IsByteAligned()
{
	return 8 % P == 0;
}

template<uint8_t> struct ByteSize;
template<> struct ByteSize<1> { using Type = uint8_t; };
template<> struct ByteSize<2> { using Type = uint16_t; };
template<> struct ByteSize<4> { using Type = uint32_t; };

//probably will never be used. uncomment if needed
//template<> struct ByteSize<8> { using Type = uint64_t; }; 

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
	static constexpr Type DefaultType = Type::RAM; 
	using Ptr = uint16_t;
	static constexpr uint8_t DefaultBytes = 2;

public:
	static constexpr int TotalAdrSpaceLwrBnd = 0x0000;
	static constexpr int TotalAdrSpaceUprBnd = 0xFFFF;

	static constexpr int ROMLwrBnd = 0x0000;
	static constexpr int ROMUprBnd = 0x1998, RAMLwrBnd = 0x2000;

	static constexpr int StackLwrBnd = 0x2300;

	static constexpr int RAMUprBnd = 0x2398, VRAMLwrBnd = 0x2400, StackUprBnd = 0x2398;
	static constexpr int VRAMUprBnd = 0x4000, UnusedLwrBnd = 0x4000;

	static constexpr int UnusedUprBnd = TotalAdrSpaceUprBnd;

public:
	//DataLoc: Which memory section to read the data from
	//Bytes: How many bytes to read - can only be 1, 2, or 4, could add 64 if wanted not dont see need
	//we are template specialising in ByteSize<Bytes>::Type, currently this is being resolved to ByteSize<1> -> ByteSize<uint8_t>
	//DataLoc -> DefaultType -> Type::ALL -> we have full address scope of memory.
	template<uint8_t Bytes = 1, Type DataLoc = DefaultType>
	typename ByteSize<Bytes>::Type DirectReadBytes(Ptr offset, ErrorCode& e)
	{
		//if the memory we are trying to access is outside of the addressable range, 
		if (!AdressableRangeCheck(GetLwrBnd<DataLoc>() + offset + Bytes, e))
		{
			e.flag |= (int)ErrorFlags::INVALID_READ | (int)ErrorFlags::INVALID_ARGUMENT;
			SetMemSectionFlag<DataLoc>(e);
			return 0;
		}
		CheckMemUpper(GetUprBnd<DataLoc>(), GetLwrBnd<DataLoc>() + offset + Bytes, e);
		if (e.flag & (int)ErrorFlags::MEM_OUT_OF_BOUND)
		{
			e.flag |= (int)ErrorFlags::INVALID_READ;
			SetMemSectionFlag<DataLoc>(e);
			SetDebugECMsg(e, "Memory read through DirectReadByte out of bound.\n"
				"Read attepmpt at offset " + toHexStr(offset) +
				".\nNote tht valid memory locations for " + GetName<DataLoc>() + " are 0x00 to " +
				toHexStr(GetUprBnd<DataLoc>() - GetLwrBnd<DataLoc>()));
			return 0;
		}
		typename ByteSize<Bytes>::Type value = JoinBytes<Bytes, DataLoc>(offset);
		SetDebugECMsg(e, "Operation succeeded");
		return value;
	}

	//DataLoc: Which memory section to get raw pointer from
	template<Type DataLoc = DefaultType>
	uint8_t* GetRawPtr(Ptr offset, ErrorCode& e)
	{
		if (!AdressableRangeCheck(GetLwrBnd<DataLoc>() + offset, e))
		{
			e.flag |= (int)ErrorFlags::INVALID_READ | (int)ErrorFlags::INVALID_ARGUMENT;
			SetMemSectionFlag<DataLoc>(e);
			return nullptr;
		}
		CheckMemUpper(GetUprBnd<DataLoc>(), GetLwrBnd<DataLoc>() + offset, e);
		if (e.flag & (int)ErrorFlags::MEM_OUT_OF_BOUND)
		{
			e.flag |= (int)ErrorFlags::INVALID_READ;
			SetMemSectionFlag<DataLoc>(e);
			SetDebugECMsg(e, "Memory read through DirectReadByte out of bound.\n"
				"Read attepmpt at offset " + toHexStr(offset) +
				".\nNote tht valid memory locations for " + GetName<DataLoc>() + " are 0x00 to " +
				toHexStr(GetUprBnd<DataLoc>() - GetLwrBnd<DataLoc>()));
			return nullptr;
		}
		SetDebugECMsg(e, "Operation succeeded");
		return m_Memory.data() + GetLwrBnd<DataLoc>() + offset;
		//return m_Memory + GetLwrBnd<DataLoc>() + offset;
	}


	//PtrLoc: Which memory section to read the pointer from
	//DataLoc: Which memory section to read Data from
	//PtrBytes: How many bytes is the pointer
	//DataBytes: How many byes of data to read - can be 1, 2 or 4
	template<uint8_t PtrBytes = DefaultBytes, uint8_t DataBytes = 1, Type PtrLoc = DefaultType, Type DataLoc = DefaultType>
	typename ByteSize<DataBytes>::Type IndirectReadBytes(Ptr offset, ErrorCode& e)
	{
		typename ByteSize<PtrBytes>::Type adr = DirectReadBytes<PtrBytes, PtrLoc>(offset, e);
		typename ByteSize<DataBytes>::Type data = DirectReadBytes<DataBytes, DataLoc>(adr, e);
		return data;
	}

	//DataLoc: Which memory section to write data to
	//Bytes: How many bytes to write,
	//in this case 1 will mean you give a data of type uint8_t, 2 of uint16_t and 4 of uint32_t
	template<uint8_t Bytes = 1, Type DataLoc = DefaultType>
	void DirectWriteBytes(Ptr offsetWritingTo, ByteSize<Bytes>::Type dataToWrite, ErrorCode& e)
	{
		if (!AdressableRangeCheck(GetLwrBnd<DataLoc>() + offsetWritingTo, e))
		{
			e.flag |= (int)ErrorFlags::INVALID_READ | (int)ErrorFlags::INVALID_ARGUMENT;
			SetMemSectionFlag<DataLoc>(e);
			return;
		}
		CheckMemUpper(GetUprBnd<DataLoc>(), GetLwrBnd<DataLoc>() + offsetWritingTo, e);
		if (e.flag & (int)ErrorFlags::MEM_OUT_OF_BOUND)
		{
			e.flag |= (int)ErrorFlags::INVALID_WRITE;
			SetMemSectionFlag<DataLoc>(e);
			SetDebugECMsg(e, "Memory write through DirectWriteByte out of bound.\n" 
				"Write attepmpt at offset " + toHexStr(offsetWritingTo) +
				".\nNote tht valid memory locations for " + GetName<DataLoc>() + " are 0x00 to " +
				toHexStr(GetUprBnd<DataLoc>() - GetLwrBnd<DataLoc>()));
			return;
		}
		memcpy(m_Memory.data() + GetLwrBnd<DataLoc>() + offsetWritingTo, &dataToWrite, Bytes);
		//memcpy(m_Memory + GetLwrBnd<DataLoc>() + offset, &data, Bytes)
		SetDebugECMsg(e, "Operation succeeded");
	}

	//WRITE ANY NUMBER OF BYTES OF DATA. NOT TESTED, TEST BEFORE USE
	
	template<Type DataLoc = DefaultType>
	void DirectWriteAnyBytes(Ptr offset, uint8_t* data, Ptr bytes, ErrorCode& e)
	{
		if (!AdressableRangeCheck(GetLwrBnd<DataLoc>() + offset + bytes, e))
		{
			e.flag |= (int)ErrorFlags::INVALID_READ | (int)ErrorFlags::INVALID_ARGUMENT;
			SetMemSectionFlag<DataLoc>(e);
			return;
		}
		CheckMemUpper(GetUprBnd<DataLoc>(), GetLwrBnd<DataLoc>() + offset + bytes, e);
		if (e.flag & (int)ErrorFlags::MEM_OUT_OF_BOUND)
		{
			e.flag |= (int)ErrorFlags::INVALID_WRITE;
			SetMemSectionFlag<DataLoc>(e);
			SetDebugECMsg(e, "Memory write through DirectWriteByte out of bound.\n"
				"Write attepmpt at offset " + toHexStr(offset) +
				".\nNote tht valid memory locations for " + GetName<DataLoc>() + " are 0x00 to " +
				toHexStr(GetUprBnd<DataLoc>() - GetLwrBnd<DataLoc>()));
			return;
		}
		memcpy(m_Memory + GetLwrBnd<DataLoc>() + offset, data, bytes);
		SetDebugECMsg(e, "Operation succeeded");
	}

	//Which data loc is the byte located in 
	//How many bytes to join
	//join the bytes in reverse order?
	template<uint8_t Bytes = DefaultBytes, Type DataLoc = DefaultType, bool Reverse = true>
	typename ByteSize<Bytes>::Type JoinBytes(Ptr offset)
	{
		if constexpr (Bytes == 1)
		{
			return m_Memory[GetLwrBnd<DataLoc>() + offset];
		}

		typename ByteSize<Bytes>::Type index = 0;
		if constexpr (Reverse)
		{
			uint8_t i = Bytes;
			typename ByteSize<Bytes>::Type totalOffset = GetLwrBnd<DataLoc>() + offset;
			do
			{
				index = (index << (8 * (Bytes / 2))) | m_Memory[totalOffset + i - 1];
				i--;
			} while (i > 0);
			return index;
		}
		else
		{
			uint8_t i = 0;
			typename ByteSize<Bytes>::Type totalOffset = GetLwrBnd<DataLoc>() + offset;
			do
			{
				if constexpr (Bytes == 1)
				{
					index = m_Memory[totalOffset];
					break;
				}
				index = (index << (8 * (Bytes / 2))) | m_Memory[totalOffset + i];
				i++;
			} while (i < Bytes);
			return index;
		}
	}
	
	void	Reset();

private:
	void CheckMemUpper(uint16_t min, uint16_t value, ErrorCode& e);
	void CheckMemLower(uint16_t max, uint16_t value, ErrorCode& e);
	
private:
	bool AdressableRangeCheck(int p, ErrorCode& e);

public:
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

	//set back to private
public:
	//uint8_t* m_Memory;
	std::array<uint8_t, 0xFFFF> m_Memory = { 0 };
};

