#pragma once

#include<string>
#include"../Common/RequestCode.h"
#include"../Common/ReturnCode.h"
#include"../Common/RoleType.h"
#include "../Servers/Server.h"
#include"../Servers/Client.h"
#include"ControllerManager.h"

class  BaseController {

public:

	Common::RequestCode RequestCode();
	virtual std::string DefaultHandle(Common::ActionCode actionCode,std::string data, Client* client, Server* server)=0;

	std::string GetClassName_m(Common::RequestCode requestCode);

protected:
	Common::RequestCode requestCode= Common::RequestCode::None_R;	
};