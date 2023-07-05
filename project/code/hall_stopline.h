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

#define HALL_PIN                (B2)

void hallInit();
bool judgeStopline();
void out_garage();
void In_Garage();

#endif /* HALL_STOPLINE_H_ */
