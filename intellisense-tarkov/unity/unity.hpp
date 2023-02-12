#pragma once

#include <Windows.h>
#include <cstdint>

#include "../dependencies/mem/mem.hpp"

#include "../dependencies/xorstr/xorstr.h"

constexpr bool mono = true;

namespace unity::values
{
	static HMODULE module = nullptr;
}

namespace unity::sdk
{
	using func_t =  void(__cdecl*)(void* data, void* user_data);

	using domain_t = std::uintptr_t;
	using thread_t = std::uintptr_t;
	using assembly_t = std::uintptr_t;
	using image_t = std::uintptr_t;
	using class_t = std::uintptr_t;
	using field_t = std::uintptr_t;

	using get_root_domain_t = domain_t(__cdecl*)();
	extern get_root_domain_t get_root_domain;

	using domain_get_t = domain_t(__cdecl*)();
	extern domain_get_t domain_get;

	using thread_attach_t = thread_t(__cdecl*)(domain_t);
	extern thread_attach_t thread_attach;

	using assembly_open_t = assembly_t(__cdecl*)(domain_t, const char*);
	extern assembly_open_t assembly_open;

	using assembly_foreach_t = void* (__cdecl*)(std::uintptr_t, void*);
	extern assembly_foreach_t assembly_foreach;

	using assembly_get_image_t = image_t(__cdecl*)(assembly_t);
	extern assembly_get_image_t assembly_get_image;

	using image_get_name_t = const char*(__cdecl*)(image_t);
	extern image_get_name_t image_get_name;

	using class_from_name_t = class_t(__cdecl*)(image_t, const char*, const char*);
	extern class_from_name_t class_from_name;

	using get_field_from_name_t = field_t(__cdecl*)(class_t, const char*);
	extern get_field_from_name_t get_field_from_name;

	using field_get_offset_t = std::uintptr_t(__cdecl*)(field_t);
	extern field_get_offset_t field_get_offset;

	using class_get_fields_t = void*(__cdecl*)(void*, void*);
	extern class_get_fields_t class_get_fields;

	using field_get_name_t = const char*(__cdecl*)(field_t);
	extern field_get_name_t field_get_name;

	using class_get_method_from_name_t = void* (__cdecl*)(class_t mclass, const char* name, int param_count);
	extern class_get_method_from_name_t class_get_method_from_name;
	
	using runtime_invoke_t = void* (__cdecl*)(void* method, void* obj, void** params, void** exc);
	extern runtime_invoke_t runtime_invoke;

	using compile_method_t = void* (__cdecl*)(void* method);
	extern compile_method_t compile_method;

	mem::vector<assembly_t> get_assemblies();

	mem::vector<field_t> get_fields(class_t c);

	image_t get_image_by_name(const char* const image_name);

	field_t get_field_by_name(unity::sdk::class_t c, const char* const field_name);

	field_t get_field_by_name(mem::vector<unity::sdk::field_t> c, const char* const field_name);

	void setup_functions();

	template<typename t = std::uintptr_t>
	inline t get_function(const char* const name)
	{
		const char* prefix = mono ? "mono_" : "il2cpp_";

		char buf[100];
		mem::cstrcpy(buf, prefix, sizeof(buf));
		mem::cstrcat(buf, name);

		return reinterpret_cast<t>(reinterpret_cast<std::uintptr_t>(LI_FN(GetProcAddress).in(LI_MODULE("kernel32.dll").cached())(unity::values::module, buf)));
	}

	bool initialize(const char* const module_name);
}