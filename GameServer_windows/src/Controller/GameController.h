#pragma once


#ifndef __GameController_H__ 
#define __GameController_H__ 

#include"BaseController.h"

class GameController:BaseController
{
public:

	GameController();

	std::string DefaultHandle(Common::ActionCode actionCode,std::string data, Client* client, Server* server);
	std::string StartGame(std::string data, Client* client, Server* server);
	std::string Move(std::string data, Client* client, Server* server);
	std::string Shoot(std::string data, Client* client, Server* server);
	std::string Attack(std::string data, Client* client, Server* server);


};
REGISTER(GameController);


//RegisterAction g_RegisterAction_GameController("GameController", (PCreateObject)CreateObjectGame);

#endif