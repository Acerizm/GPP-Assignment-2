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