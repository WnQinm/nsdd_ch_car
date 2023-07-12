/*
 * imgproc.c
 *
 *  Created on: 2023��4��8��
 *      Author: ���Σ�Ұ���ȱ�
 */

#include "imgproc.h"

uint8 rightline[ROW+2], leftline[ROW+2];//rightline�����ߣ�leftline������
uint8 road_width[ROW+2], centerline[ROW+2];//road_width����·��ȣ�centerline����·����
uint8 bin_image[MT9V03X_H][MT9V03X_W];//ͼ������
uint8 _img[RESULT_ROW][RESULT_COL];

breakpoint LeftBreakpoint, RightBreakpoint;//����Ľǵ��Ǵ������Ͽ��ģ�����startָ�����Ǹ��ǵ�
uint8 mid=COL/2;//���������һ��ͼ������ɨ�����
uint8 lostline_cnt = 0;

uint8 *PerImg_ip[RESULT_ROW][RESULT_COL];
uint8 otsu_thereshold;

uint8 otsu(uint8* data, uint16 Image_Width, uint16 Image_Height)
{
    #define GrayScale 256
    int X; uint16 Y;
    int HistGram[GrayScale];

    for (Y = 0; Y < GrayScale; Y++)
    {
        HistGram[Y] = 0; //��ʼ���Ҷ�ֱ��ͼ
    }
    for (Y = 0; Y <Image_Height; Y++) //Y<Image_Height��ΪY =Image_Height���Ա���� �ж�ֵ��
    {
        //Y=Image_Height;
        for (X = 0; X < Image_Width; X++)
        {
        HistGram[(int)data[Y*Image_Width + X]]++; //ͳ��ÿ���Ҷ�ֵ�ĸ�����Ϣ
        }
    }

    uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32 PixelIntegralFore = 0;
    int32 PixelFore = 0;
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // ��䷽��;
    int16 MinValue=0, MaxValue=0;
    uint8 Threshold = 0;


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
              Threshold = Y;
          }
    }
    return Threshold;
}

/*
 * ͼ���ֵ����������򷨣���ʹ�ú�ֱ�ӵ���bin_imageʹ��
 */
void turn_to_bin(uint8 image_w, uint8 image_h, uint8 thereshold)
{
    uint8 i,j;
    for(i = 0;i<image_h;i++)
    {
        for(j = 0;j<image_w;j++)
        {
            // ��ֵ��
            if((perspectiveImage[i][j])>thereshold)//(perspectiveImage[i][j])
                bin_image[i][j] = white;
            else
                bin_image[i][j] = black;
        }
    }
}

void ImagePerspective_Init(void)
{
    static uint8 BlackColor = 0;

#if CAR_TYPE
    double change_un_Mat[3][3] ={{-1.094710,0.998118,-37.187945},{0.021465,0.246846,-46.739827},{-0.000000,0.010732,-0.912258}};
#else
    double change_un_Mat[3][3] ={{-1.249200,1.007972,-26.999219},{-0.098009,0.294157,-41.195868},{-0.001661,0.011368,-0.857782}};
#endif

    for (int i = 0; i < RESULT_COL ;i++)
    {
        for (int j = 0; j < RESULT_ROW ;j++)
        {
            int local_x = (int) ((change_un_Mat[0][0] * i
                    + change_un_Mat[0][1] * j + change_un_Mat[0][2])
                    / (change_un_Mat[2][0] * i + change_un_Mat[2][1] * j
                            + change_un_Mat[2][2]));
            int local_y = (int) ((change_un_Mat[1][0] * i
                    + change_un_Mat[1][1] * j + change_un_Mat[1][2])
                    / (change_un_Mat[2][0] * i + change_un_Mat[2][1] * j
                            + change_un_Mat[2][2]));
            if (local_x >= 0&& local_y >= 0 && local_y < USED_ROW && local_x < USED_COL)
            {
                PerImg_ip[j][i] = &PER_IMG[local_y][local_x];
            }
            else
            {
                PerImg_ip[j][i] = &BlackColor;
            }
        }
    }
}

void findline()
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
        if(_img[ROW-1][col-1]==black && _img[ROW-1][col]==black && _img[ROW-1][col+1]==white)
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
        if(_img[ROW-1][col-1]==white && _img[ROW-1][col]==black && _img[ROW-1][col+1]==black)
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

            if(_img[row][col-1]==black && _img[row][col]==black && _img[row][col+1]==white)
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

            if(_img[row][col-1]==white && _img[row][col]==black && _img[row][col+1]==black)
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
    if(lostline_cnt>Road_Width_Min+5)
        lostline_cnt = 0;

    if(!slope_flag)
    {
        for(int row=ROW/4;row>1;row--)
        {
            if(row==2)
                slope_flag=true;
            if(leftline[row]<COL/2-Road_Width_Min || rightline[row]>COL/2+Road_Width_Min || road_width[row]<=Road_Width_Min+5)
                break;
        }
    }

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

bool IsBlack(uint8 pixel)
{
    if(pixel>otsu_thereshold)
        return false;
    else
        return true;
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
    for(uint8 row = 0; row<RESULT_ROW; row++)
    {
        if(leftline[row]<0)
            leftline[row] = 0;
        else if(leftline[row]>=MT9V03X_W)
            leftline[row] = MT9V03X_W-1;
        if(rightline[row]<0)
            rightline[row] = 0;
        else if(rightline[row]>=MT9V03X_W)
            rightline[row] = MT9V03X_W-1;
        if(centerline[row]<0)
            centerline[row] = 0;
        else if(centerline[row]>=MT9V03X_W)
            centerline[row] = MT9V03X_W-1;
        ips200_draw_point(leftline[row], row, RGB565_BLUE);
        ips200_draw_point(rightline[row], row, RGB565_BLUE);
        ips200_draw_point(centerline[row], row, RGB565_RED);
    }
}

// �жϰ�����
bool isStopLine()
{
//    ips200_draw_line(0,RESULT_ROW/2,RESULT_COL-1,RESULT_ROW/2,RGB565_PINK);
    uint8 flipCnt=0;
    uint8 previousColor=_img[RESULT_ROW/2][0];
    for(int i=1;i<RESULT_COL;i++){
        if(_img[RESULT_ROW/2][i]!=previousColor){
            flipCnt++;
            previousColor=_img[RESULT_ROW/2][i];
        }
    }
//    ips200_show_int(0,0,flipCnt,3);
    if(flipCnt>=5){
        return true;
    }else{
        return false;
    }
}

void ImageProcess()
{
    // 0ms+
    for(uint8 num=0;num<MT9V03X_H;num++)
       memcpy(bin_image[num],mt9v03x_image[num],MT9V03X_W);

    // +-10ms
    otsu_thereshold = otsu(bin_image[0], MT9V03X_W, MT9V03X_H);
    // 4-5ms
    turn_to_bin(RESULT_COL, RESULT_ROW, otsu_thereshold);

    // ��֪��Ϊʲôͨ��ͨ��������͸������ĺ���÷����޷���������Ļ��ʾͼ����Ҫ�ٸ��Ƶ��������вſ���
    for(uint8 num=0;num<RESULT_ROW;num++)
       memcpy(_img[num],bin_image[num],RESULT_COL);

    findline();
}
