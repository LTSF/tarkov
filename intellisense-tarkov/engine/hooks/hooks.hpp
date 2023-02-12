#pragma once

#include "present/present_hook.hpp"

namespace engine::hooks
{
	inline void init()
	{
		engine::hooks::present::hook();
	}

	inline void restore()
	{
		engine::hooks::present::restore();
	}
}