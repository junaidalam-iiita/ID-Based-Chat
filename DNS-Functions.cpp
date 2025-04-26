
#include "DNS-Header.h"

/* Definations for the Functions of the Class User_DB. */

int User_DB::Register_User(const char * ID, const char * IP, const char * Port){
	int i, indx, f=-1;
	for (i = 0; i < user_next; i++){
		if(strcmp(users[i].ID, ID) == 0) {
			f = 1;
			break;
		}
	}
	if(f < 0) indx = user_next++;
	else indx = i;
	cout << "Entring in indx : " << indx << " ID = " << ID << " IP = " << IP << " Port = " << Port << endl;
	strcpy(users[indx].ID, ID);
	strcpy(users[indx].IP, IP);
	strcpy(users[indx].Port, Port);
	return f;
}

int User_DB::Get_Address(const char * ID, user_data * addr){
	int i, f=-1;
	for (i = 0; i < user_next; i++){
		if(strcmp(users[i].ID, ID) == 0) {
			f = 1;
			break;
		}
	}
	memcpy(addr, &(users[i]), sizeof(struct user_data));
	return f;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Definations for the Functions of the Class My_DNS. */

My_DNS::My_DNS(char * DNS_IP, int DNS_Port, UDP_Socket mysock){
	strcpy(IP, DNS_IP);
	Port = DNS_Port;
	sockid = mysock;
}
	
void My_DNS::Register(char * myID){
	Message outmsg, inmsg;
	DNSPayloadType1 p1;
	DNSPayloadType2 p2;
	char source_IP[20];
	int source_Port;
		
	p1.f = 0;
	strcpy(p1.ID, myID);
	outmsg.info.msg_type = DNS_TYPE_1;
	outmsg.info.payload_length = sizeof(p1);
	memcpy(&outmsg.payload[0], &p1, sizeof(p1));
		
	sockid.sendMessage(IP, Port, &outmsg);
	
	sockid.recvMessage(source_IP, &source_Port, &inmsg);
	memcpy(&p2, &inmsg.payload[0], sizeof(p2));
	cout << p2.response << endl;
}

void My_DNS::Register(char * myID, char * myPort){
	Message outmsg, inmsg;
	DNSPayloadType1 p1;
	DNSPayloadType2 p2;
	char source_IP[20];
	int source_Port;
	
	p1.f = 1;
	strcpy(p1.ID, myID);
	strcpy(p1.Port, myPort);
	outmsg.info.msg_type = DNS_TYPE_1;
	outmsg.info.payload_length = sizeof(p1);
	memcpy(&outmsg.payload[0], &p1, sizeof(p1));
		
	sockid.sendMessage(IP, Port, &outmsg);
	
	sockid.recvMessage(source_IP, &source_Port, &inmsg);
	memcpy(&p2, &inmsg.payload[0], sizeof(p2));
	cout << p2.response << endl;
}
	
void My_DNS::GetAddress(char * peerID, DNSPayloadType3 * p3){
	Message outmsg, inmsg;
	DNSPayloadType1 p1;
	char source_IP[20];
	int source_Port;
		
	strcpy(p1.ID, peerID);
	outmsg.info.msg_type = DNS_TYPE_2;
	outmsg.info.payload_length = sizeof(p1);
	memcpy(&outmsg.payload[0], &p1, sizeof(p1));
		
	sockid.sendMessage(IP, Port, &outmsg);
		
	sockid.recvMessage(source_IP, &source_Port, &inmsg);
	memcpy(p3, &inmsg.payload[0], sizeof(DNSPayloadType3));
}

