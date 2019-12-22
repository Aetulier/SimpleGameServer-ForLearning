#include<iostream>
#include"mysql_m.h"
#include<string>

//连接数据库
int MySqlConn::ConnectSql(){

	mysql_init(&mysql);

	if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "mygamedb", 3306, NULL, 0) != NULL) {
		std::cout << "mysql连接成功！";
		return 1;
	}
	else
	{
		std::cout << "mysql连接失败！" << mysql_error(&mysql);
		return -1;
	}
}
//获取所有账户----测试
int MySqlConn::QueryAllData() {
	unsigned int t;
	const char *query = "select * from mygamedb.users limit 10";
	t = mysql_real_query(&mysql, query, (unsigned int)strlen(query)); 
	if (t != 0)
	{
		std::cout << "Error making query:" << mysql_error(&mysql) << std::endl;
		return -1;
	}
	else
	{
		std::cout << "Query made...ok!" << std::endl;
		return 1;
	}
	res = mysql_use_result(&mysql);

	row = mysql_fetch_row(res);

	while (row != nullptr)
	{
		std::cout << mysql_num_fields(res) << "\n";
		for (t = 0; t < mysql_num_fields(res); t++)
		{
			if (row[t] != nullptr)
				std::cout << row[t] << " ";
		}
		std::cout << std::endl;

		row = mysql_fetch_row(res);
	}
	mysql_free_result(res);
}



int MySqlConn::QueryIdData(const char* query) {

	int t;
	t = mysql_real_query(&mysql, query, (unsigned int)strlen(query));
	if (t != 0)
	{
		std::cout << "Error making query:" << mysql_error(&mysql) << std::endl;
		return -1;
	}
	else
	{
		//std::cout << "Query made...ok!" << std::endl;
		res = mysql_use_result(&mysql);
		row = mysql_fetch_row(res);
		if (row != nullptr) {
			std::string s = row[0];
			int a = std::stoi(s);
			mysql_free_result(res);
			return a;
		}
		else
		{
			mysql_free_result(res);
			return -1;
		}		
	}
}

int* MySqlConn::QueryGameData(const char* query) {

	int t;
	t = mysql_real_query(&mysql, query, (unsigned int)strlen(query));
	if (t != 0)
	{
		std::cout << "Error making query:" << mysql_error(&mysql) << std::endl;
		return nullptr;
	}
	else
	{
		std::cout << "Query made...ok!" << std::endl;
		res = mysql_use_result(&mysql);
		row = mysql_fetch_row(res);
		if (row != nullptr) {
			int* Date=new int[3];
			for (unsigned t = 1; t < 4; t++)
			{
				if (row[t] != nullptr)
				{				
					std::string s = row[t];
					Date[t-1] = std::stoi(s);
				}
			}
			mysql_free_result(res);
			return Date;
		}
		else
		{
			mysql_free_result(res);
			return nullptr;
		}
	
	}
}


int MySqlConn::DataUpdate(const char* up) {
	int t;
	t = mysql_real_query(&mysql, up, (unsigned int)strlen(up));
	if (t != 0)
	{
		std::cout << "Error Update:" << mysql_error(&mysql) << std::endl;
		return -1;
	}
	else
	{
		std::cout << "Update ...ok!" << std::endl;
		return 1;
	}
}

int MySqlConn::CloseSql() {
	mysql_close(&mysql);
	return 0;
}