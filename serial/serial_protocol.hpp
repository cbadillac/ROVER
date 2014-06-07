#ifndef _SERIAL_PROTOCOL_H
#define _SERIAL_PROTOCOL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "arduino-serial.hpp"

#define START_DATA 128   //Byte de inicio de envio de datos PC -> Arduino
#define END_DATA 255     //Byte de fin de envio de datos PC -> Arduino
#define LARGO_PAQUETE  4
#define A 0 
#define B 1
#define AB 2

int fd = 0; 

void init_serial(void);

void send_packet(uint8_t,uint8_t,uint8_t,uint8_t);

char *lectura_basica ();

uint8_t *get_packet();

void Fwd(uint8_t,uint8_t);

void Rev(uint8_t,uint8_t);

void Stop(uint8_t);

void Float(uint8_t motor);

void Arm(uint8_t, int);

uint8_t Read_analog (uint8_t AN);

uint8_t Read_digital ();

void Set_digital (uint8_t,uint8_t); // Setea en HIGH O LOW Pin salida digital del microcontrolador

uint8_t atoi_new(char []);



//Inicializa comunicación serial y abre archivo arduino para poder escribir sobre él
void init_serial()
{
		
	fd = serialport_init("/dev/ttyUSB0", 57600);
	
	if (fd == -1)
		{
			fprintf (stderr,"No se puede abrir arduino.\n");
			exit (EXIT_FAILURE);
		}
	else printf ("Se ha abierto Arduino para leer y escribir\n"); 	
		
}

//Funciones para enviar paquete a arduino con datos para los motores
/*
    Manejo de datos
    
    Id: Dispositivo
         0 = Brazo
         1 = MotorDC 
    
    MOTOR DC 
    D1: Tipo de Movimiento 
         0: forward (hacia adelante)
         1: reverse (reversa)
         2: Stop
         3: Float (neutro)
    D2: Selecciones de Motor/es
         0: Motor A
         1: Motor B
         2: Motor AB
    D3: Velocidad (0-9)
    BRAZO:
    data[1]:
    data[2]:
    data[3]:
*/

void Fwd(uint8_t motor,uint8_t vel){
	
	uint8_t Id= 1, D1 = 0, D2 = motor, D3 = vel; 
	send_packet(Id,D1,D2,D3);

}
void Rev(uint8_t motor,uint8_t vel){

	uint8_t Id= 1, D1 = 1, D2 = motor, D3 = vel; 
	send_packet(Id,D1,D2,D3);

}
void Stop(uint8_t motor){

	uint8_t Id= 1, D1 = 2, D2 = motor, D3 = 0;	
	send_packet(Id,D1,D2,D3);

}

void Float(uint8_t motor){

	uint8_t Id= 1, D1 = 3, D2 = motor, D3 = 0; 
	send_packet(Id,D1,D2,D3);

}


void Arm(uint8_t servo, int angulo){

	uint8_t Id = 0;
	uint8_t D1 = servo;
	uint8_t D2 = (angulo>>7)&0x3f;
	uint8_t D3 = (angulo&0x7f);
	send_packet(Id,D1,D2,D3);
	
}
/*
AN: 0 , 1 , 2 ,3*/
uint8_t Read_analog (uint8_t AN)
{
	uint8_t *lectura_completa = (uint8_t *)malloc (sizeof (uint8_t)); 
	uint8_t Id = 2;
	uint8_t data;
	
	send_packet(Id,AN,0,0); 
	lectura_completa = get_packet();
	
	data = lectura_completa[1];
	free (lectura_completa);
	return data;
			
	
}

/* Lee entrada digital */
uint8_t Read_digital ()
{
	uint8_t *lectura_completa = (uint8_t *)malloc (sizeof (uint8_t)); 
	uint8_t Id = 3;
	uint8_t data;
	
	send_packet(Id,0,0,0);
	 
	lectura_completa = get_packet();
	
	data = lectura_completa[1];
	
	free (lectura_completa);
	return data;
		 
			
}

void Set_digital (uint8_t Dig,uint8_t Set)
{
	/*
	 Dig: 
	 	0 : Salida Digital 5 de arduino
	 	1: Salida Digital 2 de arduino
	*/
	//uint8_t *lectura_completa = (uint8_t *)malloc (sizeof (uint8_t));
	uint8_t Id = 4;
	send_packet(Id,Dig,Set,0);
	//lectura_completa = get_packet();
	//free (lectura_completa); 
}


//Enviando paquete de datos a arduino
void send_packet(uint8_t Id,uint8_t D1,uint8_t D2,uint8_t D3)
{
	int i;
	uint8_t data_array[7]; //Arreglo byte de datos hacia arduino
	data_array[0]= START_DATA;
	data_array[1]= Id;
	data_array[2]= D1;
	data_array[3]= D2;
	data_array[4]= D3;
	data_array[5]= END_DATA;
	//printf ("ENVIADO: ");
	
	//imprimiendo arreglo de bytes en archivo arduino
	for (i=0;i < 6;i++)
	{
     	serialport_writebyte (fd,data_array[i]); 
     	usleep(10); //0,5 [mseg]	
	}
		//printf("Envio paquete\n");
        usleep(100000); //100ms
}

char *lectura_basica ()
{
	//int i; 
	//char lee;
	char *lectura_completa = (char*)malloc (sizeof (char)); 	
	
	serialport_read_until(fd, lectura_completa, '_');
	
	usleep(500);
	
	//printf("%s", lectura_completa);
	return lectura_completa; 
	
}

/* Obtiene paquete enviado desde Arduino*/
uint8_t *get_packet()
{
	uint8_t *data = (uint8_t *)malloc (sizeof (uint8_t));
	//char lee; 
	int j=0;
	int i;
	int flag_init = 0; //Flag de inicio en 0
	int flag_fin = 0;  // Flag de fin en 0
	
	
	do  	
		{
					
			if (flag_init) 
			{
				data [j] = atoi_new(lectura_basica ());
				
				if (data [j-1] == END_DATA) flag_fin = 1; // Llego fin de datos ?
							
			}
			else 
			{
				i = atoi_new (lectura_basica ());
				
				//Llego inicio de datos ?
				if (i == START_DATA)
					{
						flag_init = 1;
						j = -1;
					}
			}
					
			j++;
			
		} while (!flag_fin && j<20);
			
		
		return data; 
}
	
	
uint8_t atoi_new(char s[])
{

	uint8_t i,n;
	n = 0;
	
	for( i=0 ; s[i] >= '0' && s[i]<='9'; ++i)
		{
			n = n*10+(s[i]-'0');
		}
	return n;

}

#endif

