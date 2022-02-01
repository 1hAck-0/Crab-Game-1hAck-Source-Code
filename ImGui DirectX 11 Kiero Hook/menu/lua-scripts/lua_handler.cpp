#include "pch.h"
#include "../../cheat/cheat.h"
#include "../../utils/config.h"
#include "lua_handler.h"
#include "lua/lua.hpp"
#include "../menu.h"
#include <fstream>
#include <typeinfo>
namespace fs = std::filesystem;

// HELPER MACROS
#pragma region

#define lua_def_globalnum(name, value) 						lua_pushnumber(lua, value);\
															lua_setglobal(lua, name)

#define lua_def_globalnumex(x) 								lua_pushnumber(lua, x);\
															lua_setglobal(lua, #x)

#define lua_def_globalbool(name, value) 					lua_pushboolean(lua, value);\
															lua_setglobal(lua, name)

#define lua_def_globalstr(name, value) 						lua_pushstring(lua, value);\
															lua_setglobal(lua, name)

#define lua_def_globalstrex(x) 								lua_pushstring(lua, x);\
															lua_setglobal(lua, x)

#define lua_start_ns(nsName)								lua_newtable(lua);\
															lua_pushvalue(lua, lua_gettop(lua));\
															lua_setglobal(lua, nsName)

#define lua_ns_register(funcName, funcPtr)					lua_pushcfunction(lua, funcPtr);\
															lua_setfield(lua, -2, funcName)

#define lua_end_ns()										lua_pop(lua, lua_gettop(lua))

#define lua_start_class(className)							lua_newtable(lua);\
															int _##className##_MT_INDEX = lua_gettop(lua);\
															lua_pushvalue(lua, _##className##_MT_INDEX);\
															lua_setglobal(lua, #className);

#define lua_class_func(name, funcPtr)						lua_pushcfunction(lua, funcPtr);\
															lua_setfield(lua, -2, name)

#define lua_end_class(className)							luaL_newmetatable(lua, #className "_MT");\
															lua_pushstring(lua, XorCStr("__index"));\
															lua_pushvalue(lua, _##className##_MT_INDEX);\
															lua_settable(lua, -3)

#define lua_set_class(className)							luaL_getmetatable(lua, #className "_MT");\
															lua_setmetatable(lua, -2)



#define lua_toint(index)									int(lua_tonumber(lua, index))

#define lua_tofloat(index)									float(lua_tonumber(lua, index))

#define lua_tovec2(index)									{ lua_tofloat(index - 1), lua_tofloat(index) }

#define lua_tovec3(index)									{ lua_tofloat(index - 2), lua_tofloat(index - 1), lua_tofloat(index) }

#pragma endregion


__forceinline std::string FileToString(const fs::path& path)
{
	std::ifstream file;
	file.open(path, std::ios::binary | std::ios::ate);

	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	if (size > 0)
	{
		std::string buffer(size, ' ');
		file.seekg(0);
		file.read(&buffer[0], size);

		return buffer;
	}

	file.close();
	return "";
}


#pragma region LuaFuncs


#pragma region global

struct LuaGlobals
{
	static int Enable(lua_State* lua)
	{
		try
		{
			for (USHORT i = 0; i < cfg.script.files.size(); i++)
			{
				auto script = &cfg.script.files[i];
				if (script->lua == lua)
				{
					script->flags &= ~(ScriptFlags_StatusChanged);
					script->bEnabled = true;
					break;
				}
			}
		}
		catch (...) {}
		return 0;
	}

	static int Disable(lua_State* lua)
	{
		try
		{
			for (USHORT i = 0; i < cfg.script.files.size(); i++)
			{
				auto script = &cfg.script.files[i];
				if (script->lua == lua)
				{
					script->bEnabled = false;
					script->flags &= ~(ScriptFlags_StatusChanged);
					break;
				}
			}
		}
		catch (...) {}
		return 0;
	}

	static int Color(lua_State* lua)
	{
		try
		{
			int numArgs = lua_gettop(lua);
			if (numArgs == 4)
			{
				lua_pushnumber(lua, (ImU32)ImColor(lua_toint(-4), lua_toint(-3), lua_toint(-2), lua_toint(-1)));
			}
			else if (numArgs == 3)
			{
				lua_pushnumber(lua, (ImU32)ImColor(lua_toint(-3), lua_toint(-2), lua_toint(-1)));
			}
			else if (numArgs == 1)
			{
				byte color = lua_tonumber(lua, -1);
				lua_pushnumber(lua, (ImU32)ImColor(color, color, color));
			}
			else if (numArgs == 0)
			{
				lua_pushnumber(lua, (ImU32)ImColor(255, 255, 255));
			}

			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int LUDtoINT(lua_State* lua)
	{
		try
		{
			lua_pushnumber(lua, (uintptr_t)lua_touserdata(lua, -1));
			return 1;
		} catch(...) {}
		return 0;
	}
};

#pragma endregion // LuaFuncs->global


#pragma region menu

#define MENU_LUA_FEATURES_COUNT 104
struct LuaMenu
{
	struct Feature
	{
		enum Type : byte
		{
			Type_Float,
			Type_Int,
			Type_CString,
			Type_Bool
		};

		static size_t CreateHash(const char* name, const char* tab)
		{
			return std::hash<std::string>{}(name + std::string(tab));
		}

		__forceinline static Feature* SearchForHash(size_t hash, const Feature* features, USHORT count)
		{
			for (USHORT i = 0; i < count; i++)
			{
				if (hash == features[i].hash)
					return (Feature*)&features[i];
			}
			return nullptr;
		}

		Feature() {}
		Feature(const char* tab, const char* name, void* value, byte type) :
			 hash(CreateHash(name, tab)), value(value), type(type) {}
		~Feature() {}

		size_t hash = 0;
		byte type = 0;
		void* value = nullptr;
	};

	static const Feature features[MENU_LUA_FEATURES_COUNT];

	static int GetFeature(lua_State* lua)
	{
		try
		{
			auto feature = Feature::SearchForHash(Feature::CreateHash(lua_tostring(lua, -1), lua_tostring(lua, -2)), features, 94);

			if (feature)
			{
				switch (feature->type)
				{
				case Feature::Type_Float:
					lua_pushnumber(lua, *(float*)feature->value);
					break;
				case Feature::Type_Int:
					lua_pushnumber(lua, *(int*)feature->value);
					break;
				case Feature::Type_Bool:
					lua_pushboolean(lua, *(bool*)feature->value);
					break;
				case Feature::Type_CString:
					lua_pushstring(lua, (char*)feature->value);
					break;
				}
				return 1;
			}
		}
		catch (...) {}
		return 0;
	}

	static int SetFeature(lua_State* lua)
	{
		try
		{
			auto feature = Feature::SearchForHash(Feature::CreateHash(lua_tostring(lua, 2), lua_tostring(lua, 1)), features, MENU_LUA_FEATURES_COUNT);

			if (feature)
			{
				switch (feature->type)
				{
				case Feature::Type_Float:
					*(float*)feature->value = lua_tofloat(3);
					break;
				case Feature::Type_Int:
					*(int*)feature->value = lua_tonumber(lua, 3);
					break;
				case Feature::Type_Bool:
					*(bool*)feature->value = lua_toboolean(lua, 3);
					break;
				case Feature::Type_CString:
					strcpy_s((char*)feature->value, lua_tonumber(lua, 3), lua_tostring(lua, 4));
					break;
				}
				return 1;
			}
		}
		catch (...) {}
		return 0;
	}
};
inline const LuaMenu::Feature LuaMenu::features[MENU_LUA_FEATURES_COUNT] = {
	{ XorCStr("Main"), XorCStr("Flyhack"), &cfg.player.bFlyhack, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Fly Speed"), &cfg.player.bNoClip, Feature::Type_Float },
	{ XorCStr("Main"), XorCStr("No Clip"), &cfg.player.bNoClip, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Speedhack"), &cfg.player.bSpeedHack, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Speed"), &cfg.player.Speed, Feature::Type_Float },
	{ XorCStr("Main"), XorCStr("Super Jump"), &cfg.player.bSuperJump, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Jump Force"), &cfg.player.jumpForce, Feature::Type_Float },
	{ XorCStr("Main"), XorCStr("Infinite Jump"), &cfg.player.bInfiniteJump, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Bunnyhop"), &cfg.player.bBunnyhop, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Anti Punch"), &cfg.player.bAntiPunch, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Anti Knockback"), &cfg.player.bAntiPunch, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Anti Force"), &cfg.player.jumpForce, Feature::Type_Float },
	{ XorCStr("Main"), XorCStr("Anti W/L"), &cfg.player.bAntiWater, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Anti Void"), &cfg.player.bAntiVoid, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("No Bound Kill"), &cfg.player.bNoBoundKill, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("High Tire Jump"), &cfg.player.bSuperTireJump, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Bounce Force"), &cfg.player.tireForce, Feature::Type_Float },
	{ XorCStr("Main"), XorCStr("Super Slide"), &cfg.player.bSuperSlide, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Slide Force"), &cfg.player.slideForce, Feature::Type_Float },
	{ XorCStr("Main"), XorCStr("Infinite Slide"), &cfg.player.bInfiniteSlide, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("No Ice Slide"), &cfg.player.bNoIceSlide, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Mod Gravity"), &cfg.player.bModGravity, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Gravity"), &cfg.player.gravity, Feature::Type_Float },
	{ XorCStr("Main"), XorCStr("Godmode"), &cfg.player.bGodMode, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("RLGL Play Bot"), &cfg.player.bRLGLBot, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Commands"), &cfg.player.bCommands, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Cmd Symbol"), cfg.player.commandSymbol, Feature::Type_CString },
	{ XorCStr("Main"), XorCStr("Player Teleport"), &cfg.player.bPlayerTeleport, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Tp Distance"), &cfg.player.playerTpDist, Feature::Type_Float },
	{ XorCStr("Main"), XorCStr("RLGL Godmode"), &cfg.player.bRedLightGodmode, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("No Glass Break"), &cfg.player.bNoGlassBreak, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Suicide"), &cfg.player.bSuicide, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Break Glass"), &cfg.player.bBreakGlass, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Invisibility"), &cfg.player.bInvisibility, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("TD Teleport"), &cfg.player.bTileDriveTp, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("HBSK Teleport"), &cfg.player.bHbsTp, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Auto Tp"), &cfg.player.bAutoHBStp, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Mouse Teleport"), &cfg.player.bMouseTeleport, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Clear Chat"), &cfg.player.bClearChat, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Message"), cfg.player.message, Feature::Type_CString },
	{ XorCStr("Main"), XorCStr("Amount"), &cfg.player.messageAmount, Feature::Type_Int },
	{ XorCStr("Main"), XorCStr("Send"), &cfg.player.messageCounter, Feature::Type_Int },
	{ XorCStr("Main"), XorCStr("Start Game"), &cfg.player.bStartGame, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Next Round"), &cfg.player.bNextGame, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Next Map"), &cfg.player.nextMap, Feature::Type_Int },
	{ XorCStr("Main"), XorCStr("Next Gamemode"), &cfg.player.nextGameMode, Feature::Type_Int },
	{ XorCStr("Main"), XorCStr("Anti Tag"), &cfg.player.bAntiTag, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Send Players 2 Space"), &cfg.player.bSend2Space, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Freeze Timer"), &cfg.player.bFreezeTimer, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Respawn"), &cfg.player.bRespawn, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Freeze Player(s)"), &cfg.player.bFreezePlayers, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Frozen Player"), &cfg.player.frozenPlayer, Feature::Type_Int },
	{ XorCStr("Main"), XorCStr("Kill"), &cfg.player.bKillPlayers, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Player To Kill"), &cfg.player.playerToKill, Feature::Type_Int },
	{ XorCStr("Main"), XorCStr("Kill Button Presser"), &cfg.player.bKillCurrentButtonPresser, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Save Button Presser"), &cfg.player.bSaveCurrentButtonPresser, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Troll Button Screen"), &cfg.player.bTrollButtonScreen, Feature::Type_Bool },
	{ XorCStr("Main"), XorCStr("Only Rigged Buttons"), &cfg.player.bOnlyRiggedButtons, Feature::Type_Bool },


	{ XorCStr("Visuals"), XorCStr("Player ESP"), &cfg.visuals.vp.bESP, Feature::Type_Bool },
	{ XorCStr("Visuals"), XorCStr("Chams"), &cfg.visuals.vp.bChams, Feature::Type_Bool },
	{ XorCStr("Visuals"), XorCStr("Boxes"), &cfg.visuals.vp.bBoxes, Feature::Type_Bool },
	{ XorCStr("Visuals"), XorCStr("Snaplines"), &cfg.visuals.vp.bSnaplines, Feature::Type_Bool },
	{ XorCStr("Visuals"), XorCStr("Name ESP"), &cfg.visuals.vp.bNameESP, Feature::Type_Bool },
	{ XorCStr("Visuals"), XorCStr("RGB Effect Speed"), &cfg.visuals.vp.RGBSpeed, Feature::Type_Float },
	{ XorCStr("Visuals"), XorCStr("Limit Distance"), &cfg.visuals.vp.bLimitDistance, Feature::Type_Bool },
	{ XorCStr("Visuals"), XorCStr("Enemies Only"), &cfg.visuals.vp.bEnemiesOnly, Feature::Type_Bool },
	{ XorCStr("Visuals"), XorCStr("Weapon ESP"), &cfg.visuals.bWeaponESP, Feature::Type_Bool },
	{ XorCStr("Visuals"), XorCStr("RGB Effect"), &cfg.visuals.bWeaponRGB, Feature::Type_Bool },
	{ XorCStr("Visuals"), XorCStr("Glass ESP"), &cfg.visuals.GlassESP, Feature::Type_Bool },
	{ XorCStr("Visuals"), XorCStr("Bustling Buttons ESP"), &cfg.visuals.ButtonESP, Feature::Type_Bool },
	{ XorCStr("Visuals"), XorCStr("FOV"), &cfg.visuals.bFOV, Feature::Type_Bool },
	{ XorCStr("Visuals"), XorCStr("FOVvalue"), &cfg.visuals.FOV, Feature::Type_Int },
	{ XorCStr("Visuals"), XorCStr("Dorm Full Bright"), &cfg.visuals.bDormFullBright, Feature::Type_Bool },
	{ XorCStr("Visuals"), XorCStr("Bomb Timer"), &cfg.visuals.bBombTimer, Feature::Type_Bool },
	{ XorCStr("Visuals"), XorCStr("No Camera Shake"), &cfg.player.bNoCameraShake, Feature::Type_Bool },
	{ XorCStr("Visuals"), XorCStr("Zoom"), &cfg.visuals.bZoom, Feature::Type_Bool },
	{ XorCStr("Visuals"), XorCStr("Zoom FOV"), &cfg.visuals.bZoom, Feature::Type_Float },

	{ XorCStr("Combat"), XorCStr("Aim Assist"), &cfg.aim.bAim, Feature::Type_Bool },
	{ XorCStr("Combat"), XorCStr("Hold to Aim"), &cfg.aim.bAimKey, Feature::Type_Bool },
	{ XorCStr("Combat"), XorCStr("Smoothness"), &cfg.aim.Smoothness, Feature::Type_Float },
	{ XorCStr("Combat"), XorCStr("FOV"), &cfg.aim.bFOV, Feature::Type_Bool },
	{ XorCStr("Combat"), XorCStr("FOV Circle"), &cfg.aim.bFOVCircle, Feature::Type_Bool },
	{ XorCStr("Combat"), XorCStr("Triggerbot"), &cfg.aim.trg.bTrg, Feature::Type_Bool },
	{ XorCStr("Combat"), XorCStr("Hold to Run"), &cfg.aim.trg.bTrgKey, Feature::Type_Bool },
	{ XorCStr("Combat"), XorCStr("Limit Target Distance"), &cfg.aim.trg.bLimitDistance, Feature::Type_Bool },
	{ XorCStr("Combat"), XorCStr("Distance"), &cfg.aim.trg.maxDistance, Feature::Type_Float },
	{ XorCStr("Combat"), XorCStr("No Recoil"), &cfg.aim.bNoRecoil, Feature::Type_Bool },
	{ XorCStr("Combat"), XorCStr("Recoil Control"), &cfg.aim.recoilSlider, Feature::Type_Float },
	{ XorCStr("Combat"), XorCStr("Hold to Shoot"), &cfg.aim.bHoldToShoot, Feature::Type_Float },
	{ XorCStr("Combat"), XorCStr("Infinite Ammo"), &cfg.player.bInfiniteAmmo, Feature::Type_Bool },
	{ XorCStr("Combat"), XorCStr("Super Punch"), &cfg.player.bSuperPunch, Feature::Type_Bool },
	{ XorCStr("Combat"), XorCStr("Punch Multiplier"), &cfg.player.punchMultiplier, Feature::Type_Float },
	{ XorCStr("Combat"), XorCStr("Far Reach"), &cfg.player.bFarReach, Feature::Type_Bool },
	{ XorCStr("Combat"), XorCStr("SpwanWeapon"), &cfg.player.weaponId, Feature::Type_Int },
	{ XorCStr("Combat"), XorCStr("Spawn"), &cfg.player.ForceGiveWeapons, Feature::Type_Bool },
	{ XorCStr("Combat"), XorCStr("Spinbot"), &cfg.aim.bSpinBot, Feature::Type_Bool },
	{ XorCStr("Combat"), XorCStr("Spin Speed"), &cfg.aim.spinBotSpeed, Feature::Type_Float },
	{ XorCStr("Combat"), XorCStr("Silent Aim"), &cfg.aim.bSilentAim, Feature::Type_Bool },
	{ XorCStr("Combat"), XorCStr("Rapid Fire"), &cfg.aim.bRapidFire, Feature::Type_Bool },

	{ XorCStr("Menu"), XorCStr("RGB Effect"), &cfg.bMenuRGB, Feature::Type_Bool },
	{ XorCStr("Menu"), XorCStr("RGB Effect Speed"), &cfg.MenuRGBSpeed, Feature::Type_Float },
	{ XorCStr("Menu"), XorCStr("Menu Rouding"), &cfg.MenuRounding, Feature::Type_Float },
	{ XorCStr("Menu"), XorCStr("Auto Save"), &cfg.bAutoSave, Feature::Type_Bool },
	{ XorCStr("Menu"), XorCStr("Free Mouse on Popup"), &cfg.bFreeMouseOnPopup, Feature::Type_Bool },
};

#undef MENU_LUA_FEATURES_COUNT

#pragma endregion // LuaFuncs->menu


#pragma region GUI

struct LuaGUI
{
	static int MessageBoxA_(lua_State* lua)
	{
		try
		{
			int numArgs = lua_gettop(lua);
			if (numArgs == 3)
			{
				lua_pushnumber(lua, MessageBoxA(0, luaL_checkstring(lua, -3), luaL_checkstring(lua, -2), luaL_checknumber(lua, -1)));
			}
			else if (numArgs == 2)
			{
				lua_pushnumber(lua, MessageBoxA(0, luaL_checkstring(lua, -2), luaL_checkstring(lua, -1), 0));
			}
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int MessageBoxW_(lua_State* lua)
	{
		try
		{
			int numArgs = lua_gettop(lua);
			if (numArgs == 3)
			{
				std::string Smsg = luaL_checkstring(lua, -3);
				std::wstring Wmsg = std::wstring(Smsg.begin(), Smsg.end());

				std::string Stitle = luaL_checkstring(lua, -2);
				std::wstring Wtitle = std::wstring(Stitle.begin(), Stitle.end());

				lua_pushnumber(lua, MessageBoxW(0, Wmsg.c_str(), Wtitle.c_str(), luaL_checknumber(lua, -1)));
			}
			else if (numArgs == 2)
			{
				std::string Smsg = luaL_checkstring(lua, -2);
				std::wstring Wmsg = std::wstring(Smsg.begin(), Smsg.end());

				std::string Stitle = luaL_checkstring(lua, -1);
				std::wstring Wtitle = std::wstring(Stitle.begin(), Stitle.end());

				lua_pushnumber(lua, MessageBoxW(0, Wmsg.c_str(), Wtitle.c_str(), 0));
			}
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int MessageBoxExA_(lua_State* lua)
	{
		try
		{
			int numArgs = lua_gettop(lua);
			if (numArgs == 3)
			{
				Menu::MBCallback cl = {
					luaL_checkstring(lua, -3),
					luaL_checkstring(lua, -2),
					(DWORD)luaL_checknumber(lua, -1)
				};
			}
			else if (numArgs == 2)
			{
				Menu::MBCallback cl = {
					luaL_checkstring(lua, -2),
					luaL_checkstring(lua, -1),
					0
				};
			}
		}
		catch (...) {}
		return 0;
	}

	static int Checkbox(lua_State* lua)
	{
		try
		{
			bool checked = lua_toboolean(lua, -1);
			ImGui::Checkbox(luaL_checkstring(lua, -2), &checked);
			lua_pushboolean(lua, checked);
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int Button(lua_State* lua)
	{
		try
		{
			lua_pushboolean(lua, ImGui::Button(luaL_checkstring(lua, -1)));
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int Hotkey(lua_State* lua)
	{
		try
		{
			bool bGetKey = lua_toboolean(lua, -1);
			USHORT key = lua_tonumber(lua, -4);
			ImGui_HotkeyButton(key, lua_tostring(lua, -3), lua_tostring(lua, -2), bGetKey);
			lua_pushnumber(lua, key);
			lua_pushboolean(lua, bGetKey);
			return 2;
		}
		catch (...) {}
		return 0;
	}

	static int Text(lua_State* lua)
	{
		try
		{
			ImGui::Text(lua_tostring(lua, -1));
		}
		catch (...) {}
		return 0;
	}

	static int SliderFloat(lua_State* lua)
	{
		try
		{
			int numArgs = lua_gettop(lua);
			if (numArgs == 4)
			{
				float value = lua_tofloat(-3);
				ImGui::SliderFloat(lua_tostring(lua, -4), &value, lua_tofloat(-2), lua_tofloat(-1));
				lua_pushnumber(lua, value);
			}
			else if (numArgs == 5)
			{
				float value = lua_tofloat(-4);
				ImGui::SliderFloat(lua_tostring(lua, -5), &value, lua_tofloat(-3), lua_tofloat(-2), lua_tostring(lua, -1));
				lua_pushnumber(lua, value);
			}
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int SliderInt(lua_State* lua)
	{
		try
		{
			int numArgs = lua_gettop(lua);
			if (numArgs == 4)
			{
				int value = lua_tonumber(lua, -3);
				ImGui::SliderInt(lua_tostring(lua, -4), &value, lua_tonumber(lua, -2), lua_tonumber(lua, -1));
				lua_pushnumber(lua, value);
			}
			else if (numArgs == 5)
			{
				int value = lua_tonumber(lua, -4);
				ImGui::SliderInt(lua_tostring(lua, -5), &value, lua_tonumber(lua, -3), lua_tonumber(lua, -2), lua_tostring(lua, -1));
				lua_pushnumber(lua, value);
			}
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int InputFloat(lua_State* lua)
	{
		try
		{
			int numArgs = lua_gettop(lua);
			if (numArgs == 3)
			{
				float value = lua_tofloat(-2);
				ImGui::InputFloat(lua_tostring(lua, -3), &value, lua_tofloat(-1), 0, XorCStr("%.2f"), 0, true);
				lua_pushnumber(lua, value);
			}
			else if (numArgs == 4)
			{
				float value = lua_tofloat(-3);
				ImGui::InputFloat(lua_tostring(lua, -4), &value, lua_tofloat(-2), lua_tofloat(-1), XorCStr("%.2f"), 0, true);
				lua_pushnumber(lua, value);
			}
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int InputInt(lua_State* lua)
	{
		try
		{
			int numArgs = lua_gettop(lua);
			if (numArgs == 3)
			{
				int value = lua_tonumber(lua, -2);
				ImGui::InputInt(lua_tostring(lua, -3), &value, lua_tonumber(lua, -1), 100, 0, true);
				lua_pushnumber(lua, value);
			}
			else if (numArgs == 4)
			{
				int value = lua_tonumber(lua, -3);
				ImGui::InputInt(lua_tostring(lua, -4), &value, lua_tonumber(lua, -2), lua_tonumber(lua, -1), 0, true);
				lua_pushnumber(lua, value);
			}
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int InputText(lua_State* lua)
	{
		try
		{
			int numArgs = lua_gettop(lua);
			if (numArgs == 2)
			{
				char buf[50];
				strcpy_s(buf, lua_tostring(lua, -1));
				ImGui::InputText(lua_tostring(lua, -2), buf, sizeof(buf));
				lua_pushstring(lua, buf);
			}
			else if (numArgs == 3)
			{
				int size = lua_tonumber(lua, -1);
				if (size < 0) return 0;

				char* buf = nullptr;
				try
				{
					buf = new char[size] { 0 };
					if (!buf) return 0;

					const char* currentInput = lua_tostring(lua, -2);
					int len = strlen(currentInput);
					for (USHORT i = 0; i < len; i++)
					{
						if (i >= size) break;
						buf[i] = currentInput[i];
					}

					ImGui::InputText(lua_tostring(lua, -3), buf, size);

					lua_pushstring(lua, buf);
					delete[] buf;
				}
				catch (...) { if (buf) delete[] buf; return 0; }
			}

			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int Combo(lua_State* lua)
	{
		try
		{
			int buf = lua_tonumber(lua, -2);
			const char* str = lua_tostring(lua, -3);

			std::vector<std::string> items;
			lua_gettable(lua, -1);
			lua_pushnil(lua);
			while (lua_next(lua, -2))
			{
				items.push_back((char*)lua_tostring(lua, -1));
				lua_pop(lua, 1);
			}

			ImGui::Combo(str, &buf, items);
			lua_pushnumber(lua, buf);
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int Spacing(lua_State* lua)
	{
		ImGui::Spacing();
		return 0;
	}

	static int Separator(lua_State* lua)
	{
		ImGui::Separator();
		return 0;
	}

	static int SameLine(lua_State* lua)
	{
		try
		{
			int numArgs = lua_gettop(lua);
			if (numArgs == 0)
				ImGui::SameLine();
			else if (numArgs == 1)
				ImGui::SameLine(luaL_checknumber(lua, -1));
			else if (numArgs == 2)
				ImGui::SameLine(0, luaL_checknumber(lua, -1));
		}
		catch (...) {}
		return 0;
	}

	static int SetNextItemWidth(lua_State* lua)
	{
		try
		{
			ImGui::SetNextItemWidth(luaL_checknumber(lua, -1));
		}
		catch (...) {}
		return 0;
	}
};

#pragma endregion // LuaFuncs->GUI


#pragma region input

struct LuaInput
{
	static int IsKeyDown(lua_State* lua)
	{
		try
		{
			if (Cheat::bTyping || Cheat::activeHNWD != Client::hwnd)
			{
				lua_pushboolean(lua, false);
				return 1;
			}
			lua_pushboolean(lua, int(GetAsyncKeyState(luaL_checknumber(lua, -1)) & 0x8000));
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int IsKeyPressed(lua_State* lua)
	{
		try
		{
			if (Cheat::bTyping || Cheat::activeHNWD != Client::hwnd)
			{
				lua_pushboolean(lua, false);
				return 1;
			}
			USHORT state = GetAsyncKeyState(luaL_checknumber(lua, -1));
			lua_pushboolean(lua, int((state & 1) && (state & 0x8000)));
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int GetMousePos(lua_State* lua)
	{
		try
		{
			POINT ret;
			GetCursorPos(&ret);
			lua_pushnumber(lua, ret.x);
			lua_pushnumber(lua, ret.y);
			return 2;
		}
		catch (...) {}
		return 0;
	}

	static int SetMousePos(lua_State* lua)
	{
		try
		{
			SetCursorPos(lua_tonumber(lua, -2), lua_tonumber(lua, -1));
		}
		catch (...) {}
		return 0;
	}

	static int MouseEvent(lua_State* lua)
	{
		try
		{
			mouse_event(lua_tonumber(lua, -3), lua_tonumber(lua, -2), lua_tonumber(lua, -1), 0, 0);
		}
		catch (...) {}
		return 0;
	}

	static int SetKey(lua_State* lua)
	{
		try
		{
			INPUT ip = { INPUT_KEYBOARD };
			ip.ki.dwFlags = lua_tonumber(lua, 2);
			ip.ki.wVk = lua_tonumber(lua, 1);

			if (lua_gettop(lua) == 3)
				ip.ki.time = lua_tonumber(lua, -1);
			SendInput(1, &ip, sizeof(INPUT));
		}
		catch (...) {}
		return 0;
	}
};

#pragma endregion // LuaFuncs->input


#pragma region drawing

struct LuaDrawing
{
	static int AddLine(lua_State* lua)
	{
		try
		{
			int numArgs = lua_gettop(lua);
			if (numArgs == 4)
			{
				ImGui::GetBackgroundDrawList()->AddLine(
					lua_tovec2(-3),
					lua_tovec2(-1),
					ImColor(255, 255, 255));
			}
			else if (numArgs == 5)
			{
				ImGui::GetBackgroundDrawList()->AddLine(
					lua_tovec2(-4),
					lua_tovec2(-2),
					lua_tonumber(lua, -1));
			}
			else if (numArgs == 6)
			{
				ImGui::GetBackgroundDrawList()->AddLine(
					lua_tovec2(-5),
					lua_tovec2(-3),
					lua_tonumber(lua, -2),
					lua_tofloat(-1));
			}
		}
		catch (...) {}
		return 0;
	}

	static int AddRect(lua_State* lua)
	{
		try
		{
			int numArgs = lua_gettop(lua);
			if (numArgs == 4)
			{
				ImGui::GetBackgroundDrawList()->AddRect(
					lua_tovec2(-3),
					lua_tovec2(-1),
					ImColor(255, 255, 255));
			}
			else if (numArgs == 5)
			{
				ImGui::GetBackgroundDrawList()->AddRect(
					lua_tovec2(-4),
					lua_tovec2(-2),
					lua_tonumber(lua, -1));
			}
			else if (numArgs == 6)
			{
				ImGui::GetBackgroundDrawList()->AddRect(
					lua_tovec2(-5),
					lua_tovec2(-3),
					lua_tonumber(lua, -2),
					0,
					0,
					lua_tofloat(-1));
			}
		}
		catch (...) {}
		return 0;
	}

	static int AddFilledRect(lua_State* lua)
	{
		try
		{
			int numArgs = lua_gettop(lua);
			if (numArgs == 4)
			{
				ImGui::GetBackgroundDrawList()->AddRectFilled(
					lua_tovec2(-3),
					lua_tovec2(-1),
					ImColor(255, 255, 255));
			}
			else if (numArgs == 5)
			{
				ImGui::GetBackgroundDrawList()->AddRectFilled(
					lua_tovec2(-4),
					lua_tovec2(-2),
					lua_tonumber(lua, -1));
			}
		}
		catch (...) {}
		return 0;
	}

	static int AddCircle(lua_State* lua)
	{
		try
		{
			int numArgs = lua_gettop(lua);
			if (numArgs == 5)
			{
				ImGui::GetBackgroundDrawList()->AddCircle(
					lua_tovec2(-4),
					lua_tofloat(-3),
					ImColor(255, 255, 255),
					lua_tonumber(lua, -2),
					lua_tonumber(lua, -1));
			}
			else if (numArgs == 6)
			{
				ImGui::GetBackgroundDrawList()->AddCircle(
					lua_tovec2(-5),
					lua_tofloat(-4),
					lua_tonumber(lua, -1),
					lua_tonumber(lua, -3),
					lua_tonumber(lua, -2));
			}
		}
		catch (...) {}
		return 0;
	}

	static int AddFilledCircle(lua_State* lua)
	{
		try
		{
			int numArgs = lua_gettop(lua);
			if (numArgs == 4)
			{
				ImGui::GetBackgroundDrawList()->AddCircleFilled(
					lua_tovec2(-3),
					lua_tofloat(-2),
					ImColor(255, 255, 255),
					lua_tonumber(lua, -1));
			}
			else if (numArgs == 5)
			{
				ImGui::GetBackgroundDrawList()->AddCircleFilled(
					lua_tovec2(-4),
					lua_tofloat(-3),
					lua_tonumber(lua, -1),
					lua_tonumber(lua, -2));
			}
		}
		catch (...) {}
		return 0;
	}

	static int AddText(lua_State* lua)
	{
		try
		{
			int numArgs = lua_gettop(lua);
			if (numArgs == 3)
				ImGui::GetBackgroundDrawList()->AddText(lua_tovec2(-2), ImColor(255, 255, 255), lua_tostring(lua, -1));
			else if (numArgs == 4)
				ImGui::GetBackgroundDrawList()->AddText(lua_tovec2(-3), lua_tonumber(lua, -1), lua_tostring(lua, -2));
		}
		catch (...) {}
		return 0;
	}
};

#pragma endregion // LuaFuncs->drawing


#pragma region game

#include "../../utils/cgdata.h"
using namespace UnityEngine;


struct LuaOnlinePlayerMovement
{
	static int ToMB(lua_State* lua)
	{
		try
		{
			lua_pushlightuserdata(lua, lua_touserdata(lua, -1));
			lua_set_class(UMonoBehaviour);
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int GetPos(lua_State* lua)
	{
		try
		{
			MonoBehaviour* player = (MonoBehaviour*)lua_touserdata(lua, -1);
			if (!player) return 0;

			vec3 pos = player->get_transform()->get_position();
			lua_pushnumber(lua, pos.x);
			lua_pushnumber(lua, pos.y);
			lua_pushnumber(lua, pos.z);
			return 3;
		} catch(...) {}
		return 0;
	}

	static int GetBonePos(lua_State* lua)
	{
		try
		{
			MonoBehaviour* player = (MonoBehaviour*)lua_touserdata(lua, -1);
			if (!player) return 0;

			vec3 pos = player->GetComponentsInChildren<Transform>(XorStr("UnityEngine.Transform, UnityEngine.CoreModule"))->
				items[(int)lua_tonumber(lua, -1)]->get_position();

			lua_pushnumber(lua, pos.x);
			lua_pushnumber(lua, pos.y);
			lua_pushnumber(lua, pos.z);
			return 3;
		}
		catch (...) {}
		return 0;
	}

	static int GetRotation(lua_State* lua)
	{
		try
		{
			uintptr_t player = (uintptr_t)lua_touserdata(lua, -1);
			if (!player) return 0;

			vec2 pos = *(vec2*)(player + off::OnlinePlayerMovement::yRot - sizeof(float));
			lua_pushnumber(lua, pos.x);
			lua_pushnumber(lua, pos.y);
			return 2;
		}
		catch (...) {}
		return 0;
	}

	static int GetBoneMatrix(lua_State* lua)
	{
		try
		{
			MonoBehaviour* player = (MonoBehaviour*)lua_touserdata(lua, 1);
			if (!player) return 0;
			auto bones = player->GetComponentsInChildren<Transform>(XorCStr("UnityEngine.Transform, UnityEngine.CoreModule"));
			if (!bones) return 0;

			lua_createtable(lua, bones->length, 0);
			int newTable = lua_gettop(lua);
			for (USHORT i = 0; i < bones->length; i++)
			{
				lua_pushlightuserdata(lua, bones->items[i]);
				lua_set_class(UTransform);
				lua_rawseti(lua, newTable, i + 1);
			}

			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int GetPlayerId(lua_State* lua)
	{
		try
		{
			lua_pushnumber(lua, (uintptr_t)Cheat::getPlayerId((Component*)lua_touserdata(lua, 1)));
			return 1;
		} catch(...) {}
		return 0;
	}

	static int GetPlayerNum(lua_State* lua)
	{
		try
		{
			lua_pushnumber(lua, (int)Cheat::getPlayerNum((Component*)lua_touserdata(lua, 1)));
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int SetPlayerPos(lua_State* lua)
	{
		try
		{
			Cheat::setPlayerPos(Cheat::getPlayerId((Component*)lua_touserdata(lua, 1)), lua_tovec3(-1));
		} catch(...) {}
		return 0;
	}
};

struct LuaTransform
{
	static int GetPos(lua_State* lua)
	{
		try
		{
			vec3 pos = get(lua)->get_position();
			lua_pushnumber(lua, pos.x);
			lua_pushnumber(lua, pos.y);
			lua_pushnumber(lua, pos.z);
			return 3;
		} catch(...) {}
		return 0;
	}

	static int SetPos(lua_State* lua)
	{
		try
		{
			get(lua, 1)->set_position(lua_tovec3(-1));
		}
		catch (...) {}
		return 0;
	}

	static int GetRot(lua_State* lua)
	{
		try
		{
			vec3 rot = get(lua)->get_rotation().get_eulerAngles();
			lua_pushnumber(lua, rot.x);
			lua_pushnumber(lua, rot.y);
			lua_pushnumber(lua, rot.z);
			return 3;
		}
		catch (...) {}
		return 0;
	}

	static int SetRot(lua_State* lua)
	{
		try
		{
			get(lua, -2)->set_rotation(Quaternion::Euler(lua_tovec3(-1)));
		}
		catch (...) {}
		return 0;
	}

	static int GetGameObject(lua_State* lua)
	{
		try
		{
			lua_pushlightuserdata(lua, get(lua)->get_gameObject());
			return 1;
		}
		catch (...) {}
		return 0;
	}

private:
	static __forceinline Transform* get(lua_State* lua, int index = -1) { return (Transform*)lua_touserdata(lua, index); }
};

struct LuaRigidbody
{
	static int GetVelocity(lua_State* lua)
	{
		try
		{
			vec3 vel = get(lua)->get_velocity();
			lua_pushnumber(lua, vel.x);
			lua_pushnumber(lua, vel.y);
			lua_pushnumber(lua, vel.z);
			return 3;
		}
		catch (...) {}
		return 0;
	}

	static int SetVelocity(lua_State* lua)
	{
		try
		{
			get(lua, 1)->set_velocity(lua_tovec3(-1));
		}
		catch (...) {}
		return 0;
	}

	static int GetAngularVelocity(lua_State* lua)
	{
		try
		{
			vec3 vel = get(lua)->get_angularVelocity();
			lua_pushnumber(lua, vel.x);
			lua_pushnumber(lua, vel.y);
			lua_pushnumber(lua, vel.z);
			return 3;
		}
		catch (...) {}
		return 0;
	}

	static int SetAngularVelocity(lua_State* lua)
	{
		try
		{
			get(lua, 1)->set_angularVelocity(lua_tovec3(-1));
		}
		catch (...) {}
		return 0;
	}

	static int GetDrag(lua_State* lua)
	{
		try
		{
			lua_pushnumber(lua, get(lua)->get_drag());
			return 1;
		}
		catch (...) {}
		return 0;
	}
	
	static int SetDrag(lua_State* lua)
	{
		try
		{
			get(lua, 1)->set_drag(lua_tofloat(-1));
		}
		catch (...) {}
		return 0;
	}

	static int SetAngularDrag(lua_State* lua)
	{
		try
		{
			get(lua, 1)->set_angularDrag(lua_tofloat(-1));
		}
		catch (...) {}
		return 0;
	}

	static int SetMass(lua_State* lua)
	{
		try
		{
			get(lua, 1)->set_mass(lua_tofloat(-1));
		}
		catch (...) {}
		return 0;
	}

	static int SetUseGravity(lua_State* lua)
	{
		try
		{
			get(lua, 1)->set_useGravity(lua_toboolean(lua, -1));
		}
		catch (...) {}
		return 0;
	}

	static int GetIsKinematic(lua_State* lua)
	{
		try
		{
			lua_pushboolean(lua, get(lua)->get_isKinematic());
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int SetIsKinematic(lua_State* lua)
	{
		try
		{
			get(lua, 1)->set_isKinematic(lua_toboolean(lua, -1));
		}
		catch (...) {}
		return 0;
	}

	static int GetPosition(lua_State* lua)
	{
		try
		{
			vec3 pos = get(lua)->get_position();
			lua_pushnumber(lua, pos.x);
			lua_pushnumber(lua, pos.y);
			lua_pushnumber(lua, pos.z);
			return 3;
		}
		catch (...) {}
		return 0;
	}

	static int SetPosition(lua_State* lua)
	{
		try
		{
			get(lua, 1)->set_position(lua_tovec3(-1));
		}
		catch (...) {}
		return 0;
	}

	static int GetRot(lua_State* lua)
	{
		try
		{
			vec3 rot = get(lua)->get_rotation().get_eulerAngles();
			lua_pushnumber(lua, rot.x);
			lua_pushnumber(lua, rot.y);
			lua_pushnumber(lua, rot.z);
			return 3;
		}
		catch (...) {}
		return 0;
	}

	static int SetRot(lua_State* lua)
	{
		try
		{
			get(lua, 1)->set_rotation(Quaternion::Euler(lua_tovec3(-1)));
		}
		catch (...) {}
		return 0;
	}

	static int AddForce(lua_State* lua)
	{
		try
		{
			get(lua)->AddForce(lua_tovec3(-1));
		}
		catch (...) {}
		return 0;
	}

private:
	static __forceinline Rigidbody* get(lua_State* lua, int index = -1) { return (Rigidbody*)lua_touserdata(lua, index); }
};

struct LuaMonoBehaviour
{
	static int GetTrm(lua_State* lua)
	{
		try
		{
			Transform* tr = get(lua)->get_transform();
			if (!tr) return 0;
			lua_pushlightuserdata(lua, tr);
			lua_set_class(UTransform);
			return 1;
		} catch(...) {}
		return 0;
	}

	static int GetGameObject(lua_State* lua)
	{
		try
		{
			lua_pushlightuserdata(lua, get(lua)->get_gameObject());
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int GetPos(lua_State* lua)
	{
		try
		{
			Transform* tr = get(lua)->get_transform();
			if (!tr) return 0;
			
			vec3 pos = tr->get_position();
			lua_pushnumber(lua, pos.x);
			lua_pushnumber(lua, pos.y);
			lua_pushnumber(lua, pos.z);
			return 3;
		} catch(...) {}
		return 0;
	}

private:
	static __forceinline MonoBehaviour* get(lua_State* lua, int index = -1) { return (MonoBehaviour*)lua_touserdata(lua, index); }
};

struct LuaGame
{
	static int WorldToScreen(lua_State* lua)
	{
		try
		{
			vec3 ret = {};
			if (Cheat::camera)
				ret = Cheat::camera->WorldToScreenPoint(lua_tovec3(-1));
			lua_pushnumber(lua, ret.x);
			lua_pushnumber(lua, Client::WindowSize.y - ret.y);
			lua_pushboolean(lua, ret.z > 0.3f);
			return 3;
		}
		catch (...) {}
		return 0;
	}

	static int GetAlivePlayers(lua_State* lua)
	{
		try
		{
			auto& players = Cheat::activePlayers;
			if (!players) return 0;

			lua_createtable(lua, players->length, 0);
			int newTable = lua_gettop(lua);
			for (USHORT i = 0; i < players->length; i++)
			{
				lua_pushlightuserdata(lua, players->items[i]);
				lua_set_class(Player);
				lua_rawseti(lua, newTable, i + 1);
			}

			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int GetObjectsOfType(lua_State* lua)
	{
		try
		{
			generic_array* objects = nullptr;
			int numArgs = lua_gettop(lua);

			if (numArgs == 2)
				objects = il2cpp::find_objects_of_type(lua_tostring(lua, -2), lua_tostring(lua, -1));
			else if (numArgs == 3)
				objects = il2cpp::find_objects_of_type(lua_tostring(lua, -3), lua_tostring(lua, -2), XorCStr(""), lua_toboolean(lua, -1));

			if (!objects) return 0;

			lua_createtable(lua, objects->length, 0);
			int newTable = lua_gettop(lua);
			for (USHORT i = 0; i < objects->length; i++)
			{
				lua_pushlightuserdata(lua, (void*)objects->items[i]);
				lua_set_class(UMonoBehaviour);
				lua_rawseti(lua, newTable, i + 1);
			}

			return 1;
		} catch(...) {}
		return 0;
	}
	
	static int GetObjectOfType(lua_State* lua)
	{
		try
		{
			uintptr_t buf = Object::FindObjectOfType(lua_tostring(lua, 1), lua_tostring(lua, 2), false);

			lua_pushlightuserdata(lua, (void*)buf);
			lua_set_class(UMonoBehaviour);
			return 1;
		} catch(...) {}
		return 0;
	}

	static int GetMethod(lua_State* lua)
	{
		try
		{
			lua_pushnumber(lua, il2cpp::get_method(
					lua_tostring(lua, -6),
					lua_tostring(lua, -5),
					lua_tostring(lua, -4), 
					lua_tostring(lua, -3),
					lua_tostring(lua, -2),
					lua_tostring(lua, -1)));
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int GetClass(lua_State* lua)
	{
		try
		{
			lua_pushnumber(lua, il2cpp::get_class(
				lua_tostring(lua, -3),
				lua_tostring(lua, -2),
				lua_tostring(lua, -1)));
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int Cast(lua_State* lua)
	{
		try
		{
			void* data = lua_touserdata(lua, -2);
			std::string className = lua_tostring(lua, -1);
			className += XorStr("_MT");

			lua_pushlightuserdata(lua, data);
			luaL_getmetatable(lua, className.c_str());
			lua_setmetatable(lua, -2);
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int CallFunc(lua_State* lua)
	{
		try
		{
			std::string params = lua_tostring(lua, 3);
			std::string ret = lua_tostring(lua, 2);
			void* addr = (void*)(uintptr_t)lua_tonumber(lua, 1);

			if (ret == "Void")
			{
				if (params == "32")
				{
					reinterpret_cast<void(*)(int32_t)>(addr)
						((int32_t)lua_tonumber(lua, -1));
				}
				else if (params == "64")
				{
					reinterpret_cast<void(*)(uintptr_t)>(addr)
						((uintptr_t)lua_tonumber(lua, -1));
				}
				else if (params == "6432")
				{
					reinterpret_cast<void(*)(uintptr_t, int32_t)>(addr)
						((uintptr_t)lua_tonumber(lua, -2), (int32_t)lua_tonumber(lua, -1));
				}
				else if (params == "6464")
				{
					reinterpret_cast<void(*)(uintptr_t, uintptr_t)>(addr)
						((uintptr_t)lua_tonumber(lua, -2), (uintptr_t)lua_tonumber(lua, -1));
				}
			}
			else if (ret == "64")
			{
				if (params == "32")
				{
					lua_pushnumber(lua, reinterpret_cast<uintptr_t(*)(int32_t)>(addr)
						(lua_tonumber(lua, -1)));
				}
				else if (params == "64")
				{
					lua_pushnumber(lua, reinterpret_cast<uintptr_t(*)(uintptr_t)>(addr)
						((uintptr_t)lua_tonumber(lua, -1)));
				}
				else if (params == "6432")
				{
					lua_pushnumber(lua, reinterpret_cast<uintptr_t(*)(uintptr_t, int32_t)>(addr)
						((uintptr_t)lua_tonumber(lua, -2), lua_tonumber(lua, -1)));
				}
				else if (params == "6464")
				{
					lua_pushnumber(lua, reinterpret_cast<uintptr_t(*)(uintptr_t, uintptr_t)>(addr)
						((uintptr_t)lua_tonumber(lua, -2), (uintptr_t)lua_tonumber(lua, -1)));
				}

				return 1;
			}
			else if (ret == "32")
			{
				if (params == "32")
				{
					lua_pushnumber(lua, reinterpret_cast<int32_t(*)(uintptr_t)>(addr)
						((uintptr_t)lua_tonumber(lua, -1)));
				}
				else if (params == "64")
				{
					lua_pushnumber(lua, reinterpret_cast<int32_t(*)(uintptr_t)>(addr)
						((uintptr_t)lua_tonumber(lua, -1)));
				}
				else if (params == "6432")
				{
					lua_pushnumber(lua, reinterpret_cast<int32_t(*)(uintptr_t, int32_t)>(addr)
						((uintptr_t)lua_tonumber(lua, -2), lua_tonumber(lua, -1)));
				}
				else if (params == "6464")
				{
					lua_pushnumber(lua, reinterpret_cast<int32_t(*)(uintptr_t, uintptr_t)>(addr)
						((uintptr_t)lua_tonumber(lua, -2), (uintptr_t)lua_tonumber(lua, -1)));
				}

				return 1;
			}

		} catch(...) {}
		return 0;
	}

	static int GetHostId(lua_State* lua)
	{
		try
		{
			lua_pushnumber(lua, (uintptr_t)Cheat::getHostId());
			return 1;
		} catch(...) {}
		return 0;
	}

	static int GetAllPlayers(lua_State* lua)
	{
		try
		{
			auto players = Cheat::getAllPlayers();

			lua_createtable(lua, players.size(), 0);
			int newTable = lua_gettop(lua);
			for (USHORT i = 0; i < players.size(); i++)
			{
				lua_pushlightuserdata(lua, (void*)players[i]);
				lua_set_class(UMonoBehaviour);
				lua_rawseti(lua, newTable, i + 1);
			}

			return 1;
		} catch(...) {}
		return 0;
	}

	static int Raycast(lua_State* lua)
	{
		try
		{
			bool bHit = lua_toboolean(lua, 1);
			USHORT args = lua_gettop(lua);

			if (bHit)
			{
				if (args == 7)
				{
					RaycastHit* hit = (RaycastHit*)lua_newuserdata(lua, sizeof(RaycastHit));
					lua_pushboolean(lua, Physics::Raycast(lua_tovec3(4), lua_tovec3(7), *hit));
					lua_pushlightuserdata(lua, hit);
					return 2;
				}
				else if (args == 8)
				{
					RaycastHit* hit = (RaycastHit*)lua_newuserdata(lua, sizeof(RaycastHit));
					lua_pushboolean(lua, Physics::Raycast(lua_tovec3(4), lua_tovec3(7), *hit, lua_tofloat(8)));
					lua_pushlightuserdata(lua, hit);
					return 2;
				}
				else if (args == 9)
				{
					RaycastHit* hit = (RaycastHit*)lua_newuserdata(lua, sizeof(RaycastHit));
					lua_pushboolean(lua, Physics::Raycast(lua_tovec3(4), lua_tovec3(7), *hit, lua_tofloat(8), int(lua_tonumber(lua, 9))));
					lua_pushlightuserdata(lua, hit);
					return 2;
				}
				return 0;
			}

			if (args == 7)
			{
				lua_pushboolean(lua, Physics::Raycast(lua_tovec3(4), lua_tovec3(7)));
				return 1;
			}
			else if (args == 8)
			{
				lua_pushboolean(lua, Physics::Raycast(lua_tovec3(4), lua_tovec3(7), lua_tofloat(8)));
				return 1;
			}
			else if (args == 9)
			{
				lua_pushboolean(lua, Physics::Raycast(lua_tovec3(4), lua_tovec3(7), lua_tofloat(8), int(lua_tonumber(lua, 9))));
				return 1;
			}

		} catch(...) {}
		return 0;
	}
};

#pragma endregion // LuaFuncs->game


#pragma region LocalPlayer

struct LuaLP
{
	static int AimAtPos(lua_State* lua)
	{
		using namespace Cheat;
		try
		{
			int numArgs = lua_gettop(lua);
			if (numArgs == 3)
				aimAt(lua_tovec3(-1));
			else if (numArgs == 4)
				aimAt(lua_tovec3(-2), lua_tofloat(-1));
		}
		catch (...) {}
		return 0;
	}

	static int SetSpeed(lua_State* lua)
	{
		using namespace Cheat;
		try
		{
			if (!playerMovement)
			{
				lua_pushboolean(lua, false);
				return 1;
			}
			obscured_float val = GetObscuredFloat(lua_tofloat(-1));

			*(obscured_float*)(playerMovement + off::PlayerMovement::moveSpeed) = val;
			*(obscured_float*)(playerMovement + off::PlayerMovement::maxWalkSpeed) = val;
			*(obscured_float*)(playerMovement + off::PlayerMovement::maxRunSpeed) = val;
			*(obscured_float*)(playerMovement + off::PlayerMovement::maxSpeed) = val;
		} catch(...) {}
		lua_pushboolean(lua, false);
		return 1;
	}

	static int IsAlive(lua_State* lua)
	{
		try
		{
			lua_pushboolean(lua, (bool)Cheat::playerMovement);
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int GetMB(lua_State* lua)
	{
		using namespace Cheat;
		try
		{
			if (!playerMovement) return 0;
			lua_pushlightuserdata(lua, playerMovement);
			lua_set_class(UMonoBehaviour);
			return 1;
		} catch(...) {}
		return 0;
	}

	static int GetRigidbody(lua_State* lua)
	{
		try
		{
			if (!Cheat::playerMovement) return 0;
			lua_pushlightuserdata(lua, *(void**)(Cheat::playerMovement + off::PlayerMovement::rb));
			lua_set_class(URigidbody);
			return 1;
		}
		catch (...) {}
		return 0;
	}
	
	static int GetCamTransform(lua_State* lua)
	{
		using namespace Cheat;
		try
		{
			if (!playerMovement) return 0;
			static const auto GetPlayerCamTransform = (Transform * (*)(Component*))il2cpp::get_method(XorStr("Assembly-CSharp"),
				XorStr("UnityEngine.Transform"),
				XorStr(""),
				XorStr("PlayerMovement"),
				XorStr("GetPlayerCamTransform"),
				XorStr(""));
			lua_pushlightuserdata(lua, GetPlayerCamTransform(playerMovement));
			lua_set_class(UTransform);
			return 1;
		} catch(...) {}
		return 0;
	}
	
	static int GetPlayerId(lua_State* lua)
	{
		try
		{
			lua_pushnumber(lua, (uintptr_t)Cheat::getLocalPlayerId());
			return 1;
		} catch(...) {}
		return 0;
	}
};

#pragma endregion // LuaFuncs->LocalPlayer


#pragma region mem

struct LuaMem
{
	static int readPtr(lua_State* lua)
	{
		try
		{
			lua_pushnumber(lua, *(uintptr_t*)((uintptr_t)lua_tonumber(lua, -1)));
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int reaInt(lua_State* lua)
	{
		try
		{
			lua_pushnumber(lua, *(int*)((uintptr_t)lua_tonumber(lua, -1)));
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int readFloat(lua_State* lua)
	{
		try
		{
			lua_pushnumber(lua, *(float*)((uintptr_t)lua_tonumber(lua, -1)));
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int readByte(lua_State* lua)
	{
		try
		{
			lua_pushnumber(lua, *(byte*)((uintptr_t)lua_tonumber(lua, -1)));
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int readString(lua_State* lua)
	{
		try
		{
			lua_pushstring(lua, (char*)((uintptr_t)lua_tonumber(lua, -1)));
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int readVec2(lua_State* lua)
	{
		try
		{
			vec2 vec = *(vec2*)(uintptr_t)lua_tonumber(lua, -1);
			lua_pushnumber(lua, vec.x);
			lua_pushnumber(lua, vec.y);
			return 2;
		}
		catch (...) {}
		return 0;
	}

	static int readVec3(lua_State* lua)
	{
		try
		{
			vec3 vec = *(vec3*)(uintptr_t)lua_tonumber(lua, -1);
			lua_pushnumber(lua, vec.x);
			lua_pushnumber(lua, vec.y);
			lua_pushnumber(lua, vec.z);
			return 3;
		}
		catch (...) {}
		return 0;
	}

	static int readMBPtr(lua_State* lua)
	{
		try
		{
			lua_pushlightuserdata(lua, *(void**)((uintptr_t)lua_tonumber(lua, -1)));
			lua_set_class(UMonoBehaviour);
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int readTrmPtr(lua_State* lua)
	{
		try
		{
			lua_pushlightuserdata(lua, *(void**)((uintptr_t)lua_tonumber(lua, -1)));
			lua_set_class(UTransform);
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int readStruct(lua_State* lua)
	{
		try
		{
			size_t size = lua_tonumber(lua, 2);
			void* data = lua_newuserdata(lua, size);

			int numArgs = lua_gettop(lua);
			if (numArgs == 3)
			{
				luaL_getmetatable(lua, lua_tostring(lua, 3));
				lua_setmetatable(lua, -2);
			}

			memcpy_s(data, size, (void*)(uintptr_t)lua_tonumber(lua, 1), size);
			return 1;
		} catch(...) {}
		return 0;
	}

	static int writePtr(lua_State* lua)
	{
		try
		{
			*(void**)((uintptr_t)lua_tonumber(lua, -2)) = (void*)(uintptr_t)lua_tonumber(lua, -1);
		}
		catch (...) {}
		return 0;
	}

	static int writeInt(lua_State* lua)
	{
		try
		{
			*(int*)((uintptr_t)lua_tonumber(lua, -2)) = (int)lua_tonumber(lua, -1);
		}
		catch (...) {}
		return 0;
	}

	static int writeFloat(lua_State* lua)
	{
		try
		{
			*(float*)((uintptr_t)lua_tonumber(lua, -2)) = lua_tofloat(-1);
		}
		catch (...) {}
		return 0;
	}

	static int writeByte(lua_State* lua)
	{
		try
		{
			*(byte*)((uintptr_t)lua_tonumber(lua, -2)) = (byte)lua_tonumber(lua, -1);
		}
		catch (...) {}
		return 0;
	}

	static int writeString(lua_State* lua)
	{
		try
		{
			int size = lua_tonumber(lua, -1);
			if (size <= 0) return 0;
			const char* str = lua_tostring(lua, -2);
			if (size > strlen(str)) return 0;

			uintptr_t addr = (uintptr_t)lua_tonumber(lua, -3);
			for (USHORT i = 0; i < size; i++)
				*(char*)(addr + i * sizeof(char)) = str[i];
		}
		catch (...) {}
		return 0;
	}

	static int writeVec2(lua_State* lua)
	{
		try
		{
			*(vec2*)((uintptr_t)lua_tonumber(lua, -3)) = lua_tovec2(-1);
		}
		catch (...) {}
		return 0;
	}

	static int writeVec3(lua_State* lua)
	{
		try
		{
			*(vec3*)((uintptr_t)lua_tonumber(lua, -4)) = lua_tovec3(-1);
		}
		catch (...) {}
		return 0;
	}

	static int writeStruct(lua_State* lua)
	{
		try
		{
			void* data = lua_touserdata(lua, -1);
			size_t size = lua_tonumber(lua, -2);
			memcpy_s((void*)(uintptr_t)lua_tonumber(lua, -3), size, data, size);
		} catch(...) {}
		return 0;
	}

	static int isValidPtr(lua_State* lua)
	{
		try
		{
			lua_pushboolean(lua, (bool)IsValidPointer((uintptr_t)lua_tonumber(lua, -1), { 0x0 }));
			return 1;
		}
		catch (...) {}
		return 0;
	}

	static int getStaticPtr(lua_State* lua)
	{
		try
		{
			uintptr_t base = (uintptr_t)lua_tonumber(lua, -2);
			std::vector<int> offsets;
			lua_gettable(lua, -1);
			lua_pushnil(lua);
			while (lua_next(lua, -2))
			{
				offsets.push_back((int)lua_tonumber(lua, -1));
				lua_pop(lua, 1);
			}

			lua_pushnumber(lua, staticAddress(base, offsets));
			return 1;
		} catch(...) {}
		return 0;
	}
};

#pragma endregion // LuaFuncs->mem


#pragma endregion // LuaFuncs



bool ScriptFile::Init()
{
	lua = luaL_newstate();
	luaL_openlibs(lua);

	// --------------------------------
	// DEFINE GLOBAL VARIABLES
	// --------------------------------

	// MESSAGE BOX STUFF
	lua_def_globalnumex(MB_ICONERROR);
	lua_def_globalnumex(MB_ICONWARNING);
	lua_def_globalnumex(MB_ICONQUESTION);
	lua_def_globalnumex(MB_ICONINFORMATION);
	lua_def_globalnumex(MB_YESNO);
	lua_def_globalnumex(MB_YESNOCANCEL);
	lua_def_globalnumex(MB_RETRYCANCEL);
	lua_def_globalnumex(MB_OK);
	lua_def_globalnumex(IDYES);
	lua_def_globalnumex(IDNO);
	lua_def_globalnumex(IDCANCEL);
	lua_def_globalnumex(IDRETRY);

	// VIRTUAL KEYS DEFENITIONS
	lua_def_globalnumex(VK_ESCAPE);
	lua_def_globalnumex(VK_TAB);
	lua_def_globalnumex(VK_CAPITAL);
	lua_def_globalnumex(VK_SHIFT);
	lua_def_globalnumex(VK_CONTROL);
	lua_def_globalnumex(VK_MENU);
	lua_def_globalnumex(VK_XBUTTON1);
	lua_def_globalnumex(VK_XBUTTON2);
	lua_def_globalnumex(VK_F1);
	lua_def_globalnumex(VK_F2);
	lua_def_globalnumex(VK_F3);
	lua_def_globalnumex(VK_F4);
	lua_def_globalnumex(VK_F5);
	lua_def_globalnumex(VK_F6);
	lua_def_globalnumex(VK_F7);
	lua_def_globalnumex(VK_F8);
	lua_def_globalnumex(VK_F9);
	lua_def_globalnumex(VK_F10);
	lua_def_globalnumex(VK_F11);
	lua_def_globalnumex(VK_F12);
	lua_def_globalnumex(VK_F13);
	lua_def_globalnumex(VK_F14);
	lua_def_globalnumex(VK_F15);
	lua_def_globalnumex(VK_F16);
	lua_def_globalnumex(VK_F17);
	lua_def_globalnumex(VK_F18);
	lua_def_globalnumex(VK_F19);
	lua_def_globalnumex(VK_F20);
	lua_def_globalnumex(VK_F21);
	lua_def_globalnumex(VK_F22);
	lua_def_globalnumex(VK_F23);
	lua_def_globalnumex(VK_F24);
	lua_def_globalnumex(VK_HOME);
	lua_def_globalnumex(VK_DELETE);
	lua_def_globalnumex(VK_INSERT);
	lua_def_globalnumex(VK_END);
	lua_def_globalnumex(VK_SPACE);
	lua_def_globalnumex(VK_SCROLL);
	lua_def_globalnumex(VK_LBUTTON);
	lua_def_globalnumex(VK_RBUTTON);
	lua_def_globalnumex(VK_RETURN);
	lua_def_globalnumex(VK_NUMPAD0);
	lua_def_globalnumex(VK_NUMPAD1);
	lua_def_globalnumex(VK_NUMPAD2);
	lua_def_globalnumex(VK_NUMPAD3);
	lua_def_globalnumex(VK_NUMPAD4);
	lua_def_globalnumex(VK_NUMPAD5);
	lua_def_globalnumex(VK_NUMPAD6);
	lua_def_globalnumex(VK_NUMPAD7);
	lua_def_globalnumex(VK_NUMPAD8);
	lua_def_globalnumex(VK_NUMPAD9);

	// MOUSE EVENT DEFENITIONS
	lua_def_globalnumex(MOUSEEVENTF_MOVE);
	lua_def_globalnumex(MOUSEEVENTF_MOVE_NOCOALESCE);
	lua_def_globalnumex(MOUSEEVENTF_ABSOLUTE);
	lua_def_globalnumex(MOUSEEVENTF_HWHEEL);
	lua_def_globalnumex(MOUSEEVENTF_LEFTDOWN);
	lua_def_globalnumex(MOUSEEVENTF_WHEEL);
	lua_def_globalnumex(MOUSEEVENTF_LEFTUP);
	lua_def_globalnumex(MOUSEEVENTF_RIGHTDOWN);
	lua_def_globalnumex(MOUSEEVENTF_RIGHTUP);
	lua_def_globalnumex(MOUSEEVENTF_MIDDLEDOWN);
	lua_def_globalnumex(MOUSEEVENTF_MIDDLEUP);
	lua_def_globalnumex(MOUSEEVENTF_XDOWN);
	lua_def_globalnumex(MOUSEEVENTF_XUP);
	lua_def_globalnumex(MOUSEEVENTF_VIRTUALDESK);

	// SEND INPUT KEY DEFENITIONS
	lua_def_globalnumex(KEYEVENTF_KEYUP);
	lua_def_globalnum(XorCStr("KEYEVENTF_KEYDOWN"), 0);
	lua_def_globalnumex(KEYEVENTF_EXTENDEDKEY);
	lua_def_globalnumex(KEYEVENTF_SCANCODE);
	lua_def_globalnumex(KEYEVENTF_UNICODE);

	// INFO
	lua_def_globalbool(XorCStr("_IN_GAME"), Client::bInGame);
	lua_def_globalnum(XorCStr("_WIN_SIZE_X"), Client::WindowSize.x);
	lua_def_globalnum(XorCStr("_WIN_SIZE_Y"), Client::WindowSize.y);
	lua_def_globalnum(XorCStr("_WIN_CENTER_X"), Client::WindowCenter.x);
	lua_def_globalnum(XorCStr("_WIN_CENTER_Y"), Client::WindowCenter.y);
	static const uintptr_t GameAssembly = (uintptr_t)GetModuleHandleA(XorCStr("GameAssembly.dll"));
	lua_def_globalnum(XorCStr("_GAME_ASSEMBLY"), GameAssembly);
	lua_def_globalstr(XorCStr("__MENU_VERSION"), MenuVersion.c_str());
	lua_def_globalstr(XorCStr("__GAME_VERSION"), GameVersion.c_str());

	// --------------------------------
	// DEFINE FUNCTIONS
	// --------------------------------

	// GLOBAL
	lua_register(lua, XorCStr("Enable"), LuaGlobals::Enable);
	lua_register(lua, XorCStr("Disable"), LuaGlobals::Disable);
	lua_register(lua, XorCStr("Color"), LuaGlobals::Color);
	lua_register(lua, XorCStr("LUDtoINT"), LuaGlobals::LUDtoINT);

	// GUI
	lua_start_ns("gui");
	lua_ns_register(XorCStr("MessageBoxA"), LuaGUI::MessageBoxA_);
	lua_ns_register(XorCStr("MessageBoxW"), LuaGUI::MessageBoxW_);
	lua_ns_register(XorCStr("MessageBoxExA"), LuaGUI::MessageBoxExA_);
	lua_ns_register(XorCStr("Checkbox"), LuaGUI::Checkbox);
	lua_ns_register(XorCStr("Button"), LuaGUI::Button);
	lua_ns_register(XorCStr("Hotkey"), LuaGUI::Hotkey);
	lua_ns_register(XorCStr("Text"), LuaGUI::Text);
	lua_ns_register(XorCStr("FloatSlider"), LuaGUI::SliderFloat);
	lua_ns_register(XorCStr("IntSlider"), LuaGUI::SliderInt);
	lua_ns_register(XorCStr("InputFloat"), LuaGUI::InputFloat);
	lua_ns_register(XorCStr("InputInt"), LuaGUI::InputInt);
	lua_ns_register(XorCStr("InputText"), LuaGUI::InputText);
	lua_ns_register(XorCStr("Combo"), LuaGUI::Combo);
	lua_ns_register(XorCStr("Spacing"), LuaGUI::Spacing);
	lua_ns_register(XorCStr("Separator"), LuaGUI::Separator);
	lua_ns_register(XorCStr("SameLine"), LuaGUI::SameLine);
	lua_ns_register(XorCStr("SetNextItemWidth"), LuaGUI::SetNextItemWidth);
	lua_end_ns();

	// INPUT
	lua_start_ns("input");
	lua_ns_register(XorCStr("IsKeyDown"), LuaInput::IsKeyDown);
	lua_ns_register(XorCStr("IsKeyPressed"), LuaInput::IsKeyPressed);
	lua_ns_register(XorCStr("GetMouse"), LuaInput::GetMousePos);
	lua_ns_register(XorCStr("SetMouse"), LuaInput::SetMousePos);
	lua_ns_register(XorCStr("MouseEvent"), LuaInput::MouseEvent);
	lua_ns_register(XorCStr("SetKey"), LuaInput::SetKey);
	lua_end_ns();

	// DRAWING
	lua_start_ns("draw");
	lua_ns_register(XorCStr("AddLine"), LuaDrawing::AddLine);
	lua_ns_register(XorCStr("AddRect"), LuaDrawing::AddRect);
	lua_ns_register(XorCStr("AddFilledRect"), LuaDrawing::AddFilledRect);
	lua_ns_register(XorCStr("AddCircle"), LuaDrawing::AddCircle);
	lua_ns_register(XorCStr("AddFilledCircle"), LuaDrawing::AddFilledCircle);
	lua_ns_register(XorCStr("AddText"), LuaDrawing::AddText);
	lua_end_ns();

	// LOCAL PLAYER
	lua_start_ns("LP");
	lua_ns_register(XorCStr("AimAtPos"), LuaLP::AimAtPos);
	lua_ns_register(XorCStr("setSpeed"), LuaLP::SetSpeed);
	lua_ns_register(XorCStr("isAlive"), LuaLP::IsAlive);
	lua_ns_register(XorCStr("getMB"), LuaLP::GetMB);
	lua_ns_register(XorCStr("getRigidbody"), LuaLP::GetRigidbody);
	lua_ns_register(XorCStr("getCamTransform"), LuaLP::GetCamTransform);
	lua_ns_register(XorCStr("getId"), LuaLP::GetPlayerId);
	lua_end_ns();

	// GAME
	lua_start_ns("game");
	lua_ns_register(XorCStr("WorldToScreen"), LuaGame::WorldToScreen);
	lua_ns_register(XorCStr("GetAlivePlayers"), LuaGame::GetAlivePlayers);
	lua_ns_register(XorCStr("GetAllPlayers"), LuaGame::GetAllPlayers);
	lua_ns_register(XorCStr("GetHostId"), LuaGame::GetHostId);
	lua_ns_register(XorCStr("GetObjectOfType"), LuaGame::GetObjectOfType);
	lua_ns_register(XorCStr("GetObjectsOfType"), LuaGame::GetObjectsOfType);
	lua_ns_register(XorCStr("GetMethod"), LuaGame::GetMethod);
	lua_ns_register(XorCStr("GetClass"), LuaGame::GetClass);
	lua_ns_register(XorCStr("Cast"), LuaGame::Cast);
	lua_ns_register(XorCStr("CallFunc"), LuaGame::CallFunc);
	lua_ns_register(XorCStr("Raycast"), LuaGame::Raycast);
	lua_end_ns();

	// MENU
	lua_start_ns("menu");
	lua_ns_register(XorCStr("GetFeature"), LuaMenu::GetFeature);
	lua_ns_register(XorCStr("SetFeature"), LuaMenu::SetFeature);
	lua_end_ns();

	// MEM
	lua_start_ns("mem");
	lua_ns_register(XorCStr("readPtr"), LuaMem::readPtr);
	lua_ns_register(XorCStr("reaInt"), LuaMem::reaInt);
	lua_ns_register(XorCStr("readFloat"), LuaMem::readFloat);
	lua_ns_register(XorCStr("readByte"), LuaMem::readByte);
	lua_ns_register(XorCStr("readString"), LuaMem::readString);
	lua_ns_register(XorCStr("readVec2"), LuaMem::readVec2);
	lua_ns_register(XorCStr("readVec3"), LuaMem::readVec3);
	lua_ns_register(XorCStr("readMBPtr"), LuaMem::readMBPtr);
	lua_ns_register(XorCStr("readTrmPtr"), LuaMem::readTrmPtr);
	lua_ns_register(XorCStr("readStruct"), LuaMem::readStruct);
	lua_ns_register(XorCStr("writePtr"), LuaMem::writePtr);
	lua_ns_register(XorCStr("writeInt"), LuaMem::writeInt);
	lua_ns_register(XorCStr("writeFloat"), LuaMem::writeFloat);
	lua_ns_register(XorCStr("writeByte"), LuaMem::writeByte);
	lua_ns_register(XorCStr("writeString"), LuaMem::writeString);
	lua_ns_register(XorCStr("writeVec2"), LuaMem::writeVec2);
	lua_ns_register(XorCStr("writeVec3"), LuaMem::writeVec3);
	lua_ns_register(XorCStr("writeStruct"), LuaMem::writeStruct);
	lua_ns_register(XorCStr("isValidPtr"), LuaMem::isValidPtr);
	lua_ns_register(XorCStr("getStaticPtr"), LuaMem::getStaticPtr);
	lua_end_ns();

	// --------------------------------
	// DEFINE CLASSES
	// --------------------------------

	// ONLINE PLAYER MOVEMENT
	lua_start_class(Player);
	lua_class_func(XorCStr("getPos"), LuaOnlinePlayerMovement::GetPos);
	lua_class_func(XorCStr("setPos"), LuaOnlinePlayerMovement::SetPlayerPos);
	lua_class_func(XorCStr("getBonePos"), LuaOnlinePlayerMovement::GetBonePos);
	lua_class_func(XorCStr("getRot"), LuaOnlinePlayerMovement::GetRotation);
	lua_class_func(XorCStr("getBoneMatrix"), LuaOnlinePlayerMovement::GetBoneMatrix);
	lua_class_func(XorCStr("getId"), LuaOnlinePlayerMovement::GetPlayerId);
	lua_class_func(XorCStr("getPlayerNum"), LuaOnlinePlayerMovement::GetPlayerNum);
	lua_end_class(Player);

	// UNITY MONO BEHAVIOUR
	lua_start_class(UMonoBehaviour);
	lua_class_func(XorCStr("getPos"), LuaMonoBehaviour::GetPos);
	lua_class_func(XorCStr("getTrm"), LuaMonoBehaviour::GetTrm);
	lua_class_func(XorCStr("getGameObject"), LuaMonoBehaviour::GetGameObject);
	lua_end_class(UMonoBehaviour);

	// UNITY TRANSFORM
	lua_start_class(UTransform);
	lua_class_func(XorCStr("getPos"), LuaTransform::GetPos);
	lua_class_func(XorCStr("getRot"), LuaTransform::GetRot);
	lua_class_func(XorCStr("setPos"), LuaTransform::SetPos);
	lua_class_func(XorCStr("setRot"), LuaTransform::SetRot);
	lua_class_func(XorCStr("getGameObject"), LuaTransform::GetGameObject);
	lua_end_class(UTransform);

	// UNITY RIGIDBODY
	lua_start_class(URigidbody);
	lua_class_func(XorCStr("getVel"), LuaRigidbody::GetVelocity);
	lua_class_func(XorCStr("setVel"), LuaRigidbody::SetVelocity);
	lua_class_func(XorCStr("getAngularVel"), LuaRigidbody::GetAngularVelocity);
	lua_class_func(XorCStr("setAngularVel"), LuaRigidbody::SetAngularVelocity);
	lua_class_func(XorCStr("getDrag"), LuaRigidbody::GetDrag);
	lua_class_func(XorCStr("setDrag"), LuaRigidbody::SetDrag);
	lua_class_func(XorCStr("setAngularDrag"), LuaRigidbody::SetAngularDrag);
	lua_class_func(XorCStr("setMass"), LuaRigidbody::SetMass);
	lua_class_func(XorCStr("setUseGravity"), LuaRigidbody::SetUseGravity);
	lua_class_func(XorCStr("getIsKinematic"), LuaRigidbody::GetIsKinematic);
	lua_class_func(XorCStr("setIsKinematic"), LuaRigidbody::SetIsKinematic);
	lua_class_func(XorCStr("getPos"), LuaRigidbody::GetPosition);
	lua_class_func(XorCStr("setPos"), LuaRigidbody::SetPosition);
	lua_class_func(XorCStr("getRot"), LuaRigidbody::GetRot);
	lua_class_func(XorCStr("setRot"), LuaRigidbody::SetRot);
	lua_class_func(XorCStr("addForce"), LuaRigidbody::AddForce);
	lua_end_class(URigidbody);

	return !luaL_dostring(lua, FileToString(this->path).c_str());
}

void ScriptFile::Uninit()
{
	lua_close(lua);
	lua = nullptr;
}

bool ScriptFile::DoesFuncnExist(const char* funcName)
{
	lua_getglobal(lua, funcName);
	if (lua_type(lua, -1) == LUA_TFUNCTION)
		return true;
	return false;
}

bool ScriptFile::CallFunc(const char* funcName)
{
	lua_getglobal(lua, funcName);
	if (lua_type(lua, -1) == LUA_TFUNCTION)
		return !lua_pcall(lua, 0, 0, 0);
	return false;
}

const char* ScriptFile::GetLastError()
{
	return lua_tostring(lua, -1);
}
