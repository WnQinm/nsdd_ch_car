/*
 * imgproc.c
 *
 *  Created on: 2023��4��8��
 *      Author: ���Σ�Ұ���ȱ�
 */

#include "imgproc.h"

uint8 rightline[MT9V03X_H], leftline[MT9V03X_H];//rightline�����ߣ�leftline������
uint8 road_width[MT9V03X_H], centerline[MT9V03X_H];//road_width����·��ȣ�centerline����·����
uint8 bin_image[MT9V03X_H][MT9V03X_W];//ͼ������
uint8 perspectiveImage[MT9V03X_H][MT9V03X_W];//��͸��֮������
int16   Road_Width_Min;     // ��С�������

breakpoint LeftBreakpoint, RightBreakpoint;//����Ľǵ��Ǵ������Ͽ��ģ�����startָ�����Ǹ��ǵ�

uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row)
{
#define GrayScale 256
    uint16 Image_Width  = col;
    uint16 Image_Height = row;
    int X; uint16 Y;
    uint8* data = image;
    int HistGram[GrayScale] = {0};

    uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32 PixelIntegralFore = 0;
    int32 PixelFore = 0;
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // ��䷽��;
    uint8 MinValue=0, MaxValue=0;
    uint8 Threshold = 0;


    for (Y = 0; Y <Image_Height; Y++) //Y<Image_Height��ΪY =Image_Height���Ա���� �ж�ֵ��
    {
        //Y=Image_Height;
        for (X = 0; X < Image_Width; X++)
        {
        HistGram[(int)data[Y*Image_Width + X]]++; //ͳ��ÿ���Ҷ�ֵ�ĸ�����Ϣ
        }
    }
    for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;        //��ȡ��С�Ҷȵ�ֵ
    for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ; //��ȡ���Ҷȵ�ֵ

    if (MaxValue == MinValue)
    {
        return MaxValue;          // ͼ����ֻ��һ����ɫ
    }
    if (MinValue + 1 == MaxValue)
    {
        return MinValue;      // ͼ����ֻ�ж�����ɫ
    }

    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        Amount += HistGram[Y];        //  ��������
    }

    PixelIntegral = 0;
    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        PixelIntegral += HistGram[Y] * Y;//�Ҷ�ֵ����
    }
    SigmaB = -1;
    for (Y = MinValue; Y < MaxValue; Y++)
    {
          PixelBack = PixelBack + HistGram[Y];    //ǰ�����ص���
          PixelFore = Amount - PixelBack;         //�������ص���
          OmegaBack = (double)PixelBack / Amount;//ǰ�����ذٷֱ�
          OmegaFore = (double)PixelFore / Amount;//�������ذٷֱ�
          PixelIntegralBack += HistGram[Y] * Y;  //ǰ���Ҷ�ֵ
          PixelIntegralFore = PixelIntegral - PixelIntegralBack;//�����Ҷ�ֵ
          MicroBack = (double)PixelIntegralBack / PixelBack;//ǰ���ҶȰٷֱ�
          MicroFore = (double)PixelIntegralFore / PixelFore;//�����ҶȰٷֱ�
          Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
          if (Sigma > SigmaB)//����������䷽��g
          {
              SigmaB = Sigma;
              Threshold = (uint8)Y;
          }
    }
   return Threshold;
}


void turn_to_bin(uint8 raw_image[MT9V03X_H][MT9V03X_W], uint8 image_w, uint8 image_h)
{
    uint8 i,j;
    int thereshold = otsuThreshold(raw_image[0], image_w, image_h);
    for(i = 0;i<image_h;i++)
    {
        for(j = 0;j<image_w;j++)
        {
            if(raw_image[i][j]>thereshold)bin_image[i][j] = white;
            else bin_image[i][j] = black;
        }
    }
}

uint8 mid=COL/2;//���������һ��ͼ������ɨ�����
void findline(uint8 image[ROW][COL])
{
    // ������Ϣ��ʼ��
    LeftBreakpoint.start_y = 255;
    LeftBreakpoint.end_y = 255;
    RightBreakpoint.start_y = 255;
    RightBreakpoint.end_y = 255;

    //�����������
    for(int col=mid;col>0;col--)
    {
        if(col==1)
        {
            leftline[ROW-1] = 0;
            break;
        }
        if(image[ROW-1][col-1]==black && image[ROW-1][col]==black && image[ROW-1][col+1]==white)
        {
            leftline[ROW-1] = col;
            break;
        }
    }
    //�������ұ���
    for(int col=mid;col<COL-1;col++)
    {
        if(col==COL-2)
        {
            rightline[ROW-1] = COL-1;
            break;
        }
        if(image[ROW-1][col-1]==white && image[ROW-1][col]==black && image[ROW-1][col+1]==black)
        {
            rightline[ROW-1] = col;
            break;
        }
    }
    centerline[ROW-1] = (leftline[ROW-1]+rightline[ROW-1])>>1;
    mid = centerline[ROW-1];

    //������
    for(int row=ROW-2;row>0;row--)
    {
        // �����
        for(int col=centerline[row-1];col>0;col--)
        {
            if(col==1)//����ѵ���ͼ��߽�
            {
                leftline[row] = 0;
                break;
            }

            if(image[row][col-1]==black && image[row][col]==black && image[row][col+1]==white)
            {
                leftline[row] = col;
                // TODO:���Լ������һ�еı��ߵ�x�����࣬���������ǽǵ�
                if(row<ROW-5 && leftline[row+5]-leftline[row]>30 && LeftBreakpoint.start_y==255)//��һ���ҵ����½ǵ�
                    LeftBreakpoint.start_y = row+5;
                else if(row<ROW-5 && leftline[row]-leftline[row+5]>30 && LeftBreakpoint.end_y==255)//��һ���ҵ����Ͻǵ�
                    LeftBreakpoint.end_y = row;

//                if(LeftBreakpoint.start_y!=-1 && LeftBreakpoint.end_y==-1)//�ҵ������½ǵ㣬û�ҵ����Ͻǵ㣬��ô��ʱ����û�ѵ��߽���ҲӦ���Ǳ߽�
//                    leftline[row] = 0;

                break;
            }
        }

        // �ұ���
        for(int col=centerline[row-1];col<COL-1;col++)
        {
            if(col==COL-2)
            {
                rightline[row] = COL-1;
                break;
            }

            if(image[row][col-1]==white && image[row][col]==black && image[row][col+1]==black)
            {
                rightline[row] = col;

                if(row<ROW-5 && rightline[row]-rightline[row+5]>30 && RightBreakpoint.start_y==255)//��һ���ҵ����½ǵ�
                    RightBreakpoint.start_y = row+5;
                else if(row<ROW-5 && rightline[row+5]-rightline[row]>30 && RightBreakpoint.end_y==255)//��һ���ҵ����Ͻǵ�
                    RightBreakpoint.end_y = row;

                break;
            }
        }

        centerline[row] = (leftline[row] + rightline[row])>>1;

//        printf("%d---%d---%d\n", leftline[row], centerline[row], rightline[row]);
    }

    ips200_show_int(0, 220, LeftBreakpoint.end_y, 4);ips200_show_int(40, 220, RightBreakpoint.end_y, 4);
    ips200_show_int(0, 260, LeftBreakpoint.start_y, 4);ips200_show_int(40, 260, RightBreakpoint.start_y, 4);

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

// ���ݵ��������ֱ��ʱ,����б����Ҫ�ĵ�ĸ���(��Խ��,���ֱ��Ч��Խ��;��Խ��,���ܻ������绷���Ķ���)
// TODO:��K�������޷�45�ȣ�������Ҫ����
#define slope_len 5
void AddLine()
{
    // ������ߣ�
    if(LeftBreakpoint.start_y!=255 && LeftBreakpoint.end_y!=255)// ���½ǵ㶼��
        CommonRectificate(leftline, LeftBreakpoint.end_y, LeftBreakpoint.start_y);//end������start�����棬����begin��end
    else if(LeftBreakpoint.start_y!=255 && LeftBreakpoint.end_y==255)// ֻ���½ǵ�û���Ͻǵ�
    {
        float k = Slope_Calculate(LeftBreakpoint.start_y, LeftBreakpoint.start_y+slope_len, leftline);
        if(abs(k)<1)
            goto addline;
        float b = LeftBreakpoint.start_y - k * leftline[LeftBreakpoint.start_y];
        for(uint8 row=LeftBreakpoint.start_y;row>=0;row--)
            leftline[row] = (uint8)(row-b)/k;
    }
    else if(LeftBreakpoint.start_y==255 && LeftBreakpoint.end_y!=255)// ֻ���Ͻǵ�û���½ǵ�
    {
        float k = Slope_Calculate(LeftBreakpoint.end_y-slope_len, LeftBreakpoint.end_y, leftline);
        if(abs(k)<1)
            goto addline;
        float b = LeftBreakpoint.end_y - k * leftline[LeftBreakpoint.end_y];
        for(uint8 row=LeftBreakpoint.end_y;row<ROW;row++)
            leftline[row] =(uint8)(row-b)/k;
    }

    // ���ұ��ߣ�
    if(RightBreakpoint.start_y!=255 && RightBreakpoint.end_y!=255)// ���½ǵ㶼��
        CommonRectificate(rightline, RightBreakpoint.end_y, RightBreakpoint.start_y);
    else if(RightBreakpoint.start_y!=255 && RightBreakpoint.end_y==255)// ֻ���½ǵ�û���Ͻǵ�
    {
        float k = Slope_Calculate(RightBreakpoint.start_y, RightBreakpoint.start_y+slope_len, rightline);
        if(abs(k)<1)
            goto addline;
        float b = RightBreakpoint.start_y - k * rightline[RightBreakpoint.start_y];
        for(uint8 row=RightBreakpoint.start_y;row>=0;row--)
            rightline[row] = (uint8)(row-b)/k;
    }
    else if(RightBreakpoint.start_y==255 && RightBreakpoint.end_y!=255)// ֻ���Ͻǵ�û���½ǵ�
    {
        float k = Slope_Calculate(RightBreakpoint.end_y-slope_len, RightBreakpoint.end_y, rightline);
        if(abs(k)<1)
            goto addline;
        float b = RightBreakpoint.end_y - k * rightline[RightBreakpoint.end_y];
        for(uint8 row=RightBreakpoint.end_y;row<ROW;row++)
            rightline[row] =(uint8)(row-b)/k;
    }

    addline:
    // �������¼���
    // ���ұ��߶�������,����ʼ����
    if(LeftBreakpoint.start_y!=255 && LeftBreakpoint.end_y!=255 && RightBreakpoint.start_y!=255 && RightBreakpoint.end_y!=255)
        for(int row=max(LeftBreakpoint.start_y, RightBreakpoint.start_y);row>min(LeftBreakpoint.end_y, RightBreakpoint.end_y);row--)
            centerline[row] = (leftline[row]+rightline[row])>>1;
    // ���ұ��߶�������,����������
    if(LeftBreakpoint.start_y==255 && LeftBreakpoint.end_y!=255 && RightBreakpoint.start_y==255 && RightBreakpoint.end_y!=255)
        for(int row=min(LeftBreakpoint.end_y, RightBreakpoint.end_y);row<ROW-1;row++)
            centerline[row] = (leftline[row]+rightline[row])>>1;
    // ���ұ��߶�������,����������
    if(LeftBreakpoint.start_y!=255 && LeftBreakpoint.end_y==255 && RightBreakpoint.start_y!=255 && RightBreakpoint.end_y==255)
        for(int row=max(LeftBreakpoint.start_y, RightBreakpoint.start_y);row>0;row--)
            centerline[row] = (leftline[row]+rightline[row])>>1;
    // TODO:ֻ��һ������

}

void DynamicAimline()
{

}

// �������ɷ��������ұ���
// ******���������index:0ָ��ͼ��������******
//void BinThreshold(unsigned char imageIn[MT9V03X_H][MT9V03X_W])
//{
//    // ������Ϣ��ʼ��
//
//
//    //***********�������ɷ�**************
//    //����һ��ɨ��
//    uint8 mid=MT9V03X_W/2;
//    for(int i=mid-1;0<i;i--){
//        if(i==1){
//            leftline[0] = 0;
//            Lost_leftline.starty = ROW-1;
//            Lost_leftline.lost_time++;
//            break;
//        }else if(abs(bin_image[MT9V03X_H-1][i]-bin_image[MT9V03X_H-1][i-1])==white){
//            leftline[0]=i;
//            break;
//        }
//    }
//    for(int j=mid;j<MT9V03X_W-1;j++){
//        if(j==MT9V03X_W-2){
//            rightline[0] = MT9V03X_W-1;
//            Lost_rightline.starty = ROW-1;
//            Lost_rightline.lost_time++;
//            break;
//        }else if(abs(bin_image[MT9V03X_H-1][j]-bin_image[MT9V03X_H-1][j+1])==white){
//            rightline[0]=j;
//            break;
//        }
//    }
//
//    // ����ɨ�����
//    mid = (leftline[0]+rightline[0])>>1;
//
//    //������Ѳ�ߣ�
//    for(int i=0;i<MT9V03X_H-1;i++){
//        uint8 left=leftline[i];
//        uint8 right=rightline[i];
//        uint8 next1=i+1;  //����������������
//        uint8 next2=MT9V03X_H-1-next1;  //����ͼ��������
//
//        //����
//        uint8 isLeftDiuXian=0; //0��ʾ���ߣ��������ֱ�ʾû�ж���
//        for(int j=0;j<50&&isLeftDiuXian==0;j++){
//            if(left+j+1<MT9V03X_W&&abs(imageIn[next2][left+j]-imageIn[next2][left+j+1])==white){
//                leftline[next1]=left+j;
//                isLeftDiuXian=114;
//                if(Lost_leftline.starty != -1 && Lost_leftline.endy == -1)//���ߺ��һ���ҵ���
//                {
//                    Lost_leftline.endy = next1;
//                }
//            }
//            if(left-j-1>=0&&abs(imageIn[next2][left-j-1]-imageIn[next2][left-j])==white){
//                leftline[next1]=left-j;
//                isLeftDiuXian=114;
//                if(Lost_leftline.starty != -1 && Lost_leftline.endy == -1)//���ߺ��һ���ҵ���
//                {
//                    Lost_leftline.endy = next1;
//                }
//            }
//        }
//        //������
//        if(isLeftDiuXian==0){
//            leftline[next1]=0;
//            if(Lost_leftline.starty ==0)// �����һ�ζ��ߣ���һ�еı��ߵ㼴Ϊ�ǵ�
//            {// TODO:�ҿ������ҵ��� ָ�ҵ���next1�е��ߣ����������Ūnext1ǰһ�У�����i��
//                Lost_leftline.starty = i;
//            }
//            else// ���ǵ�һ�ζ��ߣ���Ӷ�������
//                Lost_leftline.lost_time++;
//        }
//
//
//        //����
//        uint8 isRightDiuXian=0; //0��ʾ���ߣ��������ֱ�ʾû�ж���
//        for(int j=0;j<50&&isRightDiuXian==0;j++){
//            if(right+j+1<MT9V03X_W&&abs(imageIn[next2][right+j]-imageIn[next2][right+j+1])==white){
//                rightline[next1]=right+j;
//                isRightDiuXian=114;
//                if(Lost_rightline.starty != -1 && Lost_rightline.endy == -1)//���ߺ��һ���ҵ���
//                {
//                    Lost_rightline.endy = next1;
//                }
//            }
//            if(right-j-1>=0&&abs(imageIn[next2][right-j-1]-imageIn[next2][right-j])==white){
//                rightline[next1]=right-j;
//                isRightDiuXian=114;
//                if(Lost_rightline.starty != -1 && Lost_rightline.endy == -1)//���ߺ��һ���ҵ���
//                {
//                    Lost_rightline.endy = next1;
//                }
//            }
//        }
//        //�����ߣ�������ߣ���˵����ǰ�ǻ�������ʮ��·��
//        // TODO:������������ߣ���ֻ��ע��һ�ζ���
//        if(isRightDiuXian==0){
//            rightline[next1]=MT9V03X_W-1;
//            if(Lost_leftline.starty ==0)// �����һ�ζ��ߣ���һ�еı��ߵ㼴Ϊ�ǵ�
//            {// TODO:�ҿ������ҵ��� ָ�ҵ���next1�е��ߣ����������Ūnext1ǰһ�У�����i��
//                Lost_leftline.starty = i;
//            }
//            else// ���ǵ�һ�ζ��ߣ���Ӷ�������
//                Lost_leftline.lost_time++;
//        }
//        centerline[next1] = (leftline[next1]+rightline[next1])>>1;
//        //����Ӧ���е�·״��ʶ�𣬵��Ҳ���д/doge
//        //
//        //1145141919810
//        //
//        //��·ʶ�����
//    }
//    //***********�������ɷ�����**************
//}

// �����㣬�е����⣬�ᶪ�ߣ��ȴ��Ż����Ҹ��õĴ���
void ImageFilter(unsigned char imageInput[MT9V03X_H][MT9V03X_W])
{
    uint16 temp = 0;
    for(int i = 1; i < MT9V03X_H - 1; i++)
    {
        for(int j = 1; j < MT9V03X_W - 1; j++)
        {
            temp = imageInput[i-1][j-1] + imageInput[i-1][j] + imageInput[i-1][j+1] +
                   imageInput[i  ][j-1] + imageInput[i  ][j] + imageInput[i  ][j+1] +
                   imageInput[i+1][j-1] + imageInput[i+1][j] + imageInput[i+1][j+1];

            /* ������5�����Ǳ��� �����õ� ���Ե��������Ż��˲�Ч�� */
            if(temp > 4*255)
            {
                imageInput[i][j] = 255;
            }
            else
            {
                imageInput[i][j] = 0;
            }
        }
    }
}

// ������
void Draw_Side()
{
//    ips200_clear();
    for(uint8 row = 0; row<MT9V03X_H; row++)
    {
        if(leftline[row]<0)
            leftline[row] = 0;
        if(leftline[row]>=MT9V03X_W)
            leftline[row] = MT9V03X_W-1;
        if(rightline[row]<0)
            rightline[row] = 0;
        if(rightline[row]>=MT9V03X_W)
            rightline[row] = MT9V03X_W-1;
        ips200_draw_point(leftline[row], row, RGB565_BLUE);
        ips200_draw_point(rightline[row], row, RGB565_BLUE);
        ips200_draw_point(centerline[row], row, RGB565_RED);
    }
}

void ImagePerspective_Init(uint8 BinImage[MT9V03X_H][MT9V03X_W], uint8 ResultImage[MT9V03X_H][MT9V03X_W])
{
//    float change_un_Mat[3][3] ={{-0.352043,0.324030,-23.060735},{0.012529,0.058598,-30.547456},{0.000205,0.003490,-0.632328}};
    float change_un_Mat[3][3] ={{-0.234447,0.227495,-17.207843},{0.009790,0.019755,-16.513155},{0.000104,0.002306,-0.410462}};
    for (int i = 0; i < COL ;i++) {
        for (int j = 0; j < ROW ;j++) {
            int local_x = (int) ((change_un_Mat[0][0] * i
                    + change_un_Mat[0][1] * j + change_un_Mat[0][2])
                    / (change_un_Mat[2][0] * i + change_un_Mat[2][1] * j
                            + change_un_Mat[2][2]));
            int local_y = (int) ((change_un_Mat[1][0] * i
                    + change_un_Mat[1][1] * j + change_un_Mat[1][2])
                    / (change_un_Mat[2][0] * i + change_un_Mat[2][1] * j
                            + change_un_Mat[2][2]));
            if (local_x> 0&& local_y > 0 && local_y < MT9V03X_H-1 && local_x < MT9V03X_W-1){
                ResultImage[j][i] = BinImage[local_y][local_x];
            }
            else {
                ResultImage[j][i] = black;
            }
        }
    }

}

uint8 JudgeLeftorRight(){
    uint8 mid0=(uint8)(leftline[0]+rightline[0])/2;
    uint8 mid24=(uint8)(leftline[24]+rightline[24])/2;
    if(mid24-mid0>0){
        return 1;  //����
    }else{
        return -1;  //����
    }
}

void FindLineFromOneSide(uint8 turn){
    if(turn==1){  //����
        for(int i=0;i<MT9V03X_H;i++){
            centerline[i]=rightline[i]-26;
            if(centerline[i]<0){
                centerline[i]=0;
            }
        }
    }else{
        for(int i=0;i<MT9V03X_H;i++){
            centerline[i]=leftline[i]+26;
            if(centerline[i]>=MT9V03X_W){
                centerline[i]=MT9V03X_W-1;
            }
        }
    }
}

void Image_Handle()
{
    // ��ʼ��ʱ
//       timer_start(TIM_7);


    if(mt9v03x_finish_flag)
    {
        ImagePerspective_Init(mt9v03x_image, perspectiveImage);
        turn_to_bin(perspectiveImage, MT9V03X_W, MT9V03X_H);
        ips200_displayimage03x((const uint8 *)bin_image, MT9V03X_W, MT9V03X_H);
//        bluetooth_ch9141_send_image((const uint8 *)perspectiveImage, MT9V03X_IMAGE_SIZE);
//        camera_send_image(DEBUG_UART_INDEX, (const uint8 *)bin_image, MT9V03X_IMAGE_SIZE);
//        BinThreshold(bin_image);
        findline(bin_image);//TODO:�����ϱ߽���͸�ӵ��¾�ݣ����ж�λ�ǵ㷽��������������
        AddLine();//TODO:��Ƭ��ż���쳣ͣ����������Խ��

//        FindLineFromOneSide(JudgeLeftorRight());

        Draw_Side();
        mt9v03x_finish_flag = 0;
    }

    // ������ʱ
//       timer_stop(TIM_7);
//       ips200_show_string(0, 240, "time:");
//       ips200_show_int(60, 240, timer_get(TIM_7), 8);
//       timer_clear(TIM_7);
}
