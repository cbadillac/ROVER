#ifndef __SERIAL_SERVER__
#define __SERIAL_SERVER__

/* GLOBAL DECLARATIONS */

/* FUNCTION DECLARATION */

void* thread_server_receive(const struct socket_info client_x);
void* thread_server_send(   const struct socket_info client_x);

#endif
