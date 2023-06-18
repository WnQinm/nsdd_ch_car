 /*
 * hall_stopline.c
 *
 *  Created on: 2023Äê5ÔÂ26ÈÕ
 *      Author: Lenovo
 */


#include <zf_common_headfile.h>
#include "hall_stopline.h"

void hallInit()
{
    gpio_init(HALL_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);
    timer_init(TIM_7, TIMER_MS);
}

bool judgeStopline(){
    uint32 output;
    gpio_get_level(HALL_PIN);

    output=gpio_get_level(HALL_PIN);

    return output;
}
