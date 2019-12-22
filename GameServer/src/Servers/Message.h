#pragma once

#ifndef __Message_H__ 
#define __Message_H__ 

#include"../Common/ActionCode.h"
#include"../Common/RequestCode.h"
#include<string>
#include<cstring>

#define Length_m 1024
#define byte unsigned char

class Client;

class Message
{
public:
	//Message();
	byte date[Length_m];
	int StartIndex();
	int RemainSize();
	void ReadMessage(int newDateAmount, Client* client);
    byte * PackData(Common::ActionCode actionCode, std::string data);
private:
	int startIndex = 0;
};

#endif