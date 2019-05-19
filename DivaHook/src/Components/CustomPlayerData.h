#pragma once
#include <string>

namespace DivaHook::Components
{
	struct CustomPlayerData
	{
		std::string *PlayerName;
		int LevelPlateId;
		int SkinEquip;
		int BtnSeEquip;
		int ChainslideSeEquip;
	};
}
