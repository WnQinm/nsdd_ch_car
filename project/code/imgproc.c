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

// �������ɷ��������ұ���
void BinThreshold(unsigned char imageIn[MT9V03X_H][MT9V03X_W])
{
    //***********�������ɷ�**************
    //����һ��ɨ��
    uint8 mid=MT9V03X_W/2;
//    memset(leftline,0,sizeof(uint8));
//    memset(rightline,0,sizeof(uint8));
//    memset(centerline,0,sizeof(uint8));
//    memset(road_width,0,sizeof(uint8));
//    memset(isRightLineDiuXian,0,sizeof(uint8));
//    memset(isLeftLineDiuXian,0,sizeof(uint8));
//    memset(road_width,0,sizeof(uint8));
    for(int i=mid-1;0<i;i--){
        if(i==1){
            leftline[0] = 0;
//            isLeftLineDiuXian[0]=1;
            break;
        }else if(abs(bin_image[MT9V03X_H-1][i]-bin_image[MT9V03X_H-1][i-1])==white){
            leftline[0]=i;
            break;
        }
    }
    for(int j=mid;j<MT9V03X_W-1;j++){
        if(j==MT9V03X_W-2){
            rightline[0] = MT9V03X_W-1;
//            isRightLineDiuXian[0]=1;
            break;
        }else if(abs(bin_image[MT9V03X_H-1][j]-bin_image[MT9V03X_H-1][j+1])==white){
            rightline[0]=j;
            break;
        }
    }



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
            }
            if(left-j-1>=0&&abs(imageIn[next2][left-j-1]-imageIn[next2][left-j])==white){
                leftline[next1]=left-j;
                isLeftDiuXian=114;
            }
        }
        //�����ߣ�������ߣ���˵����ǰ�ǻ�������ʮ��·��
        if(isLeftDiuXian==0){
//            isLeftLineDiuXian[next1]=1;
            leftline[next1]=0;
        }


        //����
        uint8 isRightDiuXian=0; //0��ʾ���ߣ��������ֱ�ʾû�ж���
        for(int j=0;j<50&&isRightDiuXian==0;j++){
            if(right+j+1<MT9V03X_W&&abs(imageIn[next2][right+j]-imageIn[next2][right+j+1])==white){
                rightline[next1]=right+j;
                isRightDiuXian=114;
            }
            if(right-j-1>=0&&abs(imageIn[next2][right-j-1]-imageIn[next2][right-j])==white){
                rightline[next1]=right-j;
                isRightDiuXian=114;
            }
        }
        //�����ߣ�������ߣ���˵����ǰ�ǻ�������ʮ��·��
        if(isRightDiuXian==0){
//            isRightLineDiuXian[next1]=1;
            rightline[next1]=MT9V03X_W-1;
        }

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

void adaptiveThreshold(uint8_t* img_data, uint8_t* output_data, int width, int height, int block, uint8_t clip_value){
    int half_block = block / 2;
    for(int y=half_block; y<height-half_block; y++){
        for(int x=half_block; x<width-half_block; x++){
        // ����ֲ���ֵ
        int thres = 0;
        for(int dy=-half_block; dy<=half_block; dy++){
            for(int dx=-half_block; dx<=half_block; dx++){
            thres += img_data[(x+dx)+(y+dy)*width];
          }
        }
        thres = thres / (block * block) - clip_value;
        // ���ж�ֵ��
        output_data[x+y*width] = img_data[x+y*width]>thres ? 255 : 0;
        }
    }
}

// ������
void Draw_Side()
{
    for(uint8 row = 0; row<MT9V03X_H; row++)
    {
        row=MT9V03X_H-1-row;
        ips200_draw_point(leftline[row], row, RGB565_BLUE);
        ips200_draw_point(rightline[row], row, RGB565_BLUE);
        ips200_draw_point(centerline[row], row, RGB565_RED);
    }
}

#define RESULT_ROW 120//���ͼ����
#define RESULT_COL 188

void ImagePerspective_Init(uint8 BinImage[MT9V03X_H][MT9V03X_W], uint8 ResultImage[MT9V03X_H][MT9V03X_W])
{
    float change_un_Mat[3][3] ={{-0.352043,0.324030,-23.060735},{0.012529,0.058598,-30.547456},{0.000205,0.003490,-0.632328}};
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

/*-------------------------------------------------------------------------------------------------------------------
����������
˵������ʼ���ղ�ʹ�ã�ֱ��ʹ������б�ʽ��в��ߣ��ܽ��˺����ض���ʼ����
            start������ʼ��ָ��ͻ����             stop������ֹ��ָ��          data��ȡ��ͼ��Imagel
            Line_AddΪLeft_Line_Add��Right_Line_Add         Mode 1Ϊ����2Ϊ�Ҳ���
-------------------------------------------------------------------------------------------------------------------*/
void Line_Repair(uint8 Start, uint8 Stop, int16 *Line_Add, int16 Mode)
{
    float res;              //��ʱ�����������޲����
    int16 i;                    //������
    float Ka, Kb;           //��ʱ����

    if ((Mode == 1) && (Right_Add_Start <= Stop) && Start <= 53)            //��߽粹�ߣ�53��֮��
    {/*Right_Add_Start <= Stop��ʾ�ڴ���֮ǰ���ұ߽�ɨ�趼��������ڣ�����Ҫ����*/
        for (i = Start; i >= Stop+2; i -= 2)
        {
            Line_Add[i] = Limit_Protect(Right_Add_Line[i] - Road_Width_Add[i+2], 1, COL-1); //Right_Add_Line��Right_Line��ֵ��ʵӦ����һ���ģ��ұ߽��ȥ������ȵ�����߽�
            Road_Width_Add[i] = Road_Width_Add[i+2];                                                                                //��������������Ӧ�û��ֱ��б�ʲ�������һ��
        }
    }
    else if ((Mode == 2) && (Left_Add_Start <= Stop) && Start <= 53)    //�ұ߽粹�ߣ�53��֮��
    {/*Left_Add_Start <= Stop��ʾ�ڴ���֮ǰ����߽�ɨ�趼��������ڣ�����Ҫ����*/
        for (i = Start; i >= Stop+2; i -= 2)
        {
            Line_Add[i] = Limit_Protect(Left_Add_Line[i] + Road_Width_Add[i+2], 1, COL-1);  //Left_Add_Line��Left_Line��ֵ��ʵӦ����һ���ģ���߽����������ȵ�����߽�
            Road_Width_Add[i] = Road_Width_Add[i+2];                                                                                //��������������Ӧ�û��ֱ��б�ʲ�������һ��
        }
    }
    else
    {
        if (Stop)                                                                                   //��ʼ����
        {
            if(Mode == 1)                                                                       //��߽粹��
                for(i=Stop+2; i<=Stop-4; i-=2)                              //Ѱ����߽����͹����
                {/*ΪʲôҪ��͹���㣬��Ϊͼ��߽�����б�ģ���һ����߽�ĵ�ܿ���ʮ�ֿ���ͼ������࣬ʵ��ͼ��һ�¾�������*/
                    if(leftline[i]-leftline[i+2] >= 4 )               //����������������߽�����Ĳ�ֵ�������ֵ̫��֤������㲻�ʺϲ���
                         Stop = i ;                                                             //����Ϊ͹�����Ϊֹͣ�����У������������ı߽��߸�����ʵ��
                }
            else if(Mode == 2)                                                          //�ұ߽粹��
                for(i=Stop+2; i<=Stop-4; i-=2)                              //Ѱ���ұ߽����͹����
                {/*ΪʲôҪ��͹���㣬��Ϊͼ��߽�����б�ģ���һ���ұ߽�ĵ�ܿ���ʮ�ֿ���ͼ�����Ҳ࣬ʵ��ͼ��һ�¾�������*/
                    if(rightline[i+2]-rightline[i] >= 4 )         //���������������ұ߽�����Ĳ�ֵ�������ֵ̫��֤������㲻�ʺϲ���
                        Stop = i ;                                                              //����Ϊ͹�����Ϊֹͣ�����У������������ı߽��߸�����ʵ��
                }
            if ((Right_Add_Stop >= MIDVALUE && Left_Add_Stop   >= MIDVALUE)       || \
                    (Right_Add_Stop >= MIDVALUE && Left_Add_Start  <= Right_Add_Stop) || \
                    (Left_Add_Stop  >= MIDVALUE && Right_Add_Start <= Left_Add_Stop))   //ֻ�н�������Ҫ���ߣ�������б�ʣ�ֱ����ֱ���²���
            {
                for (i = Stop-2; i <= 57; )                                                                                 //��ֹͣ���������57�п�ʼ����
                {
                    i += 2;
                    Line_Add[i] = Line_Add[Stop];                                                                           //��ֱ���²��ߣ����ü���б��
                }
            }
            else                                                                                                                                    //����ʼ�кͽ����м���б�ʲ���
            {
                Ka = 1.0*(Line_Add[Start] - Line_Add[Stop]) / (Start - Stop);               //���㷨���б��Ka
                Kb = 1.0*Line_Add[Start] - (Ka * Start);                                                        //������ʼ�㣬�������Kb
                for (i = Stop+2; i < Start; i += 2)                                                                 //��ֹͣ�е���ʼ��
                {
                    res = i * Ka + Kb;                                                                                              //����Ka,Kb���㶪���е�����
                    Line_Add[i] = Limit_Protect((int32)res, 1, COL-1);                              //�ж��Ƿ񳬳�ͼ������
                }
            }
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
������Ѱ���󻷵�����
˵��������ͼ�������һֱ����ɨ��
-------------------------------------------------------------------------------------------------------------------*/
void Fand_Left_Ring(void)
{
    int i = 0;                                          //����ѭ��
    int count_flag = 0;                         //ÿ��ɨ��ǰ��Ҫ���㣬count_flag��ʾ����ұ߽�������ϳ̶ȽϺõĵ������
    diu_hang_num = 0;                           //ÿ��ɨ��ǰ��Ҫ���㣬diu_hang_num��ʾ�������ж��ߵ���Զ�����ڵ������������꣩
    if (Line_Count <= 25)                       //��Ч��С��25�У���ʾ��������������ʼɨ��
    {
            for(i=45; i>=25; i-=2)          //��25�е�45��֮��ɨ����߽�
            {
                    if (leftline[i]==1&&leftline[i+2]==1&&leftline[i+4]==1&&leftline[i+6]==1)       //��������ж��߿�������Բ��
                    {
                            diu_hang_num = i;       //�����������ж��ߵ���Զ��������������
                            break;                          //�ҵ��˳�ѭ��
                    }
            }
            if (diu_hang_num > 23)          //�������ж��ߵ���Զ������������������23��
            {
            /*�ұ߽�û���ߣ�ֻ�ж��ߵ�ʱ�����93���ұ�31�к�51�в���ȣ���֤��б�ʣ�����߽����ǰ����û���ߣ�ֻ�ж��ߵ�ʱ�����1*/
                    if (rightline[51] != 93 && rightline[31] != 93 && rightline[31] < rightline[51] && \
                            rightline[19] != 93 && rightline[19] <= rightline[31] && \
                            leftline[19] != 1 &&  leftline[21] != 1 && leftline[57] != 1 && leftline[55] != 1 )
                    {
                            for(i=55; i>19; i-=2)
                            {
                                if(rightline[i] != 93)                                         //�Ҳ�55��19��û����
                                    if(rightline[i+2] < rightline[i])             //����Զ���еĺ�����С�ڽ����У���ʾɨ����������
                                        return ;                                                                //�ұ߽粻�������޷����к����������ؽ���
                            }
                            Curve3_Fitting(&Right_Ka, &Right_Kb, 51,31, rightline, 2);                 //����ұ߽��б��
                            /*��Ҫ���ߵ��Բ����޸�Ka��Kb�ķ�Χ��֤����ʶ��Բ��*/
                            if(Right_Ka>=0.3f && Right_Ka <= 0.65f)                                                         //�ж���ϵ�Ka�Ƿ���������Χ����ʾ�����������������
                            {
                                if(Right_Kb >= 30 && Right_Kb <= 65)                                                            //�ж���ϵ�Kb�Ƿ���������Χ����ʾ�����������������
                                {
                                    for(int aaaaa = 25; aaaaa <= 57; aaaaa += 2)
                                    {
                                        Is_Right_Line[aaaaa]  = Fit_Point(aaaaa, Right_Ka, Right_Kb);   //����Ͻ������Բ��ר����������
                                        if(rightline[aaaaa] <= (Is_Right_Line[aaaaa]+1) && rightline[aaaaa] >= (Is_Right_Line[aaaaa]-1))//��Ͻ���Ϻã��ұ߽�����
                                        {
                                            if(++count_flag == 15)                                                                          //����15�������û����
                                            {
                                                island_flag = 1;                                                                                    //������־λ��1��ʾ��Բ��
                                                island_left_mid_add_flag = 1;                                                           //��Բ�����ߴ����־λ��1��ͼ������֮�󣬰�����Բ�����������������
                                                Go_Left_Island();                                                                                   //�����󻷵���PID
                                            }
                                        }
                                    }
                                }
                            }

                    }
            }
    }
}

/*---------------------------------
 * Ԥ��ͼ����������
 */
void Image_Handle()
{
    int16 i;                                // ������
    int16 j;                                // ���ڶ���ѭ��
    int16 res;                          // ���ڽ��״̬�ж�
    Line_Count      = 0;        // ����������λ
    Left_Add_Start  = 0;        // ��λ������ʼ������
    Right_Add_Start = 0;        // ��λ�Ҳ�����ʼ������
    Left_Add_Stop   = 0;        // ��λ��������ֹ����
    Right_Add_Stop  = 0;        // ��λ�Ҳ�������ֹ����

    /***************************** TODO:�����һ֡ͼ������� *****************************/
//    for (i = ROW-3; i >= 19;i-=2)           //��ʼ���߽�ֵ����һ�л����⴦������һ������ROW-3
//    {
//            Right_Line[i]     = COL-1;      //�����߽��ʼֵ
//            Left_Line[i]      = 1;              //�����߽��ʼֵ
//            Left_Add_Flag[i]  = 1;              //��ʼ�����߱�־λ��Ҫ����Ϊ1������Ҫ����Ϊ0
//            Right_Add_Flag[i] = 1;
//    }

    for(i=ROW-3;i>=19;i-=2)
    {
        // TODO:�ǵ�д��һ�е����ݴ�������right_add_line/left_add_line�ȵ�
        /**************************** ���߼�⿪ʼ ****************************/
        // TODO:road_width_min+1�е������ˣ�������ʮ�����õ���͸�ӣ��о�Ҫ�Ĵ�Щ
        if (road_width[i] > Road_Width_Min+1)                      //������ȱ��������ʮ�ֻ�·
        {
            if (Right_Add_Line[i] >= Right_Add_Line[i+2])          //�ұ߽��һ�б�ǰһ�д��ˣ��϶�������
                if (!Right_Add_Flag[i])                            //����ҵ��߽���
                    Right_Add_Flag[i] = 1;                         //ǿ���϶�Ϊ��Ҫ����
            if (Left_Add_Line[i] <=  Left_Add_Line[i+2])           //�ұ߽��һ�б�ǰһ�д��ˣ��϶�������
                if (!Left_Add_Flag[i])                             //����ҵ��߽���
                    Left_Add_Flag[i] = 1;                          //ǿ���϶�Ϊ��Ҫ����
            if (Left_Add_Flag[i] || Right_Add_Flag[i])             //�����Ҫ����
                if (Left_Add_Stop  || Right_Add_Stop)              //����ǰ���Ѿ��������ˣ�
                    break;                                                                                                              //��ֱ�ӽ����ˣ��������ˣ�������ûɶ��
        }

        /*************************** ��һ�ֲ��߿�ʼ ***************************/
        if (Left_Add_Flag[i])                                       //�����Ҫ����
        {
            if (i >= ROW-5)                                            //ǰ���в��߲��� //TODO:����������е��ԣ���дrow-5��
            {
                if (!Left_Add_Start)                                //����ǲ��ǵ�һ�β���
                {
                    Left_Add_Start = i;                             //��¼��ಹ�߿�ʼ��
                    for(j=ROW-1; j>=i; j-=2)                //Ѱ����߽����͹����
                        if(leftline[j] > leftline[Left_Add_Start] )
                            Left_Add_Start = j;                     //����͹����
                    Left_Ka = 0;                                            //Ka����0
                    Left_Kb = Left_Add_Line[Left_Add_Start];                    //Kb���ڴ��к����꣬��͹������ֱ����
                }
                Left_Add_Line[i] =  Fit_Point(i, Left_Ka, Left_Kb); //ʹ��ǰһ֡ͼ����߽�б�ʲ��ߣ�Ka=0��ֱ�Ӳ�Kb�������ǵ�����
            }
            else                                                                    //�������ǰ����
            {
                if (!Left_Add_Start)                                //����ǲ��ǵ�һ�β���
                {
                    Left_Add_Start = i;                             //��¼��ಹ�߿�ʼ��
                    Curve1_Fitting(&Left_Ka, &Left_Kb, &Left_Add_Start, Left_Add_Line, 1);  /*Left_Add_Start��ֵ��ָ�������ݵ���һ��*///���ֱ��Ka,Kb��ֵ�����
                }
                Left_Add_Line[i] = Fit_Point(i, Left_Ka, Left_Kb);  //������ɣ�����ǰ���е�б�ʲ�һ��
            }
        }
        else                                                                        //������಻��Ҫ��Ҫ������
        {
            if (Left_Add_Start)                                     //Left_Add_Start��Ϊ0���ͱ�ʾ�Ѿ���ʼ������
                if (!Left_Add_Stop && !Left_Add_Flag[i+2] && !Left_Add_Flag[i+4])                                                       //�Ƿ��Ѿ�ֹͣ���Ƿ���������û�ж��ߣ�����Ҫ����
                    if (Left_Add_Line[i] >= Left_Add_Line[i+2] && Left_Add_Line[i+2] >= Left_Add_Line[i+4] && Right_Add_Start < 55)     //��߽��Ƿ�Զ���Ƚ���������Ҫ�󣬲��󣬾�������������
                    {
                         Left_Add_Stop = i+4;                                                                                                                                       //��¼��ಹ�߽����У���Ϊ��������û�����ˣ����Բ���ȥ�������л��ǲ���Ҫ����
                         Line_Repair(Left_Add_Start, Left_Add_Stop, Left_Add_Line, 1);                                        //���²��߷�Χ�ڵ���߽�
                    }
        }
        if (Right_Add_Flag[i])                                  //�Ҳ���Ҫ����
        {
            if (i >= 55)                                                    //ǰ���в��߲���
            {
                if (!Right_Add_Start)
                {
                    Right_Add_Start = i;                            //��¼���߿�ʼ��
                    for(j=ROW-1; j>=i; j-=2)                //Ѱ���ұ߽����͹����
                        if(rightline[j] < rightline[Right_Add_Start] )
                            Right_Add_Start = j;                    //����͹����
                    Right_Ka = 0;                                           //Ka����0
                    Right_Kb = Right_Add_Line[Right_Add_Start];                     //Kb���ڴ��к����꣬��͹������ֱ����
                }
                    Right_Add_Line[i] = Fit_Point(i, Right_Ka, Right_Kb);   //ʹ��ǰһ֡ͼ����߽�б�ʲ��ߣ�Ka=0��ֱ�Ӳ�Kb�������ǵ�����
            }
            else
            {
                if (!Right_Add_Start)                               //����ǲ��ǵ�һ�β���
                {
                    Right_Add_Start = i;                            //��¼�Ҳಹ�߿�ʼ��
                    Curve1_Fitting(&Right_Ka, &Right_Kb, &Right_Add_Start, Right_Add_Line, 2);  /*Right_Add_Start��ֵ��ָ�������ݵ���һ��*///���ֱ��Ka,Kb��ֵ�����
                }
                    Right_Add_Line[i] = Fit_Point(i, Right_Ka, Right_Kb);   //������ɣ�����ǰ���е�б�ʲ�һ��
            }
        }
        else
        {
            if (Right_Add_Start)                                    //Right_Add_Start��Ϊ0���ͱ�ʾ�Ѿ���ʼ������
            {
                if (!Right_Add_Stop && !Right_Add_Flag[i+2] && !Right_Add_Flag[i+4])                                                                    //�Ƿ��Ѿ�ֹͣ���Ƿ���������û�ж��ߣ�����Ҫ����
                {
                    if (rightline[i] <= rightline[i+2] && rightline[i+2] <= rightline[i+4]&& Left_Add_Start < 55)   //�ұ߽��Ƿ�Զ���Ƚ���������ҪС����С����������������
                    {
                        Right_Add_Stop = i+4;                                                                                                                                                           //��¼�Ҳಹ�߽����У���Ϊ��������û�����ˣ����Բ���ȥ�������л��ǲ���Ҫ����
                        Line_Repair(Right_Add_Start, Right_Add_Stop, Right_Add_Line, 2);                                                      //���²��߷�Χ�ڵ��ұ߽�
                    }
                }
            }
        }
        /*************************** ��һ�ֲ��߽��� ***************************/
    }
}


