#include "flash.h"




void Flash_Write_bytes(uint32_t Address , uint8_t * DataBuff,uint16_t length)
{
  uint16_t Count=0;	
  for( Count=0 ; Count<length ; Count++ )
  {
    FLASH_ProgramByte(Address+Count,DataBuff[Count]);	
  }
}




