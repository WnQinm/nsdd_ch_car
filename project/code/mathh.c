/*
 * mathh.c
 *
 *  Created on: 2023年4月28日
 *      Author: 清梦
 */

#include "mathh.h"

/*-------------------------------------------------------------------------------------------------------------------
函数：限幅保护
说明：补线时坐标不能小于0，大于边界值
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
函数：计算补线坐标(Poit = Ka * i + Kb)
说明：先用两点法确定Ka,Kb然后计算出第i行的补线横坐标
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
函数：曲线拟合1
说明：先用两点法确定Ka,Kb然后计算出第i行的补线横坐标
            拟合直线 y = Ka * x + Kb   Mode == 1代表左边界，Mode == 2代表右边界
-------------------------------------------------------------------------------------------------------------------*/
void Curve1_Fitting(float *Ka, float *Kb, int16 *Start, int16 *Line_Add, int16 Mode)
{
    int i;                                      //用于内部循环
    int _start;                             //临时变量储存起始点
    _start = *Start+2;
    if (Mode == 2)                      //右补线
    {
        for(i=_start; i<=ROW-1; i+=2)                           //寻找右边界近处突出点
            if(rightline[i] < rightline[*Start] )
                *Start = i;                                                                 //更新突出点
        if (*Start >= 53)
            *Start = 53;
        *Ka = 1.0*(Line_Add[*Start+6] - Line_Add[*Start]) / 6;  //计算Ka
        if (*Ka < 0)                                                                        //防止出现负值，我觉得这个用不到，
                *Ka = 0;
    }
    else if (Mode == 1)
    {
        for(i=_start; i<=ROW-1; i+=2)                           //寻找左边界近处突出点
            if(leftline[i] > leftline[*Start] )
                *Start = i;                                                                 //更新突出点
        if (*Start >= 53)
                *Start = 53;
        *Ka = 1.0*(Line_Add[*Start+6] - Line_Add[*Start]) / 6;  //计算Ka
        if (*Ka > 0)                                                                        //防止出现负值，我觉得这个用不到，
                *Ka = 0;
    }
    *Kb = 1.0*Line_Add[*Start] - (*Ka * (*Start));                      //代入公式计算Kb
}

/*-------------------------------------------------------------------------------------------------------------------
函数：曲线拟合3
说明：先用两点法确定Ka,Kb然后计算出第i行的补线横坐标
            拟合直线 y = Ka * x + Kb   Mode == 1代表左边界，Mode == 2代表右边界
-------------------------------------------------------------------------------------------------------------------*/
void Curve3_Fitting(float *Ka, float *Kb, uint8 Start,uint8 End, int16 *Line, int16 Mode)//环岛检测专用，最正规的求线
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
函数：最小二乘法线性回归
说明：舵机pid控制用
-------------------------------------------------------------------------------------------------------------------*/
int regression(int startline,int endline)
{

  int i=0,SumX=0,SumY=0,SumLines = 0;
  float SumUp=0,SumDown=0,avrX=0,avrY=0,B;
  SumLines=endline-startline;   // startline 为开始行， //endline 结束行 //SumLines

  for(i=startline;i<endline;i++)
  {
    SumX+=i;
    SumY+=centerline[i];
  }
  avrX=SumX/SumLines;     //X的平均值
  avrY=SumY/SumLines;     //Y的平均值
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
//    A=(SumY-B*SumX)/SumLines;  //截距
    return B;  //返回斜率
}

