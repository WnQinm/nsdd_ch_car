/*
 * config.h
 *
 *  Created on: 2023年6月28日
 *      Author: lid
 */

#ifndef CONFIG_H_
#define CONFIG_H_

/// 调试用参数，电机和舵机调试不能同时开启，按键冲突
#define MOTOR_DEBUG_STATUS false    // 电机调试pid模式开关，0关1开，打开调试模式必须接入蓝牙模块
#define SERVO_DEBUG_STATUS false    // 舵机调试pid模式开关，0关1开，打开调试模式必须接入蓝牙模块
#define PERSPECTIVE_DEBUG_STATUS false // 逆透视参数标定模式开关
#define ANGLE_90_MODE false      // 舵机始终中值



/// 车型参数
#define CAR_TYPE 0               // 车模类别，小车0，大车1
#define ENABLE_TOF true          // 启用tof避障
#define ENABLE_HALL true         // 启用霍尔入库检测
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
#if CAR_TYPE
    float elec_Kp = 11.5;//15;
    float elec_Kd = 5;//25;//23.5;//23;//22;//20.5;//19.5;
#else
    float elec_Kp = 9;//15;
    float elec_Kd = 7;//25;//23.5;//23;//22;//20.5;//19.5;
#endif



/// 策略类参数
#define OBSTACLE_LEFTorRIGHT 0      // 避障方向，0右1左
#define OBSTACLE_AT_STRAIGHT true   // 障碍物的位置，true是放在直道，false是放在弯道
#define LAUNCH_FORWARD 1            // 发车方向，0右1左
#define ENABLE_LOOP true            // 开启环岛识别功能，关闭可以避开环岛保证完赛



/// 开环参数
// 出入库开环参数
#if !LAUNCH_FORWARD  //大车出库向右
    uint16 In_Garage_with_Hall_pulse[4]={1000,300,2500,1200};  //小车左后倒车入库
    uint16 stopline_pulse[3]={2700,4200,3000};                 //小车右转正向入库
    uint16 outgarage_pulse[3]={1100,2600,1500};                //大车右转出库
#else  //大车出库向左
    uint16 In_Garage_with_Hall_pulse[4]={1000,300,1200,1200};  //小车右后倒车入库
    uint16 stopline_pulse[3]={2700,2100,3000};                     //小车左转正向入库
    uint16 outgarage_pulse[3]={1100,1600,1500};                    //大车左转出库
#endif
    uint16 stopAtStopline_pulse=2000;  //大车人行道靠边停车
// 避障开环参数
#if OBSTACLE_AT_STRAIGHT
    #if !CAR_TYPE&&!OBSTACLE_LEFTorRIGHT
        long obstacle_pulse[7]={1300,1700,800,1000,800,1700,1100};  //直道小车向右避障
    #elif !CAR_TYPE&&OBSTACLE_LEFTorRIGHT
        long obstacle_pulse[7]={800,1700,1300,1000,1100,1700,800};  //直道小车向左避障
    #elif CAR_TYPE && !OBSTACLE_LEFTorRIGHT
        long obstacle_pulse[7]={1500,1000,900,500,1200,1700,1500};  //直道大车向右避障
    #elif CAR_TYPE && OBSTACLE_LEFTorRIGHT
        long obstacle_pulse[7]={900,1000,1500,500,1500,1700,1200};  //直道大车向左避障
    #endif
#else
    #if !CAR_TYPE&&!OBSTACLE_LEFTorRIGHT
        long obstacle_pulse[3]={1500,6000,1500};  //弯道小车向右避障
    #elif !CAR_TYPE&&OBSTACLE_LEFTorRIGHT
        long obstacle_pulse[3]={1000,6000,1000};  //弯道小车向左避障
    #elif CAR_TYPE && !OBSTACLE_LEFTorRIGHT
        long obstacle_pulse[3]={1500,10000,1500};  //弯道大车向右避障
    #elif CAR_TYPE && OBSTACLE_LEFTorRIGHT
        long obstacle_pulse[3]={1500,10000,1500};  //弯道大车向右避障
    #endif
#endif

#endif /* CONFIG_H_ */
