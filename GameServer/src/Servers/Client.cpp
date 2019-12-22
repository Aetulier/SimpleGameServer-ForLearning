#include"Client.h"
#include"Server.h"

//造成伤害结算
bool Client::TakeDamage(int damage) {

	HP -= damage;

	if (HP<0)
	{
		HP = 0;
	}	
	if (HP <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//
bool Client::IsDie() {

	return HP <= 0;
}

MySqlConn Client::mysqlC() {
	return mysqlConn;
}
void Client::SetUserData(User* user, Result* result) {

	this->user = user;
	this->result = result;
}


std::string Client::GetUserData() {

	return std::to_string(user->Id) + "," + user->Username + "," + std::to_string(result->TotalCount) + "," + std::to_string(result->WinCount);
}

int Client::GetUserId() {

	return user->Id;
}

//每个客户端创建一个select进行接受发送数据
void Client::Start() {

	while (true)
	{

		mytime.tv_sec = 3; //select轮询间隔时间--秒
		mytime.tv_usec = 0;
		FD_ZERO(&ReadSet);
		FD_ZERO(&WriteSet);
		FD_SET(0, &ReadSet);
		if (max_fd < 0)
		{
			max_fd = 0;
		}
		FD_SET(clientSI->Socket, &ReadSet);
		FD_SET(clientSI->Socket, &WriteSet);
		if (max_fd < clientSI->Socket)
		{
			max_fd = clientSI->Socket;
		}

		if (select(max_fd+1, &ReadSet,NULL, NULL,&mytime)<0)//将NULL以形参传入Timeout，即不传入时间结构，就是将select置于阻塞状态，一定等到监视文件描述符集合中某个文件描述符发生变化为止.服务器会停到这里等待客户端相应
		{
			printf("select()   returned   with   error \n");
			return;
		}	

		if (FD_ISSET(clientSI->Socket,&ReadSet)) {
			socket_m->Total--;
			memset(clientSI->Buffer, ' ', DATA_BUFSIZE);            
			clientSI->RecvDataBuf.buf = clientSI->Buffer;           
			clientSI->RecvDataBuf.len = DATA_BUFSIZE;

			RecvBytes=recv(clientSI->Socket, clientSI->RecvDataBuf.buf, clientSI->RecvDataBuf.len, 0);
			if (RecvBytes<0)
			{				
					printf("Recv()   failed   with   error  \n");
					server->socket_m.FreeSocketInformation(clientSI);        
					return;
				
			}
			else   
			{
				clientSI->BytesRECV = RecvBytes;        
				clientSI->RecvDataBuf.buf[RecvBytes] = '\0';
				if (RecvBytes == 0)                                  
				{
					server->socket_m.FreeSocketInformation(clientSI);
					std::cout << "客户端断开！\n";
					return;
				}

				else
				{
					
					Rece(clientSI->RecvDataBuf.buf);
				}
			}
		}

	}
	
}

void Client::OnProcessMessage(Common::RequestCode requestCode, Common::ActionCode actionCode, std::string data) {

	 server->HandleRequest(requestCode, actionCode, data, this);
}

void Client::Close() {

	mysqlConn.CloseSql();
	if (room != nullptr) {
		room->QuitRoom(this);
	}	
}
//接受数据
void  Client::Rece(char* data) {

	//byte* px = (byte*)data;

	unsigned long a = clientSI->BytesRECV;

	for (unsigned long i = 0; i < a; i++)
	{
		msg.date[i] = data[i];
	}

	msg.ReadMessage(a,this);
	
}

//发送给客户端数据
void Client::Send(Common::ActionCode actionCode, std::string data) {

	
	try
	{
		mtx.lock();
		byte* bytes = msg.PackData(actionCode, data);
		
		clientSI->SendDataBuf.buf = (char*)bytes; 
		clientSI->SendDataBuf.len = (unsigned long)(8 + data.size());

		if (send(clientSocket, clientSI->SendDataBuf.buf,clientSI->SendDataBuf.len, 0)<0) {
				printf("Send()   failed   with   error \n");
				server->socket_m.FreeSocketInformation(clientSI);        
		}
		else
		{
			//std::cout << "发送数据：" << data <<"  字节数："<<SendBytes<< "  \n"<<std::endl;
			//std::cout << this->clientNum<<"\n";
			delete[] bytes;
			clientSI->BytesSEND = 0;
			clientSI->BytesRECV = 0;
		}
		mtx.unlock();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return;
	}
	
}

bool Client::IsHouseOwner() {

	return room->IsHouseOwner(this);
}
