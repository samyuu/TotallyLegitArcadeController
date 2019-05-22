#pragma once
#include "EmulatorComponent.h"
#include "../Utilities/Stopwatch.h"
#include <vector>

namespace DivaHook::Components
{
	const std::string COMPONENTS_CONFIG_FILE_NAME = "components.ini";

	class ComponentsManager
	{
	public:
		ComponentsManager();
		~ComponentsManager();
		void Initialize();
		void Update();
		void UpdateInput();
		void OnFocusGain();
		void OnFocusLost();
		void Dispose();

		inline bool GetIsInputEmulatorUsed() { return isInputEmulatorUsed; };
		inline void SetIsInputEmulatorUsed(bool value) { isInputEmulatorUsed = value; };

	private:
		bool isInputEmulatorUsed = false;
		float elpasedTime;
		Utilities::Stopwatch updateStopwatch;
		std::vector<EmulatorComponent*> components;

		void ParseAddComponents();
	};
}