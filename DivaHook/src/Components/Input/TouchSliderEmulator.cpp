#include "TouchSliderEmulator.h"
#include "../../Constants.h"
#include "../../MainModule.h"
#include "../../Input/Mouse/Mouse.h"
#include "../../Input/Keyboard/Keyboard.h"
#include "../../Input/Bindings/KeyboardBinding.h"
#include "../../Input/KeyConfig/Config.h"
#include "../../FileSystem/ConfigFile.h"
#include "../../Utilities/Math.h"
#include <algorithm>
#include <stdio.h>

using namespace DivaHook::Input;
using namespace DivaHook::Input::KeyConfig;
using namespace DivaHook::Utilities;

namespace DivaHook::Components
{
	const std::string KEY_CONFIG_FILE_NAME = "keyconfig.ini";

	TouchSliderEmulator::TouchSliderEmulator()
	{
	}

	TouchSliderEmulator::~TouchSliderEmulator()
	{
		delete LeftSideSlideLeft;
		delete LeftSideSlideRight;

		delete RightSideSlideLeft;
		delete RightSideSlideRight;
	}

	const char* TouchSliderEmulator::GetDisplayName()
	{
		return "touch_slider_emulator";
	}

	void TouchSliderEmulator::Initialize(ComponentsManager*)
	{
		sliderState = (TouchSliderState*)SLIDER_CTRL_TASK_ADDRESS;

		LeftSideSlideLeft = new Binding();
		LeftSideSlideRight = new Binding();

		RightSideSlideLeft = new Binding();
		RightSideSlideRight = new Binding();

		FileSystem::ConfigFile configFile(MainModule::GetModuleDirectory(), KEY_CONFIG_FILE_NAME);
		configFile.OpenRead();

		Config::BindConfigKeys(configFile.ConfigMap, "LEFT_SIDE_SLIDE_LEFT", *LeftSideSlideLeft, { "Q" });
		Config::BindConfigKeys(configFile.ConfigMap, "LEFT_SIDE_SLIDE_RIGHT", *LeftSideSlideRight, { "E" });

		Config::BindConfigKeys(configFile.ConfigMap, "RIGHT_SIDE_SLIDE_LEFT", *RightSideSlideLeft, { "U" });
		Config::BindConfigKeys(configFile.ConfigMap, "RIGHT_SIDE_SLIDE_RIGHT", *RightSideSlideRight, { "O" });
	}

	void TouchSliderEmulator::Update()
	{
		sliderState->State = SLIDER_OK;
	}

	void TouchSliderEmulator::UpdateInput()
	{
		sliderIncrement = GetElapsedTime() / sliderSpeed;

		const float sensorStep = (1.0f / SLIDER_SENSORS);

		EmulateSliderInput(LeftSideSlideLeft, LeftSideSlideRight, ContactPoints[0], 0.0f, 0.5f);
		EmulateSliderInput(RightSideSlideLeft, RightSideSlideRight, ContactPoints[1], 0.5f + sensorStep, 1.0f + sensorStep);

		sliderState->ResetSensors();

		for (int i = 0; i < CONTACT_POINTS; i++)
			ApplyContactPoint(ContactPoints[i], i);
	}

	void TouchSliderEmulator::OnFocusLost()
	{
		sliderState->ResetSensors();
	}

	void TouchSliderEmulator::EmulateSliderInput(Binding *leftBinding, Binding *rightBinding, ContactPoint &contactPoint, float start, float end)
	{
		bool leftDown = leftBinding->AnyDown();
		bool rightDown = rightBinding->AnyDown();

		if (leftDown)
			contactPoint.Position -= sliderIncrement;
		else if (rightDown)
			contactPoint.Position += sliderIncrement;

		if (contactPoint.Position < start)
			contactPoint.Position = end;

		if (contactPoint.Position > end)
			contactPoint.Position = start;

		bool leftTapped = leftBinding->AnyTapped();
		bool rightTapped = rightBinding->AnyTapped();

		if (leftTapped || rightTapped)
			contactPoint.Position = (start + end) / 2.0f;

		contactPoint.InContact = leftDown || rightDown;
	}

	void TouchSliderEmulator::ApplyContactPoint(ContactPoint &contactPoint, int section)
	{
		sliderState->SectionTouched[section] = contactPoint.InContact;

		int pressure = contactPoint.InContact ? FULL_PRESSURE : NO_PRESSURE;
		float position = std::clamp(contactPoint.Position, 0.0f, 1.0f);

		if (contactPoint.InContact)
		{
			int sensor = (int)(position * (SLIDER_SENSORS - 1));

			sliderState->SetSensor(sensor, pressure);
		}

		constexpr float startRange = -1.0f;
		constexpr float endRange   = +1.0f;

		sliderState->SectionPositions[section] = contactPoint.InContact ? (ConvertRange(0.0f, 1.0f, startRange, endRange, position)) : 0.0f;
	}
}