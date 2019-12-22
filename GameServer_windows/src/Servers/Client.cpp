#include"Client.h"
#include"Server.h"

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



void Client::Start() {

	while (true)
	{
		FD_ZERO(&ReadSet);
		FD_ZERO(&WriteSet);

		FD_SET(clientSI->Socket, &ReadSet);
		FD_SET(clientSI->Socket, &WriteSet);

		if (select(0, &ReadSet, &WriteSet, NULL, NULL)<=0)//将NULL以形参传入Timeout，即不传入时间结构，就是将select置于阻塞状态，一定等到监视文件描述符集合中某个文件描述符发生变化为止.服务器会停到这里等待客户端相应
		{
			printf("select()   returned   with   error   %d\n", WSAGetLastError());
			return;
		}		

		if (FD_ISSET(clientSI->Socket,&ReadSet)) {
			socket_m->Total--;
			memset(clientSI->Buffer, ' ', DATA_BUFSIZE);            
			clientSI->RecvDataBuf.buf = clientSI->Buffer;           
			clientSI->RecvDataBuf.len = DATA_BUFSIZE;               
		
			DWORD  Flags = 0;
			if (WSARecv(clientSI->Socket, &(clientSI->RecvDataBuf), 1, &RecvBytes, &Flags, NULL, NULL) == SOCKET_ERROR)
			{
				
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					printf("WSARecv()   failed   with   error   %d\n", WSAGetLastError());
					server->socket_m.FreeSocketInformation(clientSI);        
					return;
				}
			}
			else   
			{
				clientSI->BytesRECV = RecvBytes;        
				clientSI->RecvDataBuf.buf[RecvBytes] = '\0';
				if (RecvBytes == 0)                                  
				{
					server->socket_m.FreeSocketInformation(clientSI);
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

void  Client::Rece(char* data) {

	//byte* px = (byte*)data;

	DWORD a = clientSI->BytesRECV;

	for (DWORD i = 0; i < a; i++)
	{
		msg.date[i] = data[i];
	}

	msg.ReadMessage(a,this);
	
}


void Client::Send(Common::ActionCode actionCode, std::string data) {

	
	try
	{
		mtx.lock();
		byte* bytes = msg.PackData(actionCode, data);
		
		clientSI->SendDataBuf.buf = (char*)bytes;
		clientSI->SendDataBuf.len = (ULONG)(8 + data.size());

		if (WSASend(clientSocket, &(clientSI->SendDataBuf), 1, &SendBytes, 0, nullptr, nullptr) == SOCKET_ERROR) {
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				printf("WSASend()   failed   with   error   %d\n", WSAGetLastError());
				server->socket_m.FreeSocketInformation(clientSI);        
			}
		}
		else
		{
			std::cout << "发送数据：" << data <<"  字节数："<<SendBytes<< "  \n"<<std::endl;
			std::cout << this->clientNum<<"\n";
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
