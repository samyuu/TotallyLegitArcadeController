#include <cstdint>
#pragma once

namespace DivaHook::Components
{
	struct TargetState
	{
		int tgtType;
		int tgtHitState;
		int tgtRemainingTime;
		bool repress;
	};
}