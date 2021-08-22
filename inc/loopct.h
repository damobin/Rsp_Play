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
#define AIR_OFF		0x0000	//空调处于关机状态
#define AIR_ON		0x0001	//空调处于开启状态
#define AIR_COLD	0x0002	//空调处于制冷状态
#define AIR_HOT		0x0004	//空调处于制热状态
#define AIR_ADDT	0x0008	//空调处于加热状态
#define AIR_SUBT	0x0010	//空调处于降温状态
#define AIR_VAN		0x0020	//空调处于通风状态
#define AIR_DEH		0x0040	//空调处于除湿状态



#ifdef  LOOP_DEBUG
#define loop_printf(format,...)  do{    \
        printf("LINE:%d "format,__LINE__,##__VA_ARGS__);        \
}while(0);
#else
#define loop_printf(format,...)  do{
}while(0);
#endif

void *loopControl(void* arg);
void MulThreadCnt(void);

#endif
