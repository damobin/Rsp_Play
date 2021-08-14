#include "capImgs.h"
VideoCapture cap;	//启动双目摄像头
void openVideo()
{
	cap.open(0);
}

void closeVideo()
{
	cap.release();
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
