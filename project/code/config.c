//
// Created by 14838 on 2023/7/16.
//
#include "config.h"

// ��Ŷ��pid����
#if CAR_TYPE
    float elec_Kp = 17;//10.5;//15;
    float elec_Kd = 0;//0.2;//25;//23.5;//23;//22;//20.5;//19.5;
#else
    float elec_Kp = 10;
    float elec_Kd = 20;
#endif

// Ӧ�Խ���(�µ� ���� �󻷵� �һ���) û�о�0
    uint8 yuansu_cnt=1;
    uint8 yuansu_cnt_enum[10] = {1, 2, 0, 0};

/// ��������
// ����⿪������
#if !LAUNCH_FORWARD  //���ҷ���
    uint16 In_Garage_with_Hall_pulse[4]={1500,300,800,2500};  //С����󵹳����
    uint16 stopline_pulse[3]={2700,2500,2000};                 //С����ת�������
    uint16 outgarage_pulse[3]={1100,2700,2400};                //����ת����
#else  //���󷢳�
    uint16 In_Garage_with_Hall_pulse[4]={1000,300,800,200};  //С���Һ󵹳����
    uint16 stopline_pulse[3]={2700,2100,3000};                     //С����ת�������
    uint16 outgarage_pulse[3]={1100,1800,2200};                    //����ת����
#endif
uint16 stopAtStopline_pulse=1300;  //�����е�����ͣ��
// ���Ͽ�������
#if OBSTACLE_AT_STRAIGHT
    #if !CAR_TYPE&&!OBSTACLE_LEFTorRIGHT
    long obstacle_pulse[7]={1200,1700,600,1600,600,1200,1500};  //ֱ��С�����ұ���
    #elif !CAR_TYPE&&OBSTACLE_LEFTorRIGHT
    long obstacle_pulse[7]={800,2300,800,1500,800,2000,900};  //ֱ��С���������
    #elif CAR_TYPE && !OBSTACLE_LEFTorRIGHT
    long obstacle_pulse[7]={1500,1700,1000,400,1000,1300,1500};  //ֱ�������ұ���
    #elif CAR_TYPE && OBSTACLE_LEFTorRIGHT
    long obstacle_pulse[7]={1000,1700,1500,400,1500,1300,1000};  //ֱ�����������
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

// ������������
#if CAR_TYPE// �뻷ֱ�о���  ������Ǿ���  ������Ǿ���  ����������Ѳ�߾���
    uint16 Lcircle_param[4] = {2700,1700,2000,2000};
    uint16 Rcircle_param[4] = {3500,1600,2000,2000};
#else// �뻷ֱ�о���  ������Ǿ���  ������Ǿ���  �����󿪻����90��ֱ�о���
    uint16 Lcircle_param[4] = {2500,1800,3000,1000};
    uint16 Rcircle_param[4] = {3200,1300,3000,1000};
#endif
