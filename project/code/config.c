//
// Created by 14838 on 2023/7/16.
//
#include "config.h"

// 电磁舵机pid参数
#if CAR_TYPE
    float elec_Kp = 11.5;//10.5;//15;
    float elec_Kd = 0;//0.2;//25;//23.5;//23;//22;//20.5;//19.5;
#else
//小车速度17时：
//    float elec_Kp = 9;//15;
//    float elec_Kd = 7;//25;//23.5;//23;//22;//20.5;//19.5;
//小车速度25时：
    float elec_Kp = 10;
    float elec_Kd = 37;
#endif

/// 开环参数
// 出入库开环参数
#if !LAUNCH_FORWARD  //向右发车
    uint16 In_Garage_with_Hall_pulse[4]={1000,300,2500,1200};  //小车左后倒车入库
    uint16 stopline_pulse[3]={2700,2500,2000};                 //小车右转正向入库
    uint16 outgarage_pulse[3]={1100,2700,2400};                //大车右转出库
#else  //向左发车
    uint16 In_Garage_with_Hall_pulse[4]={1000,300,1200,1200};  //小车右后倒车入库
    uint16 stopline_pulse[3]={2700,2100,3000};                     //小车左转正向入库
    uint16 outgarage_pulse[3]={1100,1800,2200};                    //大车左转出库
#endif
uint16 stopAtStopline_pulse=1000;  //大车人行道靠边停车
// 避障开环参数
#if OBSTACLE_AT_STRAIGHT
    #if !CAR_TYPE&&!OBSTACLE_LEFTorRIGHT
        long obstacle_pulse[7]={800,2000,600,1600,600,2000,800};  //直道小车向右避障
    #elif !CAR_TYPE&&OBSTACLE_LEFTorRIGHT
        long obstacle_pulse[7]={600,2000,800,1600,800,1800,600};  //直道小车向左避障
    #elif CAR_TYPE && !OBSTACLE_LEFTorRIGHT
        long obstacle_pulse[7]={1500,1700,1000,800,1000,1600,1500};  //直道大车向右避障
    #elif CAR_TYPE && OBSTACLE_LEFTorRIGHT
        long obstacle_pulse[7]={1000,1700,1500,800,1500,1600,1000};  //直道大车向左避障
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
