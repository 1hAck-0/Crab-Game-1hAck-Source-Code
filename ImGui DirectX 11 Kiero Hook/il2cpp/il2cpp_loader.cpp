#include "pch.h"
#include "il2cpp_loader.h"
#include <Windows.h>
#include <cinttypes>
#include <string>
#include <vector>
#include <optional>
#include "../utils/xorstr.hpp"

#define DO_API(r, n, p) r (*n) p
#include "api_functions.h"
#undef DO_API

namespace il2cpp
{
	const translations tClasses = {
		{ u8"ႣႣႤႡႤႠႦႛႝႨႣ", "DiffuseAudio" }, // filter (at offset 0x38)
		{ u8"႟ႨႥႨႠႠႥ႟ႦႣႤ", "GameModeTag" }, // TryTagPlayer()
		{ u8"ႡႤႢႨႠႧႨ႙ႜ႙Ⴅ", "GameModeHat" }, // StealHat()
		{ u8"႙ႦႜႤႣႡ႙ႡႝႥႧ", "GameModeBombTag" }, // TryTagPlayer()
		{ u8"ႛႚႠႛႤႝႠႢ႙Ⴂ႟", "Tile" }, // SetTileColor()
		{ u8"ႝႜ႟ႣႥႤႦႜ႞Ⴂ႞", "GameModeTileDrive" }, // nTeams
		{ u8"ႛႥႨႢႤႤႧႛႦႝႡ", "GameModeHnS" }, // seekers
		{ u8"ႤႜႠႜႧႡႣႠႠႢႥ", "ItemGun" }, // GetHit()
		{ u8"ႝႣႝ႙ႧႤႛႡႠႧႥ", "ItemSnowball" }, // GetHit()
		{ u8"Ⴆႛ႟ႜႤႝႤ႙ႝႠႛ", "JumpPad" }, // pushForce
		{ u8"ႠႧႢႨ႞ႛႣ႟ႛ႙Ⴀ", "ItemPrefab" }, // set_itemId()
		{ u8"ႦႡႡႦႜႜႛႤႢ႟Ⴄ", "KillPlayerOutOfBounds" }, // killHeight
		{ u8"ႠႤႨႡႨႚႨႡ႞႙႟", "SteamManager" }, // gameAppId
		{ u8"ႝႨႤႛ႙ႝႢႡႣႠႣ", "GameServer" }, // respawnTime
		{ u8"ႥႜႤ႙ႠႣႛႝႤႦႝ", "Packet" }, // ButtonSetNextPlayer() (first argument)
		{ u8"ႡႠႡ႞ႥႡႝႨႥ႙ႚ", "GameManager" }, // whatIsPlayer (offset 0x64)
		{ u8"Ⴄႚႛ႞ႣႝႛႥႡ႟႙", "GameUI" }, // UpdateChat(bool)
		{ u8"ႣႣႝ႟ႚႤႢႚႢ႙Ⴇ", "MenuUI" }, // ExitGame()
		{ u8"႟ႢႨႡႦ႙ႠႝႜႣႢ", "ItemManager" }, // GetItemById()
		{ u8"ႤႛႥ႞ႣႣႛႢႝႝႢ", "GameModeRedLight" }, // moveThreshold
		{ u8"Ⴅႚ႞႞႙ႨႨႚႝ႟ႜ", "PlayerManager" }, // get_PlayerManager()
		{ u8"႙႟ႝ႞ႛႝ႞ႤႨႦႢ", "CameraRecoil" }, // AddRecoil
		{ u8"ႦႡ႙ႠႣႦႥႥႚ႙ႚ", "GameModeLights" }, // public void ToggleLights() (not static!)
		{ u8"ႝႨႠႛႛႠႜႜႦႚႧ", "ClientSend" }, // packetsSent
		{ u8"Ⴂႚ႙ႦႣ႙ႝ႟႞Ⴂႛ", "PlayerMovement" }, // playerCam (inside of the obfuscated class)
		{ u8"ႛ႟ႣႥ႙Ⴃ႙ႝႨ႟ႛ", "OnlinePlayerMovement" }, // get_onlinePlayerMovement()
		{ u8"ႥႠႤႡႛ႙ႤႠႢ႞Ⴄ", "PlayerStatus" }, // maxHp
		{ u8"ႢႣႛႚႦ႞ႣႣႦ႙႟", "MoveCamera" }, // mainCam
		{ u8"႟႟ႛႡႚ႟ႥႤႣႤႡ", "BustlingButtonInteract" }, // buttonId (offset 0x20)
		{ u8"ႜႤ႞႙Ⴇ႟႟ႝႤ႟Ⴃ", "GameModeButtons" }, // buttonId (offset 0x28)
		{ u8"ႠႡ႞ႧႦႧ႞ႥႝႛႠ", "Gun" }, // currentGunDragMultiplier
		{ u8"ႝႦ႞ႣႧႢႣႨႨ႞Ⴁ", "Chatbox" }, // SendMessage()
		{ u8"႟ႝႤႧႢႡႛႛႜ႟Ⴃ", "GameSettings" }, // LobbyCodeOnly
		{ u8"ႛႝ႙Ⴆ႟ႦႨႚႥႢႚ", "RedLightStatue" }, // head (Transform at offset 0x60)
		{ u8"႙ႝႦႥႝႚႢႡ႙ႥႧ", "GlassBreak" }, // MakeSolid()
		{ u8"ႠႨႠႤႨႠႚႥႧႧႜ", "ServerSend" }, // public static MakeSeeker(ulong, bool)
		{ u8"ႣႢႨ႙Ⴄ႙ႣႤႜႡႣ", "GameLoop" }, // StartGames()
		{ u8"ႧႢႦႢႦႥႦ႙႟ႚႝ", "CameraShaker" }, // DamageShake()
		{ u8"Ⴈ႙႙႙ႤႤႨႝႚႤႥ", "PlayerInventory" }, // ForceGiveItem(ItemData)
		{ u8"႞Ⴈ႞ႥႜႚႛႠႦႝႥ", "PlayerListingPrefab" }, // username (string at offset 0x28)
		{ u8"ႛႜႥႦႢႨႨႚႚႣႣ", "PlayerList" }, // UpdateList()
		{ u8"ႨႨ႞ႥႚႚႡႠႚႨႜ", "GameModeGlass" }, // SetGlassPieces()
	};
	const translations tFields = {

	};
	const translations tMehtods = {
		{ u8"ႚ႞ႢႧႥႛ႙Ⴀ႟ႝႢ", "op_Implicit" }, // float -> obscured float							// 40 53 48 83 ec ? 0f 57 c0 0f 29 74 24 ? 0f 11 01 33 c0
		{ u8"ႚႝ႟႟႟ႛႦႤ႟Ⴄ႞", "op_Implicit" }, // obscured float -> float							// find with Mono->MonoDissector->ACTk.Runtime.dll->ObscuredFloat->Methods->Check all possible methods with break points

		{ u8"႙Ⴁႚ႟Ⴀ႞ႢႛႧႣ႙", "op_Implicit" }, // bool -> obscured bool								// 48 89 5c 24 ? 57 48 83 ec ? 33 c0 0f b6 da
		{ u8"ႚႝ႟႟႟ႛႦႤ႟Ⴄ႞", "op_Implicit" }, // obscured bool -> bool								// find with Mono->MonoDissector->ACTk.Runtime.dll->ObscuredBool->Methods->Check all possible methods with break points

		{ u8"ႨႨႡ႟႞ႢႥႢႚ႞႟", "op_Implicit" }, // int -> obscured int									// 48 89 5c 24 ? 57 48 83 ec ? 0f 57 c0 33 c0 0f 11 01 89 41
		{ u8"ႚႝ႟႟႟ႛႦႤ႟Ⴄ႞", "op_Implicit" }, // obscured int -> int								// :CodeStage.AntiCheat.ObscuredTypes.ObscuredInt):System.Int32 // search for it in cheat engine

		{ u8"ႨႢႚႡႨ႞႙႙ႝ႟႙", "FindWinnerId" }, // GameLoop												// 48 89 5c 24 ? 48 89 74 24 ? 57 48 83 ec ? 80 3d ? ? ? ? ? 75 ? 48 8d 0d ? ? ? ? e8 ? ? ? ? 48 8d 0d ? ? ? ? e8 ? ? ? ? 48 8d 0d ? ? ? ? e8 ? ? ? ? 48 8d 0d ? ? ? ? e8 ? ? ? ? 48 8d 0d ? ? ? ? e8 ? ? ? ? 48 8d 0d ? ? ? ? e8 ? ? ? ? 48 8d 0d ? ? ? ? e8 ? ? ? ? c6 05 ? ? ? ? ? 33 ff 48 89 bc 24 ? ? ? ? 48 63 f7 89 74 24 ? 48 8b 05 ? ? ? ? 48 8b 88 ? ? ? ? 48 8b 01 48 85 c0 0f 84 ? ? ? ? 48 8b 48 ? 48 85 c9 0f 84 ? ? ? ? 48 8b 15 ? ? ? ? e8 ? ? ? ? 48 85 c0 0f 84 ? ? ? ? 4c 8b 05 ? ? ? ? 48 8b d0 48 8d 4c 24 ? e8 ? ? ? ? 0f 10 44 24 ? 0f 11 44 24 ? f2 0f 10 4c 24 ? f2 0f 11 4c 24 ? 66 0f 1f 44 00 ? 48 8b 15 ? ? ? ? 48 8d 4c 24 ? e8 ? ? ? ? 84 c0 74 ? 48 8b 5c 24 ? 48 8b 0d ? ? ? ? f6 81 ? ? ? ? ? 74 ? 83 b9 ? ? ? ? ? 75 ? e8 ? ? ? ? 45 33 c0 33 d2 48 8b cb e8 ? ? ? ? 84 c0 75 ? 48 85 db 0f 84 ? ? ? ? 38 43 ? 75 ? 48 8b 7b ? 48 89 bc 24 ? ? ? ? c7 44 b4 ? ? ? ? ? 8b 5c 24 ? ff c3 89 5c 24 ? 48 8b 15 ? ? ? ? 48 8d 4c 24 ? e8 ? ? ? ? eb ? c7 44 b4 ? ? ? ? ? 8b 5c 24 ? ff c3 89 5c 24 ? 48 8b 15 ? ? ? ? 48 8d 4c 24 ? e8 ? ? ? ? eb ? 48 8b 15 ? ? ? ? 48 8d 4c 24 ? e8 ? ? ? ? 48 8b 8c 24 ? ? ? ? 48 85 c9 75 ? 48 8b bc 24 ? ? ? ? 8b 5c 24 ? 85 db 74 ? 48 63 cb 83 7c 8c ? ? 75 ? 48 8b c7 4c 8d 5c 24 ? 49 8b 5b ? 49 8b 73 ? 49 8b e3 5f c3 33 c0 4c 8d 5c 24 ? 49 8b 5b ? 49 8b 73 ? 49 8b e3 5f c3 e8 ? ? ? ? 90 e8 ? ? ? ? 90 33 d2 e8

		{ u8"ႚ႞Ⴃ႞ႠႨႤႢႤႦႣ", "CanSeePlayer" }, // GameModeRedLight									// 48 89 7c 24 ? 55 48 8d 6c 24 ? 48 81 ec ? ? ? ? 80 3d ? ? ? ? ? 48 8b fa 75 ? 48 8d 0d ? ? ? ? e8 ? ? ? ? 48 8d 0d ? ? ? ? e8 ? ? ? ? 48 8d 0d ? ? ? ? e8 ? ? ? ? 48 8d 0d ? ? ? ? e8 ? ? ? ? 48 8d 0d ? ? ? ? e8 ? ? ? ? 48 8d 0d ? ? ? ? e8 ? ? ? ? c6 05 ? ? ? ? ? 33 c0 48 89 9c 24 ? ? ? ? 48 89 45 ? 0f 57 c0 89 45 ? 48 8b 05 ? ? ? ? 0f 11 45 ? 48 89 b4 24 ? ? ? ? 0f 11 45 ? 48 8b 88 ? ? ? ? 0f 29 b4 24 ? ? ? ? 48 8b 01 48 85 c0 0f 84 ? ? ? ? 48 8b 48 ? 48 85 c9 0f 84 ? ? ? ? 4c 8b 05 ? ? ? ? 48 8b d7 e8 ? ? ? ? 48 8b 0d ? ? ? ? 48 8b d8 f6 81 ? ? ? ? ? 74 ? 83 b9 ? ? ? ? ? 75 ? e8 ? ? ? ? 45 33 c0 33 d2 48 8b cb e8 ? ? ? ? 84 c0 0f 85 ? ? ? ? 48 8b 05 ? ? ? ? 48 8b 88 ? ? ? ? 48 8b 01 48 85 c0 0f 84 ? ? ? ? 48 8b 50 ? 48 85 d2 0f 84 ? ? ? ? 45 33 c0 48 8d 4d ? e8 ? ? ? ? f2 0f 10 30 8b 70 ? f2 0f 11 75 ? 89 75 ? 48 85 db 0f 84 ? ? ? ? 33 d2 48 8b cb e8 ? ? ? ? 48 85 c0 0f 84 ? ? ? ? 45 33 c0 48 8d 4d ? 48 8b d0 e8 ? ? ? ? 8b 48 ? f2 0f 10 18 0f 28 cb 89 4d ? f3 0f 10 45 ? 0f 28 d1 f3 0f 5c 45 ? f3 0f 5c 5d ? 89 4d ? 0f c6 d2 ? f3 0f 5c 55 ? 89 4d ? f3 0f 11 45 ? f2 0f 11 4d ? 33 c0 f2 0f 11 75 ? 0f 28 c3 48 89 45 ? 8b 45 ? 4c 8d 45 ? 0f 14 c2 48 8d 55 ? 0f 57 c9 f2 0f 11 45 ? 45 33 c9 89 45 ? 48 8d 4d ? 89 75 ? 0f 11 4d ? e8 ? ? ? ? 48 8b 05 ? ? ? ? 48 8b 88 ? ? ? ? 48 8b 01 48 85 c0 0f 84 ? ? ? ? 8b 48 ? 33 d2 e8 ? ? ? ? f2 0f 10 4d ? 4c 8d 45 ? 0f 10 45 ? 48 8d 4d ? 48 c7 44 24 ? ? ? ? ? f3 0f 10 1d ? ? ? ? f2 0f 11 4d ? f3 0f 10 0d ? ? ? ? 0f 29 45 ? 89 44 24 ? e8 ? ? ? ? 84 c0 0f 84 ? ? ? ? 33 d2 48 8d 4d ? e8 ? ? ? ? 48 85 c0 0f 84 ? ? ? ? 33 d2 48 8b c8 e8 ? ? ? ? 48 85 c0 0f 84 ? ? ? ? 33 d2 48 8b c8 e8 ? ? ? ? 48 8b 0d ? ? ? ? 33 d2 8b d8 e8 ? ? ? ? 3b d8 75 ? 33 d2 48 8d 4d ? e8 ? ? ? ? 48 85 c0 74 ? 33 d2 48 8b c8 e8 ? ? ? ? 48 85 c0 74 ? 33 d2 48 8b c8 e8 ? ? ? ? 48 85 c0 74 ? 48 8b 15 ? ? ? ? 48 8b c8 e8 ? ? ? ? 48 85 c0 74 ? 48 39 78 ? 75 ? b0
	};

	std::string translate(const translations& map, const std::string& value)
	{
		const auto pos = map.find(value);
		if (pos == map.end())
			return value;
		return pos->second;
	}

	std::string translateType(const std::string& type, const std::string& assembly)
	{
		Il2CppClass* cls = _inner_get_class(assembly, "", type);
		std::string tp = cls ? cls->name : type;
		tp += XorStr(", ");
		tp += assembly;
		return tp;
	}

	std::string get_method_parameters(const MethodInfo* const method_info)
	{
		std::string parameters;
		for (std::uint32_t i = 0; i < il2cpp_method_get_param_count(method_info); ++i)
		{
			if (i) parameters.append(", ");
			parameters.append(il2cpp_type_get_name(il2cpp_method_get_param(method_info, i)));
		}
		return parameters;
	}

	bool compare_field(
		const FieldInfo* const field_info,
		const std::string& field_type,
		const std::string& field_name)
	{
		if (field_name.compare(translate(tFields, field_info->name))) return false;
		if (field_type.compare(il2cpp_type_get_name(field_info->type))) return false;
		return true;
	}

	FieldInfo* find_field(
		Il2CppClass* const __class,
		const std::string& field_type,
		const std::string& field_name)
	{
		void* iterator = nullptr;
		FieldInfo* field = nullptr;

		while ((field = il2cpp_class_get_fields(__class, &iterator)))
		{
			if (compare_field(field, field_type, field_name))
			{
				return field;
			}
		}
		return NULL;
	}

	bool compare_method(
		const MethodInfo* const method_info,
		const std::string& return_type,
		const std::string& method_name,
		const std::string& parameters)
	{
		if (method_name.compare(translate(tMehtods, method_info->name))) return false;
		if (return_type != "auto" && return_type.compare(il2cpp_type_get_name(method_info->return_type))) return false;
		if (parameters != "auto" && parameters.compare(get_method_parameters(method_info))) return false;
		return true;
	}

	Il2CppMethodPointer find_method(
		Il2CppClass* const __class,
		const std::string& return_type,
		const std::string& method_name,
		const std::string& parameters)
	{
		void* iterator = nullptr;
		const MethodInfo* method = nullptr;

		while ((method = il2cpp_class_get_methods(__class, &iterator)))
		{
			if (compare_method(method, return_type, method_name, parameters))
			{
				return method->methodPointer;
			}
		}
		return nullptr;
	}

	int32_t find_offset(
		Il2CppClass* const __class,
		const std::string& field_type,
		const std::string& field_name)
	{
		void* iterator = nullptr;
		const FieldInfo* field = nullptr;

		while ((field = il2cpp_class_get_fields(__class, &iterator)))
		{
			if (compare_field(field, field_type, field_name))
			{
				return field->offset;
			}
		}
		return NULL;
	}

	static bool compare_class(
		const Il2CppClass* const __klass,
		const std::string& __namespace,
		const std::string& class_name)
	{
		if (__namespace.compare(__klass->namespaze)) return false;
		if (class_name.compare(translate(tClasses, __klass->name))) return false;
		return true;
	}

	Il2CppClass* _inner_get_class(
		const std::string& assembly_name,
		const std::string& __namespace,
		const std::string& class_name)
	{
		Il2CppDomain* domain = il2cpp_domain_get();
		const Il2CppAssembly* assembly = il2cpp_domain_assembly_open(domain, assembly_name.c_str());
		if (!assembly) return nullptr;

		const Il2CppImage* image = assembly->image;
		if (!image) return nullptr;

		const size_t count = il2cpp_image_get_class_count(image);
		for (auto i = 0u; i < count; ++i)
		{
			const auto __klass = il2cpp_image_get_class(image, i);
			if (compare_class(__klass, __namespace, class_name))
			{
				return const_cast<Il2CppClass*>(__klass);
			}
		}

		return nullptr;
	}

	std::uintptr_t get_method(
		const std::string& assembly_name,
		const std::string& return_type,
		const std::string& __namespace,
		const std::string& class_name,
		const std::string& method_name,
		const std::string& parameters)
	{
		auto __class = _inner_get_class(assembly_name, __namespace, class_name);
		if (!__class) return NULL;
		return reinterpret_cast<std::uintptr_t>(find_method(__class, return_type, method_name, parameters));
	}

	int32_t get_offset(
		const std::string& assembly_name,
		const std::string& __namespace,
		const std::string& class_name,
		const std::string& field_type,
		const std::string& field_name)
	{
		auto __class = _inner_get_class(assembly_name, __namespace, class_name);
		if (!__class) return NULL;
		return find_offset(__class, field_type, field_name);
	}

	std::uintptr_t get_class(
		const std::string& assembly_name,
		const std::string& __namespace,
		const std::string& class_name)
	{
		return reinterpret_cast<std::uintptr_t>(_inner_get_class(assembly_name, __namespace, class_name));
	}

	generic_string* string_to_generic(const std::string& input)
	{
		static const auto marshal_to_string = il2cpp::get_method(XorStr("mscorlib"),
			XorStr("System.String"),
			XorStr("System.Runtime.InteropServices"),
			XorStr("Marshal"),
			XorStr("PtrToStringAnsi"),
			XorStr("System.IntPtr"));
		return reinterpret_cast<generic_string * (*)(const char*)>(marshal_to_string)(input.c_str());
	}

	std::string generic_to_string(const generic_string* input)
	{
		std::wstring str(input->string);
		return std::string(str.begin(), str.end());
	}

	void* get_type(const std::string& type)
	{
		static const auto GetType = (void* (*)(generic_string * type))il2cpp::get_method(XorStr("mscorlib"),
			XorStr("System.Type"),
			XorStr("System"),
			XorStr("Type"),
			XorStr("GetType"),
			XorStr("System.String"));

		return GetType(string_to_generic(type));
	}

	generic_array* find_objects_of_type(const std::string& __class, const std::string& assembly, const std::string& __namespace, const bool& bGetActiveObjectsToo)
	{
		static const auto FindObjectsOfType = (generic_array * (*)(void* type, bool includeInactive))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
			XorStr("UnityEngine.Object[]"),
			XorStr("UnityEngine"),
			XorStr("Object"),
			XorStr("FindObjectsOfType"),
			XorStr("System.Type, System.Boolean"));

		return FindObjectsOfType(get_type(translateType(__class, assembly)), bGetActiveObjectsToo);
	}

	const MethodInfo* find_method_info(
		Il2CppClass* const __class,
		const std::string& return_type,
		const std::string& method_name,
		const std::string& parameters)
	{
		void* iterator = nullptr;
		const MethodInfo* method = nullptr;

		while ((method = il2cpp_class_get_methods(__class, &iterator)))
		{
			if (compare_method(method, return_type, method_name, parameters))
				return method;
		}
		return nullptr;
	}

	void* invoke(
		const std::string& assembly_name,
		const std::string& return_type,
		const std::string& __namespace,
		const std::string& class_name,
		const std::string& method_name,
		const std::string& parameters_type,
		void** parameters,
		void* object)
	{
		uintptr_t __exception = 0;
		auto exeception = &__exception;
		auto __class = _inner_get_class(assembly_name, __namespace, class_name);
		if (!__class) return nullptr;
		const auto info = find_method_info(__class, return_type, method_name, parameters_type);
		if (!info) return nullptr;
		const auto result = il2cpp_runtime_invoke(info, object, parameters, (Il2CppException**)&exeception);
		if (!result) return nullptr;
		return il2cpp_object_unbox(result);
	}

	void load()
	{
		auto hMod = GetModuleHandleA(XorCStr("GameAssembly.dll"));
		if (!hMod) return;

		#define DO_API(r, n, p) n = (r (*) p)(GetProcAddress(hMod, #n))
		#include "api_functions.h"
		#undef DO_API
	}
}
