#include "unity.hpp"
#include "../dependencies/xorstr/xorstr.h"

unity::sdk::get_root_domain_t unity::sdk::get_root_domain = nullptr;
unity::sdk::domain_get_t unity::sdk::domain_get = nullptr;
unity::sdk::thread_attach_t unity::sdk::thread_attach = nullptr;

unity::sdk::assembly_open_t unity::sdk::assembly_open;
unity::sdk::assembly_foreach_t unity::sdk::assembly_foreach = nullptr;
unity::sdk::assembly_get_image_t unity::sdk::assembly_get_image = nullptr;
unity::sdk::image_get_name_t unity::sdk::image_get_name = nullptr;

unity::sdk::class_from_name_t unity::sdk::class_from_name = nullptr;
unity::sdk::get_field_from_name_t unity::sdk::get_field_from_name = nullptr;
unity::sdk::field_get_offset_t unity::sdk::field_get_offset = nullptr;
unity::sdk::class_get_fields_t unity::sdk::class_get_fields = nullptr;

unity::sdk::field_get_name_t unity::sdk::field_get_name = nullptr;
unity::sdk::class_get_method_from_name_t unity::sdk::class_get_method_from_name = nullptr;

unity::sdk::runtime_invoke_t unity::sdk::runtime_invoke = nullptr;

unity::sdk::compile_method_t unity::sdk::compile_method = nullptr;

void _cdecl iterator(std::uintptr_t domain, mem::vector<std::uintptr_t>* v)
{
	v->push_back(domain);
}

mem::vector<unity::sdk::assembly_t> unity::sdk::get_assemblies()
{
	mem::vector<unity::sdk::assembly_t> vec{};

	assembly_foreach(reinterpret_cast<std::uintptr_t>(iterator), &vec);

	return vec;
}

mem::vector<unity::sdk::field_t> unity::sdk::get_fields(class_t c)
{
	mem::vector<unity::sdk::field_t> vec{};

	void* iter = nullptr;
	void* field;

	do
	{
		field = class_get_fields(reinterpret_cast<void*>(c), &iter);

		if (field)
			vec.push_back(reinterpret_cast<std::uintptr_t>(field));

	} while (field);

	return vec;
}

unity::sdk::image_t unity::sdk::get_image_by_name(const char* const image_name)
{
	const auto temp = get_assemblies();

	for (auto i = 0u; i < temp.size(); ++i)
	{
		if (const auto assembly = temp[i])
		{
			if (const auto image = assembly_get_image(assembly))
			{
				if (!mem::cstrcmp(image_get_name(image), image_name))
				{
					return image;
				}
			}
		}
	}

	return 0;
}

unity::sdk::field_t unity::sdk::get_field_by_name(unity::sdk::class_t c, const char* const field_name)
{
	const auto fields = get_fields(c);

	for (auto i = 0u; i < fields.size(); ++i)
	{
		if (const auto field = fields[i])
		{
			if (!mem::cstrcmp(field_get_name(field), field_name))
				return field;
		}
	}

	return 0;
}

unity::sdk::field_t unity::sdk::get_field_by_name(mem::vector<unity::sdk::field_t> c, const char* const field_name)
{
	for (auto i = 0u; i < c.size(); ++i)
	{
		if (const auto field = c[i])
		{
			if (!mem::cstrcmp(field_get_name(field), field_name))
			{
				if (const auto offset = field_get_offset(field))
					return field;
			}
		}
	}

	return 0;
}


void unity::sdk::setup_functions() //TODO MUST SPOOF RET ADDR TO INSIDE EFT
{
	unity::sdk::get_root_domain = get_function<unity::sdk::get_root_domain_t>(xorstr("get_root_domain"));
	unity::sdk::domain_get = get_function<unity::sdk::get_root_domain_t>(xorstr("domain_get"));
	unity::sdk::thread_attach = get_function<unity::sdk::thread_attach_t>(xorstr("thread_attach"));

	unity::sdk::assembly_open = get_function<assembly_open_t>(xorstr("domain_assembly_open"));
	unity::sdk::assembly_foreach = get_function<assembly_foreach_t>(xorstr("assembly_foreach"));
	unity::sdk::assembly_get_image = get_function<assembly_get_image_t>(xorstr("assembly_get_image"));
	unity::sdk::image_get_name = get_function<image_get_name_t>(xorstr("image_get_name"));

	unity::sdk::class_from_name = get_function<class_from_name_t>(xorstr("class_from_name"));
	unity::sdk::get_field_from_name = get_function<get_field_from_name_t>(xorstr("get_field_from_name"));
	unity::sdk::field_get_offset = get_function<field_get_offset_t>(xorstr("field_get_offset"));

	unity::sdk::class_get_fields = get_function<class_get_fields_t>(xorstr("class_get_fields"));
	unity::sdk::field_get_name = get_function<field_get_name_t>(xorstr("field_get_name"));

	unity::sdk::class_get_method_from_name = get_function<class_get_method_from_name_t>(xorstr("class_get_method_from_name"));
	
	unity::sdk::runtime_invoke = get_function<runtime_invoke_t>(xorstr("runtime_invoke"));

	unity::sdk::compile_method = get_function<compile_method_t>(xorstr("compile_method"));
}

bool unity::sdk::initialize(const char* const module_name)
{
	if (const auto module = LI_FN(GetModuleHandleA)(module_name))
	{
		unity::values::module = module;

		unity::sdk::setup_functions();

		return true;
	}

	return false;
}
