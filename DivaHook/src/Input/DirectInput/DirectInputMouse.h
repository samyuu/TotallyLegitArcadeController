#pragma once
#include "DirectInputDevice.h"

namespace DivaHook::Input
{
	class DirectInputMouse : public DirectInputDevice
	{
	public:
		DirectInputMouse();
		~DirectInputMouse();

		bool Poll();

		inline long GetXPosition() { return mouseState.lX; };
		inline long GetYPosition() { return mouseState.lY; };
		inline long GetMouseWheel() { return mouseState.lZ; };

	private:
		DIMOUSESTATE mouseState;
	};
}