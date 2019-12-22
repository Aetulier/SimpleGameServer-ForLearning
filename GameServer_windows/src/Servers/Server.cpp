#include<iostream>
#include"Server.h"
#include"../Controller/BaseController.h"

void Server::Start() {

    pool =new ThreadPool(6);
	socket_m.server = this;
	socket_m.SocketInit();
}


void Server::AddClient(SOCKET  c, LPSOCKET_INFORMATION SocketInfo, DWORD i) {
	
	Client* client=new Client(c, this,&socket_m,i);
	client->clientSI = SocketInfo;	
	clientVec_t.push_back(client);
	pool->submit([client]() {client->Start();});
}

void Server::ClientRecData(LPSOCKET_INFORMATION SocketInfo, char* data) {

	Client* client;
	std::vector<Client*>::iterator i;
	for (i = clientVec_t.begin(); i != clientVec_t.end();++i) {
		client = *i;
		if (client->clientSI == SocketInfo) {
			client->Rece(data);
		}
	}

}



void Server::RemoveSocket(SOCKET  c) {

	Client* client;
	std::vector<Client*>::iterator i;
	for (i = clientVec_t.begin(); i != clientVec_t.end();++i)
	{
		client = *i;
		if (client->clientSocket == c) {

			RemoveClient(client);
			break;
		}
	}

}


void Server::RemoveClient(Client* client) {

	std::vector<Client*>::iterator i;
	for ( i = clientVec_t.begin(); i !=clientVec_t.end();)
	{
		if (*i==client) {
			delete client;
			i = clientVec_t.erase(i);			
			break;
		}
		else
		{
			++i;
		}
	}
}

void Server::SendResponse(Client* client, Common::ActionCode actionCode, std::string data) {

	client->Send(actionCode, data);
}
void Server::HandleRequest(Common::RequestCode requestCode, Common::ActionCode actionCode, std::string data, Client* clien) {

	std::string s;
	s= baseCon->GetClassName_m(requestCode);
	if (s.empty() != true) {
		baseCon = (BaseController*)ClassFactory::getInstance().CreateObjectByName(s);
		std::string sendData;
		sendData=baseCon->DefaultHandle(actionCode,data, clien, this);
		if(!sendData.empty())
		SendResponse(clien, actionCode, sendData);		
	}
}

void Server::CreateRoom(Client* client) {

	Room* room=new Room(this);
	room->AddClient(client);
	RoomVec_t.push_back(room);
}
void Server::RemoveRoom(Room* room) {

	if (RoomVec_t.size() > 0 && room != nullptr) {

		std::vector<Room*>::iterator i;
		for (i = RoomVec_t.begin(); i != RoomVec_t.end();)
		{
			if (*i == room) {
				i = RoomVec_t.erase(i);
				delete room;
				break;
			}
			else
			{
				++i;
			}
		}

	}

}

std::vector<Room*> Server::GetRoomList() {
	return RoomVec_t;
}

Room* Server::GetRoomById(int id) {

	Room * p=nullptr;
	std::vector<Room*>::iterator i;
	for (i = RoomVec_t.begin(); i != RoomVec_t.end(); ++i)
	{
		p = *i;
		if (p->GetId()==id)
		{		   
			break;
		}
		else
		{
			p = nullptr;		
		}
	}
	
	return p;
}


