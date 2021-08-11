#include <public.h>
#include <tool.h>

#include <test.h>
#include <tempCal.h>
#include <loopct.h>	//
int main(int argc,char *argv[])
{
	double tempretureVal;
	cout<<"main funs"<<endl;
	cout<<exec("pwd")<<endl;
	if(argc<2){
		printf("less args\r\n");
		printf("para as : ./run.sh \"-s\" \"../data/outfile.bin\"\r\n");
		printf("para as : ./run.sh \"-r\" \"../data/outfile.bin\"\r\n");
		printf("para as : ./run.sh \"rest\" \" \"\r\n");
		printf("para as : ./run.sh \"tempcal\" \" \"\r\n");
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
		}else{
			cout << " err io files "<<endl;
		}
	}
	return 0;
}
