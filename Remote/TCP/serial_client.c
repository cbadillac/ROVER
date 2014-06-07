#ifndef __COMMON_INCLUDES__
#define __COMMON_INCLUDES__

  #include <arpa/inet.h>    
  #include <netinet/in.h>   
  #include <sys/socket.h>   // socket(), bind(), shutdown(), but not close() :S
  #include <sys/types.h>    // types_t :D
  #include <unistd.h>       //close() :S
  #include <termios.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <time.h>

#endif

#include "headers/protocolo.h"
#include "headers/serial_client.h"

int main( int argc, char* argv[]){
    
    if ( argc < 3)
    {
        printf("\nSe debe ingresar ip y puerto");
        
        return 1;
    }
    
    char port[20], ip[20];
    
    strcpy( port, argv[2]);
    strcpy( ip  , argv[1]);
    
    struct socket_info server = client_connection_setup( ip, atoi( port));
    
    if( server.error != 0)
	{
		printf("Error al configurar el socket\n");
        
		return 0;
	}
    
  printf(":: Arduino Communication Client\n\n");

  for(;;)
    {
        char send_buffer;
        char recv_buffer[100];
        
        send_buffer = getcha();
        
        if( send_all( server.socket, &send_buffer, 1, 0) <= 0)
        {
            connection_was_closed();
            
            break;
        }
        
        if( recv_all( server.socket, recv_buffer, 100, 0) <= 0)
        {
            connection_was_closed();
            printf("DEbbug  adqdas\n");
                
            break;
        }
        
        printf("Recivido: %s\n", recv_buffer); // Comentar
    }

  close( server.socket);

	return 0;
}
