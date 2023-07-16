/*
 * hall_stopline.h
 *
 *  Created on: 2023Äê5ÔÂ26ÈÕ
 *      Author: Lenovo
 */

#ifndef HALL_STOPLINE_H_
#define HALL_STOPLINE_H_

#include <zf_common_headfile.h>
#include "servo.h"
#include "motor.h"
#include "imgproc.h"
#include "judgement.h"
#include "config.h"

#define HALL_PIN                (B2)

extern uint16 stopline_delay[2];
extern uint16 outgarage_delay[4];

void hallInit();
bool judgeStopline();
void out_garage();
void In_Garage();
void wait_for_charge();
void Stop_At_Stopline();
void In_Garage_with_Hall();
void wait_for_launch();

#endif /* HALL_STOPLINE_H_ */
