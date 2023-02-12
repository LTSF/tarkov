#include "inf_stamina.hpp"

void modules::misc::inf_stamina::callback()
{
	if (!engine::values::local_game_world || !engine::values::camera)
		return;

	if (const auto lp = engine::values::local_game_world->get_local_player())
	{
		const auto physical = lp->physical;

		if (!physical)
			return;

		const auto stamina = physical->stamina;

		if (!stamina)
			return;

		stamina->current = 1000.f;
	}
}
