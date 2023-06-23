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
#include "motor.h"
#include "mathh.h"
#include "judgement.h"

#define white 255
#define black 0
#define ROW MT9V03X_H
#define COL MT9V03X_W
#define Road_Width_Min 66       // 赛道宽度
#define SearchLineEndRow 20     // 扫线结束行

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
extern breakpoint LeftBreakpoint, RightBreakpoint;
extern uint8 lostline_cnt;

uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row);
void turn_to_bin(uint8 raw_image[MT9V03X_H][MT9V03X_W], uint8 image_w, uint8 image_h);

void ImageFilter(unsigned char Bin_Image[MT9V03X_H][MT9V03X_W]);

void ImagePerspective_Init(uint8 BinImage[MT9V03X_H][MT9V03X_W], uint8 ResultImage[MT9V03X_H][MT9V03X_W]);
void track_Left_Line();
void track_Right_Line();
void Draw_Side();

void ImageProcess();

#endif /* IMGPROC_H_ */
