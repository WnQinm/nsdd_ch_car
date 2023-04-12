/*
 * motor.c
 *
 *  Created on: 2023年4月8日
 *      Author: 清梦
 */


#include "motor.h"

//初始化电机PWM引脚和方向引脚
#define MOTOR1_A   D12                      // 定义1电机正反转引脚
#define MOTOR1_B   TIM4_PWM_MAP1_CH2_D13         // 定义1电机PWM引脚

#define MOTOR2_A   D15                      // 定义2电机正反转引脚
#define MOTOR2_B   TIM4_PWM_MAP1_CH3_D14         // 定义2电机PWM引脚

void motor_init(void)
{
    //建议电磁组电机频率选用13K-17K
    //最大占空比值PWM_DUTY_MAX 可以在zf_driver_pwm.h文件中修改 默认为10000

    gpio_init(MOTOR1_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR1_B, 17000, 0);
    gpio_init(MOTOR2_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR2_B, 17000, 0);
}

void motor_pid(int16 encoder, int16 expect_speed)
{

}


void motor_control(int32 duty_1, int32 duty_2)
{
    //对占空比限幅
    if(duty_1>PWM_DUTY_MAX)         duty_1 = PWM_DUTY_MAX;
    else if(duty_1<-PWM_DUTY_MAX)   duty_1 = -PWM_DUTY_MAX;

    if(duty_2>PWM_DUTY_MAX)         duty_2 = PWM_DUTY_MAX;
    else if(duty_2<-PWM_DUTY_MAX)   duty_2 = -PWM_DUTY_MAX;


    if(0<=duty_1)
    {
        gpio_set_level(MOTOR1_A, 0);
        pwm_set_duty(MOTOR1_B, duty_1);
    }
    else
    {
        gpio_set_level(MOTOR1_A, 1);
        pwm_set_duty(MOTOR1_B, -duty_1);
    }

    if(0<=duty_2)
    {
        gpio_set_level(MOTOR2_A, 0);
        pwm_set_duty(MOTOR2_B, duty_2);
    }
    else
    {
        gpio_set_level(MOTOR2_A, 1);
        pwm_set_duty(MOTOR2_B, -duty_2);
    }


}
