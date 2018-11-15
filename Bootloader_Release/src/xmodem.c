#include "xmodem.h"
#include "flash.h"
#include "boot_interface.h"

//Xmodem ���ƽṹ��
XmodemControlTyp   XmodemControl;




//����Xmodem CRCУ��ֵ  �ۼ�У��
uint8_t XmodemAddCRC_Check( const unsigned char *buf, uint16_t length)
{
  uint16_t Count;
  uint8_t  CRCdata = 0 ; 
  for(Count = 0 ; Count < length ;Count ++)
  {
    CRCdata += buf[Count] ; 
  }
  return CRCdata;
}

//IAP��ʼ��    װ����Ҫ�õ��ĺ���
void IAP_XmodemDownload_Init(const BootInterface_Typ *BootInter)
{
  XmodemControl.IapFun = BootInter;
}

void IAP_XmodemDownload(void)
{
  uint8_t RecData = 0 ;
  uint16_t Count = 0 ;
  //��ǰдFLASH��ַ  --------------
  uint32_t IAPFlashAddr = MAIN_USER_Start_ADDR;
  XmodemControl.IapFun->SendData_Byte(NAK);   //����һ����Ӧ��  ��ʾΪXmodem 128�ֽ�Э��
  if(XmodemControl.IapFun->ReceiveData_Byte(&RecData,100000)==0) //������յ�����
  {
    if(RecData==SOH)  //����յ�����֡��ʼ�ź�
    {
      XmodemControl.DataBuff[0]=RecData;
      XmodemControl.Block = 1 ; //�տ�ʼ���� ��ǿ��Ϊ 1
      //���ֻҪ���յ�֡��ʼͷ
      while(XmodemControl.DataBuff[0]==SOH)
      {
        for(Count = 1 ; Count < 132 ; Count++ )
        {
          XmodemControl.IapFun->ReceiveData_Byte(&XmodemControl.DataBuff[Count],10000);  //���ս������� 131���ֽ����� 
        } 
        //����� �鷴��      
        if(( XmodemControl.DataBuff[1]+ XmodemControl.DataBuff[2]==0XFF )&&
           (XmodemControl.DataBuff[1] == XmodemControl.Block ) &&      //�������һ�����  
             (XmodemAddCRC_Check(&XmodemControl.DataBuff[3], 128) == XmodemControl.DataBuff[131] ))    //CRCУ��ֵ
        {
          XmodemControl.Block++;
          for( Count=0 ; Count<128 ; Count++ ,IAPFlashAddr++ )
          {
            FLASH_ProgramByte(IAPFlashAddr,XmodemControl.DataBuff[3+Count]); //дFLASH����	
          }
          XmodemControl.IapFun->SendData_Byte(ACK);  //������һ����
        }
        else
        {
          //�����ط�
          XmodemControl.IapFun->SendData_Byte(NAK);       
        }
        //��������֡����ʼͷ   ������յ�
        while(1)
        {
          if(XmodemControl.IapFun->ReceiveData_Byte(&XmodemControl.DataBuff[0],10000)==0) //������յ�����   
          {
            if(XmodemControl.DataBuff[0]==SOH) //����յ���ʼͷ
            {
              break;
            }
            else if(XmodemControl.DataBuff[0]==EOT)  //�ļ���������� 
            {
              XmodemControl.IapFun->SendData_Byte(ACK);  //����Ӧ��
              XM_SendDeug("Goto app...");
              //����û�APP����ʼ��ַ Ϊ��ת���� ����ΪAPP������������
              if((*((uint8_t FAR*)MAIN_USER_Start_ADDR)==0x82) || (*((u8 FAR*)MAIN_USER_Start_ADDR)==0xAC))
              {
                FLASH_ProgramByte(MAIN_USER_Start_ADDR-1,0xA5);  //д��APP������ɱ��
                XM_SendDeug("\r\n go to app Succeed!\r\n\r\n");   
                goto_app();
              }	
              XM_SendDeug("\r\n go to app Failed!\r\n\r\n");                 
              return;
            }
            else  //��������  ��Ϊ��ֹ����   ������¿�ʼ
            {
              return;
            }         
          } 
        }     
      }
    }    
  }      
}






