#pragma once
#include <string>

enum class ErrorFlags : int
{
	MEM_OUT_OF_BOUND = 1 << 0,
	INVALID_READ = 1 << 1,
	INVALID_WRITE = 1 << 2,
	INVALID_ARGUMENT = 1 << 3,
	MEMORY_SECTION_RAM = 1 << 4,
	MEMORY_SECTION_ROM = 1 << 5,
	MEMORY_SECTION_STACK = 1 << 6,
	MEMORY_SECTION_VRAM = 1 << 7,
	MEMORY_SECTION_ALL = 1 << 8
};

struct ErrorCode
{
	int flag;
	void Clear();
#if defined(DEBUG) || defined(_DEBUG)
	std::string msg;
#endif
};


#if defined(DEBUG) || defined(_DEBUG)
#define SetDebugMsg(e, ...) e.msg = __VA_ARGS__
#define PrintDebugMsg(e) std::cout << e.msg << '\n';
#else
#define SetDebugStringMsg(...) 
#define PrintDebugMsg(e) 
#endif

