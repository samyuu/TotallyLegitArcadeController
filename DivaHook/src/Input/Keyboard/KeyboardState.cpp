#include "KeyboardState.h"

namespace DivaHook::Input
{
	bool KeyboardState::IsDown(BYTE keycode)
	{
		return KeyStates[keycode];
	}
}
