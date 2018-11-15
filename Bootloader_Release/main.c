#include <string.h>
#include <stdio.h>
#include "stm8s.h"
#include "flash.h"
#include "xmodem.h"
#include "uart2.h"
#include "boot_interface.h"

void main(void)
{	
  disableInterrupts();  //�ر��ж�
 //���³�ʼ��STM8���ж�������  �������¶��嵽APP���ж�������   ���֮ǰ�Ѿ���ӳ�����򲻻����²���
  STM8_HanderIqr_Init();
  
  //����û����������ɱ�Ǵ���  ���ʾ��ǰ�Ѿ���APP����  ֱ������
  if(FLASH_ReadByte(MAIN_USER_Start_ADDR-1)==0xA5)
  {
   goto_app();  //����APP
  }
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); //f_psc = 16MHz/1
  Init_UART2();
  
  FLASH_Unlock(FLASH_MEMTYPE_PROG);  //FLASH����
  //��ʼ��   ����IAP ��Ҫ�õ��ĺ���
  IAP_XmodemDownload_Init(&BootInterface);
  
  //��Щ�������������Ҫ�� IAP_XmodemDownload_Init ֮�����  ��Ҫ�õ�����ĺ���ָ��
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









