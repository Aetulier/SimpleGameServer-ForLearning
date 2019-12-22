#include"User.h"
#include<string>

User::User(int id, std::string username, std::string password) {

    this->Id = id;
	this->Username = username;
	this->Password = password;
}