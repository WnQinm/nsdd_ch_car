/*
 * pit.c
 *
 *  Created on: 2023年4月8日
 *      Author: 清梦
 */


#include "pit.h"

void Main_pit_init()
{
    pit_us_init(MAIN_PIT_CH, 500);
    interrupt_set_priority(MAIN_PIT_PRIORITY, (4<<5)|5);
}

void IMG_pit_init()
{
    pit_ms_init(IMG_PIT_CH, 15);
    interrupt_set_priority(IMG_PIT_PRIORITY, (5<<5)|6);
}

void Encoder_pit_init()
{
    pit_ms_init(ENCODER_PIT_CH, GET_VELOCITY_INTERVAL);
    interrupt_set_priority(ENCODER_PIT_PRIORITY, (0<<5)|0);
}

void ADC_Battery_pit_init()
{
    pit_ms_init(ADC_BATTERY_PIT_CH, GET_BATTERY_VOLTAGE_INTERVAL);
    interrupt_set_priority(ADC_BATTERY_PIT_PRIORITY, (6<<5)|6);
}

void ALL_pit_init()
{
    Main_pit_init();
//    IMG_pit_init();
//    Encoder_pit_init();
}
