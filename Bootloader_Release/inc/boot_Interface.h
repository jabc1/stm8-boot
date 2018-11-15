#ifndef   _BOOT_INTERFACE_H_
#define   _BOOT_INTERFACE_H_

#include "stm8s.h"

//�û��������ʼ��ַ
#define MAIN_USER_Start_ADDR     ((uint32_t)0x8000+0XC00)

//����ָ������ָ������
typedef void ( *AppMainTyp)(void);


typedef struct
{
  //����һ���ֽ�����     ������ʱʱ��
  //����0���յ�����    ����δ�յ�����
  uint8_t (*ReceiveData_Byte)(uint8_t * /*BUFF*/, uint32_t /* TimeOut */);

  //����һ���ֽ�����   �޷�����
  void (*SendData_Byte)(uint8_t  /*Data*/);
  //����ָ����������     
  void (*SendData_Bytes)(uint8_t * /*BUFF*/, uint16_t /* length */);
}
BootInterface_Typ;  //ͨ�Žӿڽṹ��


//����ͨ�Žӿ�
extern const BootInterface_Typ BootInterface;

//���³�ʼ��STM8���ж�������  �������¶��嵽APP���ж�������
void STM8_HanderIqr_Init(void);


uint32_t FLASH_ReadWord(uint32_t Address);


void goto_app(void);



















#endif


