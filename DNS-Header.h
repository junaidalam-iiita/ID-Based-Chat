#ifndef DNS_HEADER_FILE
#define DNS_HEADER_FILE

#include "UDP.h"

/* The Folloiwng Three Structures to be used for Communication Between a DNS-Client and a DNS Server */ 

typedef struct{
	int f;
	char ID[20];
	char Port[20];
}DNSPayloadType1;

typedef struct{
	char response[50];
}DNSPayloadType2;

typedef struct{
	int f;
	char IP[20];
	char Port[20];
}DNSPayloadType3;

/* The Following Two Data Structures to be Used by a DNS Server. */

struct user_data{
	char ID[20];
	char IP[20];
	char Port[20];
	void Print_Me(){
		cout << "ID= " << ID << ", IP= " << IP << ", Port= " << Port << endl;
	}
};

class User_DB{
	struct user_data users[100];
	int user_next = 0;
	public:
		/* Registers user with ID against the IP and Port No. as mentioned */
		int Register_User(const char * ID, const char * IP, const char * Port);
		/* Fills the addr structure with the corresponding ID, IP and Port details for the given user ID. Returns -1 if no such ID exists */
		int Get_Address(const char * ID, user_data * addr);
};

/* The Following Class to be used by a DNS Client */

class My_DNS{
	char IP[20];
	int Port;
	UDP_Socket sockid;
	public:
	/* Create a DNS Object with the given DNS-IP and DNS-Port no. and links it to my UDP Socket */
	My_DNS(char * DNS_IP, int DNS_Port, UDP_Socket mysock);
	
	/* Registers the Given ID into the DNS. Note that, the corresponding IP and Port no. are my own IP and Port no. */
	void Register(char * myID);
	void Register(char * myID, char * myPort);
	
	
	/* Retrieves the IP and Port No. corresponding to the given ID from the DNS. */
	void GetAddress(char * peerID, DNSPayloadType3 * p3);
};

#endif /* DNS_HEADER_FILE */
