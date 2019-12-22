#pragma once

#ifndef __Socket_H__ 
#define __Socket_H__ 


#include<string>
#include<WS2tcpip.h>
#include <winsock2.h>
#include<vector>

#pragma comment(lib,"WS2_32.lib") 


#define   PORT   2233 
#define   DATA_BUFSIZE   1024 

class Server;


typedef   struct   _SOCKET_INFORMATION {
	char   Buffer[DATA_BUFSIZE];        
	WSABUF   RecvDataBuf;                        
	WSABUF   SendDataBuf;                   
	SOCKET   Socket;                            
	DWORD   BytesSEND;                    
	DWORD   BytesRECV;                    
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;


class Socket
{
public:

	Server *server;
	DWORD   Total = 0;      
	FD_SET   WriteSet;       
	FD_SET   ReadSet;        

	void  SocketInit();
	void   FreeSocketInformation(LPSOCKET_INFORMATION SocketInfo);

private:
	DWORD   TotalSockets = 0;
	std::vector<LPSOCKET_INFORMATION> clientVec; 

	SOCKET listenScok;  
	SOCKET clientSock;	

	
	bool initWinSock();
	bool createSocket();
	bool bindIPandPort(const std::string ip, const unsigned short port);
	bool listenSocket();
	void ReadyConn();
	bool AcceptSocket();

	BOOL   CreateSocketInformation(SOCKET   s);
	

};

#endif