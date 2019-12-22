#include"GameController.h"



GameController::GameController() {

	requestCode = Common::Game;	
}

std::string GameController::DefaultHandle(Common::ActionCode actionCode,std::string data, Client* client, Server* server) {
	switch (actionCode)
	{
	case Common::None_A:
		return "";
		break;
	case Common::StartGame:
		return StartGame(data, client, server);
		break;
	case Common::Move:
		return Move(data, client, server);
		break;
	case Common::Shoot:
		return Shoot(data, client, server);
		break;
	case Common::Attack:
		return Attack(data, client, server);
		break;

	default:
		return "";
		break;
	}
}
//开始游戏
std::string GameController::StartGame(std::string data, Client* client, Server* server) {

	if (client->IsHouseOwner()) {
		Room *room = client->room;
		room->BroadcastMessage(client, Common::StartGame, std::to_string(((int)Common::Success)));
		room->StartTimer();
		return  std::to_string((int)Common::Success);
	}
	else
	{
		return std::to_string((int)Common::Fail);
	}
}
//发送角色位置
std::string  GameController::Move(std::string data, Client* client, Server* server) {

	Room *room = client->room;
	if (room != nullptr)
		room->BroadcastMessage(client, Common::Move, data);
	return "";
}
//发送箭矢生成位置
std::string GameController::Shoot(std::string data, Client* client, Server* server) {
	Room* room = client->room;
	if (room != nullptr)
		room->BroadcastMessage(client, Common::Shoot, data);
	return "";

}
//击中目标
std::string GameController::Attack(std::string data, Client* client, Server* server) {

	int damage = std::stoi(data);
	Room* room = client->room;
	if (room == nullptr) return "";
	room->TakeDamage(damage, client);
	return "";
}