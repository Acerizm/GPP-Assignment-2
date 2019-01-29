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
	int ID;
	float XCoordinate;
	float YCoordinate;




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

	bool SerializeData() {
		const string json = "{ \"ID\" :" + to_string(getID()) + "}";
		Document document;
		//parse the json to the dom
		//declare the characther array
		document.Parse(json.c_str());
		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		document.Accept(writer);



	}
};