//
// Created by 14838 on 2023/7/16.
//
#include "config.h"

// 电磁舵机pid参数
#if CAR_TYPE
    float elec_Kp = 17;//10.5;//15;
    float elec_Kd = 0;//0.2;//25;//23.5;//23;//22;//20.5;//19.5;
#else
    float elec_Kp = 10;
    float elec_Kd = 20;
#endif

// 应试教育(坡道 避障 左环岛 右环岛) 没有就0
    uint8 yuansu_cnt=1;
    uint8 yuansu_cnt_enum[10] = {1, 2, 0, 0};

/// 开环参数
// 出入库开环参数
#if !LAUNCH_FORWARD  //向右发车
    uint16 In_Garage_with_Hall_pulse[4]={1500,300,800,2500};  //小车左后倒车入库
    uint16 stopline_pulse[3]={2700,2500,2000};                 //小车右转正向入库
    uint16 outgarage_pulse[3]={1100,2700,2400};                //大车右转出库
#else  //向左发车
    uint16 In_Garage_with_Hall_pulse[4]={1000,300,800,200};  //小车右后倒车入库
    uint16 stopline_pulse[3]={2700,2100,3000};                     //小车左转正向入库
    uint16 outgarage_pulse[3]={1100,1800,2200};                    //大车左转出库
#endif
uint16 stopAtStopline_pulse=1300;  //大车人行道靠边停车
// 避障开环参数
#if OBSTACLE_AT_STRAIGHT
    #if !CAR_TYPE&&!OBSTACLE_LEFTorRIGHT
    long obstacle_pulse[7]={1200,1700,600,1600,600,1200,1500};  //直道小车向右避障
    #elif !CAR_TYPE&&OBSTACLE_LEFTorRIGHT
    long obstacle_pulse[7]={800,2300,800,1500,800,2000,900};  //直道小车向左避障
    #elif CAR_TYPE && !OBSTACLE_LEFTorRIGHT
    long obstacle_pulse[7]={1500,1700,1000,400,1000,1300,1500};  //直道大车向右避障
    #elif CAR_TYPE && OBSTACLE_LEFTorRIGHT
    long obstacle_pulse[7]={1000,1700,1500,400,1500,1300,1000};  //直道大车向左避障
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

// 环岛开环参数
#if CAR_TYPE// 入环直行距离  进环打角距离  出环打角距离  出环后正常巡线距离
    uint16 Lcircle_param[4] = {2700,1700,2000,2000};
    uint16 Rcircle_param[4] = {3500,1600,2000,2000};
#else// 入环直行距离  进环打角距离  出环打角距离  出环后开环舵机90度直行距离
    uint16 Lcircle_param[4] = {2500,1800,3000,1000};
    uint16 Rcircle_param[4] = {3200,1300,3000,1000};
#endif
