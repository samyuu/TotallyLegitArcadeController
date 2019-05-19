#pragma once
#include "../IInputDevice.h"
#include "../DirectInput/DirectInputMouse.h"
#include "MouseState.h"

namespace DivaHook::Input
{
	class Mouse : public IInputDevice
	{
	public:
		~Mouse();
		
		static Mouse* GetInstance();

		bool PollInput() override;
		
		POINT GetPosition();
		POINT GetRelativePosition();
		POINT GetDeltaPosition();
		
		long GetMouseWheel();
		long GetDeltaMouseWheel();
		
		bool HasMoved();
		bool ScrolledUp();
		bool ScrolledDown();
		
		void SetPosition(int x, int y);

	private:
		Mouse();
		MouseState lastState;
		MouseState currentState;
		DirectInputMouse* directInputMouse = nullptr;

		static Mouse* instance;
	};
}

