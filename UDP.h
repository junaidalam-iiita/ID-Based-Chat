#ifndef UDP_HEADER_FILE
#define UDP_HEADER_FILE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <errno.h>     

#define MAXPAYLOAD  1024

using namespace std;

enum MESSAGE_TYPE{
	DNS_TYPE_1, DNS_TYPE_2, DNS_TYPE_3, 
	MyChat_TYPE_1, MyChat_TYPE_2
	// you can define your msg types here
};

struct header{
    enum MESSAGE_TYPE msg_type;
    int payload_length;
};

typedef struct{
    struct header info;
    unsigned char payload[MAXPAYLOAD];
}Message;

class UDP_Socket{
	int sockid;
	struct sockaddr_in myaddr;
	
	public:
		UDP_Socket();
		
		/* Use the following function only if you want to use this socket as a server socket. */
		void Initialize(int myport);
		
		/* Give the IP, Port no. of the Destnation Application / Entity and a pointer to the Message to be sent*/
		void sendMessage(const char * dest_IP, int dest_port, Message * msg);
		
		/* If any message arrives at this socket, the message will be pointed to msg, and the IP and Port no. of the sender Application / 
		Entity will also be copied to the first two arguments. */
		void recvMessage(char * source_IP, int * source_Port, Message * msg);
};


#endif /* UDP_HEADER_FILE */
