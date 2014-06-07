#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "arduino-serial.c"

int fd = 0;
void init_serial()
{
        
    fd = serialport_init("/dev/ttyACM0", 9600);
    
    if (fd == -1)
        {
            fprintf (stderr,"No se puede abrir arduino.\n");
            exit (EXIT_FAILURE);
        }
    else printf ("Se ha abierto Arduino para leer y escribir\n");   
        
}

 int main(int argc, char const *argv[])
{
        printf("Entre\n");
    

    init_serial();
    int statusError;
    int i = 0;
    char msg = 8;

    while(1)
    {
    	int ex = 1;
        for ( i = 0; i < 256; i++)
        {
        	printf("hola");

            statusError = serialport_writebyte(fd,msg);
            usleep(10);
            if (statusError != 0)
            {
                printf("Error: No se pudo mandar el mensaje\n");
            }
            ex = ex +1;
            usleep(1000000);
        }
        
    }
    
    
    return 0;
}

