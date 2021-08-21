#<<compiler
CC = g++
#<<file type
TYPE=cc
#<<out bin
TARGET = wiringPi_T1
#<<out directory
OUTDIR = ./bin

#<< flags -w去除警告   -Wall 添加警告
CFLAGS = -g -w -std=c++11

#FILE DISK


#<< head file

INCS = -I ./inc
#<< link
LINK  = 
LINK += -lwiringPi -lpthread



ifeq ($(shell uname), Linux)
#linux
INCS += -I/usr/local/include/opencv 
INCS += -I/usr/local/include 

LINKPATH = -L/usr/local/lib 
LINK += -lopencv_dnn 
LINK += -lopencv_ml
LINK += -lopencv_objdetect
LINK += -lopencv_shape
LINK += -lopencv_stitching
LINK += -lopencv_superres
LINK += -lopencv_videostab
LINK += -lopencv_calib3d
LINK += -lopencv_features2d 
LINK += -lopencv_highgui
LINK += -lopencv_videoio
LINK += -lopencv_imgcodecs
LINK += -lopencv_video
LINK += -lopencv_photo
LINK += -lopencv_imgproc
LINK += -lopencv_flann
LINK += -lopencv_core
else
#windows

INCS += -I /$(DISK)/soft/openssl/OpenSSL-Win64include/openssl
INCS += -I $(DISK):\soft\openssl\OpenSSL-Win64\include
INCS += -I $(DISK):\soft\openssl\OpenSSL-Win64\include\openssl
#<< linkpath
LINKPATH = -L $(DISK):\soft\openssl\OpenSSL-Win64\lib
#LINK  += -ltommath 
LINK  += -llibssl 
LINK  += -llibcrypto

endif 



#<< creat forld
CREAT_FORLD = XXX

CFLAGS += $(INCS)


SRC = ./src
OBJ = $(SRC)/obj

#<< all source file(*.c *.cc)
SOURCE =  $(wildcard $(SRC)/*.$(TYPE))
SOURCE := $(notdir $(SOURCE))

#<< all object file (*.o)
OBJS   =  $(patsubst %.$(TYPE),%.o,$(SOURCE))
OBJS   := $(foreach X,$(OBJS),$(OBJ)/$(X))


.PHONY:all
all:$(CREAT_FORLD) $(TARGET)
$(CREAT_FORLD):
	mkdir -p $(OBJ) $(OUTDIR)
#<< creat *.o file
$(OBJ)/%.o:$(SRC)/%.$(TYPE) 
	$(CC) $(CFLAGS) -c -o  $@ $<

#<< creat bin file
$(TARGET):$(OBJS)
	$(CC) -o $(OUTDIR)/$(TARGET) $(OBJS) $(LINKPATH) $(LINK)


.PHONY:clean
clean:
	rm -rf $(OBJ) $(OUTDIR)


