#ifndef TEMPCAL_H
#define TEMPCAL_H
#include "public.h"
#include "tool.h"
#include <sys/time.h>
#include <time.h>
#include <wiringPi.h>
#include <vector>
#include <iterator>
#define TEMP_DEBUG

#ifdef  TEMP_DEBUG
#define temp_printf(format,...)  do{    \
        printf("LINE:%d "format,__LINE__,##__VA_ARGS__);        \
}while(0);
#else
#define temp_printf(format,...)  do{
}while(0);
#endif

#define TEMP_PIN	8 //GPIO 2
#define INIT_DELAY_TIME_US	480+20 

// 函数声明
uint8_t CRC8_Table(vector<uint8_t> src,int insize);
int tempDS18B20Init();
void oneWireByteWrite(uint8_t byteBuff);
void oneWireWrite(vector<uint8_t> writeBuff);
uint8_t oneWireByteRead();
void oneWireRead(vector<uint8_t> &readBuff,uint8_t readLen);
double tempCaculate();	//单次温度获取
double AveTempGet(int times);	//times次温度获取
double AveTempGet();	//times次温度获取
#endif
