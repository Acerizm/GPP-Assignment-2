//Client
#pragma warning(disable:4996) 
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <iostream>

SOCKET Connection;

void ClientThread() {
	char buffer[256];
	while (true) {
		recv(Connection, buffer, sizeof(buffer), NULL);
		std::cout << buffer << std::endl;
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
	
	char buffer[256];
	while (true) {
		std::cin.getline(buffer, sizeof(buffer));
		send(Connection, buffer, sizeof(buffer),NULL);
		Sleep(10);
	}
	return 0;
}