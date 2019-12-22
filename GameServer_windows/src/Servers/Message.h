#pragma once

#ifndef __Message_H__ 
#define __Message_H__ 

#include"../Common/ActionCode.h"
#include"../Common/RequestCode.h"
#include <windows.h>
#include<string>

#define Length_m 1024

class Client;

class Message
{
public:
	//Message();
	~Message()
	{
		printf("\nmessageclean");
	}
	byte date[Length_m];
	int StartIndex();
	int RemainSize();
	void ReadMessage(int newDateAmount, Client* client);
     byte * PackData(Common::ActionCode actionCode, std::string data);
private:
	int startIndex = 0;
};

#endif