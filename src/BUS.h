#pragma once

#include "CPU.h"
#include "MEM.h"

//forward declaration so friend class works
class CPU;
class MEM;

class BUS
{
public:
	BUS();
	//~BUS();

	////NOTE TO NATE: we may not even need emulate the read and write signal because it is implied via the function calls.
	////p.s. we may need uint16_t read if we are reading into 16bit register and write will also need more functions.
	//void write(uint16_t addr, uint8_t data);
	//uint8_t read(uint16_t addr, bool bReadOnly = false);
	//void ConnectComponents(CPU&, Memory&); //this can be expanded on in order to include more components.

private:
	CPU* m_pCPU;
	Memory* m_pMEM;
};

