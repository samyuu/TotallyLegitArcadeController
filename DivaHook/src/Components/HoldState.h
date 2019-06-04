#pragma once
#include "../Utilities/EnumBitwiseOperations.h"

namespace DivaHook::Components
{
	enum HoldState
	{
		HOLD_NONE,
		SANKAKU = 64,
		MARU = 128,
		BATSU = 256,
		SHIKAKU = 512,
		ALL = SANKAKU + MARU + BATSU + SHIKAKU
	};
}