#pragma once
#include "DirectInputDevice.h"

namespace DivaHook::Input
{
	class Controller : public DirectInputDevice
	{
	protected:
		Controller();
		~Controller();
	};
}