#pragma once

namespace DivaHook::Components
{
	class EmulatorComponent
	{
	public:
		EmulatorComponent();
		~EmulatorComponent();

		virtual const char* GetDisplayName() = 0;
		
		virtual void Initialize() = 0;
		virtual void Update() = 0;

		virtual void UpdateInput() {};
		virtual void OnFocusGain() {};
		virtual void OnFocusLost() {};
		
		void SetElapsedTime(float value);
		float GetElapsedTime();
		float GetFrameRate();
		float GetGameFrameRate();

	private:
		float elapsedTime;
	};
}
