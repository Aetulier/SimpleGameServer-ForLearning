#include<iostream>
#include"Socket.h"
#include"Server.h"

//初始化及创建监听Socket循环监听
void Socket::SocketInit() {

	//initWinSock();
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
					close(listenScok);
					close(clientSock);
					return;
				}
			}
		//	IsConnect(SocketInfo, i);
		
	}
	

}

//为客户端连接分配内存
int  Socket::CreateSocketInformation(int  s) {

	LPSOCKET_INFORMATION   SI;                                           
//   printf("Accepted   socket   number   %d\n",   s);           

	if ((SI = (LPSOCKET_INFORMATION)malloc(sizeof(SOCKET_INFORMATION))) == NULL)
	{
		printf("malloc()   failed   with   error \n");
		return   -1;
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
	return 1;
}


//释放客户端连接
void  Socket::FreeSocketInformation(LPSOCKET_INFORMATION SocketInfo) {

	unsigned long Index;
	for (int i = 0; i <(int)clientVec.size(); i++)
	{
		if (clientVec[i] == SocketInfo) {
			Index = i;

			if (Index != (TotalSockets - 1))
			{
				for (unsigned long i = Index; i < TotalSockets - 1; i++)
				{
					clientVec[i] = clientVec[i + 1];
				}
			}
			break;
		}
	}
	//LPSOCKET_INFORMATION SI = clientVec[Index];  

	server->RemoveSocket(SocketInfo->Socket);
	if (SocketInfo->Socket != -1) {
		close(SocketInfo->Socket);       
		SocketInfo->Socket = -1;
	}
	free(SocketInfo);  

	clientVec.pop_back();
	TotalSockets--;        

	std::cout << TotalSockets<< "  客户端断开\n" << std::endl;

}

//winSocket初始化

// bool Socket::initWinSock() {
// 	WORD verision = MAKEWORD(2, 2);
// 	WSADATA lpDate;
// 	int intEr = WSAStartup(verision, &lpDate);
// 	if (intEr != 0)
// 	{
// 		std::cout << "初始化失败！" << std::endl;
// 		return false;
// 	}
// 	std::cout << "WinSock初始化成功！" << std::endl;
// 	return true;
// }

//创建监听Socket

bool Socket::createSocket() {

	listenScok = socket(AF_INET, SOCK_STREAM,0);
	if (listenScok <0) {
		std::cout << "socket创建失败！" << std::endl;		
		return false;
	}
	std::cout << "socket创建成功!" << std::endl;
	return true;
}

//

bool Socket::bindIPandPort(const std::string ip, const unsigned short port) {


	struct sockaddr_in hostAddr;
	hostAddr.sin_family = AF_INET;
	hostAddr.sin_port = htons(port);
	hostAddr.sin_addr.s_addr = INADDR_ANY;

	int err = bind(listenScok, (const struct sockaddr *)&hostAddr, sizeof(hostAddr));
	if (err <0) {
		std::cout << "本地IP绑定失败！" << std::endl;
		close(listenScok);
		return false;
	}
	return true;
}


bool Socket::listenSocket()
{
	
	int err = listen(listenScok, 5);
	if (err< 0)
	{
		std::cout << "socket监听失败！" << std::endl;
		close(listenScok);
		return false;
	}
	std::cout << "监听客户端连接中......" << std::endl;


//设置Socket为非阻塞
   int flags;
   if(flags=fcntl(listenScok,F_GETFD,0)<0)
   {
	   printf("fcntl() get flag");
	   close(listenScok);
	   return false;
   }
  if(fcntl(listenScok,F_SETFL,flags|O_NONBLOCK)<0){
	   printf("fcntl() set flag");
	   close(listenScok);
	   return false;
  }

   // if (ioctlsocket(listenScok, FIONBIO, &NonBlock) == SOCKET_ERROR)
   // {
   // 	printf("ioctlsocket() failed with error %d\n", WSAGetLastError());
   // 	closesocket(listenScok);
   // 	return false;
   // }

   return true;
}


void Socket::ReadyConn() {

	mytime.tv_sec = 27;
	mytime.tv_usec = 0;
	FD_ZERO(&ReadSet);
	FD_SET(0, &ReadSet);
	
	if(max_fd<0){
		max_fd = 0;
	}
	FD_SET(listenScok, &ReadSet);

	if(max_fd<listenScok){

		max_fd = listenScok;
	}
	//select
	Total = select(max_fd+1, &ReadSet, NULL, NULL,&mytime);
	if (Total<0)
	{
		printf("select()   returned   with   error \n");
		return ;
	}
	
}
//添加客户端Socket连接
bool Socket::AcceptSocket() {
		Total--;    
		struct sockaddr_in client_address;
		socklen_t address_len;
		int client_sock_fd = accept(listenScok, (struct sockaddr *)&client_address, &address_len);
		if (client_sock_fd>0)
		{
			// if (ioctlsocket(clientSock, FIONBIO, &NonBlock) == SOCKET_ERROR)
			// {
			// 	printf("ioctlsocket()   failed   with   error   %d\n", WSAGetLastError());
			// 	return false;
			// }
			if(fcntl(client_sock_fd,F_SETFL,fcntl(client_sock_fd,F_GETFL,0)|O_NONBLOCK)<0){
				printf("fcntl  failed   with   error");
				return false;
			}
			
			if (CreateSocketInformation(client_sock_fd) == -1)
				return false;
			std::cout << TotalSockets << "  新增客户端\n" << std::endl;
			return true;
		}
		else
		{
			printf("accept()   failed   with   error  \n");
			return false;
		}
	
	return true;
}
