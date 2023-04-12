/*
 * imgproc.c
 *
 *  Created on: 2023��4��8��
 *      Author: ����
 */

#include "imgproc.h"

#define white 255
#define black 0

#define Road_End_row 90

uint8 rightline[MT9V03X_H], leftline[MT9V03X_H];
uint8 road_width[MT9V03X_H], centerline[MT9V03X_H];

//sobel���Ӵ���֮���ͼ��û�н����ֱ�ӷ���ԭͼ��洢λ�õ�ԭ��������ͷˢ�¹��츲�ǵ����Ѿ�����õ�ͼ��Ӱ���˺���Ĳ���
unsigned char img_sobel[MT9V03X_H][MT9V03X_W];
//unsigned char img_sobel_pers[MT9V03X_H][MT9V03X_W];
uint8 OpeningRoad_Flag = 0;                     // ��·��־λ 0�����Ƕ�· 1����·

// ����sobel������ȡ������Ե
void sobelAutoThreshold(unsigned char imageIn[MT9V03X_H][MT9V03X_W], unsigned char imageOut[MT9V03X_H][MT9V03X_W])
{
    // ����˴�С
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
            // ���㲻ͬ�����ݶȷ�ֵ
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

            // �ҳ��ݶȷ�ֵ���ֵ
            for(k = 1; k < 4; k++)
            {
                if (temp[0] < temp[k])
                {
                    temp[0] = temp[k];
                }
            }

            // ʹ�����ص����������ص�֮�͵�һ��������Ϊ��ֵ
            temp[3] = (short) imageIn[i - 1][j - 1] + (short) imageIn[i - 1][j] + (short) imageIn[i - 1][j + 1]
            + (short) imageIn[i][j - 1] + (short) imageIn[i][j] + (short) imageIn[i][j + 1]
            + (short) imageIn[i + 1][j - 1] + (short) imageIn[i + 1][j] + (short) imageIn[i + 1][j + 1];

            if (temp[0] > temp[3] / 12.0f)
            {
                imageOut[i][j] = 0;     // ��
            }
            else
            {
                imageOut[i][j] = 255;     // ��
            }
        }
    }
}

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

// �������������
int my_abs(uint8 num1, uint8 num2)
{
    if(num1 > num2)
        return num1-num2;
    return num2-num1;
}

// �����߱���ɨ��
// TODO:1.����һ���⣬�����е�ɨ������������Ϊǰһ�е��е�centerline[line+1]
//      2.�ռ�������Ҫ����Ϣ������ʼĩλ�ã������ۼ�������
//      3.ʮ��·���ܿ���ͨ���޸�servo.c�еı궨��aimLine��ͨ����ͼ���·�ɨ���߾����·��ı궨�㣬�·�ɨ�����߾����Ϸ��ı궨��
//      4.�����������һ����ǰ�ᣬ���ǿ���ͨ�����㣨�����-һ�����ȣ����ұ���+һ�����ȣ�����������ߣ������������������ѭ�������ұ���ѭ�������ͨ�������кܴ����

void image_scan(uint8 image_deal[MT9V03X_H][MT9V03X_W])
{
    int line=0, list = 0;
    for(line=MT9V03X_H;line>=Road_End_row;line--)
    {
        // �ұ������е㱩����
        for(list=MT9V03X_W/2-2;list<MT9V03X_W;list++)
        {
            if((image_deal[line][list-2]==white)&&(image_deal[line][list-1]==black)&&(image_deal[line][list]==black))
            {
                rightline[line]=list;
                break;
            }
            if(list == MT9V03X_W)
            {
                //����
            }
        }
        // ��������е㱩����
        for(list=MT9V03X_W/2-2;list>1;list--)
        {
            if((image_deal[line][list]==black)&&(image_deal[line][list+1]==black)&&(image_deal[line][list+2]==white))
            {
                leftline[line]=list;
                break;
            }
            if(list==1)
            {
                //����
            }
        }
        road_width[line]=my_abs(leftline[line],rightline[line]);
        centerline[line]=(rightline[line]+leftline[line])/2;
    }
}

// ������
void Draw_Side()
{
    for(uint8 row = MT9V03X_H; row >=Road_End_row; row--)
    {
        ips200_draw_point(leftline[row], row, RGB565_RED);
        ips200_draw_point(rightline[row], row, RGB565_GREEN);
        ips200_draw_point(centerline[row], row, RGB565_BLUE);
    }
}

#define RESULT_ROW 120//���ͼ����
#define RESULT_COL 188

// ��͸�ӣ����ֲ���������ã�������ʱ����flash��ramת������ʱ����
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
