/*
 * pit.c
 *
 *  Created on: 2023年4月8日
 *      Author: 清梦
 */


#include "pit.h"

void IMG_pit_init()
{
    pit_ms_init(IMG_PIT_CH, 5);
    interrupt_set_priority(IMG_PIT_PRIORITY, 10);
}
