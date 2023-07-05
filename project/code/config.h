/*
 * config.h
 *
 *  Created on: 2023年6月28日
 *      Author: lid
 */

#ifndef CONFIG_H_
#define CONFIG_H_

// 电机和舵机调试不能同时开启，按键冲突
#define MOTOR_DEBUG_STATUS false    // 电机调试pid模式开关，0关1开，打开调试模式必须接入蓝牙模块
#define SERVO_DEBUG_STATUS true    // 舵机调试pid模式开关，0关1开，打开调试模式必须接入蓝牙模块
#define CAR_TYPE 1              // 车模类别，小车0，大车1
#define ANGLE_90_MODE false      // 舵机始终中值

#endif /* CONFIG_H_ */
