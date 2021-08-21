#include "capImags.h"
VideoCapture cap;	//启动双目摄像头
void openVideo()
{
	cap.open(0);
}

void closeVideo()
{
	cap.release();
}


void videoCat()
{
	Mat outMat;
	openVideo();
	if(!cap.isOpened()){
		cout<<"cap open err!"<<endl;
		return ;
	}
	delay(1000);	//1 秒延时a
	while(1){
		cap>>outMat;
		imshow("outMat",outMat);
		if(waitKey(20)>=0)
			break;
	}
	closeVideo();

}

Mat capImags()
{
	Mat outMat;
	if(!cap.isOpened()){
		cout<<"cap open err!"<<endl;
		return outMat;
	}
	for(int i=0;i<5;i++)
		cap>>outMat;
	
	return outMat;	
}


void FixImags(Mat inMat,Mat &outMat,int val)
{
	Mat Mattemp;
	cvtColor(inMat, Mattemp, CV_BGR2GRAY);
	threshold(Mattemp, Mattemp, val, 255.0, CV_THRESH_BINARY);	//大于进行二值化
	Mattemp.copyTo(outMat);
}

int JudegeAct(Mat PreMat,Mat AftMat,Mat &outMat)
{
	int ActJd=NOTHING;
	
	return ActJd;
}
static void concatData(string &out,struct tm *usertimes,int times)
{
	char dataTp[100];
	memset(dataTp,'\0',100);
	sprintf(dataTp,"_20%2d-%02d-%02d-%02d-%02d-%02d--%d",
					usertimes->tm_year%100,\
					usertimes->tm_mon+1,\
					usertimes->tm_mday,\
					usertimes->tm_hour,\
					usertimes->tm_min,\
					usertimes->tm_sec,\
					times);
	out.assign(dataTp,strlen(dataTp));
}
void *ActJdLoop(void *arg)
{
	int times=0;
	int judVal;
	struct tm *usertimes;
	string Namesave;
	Mat PreMat,CurMat,TempMat;
	openVideo();
	PreMat = capImags();
	do{
		sleep(1);			//延时1秒后获取当前图
		CurMat = capImags();
		times++;
		judVal = JudegeAct(PreMat,CurMat,TempMat);	//判断图片是否变形
		localTimeExc(&usertimes);
		if(judVal == HUMAN){
			cap_printf("times=%d judVal = HUMAN\r\n",times);
		}else if(judVal == LIGHT_OFF ){
			cap_printf("times=%d judVal = LIGHT_OFF\r\n",times);
		}else if(judVal == LIGHT_ON ){
			cap_printf("times=%d judVal = LIGHT_ON\r\n",times);
		}else{
			concatData(Namesave,usertimes,times);
			cout<<Namesave<<endl;
			cap_printf("times=%d judVal = NOTHING\r\n",times);
		}
		CurMat.copyTo(PreMat);
	}while(1);
	
	return NULL;
}

