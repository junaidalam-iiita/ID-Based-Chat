//./server port_no

#include "DNS-Header.h"

UDP_Socket mysock;
User_DB db;

void Process_DNSMessage_Type_1(char * source_IP, int source_Port, Message inmsg);
void Process_DNSMessage_Type_2(char * source_IP, int source_Port, Message inmsg);

void StartServer(){
	char source_IP[20];
	int source_Port;
	Message inmsg;
	
	while(1){
		mysock.recvMessage(source_IP, &source_Port, &inmsg);
		
		switch(inmsg.info.msg_type){
			case DNS_TYPE_1:
				cout << "DNS_TYPE_1 Message received from " << source_IP << "::" << source_Port <<  endl;
				Process_DNSMessage_Type_1(source_IP, source_Port, inmsg);
				break;
			case DNS_TYPE_2:
				cout << "DNS_TYPE_2 Message received from " << source_IP << "::" << source_Port <<  endl;
				Process_DNSMessage_Type_2(source_IP, source_Port, inmsg);
				break;
			default:
				cout << "Error! Undefined Message Type Received !!!" << endl;
				break;
		}
	}
}

int main(int x, char * argv[]) {	
	if(x<2){
		cout<< "Error! Mention my port no."<< endl;
		exit(1);
	}
	else 
		mysock.Initialize(stoi(argv[1]));
	
	thread th(StartServer);
	
	
	th.join();
	return(0);
}

void Process_DNSMessage_Type_1(char * source_IP, int source_Port, Message inmsg){
	Message outmsg;
	DNSPayloadType1 p1;
	DNSPayloadType2 p2;
	int f;
	
	/* Extract the input data-structure from the input message */
	memcpy(&p1, &inmsg.payload[0], sizeof(p1));
	
	//  Implement your algo.
	if(p1.f)
		f= db.Register_User(p1.ID, source_IP, p1.Port);
	else
		f= db.Register_User(p1.ID, source_IP, to_string(source_Port).c_str());
		
	if(f<0)
		strcpy(p2.response, "New User Registered.");
	else 
		strcpy(p2.response, "User Details Updated.");
		
	/* Generete the response message and send it back to the requester */		
	outmsg.info.msg_type = DNS_TYPE_2;
	outmsg.info.payload_length = sizeof(p2);
	memcpy(&outmsg.payload[0], &p2, sizeof(p2));
	mysock.sendMessage(source_IP, source_Port, &outmsg);
}

void Process_DNSMessage_Type_2(char * source_IP, int source_Port, Message inmsg){
	Message outmsg;
	DNSPayloadType1 p1;
	DNSPayloadType3 p3;
	struct user_data usraddr;
	
	/* Extract the input data-structure from the input message */
	memcpy(&p1, &inmsg.payload[0], sizeof(p1));
	
	//  Implement your algo.
	if (db.Get_Address(p1.ID, &usraddr) < 0) p3.f = -1;
	else{
		p3.f = 1;
		strcpy(p3.IP, usraddr.IP);
		strcpy(p3.Port, usraddr.Port);
	}
	/* Generete the response message and send it back to the requester */
	outmsg.info.msg_type = DNS_TYPE_3;
	outmsg.info.payload_length = sizeof(p3);
	memcpy(&outmsg.payload[0], &p3, sizeof(p3));
	mysock.sendMessage(source_IP, source_Port, &outmsg);
}


