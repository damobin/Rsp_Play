#ifndef CAP_IMGS_H
#define CAP_IMGS_H
#include <public.h>
#include <tool.h>
#include <opencv2/opencv.hpp>

using namespace cv;
//using namespace cv2;

//funs declear

void openVideo(void);
void closeVideo(void);
Mat capImags(void);
void videoCat(void);
#endif
