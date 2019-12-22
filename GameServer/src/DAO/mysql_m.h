#pragma once
#include<mysql/mysql.h>

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

