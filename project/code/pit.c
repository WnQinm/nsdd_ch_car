/*
 * pit.c
 *
 *  Created on: 2023��4��8��
 *      Author: ����
 */


#include "pit.h"

void IMG_pit_init()
{
    pit_ms_init(IMG_PIT_CH, 5);
    interrupt_set_priority(IMG_PIT_PRIORITY, 5);
}

void Servo_pit_init()
{
    pit_ms_init(SERVO_PIT_CH, 5);
    interrupt_set_priority(SERVO_PIT_PRIORITY, 6);
}
