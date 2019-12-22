#include"UserController.h"


UserController::UserController() {

	requestCode = Common::User;
}

std::string UserController::DefaultHandle(Common::ActionCode actionCode,std::string data, Client* client, Server* server) {
	
	switch (actionCode)
	{
	case Common::None_A:
		return "";
		break;
	case Common::Login:
		return Login(data, client, server);
		break;
	case Common::Register:
		return Register(data, client, server);
		break;
	default:
		return "";
		break;
	}


}
//登录
std::string UserController::Login(std::string data, Client* client, Server* server) {

	std::string strs[2];
	auto i = data.find(',');
	strs[0] = data.substr(0, i);
	strs[1] = data.substr(i + 1);

	User * user = userDAO.VerifyUser(client->mysqlC(), strs[0], strs[1]);
	
	if (user==nullptr)
	{
		return std::to_string((int)Common::Fail);
	}
	else {
		Result* res = resultDAO.GetResultByUserid(client->mysqlC(), user->Id);
		client->SetUserData(user,res);
		return std::to_string((int)Common::Success) + "," + user->Username + "," + std::to_string(res->TotalCount) + "," + std::to_string(res->WinCount);
	}
	// delete user;
}
//注册
std::string UserController::Register(std::string data, Client* client, Server* server) {

	std::string strs[2];
	auto i = data.find(',');
	strs[0] = data.substr(0, i);
	strs[1] = data.substr(i + 1);

	std::string username = strs[0]; std::string password = strs[1];
	bool res = userDAO.GetUserByUsername(client->mysqlC(), username);
	if (res) {

		return  std::to_string((int)Common::Fail);
	}
	userDAO.AddUser(client->mysqlC(), username, password);
	return std::to_string((int)Common::Success);
}
