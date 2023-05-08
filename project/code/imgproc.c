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

// 种子生成法生成左右边线
void BinThreshold(unsigned char imageIn[MT9V03X_H][MT9V03X_W])
{
    //***********种子生成法**************
    //最下一行扫线
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
            }
            if(left-j-1>=0&&abs(imageIn[next2][left-j-1]-imageIn[next2][left-j])==white){
                leftline[next1]=left-j;
                isLeftDiuXian=114;
            }
        }
        //处理丢线，如果丢线，则说明面前是环岛或者十字路口
        if(isLeftDiuXian==0){
//            isLeftLineDiuXian[next1]=1;
            leftline[next1]=0;
        }


        //右手
        uint8 isRightDiuXian=0; //0表示丢线，其他数字表示没有丢线
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
        //处理丢线，如果丢线，则说明面前是环岛或者十字路口
        if(isRightDiuXian==0){
//            isRightLineDiuXian[next1]=1;
            rightline[next1]=MT9V03X_W-1;
        }

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

void adaptiveThreshold(uint8_t* img_data, uint8_t* output_data, int width, int height, int block, uint8_t clip_value){
    int half_block = block / 2;
    for(int y=half_block; y<height-half_block; y++){
        for(int x=half_block; x<width-half_block; x++){
        // 计算局部阈值
        int thres = 0;
        for(int dy=-half_block; dy<=half_block; dy++){
            for(int dx=-half_block; dx<=half_block; dx++){
            thres += img_data[(x+dx)+(y+dy)*width];
          }
        }
        thres = thres / (block * block) - clip_value;
        // 进行二值化
        output_data[x+y*width] = img_data[x+y*width]>thres ? 255 : 0;
        }
    }
}

// 画边线
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

#define RESULT_ROW 120//结果图行列
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

/*-------------------------------------------------------------------------------------------------------------------
函数：补线
说明：有始有终才使用，直接使用两点斜率进行补线，能进此函数必定有始有终
            start补线起始行指向突出点             stop补线终止行指向          data获取的图像Imagel
            Line_Add为Left_Line_Add或Right_Line_Add         Mode 1为左补线2为右补线
-------------------------------------------------------------------------------------------------------------------*/
void Line_Repair(uint8 Start, uint8 Stop, int16 *Line_Add, int16 Mode)
{
    float res;              //临时变量，储存修补结果
    int16 i;                    //控制行
    float Ka, Kb;           //临时变量

    if ((Mode == 1) && (Right_Add_Start <= Stop) && Start <= 53)            //左边界补线（53行之后）
    {/*Right_Add_Start <= Stop表示在此行之前，右边界扫描都无问题存在，不需要补线*/
        for (i = Start; i >= Stop+2; i -= 2)
        {
            Line_Add[i] = Limit_Protect(Right_Add_Line[i] - Road_Width_Add[i+2], 1, COL-1); //Right_Add_Line与Right_Line数值其实应该是一样的，右边界减去赛道宽度等于左边界
            Road_Width_Add[i] = Road_Width_Add[i+2];                                                                                //这样补出来的线应该会比直接斜率补出来稳一点
        }
    }
    else if ((Mode == 2) && (Left_Add_Start <= Stop) && Start <= 53)    //右边界补线（53行之后）
    {/*Left_Add_Start <= Stop表示在此行之前，左边界扫描都无问题存在，不需要补线*/
        for (i = Start; i >= Stop+2; i -= 2)
        {
            Line_Add[i] = Limit_Protect(Left_Add_Line[i] + Road_Width_Add[i+2], 1, COL-1);  //Left_Add_Line与Left_Line数值其实应该是一样的，左边界加上赛道宽度等于左边界
            Road_Width_Add[i] = Road_Width_Add[i+2];                                                                                //这样补出来的线应该会比直接斜率补出来稳一点
        }
    }
    else
    {
        if (Stop)                                                                                   //有始有终
        {
            if(Mode == 1)                                                                       //左边界补线
                for(i=Stop+2; i<=Stop-4; i-=2)                              //寻找左边界近处凸出点
                {/*为什么要找凸出点，因为图像边界是倾斜的，第一个左边界的点很可能十分靠近图像最左侧，实际图像看一下就明白了*/
                    if(leftline[i]-leftline[i+2] >= 4 )               //数字是相邻两个左边界坐标的差值，如果差值太大，证明这个点不适合补线
                         Stop = i ;                                                             //更新为凸出点的为停止补线行，这样补出来的边界线更符合实际
                }
            else if(Mode == 2)                                                          //右边界补线
                for(i=Stop+2; i<=Stop-4; i-=2)                              //寻找右边界近处凸出点
                {/*为什么要找凸出点，因为图像边界是倾斜的，第一个右边界的点很可能十分靠近图像最右侧，实际图像看一下就明白了*/
                    if(rightline[i+2]-rightline[i] >= 4 )         //数字是相邻两个右边界坐标的差值，如果差值太大，证明这个点不适合补线
                        Stop = i ;                                                              //更新为凸出点的为停止补线行，这样补出来的边界线更符合实际
                }
            if ((Right_Add_Stop >= MIDVALUE && Left_Add_Stop   >= MIDVALUE)       || \
                    (Right_Add_Stop >= MIDVALUE && Left_Add_Start  <= Right_Add_Stop) || \
                    (Left_Add_Stop  >= MIDVALUE && Right_Add_Start <= Left_Add_Stop))   //只有较少行需要补线，不计算斜率，直接竖直向下补线
            {
                for (i = Stop-2; i <= 57; )                                                                                 //从停止行向近处的57行开始补线
                {
                    i += 2;
                    Line_Add[i] = Line_Add[Stop];                                                                           //竖直向下补线，不用计算斜率
                }
            }
            else                                                                                                                                    //将起始行和结束行计算斜率补线
            {
                Ka = 1.0*(Line_Add[Start] - Line_Add[Stop]) / (Start - Stop);               //两点法算出斜率Ka
                Kb = 1.0*Line_Add[Start] - (Ka * Start);                                                        //带入起始点，算出常数Kb
                for (i = Stop+2; i < Start; i += 2)                                                                 //从停止行到起始行
                {
                    res = i * Ka + Kb;                                                                                              //利用Ka,Kb计算丢线行的坐标
                    Line_Add[i] = Limit_Protect((int32)res, 1, COL-1);                              //判断是否超出图像区域
                }
            }
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
函数：寻找左环岛函数
说明：放在图像处理函数里，一直进行扫描
-------------------------------------------------------------------------------------------------------------------*/
void Fand_Left_Ring(void)
{
    int i = 0;                                          //用于循环
    int count_flag = 0;                         //每次扫描前都要清零，count_flag表示检测右边界赛道拟合程度较好的点的数量
    diu_hang_num = 0;                           //每次扫描前都要清零，diu_hang_num表示连续五行丢线的最远行所在的行数（纵坐标）
    if (Line_Count <= 25)                       //有效行小于25行，表示赛道不正常，开始扫描
    {
            for(i=45; i>=25; i-=2)          //在25行到45行之间扫描左边界
            {
                    if (leftline[i]==1&&leftline[i+2]==1&&leftline[i+4]==1&&leftline[i+6]==1)       //左边有五行丢线可能遇到圆环
                    {
                            diu_hang_num = i;       //更新连续五行丢线的最远丢线行所在行数
                            break;                          //找到退出循环
                    }
            }
            if (diu_hang_num > 23)          //连续五行丢线的最远丢线行所在行数大于23行
            {
            /*右边界没丢线，只有丢线的时候等于93，右边31行和51行不相等，保证有斜率，做左边界近处前几行没丢线，只有丢线的时候等于1*/
                    if (rightline[51] != 93 && rightline[31] != 93 && rightline[31] < rightline[51] && \
                            rightline[19] != 93 && rightline[19] <= rightline[31] && \
                            leftline[19] != 1 &&  leftline[21] != 1 && leftline[57] != 1 && leftline[55] != 1 )
                    {
                            for(i=55; i>19; i-=2)
                            {
                                if(rightline[i] != 93)                                         //右侧55到19行没丢线
                                    if(rightline[i+2] < rightline[i])             //并且远处行的横坐标小于近处行，表示扫描赛道正常
                                        return ;                                                                //右边界不正常，无法进行后续处理，返回结束
                            }
                            Curve3_Fitting(&Right_Ka, &Right_Kb, 51,31, rightline, 2);                 //拟合右边界的斜率
                            /*需要在线调试并，修改Ka，Kb的范围保证车能识别到圆环*/
                            if(Right_Ka>=0.3f && Right_Ka <= 0.65f)                                                         //判断拟合的Ka是否在正常范围，表示赛道正常，车身较正
                            {
                                if(Right_Kb >= 30 && Right_Kb <= 65)                                                            //判断拟合的Kb是否在正常范围，表示赛道正常，车身较正
                                {
                                    for(int aaaaa = 25; aaaaa <= 57; aaaaa += 2)
                                    {
                                        Is_Right_Line[aaaaa]  = Fit_Point(aaaaa, Right_Ka, Right_Kb);   //将拟合结果放入圆环专属补线数组
                                        if(rightline[aaaaa] <= (Is_Right_Line[aaaaa]+1) && rightline[aaaaa] >= (Is_Right_Line[aaaaa]-1))//拟合结果较好，右边界正常
                                        {
                                            if(++count_flag == 15)                                                                          //至少15个点拟合没问题
                                            {
                                                island_flag = 1;                                                                                    //环岛标志位置1表示左圆环
                                                island_left_mid_add_flag = 1;                                                           //左圆环中线处理标志位置1，图像处理完之后，按照左圆环方案处理中线输出
                                                Go_Left_Island();                                                                                   //设置左环岛的PID
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
 * 预定图像处理主函数
 */
void Image_Handle()
{
    int16 i;                                // 控制行
    int16 j;                                // 用于二次循环
    int16 res;                          // 用于结果状态判断
    Line_Count      = 0;        // 赛道行数复位
    Left_Add_Start  = 0;        // 复位左补线起始行坐标
    Right_Add_Start = 0;        // 复位右补线起始行坐标
    Left_Add_Stop   = 0;        // 复位左补线起终止坐标
    Right_Add_Stop  = 0;        // 复位右补线起终止坐标

    /***************************** TODO:清除上一帧图像的数据 *****************************/
//    for (i = ROW-3; i >= 19;i-=2)           //初始化边界值，第一行会特殊处理所以一进来就ROW-3
//    {
//            Right_Line[i]     = COL-1;      //给定边界初始值
//            Left_Line[i]      = 1;              //给定边界初始值
//            Left_Add_Flag[i]  = 1;              //初始化补线标志位需要补线为1，不需要补线为0
//            Right_Add_Flag[i] = 1;
//    }

    for(i=ROW-3;i>=19;i-=2)
    {
        // TODO:记得写第一行的数据处理，例如right_add_line/left_add_line等等
        /**************************** 补线检测开始 ****************************/
        // TODO:road_width_min+1有点敏感了，依赖于十分良好的逆透视，感觉要改大些
        if (road_width[i] > Road_Width_Min+1)                      //赛道宽度变宽，可能是十字或环路
        {
            if (Right_Add_Line[i] >= Right_Add_Line[i+2])          //右边界后一行比前一行大了？肯定有问题
                if (!Right_Add_Flag[i])                            //如果找到边界了
                    Right_Add_Flag[i] = 1;                         //强制认定为需要补线
            if (Left_Add_Line[i] <=  Left_Add_Line[i+2])           //右边界后一行比前一行大了？肯定有问题
                if (!Left_Add_Flag[i])                             //如果找到边界了
                    Left_Add_Flag[i] = 1;                          //强制认定为需要补线
            if (Left_Add_Flag[i] || Right_Add_Flag[i])             //如果需要补线
                if (Left_Add_Stop  || Right_Add_Stop)              //并且前面已经补过线了，
                    break;                                                                                                              //就直接结束了，不补线了，补两次没啥用
        }

        /*************************** 第一轮补线开始 ***************************/
        if (Left_Add_Flag[i])                                       //左侧需要补线
        {
            if (i >= ROW-5)                                            //前三行补线不算 //TODO:这里的行数有点迷，先写row-5了
            {
                if (!Left_Add_Start)                                //检测是不是第一次补线
                {
                    Left_Add_Start = i;                             //记录左侧补线开始行
                    for(j=ROW-1; j>=i; j-=2)                //寻找左边界近处凸出点
                        if(leftline[j] > leftline[Left_Add_Start] )
                            Left_Add_Start = j;                     //更新凸出点
                    Left_Ka = 0;                                            //Ka等于0
                    Left_Kb = Left_Add_Line[Left_Add_Start];                    //Kb等于此行横坐标，用凸出点竖直补线
                }
                Left_Add_Line[i] =  Fit_Point(i, Left_Ka, Left_Kb); //使用前一帧图像左边界斜率补线（Ka=0就直接补Kb，不就是底线吗）
            }
            else                                                                    //如果不是前三行
            {
                if (!Left_Add_Start)                                //检测是不是第一次补线
                {
                    Left_Add_Start = i;                             //记录左侧补线开始行
                    Curve1_Fitting(&Left_Ka, &Left_Kb, &Left_Add_Start, Left_Add_Line, 1);  /*Left_Add_Start的值会指向有数据的上一行*///拟合直线Ka,Kb的值会更改
                }
                Left_Add_Line[i] = Fit_Point(i, Left_Ka, Left_Kb);  //补线完成，利用前两行的斜率补一下
            }
        }
        else                                                                        //此行左侧不需要需要补线了
        {
            if (Left_Add_Start)                                     //Left_Add_Start不为0，就表示已经开始补线了
                if (!Left_Add_Stop && !Left_Add_Flag[i+2] && !Left_Add_Flag[i+4])                                                       //是否已经停止，是否连续两行没有丢线，不需要补线
                    if (Left_Add_Line[i] >= Left_Add_Line[i+2] && Left_Add_Line[i+2] >= Left_Add_Line[i+4] && Right_Add_Start < 55)     //左边界是否远处比近处的坐标要大，不大，就是赛道不正常
                    {
                         Left_Add_Stop = i+4;                                                                                                                                       //记录左侧补线结束行，因为检测过两行没丢线了，所以补上去，但此行还是不需要补线
                         Line_Repair(Left_Add_Start, Left_Add_Stop, Left_Add_Line, 1);                                        //更新补线范围内的左边界
                    }
        }
        if (Right_Add_Flag[i])                                  //右侧需要补线
        {
            if (i >= 55)                                                    //前三行补线不算
            {
                if (!Right_Add_Start)
                {
                    Right_Add_Start = i;                            //记录补线开始行
                    for(j=ROW-1; j>=i; j-=2)                //寻找右边界近处凸出点
                        if(rightline[j] < rightline[Right_Add_Start] )
                            Right_Add_Start = j;                    //更新凸出点
                    Right_Ka = 0;                                           //Ka等于0
                    Right_Kb = Right_Add_Line[Right_Add_Start];                     //Kb等于此行横坐标，用凸出点竖直补线
                }
                    Right_Add_Line[i] = Fit_Point(i, Right_Ka, Right_Kb);   //使用前一帧图像左边界斜率补线（Ka=0就直接补Kb，不就是底线吗）
            }
            else
            {
                if (!Right_Add_Start)                               //检测是不是第一次补线
                {
                    Right_Add_Start = i;                            //记录右侧补线开始行
                    Curve1_Fitting(&Right_Ka, &Right_Kb, &Right_Add_Start, Right_Add_Line, 2);  /*Right_Add_Start的值会指向有数据的上一行*///拟合直线Ka,Kb的值会更改
                }
                    Right_Add_Line[i] = Fit_Point(i, Right_Ka, Right_Kb);   //补线完成，利用前两行的斜率补一下
            }
        }
        else
        {
            if (Right_Add_Start)                                    //Right_Add_Start不为0，就表示已经开始补线了
            {
                if (!Right_Add_Stop && !Right_Add_Flag[i+2] && !Right_Add_Flag[i+4])                                                                    //是否已经停止，是否连续两行没有丢线，不需要补线
                {
                    if (rightline[i] <= rightline[i+2] && rightline[i+2] <= rightline[i+4]&& Left_Add_Start < 55)   //右边界是否远处比近处的坐标要小，不小，就是赛道不正常
                    {
                        Right_Add_Stop = i+4;                                                                                                                                                           //记录右侧补线结束行，因为检测过两行没丢线了，所以补上去，但此行还是不需要补线
                        Line_Repair(Right_Add_Start, Right_Add_Stop, Right_Add_Line, 2);                                                      //更新补线范围内的右边界
                    }
                }
            }
        }
        /*************************** 第一轮补线结束 ***************************/
    }
}


