#pragma once

#ifndef __Room_H__ 
#define __Room_H__ 

#include<list>
#include"../Common/ActionCode.h"
#include"../Common/ReturnCode.h"

class Client;
class Server;

enum  RoomType
{
	WaitingJoin,
	WaitingBattle,
	Battle,
	End
};
class Room
{
public:
	Room();
	Room(Server* server)
	{
		this->server = server;
	};

	void AddClient(Client* client);
	void RemoveClient(Client* client);
	bool IsWaitingJoin();
	std::string GetHouseOwnerData();
	int GetId();
	std::string GetRoomData();
	void BroadcastMessage(Client* excludeClient, Common::ActionCode actionCode, std::string data);
	bool IsHouseOwner(Client* client);
	void Close();
	void QuitRoom(Client* client);
	void StartTimer();
	void RunTimer();
	void TakeDamage(int damage, Client* excludeClient);

private:
	 const int MAX_HP = 200;
	 std::list<Client*> clientRoom;
	 RoomType state =WaitingJoin;
	 Server* server;
};
#endif