#include <public.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <tool.h>
//user head file
//user macro 
#ifndef _WIN32

#define KEYFILE    "../data/key.txt"
#define INPUTFILE  "../data/input.txt"
#define OUTPUTFILE "../data/output.txt"

#else

#define KEYFILE    "../data/key.txt"
#define INPUTFILE  "../data/input.txt"
#define OUTPUTFILE "../data/output.txt"

#endif
int a,b,c;
using namespace std;

std::string exec(const char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}
systemget systemcheck()
{
	systemget no_os_flag=UNDEFINE_OS;
	#ifdef linux
	   no_os_flag=linux_os;
	   cout<<"It is in Linux OS!"<<endl;
	#endif
	
	#ifdef _UNIX
	   no_os_flag=unix_os;
	   cout<<"It is in UNIX OS!"<<endl;
	#endif
	
	#ifdef __WINDOWS_
	   no_os_flag=windows_os;
	   cout<<"It is in Windows OS!"<<endl;
	#endif
	
	#ifdef _WIN32
	   no_os_flag=win32_os;
	   cout<<"It is in WIN32 OS!"<<endl;
	#endif
	
	if(UNDEFINE_OS==no_os_flag){
		cout<<"No OS Defined ,I do not know what the os is!"<<endl;
	}
	return no_os_flag;
}
//获取当前当地系统时间
time_t timep;
void localTimeExc(struct tm **usertimes)
{
	time(&timep);
	*usertimes = localtime(&timep);
}

//字符串转数组工具函数 
static int ExcStrToBytes(uint8_t *obj,uint8_t *src)
{
	char temps[2];
	int tempb=0,lens=0;
	lens = strlen((const char *)src)/3 +1;
	for(int i=0;i<lens+4;i++){
		temps[0]=src[3*i  ];
		temps[1]=src[3*i+1];
		sscanf(temps,"%02x",&tempb);
		obj[i]=tempb;		
	}
	return lens;
}

bool matIsEqual(Mat mat1,Mat mat2) {
	if (mat1.empty() && mat2.empty()) {
		return true;
	}
	if (mat1.cols != mat2.cols || mat1.rows != mat2.rows || mat1.dims != mat2.dims||
		mat1.channels()!=mat2.channels()) {
		return false;
	}
	if (mat1.size() != mat2.size() \
	 || mat1.channels() != mat2.channels() \
	 || mat1.type() != mat2.type()) {
		return false;
	}
	int nrOfElements1 = mat1.total()*mat1.elemSize();
	if (nrOfElements1 != mat2.total()*mat2.elemSize()) return false;
	bool lvRet = memcmp(mat1.data, mat2.data, nrOfElements1) == 0;
	return lvRet;
}
