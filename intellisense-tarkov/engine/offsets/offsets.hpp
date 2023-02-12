#pragma once
#include <cstdint>

#include "../../dependencies/mem/mem.hpp"

namespace engine::offsets
{
	constexpr auto game_object_manager = 0x17F8D28;

	void init();
}

namespace engine::offsets::object //TODO xor these mfs
{
	constexpr auto last_tagged_object = 0x0;
	constexpr auto first_tagged_object = 0x8;

	constexpr auto last_camera_tagged_object = 0x10;
	constexpr auto first_camera_tagged_object = 0x18;

	constexpr auto last_active_object = 0x20;
	constexpr auto first_active_object = 0x28;
}

namespace engine::offsets::base_object
{
	constexpr auto next_base_object = 0x8;
	constexpr auto game_object = 0x10;
}


namespace engine::offsets::game_object
{
	constexpr auto class_object = 0x30;
	constexpr auto name = 0x60;
}

namespace engine::offsets::class_object
{
	constexpr auto transform = 0x8;
	constexpr auto object_reference = 0x18;
}

namespace engine::offsets::game_world
{
	constexpr auto self = 0x28;

	constexpr auto registered_players = 0x80;
}

namespace engine::offsets::generic_list
{
	constexpr auto elements = 0x10;
	constexpr auto size = 0x18;

	constexpr auto first_element = 0x20;
}


