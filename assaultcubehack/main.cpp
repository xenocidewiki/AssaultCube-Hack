#include "setup.hpp"
#include <detours.h>

#pragma comment(lib, "detours.lib")

BOOLEAN WINAPI DllMain(HINSTANCE dll_handle, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH) {
		std::unique_ptr<Setup> setup = std::make_unique<Setup>();
		setup->init_detour();
	} else if (reason == DLL_PROCESS_DETACH) {

	}

	return true;
}