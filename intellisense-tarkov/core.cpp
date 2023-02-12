#include <Windows.h>

#include "engine/hooks/hooks.hpp"
#include "modules/visuals/chams/chams.hpp"
#include "unity/unity.hpp"

#ifdef _DEBUG
bool __stdcall DllMain(void*, DWORD reason, void*)
{
	if(reason == DLL_PROCESS_ATTACH)
		engine::hooks::init();

	if (reason == DLL_PROCESS_DETACH)
	{
		FreeConsole();

		engine::hooks::present::restore();
	}

	return true;
}
#else
bool __stdcall intellisense_entry()
{
	unity::sdk::initialize(xorstr("mono-2.0-bdwgc.dll"));

	engine::functions::initiate();

	engine::functions::update_players_vector();

	modules::visuals::chams::callback();

	return true;
}
#endif