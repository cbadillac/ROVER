#ifndef __SERIAL_CLIENT__
#define __SERIAL_CLIENT__

/* GLOBAL DEFINITIONS */

struct mutex_data{
    char* message_from_client[__MAX_MSG_SIZE];
};

/* FUNCTION DECLARATION */

void* thread_client_send(   const struct socket_info server_x);
void* thread_client_receive(const struct socket_info server_x);

#endif
