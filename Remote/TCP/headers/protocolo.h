#ifndef __PROTOCOLO__
#define __PROTOCOLO__

/* GLOBAL SETTINGS */
#define __SEND_DELAY_TIME   1           // [s]

#define __MAX_CONNECTIONS	1           // why more ?

#define __MAX_MSG_SIZE      100

/* GLOBAL VARIABLES DECLARATION*/

struct connection_state{
    uint8_t error;
    uint8_t some_other_thing;
};

struct socket_info{
	int socket;
	struct sockaddr_in socket_Addr;
	int8_t error;
};

static const char idle_msg[] = "algpo";    //message for idle connection (any better one?)

/* FUNCTION DECLARATION */

void* thread_server_connection( struct socket_info* client);
void* thread_client_connection( struct socket_info* server);

void connection_was_closed( void);

struct socket_info client_connection_setup( char* ip, unsigned short port);

struct socket_info server_connection_setup( unsigned short port);
struct socket_info server_connection_accept( int server_Socket);

size_t send_all(int sockfd, void *buffer, size_t len, int flags);
size_t recv_all(int sockfd, void *buffer, size_t len, int flags);

char getcha();

#endif
