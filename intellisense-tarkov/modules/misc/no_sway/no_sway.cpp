#include "no_sway.hpp"

void modules::misc::no_sway::callback()
{
	if (!engine::values::local_game_world || !engine::values::camera)
		return;

	if (const auto lp = engine::values::local_game_world->get_local_player())
	{
		const auto pwa = lp->procedural_weapon_animation;

		if (!pwa)
			return;

		const auto breath_effector = pwa->breath_effector;

		if (!breath_effector)
			return;

		breath_effector->intensity = 0.f;

		const auto motion_effector = pwa->motion_effector;

		if (!motion_effector)
			return;

		motion_effector->intensity = 0.f;
	}
}
