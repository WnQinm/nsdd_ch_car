/*
 * pit.c
 *
 *  Created on: 2023��4��8��
 *      Author: ����
 */


#include "pit.h"

void Main_pit_init()
{
    pit_us_init(MAIN_PIT_CH, 500);
    interrupt_set_priority(MAIN_PIT_PRIORITY, (4<<5)|5);
}
