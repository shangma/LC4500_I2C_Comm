/*
 * runAvrOnArduino.c
 *
 * Created: 5/14/2014 5:46:09 PM
 *  Author: ms
 */ 



/* define CPU frequency in Mhz here if not defined in Makefile */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

/* 9600 baud */
#define UART_BAUD_RATE 9600
#define ERROR 1
#define SUCCESS (!ERROR)

#include <stdlib.h>
#include <avr/io.h>
#include <string.h>     /* strcat */
#include <stdlib.h>     /* strtol */
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "uart.h"
#include "twi.h"
#include "dlpstatus.h"


uint8_t DLPCReadByte(uint8_t readCommand, uint8_t bytesOfCommandData);
uint8_t DLPWriteByte();

const char *byte_to_binary(int x)
{
	static char b[9];
	b[0] = '\0';

	int z;
	for (z = 128; z > 0; z >>= 1)
	{
		strcat(b, ((x & z) == z) ? "1" : "0");
	}

	return b;
}


int main(void)
{
	uint8_t u8ReadStatus;
	uint8_t u8WriteStatus;
	
    uart_init( UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU) );
    sei();
	TWIInit();
	
	/************************************************************************/
	/*                   Reading Data from DLPC 350                         */
	/************************************************************************/

	/*
	u8ReadStatus = DLPCReadByte(hardwareStatus);
	if (u8ReadStatus == SUCCESS)
	{
		uart_puts("Success");
		uart_puts("\r\n");
	}

	u8ReadStatus = DLPCReadByte(systemStatus);
	if (u8ReadStatus == SUCCESS)
	{
		uart_puts("Success");
		uart_puts("\r\n");
	}

	
	u8ReadStatus = DLPCReadByte(displayModeRead, 1);
	if (u8ReadStatus == SUCCESS)
	{
		uart_puts("Read Success");
		uart_puts("\r\n");
	}
	*/

	_delay_ms(1000);
	
	/************************************************************************/
	/*                       Writing Data to DLPC 350                       */
	/************************************************************************/
	
	
	u8WriteStatus = DLPWriteByte();
	if (u8WriteStatus == SUCCESS)
	{
		uart_puts("Write Success");
		uart_puts("\r\n");		
	}
	
			
    while(1)
    {
        //TODO:: Please write your application code 
		uart_puts("Read again\r\n");
		u8ReadStatus = DLPCReadByte(displayModeRead, 1);
		if (u8ReadStatus == SUCCESS)
		{
			uart_puts("Read Success");
			uart_puts("\r\n");
		}
		_delay_ms(2000);
    }
}


uint8_t DLPWriteByte()
{
	uart_puts("write will start \r\n");
	
	TWIStart();
	if (TWIGetStatus() != 0x08)
	{
		uart_puts("Can't start: ");
		uart_putc(TWIGetStatus());
		return ERROR;
	}
	uart_puts("write starts \r\n");
	
	TWIWrite(writeOnLow);
	if (TWIGetStatus() != 0x18)
	{
		uart_puts("Can't write: ");
		uart_putc(TWIGetStatus());
		return ERROR;
	}
	uart_puts("write address \r\n");
	
	TWIWrite(displayModeWrite);
	if (TWIGetStatus() != 0x28)
	{
		uart_puts("Can't write: ");
		uart_putc(TWIGetStatus());
		return ERROR;
	}
	
	TWIWrite(0x01);
	if (TWIGetStatus() != 0x28)
	{
		uart_puts("Can't write: ");
		uart_putc(TWIGetStatus());
		return ERROR;
	}
	
	TWIStop();
	uart_puts("write end \r\n");
	return SUCCESS;
	
}

uint8_t DLPCReadByte(uint8_t readCommand, uint8_t bytesOfCommandData)
{
	uart_puts("read will start \r\n");
	
	TWIStart();
	if (TWIGetStatus() == 0x08)
	{
		uart_puts("read starts \r\n");
	} else if (TWIGetStatus() == 0x10)
	{
		uart_puts("start again \r\n");	
	} else
	{
		uart_puts("Can't start: ");
		uart_putc(TWIGetStatus());
		return ERROR;
	}

	
	TWIWrite(writeOnLow);
	if (TWIGetStatus() != 0x18)
	{
		uart_puts("Can't write: ");
		uart_putc(TWIGetStatus());
		return ERROR;
	}
	uart_puts("write address \r\n");
	
	//TWIWrite(hardwareStatus);
	TWIWrite(readCommand);
	if (TWIGetStatus() != 0x28)
	{
		uart_puts("Can't write: ");
		uart_putc(TWIGetStatus());
		return ERROR;
	}
	uart_puts("write command \r\n");
	
	TWIStart();
	if (TWIGetStatus() != 0x10)
	{
		uart_puts("Can't start: ");
		uart_putc(TWIGetStatus());
		return ERROR;
	}
	uart_puts("read starts again\r\n");
	
	TWIWrite(readOnLow);
	if (TWIGetStatus() != 0x40)
	{
		uart_puts("Can't write: ");
		uart_putc(TWIGetStatus());
		return ERROR;
	}
	
	uint8_t readStatus = TWIReadACK();
	if (TWIGetStatus() != 0x50)
	{
		uart_puts("Can't read: ");
		uart_putc(TWIGetStatus());
		return ERROR;
	}
	
	while (!readStatus)
	{
		readStatus = TWIReadACK();
	}
	
	if (readStatus == 0x01)
	{
		uart_puts("right status\r\n");
	}
	
	uint8_t readByteData;
	for (int i=0; i<bytesOfCommandData; i++)
	{
		readByteData = TWIReadACK();
		if (TWIGetStatus() != 0x50)
		{
			uart_puts("Can't read: ");
			uart_putc(TWIGetStatus());
			return ERROR;
		}
		uart_puts(byte_to_binary(readByteData));
		uart_puts("\r\n");
	}

	/*
	if (bytesOfCommandData == 1)
	{
		uint8_t readHardwareStatus = TWIReadACK();
		if (TWIGetStatus() != 0x50)
		{
			uart_puts("Can't read: ");
			uart_putc(TWIGetStatus());
			return ERROR;
		}
		uart_puts(byte_to_binary(readHardwareStatus));
		uart_puts("\r\n");
	} else if (bytesOfCommandData == 16)
	{
		uint8_t readFirmwareVersion;
		for (int i=0; i<16; i++)
		{
			readFirmwareVersion = TWIReadACK();
			if (TWIGetStatus() != 0x50)
			{
				uart_puts("Can't read: ");
				uart_putc(TWIGetStatus());
				return ERROR;
			}
			uart_puts(byte_to_binary(readFirmwareVersion));
			uart_puts("\r\n");
		}
	}
	*/
	
	TWIStop();
	uart_puts("read end \r\n");
	return SUCCESS;
}

