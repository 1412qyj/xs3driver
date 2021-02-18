#ifndef __DATA_GLOBAL_H
#define __DATA_GLOBAL_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <fcntl.h>


#define XS3_DEV_LED_PATH	 "/dev/xs3led"
#define XS3_DEV_TIM_PATH	 "/dev/xs3tim"
#define XS3_DEV_UART_PATH 	 "/dev/xs3uart"

#define LED_ON	1
#define LED_OFF	0
#define LED_1 1
#define LED_2 2
#define LED_3 3

#define Tim_t int

typedef enum
{
	No_Error = 0,
	Error_Func = -70,
	Error_SLaveAddr,
	Error_Format,
	Error_Crc,
	Error_NeedTranfer,
	Error_NoNeedTranfer,
}ErrorCode_t;


//======================request========================
struct requestm
{
	unsigned char slave;
	unsigned char func;
	unsigned char addr[2];
	unsigned char count[2];
	unsigned char byte;
	unsigned char data[256];
};


struct requestx
{
	unsigned char slave;
	unsigned char func;
	unsigned char addr[2];
	unsigned char count[2];
	unsigned char crc[2];
};

//======================request========================

//======================exception========================

struct exceptions
{
	unsigned char slave;
	unsigned char func;
	unsigned char code;
	unsigned char crc[2];
};


//======================exception========================


//======================responsex========================

struct responsex
{
	unsigned char slave;
	unsigned char func;
	unsigned char byte;
	unsigned char data[256];
};

struct responsew
{
	unsigned char slave;
	unsigned char func;
	unsigned char addr[2];
	unsigned char count[2];
	unsigned char crc[2];
};

struct requestTim
{
	unsigned char slave;
	unsigned char func;
	unsigned char timdata;
	unsigned char crc[2];
};

struct respondTim
{
	unsigned char slave;
	unsigned char func;
	unsigned char timdata;
	unsigned char crc[2];
};


//======================responsex========================

// communication vector
typedef struct
{
	union{
		struct requestx x01;	 //read coil
		struct requestx x03;	 //read register
		struct requestm x0f;	 //write multiple coil
		struct requestm x10;	 //write multiple register
		struct requestTim x0a;	//read tim data
		struct requestTim x0b;	//write tim data
		unsigned char data[256]; //convenient for byte wise access
	}request;
}rtu_request_t;

typedef struct
{
	union{
		struct responsex x01;
		struct responsex x03;
		struct responsew x0f;
		struct responsew x10;
		struct respondTim x0a;	//read tim data
		struct respondTim x0b;	//write tim data
		struct exceptions exc;
		unsigned char data[256];
	}response;
}rtu_respond_t;


#endif
