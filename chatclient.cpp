
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

// this function is meant to handle ensuring all of a message is sent
int sendall(int client, char* msg, int* msg_len){
	int total = 0;
	int remain = *msg_len;
	int n;
		
	while(total < *msg_len){
		n = send(client, msg+total, remain, 0);
		if(n == -1){
			break;
		}
		total += n;
		remain -= n;
	}
	*msg_len = total;
	if(n== -1){
		return -1;
	}
	else{
		return 0;
	}	
	

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
	string user_handle;
	//client = socket(AF_INET, SOCK_STREAM, 0);
	if (client < 0)
		error("Error Opening Socket");
	else
		cout << "Socket created!" << endl;
	
	//clean struct me
	bzero((char*) &serv_addr, sizeof(serv_addr));
		
	//sets the socket addr info in the 
	//struct for addresses inside the socket address struct
	struct addrinfo hints, *res, *p;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	//make sure address info is reasonable
	if(getaddrinfo(argv[1], argv[2], &hints, &res) != 0)
		cout << "ERROR GETTING ADDRESS" << endl;
	
	//printf("Arg addr:%s:%s\n",argv[1],argv[2]);//debug check addresses 

	/*Holy Smokes. So apparently when you want to get an addr
	setup it returns a linked list, so you need to iterate over it and figure out
	which struct is a valid one. I think in a prac app you would need to hand
	shake with the server better to avoid MiM attacks. You also need to check 
	if the socket you aquired is a valid socket before checking if it will yield 
	a connection. After that you need to check if your selected socket is real or if 
	iterated out of your list and have nothing instead.
	*/	

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
	
	//cout << "Connection to the server with port: " << to_string(s_port) << endl;
	numb = recv(client, rmsg, MSG_SIZE-1, 0);
	rmsg[numb] = '\0'; //you recv bytes. Gotta put the null ending in.
	//cout << "Connection confirmed" << endl; 	
	cout << rmsg << endl;	
	umsg.resize(501);
	cout << "Enter nickname" << endl;
	getline(cin, user_handle);
	
	user_handle.append(1,'>');
	//cout << "user handle appended" << endl;
	do{	
		
		cout << user_handle ;
		getline(cin, umsg);
	//	cout << "got first enrtry" << endl;
		if(umsg.compare(quit_c) == 0){
			//close(client);
			cout << "quitting" << endl;
			run_state = false;
			break;
		}
		//cout << "inserting user handle to msg" << endl;
 		umsg.insert(0,user_handle);
 		//cout << "insert done" << endl;
 		//cout << "input is: " << umsg << endl;
 		//loop here to ensure total message length was sent
		int* them_size; 
		size_t tim;
		//cout << "new int* made" << endl;
		tim  =  umsg.size();
		//cout << "tim set" << endl;
		*them_size = (int)tim;
		//cout << "setting new char *" << endl;
		char* out_msg = new char[umsg.length()+1];
		//cout << "string cpy" << endl;
		strcpy(out_msg, umsg.c_str()); 
		//cout << "out_msg: " << out_msg << endl;
		int result = sendall(client, out_msg, them_size);
		delete[] out_msg;
		//cout << "sent" << endl;
		//cout << "recieving: ";
		numb = recv(client, rmsg, MSG_SIZE, 0);
		rmsg[numb] = '\0';
		cout << rmsg << endl;


	}while(run_state == true);
	
	
	
}










