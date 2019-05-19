#pragma once
#include <functional>
#include <Windows.h>
#include "InputState.h"
#include "../EmulatorComponent.h"
#include "../../Input/Bindings/Binding.h"

namespace DivaHook::Components
{
	struct KeyBit
	{
		uint32_t Bit;
		uint8_t KeyCode;
	};

	class InputEmulator : public EmulatorComponent
	{
	public:
		Input::Binding* TestBinding;
		Input::Binding* ServiceBinding;

		Input::Binding* StartBinding;
		Input::Binding* SankakuBinding;
		Input::Binding* ShikakuBinding;
		Input::Binding* BatsuBinding;
		Input::Binding* MaruBinding;

		Input::Binding* LeftBinding;
		Input::Binding* RightBinding;

		InputEmulator();
		~InputEmulator();

		virtual const char* GetDisplayName() override;

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void UpdateInput() override;

		virtual void OnFocusLost() override;

	private:
		const uint32_t scrollUpBit = 99;
		const uint32_t scrollDownBit = 100;

		KeyBit keyBits[20] =
		{
			{ 5, VK_LEFT },
			{ 6, VK_RIGHT },

			{ 29, VK_SPACE },
			{ 39, 'A' },
			{ 43, 'E' },
			{ 42, 'D' },
			{ 55, 'Q' },
			{ 57, 'S' }, // unsure
			{ 61, 'W' },
			{ 63, 'Y' },
			{ 84, 'L' }, // unsure

			{ 80, VK_RETURN },
			{ 81, VK_SHIFT },
			{ 82, VK_CONTROL },
			{ 83, VK_MENU },

			{ 91, VK_UP },
			{ 93, VK_DOWN },

			{ 96, MK_LBUTTON },
			{ 97, VK_MBUTTON },
			{ 98, MK_RBUTTON },
		};

		InputState* inputState;
		JvsButtons lastDownState;

		void UpdateDwGuiInput();
		InputState* GetInputStatePtr(void *address);
		JvsButtons GetJvsButtonsState(bool(*buttonTestFunc)(void*));
		char GetKeyState();

		void UpdateInputBit(uint32_t bit, uint8_t keycode);
	};
}
