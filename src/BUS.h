#pragma once

#include "CPU.h"
#include "MEM.h"

class BUS
{
public:
    BUS();
    BUS(CPU* cpu, Memory* mem);
    ~BUS();

    //NOTE TO NATE: we may not even need emulate the read and write signal because it is implied via the function calls.
    //p.s. we may need uint16_t read if we are reading into 16bit register and write will also need more functions.
    //void write(uint16_t addr, uint8_t data);
    //uint8_t read(uint16_t addr, bool bReadOnly = false);
    void write8(uint16_t addr, uint8_t data);
    void write16(uint16_t addr, uint16_t data);

    void readptr(uint16_t addr);
    void read8(uint16_t addr);
    void read16(uint16_t addr);

    void ConnetMem(Memory* mem); //this can be expanded on in order to include more components.
    void ConnetCpu(CPU* cpu); //this can be expanded on in order to include more components.

private:
    ErrorCode LastError;
    CPU* m_pCPU = nullptr;
    Memory* m_pMEM = nullptr;
};