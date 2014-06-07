#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>


#include "serial_protocol.c"
#include "headers/serial.h"

int statusError = 0;	// Error implementation

int s_init()
{
	init_serial();
	return 0;
}

int s_send(char c, uint8_t vel )
{
	switch(c)
	{
		case 'w':
		{
			Float(2);
			Fwd(2,vel);
			break;
		}

		case 's':
		{	Float(2);
			Rev(2,vel);
			break;
		}

		case 'a':
		{
			Float(2);
			Fwd(0,vel);
			Rev(1,vel);
			break;
		}

		case 'd':
		{
			Float(2);
			Fwd(1,vel);
			Rev(0,vel);
			break;
			
			
		}

		default:
		{
			Float(2);
			break;
		}
	}
    
    return statusError;
}

