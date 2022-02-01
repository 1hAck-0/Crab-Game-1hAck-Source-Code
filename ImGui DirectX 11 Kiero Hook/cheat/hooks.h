#pragma once
#include "cheat.h"
#include "UnityEngine.hpp"
#include "Assembly-CSharp.hpp"

namespace Hooks
{
	using namespace UnityEngine;
	using namespace Assembly_CSharp;

	using tGameUI_Update = void(*)(uintptr_t GameUI);
	inline tGameUI_Update oGameUI_Update;

	using tMenuUI_Start = void(*)(uintptr_t MenuUI);
	inline tMenuUI_Start oMenuUI_Start;

	using tGameManager_PunchPlayer = void(*)(uintptr_t GameManager, uintptr_t puncher, uintptr_t punched, vec3 dir);
	inline tGameManager_PunchPlayer oGameManager_PunchPlayer;

	using tPlayerMovement_PushPlayer = void(*)(uintptr_t PlayerMovement, vec3 pushForce);
	inline tPlayerMovement_PushPlayer oPlayerMovement_PushPlayer;

	using tServerSend_PunchPlayer = void(*)(uintptr_t fromClient, uintptr_t punchedPlayer, vec3 dir);
	inline tServerSend_PunchPlayer oServerSend_PunchPlayer;

	using tServerSend_LoadMap = void(*)(int mapId, int gameModeId);
	inline tServerSend_LoadMap oServerSend_LoadMap;

	using tPlayerStatus_DamagePlayer = void(*)(uintptr_t PlayerStatus, int dmg, vec3 damageDir, uintptr_t damageDoerId, int itemId);
	inline tPlayerStatus_DamagePlayer oPlayerStatus_DamagePlayer;

	using tCameraShaker_Update = void(*)(uintptr_t CameraShaker);
	inline tCameraShaker_Update oCameraShaker_Update;
	using tCameraShaker_PushShake = tCameraShaker_Update;
	inline tCameraShaker_PushShake oCameraShaker_PushShake;
	using tCameraShaker_GunShake = void(*)(uintptr_t CameraShaker, float shakeRatio);
	inline tCameraShaker_GunShake oCameraShaker_GunShake;
	using tCameraShaker_DamageShake = tCameraShaker_GunShake;
	inline tCameraShaker_DamageShake oCameraShaker_DamageShake;

	using tGameModeRedLight_CanSeePlayer = bool(*)(uintptr_t GameModeRedLight, uintptr_t pId);
	inline tGameModeRedLight_CanSeePlayer oGameModeRedLight_CanSeePlayer;

	using tChatbox_SendMessage = void(*)(uintptr_t Chatbox, generic_string* message);
	inline tChatbox_SendMessage oChatbox_SendMessage;

	using tGameModeTag_TagPlayer = void(*)(uintptr_t GameModeTag, uintptr_t tagger, uintptr_t tagged);
	inline tGameModeTag_TagPlayer oGameModeTag_TagPlayer;
	using tGameModeBombTag_TagPlayer = void(*)(uintptr_t GameModeBombTag, uintptr_t tagger, uintptr_t tagged);
	inline tGameModeBombTag_TagPlayer oGameModeBombTag_TagPlayer;
	using tGameModeHat_StealHat = void(*)(uintptr_t GameModeHat, uintptr_t stealer, uintptr_t stealedFrom);
	inline tGameModeHat_StealHat oGameModeHat_StealHat;

	using tPlayerInput_Update = void(*)(uintptr_t PlayerInput);
	inline tPlayerInput_Update oPlayerInput_Update;

	using tClientSend_PlayerRotation = void(*)(float x, float y, uintptr_t toId);
	inline tClientSend_PlayerRotation oClientSend_PlayerRotation;

	using tItemGun_TryUse = void(*)(uintptr_t ItemGun, Transform* camForward);
	inline tItemGun_TryUse oItemGun_TryUse;
	using tItemSnowball_TryUse = void(*)(uintptr_t ItemSnowball, Transform* camForward);
	inline tItemSnowball_TryUse oItemSnowball_TryUse;

	using tClientSend_DamagePlayer = void(*)(uintptr_t hurtPlayerId, int damage, vec3 damageDir, int itemID, int objectID);
	inline tClientSend_DamagePlayer oClientSend_DamagePlayer;

	using tClientSend_UseItem = void(*)(int itemId, vec3 dir);
	inline tClientSend_UseItem oClientSend_UseItem;

	using tClientSend_PlayerPosition = void(*)(vec3 position, uintptr_t toId);
	inline tClientSend_PlayerPosition oClientSend_PlayerPosition;
	
	using tCameraRecoil_AddRecoil = void(*)(vec2 newRecoil);
	inline tCameraRecoil_AddRecoil oCameraRecoil_AddRecoil;

	using tServerSend_SendButtonState = void(*)(int buttonState);
	inline tServerSend_SendButtonState oServerSend_SendButtonState;

	using tGameModeButtons_PressButton = void(*)(uintptr_t GameModeButtons, uintptr_t playerId, int buttonId);
	inline tGameModeButtons_PressButton oGameModeButtons_PressButton;

	bool Init();

	void Uninit();
}
