#include "tempCal.h"

uint8_t CRC8Table[]={
  0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
  157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
  35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
  190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
  70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
  219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
  101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
  248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
  140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
  17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
  175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
  50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
  202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
  87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
  233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
  116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
};

uint8_t CRC8_Table(vector<uint8_t> src,int insize)
{
    uint crc8 = 0;
    for(int i=0;i<insize;i++){
        crc8 = CRC8Table[crc8^src[i]];
    }
    return(crc8);
 
}
//DS18B20 初始化算法
int tempDS18B20Init()
{
	struct timeval OverTime,startTime;
	wiringPiSetup();
	digitalWrite(TEMP_PIN,HIGH);
	pinMode(TEMP_PIN,OUTPUT);
	// 温度传感器的 引脚拉低 480 US
	digitalWrite(TEMP_PIN,LOW);
	delayMicroseconds(INIT_DELAY_TIME_US);	//480
	digitalWrite(TEMP_PIN,HIGH);
	pinMode(TEMP_PIN,INPUT);
	gettimeofday(&startTime,NULL);
	while(digitalRead(TEMP_PIN)==HIGH){		
		gettimeofday(&OverTime,NULL);
		if((OverTime.tv_sec-startTime.tv_sec)>1){
			printf("cattime Over2!\r\n");			
			return -1;
		}
	}
	while(digitalRead(TEMP_PIN)==LOW);	//等待低电平消失
	delayMicroseconds(500);	
	return 0;
}
// 单总线字节写入
void oneWireByteWrite(uint8_t byteBuff)
{
	pinMode(TEMP_PIN,OUTPUT);
	for(int i=0;i<8;i++){
		if(byteBuff & 0x01){		// 发送 1
			digitalWrite(TEMP_PIN,LOW);
			delayMicroseconds(5);
			digitalWrite(TEMP_PIN,HIGH);
			delayMicroseconds(55);
		}else{					//发送 0
			digitalWrite(TEMP_PIN,LOW);
			delayMicroseconds(55);
			digitalWrite(TEMP_PIN,HIGH);
			delayMicroseconds(5);
		}
		byteBuff >>= 1;
	}
}
void oneWireWrite(vector<uint8_t> writeBuff)
{
	for(int i=0;i<(writeBuff.size()-1);i++){
		//temp_printf("in=%x\r\n",writeBuff[i]);
		oneWireByteWrite(writeBuff[i]);
	}
}
uint8_t oneWireByteRead()
{
	uint8_t readBuff;
	for(int i=0;i<8;i++){
		digitalWrite(TEMP_PIN,LOW);
		delayMicroseconds(1);
		//digitalWrite(TEMP_PIN,HIGH);

		pinMode(TEMP_PIN,INPUT);
		delayMicroseconds(14);
		readBuff >>= 1;
		if(digitalRead(TEMP_PIN)==LOW){
			readBuff &= 0x7f;
		}else{
			readBuff |= 0x80;
		}
		delayMicroseconds(40);
		digitalWrite(TEMP_PIN,HIGH);
		pinMode(TEMP_PIN,OUTPUT);
		delayMicroseconds(5);
	}
	return readBuff;
}

void oneWireRead(vector<uint8_t> &readBuffvt,uint8_t readLen)
{
	pinMode(TEMP_PIN,OUTPUT);
	for(int i=0;i<readLen;i++){
		readBuffvt.push_back( oneWireByteRead() );
	}
	pinMode(TEMP_PIN,INPUT);
}


// 单个刻度的温度为  //0.0625
double tempCaculate()
{
	vector<uint8_t> writeBuff,readBuff;
	uint8_t writeLen,readLen,crcval;
	double tempVal=0.0;
	if(tempDS18B20Init()!=0){
		printf("DS18B20 INIT ERR\r\n");
		return 0;
	}
	//oneWireByteWrite(0xf0);
	oneWireByteWrite(0x33);
	oneWireRead(readBuff,8);
	crcval = CRC8_Table(readBuff,7);
	if(crcval!=readBuff[0x07]){
		printf("rom crc ERR!\r\n");
		PRINTSTRDATA(readBuff,8,16);
		printf("crcval=%x\r\n",crcval);
	}
#if 0
	writeBuff = readBuff;
	oneWireByteWrite(0x55);
	oneWireWrite(writeBuff);
	pinMode(TEMP_PIN,INPUT);
	vector<uint8_t>().swap(readBuff);	//清空
	delayMicroseconds(100);
#endif

	if(tempDS18B20Init()!=0){
		printf("DS18B20 INIT ERR\r\n");
		return 0;
	}
	vector<uint8_t>().swap(readBuff);	//清空
	oneWireByteWrite(0xCC);
	oneWireByteWrite(0x44);
	delayMicroseconds(100);
	while(digitalRead(TEMP_PIN)==LOW);


	if(tempDS18B20Init()!=0){
		printf("DS18B20 INIT ERR\r\n");
		return 0;
	}
	oneWireByteWrite(0xCC);
	oneWireByteWrite(0xBE);
	//oneWireByteWrite(0xCC);
	oneWireRead(readBuff,9);
	PRINTSTRDATA(readBuff,9,16);
	crcval = CRC8_Table(readBuff,8);
	if(crcval!=readBuff[8]){
		printf("err temp crc\r\n");
	}
	tempVal = ((double)readBuff[0]+(double)(readBuff[1]<<8)) * 0.0625;
	return tempVal;
}

