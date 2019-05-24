#include <iostream>
#include "windows.h"
#include "InputEmulator.h"
#include "../ComponentsManager.h"
#include "../../Constants.h"
#include "../../MainModule.h"
#include "../../Input/Bindings/KeyboardBinding.h"
#include "../../Input/Bindings/MouseBinding.h"
#include "../../Input/Bindings/Ds4Binding.h"
#include "../../Input/KeyConfig/Config.h"
#include "../../Utilities/Operations.h"
#include "../../Utilities/EnumBitwiseOperations.h"
#include "../../FileSystem/ConfigFile.h"

const std::string KEY_CONFIG_FILE_NAME = "keyconfig.ini";

using namespace DivaHook::Input;
using namespace DivaHook::Input::KeyConfig;
using namespace DivaHook::Utilities;

namespace DivaHook::Components
{
	InputEmulator::InputEmulator()
	{
	}

	InputEmulator::~InputEmulator()
	{
		delete TestBinding;
		delete ServiceBinding;

		delete StartBinding;
		delete SankakuBinding;
		delete ShikakuBinding;
		delete BatsuBinding;
		delete MaruBinding;

		delete LeftBinding;
		delete RightBinding;

		Config::Keymap.clear();
	}

	const char* InputEmulator::GetDisplayName()
	{
		return "input_emulator";
	}

	void InputEmulator::Initialize(ComponentsManager* manager)
	{
		manager->SetIsInputEmulatorUsed(true);

		inputState = GetInputStatePtr((void*)INPUT_STATE_PTR_ADDRESS);
		inputState->HideCursor();

		TestBinding = new Binding();
		ServiceBinding = new Binding();
		StartBinding = new Binding();

		SankakuBinding = new Binding();
		ShikakuBinding = new Binding();
		BatsuBinding = new Binding();
		MaruBinding = new Binding();

		LeftBinding = new Binding();
		RightBinding = new Binding();

		FileSystem::ConfigFile configFile(MainModule::GetModuleDirectory(), KEY_CONFIG_FILE_NAME);
		configFile.OpenRead();

		Config::BindConfigKeys(configFile.ConfigMap, "JVS_TEST", *TestBinding, { "F1" });
		Config::BindConfigKeys(configFile.ConfigMap, "JVS_SERVICE", *ServiceBinding, { "F2" });
		Config::BindConfigKeys(configFile.ConfigMap, "JVS_START", *StartBinding, { "Enter" });
		Config::BindConfigKeys(configFile.ConfigMap, "JVS_TRIANGLE", *SankakuBinding, { "W", "I" });
		Config::BindConfigKeys(configFile.ConfigMap, "JVS_SQUARE", *ShikakuBinding, { "A", "J" });
		Config::BindConfigKeys(configFile.ConfigMap, "JVS_CROSS", *BatsuBinding, { "S", "K" });
		Config::BindConfigKeys(configFile.ConfigMap, "JVS_CIRCLE", *MaruBinding, { "D", "L" });
		Config::BindConfigKeys(configFile.ConfigMap, "JVS_LEFT", *LeftBinding, { "Q", "U" });
		Config::BindConfigKeys(configFile.ConfigMap, "JVS_RIGHT", *RightBinding, { "E", "O" });

		mouseScrollPvSelection = configFile.GetBooleanValue("mouse_scroll_pv_selection");

		//LeftBinding->AddBinding(new MouseBinding(MouseAction_ScrollUp));
		//RightBinding->AddBinding(new MouseBinding(MouseAction_ScrollDown));
	}

	void InputEmulator::Update()
	{
		return;
	}

	void InputEmulator::OnFocusLost()
	{
		// to prevent buttons from being "stuck"
		inputState->ClearState();
		inputState->HideCursor();
	}

	void InputEmulator::UpdateInput()
	{
		auto tappedFunc = [](void* binding) { return ((Binding*)binding)->AnyTapped(); };
		auto releasedFunc = [](void* binding) { return ((Binding*)binding)->AnyReleased(); };
		auto downFunc = [](void* binding) { return ((Binding*)binding)->AnyDown(); };

		lastDownState = inputState->Down.Buttons;

		inputState->Tapped.Buttons = GetJvsButtonsState(tappedFunc);
		inputState->Released.Buttons = GetJvsButtonsState(releasedFunc);
		inputState->Down.Buttons = GetJvsButtonsState(downFunc);
		inputState->DoubleTapped.Buttons = GetJvsButtonsState(tappedFunc);
		inputState->IntervalTapped.Buttons = GetJvsButtonsState(tappedFunc);

		if ((lastDownState &= inputState->Tapped.Buttons) != 0)
			inputState->Down.Buttons ^= inputState->Tapped.Buttons;

		// repress held down buttons to not block input
		//inputState->Down.Buttons ^= inputState->Tapped.Buttons;

		UpdateDwGuiInput();
	
		if (mouseScrollPvSelection)
		{
			// I originally wanted to use a MouseBinding set to JVS_LEFT / JVS_RIGHT
			// but that ended up being too slow because a PV slot can only be scrolled to once the scroll animation has finished playing
			int* slotsToScroll = (int*)PV_SEL_SLOTS_TO_SCROLL;

			auto mouse = Mouse::GetInstance();
			if (mouse->GetIsScrolledUp())
				*slotsToScroll -= 1;
			if (mouse->GetIsScrolledDown())
				*slotsToScroll += 1;
		}
	}

	void InputEmulator::UpdateDwGuiInput()
	{
		auto keyboard = Keyboard::GetInstance();
		auto mouse = Mouse::GetInstance();

		auto pos = mouse->GetRelativePosition();
		inputState->MouseX = (int)pos.x;
		inputState->MouseY = (int)pos.y;

		auto deltaPos = mouse->GetDeltaPosition();
		inputState->MouseDeltaX = (int)deltaPos.x;
		inputState->MouseDeltaY = (int)deltaPos.y;

		inputState->Key = GetKeyState();

		for (int i = 0; i < sizeof(keyBits) / sizeof(KeyBit); i++)
			UpdateInputBit(keyBits[i].Bit, keyBits[i].KeyCode);

		for (int i = InputBufferType_Tapped; i <= InputBufferType_IntervalTapped; i++)
		{
			inputState->SetBit(scrollUpBit, mouse->GetIsScrolledUp(), (InputBufferType)i);
			inputState->SetBit(scrollDownBit, mouse->GetIsScrolledDown(), (InputBufferType)i);
		}
	}

	InputState* InputEmulator::GetInputStatePtr(void *address)
	{
		return (InputState*)(*(uint64_t*)address);
	}

	JvsButtons InputEmulator::GetJvsButtonsState(bool(*buttonTestFunc)(void*))
	{
		JvsButtons buttons = JVS_NONE;

		if (buttonTestFunc(TestBinding))
			buttons |= JVS_TEST;
		if (buttonTestFunc(ServiceBinding))
			buttons |= JVS_SERVICE;

		if (buttonTestFunc(StartBinding))
			buttons |= JVS_START;

		if (buttonTestFunc(SankakuBinding))
			buttons |= JVS_TRIANGLE;
		if (buttonTestFunc(ShikakuBinding))
			buttons |= JVS_SQUARE;
		if (buttonTestFunc(BatsuBinding))
			buttons |= JVS_CROSS;
		if (buttonTestFunc(MaruBinding))
			buttons |= JVS_CIRCLE;

		if (buttonTestFunc(LeftBinding))
			buttons |= JVS_L;
		if (buttonTestFunc(RightBinding))
			buttons |= JVS_R;

		return buttons;
	}

	char InputEmulator::GetKeyState()
	{
		auto keyboard = Keyboard::GetInstance();

		bool upper = keyboard->IsDown(VK_SHIFT);
		constexpr char caseDifference = 'A' - 'a';

		char inputKey = 0x00;

		for (char key = '0'; key < 'Z'; key++)
		{
			if (keyboard->IsIntervalTapped(key))
				inputKey = (upper || key < 'A') ? key : (key - caseDifference);
		}

		if (keyboard->IsIntervalTapped(VK_BACK))
			inputKey = 0x08;

		if (keyboard->IsIntervalTapped(VK_TAB))
			inputKey = 0x09;

		if (keyboard->IsIntervalTapped(VK_SPACE))
			inputKey = 0x20;

		return inputKey;
	}

	void InputEmulator::UpdateInputBit(uint32_t bit, uint8_t keycode)
	{
		auto keyboard = Keyboard::GetInstance();

		inputState->SetBit(bit, keyboard->IsTapped(keycode), InputBufferType_Tapped);
		inputState->SetBit(bit, keyboard->IsReleased(keycode), InputBufferType_Released);
		inputState->SetBit(bit, keyboard->IsDown(keycode), InputBufferType_Down);
		inputState->SetBit(bit, keyboard->IsDoubleTapped(keycode), InputBufferType_DoubleTapped);
		inputState->SetBit(bit, keyboard->IsIntervalTapped(keycode), InputBufferType_IntervalTapped);
	}
}
