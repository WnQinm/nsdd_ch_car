/*
 * motor.c
 *
 *  Created on: 2023年4月18日
 *      Author: Lenovo
 */
#include "motor.h"

float v1=0; //左轮速度
float v2=0; //右轮速度
float pulseCount_1, pulseCount_2;

void motor_init(void)
{
    //建议电磁组电机频率选用13K-17K
    //最大占空比值PWM_DUTY_MAX 可以在zf_driver_pwm.h文件中修改 默认为10000

    gpio_init(MOTOR1_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR1_B, 17000, 0);
    gpio_init(MOTOR2_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR2_B, 17000, 0);
}

void getVelocity(float interval){
    pulseCount_1 = encoder_get_count(ENCODER_1)/5;//(float)abs();// 获取编码器计数
    pulseCount_2 = -encoder_get_count(ENCODER_2)/5;//(float)abs();// 获取编码器计数

    encoder_clear_count(ENCODER_1);// 清空编码器计数
    encoder_clear_count(ENCODER_2);// 清空编码器计数

    v1=(pulseCount_1*1.0f/1024 * 9.42)/interval *10;
    v2=(pulseCount_2*1.0f/1024 * 9.42)/interval *10;
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

int16 Motor_Bias, Motor_Last_Bias, IntegrationM; // 电机所用参数
int16 M_P=  -1;//-50
int16 M_D=0 ;
int16 M_I= 0;//-6 -1
int16 velocity= -71;//电机pid参数  -10 - -40
int16 velocity1= -55;//电机pid参数  -10 - -40
int16 velocity2= -67;//电机pid参数  -10 - -40

int16 MotorPI (int16 Encoder,int16 Target)
{
    static int16 PwmMotor=0;

    Motor_Bias = Encoder + Target;                   // 计算偏差
    IntegrationM+=Motor_Bias;
    if(IntegrationM<-300)      IntegrationM=-300;   //限幅
    else if(IntegrationM>300)  IntegrationM= 300;   //限幅
    PwmMotor = M_D * (Motor_Bias - Motor_Last_Bias) + M_P * Motor_Bias +M_I*IntegrationM; // 增量式PI控制器

    if(PwmMotor > 2000) PwmMotor = 2000;               // 限幅,等待更细的调试
    else if(PwmMotor < -2000)PwmMotor = -2000;         // 限幅

    Motor_Last_Bias = Motor_Bias;              // 保存上一次偏差

    return PwmMotor; // 增量输出
}
