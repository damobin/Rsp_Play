#ifndef LOOPCT_H
#define LOOPCT_H
#include <public.h>
#include <tool.h>
#include <sys/time.h>
#include <time.h>
#include <wiringPi.h>
#include <vector>
#include <iterator>

#include <tempCal.h>	//温度传感器取值
#include <test.h>		//用于捕获遥控和模拟遥控

#define LOOP_DEBUG

#ifdef  LOOP_DEBUG
#define loop_printf(format,...)  do{    \
        printf("LINE:%d "format,__LINE__,##__VA_ARGS__);        \
}while(0);
#else
#define loop_printf(format,...)  do{
}while(0);
#endif

void loopControl(void);


#endif
