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



static void concatData(string &out,struct tm *usertimes,int times)
{
	char dataTp[100];
	memset(dataTp,'\0',100);
	sprintf(dataTp,"_20%2d-%02d-%02d-%02d-%02d-%02d--%d.jpg",
					usertimes->tm_year%100,\
					usertimes->tm_mon+1,\
					usertimes->tm_mday,\
					usertimes->tm_hour,\
					usertimes->tm_min,\
					usertimes->tm_sec,\
					times);
	out.assign(dataTp,strlen(dataTp));
}

void FixImags(Mat inMat,Mat &outMat,int val)
{
	Mat Mattemp;
	cvtColor(inMat, Mattemp, CV_BGR2GRAY);
	threshold(Mattemp, Mattemp, val, 255.0, CV_THRESH_BINARY);	//大于进行二值化a
	medianBlur(Mattemp,Mattemp,5);		//中值滤波 滤波矩形为 5*5
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));		//腐蚀矩阵
    Mat element2 = getStructuringElement(MORPH_RECT, Size(50, 50));		//膨胀矩阵
    erode(Mattemp, Mattemp, element);	//腐蚀
    //dilate(Mattemp, Mattemp, element2);	//膨胀

	Mattemp.copyTo(outMat);
}

int JudegeAct(Mat PreMat,Mat AftMat,Mat &outMat)
{
	int ActJd=NOTHING;
	if(PreMat.cols<=0 || PreMat.rows<=0 || PreMat.data==NULL){
		cout<<"NULL PreMat"<<endl;
		return ActJd;
	}

	if(AftMat.cols<=0 || AftMat.rows<=0 || AftMat.data==NULL){
		cout<<"NULL AftMat"<<endl;
		return ActJd;
	}
	//flip(PreMat,PreMat,-1);	//摄像头原因旋转180
	flip(AftMat,AftMat,-1);
	Mat PMat,AMat;
	int TheVal = 4;
	FixImags(PreMat,PMat,TheVal);	
	FixImags(AftMat,AMat,TheVal);
#if CAP_DEBUG == 3
	imwrite("../imags/PSrcMat.jpg",PreMat);
	imwrite("../imags/PMat.jpg",PMat);
	imwrite("../imags/ASrcMat.jpg",AftMat);
	imwrite("../imags/AMat.jpg",AMat);
#endif
	//判断黑夜还是白天
	

	return ActJd;
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
		if(judVal == HUMAN){
			localTimeExc(&usertimes);
			concatData(Namesave,usertimes,times);
			imwrite(Namesave,CurMat);
			cout<<Namesave<<endl;
			cap_printf("times=%d judVal = HUMAN\r\n",times);
		}else if(judVal == LIGHT_OFF ){
			localTimeExc(&usertimes);
			concatData(Namesave,usertimes,times);
			imwrite(Namesave,CurMat);
			cout<<Namesave<<endl;
			cap_printf("times=%d judVal = LIGHT_OFF\r\n",times);
		}else if(judVal == LIGHT_ON ){
			localTimeExc(&usertimes);
			concatData(Namesave,usertimes,times);
			imwrite(Namesave,CurMat);
			cout<<Namesave<<endl;
			cap_printf("times=%d judVal = LIGHT_ON\r\n",times);
		}else{
			cap_printf("times=%d judVal = NOTHING\r\n",times);
		}
		CurMat.copyTo(PreMat);
	}while(1);
	
	return NULL;
}

