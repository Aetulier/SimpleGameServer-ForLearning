#pragma once
namespace Common {
	//客户端行为
	enum ActionCode
	{
		None_A,
		Login,
		Register,
		ListRoom,
		CreateRoom,
		JoinRoom,
		UpdateRoom,
		QuitRoom,
		StartGame,
		ShowTimer,
		StartPlay,
		Move,
		Shoot,
		Attack,
		GameOver
	};
}
