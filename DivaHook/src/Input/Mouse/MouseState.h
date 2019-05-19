#pragma once
#include <windows.h>

namespace DivaHook::Input
{
	struct MouseState
	{
		POINT Position;
		POINT RelativePosition;
		long MouseWheel;
	};
}

