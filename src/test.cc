#include "test.h"
#define OVERTIME_SECOND_NUM  15	//最大超时时间
#define LOADWAVELEVEL	0.35	//占空比
#define LOADFREQ 38000		//载波频率
void test(int choice,string filePath)
{
	vector<NECWave> NecTimeCat;
	test_printf("test funs\r\n");
	//initNecPins(RECV_PINNUM,SEND_PINNUM);
	wiringPiSetup();
	if(choice == 0){
		initNecINPins(RECV_PINNUM);
		catchlirc(RECV_PINNUM,NecTimeCat,filePath);
	}else if(choice==1){
		initNecOUTPins(SEND_PINNUM);
		sendWave(SEND_PINNUM,NecTimeCat,filePath);
	}else if(choice==2){
		wiringPiSetup();
		pinMode(RECV_PINNUM,INPUT);
		pinMode(SEND_PINNUM,INPUT);	//输出设置为输入
	}else{
		cout<<"Err filename"<<endl;
	}
}

void initNecINPins(int inPin)
{
	pinMode(inPin,INPUT);
}

void initNecOUTPins(int outPin)
{
	pinMode(outPin,OUTPUT);
	digitalWrite(outPin,HIGH);
}

void VtNECWave2File(string path,vector<NECWave> NecTimeCat)
{
	ofstream ofs;
	ofs.open(path,ios::out|ios::binary);
	if(!ofs){
		cout<<exec("pwd")<<endl;
		printf("Err fileOpen write in path:%s\r\n",path.c_str());
		return ;
	}
	for(int num =0 ;num<NecTimeCat.size();num++){
		ofs.write((char*)&NecTimeCat[num].LowTimeUs,4);
		ofs.write((char*)&NecTimeCat[num].HightTimeUs,4);
		//ofs<<NecTimeCat[num].LowTimeUs;
		//ofs<<NecTimeCat[num].HightTimeUs;
	}
	ofs.close();
}
void File2VtNECWave(string path,vector<NECWave> &NecTimeCat)
{
	char buff[4];
	ifstream ifs;
	ifs.open(path,ios::in|ios::binary);
	if(!ifs){
		cout<<exec("pwd")<<endl;
		printf("Err fileOpen read in path:%s\r\n",path.c_str());
		return ;
	}
	while(!ifs.eof()){
		NECWave NecTwave;

		//ifs>>buff;
		ifs.read(buff,4);
		NecTwave.LowTimeUs = *((int *)buff);
		//ifs>>buff;
		ifs.read(buff,4);
		NecTwave.HightTimeUs = *((int *)buff);
		
		if(ifs.eof()){
    		break;
		}
		//ifs>>NecTwave.LowTimeUs;		
		//ifs>>NecTwave.HightTimeUs;		
		NecTimeCat.push_back(NecTwave);
	}
	ifs.close();
	
}
void catchlirc(int inPin,vector<NECWave> &NecTimeCat,string savePath)
{
	bool flag=false;
	double val;
	int index = 0;
	//NECWave singleWave;
	int usCountsLow,usCountsHIGH;
	struct timeval startT,endT;
	test_printf("start catch\r\n");
	int cntTs = 0;
	while(1){
		NECWave singleWave = catchSignalWave(inPin);
		if(singleWave.LowTimeUs==0 && singleWave.HightTimeUs==0){
			if(index==0){
				test_printf("End wave Or Over time!\r\n");
				break;
			}else{
				test_printf("recv data\r\n");
				break;
			}
		}
		if(singleWave.HightTimeUs > OVERTIME_SECOND_NUM*1000000){ // Second
			test_printf("OVER TIME BREAK\r\n");
			break;
		}
		if(abs(singleWave.LowTimeUs- singleWave.HightTimeUs)>100000){ //100ms
			if(index==0){
				continue;
			}
			test_printf("END TIME BREAK\r\n");
			cout<<"LowTimeUs:"<<singleWave.LowTimeUs<<endl;
			cout<<"HightTimeUs:"<<singleWave.HightTimeUs<<endl;
			index++;
			singleWave.HightTimeUs = 4000;
			NecTimeCat.push_back(singleWave);
			break;
		}
		if(index==0){
			if(singleWave.HightTimeUs>400){
					flag = true;
			}else{
				if(cntTs==3){
					break;
				}
				cout<<"Times"<<endl;
				
				cout<<"LowTimeUs:"<<singleWave.LowTimeUs<<endl;
				cout<<"HightTimeUs:"<<singleWave.HightTimeUs<<endl;
				delay(1000);
				cntTs ++;
				index = 0;
			}
		}
		if(flag==true){
			index++;
			NecTimeCat.push_back(singleWave);
		}		
	}
	if(flag==true){
		for(int num=0;num<NecTimeCat.size();num++){
			test_printf("num:%d Low=%d Hight=%d\r\n",num,NecTimeCat[num].LowTimeUs,	\
			NecTimeCat[num].HightTimeUs);
		}
		VtNECWave2File(savePath,NecTimeCat);
	}else{
		printf("unfound signal in\r\n");
	}
	

}
NECWave catchSignalWave(int inPin)
{
	NECWave singleWave;
	struct timeval startT,endT,OverTime,OverTst;
		


again:
	gettimeofday(&OverTst,NULL);
	while(digitalRead(inPin)==HIGH){
		gettimeofday(&OverTime,NULL);
		if((OverTime.tv_sec-OverTst.tv_sec)>OVERTIME_SECOND_NUM){	//连续持续两秒的NEC不存在！
			printf("cattime over1!\r\n");
			return singleWave;
		}
	}
	gettimeofday(&startT,NULL);	//获取起始时间  下降沿时刻
	while(digitalRead(inPin)==LOW);
	gettimeofday(&endT,NULL);	//获取结束时间  上升沿时刻
	singleWave.LowTimeUs = caculateDepTime(startT,endT);

	if(singleWave.LowTimeUs<450){
		goto again;
	}	


	while(digitalRead(inPin)==HIGH){
		gettimeofday(&OverTime,NULL);
		if((OverTime.tv_sec-OverTst.tv_sec)>(OVERTIME_SECOND_NUM+1)){	//连续持续两秒的NEC不存在！
			printf("cattime over2!\r\n");
			singleWave.LowTimeUs = 0;
			singleWave.HightTimeUs = 0;
			return singleWave;
		}
	}
	gettimeofday(&startT,NULL);	//获取起始时间  下降沿
#if	0
	while(digitalRead(inPin)==LOW);
	gettimeofday(&endT,NULL);	//获取结束时间
	singleWave.HightTimeUs = caculateDepTime(startT,endT);
#else
	singleWave.HightTimeUs = caculateDepTime(endT,startT);
#endif
	return singleWave;
}

int caculateDepTime(struct timeval startT,struct timeval endT)
{
	int usCountsStart,usCountsEnd;
	usCountsStart = startT.tv_sec * 1000000 + startT.tv_usec;
	usCountsEnd   = endT.tv_sec * 1000000 + endT.tv_usec;
	return  (usCountsEnd - usCountsStart);
}

void sendWave(int outPin,vector<NECWave> NecTimeCat,string ifilePath)		//发送捕获的NecTimeCat
{
	File2VtNECWave(ifilePath,NecTimeCat);
	for(int num=0;num<NecTimeCat.size();num++){
		test_printf("num:%d Low=%d Hight=%d\r\n",num,NecTimeCat[num].LowTimeUs,	\
		NecTimeCat[num].HightTimeUs);
	}
	vector<int> loadcycleVts;
	int index=0;
	float fPeriodUs = 1000000 / LOADFREQ;
	uint32_t u32PeriodUs = fPeriodUs;
	uint32_t u32LoadHeightTime = u32PeriodUs * LOADWAVELEVEL;
	uint32_t u32LoadLowTime    = u32PeriodUs - u32LoadHeightTime;
	uint32_t loadcycleTs;
	cout<<"load Wave HeightTimeUs"<<u32LoadHeightTime<<endl;
	cout<<"load Wave LowTimeUs"<<u32LoadLowTime<<endl;
	for(auto wavePtr=NecTimeCat.begin();wavePtr!=NecTimeCat.end();wavePtr++){
       	loadcycleTs = (wavePtr->LowTimeUs+u32PeriodUs) / u32PeriodUs;		//预先把轮次获取   除法吃时间
		loadcycleVts.push_back(loadcycleTs);
	}
	for(auto wavePtr=NecTimeCat.begin();wavePtr!=NecTimeCat.end();wavePtr++){
		//发送高电平  ----携带载波
		if(1){
		//if(index!=0){
	       	loadcycleTs = loadcycleVts[index];
			LOADWAVECS(outPin,u32LoadLowTime,u32LoadHeightTime,loadcycleTs);
		}else{
			delayMicroseconds(wavePtr->LowTimeUs);
		}

		//发送低电平 不携带载波
		digitalWrite(outPin,LOW);
		delayMicroseconds(wavePtr->HightTimeUs - u32PeriodUs*3);
		index++;
	}
	digitalWrite(outPin,HIGH);
}



