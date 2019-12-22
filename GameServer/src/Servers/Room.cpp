#include<thread>
#include <mutex>
#include"Room.h"
#include"Client.h"
#include"Server.h"


void Room::AddClient(Client* client)
{
	client->HP = MAX_HP;
	clientRoom.push_back(client);
	client->room = this;

	if (clientRoom.size()>= 2)
		state = WaitingBattle;
}

void Room::RemoveClient(Client* client) {

	client->room = nullptr;
	clientRoom.remove(client);
	if (clientRoom.size()>=2)
	{
		state = WaitingBattle;
	}
	else {
		state = WaitingJoin;
	}

}

bool Room::IsWaitingJoin() {

	return state==WaitingJoin;
}

//获取房主信息
std::string Room::GetHouseOwnerData() {

	Client *c=nullptr;
	if (clientRoom.size()!=0)
	{
		c = clientRoom.front();
	}

	return c->GetUserData();
}

int Room::GetId() {

	Client *c=nullptr;
	if (clientRoom.size() != 0)
	{
		c = clientRoom.front();
	}
	if (clientRoom.size()>0&&c!=nullptr)
	{
		return c->GetUserId();
	}
	return -1;
}

std::string Room::GetRoomData() {

	std::string sb;
	Client* p;
	std::list<Client*>::iterator i;
	for ( i = clientRoom.begin(); i!=clientRoom.end() ; ++i)
	{
		p = *i;
		sb.append(p->GetUserData() + "|");
	}
	if (sb.length() > 0) {

		sb.erase(sb.length()-1);
	}

	return sb;
}

void Room::BroadcastMessage(Client* excludeClient, Common::ActionCode actionCode, std::string data) {

	Client *p;
	std::list<Client*>::iterator i;
	for (i = clientRoom.begin(); i != clientRoom.end(); ++i) {		
		if (*i != excludeClient) {
			p = *i;
			server->SendResponse(p, actionCode, data);
		}
	}

}

bool Room::IsHouseOwner(Client* client) {
	
	return  client == clientRoom.front();
}

void Room::Close() {
	Client *p;
	for (auto i = clientRoom.begin(); i != clientRoom.end(); ++i) {
		p = *i;		
		p->room = nullptr;
	}
	server->RemoveRoom(this);
}

void  Room::QuitRoom(Client* client) {
	if (client == clientRoom.front())
		Close();
	else
		clientRoom.remove(client);

}
//开始游戏计时
void Room::StartTimer() {
	std::thread t(&Room::RunTimer,this);
	t.detach();
}

 void Room::RunTimer() {
	sleep(1);
	for (int i = 3; i > 0; i--)
	{		
		BroadcastMessage(nullptr, Common::ShowTimer, std::to_string(i));
		sleep(1);
		std::cout << i << " ";
	}
	BroadcastMessage(nullptr, Common::StartPlay, "r");
	return;
}

void Room::TakeDamage(int damage, Client* excludeClient) {

	Client *p;
	bool isDie = false;
	std::list<Client*>::iterator i;
	for (i = clientRoom.begin(); i != clientRoom.end(); ++i) {
		p = *i;
		if (p!= excludeClient)
		{
			if (p->TakeDamage(damage)) {
				isDie = true;
			}
		}	
	}
	if (isDie == false) return;

	for (i = clientRoom.begin(); i != clientRoom.end(); ++i) {
		p = *i;
		if (p->IsDie()) {
			p->Send(Common::GameOver, std::to_string((int)Common::Fail));
		}
		else
		{
			p->Send(Common::GameOver, std::to_string((int)Common::Success));
		}
	}
	Close();
}