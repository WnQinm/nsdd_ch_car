/*
 * pit.h
 *
 *  Created on: 2023年4月8日
 *      Author: 清梦
 */

#ifndef PIT_H_
#define PIT_H_

#include "zf_common_headfile.h"
#define IMG_PIT_CH                  (TIM6_PIT )                                      // 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用
#define IMG_PIT_PRIORITY            (TIM6_IRQn)                                      // 对应周期中断的中断编号

void IMG_pit_init();

#endif /* PIT_H_ */
