#pragma once

#ifndef __UserController_H__ 
#define __UserController_H__ 

#include"BaseController.h"
#include"../DAO/UserDAO.h"
#include"../DAO/ResultDAO.h"

class UserController :BaseController
{
public:

	UserController();

	std::string DefaultHandle(Common::ActionCode actionCode,std::string data, Client* client, Server* server) ;
	std::string Login(std::string data, Client* client, Server* server);
	std::string Register(std::string data, Client* client, Server* server);

private:
	
	UserDAO userDAO;
	ResultDAO resultDAO;

	
};

REGISTER(UserController);


// RegisterAction g_RegisterAction_UserController("GameController",(PCreateObject)CreateObjectUser);


#endif