/*
 * imgproc.c
 *
 *  Created on: 2023年4月8日
 *      Author: 清梦，野兽先辈
 */

#include "imgproc.h"

uint8 rightline[MT9V03X_H], leftline[MT9V03X_H];//rightline：右线，leftline：左线
uint8 road_width[MT9V03X_H], centerline[MT9V03X_H];//road_width：道路宽度，centerline：道路中线
uint8 bin_image[MT9V03X_H][MT9V03X_W];//图像数组
uint8 perspectiveImage[MT9V03X_H][MT9V03X_W];//逆透视之后数组

int16   Left_Add_Start, Right_Add_Start;  // 左右补线起始行坐标
int16   Left_Add_Stop, Right_Add_Stop;    // 左右补线结束行坐标

// TODO:1.在计算中线时计算赛道宽度，并保留最小宽度到Road_Width_Min(前提是做逆透视)
//      2.修改ROW 和  COL
int16   Left_Add_Line[ROW+2], Right_Add_Line[ROW+2];        // 左右边界补线数据
int16   Left_Add_Flag[ROW+2] , Right_Add_Flag[ROW+2];       // 左右边界补线标志位
// TODO:road_width_add涉及到多处计算，记得补上
int16   Road_Width_Add[ROW+2];  // 补线赛道宽度
int16   diu_hang_num;       // 丢线行数，检测环岛用
int16   Line_Count;         // 记录成功识别到的赛道行数
float   Left_Ka = 0, Right_Ka = 0;
float   Left_Kb = 1, Right_Kb = COL-1;  // 最小二乘法参数
int16   Is_Right_Line[62] = {0},Is_Left_Line[62] = {0};//对比的临时数组，检测环岛时用
int16   island_flag = 0;   //标志为0不是环岛   标志为1是左环岛   标志为2是右环岛
int16   island_left_mid_add_flag = 0,island_right_mid_add_flag = 0;//左右环岛进出补线标志位
int16   Road_Width_Min;     // 最小赛道宽度

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
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // 类间方差;
    uint8 MinValue=0, MaxValue=0;
    uint8 Threshold = 0;


    for (Y = 0; Y <Image_Height; Y++) //Y<Image_Height改为Y =Image_Height；以便进行 行二值化
    {
        //Y=Image_Height;
        for (X = 0; X < Image_Width; X++)
        {
        HistGram[(int)data[Y*Image_Width + X]]++; //统计每个灰度值的个数信息
        }
    }
    for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值

    if (MaxValue == MinValue)
    {
        return MaxValue;          // 图像中只有一个颜色
    }
    if (MinValue + 1 == MaxValue)
    {
        return MinValue;      // 图像中只有二个颜色
    }

    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        Amount += HistGram[Y];        //  像素总数
    }

    PixelIntegral = 0;
    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        PixelIntegral += HistGram[Y] * Y;//灰度值总数
    }
    SigmaB = -1;
    for (Y = MinValue; Y < MaxValue; Y++)
    {
          PixelBack = PixelBack + HistGram[Y];    //前景像素点数
          PixelFore = Amount - PixelBack;         //背景像素点数
          OmegaBack = (double)PixelBack / Amount;//前景像素百分比
          OmegaFore = (double)PixelFore / Amount;//背景像素百分比
          PixelIntegralBack += HistGram[Y] * Y;  //前景灰度值
          PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
          MicroBack = (double)PixelIntegralBack / PixelBack;//前景灰度百分比
          MicroFore = (double)PixelIntegralFore / PixelFore;//背景灰度百分比
          Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
          if (Sigma > SigmaB)//遍历最大的类间方差g
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

uint8 mid=COL/2;//车车插电后第一张图最下行扫线起点
void findline(uint8 image[ROW][COL])
{
    // 丢线信息初始化
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

    //最下行左边线
    for(int col=mid;col>=0;col--)
    {
        if(col==0)
        {
            // 丢线
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
    //最下行右边线
    for(int col=mid;col<=COL-1;col++)
    {
        if(col==COL-1)
        {
            // 丢线
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

    //其他行
    for(int row=ROW-2;row>0;row--)
    {
        for(int col=centerline[row-1];col>=0;col--)
        {
            if(col==0)
            {
                //丢线
                leftline[row] = 0;
                if(Lost_leftline.start.x == -1)// 如果第一次丢线，上一行的边线点即为角点
                {
                    Lost_leftline.start.x = leftline[row-1];
                    Lost_leftline.start.y = row-1;
                }
                else// 不是第一次丢线，添加丢线行数
                    Lost_leftline.lost_time++;
                break;
            }
            else if(image[row][col]==black && image[row][col+1]==white && image[row][col+2]==white)
            {
                leftline[row] = col;
                if(Lost_leftline.start.x != -1 && Lost_leftline.end.x == -1)//丢线后第一次找到线
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
                //丢线
                rightline[row] = COL-1;
                if(Lost_rightline.start.x == -1)// 如果第一次丢线，上一行的边线点即为角点
                {
                    Lost_rightline.start.x = rightline[row-1];
                    Lost_rightline.start.y = row-1;
                }
                else//不是第一次丢线，添加丢线行数
                    Lost_rightline.lost_time++;
                break;
            }
            else if(image[row][col]==black && image[row][col-1]==white && image[row][col-2]==white)
            {
                rightline[row] = col;
                if(Lost_rightline.start.x != -1 && Lost_rightline.end.x == -1)//丢线后第一次找到线
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

// 种子生成法生成左右边线
// ******边线数组的index:0指代图像最下行******
void BinThreshold(unsigned char imageIn[MT9V03X_H][MT9V03X_W])
{
    // 丢线信息初始化
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


    //***********种子生成法**************
    //最下一行扫线
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

    // 更新扫线起点
    mid = (leftline[0]+rightline[0])>>1;

    //左右手巡线：
    for(int i=0;i<MT9V03X_H-1;i++){
        uint8 left=leftline[i];
        uint8 right=rightline[i];
        uint8 next1=i+1;  //用在左右线数组里
        uint8 next2=MT9V03X_H-1-next1;  //用在图像数组里

        //左手
        uint8 isLeftDiuXian=0; //0表示丢线，其他数字表示没有丢线
        for(int j=0;j<50&&isLeftDiuXian==0;j++){
            if(left+j+1<MT9V03X_W&&abs(imageIn[next2][left+j]-imageIn[next2][left+j+1])==white){
                leftline[next1]=left+j;
                isLeftDiuXian=114;
                if(Lost_leftline.start.x != -1 && Lost_leftline.end.x == -1)//丢线后第一次找到线
                {
                    Lost_leftline.end.x = leftline[next1];
                    Lost_leftline.end.y = next1;
                }
            }
            if(left-j-1>=0&&abs(imageIn[next2][left-j-1]-imageIn[next2][left-j])==white){
                leftline[next1]=left-j;
                isLeftDiuXian=114;
                if(Lost_leftline.start.x != -1 && Lost_leftline.end.x == -1)//丢线后第一次找到线
                {
                    Lost_leftline.end.x = leftline[next1];
                    Lost_leftline.end.y = next1;
                }
            }
        }
        //处理丢线
        if(isLeftDiuXian==0){
            leftline[next1]=0;
            if(Lost_leftline.start.x ==0)// 如果第一次丢线，上一行的边线点即为角点
            {
                Lost_leftline.start.x = leftline[i];// TODO:我看上面找到线 指找到了next1行的线，所以这里就弄next1前一行，即第i行
                Lost_leftline.start.y = i;
            }
            else// 不是第一次丢线，添加丢线行数
                Lost_leftline.lost_time++;
        }


        //右手
        uint8 isRightDiuXian=0; //0表示丢线，其他数字表示没有丢线
        for(int j=0;j<50&&isRightDiuXian==0;j++){
            if(right+j+1<MT9V03X_W&&abs(imageIn[next2][right+j]-imageIn[next2][right+j+1])==white){
                rightline[next1]=right+j;
                isRightDiuXian=114;
                if(Lost_rightline.start.x != -1 && Lost_rightline.end.x == -1)//丢线后第一次找到线
                {
                    Lost_rightline.end.x = rightline[next1];
                    Lost_rightline.end.y = next1;
                }
            }
            if(right-j-1>=0&&abs(imageIn[next2][right-j-1]-imageIn[next2][right-j])==white){
                rightline[next1]=right-j;
                isRightDiuXian=114;
                if(Lost_rightline.start.x != -1 && Lost_rightline.end.x == -1)//丢线后第一次找到线
                {
                    Lost_rightline.end.x = rightline[next1];
                    Lost_rightline.end.y = next1;
                }
            }
        }
        //处理丢线，如果丢线，则说明面前是环岛或者十字路口
        // TODO:如果有两处丢线，则只关注第一次丢线
        if(isRightDiuXian==0){
            rightline[next1]=MT9V03X_W-1;
            if(Lost_leftline.start.x ==0)// 如果第一次丢线，上一行的边线点即为角点
            {
                Lost_leftline.start.x = leftline[i];// TODO:我看上面找到线 指找到了next1行的线，所以这里就弄next1前一行，即第i行
                Lost_leftline.start.y = i;
            }
            else// 不是第一次丢线，添加丢线行数
                Lost_leftline.lost_time++;
        }
        centerline[next1] = (leftline[next1]+rightline[next1])>>1;
        //这里应该有道路状况识别，但我不会写/doge
        //
        //1145141919810
        //
        //道路识别结束
    }
    //***********种子生成法结束**************
}

// 清除噪点，有点问题，会丢线，等待优化或找更好的代码
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

            /* 邻域内5个点是边沿 则保留该点 可以调节这里优化滤波效果 */
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

// 画边线
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
        return 1;  //向右
    }else{
        return -1;  //向左
    }
}

void FindLineFromOneSide(uint8 turn){
    if(turn==1){  //向右
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
//曲线拟合   CommonRectificate(&P_LeftBlack[0],startPos-1,endPos+1);
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
    // 开始计时
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

    // 结束计时
//       timer_stop(TIM_7);
//       ips200_show_string(0, 240, "time:");
//       ips200_show_int(60, 240, timer_get(TIM_7), 8);
//       timer_clear(TIM_7);
}
