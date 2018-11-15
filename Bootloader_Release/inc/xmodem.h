#ifndef __XMODEM_H__
#define __XMODEM_H__
#include "stm8s.h"
#include "string.h"
#include "boot_interface.h"

//�Ƿ�ʱ�ܵ������  
#define  DEBUG      1

#if DEBUG  //�Ƿ�ʱ�ܵ������  
//�����ַ���
#define XM_SendDeug( LOG )   XmodemControl.IapFun->SendData_Bytes( LOG , strlen(LOG) )

#else

#define XM_SendDeug( LOG )   

#endif



#if DEBUG

#warning   ����Ҫ�������ɵ�BOOTLOADER ���� �����µ��� ��    MAIN_USER_Start_ADDR  ��ֵ

#endif

//֡��ʼͷ
#define SOH             0x01
#define STX             0x02
//�ļ��������
#define EOT             0x04
//Ӧ��
#define ACK             0x06
//��Ӧ��
#define NAK             0x15
//��ֹ����
#define CAN             0x18


typedef struct
{
  uint8_t Block;       //��һ�εĿ��
  uint8_t DataBuff[132];  //֡���ݻ���
 const BootInterface_Typ *IapFun;  //IAP��Ҫ�õ��ĺ���
}
XmodemControlTyp;  //Xmodem ���ƽṹ��


//Xmodem ���ƽṹ��
extern XmodemControlTyp   XmodemControl;

//IAP��ʼ��    װ����Ҫ�õ��ĺ���
void IAP_XmodemDownload_Init(const BootInterface_Typ *BootInter);

//ִ��IAP
void IAP_XmodemDownload(void);



#endif

