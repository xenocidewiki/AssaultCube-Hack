#pragma once

#include <functional>
#include "hacks.hpp"

struct Setup
{
	void init_hacks();
	void init_detour();

	void drawhud_end_hook();
};
