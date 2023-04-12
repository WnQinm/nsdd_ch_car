/*
 * imgproc.c
 *
 *  Created on: 2023年4月8日
 *      Author: 清梦
 */

#include "imgproc.h"

#define white 255
#define black 0

#define Road_End_row 90

uint8 rightline[MT9V03X_H], leftline[MT9V03X_H];
uint8 road_width[MT9V03X_H], centerline[MT9V03X_H];

//sobel算子处理之后的图像，没有将结果直接放入原图像存储位置的原因是摄像头刷新过快覆盖掉了已经处理好的图像，影响了后面的操作
unsigned char img_sobel[MT9V03X_H][MT9V03X_W];
//unsigned char img_sobel_pers[MT9V03X_H][MT9V03X_W];
uint8 OpeningRoad_Flag = 0;                     // 断路标志位 0：不是断路 1：断路

// 利用sobel算子提取赛道边缘
void sobelAutoThreshold(unsigned char imageIn[MT9V03X_H][MT9V03X_W], unsigned char imageOut[MT9V03X_H][MT9V03X_W])
{
    // 卷积核大小
    short KERNEL_SIZE = 3;
    short xStart = KERNEL_SIZE / 2;
    short xEnd = MT9V03X_W - KERNEL_SIZE / 2;
    short yStart = KERNEL_SIZE / 2;
    short yEnd = MT9V03X_H - KERNEL_SIZE / 2;
    short i, j, k;
    short temp[4];
    for(i = yStart; i < yEnd; i++)
    {
        for(j = xStart; j < xEnd; j++)
        {
            // 计算不同方向梯度幅值
            temp[0] = -(short) imageIn[i - 1][j - 1] + (short) imageIn[i - 1][j + 1]
            - (short) imageIn[i][j - 1] + (short) imageIn[i][j + 1]
            - (short) imageIn[i + 1][j - 1] + (short) imageIn[i + 1][j + 1];

            temp[1] = -(short) imageIn[i - 1][j - 1] + (short) imageIn[i + 1][j - 1]
            - (short) imageIn[i - 1][j] + (short) imageIn[i + 1][j]
            - (short) imageIn[i - 1][j + 1] + (short) imageIn[i + 1][j + 1];

            temp[2] = -(short) imageIn[i - 1][j] + (short) imageIn[i][j - 1]
            - (short) imageIn[i][j - 1] + (short) imageIn[i + 1][j]
            - (short) imageIn[i - 1][j - 1] + (short) imageIn[i + 1][j + 1];

            temp[3] = -(short) imageIn[i - 1][j] + (short) imageIn[i][j + 1]
            - (short) imageIn[i][j + 1] + (short) imageIn[i + 1][j]
            - (short) imageIn[i - 1][j + 1] + (short) imageIn[i + 1][j + 1];

            temp[0] = abs(temp[0]);
            temp[1] = abs(temp[1]);
            temp[2] = abs(temp[2]);
            temp[3] = abs(temp[3]);

            // 找出梯度幅值最大值
            for(k = 1; k < 4; k++)
            {
                if (temp[0] < temp[k])
                {
                    temp[0] = temp[k];
                }
            }

            // 使用像素点邻域内像素点之和的一定比例作为阈值
            temp[3] = (short) imageIn[i - 1][j - 1] + (short) imageIn[i - 1][j] + (short) imageIn[i - 1][j + 1]
            + (short) imageIn[i][j - 1] + (short) imageIn[i][j] + (short) imageIn[i][j + 1]
            + (short) imageIn[i + 1][j - 1] + (short) imageIn[i + 1][j] + (short) imageIn[i + 1][j + 1];

            if (temp[0] > temp[3] / 12.0f)
            {
                imageOut[i][j] = 0;     // 黑
            }
            else
            {
                imageOut[i][j] = 255;     // 白
            }
        }
    }
}

// 清除噪点，有点问题，会丢线，等待优化或找更好的代码
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

// 计算赛道宽度用
int my_abs(uint8 num1, uint8 num2)
{
    if(num1 > num2)
        return num1-num2;
    return num2-num1;
}

// 自中线暴力扫线
// TODO:1.除第一行外，其他行的扫线起点可以设置为前一行的中点centerline[line+1]
//      2.收集可能需要的信息：丢线始末位置，丢线累计行数等
//      3.十字路可能可以通过修改servo.c中的标定点aimLine来通过，图像下方扫到线就用下方的标定点，下方扫不到线就用上方的标定点
//      4.基于赛道宽度一定的前提，我们可以通过计算（左边线-一定长度）或（右边线+一定长度）获得两条中线，进而衍生出沿左边线循迹和沿右边线循迹，这对通过环岛有很大帮助

void image_scan(uint8 image_deal[MT9V03X_H][MT9V03X_W])
{
    int line=0, list = 0;
    for(line=MT9V03X_H;line>=Road_End_row;line--)
    {
        // 右边线自中点暴力搜
        for(list=MT9V03X_W/2-2;list<MT9V03X_W;list++)
        {
            if((image_deal[line][list-2]==white)&&(image_deal[line][list-1]==black)&&(image_deal[line][list]==black))
            {
                rightline[line]=list;
                break;
            }
            if(list == MT9V03X_W)
            {
                //丢线
            }
        }
        // 左边线自中点暴力搜
        for(list=MT9V03X_W/2-2;list>1;list--)
        {
            if((image_deal[line][list]==black)&&(image_deal[line][list+1]==black)&&(image_deal[line][list+2]==white))
            {
                leftline[line]=list;
                break;
            }
            if(list==1)
            {
                //丢线
            }
        }
        road_width[line]=my_abs(leftline[line],rightline[line]);
        centerline[line]=(rightline[line]+leftline[line])/2;
    }
}

// 画边线
void Draw_Side()
{
    for(uint8 row = MT9V03X_H; row >=Road_End_row; row--)
    {
        ips200_draw_point(leftline[row], row, RGB565_RED);
        ips200_draw_point(rightline[row], row, RGB565_GREEN);
        ips200_draw_point(centerline[row], row, RGB565_BLUE);
    }
}

#define RESULT_ROW 120//结果图行列
#define RESULT_COL 188

// 逆透视，各种参数都已配好，由于暂时不会flash和ram转换，暂时放弃
void ImagePerspective_Init(uint8 BinImage[MT9V03X_H][MT9V03X_W], uint8 ResultImage[MT9V03X_H][MT9V03X_W])
{
    double change_un_Mat[3][3] ={{0.098558,-0.073133,2.282418},
                                {-0.000806,0.003501,3.015971},
                                {-0.000017,-0.000820,0.129776}};
    for (int i = 0; i < RESULT_COL ;i++) {
        for (int j = 0; j < RESULT_ROW ;j++) {
            int local_x = (int) ((change_un_Mat[0][0] * i
                    + change_un_Mat[0][1] * j + change_un_Mat[0][2])
                    / (change_un_Mat[2][0] * i + change_un_Mat[2][1] * j
                            + change_un_Mat[2][2]));
            int local_y = (int) ((change_un_Mat[1][0] * i
                    + change_un_Mat[1][1] * j + change_un_Mat[1][2])
                    / (change_un_Mat[2][0] * i + change_un_Mat[2][1] * j
                            + change_un_Mat[2][2]));
            if (local_x>= 0&& local_y >= 0 && local_y < MT9V03X_H && local_x < MT9V03X_W){
                ResultImage[j][i] = BinImage[local_y][local_x];
            }
            else {
                ResultImage[j][i] = white;
            }

        }
    }

}
