#pragma once
#include "Client.h"
#include <string>


class GameClient
{
private:
	//variables	
	Client *client;
public:
	// Constructor
	GameClient();

	// Destructor
	~GameClient();

	void createClient() {
		// 2.
		//pass in the ip address and the port number
		//change the ip address to match the server's private IP address
		//both clients and the server must be in the same network - obviously
		this->client = new Client("172.18.168.58", 1111);
	}

	void sendData(std::string message) {
		this->client->SendString(message);
	}

	int ConnectToServer() {
		if (!this->client->Connect()) {
			return 0; //return false
		}
		else
			return 1;

	}

	Client *getCurrentClient() {
		return this->client;
	}
	
	

};