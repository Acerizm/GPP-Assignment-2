//Server

#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996) 
#include <WinSock2.h>
#include <iostream>


SOCKET Connections[100];
int ConnectionCounter = 0; //Holds the other connections


void ClientHandlerThread(int index) {
	char buffer[256];
	while (true) {
		//receive the connection
		recv(Connections[index], buffer, sizeof(buffer), NULL);
		for (int i = 0; i < ConnectionCounter; i++) {
			if (i == index) {
				//skip the user who sends the message
				continue;
			}
			send(Connections[i], buffer, sizeof(buffer), NULL);

		}
	}
}

int main() {

	//Winsock Startup
	WSADATA wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	SOCKADDR_IN addr; //Address that we will bind our listening socket to
	int addrlen = sizeof(addr); //length of the address (required for the accept call)
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //broadcast the server/sockets locally
	addr.sin_port = htons(1111); //port
	addr.sin_family = AF_INET; //IPv4 socket

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //Create socket to listen for new connections
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); //Bind the address to the socket
	listen(sListen, SOMAXCONN); //Places sListen socket in a state in which it is listening for an incoming
	//connection. NOTE:: SOMAXCONN = Socket Outstanding 

	SOCKET newConnection; //Socket to hold the client's connection
	//Add multiple connections
	for (int i = 0; i < 100; i++)
	{

		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //Accepts a new connection
		if (newConnection == 0) { //if accepting the client's connection failed
			std::cout << "Failed to accept client's connection" << std::endl;
		}
		else //if client connection properly accepted
		{
			std::cout << "Client Connected!" << std::endl;
			char MOTD[256] = " Welcome! This is the message of the day"; // Create buffer with message of the day
			send(newConnection, MOTD, sizeof(MOTD), NULL); //Send MOTD buffer;
			Connections[i] = newConnection;
			ConnectionCounter += 1;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(i), NULL, NULL); 
			//Create thread to handle this client
		}
	}
	system("pause");


	return 0;
}
