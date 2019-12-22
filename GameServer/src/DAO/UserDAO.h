#pragma once

#include"../Model/User.h"
#include"mysql_m.h"
#include<string>

class UserDAO
{
public:

	User* VerifyUser(MySqlConn conn, std::string username, std::string password);
	bool GetUserByUsername(MySqlConn conn, std::string username);
	void AddUser(MySqlConn conn, std::string username, std::string password);
};
