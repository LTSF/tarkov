#include "types.hpp"

#include "../engine.hpp"

#include <mutex>

engine::types::object_t* engine::types::game_object_manager_t::last_tagged_object() const
{
	return *reinterpret_cast< engine::types::object_t **>(reinterpret_cast<std::uintptr_t>(this) + offsets::object::last_tagged_object);
}

engine::types::object_t* engine::types::game_object_manager_t::first_tagged_object() const
{
	return *reinterpret_cast<engine::types::object_t **>(reinterpret_cast<std::uintptr_t>(this) + offsets::object::first_tagged_object);
}

engine::types::object_t* engine::types::game_object_manager_t::last_camera_tagged_object() const
{
	if (!this)
		return nullptr;

	return *reinterpret_cast<engine::types::object_t **>(reinterpret_cast<std::uintptr_t>(this) + offsets::object::last_camera_tagged_object);
}

engine::types::object_t* engine::types::game_object_manager_t::first_camera_tagged_object() const
{
	if (!this)
		return nullptr;

	return *reinterpret_cast<engine::types::object_t **>(reinterpret_cast<std::uintptr_t>(this) + offsets::object::first_camera_tagged_object);
}

engine::types::object_t* engine::types::game_object_manager_t::last_active_object() const
{
	if (!this)
		return nullptr;

	return *reinterpret_cast<engine::types::object_t **>(reinterpret_cast<std::uintptr_t>(this) + offsets::object::last_active_object);
}

engine::types::object_t* engine::types::game_object_manager_t::first_active_object() const
{
	if (!this)
		return nullptr;

	return *reinterpret_cast<engine::types::object_t **>(reinterpret_cast<std::uintptr_t>(this) + offsets::object::first_active_object);
}

engine::types::class_object_t* engine::types::game_object_t::class_object() const
{
	if (!this)
		return nullptr;
	return *reinterpret_cast<class_object_t **>(reinterpret_cast<std::uintptr_t>(this) + offsets::game_object::class_object);
}

engine::types::game_object_t* engine::types::base_object_t::game_object() const
{
	if (!this)
		return nullptr;
	return *reinterpret_cast<game_object_t **>(reinterpret_cast<std::uintptr_t>(this) + offsets::base_object::game_object);
}

engine::types::base_object_t* engine::types::base_object_t::next_base_object() const
{
	if (!this)
		return nullptr;
	return *reinterpret_cast<base_object_t* *>(reinterpret_cast<std::uintptr_t>(this) + offsets::base_object::next_base_object);
}

mem::vector<engine::types::base_object_t*> engine::types::game_object_manager_t::get_tagged_objects() const
{
	mem::vector< base_object_t* > tagged_objects{};

	auto last_tagged_object = this->last_tagged_object()->as< types::base_object_t >();

	if (!last_tagged_object)
		return tagged_objects;

	auto current_tagged_object = reinterpret_cast<types::base_object_t*>(this->first_tagged_object());

	while (current_tagged_object && current_tagged_object->game_object() && current_tagged_object->game_object() != last_tagged_object->game_object())
	{
		if (current_tagged_object)
			tagged_objects.push_back(current_tagged_object->as< types::base_object_t >());

		current_tagged_object = current_tagged_object->next_base_object();
	}

	if (last_tagged_object)
		tagged_objects.push_back(last_tagged_object);

	return tagged_objects;
}

mem::vector<engine::types::base_object_t*> engine::types::game_object_manager_t::get_camera_objects() const
{
	return mem::vector<base_object_t*>();
}

mem::vector<engine::types::base_object_t*> engine::types::game_object_manager_t::get_active_objects() const
{
	return mem::vector<base_object_t*>();
}

engine::types::base_object_t* engine::types::game_object_manager_t::get_tagged_object_by_name(const name_hash_t name) const
{
	return nullptr;
}

engine::types::base_object_t* engine::types::game_object_manager_t::get_camera_object_by_name(const name_hash_t name) const
{
	if (!this)
		return nullptr;

	auto last_camera_object = this->last_camera_tagged_object()->as< types::base_object_t >();

	if (!last_camera_object)
		return nullptr;

	auto current_camera_object = reinterpret_cast<types::base_object_t*>(this->first_camera_tagged_object() );

	while (current_camera_object && current_camera_object->game_object() && current_camera_object->game_object() != last_camera_object->game_object())
	{
		if (current_camera_object)
		{
			const auto game_object = current_camera_object->game_object();

			if (game_object && game_object->is(name))
			{
				return current_camera_object;
			}
		}
		else
		{
			break;
		}

		current_camera_object = current_camera_object->next_base_object();
	}

	if (last_camera_object)
	{
		const auto game_object = last_camera_object->game_object();

		if (game_object && game_object->is(name))
		{
			return last_camera_object->as< base_object_t >();
		}
	}

	return nullptr;
}

engine::types::base_object_t* engine::types::game_object_manager_t::get_active_object_by_name(const name_hash_t name) const
{
	if (!this)
		return nullptr;

	auto last_active_object = this->last_active_object()->as< types::base_object_t >();

	if (!last_active_object)
		return nullptr;

	auto current_active_object = reinterpret_cast<types::base_object_t*>(this->first_active_object());

	while (current_active_object && current_active_object->game_object() && current_active_object->game_object() != last_active_object->game_object())
	{
		if (current_active_object)
		{
			const auto game_object = current_active_object->game_object();

			if (game_object && game_object->is(name))
			{
				return current_active_object;
			}
		}
		else
		{
			break;
		}

		current_active_object = current_active_object->next_base_object();
	}

	if (last_active_object)
	{
		const auto game_object = last_active_object->game_object();

		if (game_object && game_object->is(name))
		{
			return last_active_object->as< base_object_t >();
		}
	}

	return nullptr;
}

mem::vector<engine::types::player_t*> engine::types::local_game_world_t::get_players() const
{
	mem::vector< engine::types::player_t* > players;

	if (!this)
		return players;

	const auto generic_list = *reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(this) + engine::offsets::game_world::registered_players);

	const auto size = *reinterpret_cast< std::uint32_t* >(generic_list + offsets::generic_list::size);

	if (!size)
		return players;

	const auto list = *reinterpret_cast<std::uintptr_t*>(generic_list + offsets::generic_list::elements) + offsets::generic_list::first_element;

	for (auto iterator = 0u; iterator < size; ++iterator)
	{
		const auto player = *reinterpret_cast<engine::types::player_t**>(list + (iterator * sizeof(void*)));

		if (player)
			players.push_back(player);
	}

	return players;
}

engine::types::player_t* engine::types::local_game_world_t::get_local_player() const
{
	if (!this)
		return nullptr;

	const auto generic_list = *reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(this) + engine::offsets::game_world::registered_players);

	const auto size = *reinterpret_cast<std::uint32_t*>(generic_list + offsets::generic_list::size);

	if (!size)
		return nullptr;

	const auto list = *reinterpret_cast<std::uintptr_t*>(generic_list + offsets::generic_list::elements) + offsets::generic_list::first_element;

	const auto player = *reinterpret_cast<engine::types::player_t**>(list);

	if (player)
		return player;

	return nullptr;

}
