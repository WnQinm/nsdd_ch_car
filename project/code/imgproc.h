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

#define Road_Width_Min 30       // �������
#define SearchLineEndRow 0     // ɨ�߽�����

#define RESULT_ROW (60)//���ͼ����
#define RESULT_COL (90)
#define USED_ROW (120)//����͸��ͼ������
#define USED_COL (188)

#define ROW RESULT_ROW
#define COL RESULT_COL

#define PER_IMG (bin_image)//mt9v03x_image_dvp:����͸�ӱ任��ͼ�� Ҳ����ʹ�ö�ֵ��ͼ
#define perspectiveImage  *PerImg_ip//*PerImg_ip����ʹ�õ�ͼ��ImageUsedΪ����Ѳ�ߺ�ʶ���ͼ��
extern uint8 *PerImg_ip[RESULT_ROW][RESULT_COL];

typedef struct
{
    uint8 start_y;
    uint8 end_y;
}breakpoint;

extern uint8 bin_image[MT9V03X_H][MT9V03X_W];//ͼ������

extern uint8 road_width[ROW+2], centerline[ROW+2];
extern uint8 leftline[ROW+2],rightline[ROW+2],centerline[ROW+2];
extern breakpoint LeftBreakpoint, RightBreakpoint;
extern uint8 lostline_cnt;
extern uint8 _img[RESULT_ROW][RESULT_COL];

void track_Left_Line();
void track_Right_Line();
void Draw_Side();

void ImageProcess();
void ImagePerspective_Init(void);

#endif /* IMGPROC_H_ */
