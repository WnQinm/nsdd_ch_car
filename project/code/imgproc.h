/*
 * imgproc.h
 *
 *  Created on: 2023年4月8日
 *      Author: 清梦
 */

#ifndef IMGPROC_H_
#define IMGPROC_H_

#include "zf_common_headfile.h"
#include "servo.h"
#include "mathh.h"

#define MIDVALUE    46   //  处理前40行 （ ROW-1+ROW-1-20）/2=46
#define white 255
#define black 0
#define ROW MT9V03X_H
#define COL MT9V03X_W

#define Road_End_row 50

typedef struct
{
    uint8 x;
    uint8 y;
}point;

typedef struct
{
    point start;
    point end;
    uint8 lost_time;//丢边线行数
}Lost_Edgeline;

typedef struct
{
    uint8 start_y;
    uint8 end_y;
}breakpoint;

extern uint8 bin_image[MT9V03X_H][MT9V03X_W];//图像数组
extern uint8 perspectiveImage[MT9V03X_H][MT9V03X_W];
extern uint8 OpeningRoad_Flag;
extern uint8 centerline[MT9V03X_H];
extern uint8 leftline[MT9V03X_H],rightline[MT9V03X_H],centerline[MT9V03X_H];

uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row);
void turn_to_bin(uint8 raw_image[MT9V03X_H][MT9V03X_W], uint8 image_w, uint8 image_h);

void BinThreshold(unsigned char imageIn[MT9V03X_H][MT9V03X_W]);
void ImageFilter(unsigned char Bin_Image[MT9V03X_H][MT9V03X_W]);

void ImagePerspective_Init(uint8 BinImage[MT9V03X_H][MT9V03X_W], uint8 ResultImage[MT9V03X_H][MT9V03X_W]);

void Draw_Side();

uint8 JudgeLeftorRight();
void FindLineFromOneSide(uint8 turn);

void Image_Handle();

#endif /* IMGPROC_H_ */
