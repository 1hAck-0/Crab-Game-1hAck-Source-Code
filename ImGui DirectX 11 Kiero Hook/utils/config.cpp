#include "pch.h"
#include "config.h"
#include "xorstr.hpp"
#include "json.hpp"
#include "../menu/menu.h"
using namespace nlohmann;
#include <fstream>
#include <thread>

namespace Client
{
	bool bInGame = false;
	HWND hwnd;
	RECT WindowRect;
	POINT WindowSize;
	POINT WindowCenter;

	void Init()
	{
		hwnd = FindWindowA(0, XorStr("Crab Game").c_str());
		GetWindowRect(hwnd, &WindowRect);

		WindowSize.x = WindowRect.right - WindowRect.left;
		WindowSize.y = WindowRect.bottom - WindowRect.top;
		if (WindowSize.x == 0 || WindowSize.y == 0)
		{
			WindowSize.x = GetSystemMetrics(SM_CXSCREEN);
			WindowSize.y = GetSystemMetrics(SM_CYSCREEN);
		}

		WindowCenter.x = WindowSize.x / 2;
		WindowCenter.y = WindowSize.y / 2;
	}
}

void cfg_t::SaveCfg()
{
	wchar_t FilePath[MAX_PATH];
	OPENFILENAMEW dialog = { sizeof(OPENFILENAME) };
	dialog.lpstrFile = FilePath;
	dialog.nMaxFile = MAX_PATH;
	dialog.lpstrFilter = dialog.lpstrDefExt = XorStr(L"1hAck Config File (*.1hCG)\0*.1hCG\0\0").c_str();
	dialog.lpstrTitle = XorStr(L"Save Config").c_str();
	dialog.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

	if (GetSaveFileNameW(&dialog))
		this->Save(FilePath);
}

void cfg_t::Save(const wchar_t* const fileName)
{
	std::ofstream file;
	file.open(fileName, std::ios_base::out);
	if (file.fail() || !file.is_open())
	{
		file.close();
		MessageBoxA(Client::hwnd, XorStr("Couldn't create the file!").c_str(), XorStr("1hAck Config - File Error").c_str(), MB_ICONERROR);
		return;
	}
	json j;

	// Player	
	j[XorStr("Player")][XorStr("bFlyhack")] = this->player.bFlyhack;
	j[XorStr("Player")][XorStr("flyKey")] = this->player.flyKey;
	j[XorStr("Player")][XorStr("flySpeed")] = this->player.flySpeed;
	j[XorStr("Player")][XorStr("bBunnyhop")] = this->player.bBunnyhop;
	j[XorStr("Player")][XorStr("bunnyHopKey")] = this->player.bunKey;
	j[XorStr("Player")][XorStr("bNoClip")] = this->player.bNoClip;
	j[XorStr("Player")][XorStr("noClipKey")] = this->player.noClipKey;
	j[XorStr("Player")][XorStr("bSuperPunch")] = this->player.bSuperPunch;
	j[XorStr("Player")][XorStr("superPunchKey")] = this->player.superPunchKey;
	j[XorStr("Player")][XorStr("punchMultiplier")] = this->player.punchMultiplier;
	j[XorStr("Player")][XorStr("bAntiPunch")] = this->player.bAntiPunch;
	j[XorStr("Player")][XorStr("antiForce")] = this->player.antiForce;
	j[XorStr("Player")][XorStr("bGodmode")] = this->player.bGodMode;
	j[XorStr("Player")][XorStr("godModeKey")] = this->player.godModeKey;
	j[XorStr("Player")][XorStr("message")] = this->player.message;
	j[XorStr("Player")][XorStr("messageAmount")] = this->player.messageAmount;
	j[XorStr("Player")][XorStr("bSpeedhack")] = this->player.bSpeedHack;
	j[XorStr("Player")][XorStr("Speed")] = this->player.Speed;
	j[XorStr("Player")][XorStr("SpeedKey")] = this->player.speedKey;
	j[XorStr("Player")][XorStr("bSuperJump")] = this->player.bSuperJump;
	j[XorStr("Player")][XorStr("superJumpKey")] = this->player.superJumpKey;
	j[XorStr("Player")][XorStr("jumpForce")] = this->player.jumpForce;
	j[XorStr("Player")][XorStr("bInfiniteJump")] = this->player.bInfiniteJump;
	j[XorStr("Player")][XorStr("infJumpKey")] = this->player.infJumpKey;
	j[XorStr("Player")][XorStr("bNoCameraShake")] = this->player.bNoCameraShake;
	j[XorStr("Player")][XorStr("weaponId")] = this->player.weaponId;
	j[XorStr("Player")][XorStr("bInfiniteAmmo")] = this->player.bInfiniteAmmo;
	j[XorStr("Player")][XorStr("bAntiVoid")] = this->player.bAntiVoid;
	j[XorStr("Player")][XorStr("antiVoidKey")] = this->player.antiVoidKey;
	j[XorStr("Player")][XorStr("glassBrealKey")] = this->player.glassBrealKey;
	j[XorStr("Player")][XorStr("bFarReach")] = this->player.bFarReach;
	j[XorStr("Player")][XorStr("farReachKey")] = this->player.farReachKey;
	j[XorStr("Player")][XorStr("clearChatKey")] = this->player.clearChatKey;
	j[XorStr("Player")][XorStr("bCommands")] = this->player.bCommands;
	j[XorStr("Player")][XorStr("commandSymbol")] = this->player.commandSymbol;
	j[XorStr("Player")][XorStr("bMouseTeleport")] = this->player.bMouseTeleport;
	j[XorStr("Player")][XorStr("mouseTpKey")] = this->player.mouseTpKey;
	j[XorStr("Player")][XorStr("bAntiTag")] = this->player.bAntiTag;
	j[XorStr("Player")][XorStr("antiTagKey")] = this->player.antiTagKey;
	j[XorStr("Player")][XorStr("send2SpaceKey")] = this->player.send2SpaceKey;
	j[XorStr("Player")][XorStr("bAntiWater")] = this->player.bAntiWater;
	j[XorStr("Player")][XorStr("antiWaterKey")] = this->player.antiWaterKey;
	j[XorStr("Player")][XorStr("bRedLightGodmode")] = this->player.bRedLightGodmode;
	j[XorStr("Player")][XorStr("bRLGLBot")] = this->player.bRLGLBot;
	j[XorStr("Player")][XorStr("rlglBotKey")] = this->player.rlglBotKey;
	j[XorStr("Player")][XorStr("bHatTeleport")] = this->player.bHbsTp;
	j[XorStr("Player")][XorStr("hatTeleportKey")] = this->player.hbsTpKey;
	j[XorStr("Player")][XorStr("bAntiKnockback")] = this->player.bAntiKnockback;
	j[XorStr("Player")][XorStr("bSuperSlide")] = this->player.bSuperSlide;
	j[XorStr("Player")][XorStr("superSlideKey")] = this->player.superSlideKey;
	j[XorStr("Player")][XorStr("bInfiniteSlide")] = this->player.bInfiniteSlide;
	j[XorStr("Player")][XorStr("infSlideKey")] = this->player.infSlideKey;
	j[XorStr("Player")][XorStr("slideForce")] = this->player.slideForce;
	j[XorStr("Player")][XorStr("bNoIceSlide")] = this->player.bNoIceSlide;
	j[XorStr("Player")][XorStr("bTileDriveTp")] = this->player.bTileDriveTp;
	j[XorStr("Player")][XorStr("tileDriveTpKey")] = this->player.tileDriveTpKey;
	j[XorStr("Player")][XorStr("nextGameMode")] = this->player.nextGameMode;
	j[XorStr("Player")][XorStr("bLowGravity")] = this->player.bModGravity;
	j[XorStr("Player")][XorStr("gravity")] = this->player.gravity;
	j[XorStr("Player")][XorStr("gravityKey")] = this->player.gravityKey;
	j[XorStr("Player")][XorStr("bSuperTireJump")] = this->player.bSuperTireJump;
	j[XorStr("Player")][XorStr("bSuperTireJump")] = this->player.superJumpKey;
	j[XorStr("Player")][XorStr("tireForce")] = this->player.tireForce;
	j[XorStr("Player")][XorStr("bNoBoundKill")] = this->player.bNoBoundKill;
	j[XorStr("Player")][XorStr("noBoundKillkey")] = this->player.noBoundKillkey;
	j[XorStr("Player")][XorStr("bInvisibility")] = this->player.bInvisibility;
	j[XorStr("Player")][XorStr("invisibilKey")] = this->player.invisibilKey;
	j[XorStr("Player")][XorStr("bPlayerTeleport")] = this->player.bPlayerTeleport;
	j[XorStr("Player")][XorStr("playerTpKey")] = this->player.playerTpKey;
	j[XorStr("Player")][XorStr("playerTpDist")] = this->player.playerTpDist;
	j[XorStr("Player")][XorStr("bSuperTireJump")] = this->player.bSuperTireJump;
	j[XorStr("Player")][XorStr("superTireJumpKey")] = this->player.superTireJumpKey;
	j[XorStr("Player")][XorStr("suicideKey")] = this->player.suicideKey;
	j[XorStr("Player")][XorStr("freezTimerKey")] = this->player.freezeTimerKey;
	j[XorStr("Player")][XorStr("bNoGlassBreak")] = this->player.bNoGlassBreak;
	j[XorStr("Player")][XorStr("bTrollButtonScreen")] = this->player.bTrollButtonScreen;
	j[XorStr("Player")][XorStr("bOnlyRiggedButtons")] = this->player.bOnlyRiggedButtons;

	// Visuals
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("key")] = this->visuals.vp.key;
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bPlayerESP")] = this->visuals.vp.bESP;
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bBoxes")] = this->visuals.vp.bBoxes;
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bSnaplines")] = this->visuals.vp.bSnaplines;
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Box Type")] = this->visuals.vp.BoxType;
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bBoxRGB")] = this->visuals.vp.bBoxRGB;
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Box Width")] = this->visuals.vp.BoxLineWidth;
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Snapline Type")] = this->visuals.vp.SnapType;
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bSnaplineRGB")] = this->visuals.vp.bSnaplineRGB;
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Snapline Width")] = this->visuals.vp.SnaplineLineWidth;
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Box Color")][XorStr("x")] = this->visuals.vp.BoxColor[0];
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Box Color")][XorStr("y")] = this->visuals.vp.BoxColor[1];
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Box Color")][XorStr("z")] = this->visuals.vp.BoxColor[2];
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Box Color")][XorStr("w")] = this->visuals.vp.BoxColor[3];
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Snapline Color")][XorStr("x")] = this->visuals.vp.SnaplineColor[0];
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Snapline Color")][XorStr("y")] = this->visuals.vp.SnaplineColor[1];
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Snapline Color")][XorStr("z")] = this->visuals.vp.SnaplineColor[2];
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Snapline Color")][XorStr("w")] = this->visuals.vp.SnaplineColor[3];
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("RGB Speed")] = this->visuals.vp.RGBSpeed;
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bNameESP")] = this->visuals.vp.bNameESP;
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bLimitDistance")] = this->visuals.vp.bLimitDistance;
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("maxDistance")] = this->visuals.vp.maxDistance;
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bEnemiesOnly")] = this->visuals.vp.bEnemiesOnly;
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bChams")] = this->visuals.vp.bChams;
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bChamRGB")] = this->visuals.vp.bChamRGB;
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Cham Color")][XorStr("x")] = this->visuals.vp.ChamColor[0];
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Cham Color")][XorStr("y")] = this->visuals.vp.ChamColor[1];
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Cham Color")][XorStr("z")] = this->visuals.vp.ChamColor[2];
	j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Cham Color")][XorStr("w")] = this->visuals.vp.ChamColor[3];

	j[XorStr("Visuals")][XorStr("Weapon ESP")][XorStr("bWeaponESP")] = this->visuals.bWeaponESP;
	j[XorStr("Visuals")][XorStr("Weapon ESP")][XorStr("bWeaponRGB")] = this->visuals.bWeaponRGB;
	j[XorStr("Visuals")][XorStr("Weapon ESP")][XorStr("Color")][XorStr("x")] = this->visuals.WeaponColor[0];
	j[XorStr("Visuals")][XorStr("Weapon ESP")][XorStr("Color")][XorStr("y")] = this->visuals.WeaponColor[1];
	j[XorStr("Visuals")][XorStr("Weapon ESP")][XorStr("Color")][XorStr("z")] = this->visuals.WeaponColor[2];
	j[XorStr("Visuals")][XorStr("Weapon ESP")][XorStr("Color")][XorStr("w")] = this->visuals.WeaponColor[3];

	j[XorStr("Visuals")][XorStr("Glass ESP")][XorStr("bEnabled")] = this->visuals.GlassESP;
	j[XorStr("Visuals")][XorStr("Glass ESP")][XorStr("Color")][XorStr("x")] = this->visuals.GlassColor[0];
	j[XorStr("Visuals")][XorStr("Glass ESP")][XorStr("Color")][XorStr("y")] = this->visuals.GlassColor[1];
	j[XorStr("Visuals")][XorStr("Glass ESP")][XorStr("Color")][XorStr("z")] = this->visuals.GlassColor[2];
	j[XorStr("Visuals")][XorStr("Glass ESP")][XorStr("Color")][XorStr("w")] = this->visuals.GlassColor[3];
	j[XorStr("Visuals")][XorStr("Glass ESP")][XorStr("bRGB")] = this->visuals.bRGBGlass;

	j[XorStr("Visuals")][XorStr("bBustlingButtonESP")] = this->visuals.ButtonESP;
	j[XorStr("Visuals")][XorStr("bFOV")] = this->visuals.bFOV;
	j[XorStr("Visuals")][XorStr("FOV")] = this->visuals.FOV;
	j[XorStr("Visuals")][XorStr("bDormFullBright")] = this->visuals.bDormFullBright;
	j[XorStr("Visuals")][XorStr("bBombTimer")] = this->visuals.bBombTimer;
	j[XorStr("Visuals")][XorStr("bZoom")] = this->visuals.bZoom;
	j[XorStr("Visuals")][XorStr("zoomKey")] = this->visuals.zoomKey;
	j[XorStr("Visuals")][XorStr("zoomFOV")] = this->visuals.zoomFOV;

	// Aim Assist
	j[XorStr("Aim Assist")][XorStr("bAim")] = this->aim.bAim;
	j[XorStr("Aim Assist")][XorStr("bAimKey")] = this->aim.bAimKey;
	j[XorStr("Aim Assist")][XorStr("Aim Key")] = this->aim.AimKey;
	j[XorStr("Aim Assist")][XorStr("Smoothness")] = this->aim.Smoothness;
	j[XorStr("Aim Assist")][XorStr("bFOV")] = this->aim.bFOV;
	j[XorStr("Aim Assist")][XorStr("FOV")] = this->aim.FOV;
	j[XorStr("Aim Assist")][XorStr("FOV Circle")][XorStr("bEnabled")] = this->aim.bFOVCircle;
	j[XorStr("Aim Assist")][XorStr("FOV Circle")][XorStr("x")] = this->aim.FOVCircleColor[0];
	j[XorStr("Aim Assist")][XorStr("FOV Circle")][XorStr("y")] = this->aim.FOVCircleColor[1];
	j[XorStr("Aim Assist")][XorStr("FOV Circle")][XorStr("z")] = this->aim.FOVCircleColor[2];
	j[XorStr("Aim Assist")][XorStr("FOV Circle")][XorStr("w")] = this->aim.FOVCircleColor[3];
	j[XorStr("Aim Assist")][XorStr("bNoRecoil")] = this->aim.bNoRecoil;
	j[XorStr("Aim Assist")][XorStr("recoilSlider")] = this->aim.recoilSlider;
	j[XorStr("Aim Assist")][XorStr("bHoldToShoot")] = this->aim.bHoldToShoot;
	j[XorStr("Aim Assist")][XorStr("bSpinBot")] = this->aim.bSpinBot;
	j[XorStr("Aim Assist")][XorStr("spinBotKey")] = this->aim.spinBotKey;
	j[XorStr("Aim Assist")][XorStr("spinBotSpeed")] = this->aim.spinBotSpeed;
	j[XorStr("Aim Assist")][XorStr("bSilentAim")] = this->aim.bSilentAim;
	j[XorStr("Aim Assist")][XorStr("silentAimKey")] = this->aim.silentAimKey;
	j[XorStr("Aim Assist")][XorStr("bRapidFire")] = this->aim.bRapidFire;

	// Tiggerbot
	j[XorStr("Triggerbot")][XorStr("bTriggerbot")] = this->aim.trg.bTrg;
	j[XorStr("Triggerbot")][XorStr("bTriggerbotKey")] = this->aim.trg.bTrgKey;
	j[XorStr("Triggerbot")][XorStr("Triggerbot Key")] = this->aim.trg.TrgKey;
	j[XorStr("Triggerbot")][XorStr("bLimitDistance")] = this->aim.trg.bLimitDistance;
	j[XorStr("Triggerbot")][XorStr("Max Distance")] = this->aim.trg.maxDistance;

	// Menu
	j[XorStr("Menu")][XorStr("Lock in Place")] = Menu::bLockInPlace;
	j[XorStr("Menu")][XorStr("Toggle Key")] = Menu::ToggleKey;
	j[XorStr("Menu")][XorStr("Close Key")] = Menu::CloseKey;
	j[XorStr("Menu")][XorStr("bMenuRGB")] = this->bMenuRGB;
	j[XorStr("Menu")][XorStr("MenuRGBSpeed")] = this->MenuRGBSpeed;
	j[XorStr("Menu")][XorStr("MenuColor")][XorStr("x")] = this->MenuColor[0];
	j[XorStr("Menu")][XorStr("MenuColor")][XorStr("y")] = this->MenuColor[1];
	j[XorStr("Menu")][XorStr("MenuColor")][XorStr("z")] = this->MenuColor[2];
	j[XorStr("Menu")][XorStr("MenuRounding")] = this->MenuRounding;
	j[XorStr("Menu")][XorStr("bAutoSave")] = this->bAutoSave;
	j[XorStr("Menu")][XorStr("bFreeMouseOnPopup")] = this->bFreeMouseOnPopup;
	j[XorStr("Menu")][XorStr("bGUISaveOnClose")] = this->script.bGUISaveOnClose;
	j[XorStr("Menu")][XorStr("bOpenedScriptsFolder")] = this->script.bOpenedFolder;
	std::wstring buf = this->script.OpenedFolder;
	j[XorStr("Menu")][XorStr("Scripts Folder")] = std::string(buf.begin(), buf.end());

	// Scripts hotkeys
	try
	{
		if (cfg.script.bOpenedFolder)
		{
			json buf;
			for (USHORT i = 0; i < cfg.script.files.size(); i++)
			{
				try
				{
					auto script = &cfg.script.files[i];
					if (script->hotkey != 0xADAF)
						buf[script->path.filename().string()] = script->hotkey;
				} catch(...) {}
			}
			j[XorStr("Menu")][XorStr("Script Hotkeys")] = buf;
		}
	} catch(...) {}

	file << std::setw(4) << j << std::endl;
	file.close();
}

void cfg_t::Load(const wchar_t* const fileName)
{
	std::ifstream file;
	file.open(fileName, std::ios_base::in);
	if (file.fail() || !file.is_open())
	{
		file.close();
		MessageBoxA(Client::hwnd, XorStr("Couldn't open file!").c_str(), XorStr("File Error").c_str(), 0);
		return;
	}

	ordered_json j = ordered_json::parse(file, 0, false);
	file.close();

#define trycatch(x) try { x; } catch(...) {}

	// Player
	trycatch(this->player.bFlyhack = j[XorStr("Player")][XorStr("bFlyhack")]);
	trycatch(this->player.flyKey = j[XorStr("Player")][XorStr("flyKey")]);
	trycatch(this->player.flySpeed = j[XorStr("Player")][XorStr("flySpeed")]);
	trycatch(this->player.bBunnyhop = j[XorStr("Player")][XorStr("bBunnyhop")]);
	trycatch(this->player.bunKey = j[XorStr("Player")][XorStr("bunnyHopKey")]);
	trycatch(this->player.bNoClip = j[XorStr("Player")][XorStr("bNoClip")]);
	trycatch(this->player.noClipKey = j[XorStr("Player")][XorStr("noClipKey")]);
	trycatch(this->player.bSuperPunch = j[XorStr("Player")][XorStr("bSuperPunch")]);
	trycatch(this->player.superPunchKey = j[XorStr("Player")][XorStr("superPunchKey")]);
	trycatch(this->player.punchMultiplier = j[XorStr("Player")][XorStr("punchMultiplier")]);
	trycatch(this->player.bAntiPunch = j[XorStr("Player")][XorStr("bAntiPunch")]);
	trycatch(this->player.antiForce = j[XorStr("Player")][XorStr("antiForce")]);
	trycatch(this->player.bGodMode = j[XorStr("Player")][XorStr("bGodmode")]);
	trycatch(this->player.godModeKey = j[XorStr("Player")][XorStr("godModeKey")]);
	std::string buffer;
	trycatch(buffer = j[XorStr("Player")][XorStr("message")]);
	strcpy_s(this->player.message, buffer.c_str());
	trycatch(this->player.messageAmount = j[XorStr("Player")][XorStr("messageAmount")]);
	trycatch(this->player.bSpeedHack = j[XorStr("Player")][XorStr("bSpeedhack")]);
	trycatch(this->player.Speed = j[XorStr("Player")][XorStr("Speed")]);
	trycatch(this->player.speedKey = j[XorStr("Player")][XorStr("SpeedKey")]);
	trycatch(this->player.bSuperJump = j[XorStr("Player")][XorStr("bSuperJump")]);
	trycatch(this->player.superJumpKey = j[XorStr("Player")][XorStr("superJumpKey")]);
	trycatch(this->player.jumpForce = j[XorStr("Player")][XorStr("jumpForce")]);
	trycatch(this->player.bInfiniteJump = j[XorStr("Player")][XorStr("bInfiniteJump")]);
	trycatch(this->player.infJumpKey = j[XorStr("Player")][XorStr("infJumpKey")]);
	trycatch(this->player.bNoCameraShake = j[XorStr("Player")][XorStr("bNoCameraShake")]);
	trycatch(this->player.weaponId = j[XorStr("Player")][XorStr("weaponId")]);
	trycatch(this->player.bInfiniteAmmo = j[XorStr("Player")][XorStr("bInfiniteAmmo")]);
	trycatch(this->player.bAntiPunch = j[XorStr("Player")][XorStr("bAntiPunch")]);
	trycatch(this->player.bAntiVoid = j[XorStr("Player")][XorStr("bAntiVoid")]);
	trycatch(this->player.antiVoidKey = j[XorStr("Player")][XorStr("antiVoidKey")]);
	trycatch(this->player.glassBrealKey = j[XorStr("Player")][XorStr("glassBrealKey")]);
	trycatch(this->player.bRedLightGodmode = j[XorStr("Player")][XorStr("bRedLightGodmode")]);
	trycatch(this->player.bRLGLBot = j[XorStr("Player")][XorStr("bRLGLBot")]);
	trycatch(this->player.rlglBotKey = j[XorStr("Player")][XorStr("rlglBotKey")]);
	trycatch(this->player.bFarReach = j[XorStr("Player")][XorStr("bFarReach")]);
	trycatch(this->player.farReachKey = j[XorStr("Player")][XorStr("farReachKey")]);
	trycatch(this->player.clearChatKey = j[XorStr("Player")][XorStr("clearChatKey")]);
	trycatch(this->player.bCommands = j[XorStr("Player")][XorStr("bCommands")]);
	trycatch(buffer = j[XorStr("Player")][XorStr("commandSymbol")]);
	strcpy_s(this->player.commandSymbol, buffer.c_str());
	trycatch(this->player.bMouseTeleport = j[XorStr("Player")][XorStr("bMouseTeleport")]);
	trycatch(this->player.mouseTpKey = j[XorStr("Player")][XorStr("mouseTpKey")]);
	trycatch(this->player.bAntiTag = j[XorStr("Player")][XorStr("bAntiTag")]);
	trycatch(this->player.antiTagKey = j[XorStr("Player")][XorStr("antiTagKey")]);
	trycatch(this->player.send2SpaceKey = j[XorStr("Player")][XorStr("send2SpaceKey")]);
	trycatch(this->player.bAntiWater = j[XorStr("Player")][XorStr("bAntiWater")]);
	trycatch(this->player.antiWaterKey = j[XorStr("Player")][XorStr("antiWaterKey")]);
	trycatch(this->player.bTileDriveTp = j[XorStr("Player")][XorStr("bTileDriveTp")]);
	trycatch(this->player.tileDriveTpKey = j[XorStr("Player")][XorStr("tileDriveTpKey")]);
	trycatch(this->player.bHbsTp = j[XorStr("Player")][XorStr("bHatTeleport")]);
	trycatch(this->player.hbsTpKey = j[XorStr("Player")][XorStr("hatTeleportKey")]);
	trycatch(this->player.bAntiKnockback = j[XorStr("Player")][XorStr("bAntiKnockback")]);
	trycatch(this->player.bSuperSlide = j[XorStr("Player")][XorStr("bSuperSlide")]);
	trycatch(this->player.superSlideKey = j[XorStr("Player")][XorStr("superSlideKey")]);
	trycatch(this->player.bInfiniteSlide = j[XorStr("Player")][XorStr("bInfiniteSlide")]);
	trycatch(this->player.infSlideKey = j[XorStr("Player")][XorStr("infSlideKey")]);
	trycatch(this->player.slideForce = j[XorStr("Player")][XorStr("slideForce")]);
	trycatch(this->player.bNoIceSlide = j[XorStr("Player")][XorStr("bNoIceSlide")]);
	trycatch(this->player.nextGameMode = j[XorStr("Player")][XorStr("nextGameMode")]);
	trycatch(this->player.bModGravity = j[XorStr("Player")][XorStr("bLowGravity")]);
	trycatch(this->player.gravity = j[XorStr("Player")][XorStr("gravity")]);
	trycatch(this->player.gravityKey = j[XorStr("Player")][XorStr("gravityKey")]);
	trycatch(this->player.bSuperTireJump = j[XorStr("Player")][XorStr("bSuperTireJump")]);
	trycatch(this->player.superTireJumpKey = j[XorStr("Player")][XorStr("superTireJumpKey")]);
	trycatch(this->player.tireForce = j[XorStr("Player")][XorStr("tireForce")]);
	trycatch(this->player.bNoBoundKill = j[XorStr("Player")][XorStr("bNoBoundKill")]);
	trycatch(this->player.noBoundKillkey = j[XorStr("Player")][XorStr("noBoundKillkey")]);
	trycatch(this->player.bInvisibility = j[XorStr("Player")][XorStr("bInvisibility")]);
	trycatch(this->player.invisibilKey = j[XorStr("Player")][XorStr("invisibilKey")]);
	trycatch(this->player.bPlayerTeleport = j[XorStr("Player")][XorStr("bPlayerTeleport")]);
	trycatch(this->player.playerTpKey = j[XorStr("Player")][XorStr("playerTpKey")]);
	trycatch(this->player.playerTpDist = j[XorStr("Player")][XorStr("playerTpDist")]);
	trycatch(this->player.suicideKey = j[XorStr("Player")][XorStr("suicideKey")]);
	trycatch(this->player.freezeTimerKey = j[XorStr("Player")][XorStr("freezeTimerKey")]);
	trycatch(this->player.bNoGlassBreak = j[XorStr("Player")][XorStr("bNoGlassBreak")]);
	trycatch(this->player.bTrollButtonScreen = j[XorStr("Player")][XorStr("bTrollButtonScreen")]);
	trycatch(this->player.bOnlyRiggedButtons = j[XorStr("Player")][XorStr("bOnlyRiggedButtons")]);

	// Visuals
	trycatch(this->visuals.vp.key = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("key")]);
	trycatch(this->visuals.vp.bESP = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bPlayerESP")]);
	trycatch(this->visuals.vp.bBoxes = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bBoxes")]);
	trycatch(this->visuals.vp.bSnaplines = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bSnaplines")]);
	trycatch(this->visuals.vp.BoxType = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Box Type")]);
	trycatch(this->visuals.vp.bBoxRGB = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bBoxRGB")]);
	trycatch(this->visuals.vp.BoxLineWidth = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Box Width")]);
	trycatch(this->visuals.vp.SnapType = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Snapline Type")]);
	trycatch(this->visuals.vp.bSnaplineRGB = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bSnaplineRGB")]);
	trycatch(this->visuals.vp.SnaplineLineWidth = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Snapline Width")]);
	trycatch(this->visuals.vp.BoxColor[0] = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Box Color")][XorStr("x")]);
	trycatch(this->visuals.vp.BoxColor[1] = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Box Color")][XorStr("y")]);
	trycatch(this->visuals.vp.BoxColor[2] = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Box Color")][XorStr("z")]);
	trycatch(this->visuals.vp.BoxColor[3] = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Box Color")][XorStr("w")]);
	trycatch(this->visuals.vp.SnaplineColor[0] = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Snapline Color")][XorStr("x")]);
	trycatch(this->visuals.vp.SnaplineColor[1] = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Snapline Color")][XorStr("y")]);
	trycatch(this->visuals.vp.SnaplineColor[2] = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Snapline Color")][XorStr("z")]);
	trycatch(this->visuals.vp.SnaplineColor[3] = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Snapline Color")][XorStr("w")]);
	trycatch(this->visuals.vp.RGBSpeed = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("RGB Speed")]);
	trycatch(this->visuals.vp.bNameESP = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bNameESP")]);
	trycatch(this->visuals.vp.bLimitDistance = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bLimitDistance")]);
	trycatch(this->visuals.vp.maxDistance = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("maxDistance")]);
	trycatch(this->visuals.vp.bEnemiesOnly = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bEnemiesOnly")]);
	trycatch(this->visuals.vp.bChams = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bChams")]);
	trycatch(this->visuals.vp.bChamRGB = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bChamRGB")]);
	trycatch(this->visuals.vp.ChamColor[0] = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Cham Color")][XorStr("x")]);
	trycatch(this->visuals.vp.ChamColor[1] = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Cham Color")][XorStr("y")]);
	trycatch(this->visuals.vp.ChamColor[2] = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Cham Color")][XorStr("z")]);
	trycatch(this->visuals.vp.ChamColor[3] = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("Cham Color")][XorStr("w")]);

	trycatch(this->visuals.bWeaponESP = j[XorStr("Visuals")][XorStr("Weapon ESP")][XorStr("bWeaponESP")]);
	trycatch(this->visuals.bWeaponRGB = j[XorStr("Visuals")][XorStr("Weapon ESP")][XorStr("bWeaponRGB")]);
	trycatch(this->visuals.WeaponColor[0] = j[XorStr("Visuals")][XorStr("Weapon ESP")][XorStr("Color")][XorStr("x")]);
	trycatch(this->visuals.WeaponColor[1] = j[XorStr("Visuals")][XorStr("Weapon ESP")][XorStr("Color")][XorStr("y")]);
	trycatch(this->visuals.WeaponColor[2] = j[XorStr("Visuals")][XorStr("Weapon ESP")][XorStr("Color")][XorStr("z")]);
	trycatch(this->visuals.WeaponColor[3] = j[XorStr("Visuals")][XorStr("Weapon ESP")][XorStr("Color")][XorStr("w")]);

	trycatch(this->visuals.GlassESP = j[XorStr("Visuals")][XorStr("Glass ESP")][XorStr("bEnabled")]);
	trycatch(this->visuals.GlassColor[0] = j[XorStr("Visuals")][XorStr("Glass ESP")][XorStr("Color")][XorStr("x")]);
	trycatch(this->visuals.GlassColor[1] = j[XorStr("Visuals")][XorStr("Glass ESP")][XorStr("Color")][XorStr("y")]);
	trycatch(this->visuals.GlassColor[2] = j[XorStr("Visuals")][XorStr("Glass ESP")][XorStr("Color")][XorStr("z")]);
	trycatch(this->visuals.GlassColor[3] = j[XorStr("Visuals")][XorStr("Glass ESP")][XorStr("Color")][XorStr("w")]);
	trycatch(this->visuals.bRGBGlass = j[XorStr("Visuals")][XorStr("Glass ESP")][XorStr("bRGB")]);

	trycatch(this->visuals.ButtonESP = j[XorStr("Visuals")][XorStr("bBustlingButtonESP")]);
	trycatch(this->visuals.bFOV = j[XorStr("Visuals")][XorStr("bFOV")]);
	trycatch(this->visuals.FOV = j[XorStr("Visuals")][XorStr("FOV")]);
	trycatch(this->visuals.bDormFullBright = j[XorStr("Visuals")][XorStr("bDormFullBright")]);
	trycatch(this->visuals.bBombTimer = j[XorStr("Visuals")][XorStr("bBombTimer")]);
	trycatch(this->visuals.bZoom = j[XorStr("Visuals")][XorStr("bZoom")]);
	trycatch(this->visuals.zoomKey = j[XorStr("Visuals")][XorStr("zoomKey")]);
	trycatch(this->visuals.zoomFOV = j[XorStr("Visuals")][XorStr("zoomFOV")]);

	// Aim Assist
	trycatch(this->aim.bAim = j[XorStr("Aim Assist")][XorStr("bAim")]);
	trycatch(this->aim.bAimKey = j[XorStr("Aim Assist")][XorStr("bAimKey")]);
	trycatch(this->aim.AimKey = j[XorStr("Aim Assist")][XorStr("Aim Key")]);
	trycatch(this->aim.Smoothness = j[XorStr("Aim Assist")][XorStr("Smoothness")]);
	trycatch(this->aim.bFOV = j[XorStr("Aim Assist")][XorStr("bFOV")]);
	trycatch(this->aim.FOV = j[XorStr("Aim Assist")][XorStr("FOV")]);
	trycatch(this->aim.bFOVCircle = j[XorStr("Aim Assist")][XorStr("FOV Circle")][XorStr("bEnabled")]);
	trycatch(this->aim.FOVCircleColor[0] = j[XorStr("Aim Assist")][XorStr("FOV Circle")][XorStr("x")]);
	trycatch(this->aim.FOVCircleColor[1] = j[XorStr("Aim Assist")][XorStr("FOV Circle")][XorStr("y")]);
	trycatch(this->aim.FOVCircleColor[2] = j[XorStr("Aim Assist")][XorStr("FOV Circle")][XorStr("z")]);
	trycatch(this->aim.FOVCircleColor[3] = j[XorStr("Aim Assist")][XorStr("FOV Circle")][XorStr("w")]);
	trycatch(this->aim.bNoRecoil = j[XorStr("Aim Assist")][XorStr("bNoRecoil")]);
	trycatch(this->aim.recoilSlider = j[XorStr("Aim Assist")][XorStr("recoilSlider")]);
	trycatch(this->aim.bHoldToShoot = j[XorStr("Aim Assist")][XorStr("bHoldToShoot")]);
	trycatch(this->aim.bSpinBot = j[XorStr("Aim Assist")][XorStr("bSpinBot")]);
	trycatch(this->aim.spinBotKey = j[XorStr("Aim Assist")][XorStr("spinBotKey")]);
	trycatch(this->aim.spinBotSpeed = j[XorStr("Aim Assist")][XorStr("spinBotSpeed")]);
	trycatch(this->aim.bSilentAim = j[XorStr("Aim Assist")][XorStr("bSilentAim")]);
	trycatch(this->aim.silentAimKey = j[XorStr("Aim Assist")][XorStr("silentAimKey")]);
	trycatch(this->aim.bRapidFire = j[XorStr("Aim Assist")][XorStr("bRapidFire")]);

	// Tiggerbot
	trycatch(this->aim.trg.bTrg = j[XorStr("Triggerbot")][XorStr("bTriggerbot")]);
	trycatch(this->aim.trg.bTrgKey = j[XorStr("Triggerbot")][XorStr("bTriggerbotKey")]);
	trycatch(this->aim.trg.TrgKey = j[XorStr("Triggerbot")][XorStr("Triggerbot Key")]);
	trycatch(this->aim.trg.bLimitDistance = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("bLimitDistance")]);
	trycatch(this->aim.trg.maxDistance = j[XorStr("Visuals")][XorStr("Player ESP")][XorStr("maxDistance")]);

	// Menu
	trycatch(Menu::bLockInPlace = j[XorStr("Menu")][XorStr("Lock in Place")]);
	trycatch(Menu::ToggleKey = j[XorStr("Menu")][XorStr("Toggle Key")]);
	trycatch(Menu::CloseKey = j[XorStr("Menu")][XorStr("Close Key")]);
	trycatch(this->bMenuRGB = j[XorStr("Menu")][XorStr("bMenuRGB")]);
	trycatch(this->MenuRGBSpeed = j[XorStr("Menu")][XorStr("MenuRGBSpeed")]);
	trycatch(this->MenuColor[0] = j[XorStr("Menu")][XorStr("MenuColor")][XorStr("x")]);
	trycatch(this->MenuColor[1] = j[XorStr("Menu")][XorStr("MenuColor")][XorStr("y")]);
	trycatch(this->MenuColor[2] = j[XorStr("Menu")][XorStr("MenuColor")][XorStr("z")]);
	trycatch(this->MenuRounding = j[XorStr("Menu")][XorStr("MenuRounding")]);
	trycatch(this->bAutoSave = j[XorStr("Menu")][XorStr("bAutoSave")]);
	trycatch(this->bFreeMouseOnPopup = j[XorStr("Menu")][XorStr("bFreeMouseOnPopup")]);
	trycatch(this->script.bGUISaveOnClose = j[XorStr("Menu")][XorStr("bGUISaveOnClose")]);
	trycatch(this->script.bOpenedFolder = j[XorStr("Menu")][XorStr("bOpenedScriptsFolder")]);
	buffer = XorCStr("");
	trycatch(buffer = j[XorStr("Menu")][XorStr("Scripts Folder")]);
	std::wstring wbuffer = std::wstring(buffer.begin(), buffer.end());
	wcscpy_s(this->script.OpenedFolder, wbuffer.c_str());

	// Script settings
	if (cfg.script.bOpenedFolder)
	{
		Menu::RefreshScripts();

		try
		{
			json hotkeys = j[XorStr("Menu")][XorStr("Script Hotkeys")];
			for (USHORT i = 0; i < cfg.script.files.size(); i++)
			{
				auto script = &cfg.script.files[i];
				try
				{
					DWORD key = hotkeys[script->path.filename().string()];
					script->hotkey = key;
				} catch (...) {}
			}
		}
		catch (...) {}
	}


#undef trycatch
}

void cfg_t::LoadCfg()
{
	wchar_t FilePath[MAX_PATH];
	OPENFILENAMEW dialog = { sizeof(OPENFILENAME) };
	dialog.lpstrFile = FilePath;
	dialog.nMaxFile = MAX_PATH;
	dialog.lpstrFilter = XorCStr(L"1hAck Config File (*.1hCG)\0*.1hCG\0\0");
	dialog.lpstrTitle = XorCStr(L"Load Config");
	dialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameW(&dialog))
		Load(FilePath);
}

void cfg_t::Init()
{
	WIN32_FIND_DATAW data;
	if (FindFirstFileW(XorCStr(L"*.1hCG"), &data) != INVALID_HANDLE_VALUE)
		Load(data.cFileName);
}
