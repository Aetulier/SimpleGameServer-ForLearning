#include<iostream>
#include"Socket.h"
#include"Server.h"

void Socket::SocketInit() {

	initWinSock();
	createSocket();
	bindIPandPort("127.0.0.1", 2233);
	listenSocket();
	CreateSocketInformation(listenScok);

	//ConnectAndAccept();

	while (true)
	{
		ReadyConn();

		
			if (FD_ISSET(listenScok, &ReadSet)) {
				if (AcceptSocket() == false) {
					std::cout << "\n客户端连接错误！";
					closesocket(listenScok);
					closesocket(clientSock);
					WSACleanup();

					return;
				}
			}
		//	IsConnect(SocketInfo, i);
		
	}
	

}


BOOL  Socket::CreateSocketInformation(SOCKET   s) {

	LPSOCKET_INFORMATION   SI;                                           
//   printf("Accepted   socket   number   %d\n",   s);           

	if ((SI = (LPSOCKET_INFORMATION)malloc(sizeof(SOCKET_INFORMATION))) == NULL)
	{
		printf("GlobalAlloc()   failed   with   error   %ld\n", GetLastError());
		return   FALSE;
	}
 
	SI->Socket = s;
	SI->BytesSEND = 0;
	SI->BytesRECV = 0;

	clientVec.push_back(SI);	
	
	if (s!= listenScok)
	{
		server->AddClient(s,SI,TotalSockets);
	}

	TotalSockets++;                       
	return(TRUE);
}



void  Socket::FreeSocketInformation(LPSOCKET_INFORMATION SocketInfo) {

	DWORD Index;
	for (int i = 0; i <(int)clientVec.size(); i++)
	{
		if (clientVec[i] == SocketInfo) {
			Index = i;

			if (Index != (TotalSockets - 1))
			{
				for (DWORD i = Index; i < TotalSockets - 1; i++)
				{
					clientVec[i] = clientVec[i + 1];
				}
			}
			break;
		}
	}
	//LPSOCKET_INFORMATION SI = clientVec[Index];  

	server->RemoveSocket(SocketInfo->Socket);
	if (SocketInfo->Socket != INVALID_SOCKET) {
		closesocket(SocketInfo->Socket);       
		SocketInfo->Socket = INVALID_SOCKET;
	}
	free(SocketInfo);  

	clientVec.pop_back();
	TotalSockets--;        

	std::cout << TotalSockets<< "  客户端断开\n" << std::endl;

}

bool Socket::initWinSock() {
	WORD verision = MAKEWORD(2, 2);
	WSADATA lpDate;
	int intEr = WSAStartup(verision, &lpDate);
	if (intEr != 0)
	{
		std::cout << "初始化失败！" << std::endl;
		WSACleanup();
		return false;
	}
	std::cout << "WinSock初始化成功！" << std::endl;
	return true;
}
bool Socket::createSocket() {

	listenScok = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenScok == INVALID_SOCKET) {
		std::cout << "socket创建失败！" << std::endl;
		WSACleanup();
		return false;
	}
	std::cout << "socket创建成功!" << std::endl;
	return true;
}

bool Socket::bindIPandPort(const std::string ip, const unsigned short port) {


	sockaddr_in hostAddr;
	hostAddr.sin_family = AF_INET;
	hostAddr.sin_port = htons(port);
	//in_addr addr;
	//InetPton(AF_INET, ip.c_str(), (void*)&addr);
	//hostAddr.sin_addr = addr;
	//std::cout << "ip(网络字节序):" << addr.S_un.S_addr << std::endl;
	//std::cout << "ip(常规形式):" << ip.c_str() << std::endl;

	int err = bind(listenScok, (PSOCKADDR)&hostAddr, sizeof(sockaddr));
	if (err == SOCKET_ERROR) {
		std::cout << "本地IP绑定失败！" << std::endl;
		closesocket(listenScok);
		WSACleanup();
		return false;
	}
	return true;
}

bool Socket::listenSocket()
{
	
	int err = listen(listenScok, 5);
	if (err != 0)
	{
		std::cout << "socket监听失败！" << std::endl;
		closesocket(listenScok);
		WSACleanup();
		return false;
	}
	std::cout << "监听客户端连接中......" << std::endl;

	ULONG NonBlock = 1;

	if (ioctlsocket(listenScok, FIONBIO, &NonBlock) == SOCKET_ERROR)
	{
		printf("ioctlsocket() failed with error %d\n", WSAGetLastError());
		closesocket(listenScok);
		return false;
	}

	return true;
}


void Socket::ReadyConn() {

	FD_ZERO(&ReadSet);
	FD_ZERO(&WriteSet);

	FD_SET(listenScok, &ReadSet);
	
	
 
	if ((Total =select(0, &ReadSet, &WriteSet, NULL, NULL)) == SOCKET_ERROR)
	{
		printf("select()   returned   with   error   %d\n", WSAGetLastError());
		return ;
	}
	
}

bool Socket::AcceptSocket() {
	
		Total--;    
		if ((clientSock = accept(listenScok, NULL, NULL)) != INVALID_SOCKET)
		{

			ULONG NonBlock = 1;
			if (ioctlsocket(clientSock, FIONBIO, &NonBlock) == SOCKET_ERROR)
			{
				printf("ioctlsocket()   failed   with   error   %d\n", WSAGetLastError());
				return false;
			}
			
			if (CreateSocketInformation(clientSock) == FALSE)
				return false;
			std::cout << TotalSockets << "  新增客户端\n" << std::endl;
			return true;
		}
		else
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				printf("accept()   failed   with   error   %d\n", WSAGetLastError());
				return false;
			}
		}
	
	return true;
}
