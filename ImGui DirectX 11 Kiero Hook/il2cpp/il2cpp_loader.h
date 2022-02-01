#pragma once
#include <string>
#include <cinttypes>
#include "appdata_types.h"

template <typename T>
struct generic_array_t
{
	void* klass;
	void* monitor;
	void* bounds;
	size_t length;
	T items[1024];
};

template <typename T>
struct generic_list_t
{
	void* klass;
	void* monitor;
	generic_array_t<T>* items;
	int size;
	int version;
};

template <typename TKey, typename TValue>
struct generic_dictionary_entry
{
	void* monitor;
	TKey key;
	TValue value;
};

template <typename TKey, typename TValue>
struct generic_dictionary
{
	void* klass;
	void* monitor;
	void* buckets;
	generic_array_t<generic_dictionary_entry<TKey, TValue>>* entries;
	uint32_t count;
	uint32_t version;
	uint32_t freeList;
	uint32_t freeCount;
	void* comparer;
	void* keys;
	void* values;
	unsigned char _syncRoot;
};
using generic_array = generic_array_t<uintptr_t>;

struct generic_string
{
	uintptr_t instance;
	uintptr_t pad;
	int32_t length;
	wchar_t string[1024];
};

template <size_t T_size>
struct generic_item
{
	char content[T_size] = { 0 };
};

namespace il2cpp
{
	void load();

	using translations = std::unordered_map<std::string, std::string>;
	std::string translate(const translations& map, const std::string& value);
	extern const translations tClasses;
	extern const translations tFields;
	extern const translations tMehtods;

	Il2CppClass* _inner_get_class(
		const std::string& assembly_name,
		const std::string& __namespace,
		const std::string& class_name);

	std::uintptr_t get_method(
		const std::string& assembly_name,
		const std::string& return_type,
		const std::string& __namespace,
		const std::string& class_name,
		const std::string& method_name,
		const std::string& parameters);

	std::uintptr_t get_class(
		const std::string& assembly_name,
		const std::string& __namespace,
		const std::string& class_name);

	int32_t get_offset(
		const std::string& assembly_name,
		const std::string& __namespace,
		const std::string& class_name,
		const std::string& field_type,
		const std::string& field_name);

	void* invoke(
		const std::string& assembly_name,
		const std::string& return_type,
		const std::string& __namespace,
		const std::string& class_name,
		const std::string& method_name,
		const std::string& parameters_type,
		void** parameters,
		void* object);

	std::string translateType(const std::string& type, const std::string& assembly);

	void* get_type(const std::string& type);

	generic_array* find_objects_of_type(const std::string& __class, const std::string& assembly, const std::string& __namespace = "", const bool& bGetActiveObjectsToo = false);

	generic_string* string_to_generic(const std::string& input);
	std::string generic_to_string(const generic_string* input);
}
