/*
 * imgproc.c
 *
 *  Created on: 2023��4��8��
 *      Author: ���Σ�Ұ���ȱ�
 */

#include "imgproc.h"

uint8 rightline[MT9V03X_H], leftline[MT9V03X_H];//rightline�����ߣ�leftline������
uint8 road_width[MT9V03X_H], centerline[MT9V03X_H];//road_width����·��ȣ�centerline����·����
uint8 bin_image[ROW][COL];//ͼ������
uint8 perspectiveImage[ROW][COL];//��͸��֮������

breakpoint LeftBreakpoint, RightBreakpoint;//����Ľǵ��Ǵ������Ͽ��ģ�����startָ�����Ǹ��ǵ�
uint8 mid=COL/2;//���������һ��ͼ������ɨ�����
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

/*
 * ͼ���ֵ����������򷨣���ʹ�ú�ֱ�ӵ���bin_imageʹ��
 */
void turn_to_bin(uint8 raw_image[MT9V03X_H/2][MT9V03X_W/2], uint8 image_w, uint8 image_h, int thereshold)
{
    uint8 i,j;
//    int thereshold = otsuThreshold(raw_image[0], image_w, image_h);
    for(i = 0;i<image_h;i++)
    {
        for(j = 0;j<image_w;j++)
        {
            // ��ֵ��
            if(raw_image[i][j]>thereshold)
                bin_image[i][j] = white;
            else
                bin_image[i][j] = black;
        }
    }
}

void halve_image(unsigned char *p_in,unsigned char  *p_out,unsigned char row,unsigned char col, int thereshold) //ͼ�����
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
    // ������Ϣ��ʼ��
    LeftBreakpoint.start_y = 255;
    LeftBreakpoint.end_y = 255;
    RightBreakpoint.start_y = 255;
    RightBreakpoint.end_y = 255;
    lostline_cnt = 0;

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
    road_width[ROW-1] = rightline[ROW-1]-leftline[ROW-1];
    mid = centerline[ROW-1];

    //������
    for(int row=ROW-2;row>SearchLineEndRow;row--)
    {
        // �����
        for(int col=centerline[row+1];col>0;col--)
        {
            if(col==1)//����ѵ���ͼ��߽�
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

        // �ұ���
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

        // ��ǰ�п�ȱ��&&(��һ���ҵ���ȱ�����||(���ǵ�һ���ҵ�����һ��Ҳ��ȱ��(��������������)))
        if(road_width[row]>Road_Width_Min+5 && (lostline_cnt==0 || (lostline_cnt!=0 && road_width[row+1]>Road_Width_Min+5)))
            lostline_cnt++;

//        printf("%d\n", road_width[row]);
//        printf("%d---%d---%d\n", leftline[row], centerline[row], rightline[row]);
    }

    // todo �쳣�ܶ��б��,�����ǹ��䴦֮�����������ͼ���·�
    if(lostline_cnt>Road_Width_Min+10)
        lostline_cnt = 0;

    for(int row=ROW-2;row>SearchLineEndRow-1;row--)//ֻ��¼��һ�ζ���
    {
        if(road_width[row]>Road_Width_Min+5)
        {
            // todo
            if(leftline[row]-leftline[row+1]>8 && LeftBreakpoint.end_y==255)// ���Ͻǵ�
                LeftBreakpoint.end_y=row;
            else if(leftline[row+1]-leftline[row]>8 && LeftBreakpoint.start_y==255)// ���½ǵ�
                LeftBreakpoint.start_y=row+1;
            if(rightline[row]-rightline[row+1]>8 && RightBreakpoint.start_y==255)// ���½ǵ�
                RightBreakpoint.start_y=row+1;
            else if(rightline[row+1]-rightline[row]>8 && RightBreakpoint.end_y==255)// ���Ͻǵ�
                RightBreakpoint.end_y=row;
        }
    }


}


// ������
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

// ������
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
    // todo ������ʶ��Ƚϼ��޵��������,�����Ե�С����ͷ�Ƕȣ�aimline��Զ
    //    ips200_show_float(0, 200, (rightline[default_aimline]-rightline[ROW-1])/(default_aimline-ROW+1.0), 8, 6);
}
