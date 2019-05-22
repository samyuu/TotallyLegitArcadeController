#pragma once
#include "EmulatorComponent.h"
#include <stdint.h>

namespace DivaHook::Components
{
	class StageManager : public EmulatorComponent
	{
	public:
		StageManager();
		~StageManager();

		virtual const char* GetDisplayName() override;

		virtual void Initialize(ComponentsManager*) override;
		virtual void Update() override;

	private:
		int32_t GetPlayCount();
	};
}