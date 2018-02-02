
/*
Description:
Input:
*/

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>


using namespace std;


void error (const char* msg){
	perror(msg);
	exit(0);
}

int main(int argc, char* argv[]){
	if(argc != 3){
		cerr << "Usage: prog hostname port" << endl;
		exit(0);
	}
	const int MSG_SIZE = 501;
	char smsg[MSG_SIZE];
	char rmsg[MSG_SIZE];
	char* serverIP = argv[1];
	int s_port = atoi(argv[2]);
	int client, server_sock, numb;
	struct sockaddr_in serv_addr;
	struct hostent* server;
	bool run_state = true;
	string quit_c = "/quit";
	string ts = "ts1 ";
	string umsg;
	//client = socket(AF_INET, SOCK_STREAM, 0);
	if (client < 0)
		error("Error Opening Socket");
	else
		cout << "Socket created!" << endl;
	
	//clean struct mem
	bzero((char*) &serv_addr, sizeof(serv_addr));
		
	//set server address family	
	//serv_addr.sin_family = AF_INET;
	
	//sets the socket addr info in the 
	//struct for addresses inside the socket address struct
	//inet_pton(AF_INET, serverIP, &serv_addr.sin_addr);	
	//serv_addr.sin_port = htons(s_port);
	
	struct addrinfo hints, *res, *p;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	//hints.ai_flags = AI_PASSIVE;

	if(getaddrinfo(argv[1], argv[2], &hints, &res) != 0)
		cout << "ERROR GETTING ADDRESS" << endl;
	
	//client = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	
	printf("Arg addr:%s:%s\n",argv[1],argv[2]); 
	//if (connect(client, (struct sockaddr*) &serv_addr, sizeof(serv_addr))!=0)
	
	for(p = res; p!= NULL; p = p->ai_next){
		if((client = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
			perror("client : socket failed to aquire\n");
			continue;
		}
		if(connect(client, p->ai_addr, p->ai_addrlen) == -1){
		close(client);
		perror("client : connect, this socket did not work \n");
		//error("Error Connecting");	
		continue;
		}
		break;
	}
	if(p == NULL){
		fprintf(stderr, "Client: failed to connect\n");
		return 2;
	}
	
	cout << "Connection to the server with port: " << to_string(s_port) << endl;
	numb = recv(client, rmsg, MSG_SIZE-1, 0);
	rmsg[numb] = '\0';
	cout << "Connection confirmed" << endl; 	
	cout << rmsg << endl;	
	do{	
		cout << "Input message:" << endl;
		//cin >> smsg;
		getline(cin, umsg);
		cout << "sending" << endl;
		int result = send(client, umsg.c_str(), sizeof(umsg.c_str()), 0);
		printf("Amount of bytes sent: %d \n", result);
		cout << "sent" << endl;
		if(umsg.compare(quit_c) == 0){
			//close(client);
			cout << "quitting" << endl;
			run_state = false;
			break;
		}
		cout << "recieving" << endl;
		numb = recv(client, rmsg, MSG_SIZE, 0);
		rmsg[numb] = '\0';
		cout << rmsg << endl;


	}while(run_state == true);
	
	cout << "END OF FILE" << endl;	
	
	
}










