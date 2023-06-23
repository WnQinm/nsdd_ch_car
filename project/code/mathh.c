/*
 * mathh.c
 *
 *  Created on: 2023��4��28��
 *      Author: ����
 */

#include "mathh.h"

uint8 limit(int input, uint8 max, uint8 min)
{
    if(input>max)
        return max;
    else if(input<min)
        return min;
    else
        return (uint8)input;
}

uint8 my_abs(uint8 x)
{
    if(x<0)
        return -x;
    return x;
}

uint8 max(uint8 a, uint8 b)
{
    if(a>b)
        return a;
    else
        return b;
}

uint8 min(uint8 a, uint8 b)
{
    if(a<b)
        return a;
    else
        return b;
}

float slope(uint8 upx, uint8 upy, uint8 downx, uint8 downy)
{
    return (upy-upx)/(downy-downx+eps);
}

/******************************************************************************
 *
//�������   CommonRectificate(&P_LeftBlack[0],startPos-1,endPos+1);
******************************************************************************/
void CommonRectificate(unsigned char data[],unsigned char begin,unsigned char end)
{
   unsigned char MidPos = 0;
   if (end > ROW-1)
   {
      end = ROW-1;
   }
   if (begin == end)
   {
      data[begin] = (data[begin-1]+data[begin+1])>>1;
     // BlackLineData[begin] =  LeftBlack[begin] + (RightBlack[begin]-LeftBlack[begin])/2;
   }
   else if(begin < end)
   {
      MidPos = (begin+end)>>1;
      data[MidPos] = (data[begin]+data[end])>>1;
      //BlackLineData[MidPos] =  LeftBlack[MidPos] + (RightBlack[MidPos]-LeftBlack[MidPos])/2;
      if (begin+1 < MidPos)
      {
         CommonRectificate(data,begin,MidPos);
      }
      if (MidPos+1 < end)
      {
         CommonRectificate(data,MidPos,end);
      }
   }
}

//��С���˷����б��
float Slope_Calculate(uint8 begin,uint8 end,uint8 *p)
{
  float xsum=0,ysum=0,xysum=0,x2sum=0;
  uint8 i=0;
  float result=0;
  static float resultlast;
  p=p+begin;
  for(i=begin;i<end;i++)
  {
      xsum+=i;
      ysum+=*p;
      xysum+=i*(*p);
      x2sum+=i*i;
      p=p+1;
  }
 if((end-begin)*x2sum-xsum*xsum) //�жϳ����Ƿ�Ϊ��
 {
   result=((end-begin)*xysum-xsum*ysum)/((end-begin)*x2sum-xsum*xsum);
   resultlast=result;
 }
 else
 {
  result=resultlast;
 }
 return result;
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

