/*
 * servo.c
 *
 *  Created on: 2023年4月18日
 *      Author: Lenovo
 */
#include "Read_ADC.h"
#include "servo.h"

float err;
//float TURN_A=1,TURN_B=99;
float Turn_KP=15,Turn_KD=0.9,exp_turn_KP=0;
float Angle=0;


void servo_init()
{
    pwm_init(SERVO_PIN, SERVO_MOTOR_FREQ, SERVO_CENTER);
}

float servo_chabiji(float ad_lm,float ad_rm)
{
    float cha2=0,ji2=0,output;
//    cha1=ad_l-ad_r;
    cha2=ad_lm-ad_rm;
//    ji1=ad_l+ad_r;
    ji2=ad_lm*ad_rm;
    output=500*cha2/ji2;


    return output;

}
//void servo_control()
//{
//    servo_con();
//    if(Angle<75) Angle=75;
//    if(Angle>105) Angle=105;
//    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(Angle));
//
//}

void servo_con()
{
    static float Last_Bias=0,exp_turn,Bias=0;

    Read_ADC();
    err=servo_chabiji(adc_RM,adc_LM);

    Bias=err*Turn_KP;
       exp_turn=Turn_KP*err+Turn_KD*(Bias-Last_Bias);
       Last_Bias=Bias;

       Angle=90+Bias;
       //期望角速度的计算（通过差比和的结果进行pid处理得到）I

          if(Angle<75) Angle=75;
          if(Angle>105) Angle=105;
          pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(Angle));

}


