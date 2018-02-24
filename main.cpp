//Librerias basicas
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
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

int main(int argc, char const *argv[]) {

  	vector<pthread_t> ThreadPool;
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
	cout<< "Esperando coneccion..."<<endl;
	listen(Server, 1);

	while(true){
		Client = accept(Server, (struct sockaddr*) &ServerAddress, &SocketSize);
		if(Client){
			//Crear pthread para ejecutar request y agregarlo a ThreadPool
			cout << "Cliente conectado" <<endl; //quitar
		}
		if(Client < 0 ){
			cout << "Error conectando al server" << endl;
		}
	}
  	close(Server);
	return 0;
}
