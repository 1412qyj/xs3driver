#ifndef __MODBUS_H
#define __MODBUS_H

/*
自定义11，12功能码
11read tim: 1bitslaveAdd + 1bitfun + 1bitTimData + 2bitcrc
12write tim: 1bitslaveAdd + 1bitfun + 1bitAnyData + 2bitcrc

*/

#define SLAVE_INDEX			0
#define FUNCTION_INDEX		1
#define ERRORNO_INDEX		2
#define ADDRESS_INDEX0		2
#define ADDRESS_INDEX1		3
#define COUNT_INDEX0		4
#define COUNT_INDEX1		5
#define BYTES_INDEX			6

#define MakeShort(h,l)	( (((short)(h)&0xff)<<8) | ((short)(l)&0xff) )
#define ArrayToShort(a)	( (unsigned short)MakeShort(a[0], a[1]) )
#define ArrayToCRC16(a)	( (unsigned short)MakeShort(a[1], a[0]) )
#define GetCoilCount(c) ( (c/8) + ((c%8) ? 1 : 0) )
#define ShortHig(v)		( (unsigned char)(((v)>> 8) & 0xff) )
#define ShortLow(v)		( (unsigned char)((v) & 0xff) )

typedef enum
{
	x01_read_coil = 0x01,
	x03_read_registers = 0x03,
	x0f_write_coils = 0x0F,
	x10_write_registers = 0x10,
	x0a_read_tim = 0x0a,
	x0b_write_tim = 0x0b,
}request_function_code_t;

typedef enum
{
	x80_x01_read_coil = (0x80 + 0x01),
	x80_x03_read_registers = (0x80 + 0x03),
	x80_x0f_write_coils = (0x80 + 0x0F),
	x80_x10_write_registers = (0x80 + 0x10),
	x80_x0a_read_tim = (0x80 + 0x0a),
	x80_x0b_write_tim = (0x80 + 0x0b),
}response_function_code_t;

typedef enum
{
	exception_x01 = 0x01,
	exception_x02 = 0x02,
	exception_x03 = 0x03,
	exception_x04 = 0x04,
}exception_code_t;

typedef enum
{
	No_Error = 0,
	Error_Func = -70,
	Error_SLaveAddr,
	Error_Format,
	Error_Crc,
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


int input_slave(rtu_request_t *pRequest);
int input_func(rtu_request_t *pRequest);
int input_begin_addr(rtu_request_t *pRequest);
int input_count(rtu_request_t *pRequest);
int input_coils(rtu_request_t *pRequest);
int input_registers(rtu_request_t *pRequest);
int print_request(rtu_request_t *pRequest);
int print_respond(rtu_respond_t *pResponse);
int print_errno(int, rtu_respond_t *m);
char setOne(char p, int count);
char setZero(char p, int count);


#endif
