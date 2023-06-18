/*
 * hall_stopline.h
 *
 *  Created on: 2023Äê5ÔÂ26ÈÕ
 *      Author: Lenovo
 */

#ifndef HALL_STOPLINE_H_
#define HALL_STOPLINE_H_

#include <zf_common_headfile.h>

#define HALL_PIN                (B2)

void hallInit();
bool judgeStopline();

#endif /* HALL_STOPLINE_H_ */
