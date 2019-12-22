#include"BaseController.h"


Common::RequestCode BaseController::RequestCode() {

	return requestCode;
}
//获取请求类名
std::string BaseController::GetClassName_m(Common::RequestCode requestCode)
{
	switch (requestCode)
	{
	case Common::None_R:
		return nullptr;
		break;
	case Common::User:
		return "UserController";
		break;
	case Common::Room:
		return "RoomController";
		break;
	case Common::Game:
		return "GameController";
		break;
	default:
		return nullptr;
		break;
	}
}

