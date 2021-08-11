#include "loopct.h"
#include "unistd.h"
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
void loopControl()
{
	//获取现在时间    年--月--日
	//判断现在的季节  春 - 夏 - 秋 - 冬
	//获取当地经纬度  判断这个区域的温度
	double tempDS18B20;
	time_t timep;
	struct tm *usertimes;
	time (&timep);
	usertimes = localtime(&timep);
	loop_printf("usertimes.min = %d\r\n",usertimes->tm_min);
	loop_printf("usertimes.hour = %d\r\n",usertimes->tm_hour);
	loop_printf("usertimes.day = %d\r\n",usertimes->tm_mday);
	loop_printf("usertimes.mon = %d\r\n",usertimes->tm_mon);
	loop_printf("usertimes.year = %d\r\n",usertimes->tm_year);
	loop_printf("usertimes.wday = %d\r\n",usertimes->tm_wday);
	loop_printf("%s",asctime(usertimes));
	while(1){
		time (&timep);
		//usertimes = gmtime(&timep);
		usertimes = localtime(&timep);
		if(usertimes->tm_wday<5){	//周末我要睡懒觉  不关空调
			if(usertimes->tm_hour>7 && usertimes->tm_hour<18){//早上七点后 下午6点前 关空调
				test(1,"../data/close.bin");	//多关几次 防止失败
				sleep(1);
				test(1,"../data/close.bin");	//多关几次 防止失败
				sleep(1);
				test(1,"../data/close.bin");	//多关几次 防止失败
				sleep(1);
				test(1,"../data/close.bin");	//多关几次 防止失败
				sleep(1);
				test(1,"../data/close.bin");	//多关几次 防止失败
				sleep(1);
				test(1,"../data/close.bin");	//多关几次 防止失败
				sleep(1);
				test(1,"../data/close.bin");	//多关几次 防止失败

				loop_printf("usertimes.min = %d\r\n",usertimes->tm_min);
				loop_printf("usertimes.hour = %d\r\n",usertimes->tm_hour);
				loop_printf("usertimes.day = %d\r\n",usertimes->tm_mday);
				loop_printf("usertimes.mon = %d\r\n",usertimes->tm_mon);
				loop_printf("usertimes.year = %d\r\n",usertimes->tm_year);
				loop_printf("usertimes.wday = %d\r\n",usertimes->tm_wday);
				return ;
			}
		}
		tempDS18B20 = tempCaculate();	//获取温度  让温度处于 29~32区间
		if(tempDS18B20<0){	//温度获取异常CRC异常
			continue;
		}
		//判断季节 经纬度	
		// 月份从0开始算  所以降1
		if(usertimes->tm_mon>=2 && usertimes->tm_mon<=4){	// spring 春季
			
		}else if(usertimes->tm_mon>=5 && usertimes->tm_mon<=7){	// summer 夏季
			if(tempDS18B20 > 34){
				if(usertimes->tm_mon==5){
					test(1,"../data/open_cold27.bin");		//夏季前两个月 27度
				}else if(usertimes->tm_mon==6){
					test(1,"../data/open_cold28.bin");		//夏季末 月    28度
				}else{
					test(1,"../data/open_cold28.bin");		//夏季末 月    28度
				}
			}else if(tempDS18B20 < 30){
				test(1,"../data/close.bin");
			}else{
			}
		}else if(usertimes->tm_mon>=8 && usertimes->tm_mon<=10){	//autumn  秋季

		}else{												//冬季
						
		}

		loop_printf("temp=%lf\r\n",tempDS18B20);
		sleep(10);
	}
}
