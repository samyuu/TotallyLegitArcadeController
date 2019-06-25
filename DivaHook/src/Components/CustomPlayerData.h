#pragma once
#include <string>

namespace DivaHook::Components
{
	struct CustomPlayerData
	{
		std::string *PlayerName;
		std::string *LevelName;
		int LevelPlateId;
		int SkinEquip;
		int BtnSeEquip;
		int ChainslideSeEquip;
	};
}
