#ifndef TEST_H
#define TEST_H
#include <public.h>
#include <tool.h>
#include <time.h>
#include <sys/time.h>
#include <fstream>
#include <map>
#include <iterator>
#define  TEST_DEBUG
#ifdef   TEST_DEBUG
#define test_printf(format,...)  do{	\
	printf("LINE:%d "format,__LINE__,##__VA_ARGS__);	\
}while(0);
#else
#define test_printf(format,...) {}
#endif

#define RECV_PINNUM 1  //gpio 18
#define SEND_PINNUM 0  //gpio 17


#define LOADWAVE(PIN,TL,TH)							\
	digitalWrite((PIN),HIGH);						\
	delayMicroseconds((TL));						\
	digitalWrite((PIN),LOW);						\
	delayMicroseconds((TH));						

#define LOADWAVECS(PIN,TL,TH,TIMES)			\
	for(int ts=0;ts<(int)((TIMES));ts++){	\
		LOADWAVE(PIN,TL,TH)					\
	}										
	


class NECWave{
public:
        NECWave(){
                LowTimeUs = 0;
                HightTimeUs = 0;
        };
        int LowTimeUs;
        int HightTimeUs;
};
void test(int choice,string filePath);

//void initNecPins(int inPin,int outPin); //初始化引脚方向
void initNecINPins(int inPin);
void initNecOUTPins(int outPin);
NECWave catchSignalWave(int inPin);             //捕获单个上升下降
void catchlirc(int inPin,vector<NECWave> &NecTimeCat,string savePath);  //捕获总函数
int caculateDepTime(struct timeval startT,struct timeval endT); // 计算时间差值
void sendWave(int outPin,vector<NECWave> NecTimeCat,string ifilePath);           //发送捕获的NecTimeCat
void VtNECWave2File(string path,vector<NECWave> NecTimeCat);
void File2VtNECWave(string path,vector<NECWave> &NecTimeCat);


#endif
