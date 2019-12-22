#include"UserDAO.h"
#include<iostream>


User* UserDAO::VerifyUser(MySqlConn conn, std::string username, std::string password) {

	try {
		std::string query = "select * from users where username=\"" + username + "\" and password=\"" + password + "\"";
		int id = conn.QueryIdData(query.c_str());
		if (id != -1) {

			User* user= new User(id, username, password);
			return user;
		}
		else
		{
			return nullptr;
		}
	}
	catch (const std::exception& e) {

		std::cout << e.what();
		return nullptr;
	}	
}

bool UserDAO::GetUserByUsername(MySqlConn conn, std::string username) {

	try {
		std::string query = "select * from users where username=\"" + username;
		int id = conn.QueryIdData(query.c_str());
		if (id != -1) {
	
			return true;
		}
		else
		{
			return false;
		}
	}
	catch (const std::exception& e) {

		std::cout << e.what();
		conn.CloseSql();
		return false;
	}

}
void  UserDAO::AddUser(MySqlConn conn, std::string username, std::string password) {

	try
	{
		std::string query = "insert into users set username =\""+ username +"\",password=\""+password+ "\"";
		conn.DataUpdate(query.c_str());
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
}