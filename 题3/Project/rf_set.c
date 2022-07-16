#include "hal_defs.h"
#include "hal_cc8051.h"
#include "hal_int.h"
#include "hal_mcu.h"
#include "hal_board.h"
#include "hal_led.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "hal_uart.h" 
#include "sensor_drv/sensor.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

uint8 epc1[12] = {0xE2 ,0x00 ,0x00 ,0x1D ,0x33 ,0x0E ,0x00 ,0x86 ,0x24 ,0x90 ,0x33 ,0xAB };
uint8 epc2[12] = {0xE2 ,0x00 ,0x00 ,0x1D ,0x33 ,0x0E ,0x00 ,0x86 ,0x24 ,0x90 ,0x33 ,0xA1 };


/*****点对点通讯地址设置******/
#define RF_CHANNEL                11         // 频道 11~26
#define PAN_ID                    0x0016     //网络id 
#define MY_ADDR                   0x0001     //本机模块地址
#define SEND_ADDR                 0x0002     //发送地址
/**************************************************/
static basicRfCfg_t basicRfConfig;
// 无线RF初始化
void ConfigRf_Init(void)
{
    basicRfConfig.panId       =   PAN_ID;
    basicRfConfig.channel     =   RF_CHANNEL;
    basicRfConfig.myAddr      =   MY_ADDR;
    basicRfConfig.ackRequest  =   TRUE;
    while(basicRfInit(&basicRfConfig) == FAILED);
    basicRfReceiveOn();
}

void IO_Init(void){
  P1IEN |= 0X04;
  PICTL |= 0X02;
  IEN2  |= 0X10;
  P1IFG &= ~0X04;
  P1IF =0;
  
  P0IEN |= 0X02;
  PICTL |= 0X01;
  P0IE  =  1;
  P0IFG &= ~0X04;
  P0IF = 0;
  
  MCU_IO_OUTPUT(2,0,0);
}

uint8 chuli(uint8 *buf,uint8 *epc){
  for(int a = 0;a<12;a++){
    if(buf[a+15] != epc[0]){
      return 0;
    }
    
    if(a == 11){
      return 1;
    }
  }
  
  return 0;
}

/********************MAIN************************/
void main(void)
{
    halBoardInit();//选手不得在此函数内添加代码
    ConfigRf_Init();//选手不得在此函数内添加代码

    IO_Init();
    halUartInit(115200);
    
    uint8 RxBuf[128];
    while(1)
    {
    /* user code start */
      
     
    /* user code end */
    }
}
