/* Includes ------------------------------------------------------------------*/
#include "timer_handles.h"
#include "key_scaner.h"
#include "uart_receiver.h"

void SendFanData(void);

void Time2Handler(){
	SendFanData();
}

void Time3Handler(){
}
