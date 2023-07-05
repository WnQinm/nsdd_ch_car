/*
 * gyro.h
 *
 *  Created on: 2023Äê5ÔÂ26ÈÕ
 *      Author: Lenovo
 */

#ifndef GYRO_H_
#define GYRO_H_

#include "zf_common_headfile.h"

extern int16 gyro_x, gyro_y, gyro_z;
void gyro_init();
void gyro_data();
#endif /* GYRO_H_ */
