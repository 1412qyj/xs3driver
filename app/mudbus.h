#ifndef __MODBUS_H
#define __MODBUS_H

/*
自定义11，12功能码
11read tim: 1bitslaveAdd + 1bitfun + 1bitTimData + 2bitcrc
12write tim: 1bitslaveAdd + 1bitfun + 1bitAnyData + 2bitcrc

*/

#include "data_global.h"

#define SLAVE_ADDR 0x01

#define SLAVE_INDEX			0
#define FUNCTION_INDEX		1
#define TIMDATA_INDEX 		2
#define ERRORNUM_INDEX		2


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
	exception_x01_valid_funcode = 0x01,
	exception_x02_valid_addr = 0x02,
	exception_x03_valid_data = 0x03,
	exception_x04_slave_fault = 0x04,
}exception_code_t;

int ModbusDataCheck(rtu_request_t *pRequestMsg, int len);
int ModbusRespondHandle(rtu_request_t *pRequestMsg, rtu_respond_t *pRespondMsg, int ErrorNum);



#endif
