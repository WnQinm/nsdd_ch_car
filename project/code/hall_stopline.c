 /*
 * hall_stopline.c
 *
 *  Created on: 2023��5��26��
 *      Author: Lenovo
 */

#include "hall_stopline.h"

void hallInit()
{
    gpio_init(HALL_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);
}

bool judgeStopline()
{
    return gpio_get_level(HALL_PIN);
}
