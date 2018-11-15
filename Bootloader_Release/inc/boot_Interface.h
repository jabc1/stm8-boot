#ifndef   _BOOT_INTERFACE_H_
#define   _BOOT_INTERFACE_H_

#include "stm8s.h"

//用户代码的起始地址
#define MAIN_USER_Start_ADDR     ((uint32_t)0x8000+0XC00)

//定义指向函数的指针类型
typedef void ( *AppMainTyp)(void);


typedef struct
{
  //接收一个字节数据     附带超时时间
  //返回0接收到数据    其它未收到数据
  uint8_t (*ReceiveData_Byte)(uint8_t * /*BUFF*/, uint32_t /* TimeOut */);

  //发送一个字节数据   无返回型
  void (*SendData_Byte)(uint8_t  /*Data*/);
  //发送指定长度数据     
  void (*SendData_Bytes)(uint8_t * /*BUFF*/, uint16_t /* length */);
}
BootInterface_Typ;  //通信接口结构体


//数据通信接口
extern const BootInterface_Typ BootInterface;

//重新初始化STM8的中断向量表  把它重新定义到APP的中断向量中
void STM8_HanderIqr_Init(void);


uint32_t FLASH_ReadWord(uint32_t Address);


void goto_app(void);



















#endif


