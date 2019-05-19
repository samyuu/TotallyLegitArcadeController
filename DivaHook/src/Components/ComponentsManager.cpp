#include "ComponentsManager.h"
#include "Input/InputEmulator.h"
#include "Input/TouchSliderEmulator.h"
#include "Input/TouchPanelEmulator.h"
#include "SysTimer.h"
#include "PlayerDataManager.h"
#include "FrameRateManager.h"
#include "FastLoader.h"
#include "../FileSystem/ConfigFile.h"
#include "../MainModule.h"

using ConfigFile = DivaHook::FileSystem::ConfigFile;

namespace DivaHook::Components
{
	ComponentsManager::ComponentsManager()
	{
	}

	ComponentsManager::~ComponentsManager()
	{
	}

	void ComponentsManager::ParseAddComponents()
	{
		EmulatorComponent *allComponents[]
		{
			new InputEmulator(),
			new TouchSliderEmulator(),
			new TouchPanelEmulator(),
			new SysTimer(),
			new PlayerDataManager(),
			new FrameRateManager(),
			new FastLoader(),
		};

		ConfigFile componentsConfig(MainModule::GetModuleDirectory(), COMPONENTS_CONFIG_FILE_NAME);
		bool success = componentsConfig.OpenRead();

		if (!success)
		{
			printf("ComponentsManager::ParseAddComponents(): Unable to parse %s\n", COMPONENTS_CONFIG_FILE_NAME.c_str());
			return;
		}

		size_t componentCount = sizeof(allComponents) / sizeof(EmulatorComponent*);
		components.reserve(componentCount);

		std::string trueString = "true", falseString = "false";

		for (int i = 0; i < componentCount; i++)
		{
			std::string *value;

			auto name = allComponents[i]->GetDisplayName();
			//printf("ComponentsManager::ParseAddComponents(): searching name: %s\n", name);

			if (componentsConfig.TryGetValue(name, value))
			{
				//printf("ComponentsManager::ParseAddComponents(): %s found\n", name);

				if (*value == trueString)
				{
					//printf("ComponentsManager::ParseAddComponents(): enabling %s...\n", name);
					components.push_back(allComponents[i]);
				}
				else if (*value == falseString)
				{
					//printf("ComponentsManager::ParseAddComponents(): disabling %s...\n", name);
				}
				else
				{
					//printf("ComponentsManager::ParseAddComponents(): invalid value %s for component %s\n", value, name);
				}

				delete value;
			}
			else
			{
				//printf("ParseAddComponents(): component %s not found\n", name);
				delete allComponents[i];
			}
		}
	}


	void ComponentsManager::Initialize()
	{
		ParseAddComponents();
		updateStopwatch.Start();

		for (auto& component : components)
			component->Initialize();
	}

	void ComponentsManager::Update()
	{
		elpasedTime = updateStopwatch.Restart();

		for (auto& component : components)
		{
			component->SetElapsedTime(elpasedTime);
			component->Update();
		}
	}

	void ComponentsManager::UpdateInput()
	{
		for (auto& component : components)
			component->UpdateInput();
	}

	void ComponentsManager::OnFocusGain()
	{
		for (auto& component : components)
			component->OnFocusGain();
	}

	void ComponentsManager::OnFocusLost()
	{
		for (auto& component : components)
			component->OnFocusLost();
	}

	void ComponentsManager::Dispose()
	{
		for (auto& component : components)
			delete component;
	}
}