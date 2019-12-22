#include"ResultDAO.h"
#include<iostream>
#include<string>


//获取账号信息--ID--游戏总场数及胜场
Result* ResultDAO::GetResultByUserid(MySqlConn conn, int userId) {

	try
	{
		int *date;
		std::string query = "select * from result where userid=\"" + std::to_string(userId) + "\"";
	     date = conn.QueryGameData(query.c_str());
		if (date != nullptr) {

			int id = date[0];
			int totalCount = date[1];
			int winCount = date[2];
			Result* res=new Result(id, userId, totalCount, winCount);
			delete date;
			return res;
		}
		else
		{
			Result* res = new Result(-1, userId, 0, 0);
			return res;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		//if (&conn != nullptr)
			conn.CloseSql();
		return nullptr;
	}

}