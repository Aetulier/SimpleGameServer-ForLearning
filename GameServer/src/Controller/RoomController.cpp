#include"RoomController.h"



RoomController::RoomController() {

	requestCode = Common::Room;
}

std::string RoomController::DefaultHandle(Common::ActionCode actionCode,std::string data, Client* client, Server* server) {
	
	switch (actionCode)
	{
	case Common::None_A:
		return "";
		break;
	case Common::ListRoom:
		return ListRoom(data, client, server);
		break;
	case Common::CreateRoom:
		return CreateRoom(data, client, server);
		break;
	case Common::JoinRoom:
		return JoinRoom(data, client, server);
		break;
	case Common::QuitRoom:
		return QuitRoom(data, client, server);
		break;
	default:
		return "";
		break;
	}


}
//创建游戏房间
std::string RoomController::CreateRoom(std::string data, Client* client, Server* server) {

	server->CreateRoom(client);
	return std::to_string((int)Common::Success) + "," + std::to_string((int)Common::Blue);
}
//获取游戏房间列表
std::string RoomController::ListRoom(std::string data, Client* client, Server* server) {

	std::string sb;
	std::vector<Room *> vecRoom;
	vecRoom = server->GetRoomList();
	Room* p;
	std::vector<Room *>::iterator i;
	for (i = vecRoom.begin(); i != vecRoom.end(); ++i) {
		p = *i;
		if (p->IsWaitingJoin())
		{
			sb.append(p->GetHouseOwnerData() + "|");
		}
	}
	if (sb.length()==0)
	{
		sb.append("0");
	}
	else 
	{
		sb=sb.substr(0, sb.length() - 1);
	}
	return sb;
}
//加入游戏房间
std::string RoomController::JoinRoom(std::string data, Client* client, Server* server) {

	int id = std::stoi(data);
	Room* room = server->GetRoomById(id);
	if (room == nullptr) {
		return std::to_string((int)Common::NotFound);
	}
	else if (room->IsWaitingJoin() == false) {
		return std::to_string((int)Common::Fail);
	}
	else
	{
		room->AddClient(client);
		std::string roomData = room->GetRoomData();
		room->BroadcastMessage(client, Common::UpdateRoom, roomData);

		return  std::to_string((int)Common::Success) + "," + std::to_string((int)Common::Red)+"-"+roomData;
	}
}
//退出房间
std::string RoomController::QuitRoom(std::string data, Client* client, Server* server) {

	bool isHouseOwner = client->IsHouseOwner();
	Room *room = client->room;
	if (isHouseOwner)
	{
		room->BroadcastMessage(client, Common::QuitRoom, std::to_string((int)Common::Success));
		room->Close();
		return std::to_string((int)Common::Success);
	}
	else
	{
		client->room->RemoveClient(client);
		room->BroadcastMessage(client, Common::UpdateRoom, room->GetRoomData());
		return std::to_string((int)Common::Success);
	}
}