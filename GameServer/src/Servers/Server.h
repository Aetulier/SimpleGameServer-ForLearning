#pragma once

#ifndef __Server_H__ 
#define __Server_H__ 

#include"Socket.h"
#include"Room.h"
#include"Client.h"
#include"../DAO/ThreadPool.h"

class BaseController;

class Server
{
public:

	void Start();
	void AddClient(int  c, LPSOCKET_INFORMATION SocketInfo, unsigned long i);
	void ClientRecData(LPSOCKET_INFORMATION SocketInfo,char* data);
	void RemoveSocket(int c);
	void RemoveClient(Client *client);
	void SendResponse(Client* client, Common::ActionCode actionCode, std::string data);
	void HandleRequest(Common::RequestCode requestCode, Common::ActionCode actionCode, std::string data, Client* clien);
	void CreateRoom(Client* client);
	void RemoveRoom(Room* room);
	std::vector<Room*> GetRoomList();
	Room* GetRoomById(int id);
	Socket socket_m;

private:
	ThreadPool* pool;

	std::vector<Client*> clientVec_t;
	std::vector<Room*> RoomVec_t;
	std::vector<std::future<int>> results;
	bool TreadPoolIni = false;
	BaseController *baseCon=nullptr;
};


#endif