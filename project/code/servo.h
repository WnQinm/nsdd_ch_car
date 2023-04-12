/*
 * servo.h
 *
 *  Created on: 2023年4月8日
 *      Author: 清梦
 */

#ifndef SERVO_H_
#define SERVO_H_

#include "zf_common_headfile.h"
#include "imgproc.h"
#include "gyro.h"

#define SERVO_MOTOR_FREQ 50

// 舵机角度->占空比
#define SERVO_MOTOR_DUTY(x)    (int)((float)PWM_DUTY_MAX / (1000.0 / (float)SERVO_MOTOR_FREQ) * (0.5 + (float)(x) / 90.0))

// 定义车模寻迹舵机引脚
#define SERVO_PIN           TIM2_PWM_MAP3_CH1_A15

// 舵机中值，需要根据实际调SERVO_MOTOR_DUTY中的参数
#define SERVO_CENTER        90

extern float Angle;

void servo_init(void);
void servo_control();
void AngleControl(void);
void Angle_Get(void);
void Angle_out(void);

#endif /* SERVO_H_ */
