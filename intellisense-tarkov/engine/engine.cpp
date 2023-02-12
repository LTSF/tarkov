#include "engine.hpp"
#include "../dependencies/xorstr/xorstr.h"

std::uintptr_t engine::values::unity_player = 0;

engine::types::game_object_manager_t* engine::values::game_object_manager = nullptr;
engine::types::camera_t* engine::values::camera = nullptr;
engine::types::local_game_world_t* engine::values::local_game_world = nullptr;

mem::vector< engine::types::player_t* > engine::values::players;

bool init = false;

void engine::functions::initiate()
{
	if (!init)
	{
		engine::values::unity_player = reinterpret_cast<std::uintptr_t>(LI_FN(GetModuleHandleA)(xorstr("UnityPlayer.dll"))); //TODO XOR

		if (!engine::values::unity_player)
			return;

		values::game_object_manager = *reinterpret_cast<engine::types::game_object_manager_t**>(engine::values::unity_player + engine::offsets::game_object_manager);

		if (!values::game_object_manager)
			return;

		init = true;
	}

	if (!engine::values::unity_player)
	{
		init = false;
		return;
	}

	if (!values::game_object_manager)
	{
		init = false;
		return;
	}

	//if (const auto game_world = engine::values::game_object_manager->get_active_object_by_name(engine::types::name_hash_t::game_world))
	//{
	//	if (const auto game_object = game_world->game_object())
	//	{
	//		if (const auto class_object = game_object->class_object())
	//		{
	//			if (const auto obj = class_object->get_object())
	//			{
	//				if (const auto game_world = *reinterpret_cast<types::local_game_world_t**>(obj + engine::offsets::game_world::self))
	//				{
	//					engine::values::local_game_world = game_world;
	//				}
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	engine::values::local_game_world = nullptr;
	//	engine::values::camera = nullptr;

	//	return;
	//}

	//if (const auto camera = engine::values::game_object_manager->get_camera_object_by_name(engine::types::name_hash_t::fps_camera))
	//{
	//	if (const auto game_object = camera->game_object())
	//	{
	//		if (const auto class_object = game_object->class_object())
	//		{
	//			if (const auto obj = class_object->get_object< types::camera_t* >())
	//			{
	//				engine::values::camera = obj;
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	engine::values::camera = nullptr;
	//}
}

void engine::functions::update_players_vector()
{
	if (!engine::values::local_game_world || !engine::values::camera)
		return;

	engine::values::players = engine::values::local_game_world->get_players();
}