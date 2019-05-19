#pragma once
#include "../EmulatorComponent.h"
#include "TouchPanelState.h"

namespace DivaHook::Components
{
	class TouchPanelEmulator : public EmulatorComponent
	{
	public:
		TouchPanelEmulator();
		~TouchPanelEmulator();

		virtual const char* GetDisplayName() override;

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void UpdateInput() override;

	private:
		TouchPanelState* state;
		TouchPanelState* GetTouchStatePtr(void *address);
	};
}

