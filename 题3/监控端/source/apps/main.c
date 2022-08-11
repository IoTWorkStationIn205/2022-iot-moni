/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  */
#include <string.h>
#include "board.h"
#include "hal_key.h"
#include "tim-board.h"
#include "timer_handles.h"
#include <math.h>
#include "NS_Radio.h"
#include "adc_reader.h"
#include "hal_oled.h"
#include "sht3x.h"
#include "usart1-board.h"
#include "iwdg.h"

float temp;
float humi;
float light;
uint8_t fan = 0;

static char oled_buf[20];
uint8_t Ra_Txbuf[20];
uint8_t Ra_Rxbuf[20];
uint8_t Us1_Txbuf[128];
uint8_t Us1_Rxbuf[128];

void Init() {
    BoardInitMcu();
    BoardInitPeriph();
    //keys_init();//按键初始化
	
	IWDG_Init(4,2312);
	OLED_Init();
	NS_RadioInit(433000000,20,1000,1000);
	ADCS_Init();
	USART1_Init(115200);
	SHTXX_Init();
	
	ReadRadioRxBuffer(Ra_Rxbuf);
	
    setTimer2Callback(Time2Handler);
    Tim2McuInit(1000);//定时器初始化，设置定时中断1ms中断一次
	
	setTimer3Callback(Time3Handler);
    Tim3McuInit(4);//定时器初始化，设置定时中断1ms中断一次
}

void xiafa(void){
	if(temp > 30){
		Ra_Txbuf[0] = 0xD1;
		Ra_Txbuf[1] = 0x01;
	}else if(temp < 25){
		Ra_Txbuf[0] = 0xD1;
		Ra_Txbuf[1] = 0x02;
	}else{}
	
	HAL_Delay(20);
	Radio.Send(Ra_Txbuf,2);
		
	if(light < 200){
		GpioWrite(&Led1,0);
		GpioWrite(&Led2,0);
		
		Ra_Txbuf[0] = 0xD3;
		Ra_Txbuf[1] = 0x01;
	}else if(light > 200){
		GpioWrite(&Led1,1);
		GpioWrite(&Led2,1);
		
		Ra_Txbuf[0] = 0xD3;
		Ra_Txbuf[1] = 0x02;
	}else{}
		
	HAL_Delay(20);
	Radio.Send(Ra_Txbuf,2);
}

void CollectSensorData(void){
	AdcScanChannel();
	light = AdcReadCh0();
	light = pow(10,((1.78 - log10(33/light-10))/0.6));
	call_sht11(&temp,&humi);
}

void ShowSensorData(void){
	
	OLED_ShowString(40,0,(uint8_t*)"     ");
	OLED_ShowString(40,0,(uint8_t*)"     ");
	OLED_ShowString(48,0,(uint8_t*)"        ");
	
	memset(oled_buf,0,20);
	sprintf(oled_buf,"%.2f",temp);
	OLED_ShowString(40,0,(uint8_t*)oled_buf);
	
	memset(oled_buf,0,20);
	sprintf(oled_buf,"%.2f",humi);
	OLED_ShowString(40,2,(uint8_t*)oled_buf);
	
	memset(oled_buf,0,20);
	sprintf(oled_buf,"%.2f",light);
	OLED_ShowString(48,4,(uint8_t*)oled_buf);
}

void PackSensorData(void){
	if(fan == 1){
		Us1_Txbuf[0] = 0x31;
	}else{
		Us1_Txbuf[0] = 0x30;
	}
	
	sprintf((char*)(Us1_Txbuf+1),"|%.2f|%.2f|%.2f",temp,humi,light);
	USART1_SendStr(Us1_Txbuf,strlen((char*)Us1_Txbuf));
}
																															
void oled1(void){
	OLED_ShowString(0,0,"Temp:");
	OLED_ShowString(0,2,"Humi:");
	OLED_ShowString(0,4,"Light:");
	
	ShowSensorData();
}

/**
 * Main application entry point.
 */
int main( void )
{
    Init();
	
	oled1();
	
    while( 1 )
    {
		if(ReadRadioRxBuffer(Ra_Rxbuf) > 0){
			if(Ra_Rxbuf[0] == 0xE1){	
				if(Ra_Rxbuf[1] == 0x01){
					fan = 1;
				}else{
					fan = 0;
				}
			}	
		}
		
		IWDG_Feed();
    }
}

void uart1Call(void){
	USART1_ReadRxBuffer(Us1_Rxbuf);
	
	if(Us1_Rxbuf[0] == 0xA1){
		Us1_Rxbuf[0] = 0xD4;
		
		Radio.Send(Us1_Rxbuf,2);
	}else if(Us1_Rxbuf[0] == 0xA2){
		Us1_Rxbuf[0] = 0xD2;
		
		Radio.Send(Us1_Rxbuf,4);
	}else{
		
	}
}

void Key2Call(void){
	Ra_Txbuf[0] = 0xD2;
	Ra_Txbuf[1] = 0xFF;
	Ra_Txbuf[2] = 0x00;
	Ra_Txbuf[3] = 0x00;
	
	
	Radio.Send(Ra_Txbuf,4);
}

void Key3Call(void){
	Ra_Txbuf[0] = 0xD2;
	Ra_Txbuf[1] = 0x00;
	Ra_Txbuf[2] = 0xFF;
	Ra_Txbuf[3] = 0x00;
	
	Radio.Send(Ra_Txbuf,4);
}

void Key4Call(void){
	Ra_Txbuf[0] = 0xD2;
	Ra_Txbuf[1] = 0x00;
	Ra_Txbuf[2] = 0x00;
	Ra_Txbuf[3] = 0xFF;
	
	Radio.Send(Ra_Txbuf,4);
}
