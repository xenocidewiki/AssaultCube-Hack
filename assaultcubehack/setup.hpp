#pragma once

#include <functional>
#include "hacks.hpp"

struct Setup
{
	void init_hacks();
	void init_detour();

	void drawhud_end_hook();
};

uintptr_t* original_func;

__declspec(naked) uintptr_t drawhud_hook()
{
	__asm
	{
		call Setup::drawhud_end_hook;
		mov esi, glDisable;
		push GL_BLEND;
		call esi;

		jmp[original_func];
	}
}

