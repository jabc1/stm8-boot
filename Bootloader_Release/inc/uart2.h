#ifndef __UAER2_H__
#define __UART2_H__

#include "stm8s.h"

void Init_UART2(void);

u8 Uart2_Receive(u8* ReceivedData,uint32_t t_count);

void Uart2_SendByte(uint8_t Byte);

void Uart2_SendStr(u8 *pBuffer,u16 NumToWrite);






#endif


