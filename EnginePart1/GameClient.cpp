
#include "GameClient.h"
bool GameClient::initialize() {
	myClient("127.0.0.1", 1111); //Create client to connect to server 127.0.0.1 [localhost] on port 1111
	if (!myClient.Connect()) //If client fails to connect...
	{
		return -1;
	}
}

void SendDataToServer(std::string value) {
	myClient.SendString(value); //Send string to server
}