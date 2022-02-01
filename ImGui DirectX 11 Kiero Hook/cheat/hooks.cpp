#include "pch.h"
#include "hooks.h"
#include "cheat.h"
#include <fstream>

#include "../il2cpp/il2cpp_loader.h"
#include "../kiero/minhook/include/MinHook.h"

namespace Hooks
{
	__forceinline bool __HOOK(void* const hFunc, void** const oFunc,
		const std::string& return_type,
		const std::string& class_name,
		const std::string& method_name,
		const std::string& parameters)
	{
		void* target = (void*)il2cpp::get_method(XorStr("Assembly-CSharp"), return_type, XorStr(""), class_name, method_name, parameters);

		if (target)
		{
			MH_CreateHook(target, hFunc, oFunc);
			return MH_EnableHook(target) == MH_OK;
		}
		return false;
	}

	bool Init()
	{
		// GameUI.Update - for executing cheat
		// the entire cheat depends on this hook if it fails, we can't continue
		if (!__HOOK((void*)&Cheat::hGameUI_Update, (void**)&oGameUI_Update, XorStr("System.Void"), XorStr("GameUI"), XorStr("Update"), XorStr("")))
			return false;

		// MenuUI.ExitGame - to know when we are not in-game
		__HOOK((void*)&Cheat::hMenuUI_Start, (void**)&oMenuUI_Start, XorStr("System.Void"), XorStr("MenuUI"), XorStr("Start"), XorStr(""));

		// GameManager.PunchPlayer - for Anti Punch
		__HOOK((void*)&Cheat::hGameManager_PunchPlayer, (void**)&oGameManager_PunchPlayer, XorStr("System.Void"), XorStr("GameManager"), XorStr("PunchPlayer"), XorStr("System.UInt64, System.UInt64, UnityEngine.Vector3"));

		// PlayerMovement.PushPlayer - for Anti Knockback
		__HOOK((void*)&Cheat::hPlayerMovement_PushPlayer, (void**)&oPlayerMovement_PushPlayer, XorStr("System.Void"), XorStr("PlayerMovement"), XorStr("PushPlayer"), XorStr("UnityEngine.Vector3"));

		// ServerSend.PunchPlayer - for Super Punch
		__HOOK((void*)&Cheat::hServerSend_PunchPlayer, (void**)&oServerSend_PunchPlayer, XorStr("System.Void"), XorStr("ServerSend"), XorStr("PunchPlayer"), XorStr("System.UInt64, System.UInt64, UnityEngine.Vector3"));

		// ServerSend.LoadMap - for Map Selection
		__HOOK((void*)&Cheat::hServerSend_LoadMap, (void**)&oServerSend_LoadMap, XorStr("System.Void"), XorStr("ServerSend"), XorStr("LoadMap"), XorStr("System.Int32, System.Int32"));

		// PlayerStatus.DamagePlayer - for Godmode
		__HOOK((void*)&Cheat::hPlayerStatus_DamagePlayer, (void**)&oPlayerStatus_DamagePlayer, XorStr("System.Void"), XorStr("PlayerStatus"), XorStr("DamagePlayer"), XorStr("System.Int32, UnityEngine.Vector3, System.UInt64, System.Int32"));

		// CameraShake - for No Camera Shake
		__HOOK((void*)&Cheat::hCameraShaker_Update, (void**)&oCameraShaker_Update, XorStr("System.Void"), XorStr("CameraShaker"), XorStr("Update"), XorStr(""));
		__HOOK((void*)&Cheat::hCameraShaker_GunShake, (void**)&oCameraShaker_GunShake, XorStr("System.Void"), XorStr("CameraShaker"), XorStr("GunShake"), XorStr("System.Single"));
		__HOOK((void*)&Cheat::hCameraShaker_PushShake, (void**)&oCameraShaker_PushShake, XorStr("System.Void"), XorStr("CameraShaker"), XorStr("PushShake"), XorStr(""));
		__HOOK((void*)&Cheat::hCameraShaker_DamageShake, (void**)&oCameraShaker_DamageShake, XorStr("System.Void"), XorStr("CameraShaker"), XorStr("DamageShake"), XorStr("System.Single"));

		// GameModeRedLight.CanSeePlayer - for RLGL Godmode
		__HOOK((void*)&Cheat::hGameModeRedLight_CanSeePlayer, (void**)&oGameModeRedLight_CanSeePlayer, XorStr("System.Boolean"), XorStr("GameModeRedLight"), XorStr("CanSeePlayer"), XorStr("System.UInt64"));

		// GameMode((Bomb)Tag/Hat) - for Anti Tag
		__HOOK((void*)&Cheat::hGameModeTag_TagPlayer, (void**)&oGameModeTag_TagPlayer, XorStr("System.Void"), XorStr("GameModeTag"), XorStr("TagPlayer"), XorStr("System.UInt64, System.UInt64"));
		__HOOK((void*)&Cheat::hGameModeBombTag_TagPlayer, (void**)&oGameModeBombTag_TagPlayer, XorStr("System.Void"), XorStr("GameModeBombTag"), XorStr("TryTagPlayer"), XorStr("System.UInt64, System.UInt64"));
		__HOOK((void*)&Cheat::hGameModeHat_StealHat, (void**)&oGameModeHat_StealHat, XorStr("System.Void"), XorStr("GameModeHat"), XorStr("StealHat"), XorStr("System.UInt64, System.UInt64"));

		// Chatbox.SendMessage - for Commands
		__HOOK((void*)&Cheat::hChatbox_SendMessage, (void**)&oChatbox_SendMessage, XorStr("System.Void"), XorStr("Chatbox"), XorStr("SendMessage"), XorStr("System.String"));

		// PlayerInput.Update - for blocking input when menu is opened
		__HOOK((void*)&Cheat::hPlayerInput_Update, (void**)&oPlayerInput_Update, XorStr("System.Void"), XorStr("PlayerInput"), XorStr("Update"), XorStr(""));

		// ClientSend.PlayerRotation - for Spin Bot
		__HOOK((void*)&Cheat::hClientSend_PlayerRotation, (void**)&oClientSend_PlayerRotation, XorStr("System.Void"), XorStr("ClientSend"), XorStr("PlayerRotation"), XorStr("System.Single, System.Single, System.UInt64"));

		// ItemGun.TryUse - for Silent Aim
		__HOOK((void*)&Cheat::hItemGun_TryUse, (void**)&oItemGun_TryUse, XorStr("System.Void"), XorStr("ItemGun"), XorStr("TryUse"), XorStr("UnityEngine.Transform"));
		__HOOK((void*)&Cheat::hItemSnowball_TryUse, (void**)&oItemSnowball_TryUse, XorStr("System.Void"), XorStr("ItemSnowball"), XorStr("TryUse"), XorStr("UnityEngine.Transform"));

		// ClientSend.UseItem/DamagePlayer - for Anti AK ban
		__HOOK((void*)&Cheat::hClientSend_UseItem, (void**)&oClientSend_UseItem, XorStr("System.Void"), XorStr("ClientSend"), XorStr("UseItem"), XorStr("System.Int32, UnityEngine.Vector3"));
		__HOOK((void*)&Cheat::hClientSend_DamagePlayer, (void**)&oClientSend_DamagePlayer, XorStr("System.Void"), XorStr("ClientSend"), XorStr("DamagePlayer"), XorStr("System.UInt64, System.Int32, UnityEngine.Vector3, System.Int32, System.Int32"));

		// ClientSend.PlayerPosition - for No Bound Kill
		__HOOK((void*)&Cheat::hClientSend_PlayerPosition, (void**)&oClientSend_PlayerPosition, XorStr("System.Void"), XorStr("ClientSend"), XorStr("PlayerPosition"), XorStr("UnityEngine.Vector3, System.UInt64"));

		// CameraRecoil.AddRecoil - for No Recoil
		__HOOK((void*)&Cheat::hCameraRecoil_AddRecoil, (void**)&oCameraRecoil_AddRecoil, XorStr("System.Void"), XorStr("CameraRecoil"), XorStr("AddRecoil"), XorStr("UnityEngine.Vector2"));

		// ServerSend.SendButtonState - for Troll Button Screen
		__HOOK((void*)&Cheat::hServerSend_SendButtonPress, (void**)&oServerSend_SendButtonState, XorStr("System.Void"), XorStr("ServerSend"), XorStr("SendButtonState"), XorStr("auto"));

		// GameModeButtons.PressButton - for Only Rigged Buttons
		__HOOK((void*)&Cheat::hGameModeButtons_PressButton, (void**)&oGameModeButtons_PressButton, XorStr("System.Void"), XorStr("GameModeButtons"), XorStr("PressButton"), XorStr("System.UInt64, System.Int32"));

		return true;
	}

	void Uninit()
	{
		MH_Uninitialize();
	}
}
