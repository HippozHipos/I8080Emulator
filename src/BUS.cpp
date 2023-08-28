#include "BUS.h"


BUS::BUS()
{
    //not complete, we need to access the ram and clear it
    //clear ram contents
    m_pMEM->Reset();
}

BUS::BUS(CPU* cpu, Memory* mem) 
{
    ConnetMem(mem);
    ConnetCpu(cpu);
    //not complete, we need to access the ram and clear it
    //clear ram contents
    m_pMEM->Reset();
}

BUS::~BUS()
{
    // ?? We dont own theese pointers ??
    //delete m_pCPU;
    //delete m_pMEM;
}

void BUS::ConnetMem(Memory* mem)
{
    if (mem == nullptr)
    {
        LastError.Clear();
        LastError.flag |= (int)ErrorFlags::INVALID_POINTER;
        return;
    }
    m_pMEM = mem;

}

void BUS::ConnetCpu(CPU* cpu)
{
    if (cpu == nullptr)
    {
        LastError.Clear();
        LastError.flag |= (int)ErrorFlags::INVALID_POINTER;
        return;
    }
    m_pCPU = cpu;
}

void BUS::write8(uint16_t addr, uint8_t data)
{
    LastError.Clear();
    m_pMEM->DirectWriteBytes(addr, data, LastError);
}

void BUS::write16(uint16_t addr, uint16_t data)
{
    LastError.Clear();
    m_pMEM->DirectWriteBytes<2>(addr, data, LastError);
}

void BUS::readptr(uint16_t addr)
{
    LastError.Clear();
    m_pMEM->IndirectReadBytes(addr, LastError);
}

void BUS::read8(uint16_t addr)
{
    LastError.Clear();
    m_pMEM->DirectReadBytes(addr, LastError);
}

void BUS::read16(uint16_t addr)
{
    LastError.Clear();
    m_pMEM->DirectReadBytes<2>(addr, LastError);
}