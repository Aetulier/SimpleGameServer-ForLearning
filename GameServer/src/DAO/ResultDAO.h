#pragma once
#include"mysql_m.h"
#include"../Model/Result.h"

class ResultDAO {
public:
	Result* GetResultByUserid(MySqlConn conn, int userId);
};
