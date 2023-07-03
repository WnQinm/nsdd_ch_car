/*
 * motor.h
 *
 *  Created on: 2023年4月18日
 *      Author: Lenovo
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "zf_common_headfile.h"
#include "pit.h"
#include "config.h"
#include "mathh.h"

//初始化电机PWM引脚和方向引脚
#define MOTOR1_A   D12                      // 定义1电机正反转引脚
#define MOTOR1_B   TIM4_PWM_MAP1_CH2_D13         // 定义1电机PWM引脚

#define MOTOR2_A   D15                      // 定义2电机正反转引脚
#define MOTOR2_B   TIM4_PWM_MAP1_CH3_D14         // 定义2电机PWM引脚

#define NORMAL_DUTY 800

/*pid*/
typedef struct
{
    float target_val;     //目标值
    float actual_val;     //实际值
    float err;            //定义当前偏差值
    float err_next;       //定义下一个偏差值
    float err_last;       //定义最后一个偏差值
    float Kp, Ki, Kd;     //定义比例、积分、微分系数
}_pid;

void motor_init(void);
void motor_control(int32 duty_1, int32 duty_2);
void getPulseCount();

extern _pid Lmotor_pid, Rmotor_pid;
extern int16 pulseCount_1, pulseCount_2;
extern int16 motorPWML, motorPWMR;
void PID_param_init();
void set_pid_target(float temp_val);
float get_pid_target(void);
void set_p_i_d(float p, float i, float d);
float PID_realize(uint8 LR, float temp_val);

#endif /* MOTOR_H_ */
