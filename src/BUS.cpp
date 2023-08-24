//#include "BUS.h"
//
//
//BUS::BUS()
//{
//	//not complete, we need to access the ram and clear it
//	//clear ram contents
//	m_pMEM->Reset();
//}
///*
//BUS::~BUS()
//{
//}
//
//void BUS::write(uint16_t addr, uint8_t data)
//{
//	//ram guard
//	if (addr >= RAM_LWRBND || addr <= RAM_UPRBND) //this will also be changed when we include ROM memory as well.
//		m_pCPU.ram[addr] = data;
//}
//
//uint8_t BUS::read(uint16_t addr, bool bReadOnly = false) 
//{
//	if (addr >= RAM_LWRBND || addr <= 0xFFFF) //physically shouldn't be able to read beyond RAM or ROM
//		return m_pCPU.ram[addr];
//
//	return 0x00
//}
//
//*/
