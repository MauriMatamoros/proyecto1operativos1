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

#define Port 8080

using namespace std;

void *req(void *client);

struct Params {
	char* request;
	char* route;
	char* payload;
};

int main(int argc, char const *argv[]) {
	vector<pthread_t> ThreadPool;
	FILE *html_data;
	//r - read nada mas
	html_data = fopen("index.html","r");
	char response_data[1024];
	//para leer el archivo y meterlo a response data
	fgets(response_data, 1024,html_data);
	// cout<<response_data<<endl;
	

	char http_header[2048]="HTTP/1.1 200 OK\r\n\n";
	strcat(http_header,response_data);


	fclose(html_data);
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
			pthread_t thread;
			pthread_create(&thread, NULL, req, (void*) &Client);
			pthread_join(thread, NULL);
		}
		if(Client < 0 ){
			cout << "Error conectando al server" << endl;
		}
		//mandar data al cliente con sendt
		

	}
  	close(Server);
	return 0;
}

void* req(void* Client) {
	sleep(0.1);
	cout << "client thread" << endl;
	int connection = *((int*) Client);
	char buffer[999999];
	int received, fileDirectory, bytesRead;
	
	
	received = recv(connection, buffer, 99999, 0);
	// char mime*;
	// mime = buffer;
	// cout<< mime<< endl;
	Params parametros;
	parametros.request = strtok(buffer, " ");
	parametros.route = strtok(NULL, " ");
	parametros.payload = strtok(NULL, " \t\n");
	// cout << "request: " << parametros.request<<endl;
	// cout << "route: " << parametros.route<<endl;
	// cout<< "payload: " << parametros.payload << endl;
	if(strcmp(parametros.request, "GET") == 0) {


		FILE *html_data;
		html_data = fopen("index.html","r");
		char response_data[1024];
		fgets(response_data, 1024,html_data);
		char http_header[2048]="HTTP/1.1 200 OK\r\n\n";
		strcat(http_header,response_data);
		send(*((int*)Client),http_header,sizeof(http_header),0);
		fclose(html_data);
	}else if(strcmp(parametros.request, "POST") == 0) {
		char* UncleanKey;
		char* Value;
		UncleanKey = strtok(parametros.route, "=");
		Value = strtok(NULL,"");
		int size = strlen(UncleanKey);
		size-=2;
		char Key[size];
		for (int i = 2; i < size+2; ++i){
			Key[i-2] = UncleanKey[i];
		}
		cout<<"Key: "<< Key<<endl;
		cout<<"Value: "<< Value<<endl;
		char html [2048]="HTTP/1.1 200 OK\r\n\n";
		strcat(html,"<!DOCTYPE html><html><head><title>Linux Server</title></head><body><h1>Titulo: ");
		strcat(html, Key);
		strcat(html, "</h1><br/><p>Cuerpo: ");
		strcat(html, Value);
		strcat(html, "</p></body><br/></html>");


		send(*((int*)Client),html,sizeof(html),0);
		
		


	}else if(strcmp(parametros.request, "PUT") == 0 ){
		FILE *html_data;
		html_data = fopen("index.html","r");
		char response_data[1024];
		fgets(response_data, 1024,html_data);
		char http_header[2048]="HTTP/1.1 200 OK\r\n\n";
		strcat(http_header,response_data);
		send(*((int*)Client),http_header,sizeof(http_header),0);
		fclose(html_data);


	}else{
		cout << "ERROR 404" << endl;
	}
	// cout<< "request: " <<parametros.request<<endl;
	// cout<< "route: " <<parametros.route<<endl;
	// cout<< "payload: " <<parametros.payload<<endl;
	// send(connection, buffer, 16, 0);	

	// write(connection, buffer, 16);
	shutdown (connection, SHUT_RDWR);
	close(connection);
	connection--;
}
