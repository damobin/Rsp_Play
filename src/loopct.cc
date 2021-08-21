#include "loopct.h"
#include "unistd.h"
#include "string"
#include "fstream"
#include "capImags.h"
#include <pthread.h>
#include <csignal>
#define REPET_TIMES	7
static void MulOpenTimes(string fileName,int times);	//多用重复多次开空调  因为树莓派离空调有三米远,有的时候开两次成功一次    目前成功率在60%以上 缩短距离可以提高    离空调半米近98的成功率
static void LogTimeWriteFile(string ofname,struct tm *strtm,string dummy,double tempDS18B20);	//如果出现调控写个log
enum selfMon{
	JAN = 0,
	FEB = 1,
	MAR = 2,
	APR = 3,
	MAY = 4,
	JUN = 5,
	JUL = 6,
	AUG = 7,
	SET = 8,
	OCT	= 9,
	NOV = 10,
	DEC = 11
};
void *loopControl(void* arg)
{
	//获取现在时间    年--月--日
	//判断现在的季节  春 - 夏 - 秋 - 冬
	//获取当地经纬度  判断这个区域的温度
	bool   action=0;
	double tempDS18B20;
#if	0
	time_t timep;
	struct tm *usertimes;
	time (&timep);
	usertimes = localtime(&timep);
#else
	struct tm *usertimes;
	localTimeExc(&usertimes);
#endif
	
	loop_printf("usertimes.min = %d\r\n",usertimes->tm_min);
	loop_printf("usertimes.hour = %d\r\n",usertimes->tm_hour);
	loop_printf("usertimes.day = %d\r\n",usertimes->tm_mday);
	loop_printf("usertimes.mon = %d\r\n",usertimes->tm_mon);
	loop_printf("usertimes.year = %d\r\n",usertimes->tm_year);
	loop_printf("usertimes.wday = %d\r\n",usertimes->tm_wday);
	loop_printf("%s",asctime(usertimes));
	cout<<exec("pwd")<<endl;
	while(1){
#if	0
		time (&timep);
		usertimes = localtime(&timep);
#else
		localTimeExc(&usertimes);
#endif
		if(usertimes->tm_wday<5){	//周末我要睡懒觉  不关空调
			if(usertimes->tm_hour>7 && usertimes->tm_hour<18){//早上七点后 下午6点前 关空调
				MulOpenTimes("../data/close.bin",REPET_TIMES);
				loop_printf("usertimes.min = %d\r\n",usertimes->tm_min);
				loop_printf("usertimes.hour = %d\r\n",usertimes->tm_hour);
				loop_printf("usertimes.day = %d\r\n",usertimes->tm_mday);
				loop_printf("usertimes.mon = %d\r\n",usertimes->tm_mon);
				loop_printf("usertimes.year = %d\r\n",usertimes->tm_year);
				loop_printf("usertimes.wday = %d\r\n",usertimes->tm_wday);
				return NULL;
			}
		}
		tempDS18B20 = tempCaculate();	//获取温度  让温度处于 29~32区间
		if(tempDS18B20<0){	//温度获取异常CRC异常
			continue;
		}
		//判断季节 经纬度	//没钱买
		//固定使用地区为杭州--

		// 月份从0开始算  所以降1
		if(usertimes->tm_mon>=2 && usertimes->tm_mon<=4){	// spring 春季
			
		}else if(usertimes->tm_mon>=5 && usertimes->tm_mon<=7){	// summer 夏季
			if(tempDS18B20 > 36){
				if(usertimes->tm_mon==5){
					MulOpenTimes("../data/open_cold27.bin",REPET_TIMES); //夏季前两个月 27度
					action ^= 1;
				}else if(usertimes->tm_mon==6){
					MulOpenTimes("../data/open_cold28.bin",REPET_TIMES); //夏季前两个月 27度
					action ^= 1;
				}else{
					MulOpenTimes("../data/open_cold28.bin",REPET_TIMES); //夏季前两个月 27度
					action ^= 1;
				}
				LogTimeWriteFile("../data/log.txt",usertimes,"Temp>34",tempDS18B20);
			}else if(tempDS18B20 < 30){
				MulOpenTimes("../data/close.bin",REPET_TIMES);
				LogTimeWriteFile("../data/log.txt",usertimes,"Temp<30",tempDS18B20);
				action ^= 1;
			}else{
			}
		}else if(usertimes->tm_mon>=8 && usertimes->tm_mon<=10){	//autumn  秋季

		}else{												//冬季
						
		}

		loop_printf("temp=%lf\r\n",tempDS18B20);
		sleep(10);
		if(action==1){		//完成开机 或者 关机 需要等待 1min后再次观测
			sleep(60);	
			action ^= 1;
		}
	}
	return NULL;
}

static void MulOpenTimes(string fileName,int times)
{
	for(int i=0;i<times;i++){
		test(1,fileName);	//多关几次 防止失败
		cout<<"file:"<<fileName<<" "<<"times = "<<i<<endl;
		sleep(3);
	}
}

static void LogTimeWriteFile(string ofname,struct tm *strtm,string dummy,double tempDS18B20)
{
	ofstream ofs;
	ofs.open(ofname,ios::app|ios::out);
	if(!ofs){
		cout<<"Err file Write!!!"<<endl;
		return ;
	}
	ofs<<asctime(strtm)<<endl;
	ofs<<dummy<<endl;
	ofs<<tempDS18B20<<endl;
	ofs<<endl;
	ofs.close();
}
static volatile int keepRunning = 1; 
void sig_handler( int sig )
{
    if ( sig == SIGINT){
        keepRunning = 0;
		exit(0);
    }
}

void MulThreadCnt()
{
	void *threadret;
	signal( SIGINT, sig_handler );
	pthread_t tempretureThread,ImagThread;
	pthread_create(&tempretureThread,NULL,loopControl,NULL);
	pthread_create(&ImagThread,NULL,ActJdLoop,NULL);
	pthread_join(tempretureThread,&threadret);
	//pthread_detach(tempretureThread);
#if	1
	pthread_join(ImagThread,NULL);
#else
	pthread_detach(ImagThread);
	while( keepRunning ){
		sleep(10);
	}
#endif
}

