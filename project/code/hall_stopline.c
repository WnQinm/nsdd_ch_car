/*
 * hall_stopline.c
 *
 *  Created on: 2023Äê5ÔÂ26ÈÕ
 *      Author: Lenovo
 */


#include <zf_common_headfile.h>

#define LED1 (E2)
#define LED2 (E9)
#define HALL_PIN                (B2 )

uint32 trigger_count = 0;
uint32 trigger_state = 0;

void hallinit()
{
    gpio_init(HALL_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);
            gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);
            gpio_init(LED2, GPO, GPIO_HIGH, GPO_PUSH_PULL);
            timer_init(TIM_7, TIMER_MS);
            printf("\r\nHall test.");
}

uint16 stopline()
{
    uint32 output;

    if(gpio_get_level(HALL_PIN))
                {
                    timer_stop(TIM_7);
                    trigger_count = timer_get(TIM_7);
                    timer_clear(TIM_7);
                    trigger_state = 0;
                    gpio_set_level(LED2, GPIO_HIGH);
                }
                else
                {
                    gpio_set_level(LED2, GPIO_LOW);
                    if(0 == trigger_state)
                    {
                        trigger_state = 1;
                        timer_start(TIM_7);
                    }
                }
                if(trigger_count)
                {
                    printf("\r\nHall trigger: %d ms.", trigger_count);
                    trigger_count = 0;
                }

                output=trigger_count;
          return output;
}
