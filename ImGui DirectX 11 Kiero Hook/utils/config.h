#pragma once
#include <Windows.h>
#include <filesystem>
#include <memory>
#include <queue>
#include "../imgui/imgui.h"
#include "../imgui/TextEditor.h"

extern "C"
{
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lua.hpp"
#pragma comment(lib, "lua54.lib")
}

namespace Client
{
	extern bool bInGame;
	extern bool bCheckInGame;
	extern POINT WindowSize;
	extern POINT WindowCenter;
	extern HWND hwnd;
	extern HDC hdc;
	extern RECT WindowRect;

	void Init();
}

struct trg_t
{
	bool bTrg = false;
	bool bTrgKey = true;
	USHORT TrgKey = 0xADAF; // NONE

	bool bLimitDistance = false;
	float maxDistance = 50.f;
};

struct aim_t
{
	bool bAim = false;
	bool bAimKey = true;
	USHORT AimKey = VK_MENU; // ALT

	float Smoothness = 2.f;

	bool bFOV = true;
	int FOV = 400;
	bool bFOVCircle = true;
	float FOVCircleColor[4] = { 1.f, 0, 0, 1.f };

	trg_t trg;

	bool bNoRecoil = false;
	float recoilSlider = 1.f;

	bool bHoldToShoot = false;

	bool bSpinBot = false;
	USHORT spinBotKey = 0xADAF; // NONE
	float spinBotSpeed = 5.f;
	float ySpinProcess = 0.f;

	bool bSilentAim = false;
	USHORT silentAimKey = 0xADAF; // NONE

	bool bRapidFire = false;
};

struct vis_player_t
{
	USHORT key = 0xADAF; // NONE

	bool bESP = false;
	bool bBoxes = true;
	bool bSnaplines = false;

	int BoxType = 0;
	bool bBoxRGB = false;
	float BoxLineWidth = 2.f;

	int SnapType = 0;
	bool bSnaplineRGB = false;
	float SnaplineLineWidth = 2.f;

	bool bNameESP = false;

	bool bLimitDistance = false;
	float maxDistance = 100.f;

	float RGBSpeed = 0.2f;
	float RGB[3] = { 255, 0, 0 };
	ImColor RGBColor = ImColor(255, 0, 0);
	float BoxColor[4] = { 1.f, 0, 0, 1.f };
	float SnaplineColor[4] = {1.f, 0, 0, 1.f };

	bool bEnemiesOnly = false;

	bool bChams = false;
	float ChamColor[4] = { 1.f, 0, 1.f, 1.f };
	bool bChamRGB = false;
};

struct visuals_t
{
	vis_player_t vp;

	bool GlassESP = false;
	float GlassColor[4] = { 1.f, 0, 0, 1.f };
	bool bRGBGlass = false;

	bool ButtonESP = false;

	bool bFOV = false;
	int FOV = 120;

	bool bDormFullBright = false;

	bool bBombTimer = false;
	float bombTime = -20.f;

	bool bZoom = false;
	USHORT zoomKey = 0xADAF; // NONE
	int zoomFOV = 40;

	bool bWeaponESP = false;
	float WeaponColor[4] = { 1.f, 0, 0, 1.f };
	bool bWeaponRGB = false;

};

struct player_t
{
	bool bFlyhack = false;
	USHORT flyKey = 0xADAF; // NONE
	float flySpeed = 5000.f;

	bool bBunnyhop = false;
	USHORT bunKey = VK_SPACE; // SPACE

	bool bNoClip = false;
	bool bSetNoClip = false;
	USHORT noClipKey = 0xADAF; // NONE

	bool bAntiPunch = false;
	float antiForce = 1.f;

	bool bSuperPunch = false;
	USHORT superPunchKey = 0xADAF; // NONes
	float punchMultiplier = 10.f;

	bool bNextGame = false, bStartGame = false;

	int nextMap = 0;
	int nextGameMode = 0;

	char message[80] = "1hAck";
	int messageAmount = 5;
	int messageCounter = 0;

	bool bGodMode = false;
	USHORT godModeKey = 0xADAF; // NONE

	bool bSpeedHack = false;
	float Speed = 16.f;
	USHORT speedKey = 0xADAF; // NONE
	
	bool bSuperJump = false;
	USHORT superJumpKey = 0xADAF; // NONE
	float jumpForce = 800.f;

	bool bInfiniteJump = false;
	USHORT infJumpKey = 0xADAF; // NONE

	bool bNoCameraShake = false;

	bool ForceGiveWeapons = false;
	int weaponId = 0;
	bool bSpawnWeaponAsHost = false;

	bool bInfiniteAmmo = false;

	bool bAntiVoid = false;
	USHORT antiVoidKey = 0xADAF;

	USHORT glassBrealKey = 0xADAF; // NONE
	bool bBreakGlass = false;

	bool bRedLightGodmode = false;
	bool bRLGLBot = false;
	USHORT rlglBotKey = 0xADAF; // NONE

	bool bFarReach = false;
	USHORT farReachKey = 0xADAF; // NONE

	bool bClearChat = false;
	bool bAutoClsChat = false;
	USHORT clearChatKey = 0xADAF; // NONE

	bool bCommands = false;
	char commandSymbol[4] = "!";

	bool bMouseTeleport = false;
	USHORT mouseTpKey = 0xADAF; // NONE

	bool bAntiTag = false;
	USHORT antiTagKey = 0xADAF; // NONE

	bool bSend2Space = false;
	USHORT send2SpaceKey = 0xADAF; // NONE

	bool bAntiWater = false;
	USHORT antiWaterKey = 0xADAF; // NONE

	bool bTileDriveTp = false;
	USHORT tileDriveTpKey = 0xADAF; // NONE

	bool bHbsTp = false;
	USHORT hbsTpKey = 0xADAF; // NONE
	bool bAutoHBStp = false;

	bool bAntiKnockback = false;

	bool bSuperSlide = false;
	USHORT superSlideKey = 0xADAF; // NONE
	float slideForce = 50000.f;

	bool bInfiniteSlide = false;
	USHORT infSlideKey = 0xADAF; // NONE

	bool bNoIceSlide = false;

	bool bModGravity = false;
	float gravity = 500.f;
	USHORT gravityKey = 0xADAF; // NONE

	bool bSuperTireJump = false;
	USHORT superTireJumpKey = 0xADAF; // NONE
	float tireForce = 50.f;

	bool bNoBoundKill = false;
	USHORT noBoundKillkey = 0xADAF; // NONE

	bool bInvisibility = false;
	USHORT invisibilKey = 0xADAF; // NONE

	bool bPlayerTeleport = false;
	USHORT playerTpKey = 0xADAF; // NONE
	float playerTpDist = 3.f;

	bool bSuicide = false;
	USHORT suicideKey = 0xADAF; // NONE

	bool bFreezeTimer = false;
	USHORT freezeTimerKey = 0xADAF; // NONE

	bool bRespawn = false;

	bool bFreezePlayers = false;
	int frozenPlayer = 0;

	bool bKillPlayers = false;
	int playerToKill = 0;

	bool bNoGlassBreak = false;

	bool bKillCurrentButtonPresser = false;
	bool bSaveCurrentButtonPresser = false;
	bool bTrollButtonScreen = false;
	bool bOnlyRiggedButtons = false;

	bool bTpAll = false;
};

enum ScriptFlags_
{
	ScriptFlags_HasRender = 1 << 1,
	ScriptFlags_HasUpdate = 1 << 2,
	ScriptFlags_StatusChanged = 1 << 3,
};

struct ScriptFile
{
	bool bEnabled = false;
	std::filesystem::path path;
	USHORT hotkey = 0xADAF; // NONE
	bool bGetKey = false;

	lua_State* lua = nullptr;
	byte flags = 0;

	// for scripting
	bool Init();
	void Uninit();

	bool DoesFuncnExist(const char* funcName);

	bool CallFunc(const char* funcName);

	const char* GetLastError();
};

struct scripts_t
{
	bool bScriptsWindow = false;
	bool bScriptEditor = false;
	bool bInitedEditor = false;
	TextEditor scriptEditor = {};

	bool bOpenedFolder = false;
	wchar_t OpenedFolder[MAX_PATH] = { 0 };

	std::vector<ScriptFile> files = {};

	bool bNewScript = false;
	char currentScriptName[100] = { 0 };
	std::unique_ptr<ScriptFile> currentScipt = nullptr;

	ScriptFile* GUIScript = nullptr;

	bool bGUISaveOnClose = true;
};

struct cfg_t
{
	aim_t aim;
	visuals_t visuals;
	player_t player;
	scripts_t script;

	float MenuColor[4] = { 1.f, 0.1f, 0.1f, 0.4f };
	float MenuRGB[3] = { 255, 0, 0 };
	bool bMenuRGB = false;
	float MenuRGBSpeed = 1.f;
	float MenuRounding = 0.f;

	bool bAutoSave = false;
	bool bFreeMouseOnPopup = false;

	void Init();
	void Save(const wchar_t* const fileName);
	void SaveCfg();
	void Load(const wchar_t* const fileName);
	void LoadCfg();
};
inline cfg_t cfg;
