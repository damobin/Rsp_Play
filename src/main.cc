#include <public.h>
#include <tool.h>
#include <opencv2/opencv.hpp>
#include <test.h>
#include <tempCal.h>
#include <loopct.h>	//
#include <capImgs.h>
using namespace cv;
int main(int argc,char *argv[])
{
	double tempretureVal;
	cout<<"main funs"<<endl;
	cout<<exec("pwd")<<endl;
	if(argc<2){
		printf("less args\r\n");
		printf("para as ./run.sh \"-r\" \"../data/xxx.bin\"\r\n");
		printf("para as ./run.sh \"-s\" \"../data/xxx.bin\"\r\n");
		printf("para as rest\r\n");
		printf("para as tempcal\r\n");
		printf("para as loopct\r\n");
		printf("para as imags\r\n");
		printf("para as imags \"imgPath\"\r\n");
//#define DEBUG
#ifndef DEBUG
//		test("../data/open.txt","../data/open.txt");
#else
//		test("./data/open.txt","./data/open.txt");
#endif
	}else{
		string fsname="";
		if(!strcmp(argv[1],"-r")){
			printf("filename= %s para = %s\r\n",argv[2],argv[1]);
			fsname.assign(argv[2]);
			test(0,fsname);
		}else if(!strcmp(argv[1],"-s")){
			printf("filename= %s para = %s\r\n",argv[2],argv[1]);
			fsname.assign(argv[2]);
			test(1,fsname);
		}else if(!strcmp(argv[1],"rest")){
			test(2,fsname);
		}else if(!strcmp(argv[1],"tempcal")){
			tempretureVal = tempCaculate();
			printf("temp=%lf\r\n",tempretureVal);
		}else if(!strcmp(argv[1],"loopct")){	//温度回环控制
			loopControl();
		}else if(!strcmp(argv[1],"imags")){	//温度回环控制
			openVideo();
			Mat imags = capImags();
			if(imags.cols<=0 || imags.rows<=0){
				return -1;
			}
			if(strlen(argv[2])){
				string imagsPath(argv[2]);
				printf("imgPath = %s\r\n",argv[2]);
				imwrite(imagsPath,imags);
			}else{
				imwrite("../imags/img1.jpg",imags);
			}
			closeVideo();
		}else if(!strcmp(argv[1],"video")){	//温度回环控制
			videoCat();
		}else{
			printf("err main para :argv[1]=%s argv[2]=%s\r\n",argv[1],argv[2]);
			printf("para as ./run.sh \"-r\" \"../data/xxx.bin\"\r\n");
			printf("para as ./run.sh \"-s\" \"../data/xxx.bin\"\r\n");
			printf("para as rest\r\n");
			printf("para as tempcal\r\n");
			printf("para as loopct\r\n");
			printf("para as imags\r\n");
			printf("para as imags \"imgPath\"\r\n");
		}
	}
	return 0;
}
