/*
 * imgproc.c
 *
 *  Created on: 2023年4月8日
 *      Author: 清梦，野兽先辈
 */

#include "imgproc.h"

uint8 rightline[MT9V03X_H], leftline[MT9V03X_H];//rightline：右线，leftline：左线
uint8 road_width[MT9V03X_H], centerline[MT9V03X_H];//road_width：道路宽度，centerline：道路中线
uint8 bin_image[ROW][COL];//图像数组
uint8 perspectiveImage[ROW][COL];//逆透视之后数组

breakpoint LeftBreakpoint, RightBreakpoint;//这里的角点是从下向上看的，所以start指下面那个角点
uint8 mid=COL/2;//车车插电后第一张图最下行扫线起点
uint8 lostline_cnt = 0;

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
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // 类间方差;
    uint8 MinValue=0, MaxValue=0;
    uint8 Threshold = 0;


    for (Y = 0; Y <Image_Height; Y++) //Y<Image_Height改为Y =Image_Height；以便进行 行二值化
    {
        //Y=Image_Height;
        for (X = 0; X < Image_Width; X++)
        {
        HistGram[(int)data[Y*Image_Width + X]]++; //统计每个灰度值的个数信息
        }
    }
    for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值

    if (MaxValue == MinValue)
    {
        return MaxValue;          // 图像中只有一个颜色
    }
    if (MinValue + 1 == MaxValue)
    {
        return MinValue;      // 图像中只有二个颜色
    }

    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        Amount += HistGram[Y];        //  像素总数
    }

    PixelIntegral = 0;
    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        PixelIntegral += HistGram[Y] * Y;//灰度值总数
    }
    SigmaB = -1;
    for (Y = MinValue; Y < MaxValue; Y++)
    {
          PixelBack = PixelBack + HistGram[Y];    //前景像素点数
          PixelFore = Amount - PixelBack;         //背景像素点数
          OmegaBack = (double)PixelBack / Amount;//前景像素百分比
          OmegaFore = (double)PixelFore / Amount;//背景像素百分比
          PixelIntegralBack += HistGram[Y] * Y;  //前景灰度值
          PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
          MicroBack = (double)PixelIntegralBack / PixelBack;//前景灰度百分比
          MicroFore = (double)PixelIntegralFore / PixelFore;//背景灰度百分比
          Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
          if (Sigma > SigmaB)//遍历最大的类间方差g
          {
              SigmaB = Sigma;
              Threshold = (uint8)Y;
          }
    }
   return Threshold;
}

/*
 * 图像二值化函数（大津法），使用后直接调用bin_image使用
 */
void turn_to_bin(uint8 raw_image[MT9V03X_H/2][MT9V03X_W/2], uint8 image_w, uint8 image_h, int thereshold)
{
    uint8 i,j;
//    int thereshold = otsuThreshold(raw_image[0], image_w, image_h);
    for(i = 0;i<image_h;i++)
    {
        for(j = 0;j<image_w;j++)
        {
            // 二值化
            if(raw_image[i][j]>thereshold)
                bin_image[i][j] = white;
            else
                bin_image[i][j] = black;
        }
    }
}

void halve_image(unsigned char *p_in,unsigned char  *p_out,unsigned char row,unsigned char col, int thereshold) //图像减半
{

   uint8 i, j;
   for (i = 0; i<row/2; i++)
   {
      for (j = 0;j<col/2; j++)
      {
         *(p_out+i*col/2+j)=*(p_in+i*2*col+j*2);
         *(p_out+i*col/2+j) = *(p_out+i*col/2+j)<thereshold?black:white;
      }
   }

}

void inverse_perspective(uint8 InputImage[ROW][COL], uint8 ResultImage[ROW][COL])
{

    // big car
    double change_un_Mat[3][3] ={{-0.675083,0.652094,-23.148424},{0.063251,0.162263,-31.454260},{0.001216,0.013927,-1.231693}};
    // small car
//    float change_un_Mat[3][3] ={{0.346837,-0.262261,11.624627},{0.003201,0.049002,10.329604},{0.000087,-0.002744,0.470399}};
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
                ResultImage[j][i] = InputImage[local_y][local_x];
            }
            else {
                ResultImage[j][i] = black;
            }

        }
    }
}

void findline(uint8 image[ROW][COL])
{
    // 丢线信息初始化
    LeftBreakpoint.start_y = 255;
    LeftBreakpoint.end_y = 255;
    RightBreakpoint.start_y = 255;
    RightBreakpoint.end_y = 255;
    lostline_cnt = 0;

    //最下行左边线
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
    //最下行右边线
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
    road_width[ROW-1] = rightline[ROW-1]-leftline[ROW-1];
    mid = centerline[ROW-1];

    //其他行
    for(int row=ROW-2;row>SearchLineEndRow;row--)
    {
        // 左边线
        for(int col=centerline[row+1];col>0;col--)
        {
            if(col==1)//如果搜到了图像边界
            {
                leftline[row] = 0;
                break;
            }

            if(image[row][col-1]==black && image[row][col]==black && image[row][col+1]==white)
            {
                leftline[row] = col;
                break;
            }
        }

        // 右边线
        for(int col=centerline[row+1];col<COL-1;col++)
        {
            if(col==COL-2)
            {
                rightline[row] = COL-1;
                break;
            }

            if(image[row][col-1]==white && image[row][col]==black && image[row][col+1]==black)
            {
                rightline[row] = col;
                break;
            }
        }

        centerline[row] = (leftline[row] + rightline[row])>>1;
        road_width[row] = rightline[row]-leftline[row];

        // 当前行宽度变大&&(第一次找到宽度变大的行||(不是第一次找到但上一行也宽度变大(即行数变宽得连续)))
        if(road_width[row]>Road_Width_Min+5 && (lostline_cnt==0 || (lostline_cnt!=0 && road_width[row+1]>Road_Width_Min+5)))
            lostline_cnt++;

//        printf("%d\n", road_width[row]);
//        printf("%d---%d---%d\n", leftline[row], centerline[row], rightline[row]);
    }

    // todo 异常很多行变宽,可能是拐弯处之类的赛道都在图像下方
    if(lostline_cnt>Road_Width_Min+10)
        lostline_cnt = 0;

    for(int row=ROW-2;row>SearchLineEndRow-1;row--)//只记录第一次丢线
    {
        if(road_width[row]>Road_Width_Min+5)
        {
            // todo
            if(leftline[row]-leftline[row+1]>8 && LeftBreakpoint.end_y==255)// 左上角点
                LeftBreakpoint.end_y=row;
            else if(leftline[row+1]-leftline[row]>8 && LeftBreakpoint.start_y==255)// 左下角点
                LeftBreakpoint.start_y=row+1;
            if(rightline[row]-rightline[row+1]>8 && RightBreakpoint.start_y==255)// 右下角点
                RightBreakpoint.start_y=row+1;
            else if(rightline[row+1]-rightline[row]>8 && RightBreakpoint.end_y==255)// 右上角点
                RightBreakpoint.end_y=row;
        }
    }


}


// 清除噪点
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

            /* 邻域内5个点是边沿 则保留该点 可以调节这里优化滤波效果 */
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

void track_Left_Line()
{
    for(uint8 row=ROW-1;row>SearchLineEndRow;row--)
    {
        int tmp = leftline[row] + (Road_Width_Min/2);
        centerline[row] = limit(tmp, COL-1, 0);
    }
}

void track_Right_Line()
{
    for(uint8 row=ROW-1;row>SearchLineEndRow;row--)
    {
        int tmp = rightline[row] - (Road_Width_Min/2);
        centerline[row] = limit(tmp, COL-1, 0);

    }
}

// 画边线
void Draw_Side()
{
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

void ImageProcess()
{
    int thereshold = otsuThreshold(mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
    halve_image(mt9v03x_image[0], bin_image[0], MT9V03X_H, MT9V03X_W, thereshold);
    inverse_perspective(bin_image, perspectiveImage);
//    ImageFilter(perspectiveImage);

//    bluetooth_ch9141_send_image((const uint8 *)bin_image, MT9V03X_IMAGE_SIZE);
//    camera_send_image(DEBUG_UART_INDEX, (const uint8 *)bin_image, MT9V03X_IMAGE_SIZE);
//    findline(perspectiveImage);
    // todo 想用来识别比较极限的弯道来着,先试试调小摄像头角度，aimline调远
    //    ips200_show_float(0, 200, (rightline[default_aimline]-rightline[ROW-1])/(default_aimline-ROW+1.0), 8, 6);
}
