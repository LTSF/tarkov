#include "no_fall.hpp"

#include "../../../unity/unity.hpp"

#include "../../../dependencies/mem/mem.hpp"

#include "../../../engine/types/types.hpp"

#include "../../../dependencies/xorstr/xorstr.h"


void __fastcall handle_fall_hk(void* rcx, void*, float)
{
	return;
}

void modules::misc::no_fall::callback() //todo optimize
{
	unity::sdk::thread_attach(unity::sdk::get_root_domain());

	if (const auto assembly_csharp = unity::sdk::get_image_by_name(xorstr("Assembly-CSharp")))
	{
		if (const auto player_class = unity::sdk::class_from_name(assembly_csharp, xorstr(""), xorstr("\uE801")))
		{
			if (const auto method = unity::sdk::class_get_method_from_name(player_class, xorstr("HandleFall"), -1))
			{
				//std::printf("got func \n");

				if (const auto res = unity::sdk::compile_method(method))
				{
					mem::hook(reinterpret_cast<std::uintptr_t>(res), reinterpret_cast<std::uintptr_t>(handle_fall_hk));
				}
			}
		}
	}
}
