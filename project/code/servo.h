/*
 * servo.h
 *
 *  Created on: 2023年4月18日
 *      Author: Lenovo
 */

#ifndef SERVO_H_
#define SERVO_H_

#include "zf_common_headfile.h"
#include "Read_ADC.h"
#include "judgement.h"
#include "imgproc.h"
#include "mathh.h"
#include "config.h"

#define SERVO_MOTOR_FREQ 50

// 舵机角度->占空比
#if CAR_TYPE
#define SERVO_MOTOR_DUTY(x)    ((float)PWM_DUTY_MAX / (1000.0 / (float)SERVO_MOTOR_FREQ) * (0.63 + (float)(x) / 90.0))
#else
#define SERVO_MOTOR_DUTY(x)    ((float)PWM_DUTY_MAX / (1000.0 / (float)SERVO_MOTOR_FREQ) * (0.45 + (float)(x) / 90.0))
#endif

// 定义车模寻迹舵机引脚
#define SERVO_PIN           TIM2_PWM_MAP3_CH1_A15
// 舵机中值
#define SERVO_CENTER        90


#define default_aimline ROW-1-10//默认控制行


void servo_init();
void servo_control(RaceStatus status);
void elec_pid(float current_err);
void camera_pid(float current_err);

extern float Angle;
extern float current_err_common, current_err_circle;
extern uint8 aimLine;

#if SERVO_DEBUG_STATUS
extern float elec_Kp, elec_Kd;
#endif

#endif /* SERVO_H_ */
