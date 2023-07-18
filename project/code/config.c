//
// Created by 14838 on 2023/7/16.
//
#include "config.h"

// ��Ŷ��pid����
#if CAR_TYPE
    float elec_Kp = 10;//11.5;//15;
    float elec_Kd = 30;//25;//23.5;//23;//22;//20.5;//19.5;
#else
    float elec_Kp = 9;//15;
    float elec_Kd = 7;//25;//23.5;//23;//22;//20.5;//19.5;
#endif

/// ��������
// ����⿪������
#if !LAUNCH_FORWARD  //���ҷ���
    uint16 In_Garage_with_Hall_pulse[4]={1000,300,2500,1200};  //С����󵹳����
    uint16 stopline_pulse[3]={2700,2500,2000};                 //С����ת�������
    uint16 outgarage_pulse[3]={1100,2700,2400};                //����ת����
#else  //���󷢳�
    uint16 In_Garage_with_Hall_pulse[4]={1000,300,1200,1200};  //С���Һ󵹳����
    uint16 stopline_pulse[3]={2700,2100,3000};                     //С����ת�������
    uint16 outgarage_pulse[3]={1100,1800,2200};                    //����ת����
#endif
uint16 stopAtStopline_pulse=1000;  //�����е�����ͣ��
// ���Ͽ�������
#if OBSTACLE_AT_STRAIGHT
    #if !CAR_TYPE&&!OBSTACLE_LEFTorRIGHT
        long obstacle_pulse[7]={1100,2000,800,1500,600,2000,800};  //ֱ��С�����ұ���
    #elif !CAR_TYPE&&OBSTACLE_LEFTorRIGHT
        long obstacle_pulse[7]={800,2000,1100,1500,800,2000,600};  //ֱ��С���������
    #elif CAR_TYPE && !OBSTACLE_LEFTorRIGHT
        long obstacle_pulse[7]={1500,1300,900,800,1200,1300,1500};  //ֱ�������ұ���
    #elif CAR_TYPE && OBSTACLE_LEFTorRIGHT
        long obstacle_pulse[7]={1600,1500,2000,500,1500,1300,1000};  //ֱ�����������
    #endif
#else
#if !CAR_TYPE&&!OBSTACLE_LEFTorRIGHT
long obstacle_pulse[3]={1500,6000,1500};  //���С�����ұ���
#elif !CAR_TYPE&&OBSTACLE_LEFTorRIGHT
long obstacle_pulse[3]={1000,6000,1000};  //���С���������
    #elif CAR_TYPE && !OBSTACLE_LEFTorRIGHT
        long obstacle_pulse[3]={1500,10000,1500};  //��������ұ���
    #elif CAR_TYPE && OBSTACLE_LEFTorRIGHT
        long obstacle_pulse[3]={1500,10000,1500};  //��������ұ���
#endif
#endif