//Server

#pragma comment(lib,"ws2_32.lib")
#define _WINSOCK_DEPRECIATED_NO_WARNINGS
#pragma warning(disable:4996) 
#include <WinSock2.h>
#include <iostream>


SOCKET Connections[100];
int TotalConnections = 0; //Holds the other connections

enum Packet {
	P_ChatMessage

};

bool ProcessPacket(int ID, Packet packettype) 
{
	switch (packettype)
	{
	case P_ChatMessage:
	{
		int bufferlength;
		//receive the connection
		recv(Connections[ID], (char*)&bufferlength, sizeof(int), NULL); //get bufferlength
		char *buffer = new char[bufferlength]; //Allocate buffer
		recv(Connections[ID], buffer, bufferlength, NULL); //get buffer message from client
		for (int i = 0; i < TotalConnections; i++) { //For each client connection
			if (i == ID) //skip the user who sends the message
				continue;
			Packet chatmessagepacket = P_ChatMessage; //create chat message packet to be sent
			send(Connections[ID], (char*)&chatmessagepacket, sizeof(Packet), NULL); //send chat message packet
			send(Connections[i], (char*)&bufferlength, sizeof(int), NULL); //send the buffer length
			send(Connections[i], buffer, sizeof(buffer), NULL); //send the chat message to the client
		}
		delete[] buffer;
		break;
	}
	default:
		std::cout << "Unrecognizable packet" << packettype << std::endl;
	}
	return true;
}

void ClientHandlerThread(int ID) {
	Packet packettype;

	while (true) 
	{
		recv(Connections[ID], (char*)&packettype, sizeof(Packet), NULL); //Receive packet type from client

		if (!ProcessPacket(ID, packettype)) //if the packet is not properly processed
			break; //break out of our client handler loop
	}

	closesocket(Connections[ID]); //close the socket that was being used by the client;
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
	int connectionCounter = 0; // client's ID
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
			Connections[i] = newConnection; //set socket in array to be the newest connection before creating the thread to handle the client's request
			TotalConnections++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(i), NULL, NULL);
			Packet chatmessagepacket = P_ChatMessage;
			send(Connections[i], (char*)&chatmessagepacket, sizeof(Packet), NULL); //send chat message packet
			std::string buftest = "Welcome!";
			int size = buftest.size();
			send(newConnection, (char*)&size,sizeof(int), NULL); //send size of message
			send(newConnection, buftest.c_str(), buftest.size(), NULL); //Send Message
			//Create thread to handle this client
		}
	}
	system("pause");


	return 0;
}
