#include "setup.hpp"

BOOLEAN WINAPI DllMain(HINSTANCE dll_handle, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH) {
		Setup::init_detour();
	} else if (reason == DLL_PROCESS_DETACH) {
		//Should add detach code so you can get the dll out of memory.
	}

	return true;
}