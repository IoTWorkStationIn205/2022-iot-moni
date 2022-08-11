//-------------------------------------------------------------------
// Filename: hal_led.h
//-------------------------------------------------------------------
#ifndef _HAL_LED_H_
#define _HAL_LED_H_
//-------------------------------------------------------------------
// GLOBAL FUNCTIONS
//-------------------------------------------------------------------
void halLedInit();
void halLedSet(uint8 led_id);
void halLedClear(uint8 led_id);
void halLedToggle(uint8 led_id);

#endif
