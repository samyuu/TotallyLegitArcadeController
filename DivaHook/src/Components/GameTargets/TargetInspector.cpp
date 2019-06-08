#include "TargetInspector.h"

namespace DivaHook::Components
{
	bool TargetInspector::repressTbl[maxTargetSlots];

	TargetInspector::TargetInspector()
	{		
	}

	TargetInspector::~TargetInspector()
	{
	}

	void TargetInspector::Initialize(ComponentsManager*)
	{
		tgtTypePtr = (int*)TGT_TYPE_BASE_ADDRESS;
		tgtHitStatePtr = (int*)TGT_HIT_STATE_BASE_ADDRESS;
		tgtRemainingTimePtr = (float*)TGT_REMAINING_DURATION_BASE_ADDRESS;
	}

	void TargetInspector::Update()
	{
		GetTargetStates();
		UpdateRepressTbl();
	}

	const char* TargetInspector::GetDisplayName()
	{
		return "target_inspector";
	}

	void TargetInspector::GetTargetStates()
	{
		for (int i = 0; i < maxTargetSlots; ++i)
		{
			tgtStates[i].tgtType = *((int*)((char*)tgtTypePtr + (i * offset)));
			tgtStates[i].tgtHitState = *((int*)((char*)tgtHitStatePtr + (i * offset)));
			tgtStates[i].tgtRemainingTime = *((float*)((char*)tgtRemainingTimePtr + (i * offset)));
		}
	}

	void TargetInspector::UpdateRepressTbl()
	{
		for (int i = 0; i < maxTargetSlots; ++i)
		{
			repressTbl[i] = IsWithinRange(tgtStates[i].tgtRemainingTime)
				&& HasNotBeenHit(tgtStates[i].tgtHitState)
				&& IsSameType(tgtStates[i].tgtType, InputEmulator::holdTbl);
		}
	}

	bool TargetInspector::IsWithinRange(float time)
	{
		return (time < timingThreshold && time > -timingThreshold) && (time != 0);
	}

	bool TargetInspector::HasNotBeenHit(int hitState)
	{
		return hitState == TargetHitStates::NONE;
	}

	bool TargetInspector::IsSameType(int type, int tbl[])
	{
		for (int i = 0; i < 4; ++i)
		{
		
			if (tbl[i] && (i == type || i + 4 == type || i + 18 == type))
				return true;
		
		}
		return false;
	}

	bool TargetInspector::IsAnyRepress()
	{
		for (int i = 0; i < maxTargetSlots; ++i)
		{
			if (repressTbl[i])
				return true;
		}
		return false;
	}
}
