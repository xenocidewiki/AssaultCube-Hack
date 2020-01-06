#pragma once

#include <functional>
#include "hacks.hpp"

//void __cdecl original_drawframe(int, int, float)

struct Setup
{
	//Detour stuff
	void init_detour();
	void exit_detour();

	//Other hack stuff
	void init_hacks();
};
