/*
 * config.h
 *
 *  Created on: 2023年6月28日
 *      Author: lid
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "zf_common_headfile.h"

/// 调试用参数，电机和舵机调试不能同时开启，按键冲突
#define MOTOR_DEBUG_STATUS false    // 电机调试pid模式开关，0关1开，打开调试模式必须接入蓝牙模块
#define SERVO_DEBUG_STATUS false    // 舵机调试pid模式开关，0关1开，打开调试模式必须接入蓝牙模块
#define PERSPECTIVE_DEBUG_STATUS false // 逆透视参数标定模式开关
#define ANGLE_90_MODE false      // 舵机始终中值
#define ENABLE_SLOPE true        // 启用坡道处理



/// 车型参数
#define CAR_TYPE 1               // 车模类别，小车0，大车1
#define ENABLE_TOF false          // 启用tof避障
#define ENABLE_HALL false         // 启用霍尔入库检测
// 车模速度的稳定速度
#if CAR_TYPE
    #define NORMAL_PULSE 23
    #define SLOW_PULSE 15
    #define FAST_PULSE 50
#else
    #define NORMAL_PULSE 17
    #define SLOW_PULSE 15
    #define FAST_PULSE 50
#endif
// 电磁舵机pid参数
extern float elec_Kp;
extern float elec_Kd;



/// 策略类参数
#define OBSTACLE_LEFTorRIGHT 1      // 避障方向，0右1左
#define OBSTACLE_AT_STRAIGHT true   // 障碍物的位置，true是放在直道，false是放在弯道
#define LAUNCH_FORWARD 0            // 发车方向，0右1左
#define ENABLE_LOOP true            // 开启环岛识别功能，关闭可以避开环岛保证完赛
#define SLOPE_BEFORE_OBSTACLE true     // 坡道是否在障碍物前



/// 开环参数
// 出入库开环参数
extern uint16 In_Garage_with_Hall_pulse[4];
extern uint16 stopline_pulse[3];
extern uint16 outgarage_pulse[3];
extern uint16 stopAtStopline_pulse;
// 避障开环参数
#if OBSTACLE_AT_STRAIGHT
    extern long obstacle_pulse[7];
#else
    long obstacle_pulse[3];
#endif


#endif /* CONFIG_H_ */
