#this is a Makefile test v1.0
CC = arm-linux-gcc
CFLAGS = -c -o
TARGET ?= TESTAPP
cfiles := $(wildcard *.c)					#get all .c file in current path 
objs = $(patsubst %.c,%.o, $(cfiles))		#use .o replace .c
LIBs := pthread

$(TARGET):$(objs)
	$(CC) -o $(TARGET) $(objs) -l$(LIBs)
	cp $(TARGET) ~/source/rootfs/qyj/

.PHONY:clean
clean:
	-rm $(objs) $(TARGET)
