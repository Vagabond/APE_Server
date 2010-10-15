#!/bin/bash

OS_TARGET=`uname -s`

case "$OS_TARGET" in
        linux* | Linux*)
        HOST_OS=Linux
		echo "#define USE_EPOLL_HANDLER" > ./src/configure.h
		echo "LINUX_BUILD = 1" > ./modules/plateform.mk;;
        Darwin*)
        HOST_OS=Darwin
		echo "#define USE_KQUEUE_HANDLER" > ./src/configure.h
		echo "DARWIN_BUILD = 1" > ./modules/plateform.mk;;
        *)
        HOST_IS=Linux;;
esac

if [ -e "/usr/include/mysql/mysql.h" ]
then
    echo "HAS_MYSQL = yes" > ./modules/mysql.mk
	echo "#define _USE_MYSQL 1" >> ./src/configure.h
	cd ./modules/deps/mysac/
	make
	cd ../../../
else
	echo "HAS_MYSQL = 0" > ./modules/mysql.mk
	echo "#undef _USE_MYSQL" >> ./src/configure.h
fi

if [ -e "/usr/include/zmq.h" ] || [ -e "${ZMQ_DIR}/include/zmq.h" ]; then
	echo "HAS_ZMQ = 1" > ./modules/zmq.mk
	if [ "x${ZMQ_DIR}" != "x" ]; then
		echo "ZMQ_DIR = ${ZMQ_DIR}" >> ./modules/zmq.mk
	fi
else
	echo "HAS_ZMQ = 0" > ./modules/zmq.mk
fi

cd ./deps/udns-0.0.9/
make clean && ./configure && make
cd ../js/src/
./configure && make
cd ../../../
make
cd ./modules/ && make
