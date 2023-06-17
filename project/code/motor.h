/*
 * motor.h
 *
 *  Created on: 2023Äê4ÔÂ18ÈÕ
 *      Author: Lenovo
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "zf_common_headfile.h"

void motor_init(void);
void motor_control(int32 duty_1, int32 duty_2);
void motor_pid(int16 encoder, int16 expect_speed);

#endif /* MOTOR_H_ */
