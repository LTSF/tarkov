#include "no_recoil.hpp"

#include "../../../unity/unity.hpp"

#include "../../../dependencies/mem/mem.hpp"

#include "../../../engine/types/types.hpp"

void modules::misc::no_recoil::callback()
{
	if (!engine::values::local_game_world || !engine::values::camera)
		return;

	if (const auto lp = engine::values::local_game_world->get_local_player())
	{
		const auto pwa = lp->procedural_weapon_animation;

		if (!pwa)
			return;

		const auto shot_effector = pwa->shot_effector;

		if (!shot_effector)
			return;

		shot_effector->recoil_strength_xy = { 0.f, 0.f };

		shot_effector->recoil_strength_z = { 0.f, 0.f };
	}
}
