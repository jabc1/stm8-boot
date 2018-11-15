#include "uart2.h"

void Init_UART2(void)
{
  UART2_Init(
             (u32)115200, 
             UART2_WORDLENGTH_8D, 
             UART2_STOPBITS_1, 
             UART2_PARITY_NO,
             UART2_SYNCMODE_CLOCK_DISABLE,
             UART2_MODE_TXRX_ENABLE
               ); 
  UART2_Cmd(ENABLE);
}

void Uart2_SendByte(uint8_t Byte)
{
  UART2_SendData8(Byte);
  while(( UART2_GetFlagStatus(UART2_FLAG_TXE)==RESET));
}

void Uart2_SendStr(u8 *pBuffer,u16 NumToWrite)
{
  while(NumToWrite--)
  {
    Uart2_SendByte(*pBuffer);
    pBuffer++;
  }
}


u8 Uart2_Receive(u8* ReceivedData,uint32_t t_count)
{
  while(t_count) 
  {
    if(UART2_GetFlagStatus(UART2_FLAG_RXNE)!=RESET)
    {
      *ReceivedData = UART2->DR;
      return 0 ;
    }
    t_count--;
  }
  return 1;  
}











