/*
 * pit.h
 *
 *  Created on: 2023年4月8日
 *      Author: 清梦
 */

#ifndef PIT_H_
#define PIT_H_

#include "zf_common_headfile.h"

#define MAIN_PIT_CH                  (TIM6_PIT)                                      // 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用
#define MAIN_PIT_PRIORITY            (TIM6_IRQn)                                      // 对应周期中断的中断编号

#define IMG_PIT_CH                  (TIM7_PIT)
#define IMG_PIT_PRIORITY            (TIM7_IRQn)

#define GET_VELOCITY_INTERVAL            50
#define ENCODER_PIT_CH                  (TIM3_PIT )                                      // 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用
#define ENCODER_PIT_PRIORITY            (TIM3_IRQn)                                      // 对应周期中断的中断编号

#define ENCODER_1                   (TIM1_ENCOEDER)
#define ENCODER_1_A                 (TIM1_ENCOEDER_MAP3_CH1_E9)
#define ENCODER_1_B                 (TIM1_ENCOEDER_MAP3_CH2_E11)

#define ENCODER_2                   (TIM9_ENCOEDER)
#define ENCODER_2_A                 (TIM9_ENCOEDER_MAP3_CH1_D9)
#define ENCODER_2_B                 (TIM9_ENCOEDER_MAP3_CH2_D11)

#define ADC_BATTERY_PIT_CH             (TIM5_PIT)
#define ADC_BATTERY_PIT_PRIORITY       (TIM5_IRQn)
#define GET_BATTERY_VOLTAGE_INTERVAL      5000

void ALL_pit_init();

#endif /* PIT_H_ */
