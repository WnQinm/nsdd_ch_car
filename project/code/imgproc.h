/*
 * imgproc.h
 *
 *  Created on: 2023年4月8日
 *      Author: 清梦
 */

#ifndef IMGPROC_H_
#define IMGPROC_H_

#include "zf_common_headfile.h"
extern unsigned char img_sobel[MT9V03X_H][MT9V03X_W];
//extern unsigned char img_sobel_pers[MT9V03X_H][MT9V03X_W];
extern uint8 OpeningRoad_Flag;
extern uint8 centerline[MT9V03X_H];

//void Draw_LR_Side();
//void Draw_UD_Side();

void sobelAutoThreshold(unsigned char imageIn[MT9V03X_H][MT9V03X_W], unsigned char imageOut[MT9V03X_H][MT9V03X_W]);
void ImageFilter(unsigned char Bin_Image[MT9V03X_H][MT9V03X_W]);
//uint8 Get_LR_SideLine(uint8 imageInput[MT9V03X_H][MT9V03X_W]);
//uint8 GET_UD_SideLine(uint8_t imageInput[MT9V03X_H][MT9V03X_W]);
//uint8 RoadIsNoSide(uint8 imageInput[MT9V03X_H][MT9V03X_W], uint8 lineIndex);
//void LostSideProcess(uint8_t imageInput[MT9V03X_H][MT9V03X_W], uint8_t imageOut[MT9V03X_H][2], uint8_t mode, uint8_t lineIndex);

//int16 GetSteerBaseError(uint8 imageSide[MT9V03X_H][2], uint8 lineIndex);
//void GetSteerError();
void ImagePerspective_Init(uint8 BinImage[MT9V03X_H][MT9V03X_W], uint8 ResultImage[MT9V03X_H][MT9V03X_W]);
void image_scan(uint8 image_deal[MT9V03X_H][MT9V03X_W]);
void Draw_Side();

#endif /* IMGPROC_H_ */
