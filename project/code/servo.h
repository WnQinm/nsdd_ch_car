/*
 * servo.h
 *
 *  Created on: 2023��4��18��
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

// ����Ƕ�->ռ�ձ�
#if CAR_TYPE
#define SERVO_MOTOR_DUTY(x)    ((float)PWM_DUTY_MAX / (1000.0 / (float)SERVO_MOTOR_FREQ) * (0.63 + (float)(x) / 90.0))
#else
#define SERVO_MOTOR_DUTY(x)    ((float)PWM_DUTY_MAX / (1000.0 / (float)SERVO_MOTOR_FREQ) * (0.45 + (float)(x) / 90.0))
#endif

// ���峵ģѰ���������
#define SERVO_PIN           TIM2_PWM_MAP3_CH1_A15
// �����ֵ
#define SERVO_CENTER        90


#define default_aimline ROW-1-10//Ĭ�Ͽ�����


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
