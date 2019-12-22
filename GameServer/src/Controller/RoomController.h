#pragma once

#ifndef __RoomController_H__ 
#define __RoomController_H__ 


#include"BaseController.h"

class RoomController :BaseController
{
public:

	RoomController();


	std::string DefaultHandle(Common::ActionCode actionCode,std::string data, Client* client, Server* server);

	std::string CreateRoom(std::string data, Client* client, Server* server);
	std::string ListRoom(std::string data, Client* client, Server* server);
	std::string JoinRoom(std::string data, Client* client, Server* server);
	std::string QuitRoom(std::string data, Client* client, Server* server);

};
REGISTER(RoomController);

//RegisterAction g_RegisterAction_RoomController("GameController",(PCreateObject)CreateObjectRoom);

#endif