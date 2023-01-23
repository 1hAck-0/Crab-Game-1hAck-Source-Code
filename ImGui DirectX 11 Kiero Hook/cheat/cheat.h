#pragma once
#include <Windows.h>
#include <vector>
#include "../menu/menu.h"
#include "../il2cpp/il2cpp_loader.h"
#include "UnityEngine.hpp"
#include "Assembly-CSharp.hpp"

struct Ent
{
	std::string name;
	vec2 headPos, feetPos;
	vec2 b1, b2, b3, b4, t1, t2, t3, t4;
};

#define TORAD(x) ((x) * 0.01745329252f)
namespace Cheat
{
	using namespace UnityEngine;
	using namespace Assembly_CSharp;

	extern HWND activeHNWD;

	extern std::vector<Ent> players;
	extern std::vector<std::string> playerNames;

	extern bool bTyping;

	extern generic_array_t<Component*>* activePlayers;
	extern generic_array_t<Component*>* glassPieces;
	extern generic_array_t<Component*>* bustlingButtons;
	extern generic_array_t<Component*>* gameModeButton;
	extern generic_array_t<Component*>* Chatbox;

	extern Component* playerMovement;
	extern Camera* camera;
	extern Component* chat;
	extern uintptr_t hostId;

	// UTIL FUNCTIONS
	uintptr_t getLocalPlayerId();
	vec3 getLocalPos();
	constexpr const int& getPlayerLayer();
	std::string getPlayerName(Component* onlinePlayerMovement);
	uintptr_t getPlayerId(Component* const onlinePlayerMovement);
	void giveItemToPlayer(const int itemId, const uintptr_t& playerId);
	Component* getClosestEnemy();
	void Cham(Component* cmp, const Color& color = { 1.f, 0, 0, 1.f });
	uintptr_t getHostId();
	Component* getHost();
	Component* getSteamManager();
	uintptr_t getCurrentMapId();
	void respawnPlayer(Component* pm, float respawnTime = 0.f);
	void setPlayerPos(uintptr_t id, vec3 pos);
	int getPlayerNum(Component* opm);
	Component* getPlayerPm(Component* opm);
	void killPlayer(Component* pm);
	enum class GameModeState : byte { Freeze, Playing, Ended, GameOver };
	void aimAt(const vec3& worldPos, const float smoothness = 1.f);
	GameModeState getGameModeState();
	std::vector<MonoBehaviour*> getAllPlayers();


	using obscured_float = generic_item<0x14>;
	inline auto GetObscuredFloat = [](float v)->obscured_float
	{
		void* __value = &v;
		obscured_float* result = (obscured_float*)il2cpp::invoke(XorStr("ACTk.Runtime"),
			XorStr("CodeStage.AntiCheat.ObscuredTypes.ObscuredFloat"),
			XorStr("CodeStage.AntiCheat.ObscuredTypes"),
			XorStr("ObscuredFloat"),
			XorStr("op_Implicit"),
			XorStr("System.Single"),
			&__value,
			nullptr);
		return *result;
	};
	inline auto GetFloatFromOF = [](obscured_float v)->float
	{
		void* __value = &v;
		float* result = (float*)il2cpp::invoke(XorStr("ACTk.Runtime"),
			XorStr("System.Single"),
			XorStr("CodeStage.AntiCheat.ObscuredTypes"),
			XorStr("ObscuredFloat"),
			XorStr("op_Implicit"),
			XorStr("CodeStage.AntiCheat.ObscuredTypes.ObscuredFloat"),
			&__value,
			nullptr);
		return *result;
	};

	using obscured_int = generic_item<0xA>;
	inline auto GetObscuredInt = [](int v)->obscured_int
	{
		void* __value = &v;
		obscured_int* result = (obscured_int*)il2cpp::invoke(XorStr("ACTk.Runtime"),
			XorStr("CodeStage.AntiCheat.ObscuredTypes.ObscuredInt"),
			XorStr("CodeStage.AntiCheat.ObscuredTypes"),
			XorStr("ObscuredInt"),
			XorStr("op_Implicit"),
			XorStr("System.Int32"),
			&__value,
			nullptr);
		return *result;
	};
	inline auto GetIntFromOI = [](obscured_int v)->int
	{
		void* __value = &v;
		int* result = (int*)il2cpp::invoke(XorStr("ACTk.Runtime"),
			XorStr("System.Int32"),
			XorStr("CodeStage.AntiCheat.ObscuredTypes"),
			XorStr("ObscuredInt"),
			XorStr("op_Implicit"),
			XorStr("CodeStage.AntiCheat.ObscuredTypes.ObscuredInt"),
			&__value,
			nullptr);
		return *result;
	};		// SOME HELPER FUNCS

	using obsecured_bool = generic_item<0xA>;
	inline auto GetObsucredBool = [](bool v)->obsecured_bool
	{
		void* __value = &v;
		obsecured_bool* result = (obsecured_bool*)il2cpp::invoke(XorStr("ACTk.Runtime"),
			XorStr("CodeStage.AntiCheat.ObscuredTypes.ObscuredBool"),
			XorStr("CodeStage.AntiCheat.ObscuredTypes"),
			XorStr("ObscuredBool"),
			XorStr("op_Implicit"),
			XorStr("System.Boolean"),
			&__value,
			nullptr);
		return *result;
	};
	inline auto GetBoolFromOB = [](obsecured_bool v)->bool
	{
		void* __value = &v;
		bool* result = (bool*)il2cpp::invoke(XorStr("ACTk.Runtime"),
			XorStr("System.Boolean"),
			XorStr("CodeStage.AntiCheat.ObscuredTypes"),
			XorStr("ObscuredBool"),
			XorStr("op_Implicit"),
			XorStr("CodeStage.AntiCheat.ObscuredTypes.ObscuredBool"),
			&__value,
			nullptr);
		return *result;
	};

	// VISUALS
	void RenderVisuals();

	// HOOKS
	void hGameUI_Update(uintptr_t GameUI);

	void hMenuUI_Start(uintptr_t MenuUI);

	void hGameManager_PunchPlayer(uintptr_t GameManager, uintptr_t puncher, uintptr_t punched, vec3 dir);

	void hPlayerMovement_PushPlayer(uintptr_t PlayerMovement, vec3 pushForce);

	void hServerSend_PunchPlayer(uintptr_t fromClient, uintptr_t punchedPlayer, vec3 dir);

	void hServerSend_LoadMap(int mapId, int gameModeId);

	void hPlayerStatus_DamagePlayer(uintptr_t PlayerStatus, int dmg, vec3 damageDir, uintptr_t damagerDoerId, int itemId);

	void hCameraShaker_Update(uintptr_t CameraShaker);
	void hCameraShaker_GunShake(uintptr_t CameraShaker, float shakeRatio);
	void hCameraShaker_PushShake(uintptr_t CameraShaker);
	void hCameraShaker_DamageShake(uintptr_t CameraShaker, float shakeRatio);

	bool hGameModeRedLight_CanSeePlayer(uintptr_t GameModeRedLight, uintptr_t pId);

	void hChatbox_SendMessage(uintptr_t Chatbox, generic_string* message);

	void hGameModeTag_TagPlayer(uintptr_t GameModeTag, uintptr_t tagger, uintptr_t tagged);
	void hGameModeBombTag_TagPlayer(uintptr_t GameModeBombTag, uintptr_t tagger, uintptr_t tagged);
	void hGameModeHat_StealHat(uintptr_t GameModeHat, uintptr_t stealer, uintptr_t stolenFrom);

	void hPlayerInput_Update(uintptr_t PlayerInput);

	void hClientSend_PlayerRotation(float x, float y, uintptr_t toId);

	void hClientSend_PlayerPosition(vec3 position, uintptr_t toId);

	void hItemGun_TryUse(uintptr_t ItemGun, Transform* camForward);
	void hItemSnowball_TryUse(uintptr_t ItemGun, Transform* camForward);

	void hClientSend_DamagePlayer(uintptr_t hurtPlayerId, int damage, vec3 damageDir, int itemID, int objectID);

	void hClientSend_UseItem(int itemId, vec3 dir);

	void hCameraRecoil_AddRecoil(vec2 newRecoil);

	enum class ButtonModeState : int { RevealGreen, RevealRed, ShowTimer, ResetButtons };
	void hServerSend_SendButtonPress(ButtonModeState buttonState);

	void hGameModeButtons_PressButton(uintptr_t GameModeButtons, uintptr_t playerId, int buttonId);
}
