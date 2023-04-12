/*
 * servo.c
 *
 *  Created on: 2023��4��8��
 *      Author: ����
 */

#include "servo.h"

extern uint16 gyro[3];

uint8   weightbase[30] = {                        //0Ϊͼ�����
         1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
         5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
        10,10,10,10,10,10,10,10,10,10,};    //����    //ע��б�ʱ仯���������,Ҫƽ��
int weightSum, lineSum, midline_f, midline_ff, midline_fff;


int16 ServoDuty;

int16 OUT_PWM=0;       //ת�����PWM                                                                       90                 95       93       80
float Angle_kp=31.3;    //ת��PID���⻷��32.4     31.5    32.1     31.9   31.7    31.3    32.2    30.5    29.5    29.5        30       31.2      31.3
float value = 3;     //ѭ��P�ı仯��С                                              5       4     3.5       3     2.4         2.3        3         3
float Angle_ki=0;
float Angle_kd=0;
float Gro3_p = 0.3;   //�����ǵĳ˻�ϵ��//0.19     0.75    0.75    0.585 0.545    0.445   0.415     0.4    0.31    0.15       0.21      0.35       0.3
uint8 Number=0;       //���⻷������
float Angle=0;          //Ŀ��Ƕ�
float Angle_last=0;
int ServoDuty_Last=0;
int Servoduty=0;        //�⻷ƫ��

// ��ʼ�����
void servo_init(void)
{
    pwm_init(SERVO_PIN, SERVO_MOTOR_FREQ, SERVO_CENTER);
}

// �������
void servo_control()
{
    ips200_show_string(0, 220, "Angle:");
    ips200_show_float(70, 220, Angle, 3, 3);
    if(Angle<-15) Angle=-15;
    if(Angle>15) Angle=15;
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(-Angle+90));

}

// ��С���˷����Իع�
int regression(int startline,int endline)
{

  int i=0,SumX=0,SumY=0,SumLines = 0;
  float SumUp=0,SumDown=0,avrX=0,avrY=0,B;
  SumLines=endline-startline;   // startline Ϊ��ʼ�У� //endline ������ //SumLines

  for(i=startline;i<endline;i++)
  {
    SumX+=i;
    SumY+=centerline[i];
  }
  avrX=SumX/SumLines;     //X��ƽ��ֵ
  avrY=SumY/SumLines;     //Y��ƽ��ֵ
  SumUp=0;
  SumDown=0;
  for(i=startline;i<endline;i++)
  {
    SumUp+=(centerline[i]-avrY)*(i-avrX);
    SumDown+=(i-avrX)*(i-avrX);
  }
  if(SumDown==0)
    B=0;
  else
    B=(int)(SumUp/SumDown);
//    A=(SumY-B*SumX)/SumLines;  //�ؾ�
    return B;  //����б��
}

//*********������Ŀǰ�ջ������ܵĴ���*************
#define  Kp 0.7
#define  Kd 0.3

// ��pid
void servo_base_pid()
{
    int current_err = Angle;
    static int previous_err = 0;
    static int sum_err = 0;

    sum_err += current_err;

    Angle = Kp * current_err + Kd * (current_err - previous_err);

    previous_err = current_err;
}

// ����ƫ�ͨ��ĳ���м��㣬�����ٱ��ʱ���޷�����
#define aimLine MT9V03X_H-20
void AngleErr()
{
//    for(int i = 0; i < 30; i++) {
//        weightSum += weightbase[i];
//        lineSum += weightbase[i] * centerline[MT9V03X_H-30+i];
//    }   //��������ɨ
//
//    Angle = (float)lineSum / weightSum - 70;
//    midline_fff = midline_ff;
//    midline_ff  = midline_f;
//    midline_f = Angle;
//    Angle = midline_fff * 0.50f + midline_ff * 0.30f + midline_f * 0.20f;
//
//    int slope = regression(MT9V03X_H - 30, MT9V03X_H);
//    Angle = Angle * 0.80f + slope * 0.20f;

// �ͼ�ת��
    Angle = ((  5 * centerline[aimLine] +
                3 * centerline[aimLine + 1] +
                2 * centerline[aimLine + 2]) / (10.000f)) - 70;
//    midline_fff = midline_ff;
//    midline_ff  = midline_f;
//    midline_f = Angle;
//    Angle = midline_fff * 0.50f + midline_ff * 0.30f + midline_f * 0.20f;

//    Angle = Angle * 0.80f - regression(MT9V03X_H - 10, MT9V03X_H) * 0.20f;
    servo_base_pid();
}

//*********������Ŀǰ�ջ������ܵĴ���*************

//*********������cv�Ĵ���pid�������⻷*************
// ת�򻷿��ƣ��⻷15ms�ڻ�5ms
void AngleControl(void)
{
    AngleErr();
//    Servoduty = (int)ServoDuty; //����ƫ��
//    //�����ڻ���ִ��һ���⻷
//    if(Number==3)
//    {
//        Number=0;
//        Angle_Get();
//    }
//    Number++;
//    Angle=Angle_last+(Angle-Angle_last)*Number/3;
//    Angle_out();

    servo_control();
}

void Angle_Get(void)
{
    static uint16 i;
    if(Servoduty >= -10 && Servoduty <= 10 && OpeningRoad_Flag ==0)
    {
        Angle=(Angle_kp - 2 * value)*Servoduty+Angle_kd*(Servoduty-ServoDuty_Last);

    }
    else if(((Servoduty > 10 && Servoduty <= 35) || (Servoduty >= -35 && Servoduty < -10)) && OpeningRoad_Flag == 0)
    {
        Angle=(Angle_kp - value)*Servoduty+Angle_kd*(Servoduty-ServoDuty_Last);
    }
    else if(OpeningRoad_Flag == 1)      //��·ʱ��ת��PҪ��
    {
        if(++i >= 300)
        {
            i = 300;
            Angle=Angle_kp *Servoduty+3*(Servoduty-ServoDuty_Last);
        }
        else
        {
            Angle=(Angle_kp + 5)*Servoduty+5*(Servoduty-ServoDuty_Last);
        }
    }
    else
    {
        Angle=Angle_kp*Servoduty+Angle_kd*(Servoduty-ServoDuty_Last);
    }

    Angle=0; //������仰����ת��ֻ���ڻ����⻷���Ϊ0
    ServoDuty_Last=Servoduty;
    Angle_last=Angle;
}

void Angle_out(void)
{
  gyro_data();
  float error;
  static  float last_error;
  float price_Differention;
  float price_Proportion;
  static float Pre1_Error[4];
  float Direct_Parameter;
  if(gyro[2] > 2500) gyro[2] = 2500;
  if(gyro[2] < -2500) gyro[2] = -2500;
  error = Angle*8.0 + gyro[2]*Gro3_p;  //ת���ڻ�������һ�㲻�øģ��ı����Ĳ������ı������С
  price_Proportion=0.35 * error;
  price_Differention= 0.35 * (error - last_error);

  Direct_Parameter = price_Proportion + price_Differention ;//

  last_error =  error;
  Pre1_Error[3]=Pre1_Error[2];
  Pre1_Error[2]=Pre1_Error[1];
  Pre1_Error[1]=Pre1_Error[0];
  Pre1_Error[0]=Direct_Parameter;
  Direct_Parameter=Pre1_Error[0]*0.8+Pre1_Error[1]*0.1+Pre1_Error[2]*0.06+Pre1_Error[3]*0.04 ;
  OUT_PWM=(int16)(Direct_Parameter);
}
//*********������cv�Ĵ���pid�������⻷*************
