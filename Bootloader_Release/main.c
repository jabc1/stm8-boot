#include <string.h>
#include <stdio.h>
#include "stm8s.h"
#include "flash.h"
#include "xmodem.h"
#include "uart2.h"
#include "boot_interface.h"

void main(void)
{	
  disableInterrupts();  //关闭中断
 //重新初始化STM8的中断向量表  把它重新定义到APP的中断向量中   如果之前已经重映射了则不会重新操作
  STM8_HanderIqr_Init();
  
  //如果用户代码更新完成标记存在  则表示当前已经有APP代码  直接运行
  if(FLASH_ReadByte(MAIN_USER_Start_ADDR-1)==0xA5)
  {
   goto_app();  //运行APP
  }
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); //f_psc = 16MHz/1
  Init_UART2();
  
  FLASH_Unlock(FLASH_MEMTYPE_PROG);  //FLASH解锁
  //初始化   挂载IAP 需要用到的函数
  IAP_XmodemDownload_Init(&BootInterface);
  
  //这些调试输出语句必须要在 IAP_XmodemDownload_Init 之后调用  需要用到里面的函数指针
  XM_SendDeug("\r\n\r\n");
  XM_SendDeug("Bootloader Start...\r\n");	
  XM_SendDeug(__DATE__ "     " __TIME__ "\r\n");
  XM_SendDeug("Stm8s Uart IAP  Baud 115200  Xmodem 128Byte \r\n");
  XM_SendDeug("Bootloader Size 3K \r\n");
  
  while (1)
  {
    IAP_XmodemDownload();
  }
}









