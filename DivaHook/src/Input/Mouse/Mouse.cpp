#include "Mouse.h"
#include "../../MainModule.h"

namespace DivaHook::Input
{
	Mouse* Mouse::instance;

	Mouse::Mouse()
	{
		directInputMouse = new DirectInputMouse();
	}

	Mouse::~Mouse()
	{
		if (directInputMouse != nullptr)
			delete directInputMouse;
	}

	Mouse* Mouse::GetInstance()
	{
		if (instance == nullptr)
			instance = new Mouse();

		return instance;
	}

	POINT Mouse::GetPosition()
	{
		return currentState.Position;
	}

	POINT Mouse::GetRelativePosition()
	{
		return currentState.RelativePosition;
	}

	POINT Mouse::GetDeltaPosition()
	{
		return 
		{ 
			currentState.Position.x - lastState.Position.x, 
			currentState.Position.y - lastState.Position.y 
		};
	}

	long Mouse::GetMouseWheel()
	{
		return currentState.MouseWheel;
	}

	long Mouse::GetDeltaMouseWheel()
	{
		return currentState.MouseWheel - lastState.MouseWheel;
	}

	bool Mouse::HasMoved()
	{
		POINT delta = GetDeltaPosition();
		return delta.x != 0 || delta.y != 0;
	}

	bool Mouse::ScrolledUp()
	{
		return GetDeltaMouseWheel() > 0;
	}

	bool Mouse::ScrolledDown()
	{
		return GetDeltaMouseWheel() < 0;
	}

	void Mouse::SetPosition(int x, int y)
	{
		lastState.Position.x = x;
		lastState.Position.y = y;
		SetCursorPos(x, y);
	}

	bool Mouse::PollInput()
	{
		lastState = currentState;

		GetCursorPos(&currentState.Position);
		currentState.RelativePosition = currentState.Position;

		if (MainModule::DivaWindowHandle != NULL)
			ScreenToClient(MainModule::DivaWindowHandle, &currentState.RelativePosition);

		if (directInputMouse != nullptr)
		{
			if (directInputMouse->Poll())
				currentState.MouseWheel += directInputMouse->GetMouseWheel();
		}

		return true;
	}
}
