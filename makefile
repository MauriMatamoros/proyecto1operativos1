server: server.cpp
	g++ server.cpp -o server -pthread

client:	client.cpp
	g++ client.cpp -o client

clean:
	rm server client