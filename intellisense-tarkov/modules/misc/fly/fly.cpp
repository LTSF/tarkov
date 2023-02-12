#include "fly.hpp"

#include "../../../unity/unity.hpp"

#include "../../../dependencies/mem/mem.hpp"

#include "../../../engine/types/types.hpp"

#include "../../../dependencies/xorstr/xorstr.h"

std::uintptr_t old_jump_addr = 0;
std::uintptr_t old_jump_addr2 = 0;

void __fastcall jump_hk(engine::types::jump_t* rcx)
{

	if(!rcx)
		return reinterpret_cast<void(__fastcall*)(engine::types::jump_t*)>(old_jump_addr)(rcx);

	auto mc = rcx->movement_context;

	if (!mc)
		return reinterpret_cast<void(__fastcall*)(engine::types::jump_t*)>(old_jump_addr)(rcx);

	mc->pose_level = 0.7;
	mc->grounded = true;
	
	return reinterpret_cast<void(__fastcall*)(engine::types::jump_t*)>(old_jump_addr)(rcx);
}

void __fastcall jump_hk2(engine::types::jump_t* rcx)
{

	if (!rcx)
		return reinterpret_cast<void(__fastcall*)(engine::types::jump_t*)>(old_jump_addr2)(rcx);

	auto mc = rcx->movement_context;

	if (!mc)
		return reinterpret_cast<void(__fastcall*)(engine::types::jump_t*)>(old_jump_addr2)(rcx);

	mc->pose_level = 0.7;
	mc->grounded = true;

	return reinterpret_cast<void(__fastcall*)(engine::types::jump_t*)>(old_jump_addr2)(rcx);
}

bool __fastcall hookyhooky(engine::types::jump_t* rcx) //filter condition
{

	return false;
}


void modules::misc::fly::callback() //todo optimize
{
	unity::sdk::thread_attach(unity::sdk::get_root_domain());

	if (const auto assembly_csharp = unity::sdk::get_image_by_name(xorstr("Assembly-CSharp")))
	{
		if (const auto player_class = unity::sdk::class_from_name(assembly_csharp, xorstr(""), xorstr("\uE61E")))
		{
			if (const auto method = unity::sdk::class_get_method_from_name(player_class, xorstr("Jump"), -1))
			{

				if (const auto res = unity::sdk::compile_method(method))
				{
					old_jump_addr = mem::hook(reinterpret_cast<std::uintptr_t>(res), reinterpret_cast<std::uintptr_t>(jump_hk));
				}
			}
		}

		if (const auto player_class = unity::sdk::class_from_name(assembly_csharp, xorstr(""), xorstr("\uE61B")))
		{
			if (const auto method = unity::sdk::class_get_method_from_name(player_class, xorstr("Jump"), -1))
			{

				if (const auto res = unity::sdk::compile_method(method))
				{
					old_jump_addr2 = mem::hook(reinterpret_cast<std::uintptr_t>(res), reinterpret_cast<std::uintptr_t>(jump_hk2));
				}
			}
		}

		if (const auto player_class = unity::sdk::class_from_name(assembly_csharp, xorstr(""), xorstr("\uE620")))
		{
			if (const auto method = unity::sdk::class_get_method_from_name(player_class, xorstr("PhysicalConditionIs"), -1))
			{

				if (const auto res = unity::sdk::compile_method(method))
				{
					mem::hook(reinterpret_cast<std::uintptr_t>(res), reinterpret_cast<std::uintptr_t>(hookyhooky));
				}
			}
		}
	}
}

/*
std::uintptr_t old_jump_addr = 0;

void __fastcall ManualAnimatorMoveUpdate_hk(engine::types::jump_t* rcx, float delta_time)
{
	if(!rcx)
		return reinterpret_cast<void(__fastcall*)(engine::types::jump_t*, float delta_time)>(old_jump_addr)(rcx, delta_time);

	auto mc = rcx->movement_context;

	if (!mc)
		return reinterpret_cast<void(__fastcall*)(engine::types::jump_t*, float delta_time)>(old_jump_addr)(rcx, delta_time);

	if(!mc->grounded)
		mc->free_fall_time = -100.f;

	return reinterpret_cast<void(__fastcall*)(engine::types::jump_t*, float delta_time)>(old_jump_addr)(rcx, delta_time);
}

void modules::misc::fly::callback() //todo optimize
{
	unity::sdk::thread_attach(unity::sdk::get_root_domain());

	if (const auto assembly_csharp = unity::sdk::get_image_by_name(xorstr("Assembly-CSharp")))
	{
		if (const auto player_class = unity::sdk::class_from_name(assembly_csharp, xorstr(""), xorstr("\uE61C")))
		{
			if (const auto method = unity::sdk::class_get_method_from_name(player_class, xorstr("ManualAnimatorMoveUpdate"), -1))
			{
				std::printf("got func \n");

				if (const auto res = unity::sdk::compile_method(method))
				{
					old_jump_addr = mem::hook(reinterpret_cast<std::uintptr_t>(res), reinterpret_cast<std::uintptr_t>(ManualAnimatorMoveUpdate_hk));
				}
			}
		}
	}
}
*/
