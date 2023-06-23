/*
 * imgproc.h
 *
 *  Created on: 2023��4��8��
 *      Author: ����
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
#define ROW MT9V03X_H/2
#define COL MT9V03X_W/2
#define Road_Width_Min 66       // �������
#define SearchLineEndRow 20     // ɨ�߽�����

typedef struct
{
    uint8 start_y;
    uint8 end_y;
}breakpoint;

extern uint8 bin_image[MT9V03X_H/2][MT9V03X_W/2];//ͼ������
extern uint8 perspectiveImage[MT9V03X_H/2][MT9V03X_W/2];
extern uint8 OpeningRoad_Flag;
extern uint8 centerline[MT9V03X_H];
extern uint8 leftline[MT9V03X_H],rightline[MT9V03X_H],centerline[MT9V03X_H];
extern breakpoint LeftBreakpoint, RightBreakpoint;
extern uint8 lostline_cnt;

void track_Left_Line();
void track_Right_Line();
void Draw_Side();

void ImageProcess();

#endif /* IMGPROC_H_ */
