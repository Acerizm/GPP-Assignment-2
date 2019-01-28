#pragma once
#include "Client.h"

class GameClient
{
private:
	//variables	
	Client client;
public:
	// Constructor
	GameClient();

	// Destructor
	virtual ~GameClient() = 0;

	void setClient(Client value) {
		this->client = value;
	}



	
	

};