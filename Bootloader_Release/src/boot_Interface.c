#include "boot_interface.h"
#include "flash.h"
#include "uart2.h"


//����ͨ�Žӿ�  �����������Ҫ���ֶ����
const BootInterface_Typ BootInterface={
Uart2_Receive,
Uart2_SendByte,
Uart2_SendStr
};

uint32_t FLASH_ReadWord(uint32_t Address)
{
 return(*(PointerAttr uint32_t *) (uint16_t)Address);       
}


//���³�ʼ��STM8���ж�������  �������¶��嵽APP���ж�������
void STM8_HanderIqr_Init(void)
{
  disableInterrupts();   //�ر��ж�  
  uint8_t Index;	
  FLASH_Unlock(FLASH_MEMTYPE_PROG);
  for(Index = 1; Index < 0X20;Index++)
  {
   if(FLASH_ReadWord(0X8000+4*Index)!=(0X82000000+MAIN_USER_Start_ADDR+Index*4))
   {
    FLASH_ProgramWord(0X8000+4*Index,0X82000000+MAIN_USER_Start_ADDR+Index*4);
   }
  }
  FLASH_Lock(FLASH_MEMTYPE_PROG);
}

//��ת���û�����
void goto_app(void)
{
  const AppMainTyp MainUserApplication = (AppMainTyp)MAIN_USER_Start_ADDR;
  if((*((u8 FAR*)MainUserApplication)==0x82) || (*((u8 FAR*)MainUserApplication)==0xAC))
  {
    MainUserApplication();
  }	
}















