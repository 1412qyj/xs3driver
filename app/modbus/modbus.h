#ifndef XS3_MODBUS_H
#define XS3_MODBUS_H

/*
定义一个Modbus协议，用于读写定时器的数据
(1bit 地址) (1bit 11功能码--读取当前的定时发脉冲秒数) (1bit FF)			          (2bit CRC)
(1bit 地址) (1bit 12功能码--写入当前的定时发脉冲秒数) (1bit 秒数) (2bit CRC)

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
	MinCoilCount = 1,
	MaxCoilCount = 2000,
}MinMaxCoilCount_t;

typedef enum
{
	MinRegisterCount = 1,
	MaxRegisterCount = 125,
}MinMaxRegisterCount_t;

typedef enum
{
	MinRegisterValue = 0x0000,
	MaxRegisterValue = 0xFFFF,
}RegisterValue_t;

typedef enum
{
	x01_read_coil = 0x01,
	x03_read_registers = 0x03,
	x0f_write_coils = 0x0F,
	x10_write_registers = 0x10,
	x11_read_tim = 0x11,
	x12_write_tim = 0x12,
}request_function_code_t;

typedef enum
{
	x80_x01_read_coil = (0x80 + 0x01),
	x80_x03_read_registers = (0x80 + 0x03),
	x80_x0f_write_coils = (0x80 + 0x0F),
	x80_x10_write_registers = (0x80 + 0x10),
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
	Error_Ok = 0,
	Error_SlaveAddr = -1,
	Error_Func = -2,
	Error_Crc = -3,
	Error_Format = -4
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

//======================responsex========================

// communication vector
typedef struct
{
	union{
		struct requestx x01;	 //read coil
		struct requestx x03;	 //read register
		struct requestm x0f;	 //write multiple coil
		struct requestm x10;	 //write multiple register
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
		struct exceptions exc;
		unsigned char data[256];
	}response;
}rtu_respond_t;


#endif

