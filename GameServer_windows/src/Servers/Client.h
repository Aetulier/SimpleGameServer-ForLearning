#pragma once

#ifndef __Client_H__ 
#define __Client_H__ 

#include <mutex>
#include"Socket.h"
#include"../Model/User.h"
#include"../Model/Result.h"
#include"Message.h"
#include"../DAO/mysql_m.h"

class Server;
class Room;

class Client
{
public:
	//Client();

	Client(SOCKET clientSocket, Server* server,Socket* socket_m, DWORD i) {

		this->clientSocket = clientSocket;
		this->server = server;
		this->socket_m = socket_m;
		this->clientNum = i;			
		mysqlConn.ConnectSql();
	};
	~Client()
	{
		printf("\nclientclean");		
	}
	SOCKET clientSocket;
	LPSOCKET_INFORMATION clientSI;
	DWORD clientNum;
	int HP;
	bool TakeDamage(int damage);
	bool IsDie();
	MySqlConn mysqlC();
	Room* room;
	void SetUserData(User* user, Result* result);
	 //Room room_m();
	std::string GetUserData();
	int GetUserId();
	void Start();
    void OnProcessMessage(Common::RequestCode requestCode, Common::ActionCode actionCode,std::string data);	
	void Close();
	void Rece(char* data);
	void Send(Common::ActionCode actionCode, std::string data);
	bool IsHouseOwner();

private:
	 Server* server;
	 Message msg;
	 MySqlConn mysqlConn;
	 Socket* socket_m;
	 User* user;
	 Result* result; 
	 std::mutex mtx;
	 FD_SET   WriteSet;        // 获取可写性的套接字集合	
	 FD_SET   ReadSet;        // 获取可读性的套接字集合
	 DWORD   SendBytes;        
	 DWORD   RecvBytes;         

};

#endif