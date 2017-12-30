#include "global.h"
#include <sstream>
#include <string>

using namespace std;


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hInstance = hModule;
		DisableThreadLibraryCalls(hModule);
		break;
	}

	return TRUE;
}