#pragma once

#ifndef __Socket_H__ 
#define __Socket_H__ 


#include<string>
#include <netinet/in.h>   //for souockaddr_in
#include <sys/types.h>     
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>

//for select
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
#include <strings.h>   //for bzero

#include<vector>


#define   PORT   2233 
#define   DATA_BUFSIZE   1024 

class Server;

 typedef struct _WSABUF {
    u_long len;
    char *buf;
  } WSABUF,*LPWSABUF;


typedef   struct   _SOCKET_INFORMATION {
	char   Buffer[DATA_BUFSIZE];        
	WSABUF   RecvDataBuf;                        
	WSABUF   SendDataBuf;                   
	int   Socket;                            
	unsigned long   BytesSEND;                    
	unsigned long   BytesRECV;                    
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;


class Socket
{
public:

	Server *server;
	unsigned long   Total = 0;      
	fd_set    WriteSet;       
	fd_set    ReadSet;   
	int max_fd = 1;
	struct timeval mytime;     

	void  SocketInit();
	void   FreeSocketInformation(LPSOCKET_INFORMATION SocketInfo);

private:
	unsigned long   TotalSockets = 0;
	std::vector<LPSOCKET_INFORMATION> clientVec; 

	int listenScok;  
	int clientSock;	

	
	bool initWinSock();
	bool createSocket();
	bool bindIPandPort(const std::string ip, const unsigned short port);
	bool listenSocket();
	void ReadyConn();
	bool AcceptSocket();

	int  CreateSocketInformation(int   s);
	

};

#endif