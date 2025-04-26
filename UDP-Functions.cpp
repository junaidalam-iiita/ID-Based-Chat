#include "UDP.h"

/* Definations for the Functions of the Class UDP_Socket. */

UDP_Socket::UDP_Socket(){
	sockid = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockid < 0){
		perror("socket creation failed");
		exit(1);
	}
}

void UDP_Socket::Initialize(int myport){
	sockid = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockid < 0){
		perror("socket creation failed");
		exit(1);
	}
	memset(&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = INADDR_ANY;
	myaddr.sin_port = htons(myport);
	if( bind(sockid,(struct sockaddr *)&myaddr, sizeof(myaddr)) <0){
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
}

void UDP_Socket::sendMessage(const char * dest_IP, int dest_port, Message * msg){
	struct sockaddr_in receiver;
	int sendsize, pay_size;
		    	
	pay_size = msg->info.payload_length;

	receiver.sin_family = AF_INET;
	receiver.sin_addr.s_addr = inet_addr(dest_IP);
	receiver.sin_port = htons(dest_port);
		
	sendsize = sendto(sockid, msg, sizeof(struct header) + pay_size, MSG_CONFIRM, (const struct sockaddr *) &receiver, sizeof(receiver));
	//cout << "Message Type-" << msg->info.msg_type << " sent to " << dest_IP << "::"<< dest_port << ". Size= " << sendsize << " bytes" << endl;
	if(sendsize < 0){
		perror("Message Sending failed");
		exit(1);
	}
}
		
void UDP_Socket::recvMessage(char * source_IP, int * source_Port, Message * msg){
	struct sockaddr_in source;
	int recvsize;
			
	socklen_t length = sizeof(source);
			
	memset(&source, 0, sizeof(source));
	recvsize = recvfrom(sockid, msg, sizeof(Message), MSG_WAITALL, (struct sockaddr *) &source, &length);
	strcpy(source_IP , inet_ntoa(source.sin_addr));
	*source_Port = ntohs(source.sin_port);
	//cout << "Message Type-" << msg->info.msg_type << " received from " << source_IP << "::" << *source_Port << ". Size = " << recvsize << " bytes" << endl;
}
