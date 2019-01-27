//Client
#pragma warning(disable:4996) 
#define _WINSOCK_DEPRECIATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <string>


SOCKET Connection; //the client's connection to the server

enum Packet {
	P_ChatMessage

};

bool ProcessPacket(Packet packettype) {
	switch (packettype) {
	case P_ChatMessage:
	{
		int bufferlength; //holds the length of the message we are receiving
		recv(Connection, (char*)&bufferlength, sizeof(int), NULL); //receive the buffer's length
		char *buffer = new char[bufferlength]; //allocate buffer'
		recv(Connection, buffer, bufferlength, NULL);
		std::cout << buffer << std::endl;
		recv(Connection, buffer, bufferlength, NULL);
		delete[] buffer; //deallocate buffer
		break;
	}
	default:
		std::cout << "Unrecognized packet" << packettype << std::endl;
		break;
	}
	return true;
}

void ClientThread() {
	Packet packettype;
	while (true) {
		recv(Connection, (char*)&packettype, sizeof(Packet), NULL);

		if (!ProcessPacket(packettype)) //if the packet is not properly processed
			break; //break out of our client handler loop
	}

	closesocket(Connection);
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
	int sizeofaddr = sizeof(addr); //length of the address (required for the accept call)
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //broadcast the server/sockets locally
	addr.sin_port = htons(1111); //port
	addr.sin_family = AF_INET; //IPv4 socket

	Connection = socket(AF_INET, SOCK_STREAM, NULL); //Set connection socket
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) {
		//if we are unable to connect
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return 0; //failed to connect
	}

	std::cout << "Connected" << std::endl;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);
	
	std::string userinput; //holds the user's chat message
	//string Something = "Hello World";
	while (true) {
		std::getline(std::cin,userinput); //Get line if the user presses enter and fill the buffer
		int bufferlength = userinput.size(); //Find buffer length
		Packet packettype = P_ChatMessage; //Create packet type: Chat Message to be sent to the server
		send(Connection, (char*)&packettype, sizeof(Packet), NULL);
		send(Connection, (char*)&bufferlength, sizeof(int), NULL); //send the buffer length
		send(Connection, userinput.c_str(), bufferlength, NULL); //send the chat message to the client
		Sleep(10);
	}
	return 0;
}