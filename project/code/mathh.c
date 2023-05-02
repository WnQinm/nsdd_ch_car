/*
 * mathh.c
 *
 *  Created on: 2023��4��28��
 *      Author: ����
 */

#include "mathh.h"

/*-------------------------------------------------------------------------------------------------------------------
�������޷�����
˵��������ʱ���겻��С��0�����ڱ߽�ֵ
-------------------------------------------------------------------------------------------------------------------*/
int16 Limit_Protect(int16 num, int32 min, int32 max)
{
    if (num >= max)
        return max;
    else if (num <= min)
        return min;
    else
        return num;
}

/*-------------------------------------------------------------------------------------------------------------------
���������㲹������(Poit = Ka * i + Kb)
˵�����������㷨ȷ��Ka,KbȻ��������i�еĲ��ߺ�����
-------------------------------------------------------------------------------------------------------------------*/
int16 Fit_Point(uint8 i, float Ka, float Kb)
{
    float res;
    int16 Result;
    res = i * Ka + Kb;
    Result = Limit_Protect((int16)res, 1, COL-1);
    return Result;
}

/*-------------------------------------------------------------------------------------------------------------------
�������������1
˵�����������㷨ȷ��Ka,KbȻ��������i�еĲ��ߺ�����
            ���ֱ�� y = Ka * x + Kb   Mode == 1������߽磬Mode == 2�����ұ߽�
-------------------------------------------------------------------------------------------------------------------*/
void Curve1_Fitting(float *Ka, float *Kb, int16 *Start, int16 *Line_Add, int16 Mode)
{
    int i;                                      //�����ڲ�ѭ��
    int _start;                             //��ʱ����������ʼ��
    _start = *Start+2;
    if (Mode == 2)                      //�Ҳ���
    {
        for(i=_start; i<=ROW-1; i+=2)                           //Ѱ���ұ߽����ͻ����
            if(rightline[i] < rightline[*Start] )
                *Start = i;                                                                 //����ͻ����
        if (*Start >= 53)
            *Start = 53;
        *Ka = 1.0*(Line_Add[*Start+6] - Line_Add[*Start]) / 6;  //����Ka
        if (*Ka < 0)                                                                        //��ֹ���ָ�ֵ���Ҿ�������ò�����
                *Ka = 0;
    }
    else if (Mode == 1)
    {
        for(i=_start; i<=ROW-1; i+=2)                           //Ѱ����߽����ͻ����
            if(leftline[i] > leftline[*Start] )
                *Start = i;                                                                 //����ͻ����
        if (*Start >= 53)
                *Start = 53;
        *Ka = 1.0*(Line_Add[*Start+6] - Line_Add[*Start]) / 6;  //����Ka
        if (*Ka > 0)                                                                        //��ֹ���ָ�ֵ���Ҿ�������ò�����
                *Ka = 0;
    }
    *Kb = 1.0*Line_Add[*Start] - (*Ka * (*Start));                      //���빫ʽ����Kb
}

/*-------------------------------------------------------------------------------------------------------------------
�������������3
˵�����������㷨ȷ��Ka,KbȻ��������i�еĲ��ߺ�����
            ���ֱ�� y = Ka * x + Kb   Mode == 1������߽磬Mode == 2�����ұ߽�
-------------------------------------------------------------------------------------------------------------------*/
void Curve3_Fitting(float *Ka, float *Kb, uint8 Start,uint8 End, int16 *Line, int16 Mode)//�������ר�ã������������
{
    if (Mode==1)
    {
        *Ka = 1.0*((Line[Start]) - Line[End]) / (Start-End);
        *Kb = 1.0*Line[End] - (*Ka * End);
    }
    else
    {
        *Ka = 1.0*((Line[Start]) - Line[End]) / (Start-End);
        *Kb = 1.0*Line[End] - (*Ka * End);
    }
}

/*-------------------------------------------------------------------------------------------------------------------
��������С���˷����Իع�
˵�������pid������
-------------------------------------------------------------------------------------------------------------------*/
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

