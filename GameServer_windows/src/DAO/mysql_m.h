#pragma once
#include "D:\Program Files\mysql\mysql-8.0.18-winx64\include\mysql.h"

class MySqlConn
{
public:
	int ConnectSql();
	int QueryAllData();
	int QueryIdData(const char* query);
	int* QueryGameData(const char* query);
	int DataUpdate(const char* up);
	int CloseSql();
private:
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
};

