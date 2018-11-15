#ifndef __XMODEM_H__
#define __XMODEM_H__
#include "stm8s.h"
#include "string.h"
#include "boot_interface.h"

//是否时能调试输出  
#define  DEBUG      1

#if DEBUG  //是否时能调试输出  
//发送字符串
#define XM_SendDeug( LOG )   XmodemControl.IapFun->SendData_Bytes( LOG , strlen(LOG) )

#else

#define XM_SendDeug( LOG )   

#endif



#if DEBUG

#warning   你需要根据生成的BOOTLOADER 代码 来重新调整 宏    MAIN_USER_Start_ADDR  的值

#endif

//帧起始头
#define SOH             0x01
#define STX             0x02
//文件传输完成
#define EOT             0x04
//应答
#define ACK             0x06
//非应答
#define NAK             0x15
//中止传输
#define CAN             0x18


typedef struct
{
  uint8_t Block;       //下一次的块号
  uint8_t DataBuff[132];  //帧数据缓存
 const BootInterface_Typ *IapFun;  //IAP需要用到的函数
}
XmodemControlTyp;  //Xmodem 控制结构体


//Xmodem 控制结构体
extern XmodemControlTyp   XmodemControl;

//IAP初始化    装载需要用到的函数
void IAP_XmodemDownload_Init(const BootInterface_Typ *BootInter);

//执行IAP
void IAP_XmodemDownload(void);



#endif

