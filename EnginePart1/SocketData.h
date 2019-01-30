#pragma once

#include <string>
#include "SocketData.h"
#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/stringbuffer.h"
#include <iostream> //for std::cout

using namespace std;
using namespace rapidjson;

class SocketData {
private:

	//attributes
	int ID = 1;
	float XCoordinate = 0;
	float YCoordinate = 0;
	int numOfPlayersVoted = 0;




public:

	SocketData() {};
	~SocketData() {};

	void setID(int id) {
		this->ID = id;
	}

	int getID() {
		return this->ID;
	}

	void setXCoordinate(float x) {
		this->XCoordinate = x;
	}

	float getXCoordinate() {
		return this->XCoordinate;
	}

	void setYCoordinate(float y) {
		this->YCoordinate = y;
	}

	float getYCoordinate() {
		return this->YCoordinate;
	}

	void setNumOfPlayersVoted(int value) {
		this->numOfPlayersVoted = value;
	}

	int getNumOfPlayesVoted() {
		return this->numOfPlayersVoted;
	}

	string getJsonData() {
		const string json = "{ \"id\":"  + to_string(getID()) + ",\"XCoordinate\":" + to_string(getXCoordinate()) + ",\"YCoordinate\":" + to_string(getYCoordinate()) + ",\"numOfPlayersVoted\":"+ to_string(getNumOfPlayesVoted()) + "}";
		return json;
	}

	Document getDocument(string value) {
		Document document;
		//parse the json to the dom
		//declare the characther array
		document.Parse(value.c_str());
		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		document.Accept(writer);
		return document;
	}

};