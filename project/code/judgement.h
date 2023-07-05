/*
 * judgement.h
 *
 *  Created on: 2023年6月18日
 *      Author: 清梦
 */

#ifndef JUDGEMENT_H_
#define JUDGEMENT_H_

#define CROSS_DELAY_TIME 3000

// 道路状态枚举
// 或许可以吧循右线放这里
typedef enum
{
    Status_Stop   = 0,
    Status_Common = 1,
    Status_Circle = 2,
    Status_LCamera = 3,
    Status_RCamera = 4
}RaceStatus;

#include "zf_common_headfile.h"
#include "Read_ADC.h"
#include "imgproc.h"
#include "servo.h"

void judgement();

extern RaceStatus CURRENT_STATUS;

extern uint8 out_garage_flag, in_garage_flag;
extern uint8 left_circle_flag, right_circle_flag, cross_flag, obstacle_flag;
extern uint8 circle_threshold, cross_threshold;
extern uint8 circle_status;
extern uint16 cross_cnt;
extern bool out_flag;

#endif /* JUDGEMENT_H_ */
