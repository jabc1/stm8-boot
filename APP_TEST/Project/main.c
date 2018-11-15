#include "stm8s.h"
#include "string.h"


void Usart_SendStr(uint8_t* buff,uint16_t length )
{
  while(length--)
  {
    UART2_SendData8(*buff++); 
    while (UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET);
  }
}

#define DebugLog(x)   Usart_SendStr(x,strlen(x))


void Delay(uint32_t time)
{
  while(time--);
}


int main( void )
{ 
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  GPIO_Init(GPIOE,GPIO_PIN_5,GPIO_MODE_OUT_PP_HIGH_FAST);
  UART2_Init(115200,UART2_WORDLENGTH_8D,UART2_STOPBITS_1,UART2_PARITY_NO,UART2_SYNCMODE_CLOCK_DISABLE,UART2_MODE_TXRX_ENABLE);
  UART2_ITConfig(UART2_IT_RXNE,ENABLE); //开启接收中断
  UART2_Cmd(ENABLE);
  enableInterrupts();   //开启中断
  while(1)
  {
    DebugLog("1234\r\n");
    Delay(10000);
  }  
}


