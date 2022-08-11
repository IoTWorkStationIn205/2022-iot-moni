/* Includes ------------------------------------------------------------------*/
#include "timer_handles.h"
#include "key_scaner.h"
#include "uart_receiver.h"

void CollectSensorData(void);
void ShowSensorData(void);
void xiafa(void);
void PackSensorData(void);

void Time2Handler(){
	CollectSensorData();
	ShowSensorData();
	xiafa();
	PackSensorData();
}

void Time3Handler(){
	uartReceiver();
}
