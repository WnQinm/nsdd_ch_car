/*
 * imgproc.c
 *
 *  Created on: 2023��4��8��
 *      Author: ���Σ�Ұ���ȱ�
 */

#include "imgproc.h"

uint8 rightline[MT9V03X_H], leftline[MT9V03X_H];//rightline�����ߣ�leftline������
uint8 road_width[MT9V03X_H], centerline[MT9V03X_H];//road_width����·��ȣ�centerline����·����
uint8 bin_image[MT9V03X_H][MT9V03X_W];//ͼ������
uint8 perspectiveImage[MT9V03X_H][MT9V03X_W];//��͸��֮������

int16   Left_Add_Start, Right_Add_Start;  // ���Ҳ�����ʼ������
int16   Left_Add_Stop, Right_Add_Stop;    // ���Ҳ��߽���������

// TODO:1.�ڼ�������ʱ����������ȣ���������С��ȵ�Road_Width_Min(ǰ��������͸��)
//      2.�޸�ROW ��  COL
int16   Left_Add_Line[ROW+2], Right_Add_Line[ROW+2];        // ���ұ߽粹������
int16   Left_Add_Flag[ROW+2] , Right_Add_Flag[ROW+2];       // ���ұ߽粹�߱�־λ
// TODO:road_width_add�漰���ദ���㣬�ǵò���
int16   Road_Width_Add[ROW+2];  // �����������
int16   diu_hang_num;       // ������������⻷����
int16   Line_Count;         // ��¼�ɹ�ʶ�𵽵���������
float   Left_Ka = 0, Right_Ka = 0;
float   Left_Kb = 1, Right_Kb = COL-1;  // ��С���˷�����
int16   Is_Right_Line[62] = {0},Is_Left_Line[62] = {0};//�Աȵ���ʱ���飬��⻷��ʱ��
int16   island_flag = 0;   //��־Ϊ0���ǻ���   ��־Ϊ1���󻷵�   ��־Ϊ2���һ���
int16   island_left_mid_add_flag = 0,island_right_mid_add_flag = 0;//���һ����������߱�־λ
int16   Road_Width_Min;     // ��С�������

Lost_Edgeline Lost_leftline, Lost_rightline;

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


void turn_to_bin(uint8 raw_image[MT9V03X_H][MT9V03X_W], uint8 image_w, uint8 image_h)
{
    uint8 i,j;
    int thereshold = otsuThreshold(raw_image[0], image_w, image_h);
    for(i = 0;i<image_h;i++)
    {
        for(j = 0;j<image_w;j++)
        {
            if(raw_image[i][j]>thereshold)bin_image[i][j] = white;
            else bin_image[i][j] = black;
        }
    }
}

uint8 mid=COL/2;//���������һ��ͼ������ɨ�����
void findline(uint8 image[ROW][COL])
{
    // ������Ϣ��ʼ��
    Lost_leftline.start.x = -1;
    Lost_leftline.start.y = -1;
    Lost_rightline.start.x = -1;
    Lost_rightline.start.y = -1;
    Lost_leftline.end.x = -1;
    Lost_leftline.end.y = -1;
    Lost_rightline.end.x = -1;
    Lost_rightline.end.y = -1;
    Lost_leftline.lost_time = 0;
    Lost_rightline.lost_time = 0;

    //�����������
    for(int col=mid;col>=0;col--)
    {
        if(col==0)
        {
            // ����
            leftline[ROW-1] = 0;
            Lost_leftline.start.x = -1;
            Lost_leftline.start.y = ROW-1;
            Lost_leftline.lost_time++;
            break;
        }
        else if(image[ROW-1][col]==black && image[ROW-1][col+1]==white && image[ROW-1][col+2]==white)
        {
            leftline[ROW-1] = col;
            break;
        }
    }
    //�������ұ���
    for(int col=mid;col<=COL-1;col++)
    {
        if(col==COL-1)
        {
            // ����
            rightline[ROW-1] = COL-1;
            Lost_rightline.start.x = -1;
            Lost_rightline.start.y = ROW-1;
            Lost_rightline.lost_time++;
            break;
        }
        else if(image[ROW-1][col]==black && image[ROW-1][col-1]==white && image[ROW-1][col-2]==white)
        {
            rightline[ROW-1] = col;
            break;
        }
    }
    centerline[ROW-1] = (leftline[ROW-1]+rightline[ROW-1])>>1;
    mid = centerline[ROW-1];

    //������
    for(int row=ROW-2;row>0;row--)
    {
        for(int col=centerline[row-1];col>=0;col--)
        {
            if(col==0)
            {
                //����
                leftline[row] = 0;
                if(Lost_leftline.start.x == -1)// �����һ�ζ��ߣ���һ�еı��ߵ㼴Ϊ�ǵ�
                {
                    Lost_leftline.start.x = leftline[row-1];
                    Lost_leftline.start.y = row-1;
                }
                else// ���ǵ�һ�ζ��ߣ���Ӷ�������
                    Lost_leftline.lost_time++;
                break;
            }
            else if(image[row][col]==black && image[row][col+1]==white && image[row][col+2]==white)
            {
                leftline[row] = col;
                if(Lost_leftline.start.x != -1 && Lost_leftline.end.x == -1)//���ߺ��һ���ҵ���
                {
                    Lost_leftline.end.x = leftline[row];
                    Lost_leftline.end.y = row;
                }
                break;
            }
        }
        for(int col=centerline[row-1];col<=COL-1;col++)
        {
            if(col==COL-1)
            {
                //����
                rightline[row] = COL-1;
                if(Lost_rightline.start.x == -1)// �����һ�ζ��ߣ���һ�еı��ߵ㼴Ϊ�ǵ�
                {
                    Lost_rightline.start.x = rightline[row-1];
                    Lost_rightline.start.y = row-1;
                }
                else//���ǵ�һ�ζ��ߣ���Ӷ�������
                    Lost_rightline.lost_time++;
                break;
            }
            else if(image[row][col]==black && image[row][col-1]==white && image[row][col-2]==white)
            {
                rightline[row] = col;
                if(Lost_rightline.start.x != -1 && Lost_rightline.end.x == -1)//���ߺ��һ���ҵ���
                {
                    Lost_rightline.end.x = rightline[row];
                    Lost_rightline.end.y = row;
                }
                break;
            }
        }
        centerline[row] = (leftline[row] + rightline[row])>>1;
//        printf("%d---%d---%d\n", leftline[row], centerline[row], rightline[row]);
    }
//    printf("-----------------\n");
}

// �������ɷ��������ұ���
// ******���������index:0ָ��ͼ��������******
void BinThreshold(unsigned char imageIn[MT9V03X_H][MT9V03X_W])
{
    // ������Ϣ��ʼ��
    Lost_leftline.start.x = -1;
    Lost_leftline.start.y = -1;
    Lost_rightline.start.x = -1;
    Lost_rightline.start.y = -1;
    Lost_leftline.end.x = -1;
    Lost_leftline.end.y = -1;
    Lost_rightline.end.x = -1;
    Lost_rightline.end.y = -1;
    Lost_leftline.lost_time = 0;
    Lost_rightline.lost_time = 0;


    //***********�������ɷ�**************
    //����һ��ɨ��
    uint8 mid=MT9V03X_W/2;
    for(int i=mid-1;0<i;i--){
        if(i==1){
            leftline[0] = 0;
            Lost_leftline.start.x = -1;
            Lost_leftline.start.y = ROW-1;
            Lost_leftline.lost_time++;
            break;
        }else if(abs(bin_image[MT9V03X_H-1][i]-bin_image[MT9V03X_H-1][i-1])==white){
            leftline[0]=i;
            break;
        }
    }
    for(int j=mid;j<MT9V03X_W-1;j++){
        if(j==MT9V03X_W-2){
            rightline[0] = MT9V03X_W-1;
            Lost_rightline.start.x = -1;
            Lost_rightline.start.y = ROW-1;
            Lost_rightline.lost_time++;
            break;
        }else if(abs(bin_image[MT9V03X_H-1][j]-bin_image[MT9V03X_H-1][j+1])==white){
            rightline[0]=j;
            break;
        }
    }

    // ����ɨ�����
    mid = (leftline[0]+rightline[0])>>1;

    //������Ѳ�ߣ�
    for(int i=0;i<MT9V03X_H-1;i++){
        uint8 left=leftline[i];
        uint8 right=rightline[i];
        uint8 next1=i+1;  //����������������
        uint8 next2=MT9V03X_H-1-next1;  //����ͼ��������

        //����
        uint8 isLeftDiuXian=0; //0��ʾ���ߣ��������ֱ�ʾû�ж���
        for(int j=0;j<50&&isLeftDiuXian==0;j++){
            if(left+j+1<MT9V03X_W&&abs(imageIn[next2][left+j]-imageIn[next2][left+j+1])==white){
                leftline[next1]=left+j;
                isLeftDiuXian=114;
                if(Lost_leftline.start.x != -1 && Lost_leftline.end.x == -1)//���ߺ��һ���ҵ���
                {
                    Lost_leftline.end.x = leftline[next1];
                    Lost_leftline.end.y = next1;
                }
            }
            if(left-j-1>=0&&abs(imageIn[next2][left-j-1]-imageIn[next2][left-j])==white){
                leftline[next1]=left-j;
                isLeftDiuXian=114;
                if(Lost_leftline.start.x != -1 && Lost_leftline.end.x == -1)//���ߺ��һ���ҵ���
                {
                    Lost_leftline.end.x = leftline[next1];
                    Lost_leftline.end.y = next1;
                }
            }
        }
        //������
        if(isLeftDiuXian==0){
            leftline[next1]=0;
            if(Lost_leftline.start.x ==0)// �����һ�ζ��ߣ���һ�еı��ߵ㼴Ϊ�ǵ�
            {
                Lost_leftline.start.x = leftline[i];// TODO:�ҿ������ҵ��� ָ�ҵ���next1�е��ߣ����������Ūnext1ǰһ�У�����i��
                Lost_leftline.start.y = i;
            }
            else// ���ǵ�һ�ζ��ߣ���Ӷ�������
                Lost_leftline.lost_time++;
        }


        //����
        uint8 isRightDiuXian=0; //0��ʾ���ߣ��������ֱ�ʾû�ж���
        for(int j=0;j<50&&isRightDiuXian==0;j++){
            if(right+j+1<MT9V03X_W&&abs(imageIn[next2][right+j]-imageIn[next2][right+j+1])==white){
                rightline[next1]=right+j;
                isRightDiuXian=114;
                if(Lost_rightline.start.x != -1 && Lost_rightline.end.x == -1)//���ߺ��һ���ҵ���
                {
                    Lost_rightline.end.x = rightline[next1];
                    Lost_rightline.end.y = next1;
                }
            }
            if(right-j-1>=0&&abs(imageIn[next2][right-j-1]-imageIn[next2][right-j])==white){
                rightline[next1]=right-j;
                isRightDiuXian=114;
                if(Lost_rightline.start.x != -1 && Lost_rightline.end.x == -1)//���ߺ��һ���ҵ���
                {
                    Lost_rightline.end.x = rightline[next1];
                    Lost_rightline.end.y = next1;
                }
            }
        }
        //�����ߣ�������ߣ���˵����ǰ�ǻ�������ʮ��·��
        // TODO:������������ߣ���ֻ��ע��һ�ζ���
        if(isRightDiuXian==0){
            rightline[next1]=MT9V03X_W-1;
            if(Lost_leftline.start.x ==0)// �����һ�ζ��ߣ���һ�еı��ߵ㼴Ϊ�ǵ�
            {
                Lost_leftline.start.x = leftline[i];// TODO:�ҿ������ҵ��� ָ�ҵ���next1�е��ߣ����������Ūnext1ǰһ�У�����i��
                Lost_leftline.start.y = i;
            }
            else// ���ǵ�һ�ζ��ߣ���Ӷ�������
                Lost_leftline.lost_time++;
        }
        centerline[next1] = (leftline[next1]+rightline[next1])>>1;
        //����Ӧ���е�·״��ʶ�𣬵��Ҳ���д/doge
        //
        //1145141919810
        //
        //��·ʶ�����
    }
    //***********�������ɷ�����**************
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

// ������
void Draw_Side()
{
//    uint8 y;
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

void ImagePerspective_Init(uint8 BinImage[MT9V03X_H][MT9V03X_W], uint8 ResultImage[MT9V03X_H][MT9V03X_W])
{
//    float change_un_Mat[3][3] ={{-0.352043,0.324030,-23.060735},{0.012529,0.058598,-30.547456},{0.000205,0.003490,-0.632328}};
    float change_un_Mat[3][3] ={{-0.234447,0.227495,-17.207843},{0.009790,0.019755,-16.513155},{0.000104,0.002306,-0.410462}};
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
                ResultImage[j][i] = BinImage[local_y][local_x];
            }
            else {
                ResultImage[j][i] = black;
            }

        }
    }

}

uint8 JudgeLeftorRight(){
    uint8 mid0=(uint8)(leftline[0]+rightline[0])/2;
    uint8 mid24=(uint8)(leftline[24]+rightline[24])/2;
    if(mid24-mid0>0){
        return 1;  //����
    }else{
        return -1;  //����
    }
}

void FindLineFromOneSide(uint8 turn){
    if(turn==1){  //����
        for(int i=0;i<MT9V03X_H;i++){
            centerline[i]=rightline[i]-26;
            if(centerline[i]<0){
                centerline[i]=0;
            }
        }
    }else{
        for(int i=0;i<MT9V03X_H;i++){
            centerline[i]=leftline[i]+26;
            if(centerline[i]>=MT9V03X_W){
                centerline[i]=MT9V03X_W-1;
            }
        }
    }

}

/******************************************************************************
 *
//�������   CommonRectificate(&P_LeftBlack[0],startPos-1,endPos+1);
******************************************************************************/
void CommonRectificate(unsigned char data[],unsigned char begin,unsigned char end)
{
       unsigned char MidPos = 0;
       if (end > ROW-1)
       {
          end = ROW-1;
       }
       if (begin == end)
       {
          data[begin] = (data[begin-1]+data[begin+1])>>1;
         // BlackLineData[begin] =  LeftBlack[begin] + (RightBlack[begin]-LeftBlack[begin])/2;
       }
       else if(begin < end)
       {
          MidPos = (begin+end)>>1;
          data[MidPos] = (data[begin]+data[end])>>1;
          //BlackLineData[MidPos] =  LeftBlack[MidPos] + (RightBlack[MidPos]-LeftBlack[MidPos])/2;
          if (begin+1 < MidPos)
          {
             CommonRectificate(data,begin,MidPos);
          }
          if (MidPos+1 < end)
          {
             CommonRectificate(data,MidPos,end);
          }
       }
}

void Image_Handle()
{
    // ��ʼ��ʱ
//       timer_start(TIM_7);


    if(mt9v03x_finish_flag)
    {
        turn_to_bin(mt9v03x_image, MT9V03X_W, MT9V03X_H);
        ImagePerspective_Init(bin_image, perspectiveImage);
        ips200_displayimage03x((const uint8 *)perspectiveImage, MT9V03X_W, MT9V03X_H);
//        bluetooth_ch9141_send_image((const uint8 *)perspectiveImage, MT9V03X_IMAGE_SIZE);
//        camera_send_image(DEBUG_UART_INDEX, (const uint8 *)bin_image, MT9V03X_IMAGE_SIZE);
//        BinThreshold(perspectiveImage);
        findline(perspectiveImage);
//        CommonRectificate(leftline, Lost_leftline.start.y, Lost_leftline.end.y);
//        CommonRectificate(leftline, Lost_rightline.start.y, Lost_rightline.end.y);
//        FindLineFromOneSide(JudgeLeftorRight());

        Draw_Side();
        mt9v03x_finish_flag = 0;
    }

    // ������ʱ
//       timer_stop(TIM_7);
//       ips200_show_string(0, 240, "time:");
//       ips200_show_int(60, 240, timer_get(TIM_7), 8);
//       timer_clear(TIM_7);
}
