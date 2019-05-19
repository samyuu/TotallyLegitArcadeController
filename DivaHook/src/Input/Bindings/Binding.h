#pragma once
#include <vector>
#include "IInputBinding.h"

namespace DivaHook::Input
{
	class Binding
	{
	public:
		std::vector<IInputBinding*> InputBindings;

		Binding();
		~Binding();

		void AddBinding(IInputBinding* inputBinding);

		bool AnyDown();
		bool AnyTapped();
		bool AnyReleased();

		int GetDownCount();
		int GetTappedCount();
		int GetReleasedCount();
	};
}

