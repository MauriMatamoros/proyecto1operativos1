//Librerias basicas
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <unistd.h>

//Threads y sockets
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
	char *address;
	address = argv[1];

	int Client;
	Client = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in remote_address;
	remote_address.sin_family = AF_INET;
	remote_address.sin_port = htons(80);
	inet_aton(address, &remote_address.sin_addr);

	connect(Client, (struct sockaddr * )&remote_address, sizeof(remote_address));

	char request[] = "GET / HTTP/1.1\r\n\r\n";
	char response[4096];

	send(Client, request, sizeof(request), 0);
	recv(Client, &response, sizeof(response), 0);

	printf("response: %s\n", response);
	close(Client);

	return 0;
}
