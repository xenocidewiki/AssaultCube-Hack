#pragma once
/*
I suppose changing the datatypes to something like void* could be better, in order to avoid a lot of casts...
Inserts a 5 byte jump detour into a location we specify. 
*/
uintptr_t* detour_function(uintptr_t* original_address, uintptr_t detour_address, size_t length)

{
	unsigned long old_protect;
	VirtualProtect(original_address, length, PAGE_EXECUTE_READWRITE, &old_protect);

	uintptr_t detour_relative_address = detour_address - (reinterpret_cast<uintptr_t>(original_address) + 5);

	*reinterpret_cast<byte*>(original_address) = 0xE9;
	*reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(original_address) + 1) = static_cast<uint32_t>(detour_relative_address);

	for (int i = 5; i < length; i++)
		*reinterpret_cast<byte*>(reinterpret_cast<uint32_t>(original_address) + i) = 0x90;

	VirtualProtect(original_address, length, old_protect, 0);

	return reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(original_address) + 5);
}

uintptr_t* original_func;

/*
Courtesy of A200K

Jumps to our hook code, and calls the instructions we initially replaced, then continues execution of the video game.
*/
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
