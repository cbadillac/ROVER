#ifndef __COMMON_INCLUDES__
#define __COMMON_INCLUDES__

  #include <arpa/inet.h>
  #include <netinet/in.h>
  #include <sys/socket.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <pthread.h>
  #include <stdio.h>
  #include <string.h>
  #include <time.h>
  #include <stdlib.h>

#endif

#include "headers/protocolo.h"
#include "headers/serial_server.h"
#include "headers/serial.h"

#define SEND_LENGTH 100

int main( int argc, char* argv[]){

	s_init();
	
	char port[5];
	if(argc >= 2)
		strcpy(port,argv[1]);
	else
	{
		printf("no se ingreso puerto del servidor, saliendo\n");
		return 0;
	}
    
    const struct socket_info server = server_connection_setup(atoi(port));

	if( server.error > 0)
	{
		printf("Error al configurar el socket\n");
        
		return 0;
	}
    
    printf("\nSocket configurado con exito\n");
    
     uint8_t vel = 80;
          uint8_t *velp = &vel;
    
    for(;;)
    {
		  struct socket_info client = server_connection_accept( server.socket);

		  printf("Conección aceptada : %#x.\n", client.socket_Addr.sin_addr.s_addr);
    
      for(;;)
      {
          //char send_buffer[100]="mensaje idle";//strlen( idle_msg)
          char recv_buffer, last_char=' ';
        
          if( recv( client.socket, &recv_buffer, 1, 0) <= 0)
          {
              connection_was_closed();
                
              break;
          }
        
          printf("Recibido: %c\n", recv_buffer);

         
          switch(recv_buffer)
          {
			case 'w':
			{
				char go[]="avanzando";
				send_all( client.socket, go, SEND_LENGTH, 0);
				s_send('w', vel);
				last_char = 'w';
				break;
			}
			case 'a':
			{
				char left[]="girando a la izquierda";
				send_all( client.socket, left, SEND_LENGTH, 0);
				s_send('a', vel);
				last_char = 'a';
				break;
			}
			case 's':
			{
				char back[]="retrocediendo";
				send_all( client.socket, back, SEND_LENGTH, 0);
				s_send('s', vel);
				last_char = 's';
				break;
			}
			case 'd':
			{
				char right[]="girando a la derecha";
				send_all( client.socket, right, SEND_LENGTH, 0);
				s_send('d', vel);
				last_char = 'd';
				break;
			}
			case '.':
			{
				char brake[]="acelerando";
				send_all( client.socket, brake, SEND_LENGTH, 0);
				*velp = (vel == 100)? vel: vel+10;
				s_send(last_char, vel);
				break;
			}
			case ',':
			{
				char brake[]="frenado";
				send_all( client.socket, brake, SEND_LENGTH, 0);
				*velp = (vel == 0)? vel: vel-10;
				s_send(last_char, vel);
				break;
			}
			default:
			{
				char nada[]="cualquier cosa";
				send_all( client.socket, nada, SEND_LENGTH, 0);
				s_send('l', vel);
				break;
			}
		   }
			

          /*if( send_all( client.socket, send_buffer, strlen( send_buffer), 0) <= 0)
          {
              connection_was_closed();
            
              break;
          }*/
      }
      
    }
    
    close( server.socket);//error: a label can only be part of a statement and a declaration is not a statement

	return 0;
}
