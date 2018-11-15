#include "xmodem.h"
#include "flash.h"
#include "boot_interface.h"

//Xmodem 控制结构体
XmodemControlTyp   XmodemControl;




//返回Xmodem CRC校验值  累加校验
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

//IAP初始化    装载需要用到的函数
void IAP_XmodemDownload_Init(const BootInterface_Typ *BootInter)
{
  XmodemControl.IapFun = BootInter;
}

void IAP_XmodemDownload(void)
{
  uint8_t RecData = 0 ;
  uint16_t Count = 0 ;
  //当前写FLASH地址  --------------
  uint32_t IAPFlashAddr = MAIN_USER_Start_ADDR;
  XmodemControl.IapFun->SendData_Byte(NAK);   //发送一个非应答  表示为Xmodem 128字节协议
  if(XmodemControl.IapFun->ReceiveData_Byte(&RecData,100000)==0) //如果有收到数据
  {
    if(RecData==SOH)  //如果收到数据帧起始信号
    {
      XmodemControl.DataBuff[0]=RecData;
      XmodemControl.Block = 1 ; //刚开始接收 标记块号为 1
      //如果只要接收到帧起始头
      while(XmodemControl.DataBuff[0]==SOH)
      {
        for(Count = 1 ; Count < 132 ; Count++ )
        {
          XmodemControl.IapFun->ReceiveData_Byte(&XmodemControl.DataBuff[Count],10000);  //接收接下来的 131个字节数据 
        } 
        //块号与 块反码      
        if(( XmodemControl.DataBuff[1]+ XmodemControl.DataBuff[2]==0XFF )&&
           (XmodemControl.DataBuff[1] == XmodemControl.Block ) &&      //块号与上一个块号  
             (XmodemAddCRC_Check(&XmodemControl.DataBuff[3], 128) == XmodemControl.DataBuff[131] ))    //CRC校验值
        {
          XmodemControl.Block++;
          for( Count=0 ; Count<128 ; Count++ ,IAPFlashAddr++ )
          {
            FLASH_ProgramByte(IAPFlashAddr,XmodemControl.DataBuff[3+Count]); //写FLASH数据	
          }
          XmodemControl.IapFun->SendData_Byte(ACK);  //请求下一个包
        }
        else
        {
          //请求重发
          XmodemControl.IapFun->SendData_Byte(NAK);       
        }
        //接收数据帧的起始头   必须接收到
        while(1)
        {
          if(XmodemControl.IapFun->ReceiveData_Byte(&XmodemControl.DataBuff[0],10000)==0) //如果有收到数据   
          {
            if(XmodemControl.DataBuff[0]==SOH) //如果收到起始头
            {
              break;
            }
            else if(XmodemControl.DataBuff[0]==EOT)  //文件传输完成了 
            {
              XmodemControl.IapFun->SendData_Byte(ACK);  //发送应答
              XM_SendDeug("Goto app...");
              //如果用户APP的起始地址 为跳转命令 则认为APP代码是正常的
              if((*((uint8_t FAR*)MAIN_USER_Start_ADDR)==0x82) || (*((u8 FAR*)MAIN_USER_Start_ADDR)==0xAC))
              {
                FLASH_ProgramByte(MAIN_USER_Start_ADDR-1,0xA5);  //写入APP更新完成标记
                XM_SendDeug("\r\n go to app Succeed!\r\n\r\n");   
                goto_app();
              }	
              XM_SendDeug("\r\n go to app Failed!\r\n\r\n");                 
              return;
            }
            else  //其它数据  认为终止传输   则会重新开始
            {
              return;
            }         
          } 
        }     
      }
    }    
  }      
}






