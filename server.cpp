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

#define Port 8000

using namespace std;

void *req(void *client);

int main(int argc, char const *argv[]) {

	FILE *html_data;
	//r - read nada mas
	html_data = fopen("index.html","r");
	char response_data[1024];
	//para leer el archivo y meterlo a response data
	fgets(response_data, 1024,html_data);
	cout<<response_data<<endl;
	vector<pthread_t> ThreadPool;

	char http_header[2048]="HTTP/1.1 200 OK\r\n\n";
	strcat(http_header,response_data);



	int Client;
	int Accept = 1;
	char buffer[256]; //Buffer para recibir mensajes

	//Server: Address domain del socket, Tipo de socket y Protocolo (0 indica que el sistema elige el mejor)
	int Server = socket(AF_INET, SOCK_STREAM, 0);
	if(Server < 0){
		cout << "ERROR" <<endl;
		return 1;
	}else{
		cout << "Socket abierto con exito" <<endl;
	}

	struct sockaddr_in ServerAddress;
	socklen_t SocketSize;
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htons(INADDR_ANY);
	ServerAddress.sin_port = htons(Port);
	if (setsockopt(Server, SOL_SOCKET, SO_REUSEADDR, &Accept, sizeof(Accept)) == -1) {
		cout << "Error seteando opciones del socket" <<endl;
		return 1;
	}

	if ((bind(Server, (struct sockaddr*) &ServerAddress, sizeof(ServerAddress))) < 0) {
		perror("Error on bind");
		return 1;
	}

	SocketSize = sizeof(ServerAddress);
	cout<< "Esperando conexion..."<<endl;
	listen(Server,10);

	while(true){
		Client = accept(Server, (struct sockaddr*) &ServerAddress, &SocketSize);
		if(Client){
			send(Client,http_header,sizeof(http_header),0);
			pthread_t thread;
			pthread_create(&thread, NULL, req, (void*) &Client);
		}
		if(Client < 0 ){
			cout << "Error conectando al server" << endl;
		}
		//mandar data al cliente con send
	}
  	close(Server);
	return 0;
}

void* req(void* Client) {
	sleep(0.1);
	std::cout << "client thread" << '\n';
	int connection = *((int*) Client);
	char buffer[999999], *requestLine[3], dataToSend[256], path[99999], *requestLine2[7];
	int received, fileDirectory, bytesRead;
	received = recv(connection, buffer, 99999, 0);
	cout<<buffer<<endl;
	send(connection, buffer, 16, 0);
	write(connection, buffer, 16);
	shutdown (connection, SHUT_RDWR);
	close(connection);
	connection--;
}
