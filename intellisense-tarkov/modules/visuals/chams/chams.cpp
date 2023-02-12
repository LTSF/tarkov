
#include "chams.hpp"

#include "../../../unity/unity.hpp"

#include "../../../dependencies/mem/mem.hpp"

#include "../../../engine/types/types.hpp"
#include "../../../dependencies/xorstr/xorstr.h"


std::uintptr_t old_addr = 0;

void* __fastcall get_skinned_mesh_renderer_hk(void* rcx) 
{
	//auto& ret = *(reinterpret_cast<std::uintptr_t*>(_AddressOfReturnAddress()));
	//const auto old = ret;

	//ret = 0;

	const auto smr = reinterpret_cast<engine::types::skinned_mesh_renderer_t* (__fastcall*)(void*)>(old_addr)(rcx);

	//if (!smr)
	//	return smr;

	//const auto renderer = smr->renderer;

	//const auto material = reinterpret_cast<void* (__fastcall*)(void*)>(engine::values::unity_player + 0x677270)(renderer);

	//reinterpret_cast<void(__fastcall*)(void*, void*)>(engine::values::unity_player + 0x7E4D60)(material, nullptr);

	//ret = old;

	return smr;
}

void modules::visuals::chams::callback()
{
	unity::sdk::thread_attach(unity::sdk::get_root_domain());

	if (const auto assembly_csharp = unity::sdk::get_image_by_name(xorstr("Assembly-CSharp")))
	{
		if (const auto player_class = unity::sdk::class_from_name(assembly_csharp, xorstr("Diz.Skinning"), xorstr("Skin")))
		{
			if (const auto method = unity::sdk::class_get_method_from_name(player_class, xorstr("get_SkinnedMeshRenderer"), -1))
			{
				if (const auto res = unity::sdk::compile_method(method))
				{
					old_addr = mem::hook(reinterpret_cast<std::uintptr_t>(res), reinterpret_cast<std::uintptr_t>(get_skinned_mesh_renderer_hk));
				}
			}
		}
	}
}
