#pragma once

class Result
{
public:

	Result(int id, int userId, int totalCount, int winCount) {
		this->Id = id;
		this->Userid = userId;
		this->TotalCount = totalCount;
		this->WinCount = winCount;
	}

	int Id;
	int Userid;
	int TotalCount;
	int WinCount;

};


