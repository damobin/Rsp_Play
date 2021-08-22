#include "capImags.h"
#include "math.h"
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
	sprintf(dataTp,"../imags/_20%2d-%02d-%02d-%02d-%02d-%02d--%d",
					usertimes->tm_year%100,\
					usertimes->tm_mon+1,\
					usertimes->tm_mday,\
					usertimes->tm_hour,\
					usertimes->tm_min,\
					usertimes->tm_sec,\
					times);
	out.assign(dataTp,strlen(dataTp));
}
//室内开台灯的时候亮度为 11~12
//室内开顶灯的时候亮度为 76~86
//室内开顶和台灯的时候亮度为 90~93

//计算图片的亮度
int GetLight(Mat inMat)
{
	double sumVal=0;
	Mat Mattemp;
	if(inMat.cols<=0||inMat.rows<=0||inMat.data==NULL){
		printf("getLight Err Inmat cols/rows/data Err\r\n");
		return -1;
	}
	cvtColor(inMat, Mattemp, CV_BGR2GRAY);
	for(int i=0;i<inMat.rows;i++){
		for(int j=0;j<inMat.cols;j++){
			sumVal += Mattemp.at<uint8_t>(i,j);
		}
	}
	sumVal = sumVal / (inMat.rows*inMat.cols);
	return (int)sumVal;
}

int GetDiffVal(Mat inMat,int aveLight)
{
	double sumDiffVal=0;
	Mat Mattemp;
	if(inMat.cols<=0||inMat.rows<=0||inMat.data==NULL){
		printf("getLight Err Inmat cols/rows/data Err\r\n");
		return -1;
	}
	cvtColor(inMat, Mattemp, CV_BGR2GRAY);
	for(int i=0;i<inMat.rows;i++){
		for(int j=0;j<inMat.cols;j++){
			sumDiffVal += pow((Mattemp.at<uint8_t>(i,j) - aveLight),2);
		}
	}
	sumDiffVal = sumDiffVal / (inMat.rows*inMat.cols);
	return sumDiffVal;
}

void FixImags(Mat inMat,Mat &outMat,int val)
{
	int light=0;
	Mat Mattemp;
	cvtColor(inMat, Mattemp, CV_BGR2GRAY);
	threshold(Mattemp, Mattemp, val, 255.0, CV_THRESH_BINARY);	//大于阈值进行二值化为 255
	medianBlur(Mattemp,Mattemp,5);		//中值滤波 滤波矩形为 5*5
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));		//腐蚀矩阵
    Mat element2 = getStructuringElement(MORPH_RECT, Size(50, 50));		//膨胀矩阵
    erode(Mattemp, Mattemp, element);	//腐蚀
    dilate(Mattemp, Mattemp, element2);	//膨胀			//异常！！

	Mattemp.copyTo(outMat);
}
static void INVMat(Mat &inmat)
{
	for(int i=0;i<inmat.rows;i++){
		for(int j=0;j<inmat.cols;j++){
			if(inmat.at<uint8_t>(i,j)==0){
				inmat.at<uint8_t>(i,j)=0xff;
			}else{
				inmat.at<uint8_t>(i,j)=0x0;
			}
		}
	}
}
#define HUMAN_WIDTH_MIN		15
#define HUMAN_WIDTH_MAX		100
#define HUMAN_HEIGHT_MIN	15
#define HUMAN_HEIGHT_MAX	200
int FoundRectPoints(Mat inMat,Mat &outMat,vector<Rect> &vRts,Point start,Point end)
{
	int ManNum=0;
	outMat = inMat.clone();
    vector<vector<Point>> contours;
    vector<Vec4i> hierarcy;
	Mat TempMat;
	inMat.copyTo(TempMat);
	INVMat(TempMat);
	//查找轮廓
//    findContours(inMat, contours, hierarcy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
    findContours(TempMat, contours, hierarcy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
    vector<vector<Point>>contours_poly(contours.size());
    vector<Rect> boundRect(contours.size()); //定义外接矩形集合
	int x0 = 0, y0 = 0, w0 = 0, h0 = 0;
    for (int i = 0; i<contours.size(); i++)
    {
        approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
        boundRect[i] = boundingRect(Mat(contours_poly[i]));
#if CAP_DEBUG == 3
		cout<<"bund:"<<boundRect[i]<<endl;
#endif
        if (boundRect[i].width >HUMAN_WIDTH_MIN
		 && boundRect[i].width <HUMAN_WIDTH_MAX
         && boundRect[i].height>HUMAN_HEIGHT_MIN
		 && boundRect[i].height<HUMAN_HEIGHT_MAX) {//轮廓筛选
            x0 = boundRect[i].x;		//位于左上角
            y0 = boundRect[i].y;
            w0 = boundRect[i].width;
            h0 = boundRect[i].height;
            rectangle(outMat,Point(x0, y0),Point(x0+w0,y0+h0),Scalar(0,255,0),2,8,0);
			vRts.push_back(boundRect[i]);
            //经过这条线（区间），车辆数量+1
			if( (start.x==end.x) && (start.y!=end.y) ){
#if	0
	            if ((x0 + w0 / 2 + 1) >= start.x && (x0 + w0 / 2 - 1) <= (start.x+4)) {	
	                ManNum++;
    	        }
#else
	            if ( x0  < start.x  && (x0 + w0) > (start.x+1) ) {	
	                ManNum++;
    	        }
#endif
			}else{
#if	0
	            if ((y0 + h0 / 2 + 1) >= start.y && (y0 + h0 / 2 - 1) <= (start.y+4)) {
	                ManNum++;
    	        }
#else

	            if ( y0  < start.y  && (y0 + h0) > (start.y+1) ) {	
	                ManNum++;
    	        }
#endif
			}
        }
        line(outMat, Point(420, 0), Point(420, 480), Scalar(0, 0, 255), 1, 8);//画红线
        //Point org(0, 35);
        //putText(outMat,"CarNum="+intToString(CarNum),org,FONT_HERSHEY_SIMPLEX,0.8f,Scalar(0, 255, 0), 2);
    }
	return ManNum;

}
int JudegeAct(Mat PreMat,Mat AftMat,Mat &outMat)
{
	int ActJd=NOTHING;
	int lightPre,lightAft;
	if(PreMat.cols<=0 || PreMat.rows<=0 || PreMat.data==NULL){
		cout<<"NULL PreMat"<<endl;
		return ActJd;
	}

	if(AftMat.cols<=0 || AftMat.rows<=0 || AftMat.data==NULL){
		cout<<"NULL AftMat"<<endl;
		return ActJd;
	}
	//flip(PreMat,PreMat,-1);	//摄像头旋转180  图片旋转180恢复
	flip(AftMat,AftMat,-1);
	Mat PMat,AMat;
	int TheVal = 4;

	lightPre = GetLight(PreMat);
	lightAft = GetLight(AftMat);

	FixImags(PreMat,PMat,lightPre/2);	//按照光强 1/3 进行二值化
   	FixImags(AftMat,AMat,lightAft/2);
#if CAP_DEBUG == 3
	cap_printf("preLight = %d\r\n",lightPre);
	cap_printf("aftLight = %d\r\n",lightAft);
	imwrite("../imags/PSrcMat.jpg",PreMat);
	imwrite("../imags/PMat.jpg",PMat);
	imwrite("../imags/ASrcMat.jpg",AftMat);
	imwrite("../imags/AMat.jpg",AMat);
#endif
	//判断黑夜还是白天
	if(abs(lightPre-lightAft)>60){
		if(lightPre < lightAft){
			ActJd |= LIGHT_ON;
		}else{
			ActJd |= LIGHT_OFF;
		}
	}
	Mat ResultMat;
	int ManNum;
	vector<Rect> OutRect;
	ManNum = FoundRectPoints(AMat,ResultMat,OutRect,Point(460,0),Point(460,480));
	if(ManNum!=0){
		imwrite("../imags/ResultMat.jpg",ResultMat);
		ActJd |= HUMAN;
	}
	//当人出现的时候 光强变化应该没那么夸张		//自测
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
	PreMat = capImags();		//再次载入  防止第一次图片采集出现模糊
	sleep(1);					//延时1秒后获取当前图
	PreMat = capImags();		//再次载入  防止第一次图片采集出现模糊
	do{
		sleep(1);			//延时1秒后获取当前图
		CurMat = capImags();
		times++;
		judVal = JudegeAct(PreMat,CurMat,TempMat);	//判断图片是否变形
		if(judVal & HUMAN){
			localTimeExc(&usertimes);
			concatData(Namesave,usertimes,times);
			Namesave += "_HUMAN.jpg";
			imwrite(Namesave,CurMat);
			cout<<Namesave<<endl;
			cap_printf("times=%d judVal = HUMAN\r\n",times);
		}
		if(judVal & LIGHT_OFF ){
			localTimeExc(&usertimes);
			concatData(Namesave,usertimes,times);
			Namesave += "_OFF.jpg";
			imwrite(Namesave,CurMat);
			cout<<Namesave<<endl;
			cap_printf("times=%d judVal = LIGHT_OFF\r\n",times);
		}
		if(judVal & LIGHT_ON ){
			localTimeExc(&usertimes);
			concatData(Namesave,usertimes,times);
			Namesave += "_ON.jpg";
			imwrite(Namesave,CurMat);
			cout<<Namesave<<endl;
			cap_printf("times=%d judVal = LIGHT_ON\r\n",times);
		}
		if(judVal & NOTHING){
			cap_printf("times=%d judVal = NOTHING\r\n",times);
		}
		CurMat.copyTo(PreMat);
	}while(1);
	
	return NULL;
}

