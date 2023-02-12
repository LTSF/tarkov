#include "present_hook.hpp"

#include <d3d11.h>
#include <cstdint>

#include "../../../unity/unity.hpp"

#include "../../../dependencies/mem/mem.hpp"

#include "../../../dependencies/xorstr/xorstr.h"

#include "../../offsets/offsets.hpp"

#include "../../engine.hpp"

#include "../../../modules/visuals/chams/chams.hpp"

#include <time.h>
#include "../../../modules/misc/no_intertia/no_intertia.hpp"
#include "../../../modules/misc/no_sway/no_sway.hpp"
#include "../../../modules/misc/no_recoil/no_recoil.hpp"
#include "../../../modules/misc/inf_stamina/inf_stamina.hpp"
#include "../../../modules/misc/no_fall/no_fall.hpp"
#include "../../../modules/misc/fly/fly.hpp"



#pragma comment(lib, "d3d11.lib")

constexpr const auto original_function_object = 0x3B8610;

std::uintptr_t old_present_scene = 0;

bool setup = false;

int msec = 0, trigger = 1000;
auto before = LI_FN(GetTickCount)();

HRESULT WINAPI present_hook(IDXGISwapChain* swapchain, UINT si, UINT flags)
{
	if (!setup)
	{
		setup = true;

		unity::sdk::initialize(xorstr("mono-2.0-bdwgc.dll"));

		engine::functions::initiate();

		engine::functions::update_players_vector();

		modules::visuals::chams::callback();

		modules::misc::no_fall::callback();

		modules::misc::fly::callback();

		//std::printf("LocalPlayer: %p\n", engine::values::local_game_world->get_local_player());
	}

	msec = LI_FN(GetTickCount)() - before;

	if (msec > trigger)
	{
		engine::functions::initiate();

		engine::functions::update_players_vector();

		modules::misc::no_inertia::callback();

		modules::misc::no_sway::callback();

		modules::misc::no_recoil::callback();

		modules::misc::inf_stamina::callback();

		before = LI_FN(GetTickCount)();
	}

#ifdef _DEBUG
	if (GetAsyncKeyState(VK_F10)) //ctrl F10
	{
		std::printf("Unloading\n");

		FreeConsole();

		engine::hooks::present::restore();

		FreeLibrary(GetModuleHandleA("intellisense-tarkov.dll"));
	}
#endif

	return reinterpret_cast<HRESULT(__stdcall*)(IDXGISwapChain * swapchain, UINT si, UINT Flags)>(old_present_scene)(swapchain, si, flags);
}

void engine::hooks::present::hook()
{
#ifdef _DEBUG
	FILE* input_output_stream;

	AllocConsole();

	freopen_s(&input_output_stream, "CONIN$", "r", stdin);
	freopen_s(&input_output_stream, "CONOUT$", "w", stdout);
	freopen_s(&input_output_stream, "CONOUT$", "w", stderr);

	SetConsoleTitleA("intellisense dbg");
#endif

	const auto based_original_function_object = original_function_object + reinterpret_cast<std::uintptr_t>(LI_FN(GetModuleHandleA)(xorstr("OWClient.dll"))) + 0x18;

	old_present_scene = *reinterpret_cast<std::uintptr_t*>(based_original_function_object);

	*reinterpret_cast<std::uintptr_t*>(based_original_function_object) = reinterpret_cast<std::uintptr_t>(present_hook);

	unity::sdk::initialize(xorstr("mono-2.0-bdwgc.dll"));

	engine::functions::initiate();

	engine::functions::update_players_vector();

	modules::visuals::chams::callback();
}

void engine::hooks::present::restore()
{
	const auto based_original_function_object = original_function_object + reinterpret_cast<std::uintptr_t>(LI_FN(GetModuleHandleA)(xorstr("OWClient.dll"))) + 0x18;

	*reinterpret_cast<std::uintptr_t*>(based_original_function_object) = old_present_scene;
}
