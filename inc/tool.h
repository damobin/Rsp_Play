#ifndef TOOL_H
#define TOOL_H


#include <public.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
enum systemget{
        linux_os        = 0,
        unix_os         = 1,
        windows_os      = 2,
        win32_os        = 3,
        UNDEFINE_OS = 0xff,
};

systemget systemcheck();
std::string exec(const char* cmd);
static int ExcStrToBytes(uint8_t *Obj,uint8_t *Src);            //字符串转数组
bool matIsEqual(Mat mat1,Mat mat2);

#endif
