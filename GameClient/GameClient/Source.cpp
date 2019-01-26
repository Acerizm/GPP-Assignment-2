//Client
#pragma warning(disable:4996) 
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <iostream>

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

	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL); //Set connection socket
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) {
		//if we are unable to connect
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return 0; //failed to connect
	}

	std::cout << "Connected" << std::endl;
	char MOTD[256];
	recv(Connection, MOTD, sizeof(MOTD), NULL); //Receive Message of the day buffer into the MOTD array
	std::cout << "MOTD: " << MOTD << std::endl;
	system("pause");

	return 0;
}