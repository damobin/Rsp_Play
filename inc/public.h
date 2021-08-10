#ifndef PUBLIC_H
#define PUBLIC_H

#include <public.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <iostream>
using namespace std;

// add c head file
#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#ifdef __cplusplus
}
#endif
//end add c head file
#include <iomanip>
typedef unsigned long long  uint64_t;
typedef unsigned int  uint32_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;


#define STOP	\
	fflush(stdout);	\
	exit(0);

#define PRINTSTRDATA(src,len,step)	do{			\
	if(len<=step){								\
		printf("\n");							\
		for(int a=0;  a<len;  a++){				\
			printf("%02x ",src[a]);				\
		}										\
	}else{										\
		printf("\n");							\
		for(int a=0;  a<len;  a++){				\
			printf("%02x ",src[a]);				\
			if(a!=0 && (a+1)%step == 0){		\
				printf("\n");					\
			}									\
		}										\
	}											\
	printf("\n");								\
}while(0);

#define OSSTRDATA(os,src,len,step)	do{				\
	if(len<=step){									\
		os<<endl;									\
		for(int a=0;  a<len;  a++){					\
			os<<setw(2)<<hex<<static_cast<int>(src[a])<<" ";	\
		}											\
	}else{											\
		printf("\n");								\
		for(int a=0;  a<len;  a++){					\
			os<<setw(2)<<hex<<static_cast<int>(src[a])<<" ";	\
			if(a!=0 && (a+1)%step == 0){			\
				os<<endl;							\
			}										\
		}											\
	}												\
	printf("\n");									\
}while(0);


//MATRIX COPY
#define MATRIXCPYS(DESTS,SRCS,ORDER)	do{			\
	for(int a=0;a<ORDER;a++){							\
		for(int b=0;b<ORDER;b++){						\
			*(DESTS[a]+b) = *(SRCS[a]+b);			\
		}											\
	}												\
}while(0);

//MATRIX EXCHANGE TO INEX
#define MATRIXEXCHANGEINDEX(INDEX,MATRIX,ORDER)	do{			\
	for(int a=0;a<ORDER;a++){									\
		for(int b=0;b<ORDER;b++){								\
		*(INDEX+a*ORDER + b) = *(MATRIX[a]+b);				\
		}													\
	}														\
}while(0);

//字符串copy
#define CHARCPYS(DESTS,SRCS,LEN)	do{		\
	for(int a=0;a<LEN;a++)						\
	{										\
		*(DESTS+a) = *(SRCS+a);				\
	}										\
}while(0);

//字符串Xor
#define CHARXORS(DESTS,SRCS,LEN)	do{		\
	for(int a=0;a<LEN;a++)						\
	{										\
		*((DESTS)+a) ^= *((SRCS)+a);		\
	}										\
}while(0);

#endif
