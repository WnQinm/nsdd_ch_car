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
