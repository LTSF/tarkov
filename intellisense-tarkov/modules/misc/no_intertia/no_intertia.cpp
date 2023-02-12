#include "no_intertia.hpp"

#include "../../../unity/unity.hpp"

#include "../../../dependencies/mem/mem.hpp"

#include "../../../engine/types/types.hpp"

#include "../../../dependencies/xorstr/xorstr.h"

void modules::misc::no_inertia::callback() //todo optimize
{
	if (!engine::values::local_game_world || !engine::values::camera)
		return;

	unity::sdk::thread_attach(unity::sdk::get_root_domain());

	if (const auto assembly_csharp = unity::sdk::get_image_by_name(xorstr("Assembly-CSharp")))
	{
		if (const auto player_class = unity::sdk::class_from_name(assembly_csharp, xorstr(""), xorstr("\uE62A")))
		{
			if (const auto method = unity::sdk::class_get_method_from_name(player_class, xorstr("get_InertiaSettings"), -1))
			{
				if (const auto res = unity::sdk::compile_method(method))
				{
					const auto lp = engine::values::local_game_world->get_local_player();

					if (!lp)
						return;

					const auto mc = lp->movement_context;

					if (!mc)
						return;


					const auto inertia_settings = reinterpret_cast<engine::types::inertia_settings_t*(__fastcall*)(void*)>(res)(mc);

					if (inertia_settings)
					{
						inertia_settings->MoveTime = 0.f;
						inertia_settings->MinDirectionBlendTime = 0.f;
						inertia_settings->PenaltyPower = 0.f;
						inertia_settings->BaseJumpPenalty = 0.f;
						inertia_settings->DurationPower = 0.f;
						inertia_settings->BaseJumpPenaltyDuration = 0.f;
						inertia_settings->FallThreshold = 0.f;
					}
				}
			}
		}
	}
}
