#!/bin/sh
rets=$(whoami)
echo "$rets"
disk='g'
make clean
if [ $rets == "mobin" ]
	then
		make DISK=g
	else
		make
fi

if [ $? -ne 0 ]
	then
		echo "make error"
	else
		echo "success!"
		if [ $# -ge 1 ]
			then		
				cd ./bin
				du -h *
				./* "$*"
				cd ..
			else
				cd ./bin
				du -h *				
				./*
				cd ..
		fi
fi 
