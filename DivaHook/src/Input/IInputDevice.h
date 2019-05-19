#pragma once

namespace DivaHook::Input
{
	class IInputDevice
	{
	public:
		virtual bool PollInput() = 0;
	};
}
