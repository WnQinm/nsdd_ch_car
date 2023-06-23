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

//初始化电机PWM引脚和方向引脚
#define MOTOR1_A   D12                      // 定义1电机正反转引脚
#define MOTOR1_B   TIM4_PWM_MAP1_CH2_D13         // 定义1电机PWM引脚

#define MOTOR2_A   D15                      // 定义2电机正反转引脚
#define MOTOR2_B   TIM4_PWM_MAP1_CH3_D14         // 定义2电机PWM引脚

void motor_init(void);
void motor_control(int32 duty_1, int32 duty_2);
void motor_pid(int16 encoder, int16 expect_speed);

extern float v1, v2;
extern float pulseCount_1, pulseCount_2;
void getVelocity(float interval);
int16 MotorPI (int16 Encoder,int16 Target);

#endif /* MOTOR_H_ */
