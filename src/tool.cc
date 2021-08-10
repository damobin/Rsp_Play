#include <public.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
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

enum systemget{
	linux_os 	= 0,
	unix_os 	= 1,
	windows_os 	= 2,
	win32_os 	= 3,
	UNDEFINE_OS = 0xff,
};
using namespace std;
//<<funs declear
systemget systemcheck();
std::string exec(const char* cmd);
static int ExcStrToBytes(uint8_t *Obj,uint8_t *Src);		//字符串转数组

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
