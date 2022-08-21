#include "hal_defs.h"
#include "hal_cc8051.h"
#include "hal_int.h"
#include "hal_mcu.h"
#include "hal_board.h"
#include "hal_led.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "hal_uart.h" 
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "TIMER.h"

uint32 counter = 0;
uint8 EPC_Txbuf[16] = {0xFF ,0x55 ,0x00 ,0x00 ,0x03 ,0x0A ,0x07 ,0xBB ,0x00 ,0x22 ,0x00 ,0x00 ,0x22 ,0x7E ,0xC7 ,0xC0};
uint8 EPC_a[12] = {0xE2 ,0x00 ,0x00 ,0x1D ,0x33 ,0x0E ,0x02 ,0x09 ,0x24 ,0x00 ,0xBA ,0xC0};
uint8 EPC_b[12] = {0xE2 ,0x00 ,0x41 ,0x06 ,0x27 ,0x11 ,0x00 ,0x38 ,0x23 ,0x00 ,0x2A ,0x91};


void Io_Init(void){
  P1IEN |= 0X04;
  PICTL |= 0X02;
  IEN2  |= 0X10;
  P1IFG &= ~0X04;
  P1IF = 0;
  
  P0IEN |= 0X02;
  PICTL |= 0X01;
  P0IE = 1;
  P0IFG &= ~0X04;
  P0IF = 0;
  
  MCU_IO_OUTPUT(2,0,0);
  MCU_IO_OUTPUT(1,7,0);
  
  MCU_IO_OUTPUT(1,0,0);
  MCU_IO_OUTPUT(1,1,0);
  MCU_IO_OUTPUT(1,3,0);
  MCU_IO_OUTPUT(1,4,0);
}


int CheakEpc(uint8* buf){
  halUartWrite(buf,12);
  
  for(int a = 0;a<12;a++){
    if(buf[a] != EPC_a[a]){
      break;
    }
    if(a == 11){
      return 1;
    }
  }
  
   for(int a = 0;a<12;a++){
    if(buf[a] != EPC_b[a]){
      return -1;
    }
    if(a == 11){
      return 2;
    }
  }
   return -1;
} 

int Boli(uint8* buf,uint8 num){
  uint8 bufPtr = 0;
  uint8 ii = 0;

  for(int a = 0;a < num;a++){
    bufPtr += 8+(a*16);
    ii = CheakEpc((buf+bufPtr));
    if(ii == 1 || ii == 2){
      return ii;
    }
  }
  
  return ii;
}

void Clear_Data(void){
  P1_0 = 0;
  P1_1 = 0;
  P1_3 = 0;
  P1_4 = 0;
  
  P2_0 = 0;

  P1_7 = 0;
  
  counter = 0;
}

/********************MAIN************************/
void main(void)
{
    halBoardInit();
    halUartInit(115200);
    Timer4_Init();
    Io_Init();
    
    Timer4_On();
    
    uint8 Rxbuf[128];
    int ii = 0;
    uint8 len = 0;
    while(1)
    {
      halUartWrite(EPC_Txbuf,16);
      halMcuWaitMs(1500);
      len = halUartRead(Rxbuf,128);
      if(len > 18){
        ii = Boli(Rxbuf+7,Rxbuf[6]/18);
        //ii = CheakEpc(Rxbuf+15);
        if(ii == 1){
          Clear_Data();
          P1_0 = 1;
          P1_1 = 1;
          P2_0 = 1;
        }else if(ii == 2){
          Clear_Data();
          P1_3 = 1;
          P1_4 = 1;
          P2_0 = 1;
        }else{
          Clear_Data();
          P1_7 = 1;
        }
      }
      halMcuWaitMs(2);
      
    }
}

void Time4_Call(void){
  counter++;
  if(counter > 5000){
    Clear_Data();
  }
}

#pragma vector = P1INT_VECTOR
__interrupt void Sw1(void){
  P1IFG &= ~0X04;
  P1IF = 0;
  
  Clear_Data();
  P2_0 = 1;
  
  P1IFG &= ~0X04;
  P1IF = 0;
} 

#pragma vector = P0INT_VECTOR
__interrupt void Sw2(void){
  P0IFG &= ~0X02;
  P0IF = 0;
  
  Clear_Data();
  P1_7 = 1;
  
  
  P0IFG &= ~0X02;
  P0IF = 0;
} 