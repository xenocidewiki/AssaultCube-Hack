#include "setup.hpp"
#include <detours.h>
#include <gl/GL.h>
#include <gl/glu.h>

#pragma comment(lib, "detours.lib")
#pragma comment(lib, "opengl32.lib")

uintptr_t* detour_function(uintptr_t* original_address, uintptr_t detour_address, size_t length)
{
	unsigned long old_protect;
	VirtualProtect(original_address, length, PAGE_EXECUTE_READWRITE, &old_protect);

	uintptr_t detour_relative_address				= detour_address - ((uintptr_t)original_address + 5);

	*(byte*)original_address						= 0xE9;
	*(uint32_t*)((uint32_t)original_address + 1)	= (uint32_t)detour_relative_address;

	for (int i = 5; i < length; i++)	
		*(byte*)((uint32_t)original_address + i)	= 0x90;

	VirtualProtect(original_address, length, old_protect, 0);

	return (uintptr_t*)((uintptr_t)original_address + 5);
}

void drawhud_end_hook()
{
	glPushMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLoadIdentity();
	glOrtho(0, 1024, 768, 0, 0, 1);
	glDepthMask(GL_FALSE);
	glDisable(GL_TEXTURE_2D);
	GLfloat param;
	glGetFloatv(GL_LINE_WIDTH, &param);

	glColor3f(50, 50, 50);
	glLineWidth(10);
	glBegin(GL_LINES);
	glVertex2f(10, 10);
	glVertex2f(100, 100);
	glEnd();
	glDepthMask(GL_TRUE);
	glEnable(GL_TEXTURE_2D);

	glPopMatrix();

}
uintptr_t* original_func;
__declspec(naked) uintptr_t drawhud_hook()
{
	__asm
	{
		call drawhud_end_hook;
		mov esi, glDisable;
		push GL_BLEND;
		call esi;

		jmp [original_func];
	}
}

BOOLEAN WINAPI DllMain(HINSTANCE dll_handle, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH) {
		std::unique_ptr<Setup> setup = std::make_unique<Setup>();
		setup->init_hacks();
		//original_func = detour_function((uintptr_t*)0x40C375, (uintptr_t)drawhud_hook, 13);
	} else if (reason == DLL_PROCESS_DETACH) {
	}

	return true;
}