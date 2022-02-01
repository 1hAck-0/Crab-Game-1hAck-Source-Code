#include "pch.h"
#include "cheat.h"
#include "../includes.h"
#include "../utils/config.h"
#include "../utils/cgdata.h"
#include "hooks.h"

#include <vector>
#include <fstream>
#include <thread>
#include "../menu/lua-scripts/lua_handler.h"

#define debug(format, x) char buffer[15]; sprintf_s(buffer, XorStr(format).c_str(), x); MessageBoxA(0, buffer, XorStr("Address Debugger").c_str(), 0)
#define M_PI 3.14159265359f

std::string GetTimeString()
{
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[100];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
	return buffer;
}

void Log(const std::string& data)
{
	std::ofstream file;
	file.open(XorStr("1hAck Log.txt"), std::ios::app);
	file << XorStr(" [") << GetTimeString() << XorStr("]\t") << data << std::endl;
	file.close();
}

namespace Cheat
{
	using namespace UnityEngine;

	// VISUAL VARS
	uint64_t lastTimeRan = 0;
	std::vector<Ent> players;
	std::vector<std::string> playerNames;
	std::vector<vec3> glassEspPosEx = {};
	std::vector<vec2> glassEspEx = {};
	vec2 dngButton = { -1.f, 0 };
	bool bShowCmdHelp = false;
	bool bShowItemHelp = false;

	// UPDATE VARS
	generic_array_t<Component*>* activePlayers;
	generic_array_t<Component*>* glassPieces;
	generic_array_t<Component*>* bustlingButtons;
	generic_array_t<Component*>* gameModeButton;
	generic_array_t<Component*>* Chatbox;

	Component* playerMovement;
	Camera* camera;
	Component* chat;
	uintptr_t hostId;

	generic_list_t<int>* realPieces;

	bool bShowPlayers = false;

	// AIM VARS
	vec3 closest;
	float dst = 0;
	float closestDst = 99999.f;
	POINT mouse{};

	// SOME STUFF
	bool bTyping = false;
	HWND activeHNWD;






	// UPDATE STUFF
	void UpdatePointers()
	{
		// PLAYERMOVEMENT
		generic_array* PlayerMovement = il2cpp::find_objects_of_type(XorStr("PlayerMovement"), XorStr("Assembly-CSharp"));
		if (PlayerMovement && PlayerMovement->length)
			playerMovement = (Component*)PlayerMovement->items[0];
		else
			playerMovement = 0;

		// IS TYPING
		static const uintptr_t Chatbox_get_typing = il2cpp::get_method(XorStr("Assembly-CSharp"),
			XorStr("System.Boolean"),
			XorStr(""),
			XorStr("Chatbox"),
			XorStr("get_typing"),
			XorStr(""));

		Chatbox = (generic_array_t<Component*>*)il2cpp::find_objects_of_type(XorStr("Chatbox"), XorStr("Assembly-CSharp"));

		// HOST ID
		hostId = getHostId();

		if (Chatbox && Chatbox->length)
		{
			chat = Chatbox->items[0];

			if (reinterpret_cast<bool(*)(Component* chatbox)>(Chatbox_get_typing)(Chatbox->items[0]))
				bTyping = true;
			else
				bTyping = false;
		}
		else
			bTyping = false, chat = 0;

		// CAMERA
		generic_array* MoveCamera = il2cpp::find_objects_of_type(XorStr("MoveCamera"), XorStr("Assembly-CSharp"));
		if (MoveCamera && MoveCamera->length && MoveCamera->items[0])
			camera = *(Camera**)(MoveCamera->items[0] + off::MoveCamera::mainCam);

		// BOMB TIMER
		if (cfg.visuals.bBombTimer)
		{
			generic_array* GameModeBombTag = il2cpp::find_objects_of_type(XorStr("GameModeBombTag"), XorStr("Assembly-CSharp"));
			if (GameModeBombTag && GameModeBombTag->length)
			{
				cfg.visuals.bombTime = *(float*)(*(uintptr_t*)(GameModeBombTag->items[0] + 0x50) + 0x10);
			}
			else
				cfg.visuals.bombTime = -20.f;
		}

		// UPDATE ESP PLAYERS
		players.clear();
		activePlayers = (generic_array_t<Component*>*)il2cpp::find_objects_of_type(XorStr("OnlinePlayerMovement"), XorStr("Assembly-CSharp"));
		if (activePlayers && activePlayers->length)
		{
			generic_list_t<uintptr_t>* tagged = nullptr;
			int gameMode = 0;
			bool amItagged = false;
			if (cfg.visuals.vp.bEnemiesOnly)
			{
				switch (0)
				{
				case 0:
					generic_array * GameMode = il2cpp::find_objects_of_type(XorStr("GameModeTag"), XorStr("Assembly-CSharp"));

					if (GameMode && GameMode->length)
					{
						tagged = *(generic_list_t<uintptr_t>**)(GameMode->items[0] + 0x38);
						gameMode = 4;
						break;
					}

					GameMode = il2cpp::find_objects_of_type(XorStr("GameModeHnS"), XorStr("Assembly-CSharp"));

					if (GameMode && GameMode->length)
					{
						tagged = *(generic_list_t<uintptr_t>**)(GameMode->items[0] + 0x38);
						gameMode = 5;
						break;
					}
					GameMode = il2cpp::find_objects_of_type(XorStr("GameModeBombTag"), XorStr("Assembly-CSharp"));

					if (GameMode && GameMode->length)
					{
						tagged = *(generic_list_t<uintptr_t>**)(GameMode->items[0] + 0x48);
						gameMode = 6;
						break;
					}

					GameMode = il2cpp::find_objects_of_type(XorStr("GameModeHat"), XorStr("Assembly-CSharp"));

					if (GameMode && GameMode->length)
					{
						tagged = *(generic_list_t<uintptr_t>**)(GameMode->items[0] + 0x38);
						gameMode = 10;
					}
				}
				
				if (tagged)
				{
					for (int i = 0; i < tagged->size; i++)
					{
						if (getLocalPlayerId() == tagged->items->items[i])
						{
							amItagged = true;
							break;
						}
					}
				}
			}

			for (USHORT i = 0; i < activePlayers->length; i++)
			{
				Component*& player = activePlayers->items[i];

				if (cfg.visuals.vp.bEnemiesOnly && tagged && tagged->size)
				{
					bool bCont = false;
					const uintptr_t pId = getPlayerId(player);

					if (amItagged)
					{
						for (int j = 0; j < tagged->size; j++)
						{
							if (pId == tagged->items->items[j])
							{
								bCont = true;
								break;
							}
						}
					}
					else
					{
						bool bFound = false;
						for (int j = 0; j < tagged->size; j++)
						{
							if (pId == tagged->items->items[j])
							{
								bFound = true;
								break;
							}
						}
						bCont = !bFound;
					}

					if (bCont)
						continue;
				}

				auto bones = player->GetComponentsInChildren<Transform>(XorStr("UnityEngine.Transform, UnityEngine.CoreModule"));
				if (!bones || bones->length <= 59)
					continue;

				vec3 WFeetPos = bones->items[59]->get_position();
				if (cfg.visuals.vp.maxDistance && cfg.visuals.vp.maxDistance < vec3::Distance(getLocalPos(), WFeetPos))
					continue;

				// FEET
				vec3 feetPos = camera->WorldToScreenPoint(WFeetPos);
				if (feetPos.z < 0.3f)
					continue;
				feetPos.y = Client::WindowSize.y - feetPos.y;

				// HEAD
				vec3 headPos = camera->WorldToScreenPoint(bones->items[30]->get_position());
				if (headPos.z < 0.3f)
					continue;
				headPos.y = Client::WindowSize.y - headPos.y;

				// 3D BOX
				if (cfg.visuals.vp.BoxType == 2)
				{
					static constexpr const vec4 scaling = { 1.f, 1.f, 0.07f, 0.15f };

					float y_rot = *(float*)(player + off::OnlinePlayerMovement::yRot);

					vec3 b1, b2, b3, b4, t1, t2, t3, t4;
					b1.y = b2.y = b3.y = b4.y = WFeetPos.y + scaling.w;

					b1.x = WFeetPos.x - cosf(TORAD(y_rot + 45)) * scaling.x;
					b1.z = WFeetPos.z + sinf(TORAD(y_rot + 45)) * scaling.y;

					b2.x = WFeetPos.x - cosf(TORAD(y_rot + 135)) * scaling.x;
					b2.z = WFeetPos.z + sinf(TORAD(y_rot + 135)) * scaling.y;

					b3.x = WFeetPos.x - cosf(TORAD(y_rot + 225)) * scaling.x;
					b3.z = WFeetPos.z + sinf(TORAD(y_rot + 225)) * scaling.y;

					b4.x = WFeetPos.x - cosf(TORAD(y_rot + 315)) * scaling.x;
					b4.z = WFeetPos.z + sinf(TORAD(y_rot + 315)) * scaling.y;

					t1 = b1;
					t2 = b2;
					t3 = b3;
					t4 = b4;
					t1.y = t2.y = t3.y = t4.y = bones->items[30]->get_position().y + scaling.z;

					b1 = camera->WorldToScreenPoint(b1);
					b2 = camera->WorldToScreenPoint(b2);
					b3 = camera->WorldToScreenPoint(b3);
					b4 = camera->WorldToScreenPoint(b4);

					if (b1.z > 0.2f && b2.z > 0.2f && b3.z > 0.2f && b4.z > 0.2f)
					{
						b1.y = Client::WindowSize.y - b1.y;
						b2.y = Client::WindowSize.y - b2.y;
						b3.y = Client::WindowSize.y - b3.y;
						b4.y = Client::WindowSize.y - b4.y;

						t1 = camera->WorldToScreenPoint(t1);
						t2 = camera->WorldToScreenPoint(t2);
						t3 = camera->WorldToScreenPoint(t3);
						t4 = camera->WorldToScreenPoint(t4);

						t1.y = Client::WindowSize.y - t1.y;
						t2.y = Client::WindowSize.y - t2.y;
						t3.y = Client::WindowSize.y - t3.y;
						t4.y = Client::WindowSize.y - t4.y;

						if (t1.z > 0.2f && t2.z > 0.2f && t3.z > 0.2f && t4.z > 0.2f)
							players.push_back({ (cfg.visuals.vp.bNameESP ? getPlayerName(player) : ""), headPos, feetPos, b1, b2, b3, b4, t1, t2, t3, t4 });
					}
				}
				else
					players.push_back({ (cfg.visuals.vp.bNameESP ? getPlayerName(player) : ""), headPos, feetPos });

				// CHAM
				if (cfg.visuals.vp.bChams)
				{
					Cham(player, (cfg.visuals.vp.bChamRGB ? *(Color*)&cfg.visuals.vp.RGBColor.Value : *(Color*)&cfg.visuals.vp.ChamColor));
				}
			}
		}

		// UPDATE PLAYER NAMES for PLAYER LIST
		playerNames.clear();
		for (USHORT i = 0; i < activePlayers->length; i++)
			playerNames.push_back(getPlayerName(activePlayers->items[i]));

		// GLASS CHAMS
		glassPieces = (generic_array_t<Component*>*)il2cpp::find_objects_of_type(XorStr("GlassBreak"), XorStr("Assembly-CSharp"));
		if (cfg.visuals.GlassESP && glassPieces)
		{
			for (byte i = 0; i < glassPieces->length; i++)
			{
				Cham(glassPieces->items[i], (cfg.visuals.bRGBGlass ? *(Color*)&cfg.visuals.vp.RGBColor.Value : *(Color*)&cfg.visuals.GlassColor));
			}
		}

		// WEAPON CHAMS
		if (cfg.visuals.bWeaponESP)
		{
			auto weapons = (generic_array_t<Component*>*)il2cpp::find_objects_of_type(XorStr("ItemPrefab"), XorStr("Assembly-CSharp"));

			if (weapons)
			{
				for (int i = 0; i < weapons->length; i++)
				{
					Cham(weapons->items[i], (cfg.visuals.bWeaponRGB ? *(Color*)&cfg.visuals.vp.RGBColor.Value : *(Color*)&cfg.visuals.WeaponColor));
				}
			}
		}

		// BUSTLING BUTTON
		dngButton.x = -1.f;
		if (cfg.visuals.ButtonESP)
		{
			// get bustling buttons
			bustlingButtons = (generic_array_t<Component*>*)il2cpp::find_objects_of_type(XorStr("BustlingButtonInteract"), XorStr("Assembly-CSharp"));
			gameModeButton = (generic_array_t<Component*>*)il2cpp::find_objects_of_type(XorStr("GameModeButtons"), XorStr("Assembly-CSharp"));

			if (gameModeButton && gameModeButton->length && bustlingButtons)
			{
				int dangerousButton = *(int*)(gameModeButton->items[0] + off::GameModeButtons::dangerousButton);

				for (int i = 0; i < bustlingButtons->length; i++)
				{
					if (dangerousButton != *(int*)(bustlingButtons->items[i] + off::BustlingButtonInteract::buttonId))
						continue;

					vec3 screenPos = camera->WorldToScreenPoint(bustlingButtons->items[i]->get_transform()->get_position());

					if (screenPos.z < 0.3f)
						continue;
					screenPos.y = Client::WindowSize.y - screenPos.y;

					dngButton = screenPos;
					break;
				}
			}
		}
	}

	// AIM ASSIST + TRIGGERBOT
	void RunAim()
	{
		static const Il2CppClass* opm = il2cpp::_inner_get_class(XorStr("Assembly-CSharp"), XorStr(""), XorStr("OnlinePlayerMovement"));
		static const std::string opmType = std::string(opm->name) + XorStr(", Assembly-CSharp");

		// AIM ASSIST
		if (cfg.aim.bAim && (!cfg.aim.bAimKey || (cfg.aim.AimKey != 0xADAF && GetAsyncKeyState(cfg.aim.AimKey) & 0x8000)))
		{
			#define MAX_FLOAT 9999999999999.f

			// FIND CLOSEST ENEMY
			closestDst = 9999999999999.f;
			bool bFound = false;
			const vec3 origin = getLocalPos();
			for (int i = 0; i < activePlayers->length; i++)
			{
				auto bones = activePlayers->items[i]->GetComponentsInChildren<Transform>(XorStr("UnityEngine.Transform, UnityEngine.CoreModule"));
				if (!bones || bones->length <= 30)
					continue;

				vec3 headPos = camera->WorldToScreenPoint(bones->items[30]->get_position());

				// IS BEHIND SCREEN CHECK
				if (headPos.z < 0.3f)
					continue;
				headPos.y = Client::WindowSize.y - headPos.y;

				// FOV CHECK
				dst = sqrt((headPos.x - mouse.x) * (headPos.x - mouse.x) + (headPos.y - mouse.y) * (headPos.y - mouse.y));
				if (cfg.aim.bFOV && cfg.aim.FOV < dst)
					continue;

				// IS CLOSES ENEMY
				if (dst < closestDst)
				{
					closestDst = dst;
					closest = bones->items[30]->get_position();
					bFound = true;
				}
			}

			if (bFound)
				aimAt(closest, cfg.aim.Smoothness);
		}

		// TRIGGERBOT
		if (cfg.aim.trg.bTrg && (!cfg.aim.trg.bTrgKey || (cfg.aim.trg.TrgKey != 0xADAF && GetAsyncKeyState(cfg.aim.trg.TrgKey) & 0x8000)))
		{
			// FIND NEW TARGET
			Ray ray = camera->ScreenPointToRay({ (float)Client::WindowCenter.x, (float)Client::WindowCenter.y });

			RaycastHit hit;
			if (Physics::Raycast(ray, hit, cfg.aim.trg.bLimitDistance ? cfg.aim.trg.maxDistance : 100000.f))
			{
				if (hit.get_collider()->GetComponentInParent(opmType))
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				}
			}
		}

		// RAPID FIRE
		if (cfg.aim.bRapidFire)
		{
			generic_array* objs = il2cpp::find_objects_of_type(XorStr("ItemPrefab"), XorStr("Assembly-CSharp"));
			for (USHORT i = 0; i < objs->length; i++)
				*(bool*)(objs->items[i] + 0x28) = true;
		}

		// SPINBOT
		if (cfg.aim.bSpinBot)
		{
			cfg.aim.ySpinProcess += cfg.aim.spinBotSpeed;
			cfg.aim.ySpinProcess = fmodf(cfg.aim.ySpinProcess, 360.f);
			for (USHORT i = 0; i < activePlayers->length; i++)
			{
				static const uintptr_t ClientSend_PlayerRotation = il2cpp::get_method(XorStr("Assembly-CSharp"),
					XorStr("System.Void"),
					XorStr(""),
					XorStr("ClientSend"),
					XorStr("PlayerRotation"),
					XorStr("System.Single, System.Single, System.UInt64"));

				reinterpret_cast<void(*)(float x, float y, uintptr_t toId)>
					(ClientSend_PlayerRotation)(0, 0, getPlayerId(activePlayers->items[i]));
			}
		}
	}

	// RENDER VISUALS
	void RenderVisuals()
	{
		ImDrawList* const draw = ImGui::GetBackgroundDrawList();

		//char buffer[20];
		//sprintf_s(buffer, "modeTime: %.2f", modeTime);
		//draw->AddText(ImVec2(100, 100), ImColor(255, 0, 0), buffer);

		// FOV CIRCLE
		if (cfg.aim.bAim && cfg.aim.bFOV && cfg.aim.bFOVCircle)
			draw->AddCircle(ImVec2(Client::WindowCenter.x, Client::WindowCenter.y),
				cfg.aim.FOV, ImColor(*(ImVec4*)&cfg.aim.FOVCircleColor), 50, cfg.aim.FOV * 0.2f * 0.02f);

		// RGB
		if ((cfg.visuals.GlassESP || (cfg.visuals.vp.bESP && (cfg.visuals.vp.bBoxes || cfg.visuals.vp.bSnaplines || cfg.visuals.vp.bNameESP || cfg.visuals.vp.bChams || cfg.visuals.bWeaponESP))) &&
			(cfg.visuals.bRGBGlass || cfg.visuals.vp.bBoxRGB || cfg.visuals.vp.bSnaplineRGB || cfg.visuals.vp.bChamRGB || cfg.visuals.bWeaponRGB))
		{
			static bool bStartOfColors = true;
			RGBEffect(bStartOfColors, cfg.visuals.vp.RGBSpeed, cfg.visuals.vp.RGB);
			cfg.visuals.vp.RGBColor = ImColor((int)cfg.visuals.vp.RGB[0], (int)cfg.visuals.vp.RGB[1], (int)cfg.visuals.vp.RGB[2]);
		}

		// PLAYER ESP
		if (cfg.visuals.vp.bESP && (cfg.visuals.vp.bBoxes || cfg.visuals.vp.bSnaplines || cfg.visuals.vp.bNameESP))
		{
			for (unsigned i = 0; i < players.size(); i++)
			{
				const vec2& headPos = players[i].headPos;
				const vec2& feetPos = players[i].feetPos;

				// SNAPLINES
				if (cfg.visuals.vp.bSnaplines)
				{
					switch (cfg.visuals.vp.SnapType)
					{
						// FROM BUTTON TO ORIGIN
					case 0:
						draw->AddLine(ImVec2(Client::WindowCenter.x, Client::WindowSize.y - 5), ImVec2(feetPos.x, feetPos.y),
							(cfg.visuals.vp.bSnaplineRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.SnaplineColor)), cfg.visuals.vp.SnaplineLineWidth);
						break;

						// FROM BUTTON TO HEAD
					case 1:
						draw->AddLine(ImVec2(Client::WindowCenter.x, Client::WindowSize.y - 5), ImVec2(headPos.x, headPos.y),
							(cfg.visuals.vp.bSnaplineRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.SnaplineColor)), cfg.visuals.vp.SnaplineLineWidth);
						break;

						// FROM CENTER TO ORIGIN
					case 2:
						draw->AddLine(ImVec2(Client::WindowCenter.x, Client::WindowCenter.y), ImVec2(feetPos.x, feetPos.y),
							(cfg.visuals.vp.bSnaplineRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.SnaplineColor)), cfg.visuals.vp.SnaplineLineWidth);
						break;

						// FROM CENTER TO HEAD
					case 3:
						draw->AddLine(ImVec2(Client::WindowCenter.x, Client::WindowCenter.y), ImVec2(headPos.x, headPos.y),
							(cfg.visuals.vp.bSnaplineRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.SnaplineColor)), cfg.visuals.vp.SnaplineLineWidth);
						break;
					}
				}

				// BOXES
				if (cfg.visuals.vp.bBoxes)
				{
					switch (cfg.visuals.vp.BoxType)
					{
						// RECTANGLE
					case 0:
					{
						const float scale = (feetPos.y - headPos.y) * 0.2f;
						draw->AddRect(ImVec2(headPos.x - scale, headPos.y), ImVec2(feetPos.x + scale, feetPos.y),
							(cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)), 0, 0,
							cfg.visuals.vp.BoxLineWidth);
					}
					break;

					// CORNER BOX
					case 1:
					{
						const float scale = (feetPos.y - headPos.y) * 0.2f;
						const float corner = scale * 0.6f;

						// TOP LEFT
						draw->AddLine(ImVec2(headPos.x - scale, headPos.y), ImVec2(headPos.x - scale + corner, headPos.y),
							(cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)),
							cfg.visuals.vp.BoxLineWidth);
						draw->AddLine(ImVec2(headPos.x - scale, headPos.y), ImVec2(headPos.x - scale, headPos.y + corner),
							(cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)),
							cfg.visuals.vp.BoxLineWidth);

						// TOP RIGHT
						draw->AddLine(ImVec2(headPos.x + scale, headPos.y), ImVec2(headPos.x + scale - corner, headPos.y),
							(cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)),
							cfg.visuals.vp.BoxLineWidth);
						draw->AddLine(ImVec2(headPos.x + scale, headPos.y), ImVec2(headPos.x + scale, headPos.y + corner),
							(cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)),
							cfg.visuals.vp.BoxLineWidth);

						// BOTTOM RIGHT
						draw->AddLine(ImVec2(feetPos.x + scale, feetPos.y), ImVec2(feetPos.x + scale - corner, feetPos.y),
							(cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)),
							cfg.visuals.vp.BoxLineWidth);
						draw->AddLine(ImVec2(feetPos.x + scale, feetPos.y), ImVec2(feetPos.x + scale, feetPos.y - corner),
							(cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)),
							cfg.visuals.vp.BoxLineWidth);

						// BOTTOM LEFT
						draw->AddLine(ImVec2(feetPos.x - scale, feetPos.y), ImVec2(feetPos.x - scale + corner, feetPos.y),
							(cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)),
							cfg.visuals.vp.BoxLineWidth);
						draw->AddLine(ImVec2(feetPos.x - scale, feetPos.y), ImVec2(feetPos.x - scale, feetPos.y - corner),
							(cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)),
							cfg.visuals.vp.BoxLineWidth);
					}
					break;

					// 3D BOX
					case 2:
					{
						const Ent* const p = &players[i];

						// BOTTOM
						draw->AddLine(p->b1, p->b2, (cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)), cfg.visuals.vp.BoxLineWidth);
						draw->AddLine(p->b2, p->b3, (cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)), cfg.visuals.vp.BoxLineWidth);
						draw->AddLine(p->b3, p->b4, (cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)), cfg.visuals.vp.BoxLineWidth);
						draw->AddLine(p->b4, p->b1, (cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)), cfg.visuals.vp.BoxLineWidth);

						// TOP
						draw->AddLine(p->t1, p->t2, (cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)), cfg.visuals.vp.BoxLineWidth);
						draw->AddLine(p->t2, p->t3, (cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)), cfg.visuals.vp.BoxLineWidth);
						draw->AddLine(p->t3, p->t4, (cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)), cfg.visuals.vp.BoxLineWidth);
						draw->AddLine(p->t4, p->t1, (cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)), cfg.visuals.vp.BoxLineWidth);

						// COLUMNS
						draw->AddLine(p->t1, p->b1, (cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)), cfg.visuals.vp.BoxLineWidth);
						draw->AddLine(p->t2, p->b2, (cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)), cfg.visuals.vp.BoxLineWidth);
						draw->AddLine(p->t3, p->b3, (cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)), cfg.visuals.vp.BoxLineWidth);
						draw->AddLine(p->t4, p->b4, (cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)&cfg.visuals.vp.BoxColor)), cfg.visuals.vp.BoxLineWidth);
					}
					break;
					}
				}

				// NAME
				if (cfg.visuals.vp.bNameESP)
				{
					draw->AddText(ImVec2(headPos.x - ImGui::CalcTextSize(players[i].name.c_str()).x / 2 + 8, headPos.y - 20),
						(cfg.visuals.vp.bBoxRGB ? cfg.visuals.vp.RGBColor : ImColor(*(ImVec4*)cfg.visuals.vp.BoxColor)),
						players[i].name.c_str());
				}
			}
		}

		// BUSTLING BUTTONS ESP
		if (cfg.visuals.ButtonESP && dngButton.x != -1)
		{
			draw->AddLine(ImVec2(Client::WindowCenter.x, Client::WindowSize.y - 5), dngButton, ImColor(255, 0, 0), 2.f);
		}

		// GLASS ESP
		if (cfg.player.bNoGlassBreak && cfg.visuals.GlassESP)
		{
			for (byte i = 0; i < glassEspEx.size(); i++)
				draw->AddLine(ImVec2(Client::WindowCenter.x, Client::WindowSize.y - 5), glassEspEx[i],
					ImColor(cfg.visuals.bRGBGlass ? *(ImVec4*)&cfg.visuals.vp.RGBColor.Value : *(ImVec4*)&cfg.visuals.GlassColor),
					1.f);
		}

		// BOMB TIMER
		if (cfg.visuals.bBombTimer && cfg.visuals.bombTime != -20.f)
		{
			char buffer[25];
			sprintf_s(buffer, XorCStr("Bomb Timer: %.2fs"), cfg.visuals.bombTime);

			draw->AddText(ImVec2(100, 100), ImColor(255, 0, 0), buffer);
		}

		// COMMANDS HELP WINDOW
		if (bShowCmdHelp)
		{
			if (!ImGui::IsPopupOpen(XorCStr("Command Help")))
				ImGui::OpenPopup(XorCStr("Command Help"));
			if (ImGui::BeginPopupModal(XorCStr("Command Help"), 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
			{
				if (ImGui::IsWindowAppearing())
					ImGui::SetWindowPos(ImVec2(Client::WindowCenter.x - ImGui::GetWindowWidth() / 2, Client::WindowCenter.y - ImGui::GetWindowHeight() / 2));

				ImGui::Text(XorCStr("All commands:\n\n\
give: gives an item to player;\tsyntax: give [item name] [player name/id(optional)]\n\
tp: teleports to player;\tsyntax: tp [player name/id]\n\
tphere: teleports player(s) to you (host only);\tsyntax: tphere [player name/id]\n\
start: starts the game (host only)\n\
next: next round (host only)\n\
set: sets some options;\tsyntax: set [feature name] [value]\n\
gamemode: sets a gamemode;\tsyntax: gamemode [gamemode id(0 - 2)]\n\
clear: clears the chat\n\
players: shows player list\n\
items: shows item info\n\
respawn: respawns player(s) (host only);\tsyntax: respawn [player name/id]\n\
kill: kills player(s) (host only);\tsyntax: kill [player name/id]\n\
help: pops up this window\n"));

				if (ImGui::Button(XorCStr("OK")))
					ImGui::CloseCurrentPopup(), bShowCmdHelp = false;
				ImGui::EndPopup();
			}
		}

		// ITEMS HELP WINDOW
		else if (bShowItemHelp)
		{
			if (!ImGui::IsPopupOpen(XorCStr("Items##cmd")))
				ImGui::OpenPopup(XorCStr("Items##cmd"));
			if (ImGui::BeginPopupModal(XorCStr("Items##cmd"), 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
			{
				if (ImGui::IsWindowAppearing())
					ImGui::SetWindowPos(ImVec2(Client::WindowCenter.x - ImGui::GetWindowWidth() / 2, Client::WindowCenter.y - ImGui::GetWindowHeight() / 2));

				ImGui::Text(XorCStr("rifle: 0\npistol: 1\nrevolver: 2\ndb: 3\nbat: 4\nbomb: 5\nkatana: 6\nknife: 7\npipe: 8\snowball: 9\nstick: 10\n"));

				if (ImGui::Button(XorCStr("OK")))
					ImGui::CloseCurrentPopup(), bShowItemHelp = false;
				ImGui::EndPopup();
			}
		}

		// PLAYERS HELP WINDOW
		else if (bShowPlayers)
		{
			if (!ImGui::IsPopupOpen(XorCStr("Player List##cmd")))
				ImGui::OpenPopup(XorCStr("Player List##cmd"));
			if (ImGui::BeginPopupModal(XorCStr("Player List##cmd"), 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
			{
				if (ImGui::IsWindowAppearing())
					ImGui::SetWindowPos(ImVec2(Client::WindowCenter.x - ImGui::GetWindowWidth() / 2, Client::WindowCenter.y - ImGui::GetWindowHeight() / 2));

				for (USHORT i = 0; i < playerNames.size(); i++)
				{
					if (i >= activePlayers->length) break;
					ImGui::Text(XorCStr("%s - %d"), playerNames[i], getPlayerNum(activePlayers->items[i]));
				}
				if (!playerNames.size())
					ImGui::Text(XorCStr("No players"));

				if (ImGui::Button(XorCStr("OK")))
					ImGui::CloseCurrentPopup(), bShowPlayers = false; 
				ImGui::EndPopup();
			}
		}
	}

	// MAIN
	float invisibilityHight = 0.f;
	float minGlassHeight = 0.f;
	void UpdatePlayer()
	{
		// MAIN
		static INPUT ip = { INPUT_KEYBOARD };
		if (playerMovement)
		{
			// FLYHACK
			static bool bResetFly = false;
			if (cfg.player.bFlyhack)
			{
				*(obsecured_bool*)(playerMovement + off::PlayerMovement::underWater) = GetObsucredBool(true);
				*(obscured_float*)(playerMovement + off::PlayerMovement::swimSpeed) = GetObscuredFloat(cfg.player.flySpeed);
				bResetFly = true;
			}
			else if (bResetFly)
			{
				*(obsecured_bool*)(playerMovement + off::PlayerMovement::underWater) = GetObsucredBool(false);
				*(obscured_float*)(playerMovement + off::PlayerMovement::swimSpeed) = GetObscuredFloat(50.f);
				bResetFly = false;
			}

			bool bAllowHotkeys = !bTyping && Client::hwnd == activeHNWD;
			auto KeyEv = [](const DWORD& key) -> void
			{
				ip.ki.wVk = key;
				SendInput(1, &ip, sizeof(INPUT));
			};

			// BUNNYHOP
			if (cfg.player.bBunnyhop && bAllowHotkeys &&
				cfg.player.bunKey != 0xADAF && GetAsyncKeyState(cfg.player.bunKey) & 0x8000)
			{
				ip.ki.time = 0;
				ip.ki.wVk = VK_SPACE;

				ip.ki.dwFlags = KEYEVENTF_KEYUP;
				SendInput(1, &ip, sizeof(INPUT));

				ip.ki.dwFlags = 0;
				SendInput(1, &ip, sizeof(INPUT));
			}

			// NO CLIP
			static const uintptr_t set_collider = il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Collider"),
				XorStr("set_enabled"),
				XorStr("System.Boolean"));

			if (cfg.player.bSetNoClip)
			{
				generic_array* colliders = il2cpp::find_objects_of_type(XorStr("UnityEngine.Collider"), XorStr("UnityEngine.PhysicsModule"));
				if (colliders && colliders->length)
				{
					for (int i = 0; i < colliders->length; i++)
					{
						reinterpret_cast<void(*)(uintptr_t Collider, bool enabled)>
							(set_collider)(colliders->items[i], !cfg.player.bNoClip);
					}
				}
				cfg.player.bSetNoClip = false;
			}

			// SPEED HACK
			{
				auto SetSpeed = [](float moveSpeed, float iceMoveSpeedFaster = 0, float maxWalkSpeed = 0, float maxRunSpeed = 0, float  maxSpeed = 0) -> void
				{
					if (iceMoveSpeedFaster == 0)
					{
						obscured_float val = GetObscuredFloat(moveSpeed);

						*(obscured_float*)(playerMovement + off::PlayerMovement::moveSpeed) = val;
						*(obscured_float*)(playerMovement + off::PlayerMovement::maxWalkSpeed) = val;
						*(obscured_float*)(playerMovement + off::PlayerMovement::maxRunSpeed) = val;
						*(obscured_float*)(playerMovement + off::PlayerMovement::maxSpeed) = val;
					}
					else
					{
						obscured_float val = GetObscuredFloat(128000.f);
						*(obscured_float*)(playerMovement + off::PlayerMovement::moveSpeed) = val;

						val = GetObscuredFloat(6.5f);
						*(obscured_float*)(playerMovement + off::PlayerMovement::maxWalkSpeed) = val;
						*(obscured_float*)(playerMovement + off::PlayerMovement::maxSpeed) = val;

						val = GetObscuredFloat(13.f);
						*(obscured_float*)(playerMovement + off::PlayerMovement::maxRunSpeed) = val;
					}
				};

				using obscured_float = generic_item<0x14>;
				static bool bResetSpeed = false;
				if (cfg.player.bSpeedHack)
				{
					SetSpeed(cfg.player.Speed);
					bResetSpeed = true;
				}
				else if (bResetSpeed)
				{
					SetSpeed(0, 12);
					bResetSpeed = false;
				}
			}

			// SUPER JUMP
			static bool bResetSuperJump = false;
			if (cfg.player.bSuperJump)
			{
				*(float*)(playerMovement + off::PlayerMovement::jumpForce) = cfg.player.jumpForce;
				bResetSuperJump = true;
			}
			else if (bResetSuperJump)
			{
				*(float*)(playerMovement + off::PlayerMovement::jumpForce) = 399.99999999f;
				bResetSuperJump = false;
			}

			// INFINITE JUMP
			static const uintptr_t PlayerMovement_PushPlayer = il2cpp::get_method(XorStr("Assembly-CSharp"),
				XorStr("System.Void"),
				XorStr(""),
				XorStr("PlayerMovement"),
				XorStr("PushPlayer"),
				XorStr("UnityEngine.Vector3"));
			if (cfg.player.bInfiniteJump && bAllowHotkeys)
			{
				static bool bWait = false;
				if (GetAsyncKeyState(VK_SPACE) & 0x8000)
				{
					if (!bWait)
					{
						*(bool*)(playerMovement + off::PlayerMovement::grounded) = true;
						reinterpret_cast<void(*)(Component * PlayerMovement, vec3 pushForce)>(PlayerMovement_PushPlayer)
							(playerMovement, { 0, *(float*)(playerMovement + off::PlayerMovement::jumpForce) / 10.f, 0 });
						bWait = true;
					}
				}
				else
					bWait = false;
			}

			// SPAWN WEAPON
			if (cfg.player.ForceGiveWeapons && cfg.player.weaponId >= 0)
			{
				if (cfg.player.bSpawnWeaponAsHost)
					giveItemToPlayer(cfg.player.weaponId, getLocalPlayerId()), cfg.player.bSpawnWeaponAsHost = false;
				else
					giveItemToPlayer(cfg.player.weaponId, 0);
				cfg.player.ForceGiveWeapons = false;
			}

			// INFINITE AMMO
			if (cfg.player.bInfiniteAmmo)
			{
				generic_array* items = il2cpp::find_objects_of_type(XorCStr("ItemData"), XorStr("Assembly-CSharp"));

				if (items)
				{
					for (int i = 0; i < items->length; i++)
					{
						*(int*)(items->items[i] + off::ItemData::currentAmmo) =
							*(int*)(items->items[i] + off::ItemData::currentAmmo + 0x4); // set current ammo to max ammo
					}
				}
			}

			// BREAK GLASS
			if (cfg.player.bBreakGlass)
			{
				static vec3 posBefore1stTel = { 0.1291f };

				glassPieces = (generic_array_t<Component*>*)il2cpp::find_objects_of_type(XorStr("GlassBreak"), XorStr("Assembly-CSharp"));
				if (glassPieces && glassPieces->length > 0)
				{
					static int counter1 = 0;

					if (counter1 == 0)
					{
						using namespace std::chrono_literals;
						Transform* ourTransform = playerMovement->get_transform();

						if (posBefore1stTel.x == 0.1291f)
							posBefore1stTel = ourTransform->get_position();

						ourTransform->set_position(glassPieces->items[0]->get_transform()->get_position());

						counter1 = 1;
					}
					else
						counter1--;
				}
				else
				{
					cfg.player.bBreakGlass = false;
					if (posBefore1stTel.x != 0.1291f)
						playerMovement->get_transform()->set_position(posBefore1stTel);
					posBefore1stTel.x = 0.1291f;
				}
			}

			// RED LIGHT GREEN LIGHT BOT
			static bool bResetBot = false;
			if (cfg.player.bRLGLBot && bAllowHotkeys && !Menu::bShowMenu)
			{
				static bool bFinished = false;
				generic_array* redLightStatue = il2cpp::find_objects_of_type(XorStr("RedLightStatue"), XorStr("Assembly-CSharp"));
				if (redLightStatue && redLightStatue->length)
				{
					if (!bFinished)
					{
						const float currentTime = *(float*)(redLightStatue->items[0] + off::RedLightStatue::currentTime);

						// GREEN LIGHT
						static vec3 lastPosition = {};
						static constexpr const WORD timerMax = 30;
						static short counter = timerMax, counter2 = 0;
						if (currentTime == .5f)
						{
							vec3 dollPos = (*(Transform**)(redLightStatue->items[0] + off::RedLightStatue::head))->get_position();
							dollPos.y -= 4.5f;
							dollPos.x += 10.f;
							dollPos.z += 10.f;

							if (dollPos.z - 0.5f <= getLocalPos().z)
							{
								bFinished = true;

								ip.ki.dwFlags = KEYEVENTF_KEYUP;
								ip.ki.time = 0;

								KeyEv('W');
								KeyEv(VK_SHIFT);
							}
							else
							{
								vec3 scnPos = camera->WorldToScreenPoint(dollPos);

								static bool bStuck = false;
								if (scnPos.z > 1.f)
								{
									ip.ki.dwFlags = 0;
									ip.ki.time = 0;
									KeyEv('W');
									KeyEv(VK_SHIFT);

									// CHECK IF WE ARE STUCK
									if (counter <= 0)
									{
										const vec3 curPos = getLocalPos();
										if (Vec3::Distance(lastPosition, curPos) < 1.f)
										{
											mouse_event(MOUSEEVENTF_MOVE, curPos.x < lastPosition.x ? -800 : 800, 0, 0, 0);
											bStuck = true;
											counter2 = 60;
										}
										else
											bStuck = false;
										lastPosition = curPos;
										counter = timerMax;
									}
									else
										counter--;

									if (!bStuck)
									{
										if (counter2 <= 0)
										{
											// LOOK AT DOLL
											scnPos.y = Client::WindowSize.y - scnPos.y;
											mouse_event(MOUSEEVENTF_MOVE, (scnPos.x - mouse.x) / 2.f, (scnPos.y - mouse.y) / 2.f, 0, 0);
										}
										else
											counter2--;
									}
								}
								else if (counter2 <= 0)
									mouse_event(MOUSEEVENTF_MOVE, 15, 0, 0, 0);
								else
									counter2--;
								bResetBot = true;
							}
						}

						// RED LIGHT
						else if (bResetBot)
						{
							ip.ki.dwFlags = KEYEVENTF_KEYUP;
							ip.ki.time = 0;

							KeyEv('W');
							KeyEv(VK_SHIFT);

							counter = timerMax;
							lastPosition = getLocalPos();

							bResetBot = false;
						}
					}
				}
				else
					bFinished = false;
			}
			else if (bResetBot)
			{
				ip.ki.dwFlags = KEYEVENTF_KEYUP;
				ip.ki.time = 0;
				KeyEv('W');
				KeyEv(VK_SHIFT);
				bResetBot = false;
			}

			// FAR REACH
			static vec3 posBeforeTel = {};
			static Component* targetOpm = 0;
			static bool bResetFarReach = false;
			static int counter1 = 0;
			if (cfg.player.bFarReach && bAllowHotkeys && !bResetFarReach && cfg.player.farReachKey != 0xADAF && GetAsyncKeyState(cfg.player.farReachKey) & 0x8000)
			{
				closestDst = MAX_FLOAT;
				bool bFound = false;
				Component* player = 0;
				for (int i = 0; i < activePlayers->length; i++)
				{
					vec3 WHeadPos = *(vec3*)(activePlayers->items[i] + off::OnlinePlayerMovement::lastPositon);
					WHeadPos.y += 1.f;

					vec3 headPos = camera->WorldToScreenPoint(WHeadPos);

					// IS BEHIND SCREEN CHECK
					if (headPos.z < 0.3f)
						continue;
					headPos.y = Client::WindowSize.y - headPos.y;

					// IS CLOSES ENEMY
					dst = sqrt((headPos.x - mouse.x) * (headPos.x - mouse.x) + (headPos.y - mouse.y) * (headPos.y - mouse.y));
					if (dst < closestDst)
					{
						closestDst = dst;
						player = activePlayers->items[i];
						bFound = true;
					}
				}

				if (bFound)
				{
					Transform* myTransform = playerMovement->get_transform();

					posBeforeTel = myTransform->get_position();
					targetOpm = player;

					myTransform->set_position(player->get_transform()->get_position());

					counter1 = 20;
					bResetFarReach = true;
				}
			}
			else if (bResetFarReach)
			{
				if (counter1)
				{
					if (counter1 == 10)
					{
						static const uintptr_t ClientSend_PunchPlayer = il2cpp::get_method(XorStr("Assembly-CSharp"),
							XorStr("System.Void"),
							XorStr(""),
							XorStr("ClientSend"),
							XorStr("PunchPlayer"),
							XorStr("System.UInt64, UnityEngine.Vector3"));

						vec3 dir = targetOpm->get_transform()->get_position() - getLocalPos();
						dir.x *= 40;
						dir.y *= 40;
						dir.z *= 40;
						reinterpret_cast<void(*)(uintptr_t playerId, vec3 dir)>(ClientSend_PunchPlayer)(getPlayerId(targetOpm), dir);
					}
					counter1--;
				}
				else
				{
					playerMovement->get_transform()->set_position(posBeforeTel);
					bResetFarReach = false;
				}
			}

			// MOUSE TELEPORT
			if (cfg.player.bMouseTeleport && bAllowHotkeys && !bResetFarReach && cfg.player.mouseTpKey != 0xADAF && PressKey(cfg.player.mouseTpKey))
			{
				Ray ray = camera->ScreenPointToRay({ (float)Client::WindowCenter.x, (float)Client::WindowCenter.y });

				static const uintptr_t Raycast = il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
					XorStr("System.Boolean"),
					XorStr("UnityEngine"),
					XorStr("Physics"),
					XorStr("Raycast"),
					XorStr("UnityEngine.Ray, UnityEngine.RaycastHit&"));

				RaycastHit rayhit;
				if (Physics::Raycast(ray, rayhit))
					playerMovement->get_transform()->set_position(rayhit.m_Point);
			}

			// ANTI VOID/WATER/LAVA
			static bool bResetWater = false;
			static int lavaCounter = 0;
			if (bResetWater)
			{
				if (lavaCounter)
					lavaCounter--;
				else
				{
					*(bool*)(playerMovement + off::PlayerMovement::grounded) = true;
					reinterpret_cast<void(*)(Component* PlayerMovement, vec3 pushForce)>(PlayerMovement_PushPlayer)
						(playerMovement, { 0, -30.f, 0 });
					bResetWater = false;
				}
			}
			else if (!cfg.player.bFlyhack && ((cfg.player.bAntiVoid && playerMovement->get_transform()->get_position().y <= -30.f) ||
				(cfg.player.bAntiWater && GetBoolFromOB(*(obsecured_bool*)(playerMovement + off::PlayerMovement::underWater)))))
			{
				*(bool*)(playerMovement + off::PlayerMovement::grounded) = true;
				reinterpret_cast<void(*)(Component* PlayerMovement, vec3 pushForce)>(PlayerMovement_PushPlayer)
					(playerMovement, { 0, 100.f, 0 });
				bResetWater = true;
				lavaCounter = 20;
			}

			// TILE DRIVE TELEPORT
			static int tileDvCounter = 0;
			if (cfg.player.bTileDriveTp)
			{
				if (tileDvCounter)
					tileDvCounter--;
				else
				{
					generic_array* GameModeTileDrive = il2cpp::find_objects_of_type(XorStr("GameModeTileDrive"), XorStr("Assembly-CSharp"));
					if (GameModeTileDrive && GameModeTileDrive->length)
					{
						generic_array* tiles = il2cpp::find_objects_of_type(XorStr("Tile"), XorStr("Assembly-CSharp"));
						if (tiles && tiles->length)
						{
							auto idToTeams = *(generic_dictionary<uintptr_t, int>**)(GameModeTileDrive->items[0] + 0x38);
							static const uintptr_t myId = getLocalPlayerId();
							int myTeam = 0xADAF;

							for (int i = 0; i < idToTeams->count; i++)
							{
								if (myId == idToTeams->entries->items[i].key)
								{
									myTeam = idToTeams->entries->items[i].value;
									break;
								}
							}

							if (myTeam != 0xADAF)
							{
								for (int i = 0; i < tiles->length; i++)
								{
									if (*(int*)(tiles->items[i] + 0x6C) == myTeam)
										continue;

									playerMovement->get_transform()->set_position(*(vec3*)(tiles->items[i] + 0x30));

									tileDvCounter = 5;
									break;
								}
							}
						}
					}
				}
			}

			// HOLD TO SHOOT
			auto MouseLeftClick = []()->void
			{
				static constexpr const INPUT MouseLeftClick[2] = {
					{ INPUT_MOUSE, { 0, 0, 0, MOUSEEVENTF_LEFTDOWN   } },
					{ INPUT_MOUSE, { 0, 0, 0, MOUSEEVENTF_LEFTUP } }
				};

				SendInput(2, (INPUT*)MouseLeftClick, sizeof(INPUT));
			};
			if (cfg.aim.bHoldToShoot && !Menu::bShowMenu && bAllowHotkeys && GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				static const uintptr_t PlayerInventory_UseItem = il2cpp::get_method(XorStr("Assembly-CSharp"),
					XorStr("System.Void"),
					XorStr(""),
					XorStr("PlayerInventory"),
					XorStr("UseItem"),
					XorStr(""));
				static const uintptr_t PlayerInventory_MouseUp = il2cpp::get_method(XorStr("Assembly-CSharp"),
					XorStr("System.Void"),
					XorStr(""),
					XorStr("PlayerInventory"),
					XorStr("MouseUp"),
					XorStr(""));

				auto PlayerInv = (generic_array_t<Component*>*)il2cpp::find_objects_of_type(XorStr("PlayerInventory"), XorStr("Assembly-CSharp"));
				if (PlayerInv && PlayerInv->length)
				{
					reinterpret_cast<void(*)(Component * PlayerInventory)>(PlayerInventory_MouseUp)
						(PlayerInv->items[0]);
					reinterpret_cast<void(*)(Component* PlayerInventory)>(PlayerInventory_UseItem)
						(PlayerInv->items[0]);
				}
			}

			// HBSK TELEPORT
			static bool bResetHBS = false;
			static vec3 posBeforeHBS{};
			if (cfg.player.bHbsTp && ((cfg.player.bAutoHBStp && !Menu::bShowMenu) || (bAllowHotkeys && cfg.player.hbsTpKey != 0xADAF && GetAsyncKeyState(cfg.player.hbsTpKey) & 0x8000)))
			{
				generic_list_t<uintptr_t>* tagged = nullptr;
				int gameMode = 0;
				switch (0)
				{
				case 0:
					generic_array * GameMode = il2cpp::find_objects_of_type(XorStr("GameModeTag"), XorStr("Assembly-CSharp"));

					if (GameMode && GameMode->length)
					{
						tagged = *(generic_list_t<uintptr_t>**)(GameMode->items[0] + 0x38);
						gameMode = 4;
						break;
					}

					GameMode = il2cpp::find_objects_of_type(XorStr("GameModeHnS"), XorStr("Assembly-CSharp"));

					if (GameMode && GameMode->length)
					{
						tagged = *(generic_list_t<uintptr_t>**)(GameMode->items[0] + 0x38);
						gameMode = 5;
						break;
					}
					GameMode = il2cpp::find_objects_of_type(XorStr("GameModeBombTag"), XorStr("Assembly-CSharp"));

					if (GameMode && GameMode->length)
					{
						tagged = *(generic_list_t<uintptr_t>**)(GameMode->items[0] + 0x48);
						gameMode = 6;
						break;
					}

					GameMode = il2cpp::find_objects_of_type(XorStr("GameModeHat"), XorStr("Assembly-CSharp"));

					if (GameMode && GameMode->length)
					{
						tagged = *(generic_list_t<uintptr_t>**)(GameMode->items[0] + 0x38);
						gameMode = 10;
					}
				}

				if (tagged)
				{
					bool amItagged = false;
					uintptr_t someOneInList = 0;
					for (int i = 0; i < tagged->size; i++)
					{
						if (getLocalPlayerId() == tagged->items->items[i])
						{
							amItagged = true;
							if (someOneInList)
								break;
						}
						else
							someOneInList = tagged->items->items[i];
					}

					auto DoStuff = [&](Component*& opm)->void
					{
						float y_rot = *(float*)(opm + off::OnlinePlayerMovement::yRot);
						vec3 pos = opm->get_transform()->get_position();
						vec3 aimPos = pos;

						pos.x = pos.x - cosf(TORAD(y_rot + 270)) * 2.f;
						pos.z = pos.z + sinf(TORAD(y_rot + 270)) * 2.f;

						playerMovement->get_transform()->set_position(pos);

						aimPos.y += 1.f;
						aimAt(aimPos, 1.f);

						MouseLeftClick();
					};

					switch (gameMode)
					{
						// TAG
						// HIDE AND SEEK
						// BOMB TAG
					case 4:
					case 5:
					case 6:
						if (amItagged)
						{
							for (int i = 0; i < activePlayers->length; i++)
							{
								bool bOnList = false;
								for (int j = 0; j < tagged->size; j++)
								{
									if (tagged->items->items[j] == getPlayerId(activePlayers->items[i]))
									{
										bOnList = true;
										break;
									}
								}

								if (!bOnList)
								{
									if (!bResetHBS)
									{
										posBeforeHBS = playerMovement->get_transform()->get_position();
										bResetHBS = true;
									}

									DoStuff(activePlayers->items[i]);
									break;
								}
							}
						}
						else if (bResetHBS)
							playerMovement->get_transform()->set_position(posBeforeHBS), bResetHBS = false;
						break;

						// HAT KING
					case 10:
						if (!amItagged && someOneInList)
						{
							for (int i = 0; i < activePlayers->length; i++)
							{
								if (someOneInList == getPlayerId(activePlayers->items[i]))
								{
									if (!bResetHBS)
									{
										posBeforeHBS = playerMovement->get_transform()->get_position();
										bResetHBS = true;
									}

									DoStuff(activePlayers->items[i]);
									break;
								}
							}
						}
						else if (bResetHBS)
							playerMovement->get_transform()->set_position(posBeforeHBS), bResetHBS = false;
						break;
					}
				}
				else
					bResetHBS = false;
			}
			else if (bResetHBS)
				playerMovement->get_transform()->set_position(posBeforeHBS), bResetHBS = false;

			// SUPER SLIDE
			static bool bResetSlide = false;
			if (cfg.player.bSuperSlide)
			{
				obscured_float val = GetObscuredFloat(cfg.player.slideForce);
				*(obscured_float*)(playerMovement + 0xEC) = val;
				bResetSlide = true;
			}
			else if (bResetSlide)
			{
				obscured_float val = GetObscuredFloat(8000.f);
				*(obscured_float*)(playerMovement + 0xEC) = val;
			}

			// NO ICE SLIDE
			if (cfg.player.bNoIceSlide)
				*(int*)(playerMovement + 0x2B4) = 0;

			// INFINITE SLIDE
			if (cfg.player.bInfiniteSlide && *(int*)(playerMovement + 0x174) == 2)
			{
				static const uintptr_t Rigidbody_get_velocity = il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
					XorStr("UnityEngine.Vector3"),
					XorStr("UnityEngine"),
					XorStr("Rigidbody"),
					XorStr("get_velocity"),
					XorStr(""));

				vec3 vel = reinterpret_cast<vec3(*)(uintptr_t rb)>(Rigidbody_get_velocity)(*(uintptr_t*)(playerMovement + 0x38));
				if (sqrt(vel.x * vel.x + vel.z * vel.z) <= 10.f)
				{
					static constexpr const float pushForce = 4.f;

					*(bool*)(playerMovement + off::PlayerMovement::grounded) = true;
					reinterpret_cast<void(*)(uintptr_t PlayerMovement, vec3 pushForce)>(PlayerMovement_PushPlayer)
						((uintptr_t)playerMovement, { vel.x > 0 ? pushForce : -pushForce, 0, vel.z > 0 ? pushForce : -pushForce });
				}
			}

			// LOW GRAVITY
			static bool bResetGravity = false;
			if (cfg.player.bModGravity)
			{
				*(float*)(playerMovement + 0x110) = cfg.player.gravity;
				bResetGravity = true;
			}
			else if (bResetGravity)
			{
				*(float*)(playerMovement + 0x110) = 2000.f;
				bResetGravity = false;
			}

			// SUPER TIRE JUMP
			static bool bResetTires = false;
			if (cfg.player.bSuperTireJump)
			{
				generic_array* tires = il2cpp::find_objects_of_type(XorStr("JumpPad"), XorStr("Assembly-CSharp"));
				if (tires)
				{
					for (USHORT i = 0; i < tires->length; i++)
					{
						*(float*)(tires->items[i] + 0x20) = cfg.player.tireForce;
					}
					bResetTires = true;
				}
			}
			else if (bResetTires)
			{
				generic_array* tires = il2cpp::find_objects_of_type(XorStr("JumpPad"), XorStr("Assembly-CSharp"));
				if (tires)
				{
					for (USHORT i = 0; i < tires->length; i++)
					{
						*(float*)(tires->items[i] + 0x20) = 30.f;
					}
				}
				bResetTires = false;
			}

			// NO BOUND KILL
			static bool bResetBounds = false;
			if (cfg.player.bNoBoundKill)
			{
				generic_array_t<Component*>* Bounds = (generic_array_t<Component*>*)il2cpp::find_objects_of_type(XorStr("KillPlayerOutOfBounds"), XorStr("Assembly-CSharp"));
				for (USHORT i = 0; i < Bounds->length; i++)
				{
					Bounds->items[i]->Destroy();
				}
				bResetBounds = true;
			}
			else if (bResetBounds)
			{
				generic_array_t<Component*>* obj = (generic_array_t<Component*>*)il2cpp::find_objects_of_type(XorStr("GameManager"), XorStr("Assembly-CSharp"));
				if (obj && obj->length)
				{
					static const Il2CppClass* kpoob = il2cpp::_inner_get_class(XorStr("Assembly-CSharp"), XorStr(""), XorStr("KillPlayerOutOfBounds"));
					static const std::string type = std::string(kpoob->name) + XorStr(", Assembly-CSharp");

					obj->items[0]->get_gameObject()->AddComponent(type);
				}
				bResetBounds = false;
			}

			static const uintptr_t ClientSend_PlayerPosition = il2cpp::get_method(XorStr("Assembly-CSharp"),
				XorStr("System.Void"),
				XorStr(""),
				XorStr("ClientSend"),
				XorStr("PlayerPosition"),
				XorStr("UnityEngine.Vector3, System.UInt64"));

			// INVISIBILITY
			static bool bResetInv = false;
			if ((cfg.player.bInvisibility || bResetInv) && activePlayers->length)
			{
				srand(time(0));
				for (USHORT i = 0; i < activePlayers->length; i++)
				{
					reinterpret_cast<void(*)(vec3 position, uintptr_t toId)>
						(ClientSend_PlayerPosition)(playerMovement->get_transform()->get_position(), getPlayerId(activePlayers->items[i]));
				}

				if (cfg.player.bInvisibility)
				{
					bResetInv = true, invisibilityHight += 50.f;
					invisibilityHight += 5.f * (invisibilityHight < 400.f);
				}
				else
				{
					bResetInv = false;
					invisibilityHight = 0.f;
				}
			}

			// PLAYER TP
			if (cfg.player.bPlayerTeleport && activePlayers->length && bAllowHotkeys && GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				Component* opm = getClosestEnemy();
				opm->get_transform()->set_position(
					camera->get_transform()->get_forward() * cfg.player.playerTpDist + getLocalPos());
			}

			// SUICIDE
			if (cfg.player.bSuicide)
			{
				generic_array* PlayerStatus = il2cpp::find_objects_of_type(XorStr("PlayerStatus"), XorStr("Assembly-CSharp"));

				if (playerMovement && PlayerStatus && PlayerStatus->length)
					Hooks::oPlayerStatus_DamagePlayer(PlayerStatus->items[0], 101, { 0.f, 0.f, 0.f }, 0, 0);

				cfg.player.bSuicide = false;
			}
		}



		// START GAME
		if (cfg.player.bStartGame)
		{
			static const uintptr_t StartGame = il2cpp::get_method(XorStr("Assembly-CSharp"),
				XorStr("System.Void"),
				XorStr(""),
				XorStr("GameLoop"),
				XorStr("StartGames"),
				XorStr(""));

			generic_array* GameLoop = il2cpp::find_objects_of_type(XorStr("GameLoop"), XorStr("Assembly-CSharp"));

			if (GameLoop && GameLoop->length && StartGame)
			{
				generic_array* GameModeWaiting = il2cpp::find_objects_of_type(XorStr("GameModeWaiting"), XorStr("Assembly-CSharp"));
				if (GameModeWaiting && GameModeWaiting->length)
					reinterpret_cast<void(*)(uintptr_t GameLoop)>(StartGame)(GameLoop->items[0]);
			}

			cfg.player.bStartGame = false;
		}

		// NEXT GAME
		if (cfg.player.bNextGame)
		{
			static const uintptr_t NextGame = il2cpp::get_method(XorStr("Assembly-CSharp"),
				XorStr("System.Void"),
				XorStr(""),
				XorStr("GameLoop"),
				XorStr("NextGame"),
				XorStr(""));

			generic_array* GameLoop = il2cpp::find_objects_of_type(XorStr("GameLoop"), XorStr("Assembly-CSharp"));

			if (GameLoop && GameLoop->length)
			{
				generic_array* GameModeWaiting = il2cpp::find_objects_of_type(XorStr("GameModeWaiting"), XorStr("Assembly-CSharp"));
				if (!GameModeWaiting || !GameModeWaiting->length)
					reinterpret_cast<void(*)(uintptr_t GameLoop)>(NextGame)(GameLoop->items[0]);
			}

			cfg.player.bNextGame = false;
		}

		// SEND MESSAGES
		static const uintptr_t Chatbox_SendMessage = il2cpp::get_method(XorStr("Assembly-CSharp"),
			XorStr("System.Void"),
			XorStr(""),
			XorStr("Chatbox"),
			XorStr("SendMessage"),
			XorStr("System.String"));
		if (cfg.player.messageCounter)
		{
			static int messageCounter = 0;
			if (messageCounter)
				messageCounter--;
			else
			{
				if (Chatbox_SendMessage && chat)
				{
					generic_string* gnString = il2cpp::string_to_generic(cfg.player.message);
					reinterpret_cast<void(*)(Component* Chatbox, generic_string * message)>(Chatbox_SendMessage)(chat, gnString);
				}

				cfg.player.messageCounter--;
				messageCounter = 50;
			}
		}

		// ZOOM
		bool bAllowFOVChanger = true;
		auto SetLocalFOV = [](const generic_array* const& const CurrentSettings, const float& v)->void
		{
			static const uintptr_t CurrentSettings_UpdateFov = il2cpp::get_method(XorStr("Assembly-CSharp"),
				XorStr("System.Void"),
				XorStr(""),
				XorStr("CurrentSettings"),
				XorStr("UpdateFov"),
				XorStr("System.Single"));

			reinterpret_cast<void(*)(uintptr_t CurrentSettings, float i)>
				(CurrentSettings_UpdateFov)(CurrentSettings->items[0], v);
		};
		if (cfg.visuals.bZoom)
		{
			static int bResetFOV = false;
			static int prevFOV = 90;
			if (cfg.visuals.zoomKey != 0xADAF && GetAsyncKeyState(cfg.visuals.zoomKey) & 0x8000)
			{
				generic_array* CurrentSettings = il2cpp::find_objects_of_type(XorStr("CurrentSettings"), XorStr("Assembly-CSharp"));
				if (CurrentSettings && CurrentSettings->length)
				{
					if (!bResetFOV)
						prevFOV = *(int*)(CurrentSettings->items[0] + 0x1C);

					SetLocalFOV(CurrentSettings, cfg.visuals.zoomFOV);

					bResetFOV = true;
				}
				bAllowFOVChanger = false;
			}
			else if (bResetFOV)
			{
				generic_array* CurrentSettings = il2cpp::find_objects_of_type(XorStr("CurrentSettings"), XorStr("Assembly-CSharp"));
				if (CurrentSettings && CurrentSettings->length)
					SetLocalFOV(CurrentSettings, prevFOV);
				bResetFOV = false;
			}
		}

		// FOV CHANGER
		if (bAllowFOVChanger)
		{
			static int prevFOV = 90;
			static bool bResetFOV = false;
			if (cfg.visuals.bFOV)
			{
				generic_array* CurrentSettings = il2cpp::find_objects_of_type(XorStr("CurrentSettings"), XorStr("Assembly-CSharp"));
				if (CurrentSettings && CurrentSettings->length)
				{
					if (!bResetFOV)
						prevFOV = *(int*)(CurrentSettings->items[0] + 0x1C);

					SetLocalFOV(CurrentSettings, cfg.visuals.FOV);

					bResetFOV = true;
				}
			}
			else if (bResetFOV)
			{
				generic_array* CurrentSettings = il2cpp::find_objects_of_type(XorStr("CurrentSettings"), XorStr("Assembly-CSharp"));
				if (CurrentSettings && CurrentSettings->length)
				{
					SetLocalFOV(CurrentSettings, prevFOV);
					bResetFOV = false;
				}
			}
		}

		// DORM FULL BRIGHT
		if (cfg.visuals.bDormFullBright)
		{
			static const uintptr_t GameModeLights_ToggleLights = il2cpp::get_method(XorStr("Assembly-CSharp"),
				XorStr("System.Void"),
				XorStr(""),
				XorStr("GameModeLights"),
				XorStr("ToggleLights"),
				XorStr("System.Boolean"));

			generic_array* GameModeLights = il2cpp::find_objects_of_type(XorStr("GameModeLights"), XorStr("Assembly-CSharp"));
			if (GameModeLights && GameModeLights->length && !*(bool*)(GameModeLights->items[0] + 0x3C))
			{
				reinterpret_cast<void(*)(uintptr_t GameModeLights, bool b)>(GameModeLights_ToggleLights)
					(GameModeLights->items[0], true);
			}
		}

		// CLEAR CHAT
		static int clearAmount = 0;
		static int clearCounter = 10;
		if (cfg.player.bClearChat || cfg.player.bAutoClsChat)
			clearAmount = 20, cfg.player.bClearChat = false;
		if (clearAmount && Chatbox && Chatbox->length)
		{
			if (!cfg.player.bClearChat)
				clearAmount = 0, clearAmount = 0;
			if (clearCounter)
				clearCounter--;
			else
			{
				reinterpret_cast<void(*)(Component* Chatbox, generic_string* message)>(Chatbox_SendMessage)
					(Chatbox->items[0], il2cpp::string_to_generic(XorStr("super idol sussy baka 19 dollar mincraft amogus packet card 1hAck sus ngl ok...")));

				clearCounter = 10;
				clearAmount--;
			}
		}
		else
			clearAmount = 0;

		// SEND PLAYERS 2 SPACE
		static int spaceCounter = 0;
		if (cfg.player.bSend2Space)
		{
			if (spaceCounter < activePlayers->length)
			{
				static const uintptr_t PunchPlayer = il2cpp::get_method(XorStr("Assembly-CSharp"),
					XorStr("System.Void"),
					XorStr(""),
					XorStr("ServerSend"),
					XorStr("PunchPlayer"),
					XorStr("System.UInt64, System.UInt64, UnityEngine.Vector3"));

				if (!playerMovement)
				{
					if (activePlayers && (activePlayers->length >= 2 || spaceCounter))
					{
						if (!spaceCounter)
						{
							reinterpret_cast<void(*)(uintptr_t fromClient, uintptr_t punched, vec3 dir)>
								(PunchPlayer)
								(getPlayerId(activePlayers->items[1]), getPlayerId(activePlayers->items[spaceCounter]), { 0.f, 200.f, 0.f });
						}
						else
						{
							reinterpret_cast<void(*)(uintptr_t fromClient, uintptr_t punched, vec3 dir)>
								(PunchPlayer)
								(getPlayerId(activePlayers->items[0]), getPlayerId(activePlayers->items[spaceCounter]), { 0.f, 200.f, 0.f });
						}
					}
				}
				else
				{
					reinterpret_cast<void(*)(uintptr_t fromClient, uintptr_t punched, vec3 dir)>
						(PunchPlayer)
						(getLocalPlayerId(), getPlayerId(activePlayers->items[spaceCounter]), {0.f, 200.f, 0.f});
				}

				spaceCounter++;
			}
			else
				spaceCounter = 0, cfg.player.bSend2Space = false;
		}

		// FREEZE TIMER
		static bool bInitedTimer = false;
		if (cfg.player.bFreezeTimer)
		{
			static float freezeTimer = 0.f;

			generic_array* GameMode = il2cpp::find_objects_of_type(XorStr("GameMode"), XorStr("Assembly-CSharp"));
			if (GameMode && GameMode->length)
			{
				if (!bInitedTimer)
				{
					if (IsValidPointer(GameMode->items[0] + 0x18, { 0x10 }) && *(uintptr_t*)(GameMode->items[0] + 0x18))
						freezeTimer = *(float*)(*(uintptr_t*)(GameMode->items[0] + 0x18) + 0x10);
					bInitedTimer = true;
				}
				else
				{
					if (IsValidPointer(GameMode->items[0] + 0x18, { 0x10 }) && *(uintptr_t*)(GameMode->items[0] + 0x18))
						*(float*)(*(uintptr_t*)(GameMode->items[0] + 0x18) + 0x10) = freezeTimer;
				}
			}
			else bInitedTimer = false;
		}
		else bInitedTimer = false;

		// RESPAWN PLAYERS
		if (cfg.player.bRespawn)
		{
			respawnPlayer(0, 0.f);
			cfg.player.bRespawn = false;
		}

		// FREEZE PLAYERS
		static std::vector<std::pair<uintptr_t, vec3>> positions = {};
		static std::pair<uintptr_t, vec3> individualPos = {};
		switch (cfg.player.bFreezePlayers)
		{
		case true:
		{
			if (!cfg.player.frozenPlayer) // all players
			{
				individualPos = { 0, {} };

				for (USHORT i = 0; i < activePlayers->length; i++)
				{
					uintptr_t id = getPlayerId(activePlayers->items[i]);
					bool found = false;	

					for (USHORT j = 0; j < positions.size(); j++)
					{
						if (positions[j].first == id)
						{
							found = true;
							setPlayerPos(id, positions[j].second);
							break;
						}
					}

					if (!found)
						positions.push_back({ id, activePlayers->items[i]->get_transform()->get_position() });
				}
			}
			else // individual player
			{
				positions.clear();

				if (!individualPos.first)
				{
					if (!activePlayers->length)
					{
						cfg.player.frozenPlayer = 0;
						cfg.player.bFreezePlayers = false;
						break;
					}
					int realPlayer = cfg.player.frozenPlayer - 1;

					if (realPlayer < 0 || activePlayers->length <= realPlayer)
					{
						cfg.player.frozenPlayer = activePlayers->length;
						realPlayer = activePlayers->length - 1;
					}

					auto player = activePlayers->items[realPlayer];

					individualPos = { getPlayerId(player), player->get_transform()->get_position() };
				}
				else
				{
					bool bFound = false;
					for (USHORT i = 0; i < activePlayers->length; i++)
					{
						auto player = activePlayers->items[i];
						if (getPlayerId(player) == individualPos.first)
						{
							cfg.player.frozenPlayer = i + 1;
							setPlayerPos(individualPos.first, individualPos.second);
							bFound = true;
							break;
						}
					}

					if (!bFound)
					{
						cfg.player.frozenPlayer = 0;
						cfg.player.bFreezePlayers = false;
					}
				}
			}
		}
		break;

		case false:
			positions.clear();
			individualPos = { 0, {} };
		}

		// KILL PLAYERS
		switch (cfg.player.bKillPlayers)
		{
		case true:
			cfg.player.bKillPlayers = false;

			if (!cfg.player.playerToKill) // everyone
			{
				for (USHORT i = 0; i < activePlayers->length; i++)
				{
					killPlayer(getPlayerPm(activePlayers->items[i]));
				}
			}
			else
			{
				if (!activePlayers->length)
				{
					cfg.player.playerToKill = 0;
					break;
				}
				int realPlayer = cfg.player.playerToKill - 1;

				if (realPlayer < 0 || activePlayers->length <= realPlayer)
				{
					cfg.player.playerToKill = activePlayers->length;
					realPlayer = activePlayers->length - 1;
				}

				killPlayer(getPlayerPm(activePlayers->items[realPlayer]));
			}
		}		

		// NO GLASS BREAK
		switch (cfg.player.bNoGlassBreak)
		{
		case true:
		{
			if (!glassPieces || getGameModeState() == GameModeState::Freeze)
			{
				minGlassHeight = 0.f;
				glassEspPosEx.clear();
				break;
			}

			static const uintptr_t GlassBreak_MakeSolid = il2cpp::get_method(XorStr("Assembly-CSharp"),
				XorStr("System.Void"),
				XorStr(""),
				XorStr("GlassBreak"),
				XorStr("MakeSolid"),
				XorStr(""));
			auto GameModeGlass = il2cpp::find_objects_of_type(XorStr("GameModeGlass"), XorStr("Assembly-CSharp"));
			if (!GameModeGlass || !GameModeGlass->length)
			{
				minGlassHeight = 0.f;
				glassEspPosEx.clear();
				break;
			}

			for (USHORT i = 0; i < glassPieces->length; i++)
			{
				vec3 pos = glassPieces->items[i]->get_transform()->get_position();
				minGlassHeight = pos.y;

				reinterpret_cast<void(*)(Component * GlassBreak)>(GlassBreak_MakeSolid)
					(glassPieces->items[i]);

				glassEspPosEx.push_back(pos);
			}
		}
			break;

		case false:
			minGlassHeight = 0.f;
			glassEspPosEx.clear();
		}
		if (cfg.visuals.GlassESP)
		{
			glassEspEx.clear();
			for (byte i = 0; i < glassEspPosEx.size(); i++)
			{
				vec3 screenPos = camera->WorldToScreenPoint(glassEspPosEx[i]);
				if (screenPos.z > 0.3f)
				{
					screenPos.y = Client::WindowSize.y - screenPos.y;
					glassEspEx.push_back(screenPos);
				}
			}
		}

		static const uintptr_t ServerSend_PressButton = il2cpp::get_method(XorStr("Assembly-CSharp"),
			XorStr("System.Void"),
			XorStr(""),
			XorStr("ServerSend"),
			XorStr("ButtonPressed"),
			XorStr("System.UInt64, System.Int32"));

		// KILL CURRENT BUTTON PRESSER
		switch (cfg.player.bKillCurrentButtonPresser)
		{
		case true:
			cfg.player.bKillCurrentButtonPresser = false;

			generic_array* GameModeButtons = il2cpp::find_objects_of_type(XorStr("GameModeButtons"), XorStr("Assembly-CSharp"));
			if (!GameModeButtons || !GameModeButtons->length || getGameModeState() == GameModeState::Freeze) break;

			uintptr_t targetId = *(uintptr_t*)(GameModeButtons->items[0] + 0x50);
			if (targetId)
			{
				reinterpret_cast<void(*)(uintptr_t playerId, int buttonId)>(ServerSend_PressButton)
					(targetId, *(int*)(GameModeButtons->items[0] + 0x38));
			}
		}

		// SAVE CURRENT BUTTON PRESSER
		switch (cfg.player.bSaveCurrentButtonPresser)
		{
		case true:
			cfg.player.bSaveCurrentButtonPresser = false;

			generic_array* GameModeButtons = il2cpp::find_objects_of_type(XorStr("GameModeButtons"), XorStr("Assembly-CSharp"));
			if (!GameModeButtons || !GameModeButtons->length || getGameModeState() == GameModeState::Freeze) break;

			generic_array* BustilingButtons = il2cpp::find_objects_of_type(XorStr("BustlingButtonInteract"), XorStr("Assembly-CSharp"));
			if (!BustilingButtons || !BustilingButtons->length) break;

			uintptr_t targetId = *(uintptr_t*)(GameModeButtons->items[0] + 0x50);
			if (!targetId) break;

			int dangerousButton = *(int*)(GameModeButtons->items[0] + 0x38);

			int safeButtonId = -1;
			for (byte i = 0; i < BustilingButtons->length; i++)
			{
				int buttonId = *(int*)(BustilingButtons->items[i] + 0x20);
				if (buttonId != dangerousButton)
				{
					safeButtonId = buttonId;
					break;
				}
			}

			if (safeButtonId != -1 && targetId)
			{
				reinterpret_cast<void(*)(uintptr_t playerId, int buttonId)>(ServerSend_PressButton)
					(targetId, safeButtonId);
			}
		}
		
		// TP ALL
		static const float PLAYER_HITBOX_SIZE = 3.f;
		struct TpAll_t
		{
			USHORT TpAllCounter = 0;
			USHORT timer = 0;
			vec3 myPos = {};

			USHORT playersInCircle = 0;
			float radius = 0;
			USHORT maxPlayersInCircle = 0;

			TpAll_t() {}
			TpAll_t(bool)
			{
				this->radius = 5.f;
				this->maxPlayersInCircle = 2 * radius * M_PI / PLAYER_HITBOX_SIZE;
				this->TpAllCounter = activePlayers->length;
				this->myPos = getLocalPos();
				cfg.player.bTpAll = false;
			}

			__forceinline auto operator->() { return this; }
		};
		static TpAll_t tpAll;

		if (cfg.player.bTpAll)
			tpAll = TpAll_t(true);

		switch (1)
		{
		case 1:
			if (tpAll->TpAllCounter > 0)
			{
				if (tpAll->timer)
				{
					tpAll->timer--;
					break;
				}

				for (USHORT i = 0; i < 1; i++)
				{
					if (tpAll->playersInCircle > tpAll->maxPlayersInCircle)
					{
						tpAll->radius += PLAYER_HITBOX_SIZE;
						tpAll->playersInCircle = 0;
						tpAll->maxPlayersInCircle = 2 * tpAll->radius * M_PI / PLAYER_HITBOX_SIZE;
					}

					float distance = 360.f / tpAll->maxPlayersInCircle;
					float x = tpAll->myPos.x + cosf(TORAD(tpAll->playersInCircle * distance)) * tpAll->radius;
					float z = tpAll->myPos.z + sinf(TORAD(tpAll->playersInCircle * distance)) * tpAll->radius;

					setPlayerPos(getPlayerId(activePlayers->items[tpAll->TpAllCounter - 1]), { x, tpAll->myPos.y, z });
					tpAll->playersInCircle++;
					tpAll->TpAllCounter--;
					tpAll->timer = 5;
				}
			}
			else
				tpAll = TpAll_t();

			break;
		}
	}

	// RUN SCRIPTS
	__forceinline void RunScripts()
	{
		for (USHORT i = 0; i < cfg.script.files.size(); i++)
		{
			auto script = &cfg.script.files[i];

			if (script->lua)
			{
				lua_pushboolean(script->lua, true);
				lua_setglobal(script->lua, XorCStr("_IN_GAME"));

				// RUN OnEnable/OnDisbale
				if (script->flags & ScriptFlags_StatusChanged)
				{
					// OnEnable
					if (script->bEnabled)
					{
						script->CallFunc(XorCStr("OnEnable"));
					}
					// OnDisable
					else
					{
						script->CallFunc(XorCStr("OnDisable"));
						script->Uninit();
					}
					script->flags &= ~(ScriptFlags_StatusChanged);
				}

				// RUN Update
				else if (script->bEnabled && (script->flags & ScriptFlags_HasUpdate))
				{
					script->CallFunc(XorCStr("Update"));
				}
			}
		}
	}



	// ------------------- HOOKS -------------------

	static bool bAllowInput = true;
	void hGameUI_Update(uintptr_t GameUI)
	{
		Client::bInGame = true;
		if (Menu::bMenuStatusChanged)
		{
			if (cfg.bFreeMouseOnPopup)
			{
				if (Cursor::get_lockState() && Menu::bShowMenu)
				{
					Cursor::set_locakState(0);
					Cursor::set_visible(true);
					bAllowInput = false;
				}
				else
				{
					Cursor::set_locakState(1);
					Cursor::set_visible(false);
					bAllowInput = true;
				}
			}
			else
				bAllowInput = true;

			Menu::bMenuStatusChanged = false;
		}

		UpdatePointers();

		GetCursorPos(&mouse);
		if (!bTyping && Client::hwnd == activeHNWD)
			RunAim();

		UpdatePlayer();
		RunScripts();

		return Hooks::oGameUI_Update(GameUI);
	}

	void hMenuUI_Start(uintptr_t MenuUI)
	{
		Client::bInGame = false;
		for (USHORT i = 0; i < cfg.script.files.size(); i++)
		{
			auto script = &cfg.script.files[i];
			if (script->lua)
			{
				lua_pushboolean(script->lua, false);
				lua_setglobal(script->lua, XorCStr("_IN_GAME"));
			}
		}
		return Hooks::oMenuUI_Start(MenuUI);
	}

	void hGameManager_PunchPlayer(uintptr_t GameManager, uintptr_t puncher, uintptr_t punched, vec3 dir)
	{
		if (cfg.player.bAntiPunch && punched == getLocalPlayerId())
		{
			if (cfg.player.antiForce < 1.f)
			{
				dir.x *= 1.f - cfg.player.antiForce;
				dir.y *= 1.f - cfg.player.antiForce;
				dir.z *= 1.f - cfg.player.antiForce;
			}
			else
				return;
		}

		return Hooks::oGameManager_PunchPlayer(GameManager, puncher, punched, dir);
	}

	void hPlayerMovement_PushPlayer(uintptr_t PlayerMovement, vec3 pushForce)
	{
		if (cfg.player.bAntiKnockback)
		{
			if (cfg.player.antiForce < 1.f)
			{
				pushForce.x *= 1.f - cfg.player.antiForce;
				pushForce.y *= 1.f - cfg.player.antiForce;
				pushForce.z *= 1.f - cfg.player.antiForce;
			}
			else
				return;
		}

		return Hooks::oPlayerMovement_PushPlayer(PlayerMovement, pushForce);
	}

	void hServerSend_PunchPlayer(uintptr_t fromClient, uintptr_t punchedPlayer, vec3 dir)
	{
		if (cfg.player.bSuperPunch && getLocalPlayerId() == fromClient)
		{
			dir.x *= cfg.player.punchMultiplier;
			dir.y *= cfg.player.punchMultiplier;
			dir.z *= cfg.player.punchMultiplier;
		}

		return Hooks::oServerSend_PunchPlayer(fromClient, punchedPlayer, dir);
	}

	void hServerSend_LoadMap(int mapId, int gameModeId)
	{
		auto GetGameModeId = [&]()->int
		{
			switch (mapId)
			{
			// RED LIGHT GREEN LIGHT - 1
			case 5: // DESERT
			case 17: // PLAINS
			case 22: // SLIPPERY SLOPE
				return 1;

			// KING OF HILL - 2
			case 3: // COLOR CLIMB
			case 9: // HASTY HILL
			case 25: // SMALL HILL
				return 2;

			// STEPPING STONES - 3
			case 8: // GLASS JUMP
			case 11: // ICY ISLANDS
			case 13: // ISLANDS
			case 24: // SMALL GLASS JUMP
			case 26: // SMALL ICY ISLANDS
			case 27: // SMALL ISLANDS
			case 45: // SANDY ISLANDS
			case 46: // SMALL SANDY ISLANDS
				return 3;

			// TAG - 4
			// HIDE AND SEEK - 5
			// BOMB TAG - 6
			// HAT KING - 10
			case 0: // BITTER BEACH
			case 1: // BLUELINE
			case 2: // COCKY CONTAINERS
			case 7: // FUNKY FIELD
			case 14: // KARLSON
			case 15: // LANKY LAVA
			case 18: // PLAYGROUND
			case 19: // PLAYGROUND2
			case 20: // RETURN TO MONKE
			case 21: // SANDSTORM
			case 23: // SMALL COLOR CLIMB
			case 28: // SMALL PLAYGROUND
			case 32: // SUNNY SALOON
			case 33: // TOXIC TRAIN
			case 34: // TWISTED ROWERS
			case 35: // MINI MONKE
			case 36: // SMALL BEACH
			case 37: // SMALL SALOON
			case 38: // SMALL CONTAINERS
			case 39: // TINY TOWN 2
			case 40: // TINY TOWN
				switch (cfg.player.nextGameMode)
			{
			case 0:
			{
				srand((unsigned)time(0));
				int game = rand() % 4;
				switch (game)
				{
				case 0: return 4;
				case 1: return 5;
				case 2: return 6;
				case 3: return 10;
				}
			}

			case 1: return 4;
			case 2: return 5;
			case 3: return 6;
			case 4: return 10;
			}
			return 4;

			// LIGHTS OUT - 7
			case 6: // DORM
				return 7;

			// THE FLOOR IS LAVA - 8
			case 4: // CRUSTY ROCKS
			case 12: // ICY ROCKS
			case 16: // LAVA LAKE
			case 58: // SANDY STONES
				return 8;

			// TILE DRIVE - 9
			case 30: // SPLAT
			case 31: // SPLOT
				return 9;

			// RACE - 11
			case 54: // LAVA CLIMB
			case 55: // MACARONI MOUNTAIN
			case 56: // SUSSY SANDCASTLE
			case 57: // SUSSY SLOPE
				return 11;

			// BUSTLING BUTTONS - 12
			case 53: // SAUCY STAGE
				return 12;

			// PRACTICE - 13

			// DEATH FROM ABOVE - 14
			case 49: // LAVA DUMP
			case 51: // SALTY ISLAND
			case 52: // SKYBOX
				return 14;

			// BLOCK DROP - 15
			case 47: // CHEEKY CHAMBER
			case 48: // LAVA DROP
			case 50: // PEACEFUL PLATFORM
				return 15;

			// SNOW BRAWL - 16
			case 10: // ICY CRACK
			case 29: // SNOWTOP
				return 16;

			// DODGEBALL - 17
			case 41: // DODGY FIELDS
			case 42: // DODGY ICE
			case 43: // DODGY SNOW
			case 44: // DODGY STREETS
				return 17;

			// CRAB FIGHT - 18
			case 59: // CRABFIELDS
			case 60: // CRABHEAT
			case 61: // CRABLAND
				return 18;

			default:
				return gameModeId;
			}
		};

		if (cfg.player.nextMap > 0 && gameModeId)
		{
			mapId = cfg.player.nextMap - 1;
			gameModeId = GetGameModeId();
		}

		return Hooks::oServerSend_LoadMap(mapId, gameModeId);
	}

	void hPlayerStatus_DamagePlayer(uintptr_t PlayerStatus, int dmg, vec3 damageDir, uintptr_t damagerDoerId, int itemId)
	{
		generic_array* playerStatuses = il2cpp::find_objects_of_type(XorStr("PlayerStatus"), XorStr("Assembly-CSharp"));

		if (playerStatuses && playerStatuses->length && playerStatuses->items[0] == PlayerStatus)
		{
			if (cfg.player.bGodMode)
			{
				if (cfg.player.bAntiPunch && cfg.player.antiForce == 1.f)
					return;
				else
					dmg = 0;
			}
			if (cfg.player.bAntiPunch)
			{
				damageDir.x *= 1.f - cfg.player.antiForce;
				damageDir.y *= 1.f - cfg.player.antiForce;
				damageDir.z *= 1.f - cfg.player.antiForce;
			}
		}

		return Hooks::oPlayerStatus_DamagePlayer(PlayerStatus, dmg, damageDir, damagerDoerId, itemId);
	}

	void hCameraShaker_Update(uintptr_t CameraShaker)
	{
		if (cfg.player.bNoCameraShake)
			return;

		return Hooks::oCameraShaker_Update(CameraShaker);
	}
	void hCameraShaker_GunShake(uintptr_t CameraShaker, float shakeRatio)
	{
		if (cfg.player.bNoCameraShake)
			return;

		return Hooks::oCameraShaker_GunShake(CameraShaker, shakeRatio);
	}	 
	void hCameraShaker_PushShake(uintptr_t CameraShaker)
	{
		if (cfg.player.bNoCameraShake)
			return;

		return Hooks::oCameraShaker_PushShake(CameraShaker);
	}
	void hCameraShaker_DamageShake(uintptr_t CameraShaker, float shakeRatio)
	{
		if (cfg.player.bNoCameraShake)
			return;

		return Hooks::oCameraShaker_DamageShake(CameraShaker, shakeRatio);
	}

	bool hGameModeRedLight_CanSeePlayer(uintptr_t GameModeRedLight, uintptr_t pId)
	{
		if (cfg.player.bRedLightGodmode && getLocalPlayerId() == pId)
			return false;

		return Hooks::oGameModeRedLight_CanSeePlayer(GameModeRedLight, pId);
	}
	
	void hGameModeTag_TagPlayer(uintptr_t GameModeTag, uintptr_t tagger, uintptr_t tagged)
	{
		if (cfg.player.bAntiTag && getLocalPlayerId() == tagged)
			return;
		return Hooks::oGameModeTag_TagPlayer(GameModeTag, tagger, tagged);
	}
	void hGameModeBombTag_TagPlayer(uintptr_t GameModeBombTag, uintptr_t tagger, uintptr_t tagged)
	{
		if (cfg.player.bAntiTag && getLocalPlayerId() == tagged)
			return;
		return Hooks::oGameModeBombTag_TagPlayer(GameModeBombTag, tagger, tagged);
	}
	void hGameModeHat_StealHat(uintptr_t GameModeHat, uintptr_t stealer, uintptr_t stolenFrom)
	{
		if (cfg.player.bAntiTag && getLocalPlayerId() == stolenFrom)
			return;
		return Hooks::oGameModeHat_StealHat(GameModeHat, stealer, stolenFrom);
	}

	void hPlayerInput_Update(uintptr_t PlayerInput)
	{
		if (cfg.bFreeMouseOnPopup && !bAllowInput && Menu::bShowMenu)
			return;

		return Hooks::oPlayerInput_Update(PlayerInput);
	}

	void hClientSend_PlayerRotation(float x, float y, uintptr_t toId)
	{
		// SPINBOT
		if (cfg.aim.bSpinBot)
		{
			y = cfg.aim.ySpinProcess;
			x = 90.f;
		}

		// RLGL GODMODE
		if (cfg.player.bRedLightGodmode && toId == hostId)
		{
			static vec2 lastAlvPos = { x, y };

			generic_array* redLightStatue = il2cpp::find_objects_of_type(XorStr("RedLightStatue"), XorStr("Assembly-CSharp"));
			if (redLightStatue && redLightStatue->length)
			{
				if (*(float*)(redLightStatue->items[0] + off::RedLightStatue::currentTime) == 0.5f)
					lastAlvPos = { x, y };
				else
					x = lastAlvPos.x, y = lastAlvPos.y;
			}
		}

		return Hooks::oClientSend_PlayerRotation(x, y, toId);
	}

	void hClientSend_PlayerPosition(vec3 position, uintptr_t toId)
	{
		// INVISIBILITY
		if (cfg.player.bInvisibility)
		{
			bool bFound = false;
			for (USHORT i = 0; i < activePlayers->length; i++)
			{
				if (getPlayerId(activePlayers->items[i]) == toId)
				{
					bFound = true;
					break;
				}
			}

			if (bFound)
			{
				if (toId != hostId)
				{
					position.y -= (rand() % 400) * (invisibilityHight < 400.f) + invisibilityHight;
				}
				else
				{
					position.y += (rand() % 400) * (invisibilityHight < 400.f) + invisibilityHight;
				}
			}
		}

		// RLGL GODMODE
		if (cfg.player.bRedLightGodmode && toId == hostId)
		{
			static vec3 lastAlvPos = position;

			generic_array* redLightStatue = il2cpp::find_objects_of_type(XorStr("RedLightStatue"), XorStr("Assembly-CSharp"));
			if (redLightStatue && redLightStatue->length)
			{
				if (*(float*)(redLightStatue->items[0] + off::RedLightStatue::currentTime) == 0.5f)
					lastAlvPos = position;
				else
					position = lastAlvPos;
			}
		}

		// NO BOUND KILL
		if (cfg.player.bNoBoundKill && position.y < -10.5f && toId == hostId)
		{
			generic_array* gameModeWaiting = il2cpp::find_objects_of_type(XorStr("GameModeWaiting"), XorStr("Assembly-CSharp"));

			if (gameModeWaiting && !gameModeWaiting->length)
				position.y = -10.5f;
		}

		// NO GLASS BREAK
		if (cfg.player.bNoGlassBreak && minGlassHeight && playerMovement)
		{
			float myFeetPos = (*(Transform**)(playerMovement + 0x1C0))->get_position().y;

			float minHeight = minGlassHeight;
			if (minGlassHeight > 10.f)
				minHeight += 0.255f;
			else
				minHeight += 0.71f;

			if (minHeight >= myFeetPos)
				position.y += minHeight - myFeetPos;
		}

		return Hooks::oClientSend_PlayerPosition(position, toId);
	}

	void hItemGun_TryUse(uintptr_t ItemGun, Transform* camForward)
	{
		// SILENT AIM
		if (cfg.aim.bSilentAim && playerMovement)
		{
			Component* opm = getClosestEnemy();
			if (opm)
			{
				auto bones = opm->GetComponentsInChildren<Transform>(XorStr("UnityEngine.Transform, UnityEngine.CoreModule"));

				if (bones && bones->length > 30)
					camForward->set_rotation(Quaternion::LookRotation(
						bones->items[30]->get_position() - getLocalPos()));
			}
		}

		return Hooks::oItemGun_TryUse(ItemGun, camForward);
	}
	void hItemSnowball_TryUse(uintptr_t ItemGun, Transform* camForward)
	{
		// SILENT AIM
		if (cfg.aim.bSilentAim && playerMovement)
		{
			Component* opm = getClosestEnemy();
			if (opm)
			{
				auto bones = opm->GetComponentsInChildren<Transform>(XorStr("UnityEngine.Transform, UnityEngine.CoreModule"));

				if (bones && bones->length > 30)
				{
					vec3 desiredPos = bones->items[30]->get_position();
					desiredPos.y += vec3::Distance(desiredPos, getLocalPos()) / 2 / 17.f;

					camForward->set_rotation(Quaternion::LookRotation(desiredPos - getLocalPos()));
				}
			}
		}

		return Hooks::oItemSnowball_TryUse(ItemGun, camForward);
	}

	void hClientSend_DamagePlayer(uintptr_t hurtPlayerId, int damage, vec3 damageDir, int itemID, int objectID)
	{
		if (itemID == 0 || itemID == 3)
			itemID = 1;

		return Hooks::oClientSend_DamagePlayer(hurtPlayerId, damage, damageDir, itemID, objectID);
	}

	void hClientSend_UseItem(int itemId, vec3 dir)
	{
		if (itemId == 0 || itemId == 3) // RIFLE or DOUBLE BARREL
			itemId = 1;

		Hooks::oClientSend_UseItem(itemId, dir);

		if (itemId == 9 && cfg.player.bInfiniteAmmo) // SNOWBALL
		{
			giveItemToPlayer(9, getLocalPlayerId() == hostId ? getLocalPlayerId() : 0);
		}
	}

	void hCameraRecoil_AddRecoil(vec2 newRecoil)
	{
		if (cfg.aim.bNoRecoil)
		{
			if (cfg.aim.recoilSlider >= 1.f)
				return;
			else
			{
				newRecoil.x *= (1.f - cfg.aim.recoilSlider);
				newRecoil.y *= (1.f - cfg.aim.recoilSlider);
			}
		}
		return Hooks::oCameraRecoil_AddRecoil(newRecoil);
	}

	void hServerSend_SendButtonPress(ButtonModeState buttonState)
	{
		if (cfg.player.bTrollButtonScreen)
		{
			if (buttonState == ButtonModeState::RevealGreen)
				buttonState = ButtonModeState::RevealRed;
			else if (buttonState == ButtonModeState::RevealRed)
				buttonState = ButtonModeState::RevealGreen;
		}
		return Hooks::oServerSend_SendButtonState((int)buttonState);
	}

	void hGameModeButtons_PressButton(uintptr_t GameModeButtons, uintptr_t playerId, int buttonId)
	{
		if (cfg.player.bOnlyRiggedButtons && playerId != getLocalPlayerId())
			buttonId = *(int*)(GameModeButtons + 0x38);
		return Hooks::oGameModeButtons_PressButton(GameModeButtons, playerId, buttonId);
	}

	void HandleCommand(const uintptr_t& Chatbox, std::string& cmd, std::string& data, USHORT dataStart, USHORT dataEnd, const std::string& message)
	{
		static constexpr const vec4 errorColor = { 0.88f, 0.31f, 0.18f, 1.f };
		auto LogMessage = [&](const std::string& msg, const vec4& color)->void
		{
			static const uintptr_t Chatbox_AppendMessage = il2cpp::get_method(XorStr("Assembly-CSharp"),
				XorStr("System.Void"),
				XorStr(""),
				XorStr("Chatbox"),
				XorStr("AppendMessage"),
				XorStr("System.UInt64, System.String, System.String"));

			vec4 prevColor = *(vec4*)(Chatbox + 0x30);
			*(vec4*)(Chatbox + 0x30) = color;
			reinterpret_cast<void(*)(uintptr_t Chatbox, uintptr_t fromUser, generic_string * message, generic_string * fromUsername)>
				(Chatbox_AppendMessage)(Chatbox, getLocalPlayerId(), il2cpp::string_to_generic(msg),
					il2cpp::string_to_generic(XorStr("1hAck CGMM")));
			*(vec4*)(Chatbox + 0x30) = prevColor;
		};

		auto StringToPlayerId = [&](bool& bSet)->USHORT
		{
			int player = -1;

			// GET PLAYER THROUGH ID
			if (sscanf_s(data.c_str(), XorCStr("%d"), &player) == 1 && player != -1)
			{
				for (USHORT i = 0; i < activePlayers->length; i++)
				{
					if (player == getPlayerNum(activePlayers->items[i]))
					{
						player = i;
						bSet = true;
						break;
					}
				}
				if (!bSet)
				{
					LogMessage(XorStr("[ERROR] Invalid player id entered"), errorColor);
				}
			}

			// GET PLAYER THROUGH NAME
			else
			{
				std::string playerName;
				for (USHORT i = dataStart; i < message.length(); i++)
					playerName += message[i];
				for (USHORT i = 0; i < activePlayers->length; i++)
				{
					if (getPlayerName(activePlayers->items[i]) == playerName)
					{
						player = i;
						bSet = true;
						break;
					}
				}
				if (!bSet)
				{
					std::string msg = XorStr("[ERROR] No player with name '");
					msg += data;
					msg += XorStr("'");
					LogMessage(msg, errorColor);
				}
			}

			return player;
		};

		// COMMANDS
		if (cmd == "give")
		{
			bool bGiveWeapon = true;
			int weaponId = -1;

			if (sscanf_s(data.c_str(), "%d", &weaponId) == 1 && weaponId != -1)
			{
				if (weaponId < 0 || weaponId >= 11)
				{
					LogMessage(XorStr("[ERROR] The given item id is doesn't corespond to any item, look up '!items' command"), errorColor);
					bGiveWeapon = false;
				}
			}
			else
			{
				if (data == "rifle")
					weaponId = 0;
				else if (data == "pistol")
					weaponId = 1;
				else if (data == "revolver")
					weaponId = 2;
				else if (data == "db")
					weaponId = 3;
				else if (data == "bat")
					weaponId = 4;
				else if (data == "bomb")
					weaponId = 5;
				else if (data == "katana")
					weaponId = 6;
				else if (data == "knife")
					weaponId = 7;
				else if (data == "pipe")
					weaponId = 8;
				else if (data == "snowball")
					weaponId = 9;
				else if (data == "stick")
					weaponId = 10;
				else
				{
					bGiveWeapon = false;
					std::string resultMsg = XorStr("[ERROR] No item with name '");
					resultMsg += data;
					resultMsg += XorStr("'");
					LogMessage(resultMsg, errorColor);
				}
			}

			if (bGiveWeapon)
			{
				std::string playerName;
				for (USHORT i = dataEnd + 2; i < message.length(); i++)
					playerName += message[i];

				uintptr_t playerId = 0;
				if (playerName.length())
				{
					if (playerName == "all" || playerName == "everyone")
					{
						for (USHORT i = 0; i < activePlayers->length; i++)
						{
							giveItemToPlayer(weaponId, getPlayerId(activePlayers->items[i]));
						}
						bGiveWeapon = false;
					}
					else if (playerName == "me") {}
					else if (playerName == "host")
					{
						giveItemToPlayer(weaponId, getLocalPlayerId());
						bGiveWeapon = false;
					}
					else
					{
						bool bFound = false;
						int player = -1;
						if (sscanf_s(playerName.c_str(), XorCStr("%d"), &player) == 1 && player != -1)
						{
							for (USHORT i = 0; i < activePlayers->length; i++)
							{
								if (player == getPlayerNum(activePlayers->items[i]))
								{
									playerId = getPlayerId(activePlayers->items[i]);
									bFound = true;
									break;
								}
							}
							if (!bFound)
							{
								LogMessage(XorStr("[ERROR] Invalid player id entered"), errorColor);
							}
						}
						else
						{
							for (int i = 0; i < activePlayers->length; i++)
							{
								if (getPlayerName(activePlayers->items[i]) == playerName)
								{
									playerId = getPlayerId(activePlayers->items[i]);
									bFound = true;
									break;
								}
							}
							if (!bFound)
							{
								std::string msg = XorStr("[ERROR] No player with name '");
								msg += playerName;
								msg += XorStr("'");
								LogMessage(msg, errorColor);
							}
						}
						bGiveWeapon = bFound;
					}
				}

				if (bGiveWeapon)
					giveItemToPlayer(weaponId, playerId);
			}
		}

		else if (cmd == "tp" && playerMovement)
		{
			std::string playerName;
			for (USHORT i = dataStart; i < message.length(); i++)
				playerName += message[i];

			if (playerName.length())
			{
				int player = -1;
				bool bSet = false;
				if (sscanf_s(data.c_str(), XorCStr("%d"), &player) == 1 && player != -1)
				{
					for (USHORT i = 0; i < activePlayers->length; i++)
					{
						if (player == getPlayerNum(activePlayers->items[i]))
						{
							player = i;
							bSet = true;
							break;
						}
					}
					if (!bSet)
					{
						LogMessage(XorStr("[ERROR] Invalid player id entered"), errorColor);
					}
				}
				else
				{
					if (data == "center")
					{
						playerMovement->get_transform()->set_position({ 0, 0, 0 });
					}
					else if (data == "finish")
					{
						switch (getCurrentMapId())
						{
							// DESERT
						case 3:
							playerMovement->get_transform()->set_position({ 19.889515, 2.902541, 141.960587 });
							break;

							// PLAINS
						case 26:
							playerMovement->get_transform()->set_position({ -2.317573, 26.718513, 143.659668 });
							break;

							// SLIPPERY SLOPE
						case 47:
							playerMovement->get_transform()->set_position({ -2.314326, 2.902541, 142.657562 });
							break;

							// LAVA CLIMB
						case 21:
							playerMovement->get_transform()->set_position({ 44.220383f, 39.902542f, -14.055148f });
							break;

							// MACARONI MOUNTAIN
						case 14:
							playerMovement->get_transform()->set_position({ -66.627510, 31.902540, 104.575096 });
							break;

							// SUSSY SANDCASTLE
						case 33:
							playerMovement->get_transform()->set_position({ -77.131828, 31.902542, 116.753746 });
							break;

						default:
							LogMessage(XorStr("[ERROR] The 'finish' keyword is not available for the current map"), errorColor);
						}
					}
					else
					{
						for (USHORT i = 0; i < activePlayers->length; i++)
						{
							if (getPlayerName(activePlayers->items[i]) == playerName)
							{
								player = i;
								bSet = true;
								break;
							}
						}
						if (!bSet)
						{
							std::string msg = XorStr("[ERROR] No player with name '");
							msg += data;
							msg += XorStr("'");
							LogMessage(msg, errorColor);
						}
					}
				}

				if (bSet)
				{
					playerMovement->get_transform()->set_position(
						activePlayers->items[player]->get_transform()->get_position());
				}
			}
			else
				LogMessage(XorStr("[ERROR] Please enter a player name or id"), errorColor);
		}

		else if (cmd == "clear")
		{
			cfg.player.bClearChat = true;
		}

		else if (cmd == "start")
		{
			cfg.player.bStartGame = true;
		}

		else if (cmd == "next")
		{
			cfg.player.bNextGame = true;
		}

		else if (cmd == "gamemode")
		{
			int gamemode = -1;
			if (data.length() && sscanf_s(data.c_str(), XorCStr("%d"), &gamemode) == 1 && gamemode != -1)
			{
				switch (gamemode)
				{
					// SURVIVLE
				case 0:
					cfg.player.bAntiPunch = false;
					cfg.player.bAntiVoid = false;
					cfg.player.bGodMode = false;
					break;

					// CREATIVE
				case 1:
					cfg.player.bAntiPunch = true;
					cfg.player.bAntiVoid = true;
					cfg.player.bGodMode = true;
					break;

					// ADVENTURE
				case 2:
					cfg.player.bSpeedHack = true;
					cfg.player.Speed = 20.f;
					cfg.player.bSuperJump = true;
					cfg.player.jumpForce = 550.f;
					cfg.player.bAntiVoid = true;
					break;

				default:
					LogMessage(XorStr("[ERROR] The given gamemode is unrecognized"), errorColor);
				}
			}
			else
				LogMessage(XorStr("[ERROR] Please enter a valid gamemode"), errorColor);
		}

		else if (cmd == "set")
		{
			std::string strv;
			for (int i = dataEnd + 2; i < message.length(); i++)
				strv += message[i];

			int value = -1;
			bool bAllowError = true;
			sscanf_s(strv.c_str(), XorCStr("%d"), &value);

			if (value == -1)
			{
				if (data == "map") 
				{
					if (strv == "Random")
					{
						cfg.player.nextMap = 0;
						bAllowError = false;
					}
					else
					{
						for (USHORT i = 1; i < IM_ARRAYSIZE(Items_Maps); i++)
						{
							if (strv == Items_Maps[i])
							{
								value = i - 1;
								break;
							}
						}
						if (value == -1)
						{
							std::string msg = XorStr("[ERROR] No map with name '");
							msg += strv;
							msg += XorStr("'");
							LogMessage(msg, errorColor);
						}
					}
				}
				else
				{
					if (strv == "true")
						value = 1;
					else if (strv == "false")
						value = 0;
				}
			}

			if (value >= 0)
			{
				if (data == "noclip")
				{
					cfg.player.bNoClip = value;
				}
				else if (data == "godmode")
				{
					cfg.player.bGodMode = value;
				}
				else if (data == "antipunch")
				{
					cfg.player.bAntiPunch = value;
				}
				else if (data == "bunnyhop")
				{
					cfg.player.bBunnyhop = value;
				}
				else if (data == "timer")
				{
					generic_array* GameMode = il2cpp::find_objects_of_type(XorStr("GameMode"), XorStr("Assembly-CSharp"));
					if (GameMode && GameMode->length)
					{
						if (IsValidPointer(GameMode->items[0] + 0x18, { 0x10 }) && *(uintptr_t*)(GameMode->items[0] + 0x18))
						{
							if (value > 0)
								*(float*)(*(uintptr_t*)(GameMode->items[0] + 0x18) + 0x10) = value;
							else
								*(float*)(*(uintptr_t*)(GameMode->items[0] + 0x18) + 0x10) = 0.01f;
						}
					}
				}
				else if (data == "map")
				{
					cfg.player.nextMap = value + 1;
				}
				else
				{
					std::string msg = XorStr("[ERROR] No variable with name '");
					msg += data;
					msg += XorStr("'");
					LogMessage(msg, errorColor);
				}
			}
			else if (bAllowError)
				LogMessage(XorStr("[ERROR] Invalid value given"), errorColor);
		}

		else if (cmd == "items")
		{
			bShowItemHelp = true;
		}

		else if (cmd == "players")
		{
			bShowPlayers = true;
		}

		else if (cmd == "help")
		{
			bShowCmdHelp = true;
		}

		else if (cmd == "respawn")
		{
			switch (1)
			{
			case 1:
				if (!data.length())
				{
					respawnPlayer(0, 0.f);
					break;
				}

				// GET PLAYERS
				auto players = getAllPlayers();

				// EVERYONE
				if (data == "all" || data == "everyone")
				{
					for (USHORT i = 0; i < players.size(); i++)
					{
						MonoBehaviour* pm = players[i];
						if (!pm) continue;

						uintptr_t id = *(uintptr_t*)(pm + 0x18);
						if (id == getLocalPlayerId()) continue;

						// IS DEAD
						if (*(bool*)(pm + 0x39))
							respawnPlayer(pm, 0.f);
					}
					break;
				}

				int player = -1;
				bool bSet = false;

				// GET PLAYER THROUGH ID
				if (sscanf_s(data.c_str(), XorCStr("%d"), &player) == 1 && player != -1)
				{
					for (USHORT i = 0; i < players.size(); i++)
					{
						auto pm = players[i];
						if (!pm) continue;

						if (player == *(int*)(pm + 0x94))
						{
							player = i;
							bSet = true;
							break;
						}
					}
					if (!bSet)
					{
						LogMessage(XorStr("[ERROR] Invalid player id entered"), errorColor);
					}
				}

				// GET PLAYER THROUGH NAME
				else
				{
					for (USHORT i = 0; i < players.size(); i++)
					{
						Component* pm = players[i];
						if (pm)
						{
							if (il2cpp::generic_to_string(*(generic_string**)(pm + 0x40)) == data)
							{
								player = i;
								bSet = true;
								break;
							}
						}
					}
					if (!bSet)
					{
						std::string msg = XorStr("[ERROR] No player with name '");
						msg += data;
						msg += XorStr("'");
						LogMessage(msg, errorColor);
					}
				}

				// RESPAWN TARGET PLAYER
				if (bSet && player >= 0)
				{
					respawnPlayer(players[player], 0.f);
				}
			}
		}

		else if (cmd == "tphere")
		{
			switch (1)
			{
			case 1:
				if (!data.length())
				{
					LogMessage(XorStr("[ERROR] Invalid player given"), errorColor);
					break;
				}

				if (data == "all" || data == "everyone")
				{
					cfg.player.bTpAll = true;
					break;
				}

				bool bSet = false;
				USHORT player = StringToPlayerId(bSet);

				if (bSet && player >= 0)
				{
					setPlayerPos(getPlayerId(activePlayers->items[player]), getLocalPos());
				}
			}
		}

		else if (cmd == "kill")
		{
			switch (1)
			{
			case 1:
				if (!data.length())
				{
					LogMessage(XorStr("[ERROR] Invalid player given"), errorColor);
					break;
				}

				if (data == "all" || data == "everyone")
				{
					for (USHORT i = 0; i < activePlayers->length; i++)
						killPlayer(getPlayerPm(activePlayers->items[i]));
					break;
				}

				bool bSet = false;
				USHORT player = StringToPlayerId(bSet);

				if (bSet && player >= 0)
				{
					killPlayer(getPlayerPm(activePlayers->items[player]));
				}
			}
		}

		else
			LogMessage(XorStr("[ERROR] Unkown command"), errorColor);
	}

	// COMMANDS
	void hChatbox_SendMessage(uintptr_t Chatbox, generic_string* message)
	{
		const size_t cmdLen = strlen(cfg.player.commandSymbol);
		if (cfg.player.bCommands && message->length > cmdLen)
		{
			const std::wstring buf = message->string;
			const std::string message = std::string(buf.begin(), buf.end());

			bool bCommand = false;
			for (int i = 0; i < cmdLen; i++)
			{
				if (message[i] != cfg.player.commandSymbol[i])
					break;
				if (i == cmdLen - 1)
					bCommand = true;
			}

			if (bCommand)
			{
				std::string cmd;
				USHORT cmdEnd = 0;
				for (USHORT i = cmdLen; i < message.length(); i++)
				{
					if (message[i] == ' ')
						break;
					cmd += message[i];
					cmdEnd = i;
				}

				std::string data;
				USHORT dataEnd = 0;
				for (USHORT i = cmdEnd + 2; i < message.length(); i++)
				{
					if (message[i] == ' ')
						break;
					data += message[i];
					dataEnd = i;
				}

				HandleCommand(Chatbox, cmd, data, cmdEnd + 2, dataEnd, message);

				// RESET CHATBOX
				static const uintptr_t TMP_InputField_set_text = il2cpp::get_method(XorStr("Unity.TextMeshPro"),
					XorStr("System.Void"),
					XorStr("TMPro"),
					XorStr("TMP_InputField"),
					XorStr("set_text"),
					XorStr("System.String"));
				static const uintptr_t Chatbox_set_typing = il2cpp::get_method(XorStr("Assembly-CSharp"),
					XorStr("System.Void"),
					XorStr(""),
					XorStr("Chatbox"),
					XorStr("set_typing"),
					XorStr("System.Boolean"));
				
				reinterpret_cast<void(*)(uintptr_t TMP_InputField, generic_string * text)>
					(TMP_InputField_set_text)(*(uintptr_t*)(Chatbox + 0x20), il2cpp::string_to_generic(XorStr("")));
				reinterpret_cast<void(*)(uintptr_t Chatbox, bool typing)>(Chatbox_set_typing)(Chatbox, false);
				return Hooks::oChatbox_SendMessage(Chatbox, il2cpp::string_to_generic(XorStr("")));
			}
		}

		return Hooks::oChatbox_SendMessage(Chatbox, message);
	}



	// ------------------- UTILS -------------------

	uintptr_t getLocalPlayerId()
	{
		static const uintptr_t Module = (uintptr_t)GetModuleHandleA(XorStr("steam_api64_net.dll").c_str());
		static const uintptr_t id = *(uintptr_t*)(Module + off::Steam::steamId);
		return id;
	}

	vec3 getLocalPos()
	{
		if (camera)
			return camera->get_transform()->get_position();
		return {};
	}

	std::string getPlayerName(Component* onlinePlayerMovement)
	{
		generic_string* name = *(generic_string**)staticAddress((uintptr_t)onlinePlayerMovement + off::OnlinePlayerMovement::playerManager,
			{ off::PlayerManager::username });

		std::wstring wName = std::wstring(name->string);
		return std::string(wName.begin(), wName.end());
	}

	constexpr const int& getPlayerLayer()
	{
		return 8;
	}

	void aimAt(const vec3& worldPos, const float smoothness)
	{
		generic_array* PlayerInput = il2cpp::find_objects_of_type(XorStr("PlayerInput"), XorStr("Assembly-CSharp"));
		if (!PlayerInput || PlayerInput->length < 1)
			return;

		vec3 myPos = getLocalPos();
		vec3 direction = { worldPos.x - myPos.x, worldPos.y - myPos.y, worldPos.z - myPos.z };

		float desiredRecoilX = 0.f;
		generic_array* CameraRecoil = il2cpp::find_objects_of_type(XorStr("CameraRecoil"), XorStr("Assembly-CSharp"));
		if (CameraRecoil && CameraRecoil->length)
		{
			desiredRecoilX = *(float*)(CameraRecoil->items[0] + 0x18);
		}

		Quaternion targetRotation = Quaternion::LookRotation(direction);
		if (smoothness > 1.f)
		{
			targetRotation = Quaternion::Slerp(camera->get_transform()->get_rotation(), targetRotation, 1.f / (smoothness * 10));
		}

		// SET Y
		static const auto GetPlayerCamTransform = (Transform * (*)(Component*))il2cpp::get_method(XorStr("Assembly-CSharp"),
			XorStr("UnityEngine.Transform"),
			XorStr(""),
			XorStr("PlayerMovement"),
			XorStr("GetPlayerCamTransform"),
			XorStr(""));
		GetPlayerCamTransform(playerMovement)->set_rotation(targetRotation);

		float deltaLength = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
		float desiredXangle = -asin(direction.y / deltaLength) * (180 / M_PI);
		desiredXangle -= desiredRecoilX;

		if (smoothness > 1.f)
		{
			float currentXangle = *(float*)(PlayerInput->items[0] + 0x18);
			desiredXangle = currentXangle + (desiredXangle - currentXangle) / (smoothness * 10);
		}

		if (desiredXangle > 90) desiredXangle = 90;
		else if (desiredXangle < -90) desiredXangle = -90;

		// SET X
		*(float*)(PlayerInput->items[0] + 0x18) = desiredXangle;
	}

	uintptr_t getPlayerId(Component* onlinePlayerMovement)
	{
		uintptr_t id = *(uintptr_t*)staticAddress((uintptr_t)onlinePlayerMovement + off::OnlinePlayerMovement::playerManager,
			{ 0x18 });

		return id;
	}

	void giveItemToPlayer(const int itemId, const uintptr_t& playerId)
	{
		static const uintptr_t ItemManager_GetItemById = il2cpp::get_method(XorStr("Assembly-CSharp"),
			XorStr("ItemData"),
			XorStr(""),
			XorStr("ItemManager"),
			XorStr("GetItemById"),
			XorStr("System.Int32"));

		if (playerId == 0)
		{
			static const uintptr_t PlayerInventory_ForceGiveItem = il2cpp::get_method(XorStr("Assembly-CSharp"),
				XorStr("System.Void"),
				XorStr(""),
				XorStr("PlayerInventory"),
				XorStr("ForceGiveItem"),
				XorStr("ItemData"));

			generic_array* playerInventory = il2cpp::find_objects_of_type(XorStr("PlayerInventory"), XorStr("Assembly-CSharp"));

			if (playerInventory && playerInventory->length)
			{
				void* item = reinterpret_cast<void* (*)(int itemId)>(ItemManager_GetItemById)(itemId);
				reinterpret_cast<void(*)(uintptr_t PlayerInventory, void* item)>
					(PlayerInventory_ForceGiveItem)(playerInventory->items[0], item);
			}
		}
		else
		{
			static const uintptr_t GameServer_ForceGiveWeapon = il2cpp::get_method(XorStr("Assembly-CSharp"),
				XorStr("System.Void"),
				XorStr(""),
				XorStr("GameServer"),
				XorStr("ForceGiveWeapon"),
				XorStr("System.UInt64, System.Int32, System.Int32"));

			reinterpret_cast<void(*)(uintptr_t toClient, int itemId, int objectId)>
				(GameServer_ForceGiveWeapon)(playerId, itemId, 1);
		}
	}

	inline Component* getClosestEnemy()
	{
		if (activePlayers && activePlayers->length)
		{
			closestDst = MAX_FLOAT;
			Component* opm = activePlayers->items[0];
			const vec3 origin = getLocalPos();

			for (int i = 0; i < activePlayers->length; i++)
			{
				vec3 WHeadPos = *(vec3*)(activePlayers->items[i] + off::OnlinePlayerMovement::lastPositon);
				WHeadPos.y += 1.f;

				// IS BEHIND SCREEN CHECK
				vec3 headPos = camera->WorldToScreenPoint(WHeadPos);
				if (headPos.z < 0.3f)
					continue;
				headPos.y = Client::WindowSize.y - headPos.y;

				// IS CLOSES ENEMY
				dst = sqrt((headPos.x - mouse.x) * (headPos.x - mouse.x) + (headPos.y - mouse.y) * (headPos.y - mouse.y));
				if (dst < closestDst)
				{
					closestDst = dst;
					opm = activePlayers->items[i];
				}
			}

			return opm;
		}
		return 0;
	}

	void Cham(Component* cmp, const Color& color)
	{
		generic_array_t<Renderer*>* renderers = cmp->GetComponentsInChildren<Renderer>(XorStr("UnityEngine.Renderer, UnityEngine.CoreModule"));

		if (renderers)
		{
			for (int j = 0; j < renderers->length; j++)
			{
				Material* Material = renderers->items[j]->get_material();

				static Shader* chamShader = Shader::Find(XorStr("Hidden/Internal-Colored"));

				Material->set_shader(chamShader);
				Material->SetInt(XorStr("_ZTest"), 0);
				Material->set_color(color);
			}
		}
	}

	Component* getSteamManager()
	{
		generic_array* SteamManager = il2cpp::find_objects_of_type(XorStr("SteamManager"), XorStr("Assembly-CSharp"));
		if (SteamManager && SteamManager->length)
			return (Component*)SteamManager->items[0];
		return 0;
	}

	uintptr_t getHostId()
	{
		generic_array* SteamManager = il2cpp::find_objects_of_type(XorStr("SteamManager"), XorStr("Assembly-CSharp"));
		if (SteamManager && SteamManager->length)
			return *(uintptr_t*)(SteamManager->items[0] + 0x34);
		return 0;
	}

	Component* getHost()
	{
		for (USHORT i = 0; i < activePlayers->length; i++)
		{
			if (getPlayerId(activePlayers->items[i]) == hostId)
				return activePlayers->items[i];
		}
		return 0;
	}

	uintptr_t getCurrentMapId()
	{
		generic_array* Map = il2cpp::find_objects_of_type(XorStr("LoadingScreen"), XorStr("Assembly-CSharp"));
		if (Map && Map->length)
			return *(uintptr_t*)(Map->items[0] + 0x28);
		return 0;
	}

	void respawnPlayer(Component* pm, float respawnTime)
	{
		static const uintptr_t GameServer_QueueRespawn = il2cpp::get_method(XorStr("Assembly-CSharp"),
			XorStr("System.Void"),
			XorStr(""),
			XorStr("GameServer"),
			XorStr("QueueRespawn"),
			XorStr("System.UInt64, System.Single"));

		if (GameServer_QueueRespawn)
		{
			generic_array* GameServer = il2cpp::find_objects_of_type(XorStr("GameManager"), XorStr("Assembly-CSharp"));
			if (GameServer && GameServer->length)
			{
				if (pm) // a player
				{
					*(bool*)(pm + 0x39) = false;
					reinterpret_cast<void(*)(uintptr_t, uintptr_t, float)>(GameServer_QueueRespawn)
						(GameServer->items[0], *(uintptr_t*)(pm + 0x18), 0.f);
					*(bool*)(pm + 0x39) = false;
				}
				else // local player
				{
					reinterpret_cast<void(*)(uintptr_t, uintptr_t, float)>(GameServer_QueueRespawn)
						(GameServer->items[0], getLocalPlayerId(), 0.f);
					if (playerMovement)
						*(bool*)(playerMovement + 0x40) = false; // is dead to false
				}
			}
		}
	}

	void setPlayerPos(uintptr_t id, vec3 pos)
	{
		static const uintptr_t ServerSend_RespawnPlayer = il2cpp::get_method(XorStr("Assembly-CSharp"),
			XorStr("System.Void"),
			XorStr(""),
			XorStr("ServerSend"),
			XorStr("RespawnPlayer"),
			XorStr("System.UInt64, UnityEngine.Vector3"));
		reinterpret_cast<void(*)(uintptr_t, vec3)>(ServerSend_RespawnPlayer)
			(id, pos);
	}

	int getPlayerNum(Component* opm)
	{
		return *(int*)(*(uintptr_t*)(opm + off::OnlinePlayerMovement::playerManager) + 0x94);
	}

	void killPlayer(Component* pm)
	{
		static const uintptr_t ServerSend_PlayerDied = il2cpp::get_method(XorStr("Assembly-CSharp"),
			XorStr("System.Void"),
			XorStr(""),
			XorStr("ServerSend"),
			XorStr("PlayerDied"),
			XorStr("System.UInt64, System.UInt64, UnityEngine.Vector3"));
		
		reinterpret_cast<void(*)(uintptr_t, uintptr_t, vec3)>(ServerSend_PlayerDied)
			(*(uintptr_t*)(pm + 0x18), 0, {});
		*(bool*)(pm + 0x39) = true; // set dead to true
	}

	Component* getPlayerPm(Component* opm)
	{
		return *(Component**)(opm + 0x18);
	}

	GameModeState getGameModeState()
	{
		generic_array* GameMode = il2cpp::find_objects_of_type(XorStr("GameMode"), XorStr("Assembly-CSharp"));
		if (GameMode && GameMode->length)
			return (GameModeState)*(int*)(GameMode->items[0] + 0x20);
		return (GameModeState)0xFF;
;	}

	std::vector<MonoBehaviour*> getAllPlayers()
	{
		static const uintptr_t PlayerList_Toggle = il2cpp::get_method(XorStr("Assembly-CSharp"),
			XorStr("System.Void"),
			XorStr(""),
			XorStr("PlayerList"),
			XorStr("Toggle"),
			XorStr("System.Boolean"));

		// UPDATE LIST
		generic_array* PlayerList = il2cpp::find_objects_of_type(XorStr("PlayerList"), XorStr("Assembly-CSharp"));
		if (!PlayerList || !PlayerList->length || !PlayerList_Toggle)
			return {};

		reinterpret_cast<void(*)(uintptr_t, bool)>(PlayerList_Toggle)
			(PlayerList->items[0], true);

		// GET PLAYERS
		auto players = (generic_array_t<MonoBehaviour*>*)il2cpp::find_objects_of_type(XorStr("PlayerListingPrefab"), XorStr("Assembly-CSharp"));
		
		std::vector<MonoBehaviour*> pmPlayers(players->length);
		for (USHORT i = 0; i < players->length; i++)
			pmPlayers[i] = *(MonoBehaviour**)(players->items[i] + 0x40);

		reinterpret_cast<void(*)(uintptr_t, bool)>(PlayerList_Toggle)
			(PlayerList->items[0], false);

		return pmPlayers;
	}
}
