#ifndef CAP_IMAGS_H
#define CAP_IMAGS_H
#include <public.h>
#include <tool.h>
#include <opencv2/opencv.hpp>
#include <unistd.h>

#define NOTHING		0x00
#define HUMAN 		0x01
#define LIGHT_OFF 	0x02
#define LIGHT_ON  	0x03

#define P_WIDTH		640
#define P_HEIGHT	480


#define CAP_DEBUG	3

#ifdef  CAP_DEBUG
#define cap_printf(format,...)  do{    \
        printf("LINE:%d "format,__LINE__,##__VA_ARGS__);        \
}while(0);
#else
#define cap_printf(format,...)  do{
}while(0);
#endif

using namespace cv;
//funs declear
void openVideo(void);
void closeVideo(void);
Mat capImags(void);
void videoCat(void);
void FixImags(Mat inMat,Mat &outMat,int val);
int JudgeAct(Mat PreMat,Mat AftMat,Mat &outMat);
static void concatData(string &out,struct tm *usertimes,int times);
void *ActJdLoop(void *arg);

#endif
