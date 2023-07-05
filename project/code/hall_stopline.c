 /*
 * hall_stopline.c
 *
 *  Created on: 2023Äê5ÔÂ26ÈÕ
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

void out_garage()
{
    ips200_show_string(0, 0, "wait for key1 to start...");
    while(!out_garage_flag)
    {
        key_scanner();
        switch(key_get_state(KEY_1))
        {
            case KEY_SHORT_PRESS:
            case KEY_LONG_PRESS:
                ips200_show_string(0, 20, "running out...");
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
                motor_control(1200,1200);
                system_delay_ms(1000);
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(75));
                system_delay_ms(875);
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
                motor_control(0,0);
                system_delay_ms(300);
                motor_control(-1200,-1200);
                system_delay_ms(1500);
                motor_control(0,0);
                out_garage_flag = true;
                break;
            default:
                break;
        }
    }
    ips200_clear();
}

void In_Garage()
{
    motor_control(1000,1000);
    system_delay_ms(800);
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(75));
    system_delay_ms(800);
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
    motor_control(0,0);

    ips200_clear();
    ips200_show_string(0,0,"Finished!");
    while(1){}
}
