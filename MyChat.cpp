// ./client alice/bob/KDC Port No.
#include "UDP.h"
#include "DNS-Header.h"

UDP_Socket mychatsock;
UDP_Socket mydnssock;

typedef struct {
	char senderID[20];
	char chat_mssg[50];
}ProtocolPayloadType1;

typedef struct {

}ProtocolPayloadType2;

void Process_Protocol_Message_Type_1(char * source_IP, int source_Port, Message inmsg);
void Process_Protocol_Message_Type_2(char * source_IP, int source_Port, Message inmsg);


void Listener(){
	char source_IP[20];
	int source_Port;
	Message inmsg;
	
	while(1){
		mychatsock.recvMessage(source_IP, &source_Port, &inmsg);
		
		switch(inmsg.info.msg_type){
			case MyChat_TYPE_1:
				//cout << " MyChat_TYPE_1 Message received from " << source_IP << "::" << source_Port <<  endl;
				Process_Protocol_Message_Type_1(source_IP, source_Port, inmsg);
				break;
			case MyChat_TYPE_2:
				//cout << " MyChat_TYPE_2 Message received from " << source_IP << "::" << source_Port <<  endl;
				Process_Protocol_Message_Type_2(source_IP, source_Port, inmsg);
				break;
			default:
				cout << "Error! Undefined Message Type Received !!!" << endl;
				break;
		}
	}
}


int main(int x, char * argv[]) {	
	char myID[20], peerID[20], mychatport[20];
	char DNS_IP[20]; int DNS_Port;
	char chat_msg[50];
	ProtocolPayloadType1 m1;
	Message outmsg;
	
	DNSPayloadType3 p3;
	
	if(x<3){
		cout<< "Error! Mention my ID/Name and Port No."<< endl;
		exit(1);
	}
	else{
		strcpy(myID, argv[1]);
		strcpy(mychatport, argv[2]);
		mychatsock.Initialize(stoi(mychatport));
	}
	
	
	
	cout << "Enter the IP, and Port no. of the DNS Server, seperated by a space" << endl;
	cin >> DNS_IP; cin >> DNS_Port;
	My_DNS mydns(DNS_IP, DNS_Port, mydnssock);
	
	mydns.Register(myID, mychatport);
	
	thread th(Listener);
	
	cout << "When You Wish to Send a Message, Enter the ID/Name and your Message Seperate by a Space:- " << endl;
	
	while(1){
		
		cin >> peerID;
		//cout << "ID is: " << peerID << endl;
		cin >> chat_msg;
		mydns.GetAddress(peerID, &p3);
		if(p3.f<0){
			cout << "No Such ID Exists!!!" << endl;
		}
		else{
			//cout << "IP and Port no. of " << peerID << " is: " << p3.IP << "::" << p3.Port << endl;
			strcpy(m1.senderID, myID);
			strcpy(m1.chat_mssg, chat_msg);
			outmsg.info.msg_type = MyChat_TYPE_1;
			outmsg.info.payload_length = sizeof(m1);
			memcpy(&outmsg.payload[0], &m1, sizeof(m1));
			mydnssock.sendMessage(p3.IP, stoi(p3.Port), &outmsg);
		}
		
		
	}
	
	
	th.join();
	return(0);
}

void Process_Protocol_Message_Type_1(char * source_IP, int source_Port, Message inmsg){
	ProtocolPayloadType1 m1;
	/* Extract the input data-structure from the input message */
	memcpy(&m1, &inmsg.payload[0], sizeof(m1));
	
	//  Implement your algo.
	cout << m1.senderID << " says: " << m1.chat_mssg << endl;
}

void Process_Protocol_Message_Type_2(char * source_IP, int source_Port, Message inmsg){


}


