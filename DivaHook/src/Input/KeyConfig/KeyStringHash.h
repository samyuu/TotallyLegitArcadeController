#pragma once
#include "KeyString.h"

namespace DivaHook::Input::KeyConfig
{
	struct KeyStringHash
	{
		size_t operator()(const KeyString& key) const;
	};
}
