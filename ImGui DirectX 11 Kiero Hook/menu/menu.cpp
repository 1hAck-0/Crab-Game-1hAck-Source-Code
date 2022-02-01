#include "pch.h"
#include "menu.h"
#include "../utils/config.h"
#include "../utils/fonts.h"
#include "../utils/icons.h"
#include "../utils/cgdata.h"
#include "../cheat/cheat.h"
#include "../il2cpp/il2cpp_loader.h"
#include "../utils/json.hpp"
#include <fstream>
#include <urlmon.h>
#include <shlobj_core.h> // for the folder dialog
#include "lua-scripts/lua_handler.h"
#pragma comment(lib, "urlmon.lib")


void ImGui_HotkeyButton(USHORT& dst, const char* const label, const char* new_button_id, bool& get_key)
{
	std::string button;
	switch (dst)
	{
	case 0xADAF:
		button += XorStr("NONE");
		break;
	case VK_ESCAPE:
		button += XorStr("ESC");
		break;
	case VK_TAB:
		button += XorStr("TAB");
		break;
	case VK_CAPITAL:
		button += XorStr("CAPS");
		break;
	case VK_SHIFT:
		button += XorStr("SHIFT");
		break;
	case VK_CONTROL:
		button += XorStr("CTRL");
		break;
	case VK_MENU:
		button += XorStr("ALT");
		break;
	case VK_XBUTTON1:
		button += XorStr("MB 1");
		break;
	case VK_XBUTTON2:
		button += XorStr("MB 2");
		break;
	case VK_F1:
		button += XorStr("F1");
		break;
	case VK_F2:
		button += XorStr("F2");
		break;
	case VK_F3:
		button += XorStr("F3");
		break;
	case VK_F4:
		button += XorStr("F4");
		break;
	case VK_F5:
		button += XorStr("F5");
		break;
	case VK_F6:
		button += XorStr("F6");
		break;
	case VK_F7:
		button += XorStr("F7");
		break;
	case VK_F8:
		button += XorStr("F8");
		break;
	case VK_F9:
		button += XorStr("F9");
		break;
	case VK_F10:
		button += XorStr("F10");
		break;
	case VK_F11:
		button += XorStr("F11");
		break;
	case VK_F12:
		button += XorStr("F12");
		break;
	case VK_F13:
		button += XorStr("F13");
		break;
	case VK_F14:
		button += XorStr("F14");
		break;
	case VK_F15:
		button += XorStr("F15");
		break;
	case VK_F16:
		button += XorStr("F16");
		break;
	case VK_F17:
		button += XorStr("F17");
		break;
	case VK_F18:
		button += XorStr("F18");
		break;
	case VK_F19:
		button += XorStr("F19");
		break;
	case VK_F20:
		button += XorStr("F20");
		break;
	case VK_F21:
		button += XorStr("F21");
		break;
	case VK_F22:
		button += XorStr("F22");
		break;
	case VK_F23:
		button += XorStr("F23");
		break;
	case VK_F24:
		button += XorStr("F24");
		break;
	case VK_HOME:
		button += XorStr("HOME");
		break;
	case VK_DELETE:
		button += XorStr("DELETE");
		break;
	case VK_INSERT:
		button += XorStr("INSERT");
		break;
	case VK_END:
		button += XorStr("END");
		break;
	case VK_SPACE:
		button += XorStr("SPACE");
		break;
	case VK_SCROLL:
		button += XorStr("SCROLL");
		break;
	case VK_LBUTTON:
		button += XorStr("LCLICK");
		break;
	case VK_RBUTTON:
		button += XorStr("RCLICK");
		break;
	case VK_RETURN:
		button += XorStr("ENTER");
		break;
	case VK_NUMPAD0:
		button += XorStr("NUM0");
		break;
	case VK_NUMPAD1:
		button += XorStr("NUM1");
		break;
	case VK_NUMPAD2:
		button += XorStr("NUM2");
		break;
	case VK_NUMPAD3:
		button += XorStr("NUM3");
		break;
	case VK_NUMPAD4:
		button += XorStr("NUM4");
		break;
	case VK_NUMPAD5:
		button += XorStr("NUM5");
		break;
	case VK_NUMPAD6:
		button += XorStr("NUM6");
		break;
	case VK_NUMPAD7:
		button += XorStr("NUM7");
		break;
	case VK_NUMPAD8:
		button += XorStr("NUM8");
		break;
	case VK_NUMPAD9:
		button += XorStr("NUM9");
		break;
	case 0:
		button += XorStr("...");
		break;

	default:
		button.push_back((char)dst);
	}
	button += new_button_id;

	if (ImGui::Button(button.c_str(), ImVec2(70, 0)))
		get_key = true;
	if (ImGui::IsItemHovered())
	{
		if (dst != 0xADAF)
			ImGui::SetTooltip(XorCStr("Double Click to Reset"));
		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			get_key = false;
			dst = 0xADAF;
		}
	}

	ImGui::SameLine();
	ImGui::Text(label);

	if (get_key)
	{
		dst = 0;
		for (byte i = 1; i < 0xFF; i++)
		{
			if (GetAsyncKeyState(i) & 0x8000)
			{
				get_key = false;
				dst = i;
				break;
			}
		}
	}
}

namespace ImGui
{
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}
}

namespace Menu
{
	bool bInit = true;
	bool bActive = true;
	bool bShowMenu = true;
	bool bLockInPlace = false;
	byte tab = 0;

	USHORT ToggleKey = VK_F1;
	USHORT CloseKey = VK_F4;

	namespace fs = std::filesystem;
	auto settings = &cfg.script;
	auto editor = &settings->scriptEditor;

#pragma region scripts stuff
	void RefreshScripts()
	{
		if (!settings->bOpenedFolder)
		{
			settings->files.clear();
			return;
		}

		if (fs::exists(settings->OpenedFolder))
		{
			for (const auto& dir : fs::directory_iterator(settings->OpenedFolder))
			{
				try
				{
					if (!dir.is_directory())
					{
						auto hasEnding = [](std::string const& fullString, std::string const& ending)->bool
						{
							if (fullString.length() >= ending.length())
								return !fullString.compare(fullString.length() - ending.length(), ending.length(), ending);
							return true;
						};

						bool bFound = false;
						const fs::path& path = dir.path();
						if (!hasEnding(path.string().c_str(), XorCStr(".lua")))
							continue;

						for (int i = 0; i < settings->files.size(); i++)
						{
							const auto& script = settings->files[i];
							if (script.path == path)
							{
								bFound = true;
								break;
							}
						}

						if (!bFound)
						{
							ScriptFile file;
							file.path = path;

							settings->files.push_back(file);
						}
					}
				}
				catch (...) {}
			}
		}
		else
		{
			settings->bOpenedFolder = false;
		}

		for (int i = 0; i < settings->files.size(); i++)
		{
			const auto& script = settings->files[i];
			if (!fs::exists(script.path) || script.path.parent_path() != settings->OpenedFolder)
			{
				settings->files.erase(settings->files.begin() + i);
				i--;
			}
		}
	}

	DWORD __stdcall OpenFolderDialog(void*)
	{
		wchar_t szTitle[MAX_PATH] = { 0 };
		BROWSEINFOW dialog = { 0 };
		dialog.lpszTitle = XorCStr(L"Open Script Folder");
		dialog.pszDisplayName = szTitle;
		dialog.hwndOwner = Client::hwnd;

		if (LPITEMIDLIST pidl = SHBrowseForFolderW(&dialog))
		{
			SHGetPathFromIDListW(pidl, szTitle);
			settings->bOpenedFolder = true;
			wcscpy_s(settings->OpenedFolder, szTitle);
			RefreshScripts();
		}

		return 0;
	}
	DWORD __stdcall OpenSaveFileDialog(void* bCloseEditorAfterSave)
	{
		wchar_t Filename[MAX_PATH] = { 0 };

		OPENFILENAMEW dialog = { sizeof(OPENFILENAMEW) };
		dialog.hwndOwner = Client::hwnd;
		dialog.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
		dialog.nMaxFile = MAX_PATH;
		dialog.lpstrFile = Filename;
		dialog.lpstrTitle = XorCStr(L"Save 1hAck Lua Script");
		dialog.lpstrFilter = dialog.lpstrDefExt = XorCStr(L"Lua Script (*.lua)\0*.lua\0\0");
		dialog.lpstrInitialDir = settings->OpenedFolder;

		if (GetSaveFileNameW(&dialog))
		{
			std::ofstream file;
			file.open(Filename, std::ios::out);
			if (file.fail() || !file.is_open())
			{
				MessageBoxA(Client::hwnd, XorCStr("Couldn't create or open the file!"), XorCStr("File Error"), MB_ICONERROR);
				return 0;
			}

			std::string text = editor->GetText();
			if (text[text.length() - 1] == '\n') text.pop_back();
			file << text;

			file.close();

			if (!settings->bOpenedFolder)
			{
				try
				{
					wcscpy_s(settings->OpenedFolder, fs::path(Filename).parent_path().wstring().c_str());
					settings->bOpenedFolder = true;
				}
				catch (...) {}
			}

			if (settings->bNewScript)
			{
				settings->currentScipt.release();
				settings->currentScipt = std::make_unique<ScriptFile>();
				settings->currentScipt->path = Filename;

				strcpy_s(settings->currentScriptName, settings->currentScipt->path.filename().string().c_str());

				settings->bNewScript = false;
			}

			RefreshScripts();

			if (bCloseEditorAfterSave)
				settings->bScriptEditor = false;
		}

		return 0;
	}

	__forceinline bool StringToFile(const std::string& fileName, std::string& text)
	{
		std::ofstream file;
		file.open(fileName, std::ios::out);
		if (file.fail() || !file.is_open())
			return false;
		if (text[text.length() - 1] == '\n') text.pop_back();
		file << text;
		file.close();
		return true;
	}

	void OnScriptToggled(ScriptFile* script)
	{
		// ENABLING
		if (script->bEnabled)
		{
			if (!script->Init())
			{
				script->bEnabled = false;
				try
				{
					std::string message = XorStr("Lua Error: ");
					message += script->GetLastError();

					MBCallback cl = {
						message.c_str(),
						XorCStr("Lua Script Error"),
						MB_ICONERROR
					};

					script->Uninit();
				}
				catch (...) {}

				return;
			}

			bool validScript = false;
			if (script->DoesFuncnExist(XorCStr("OnEnable")))
			{
				script->flags ^= ScriptFlags_StatusChanged;
				validScript = true;
			}

			script->flags |= ScriptFlags_HasUpdate * script->DoesFuncnExist(XorCStr("Update"));
			if (script->flags & ScriptFlags_HasUpdate)
				validScript = true;

			if (!validScript)
			{
				script->bEnabled = false;
				MBCallback cl = {
					XorCStr("The script must have at least 'void OnEnable(void)' or 'void Update(void)' functions!"),
					XorCStr("Invalid 1hAck Script"),
					MB_ICONERROR
				};
				script->Uninit();
				return;
			}

			script->flags |= ScriptFlags_HasRender * script->DoesFuncnExist(XorCStr("OnRender"));

			if (script->DoesFuncnExist(XorCStr("OnGUIEnable")))
			{
				script->CallFunc(XorCStr("OnGUIEnable"));
				if (!script->bEnabled)
					script->Uninit();
			}
			return;
		}

		if (script->DoesFuncnExist(XorCStr("OnGUIDisable")))
		{
			script->CallFunc(XorCStr("OnGUIDisable"));
			if (script->bEnabled)
				return;
		}

		// DISABLING
		if (script->DoesFuncnExist(XorCStr("OnDisable")))
		{
			script->flags ^= ScriptFlags_StatusChanged;
		}
		else
		{
			script->flags &= ~(ScriptFlags_StatusChanged);
			script->Uninit();
		}
	}
#pragma endregion

	void RenderScriptsWindow()
	{
		// BEGIN WINDOW
		static bool bInitedMainWindow = false;
		if (!bInitedMainWindow)
		{
			ImGui::SetNextWindowSize(ImVec2(600, 300));
			ImGui::SetNextWindowPos({ float(Client::WindowCenter.x) - 300, float(Client::WindowCenter.y) - 150 });

			bInitedMainWindow = true;
		}
		ImGui::Begin(XorCStr(ICON_FA_CODE" Scripts"), &settings->bScriptsWindow, ImGuiWindowFlags_NoCollapse);

		// WINDOW POPUP
		static bool bNewScriptPopup = false;
		if (ImGui::BeginPopupContextWindow())
		{
			if (settings->bOpenedFolder)
			{
				// SHOW IN FOLDER
				if (ImGui::MenuItem(XorCStr(ICON_FA_FOLDER " Show in Folder")))
					ShellExecuteW(0, 0, settings->OpenedFolder, 0, 0, SW_SHOWNORMAL);

				// CLOSE FOLDER
				if (ImGui::MenuItem(XorCStr("Close Folder")))
				{
					settings->bOpenedFolder = false;
					RefreshScripts();
				}
			}

			// NEW SCRIPT
			if (ImGui::MenuItem(XorCStr(ICON_FA_PLUS " New Script##menu item")))
			{
				bNewScriptPopup = true;
				ImGui::CloseCurrentPopup();
			}

			// REFRESH
			if (ImGui::MenuItem(XorCStr(ICON_FA_UNDO" Refresh")))
				RefreshScripts();

			ImGui::EndPopup();
		}

		// NEW SCRIPT POPUP
		static int newScriptTemp = 0;
		if (bNewScriptPopup)
		{
			if (ImGui::BeginPopupModal(XorCStr("New Script##popup"), &bNewScriptPopup, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
			{
				if (ImGui::IsWindowAppearing())
				{
					newScriptTemp = 0;
				}

				ImGui::SetNextItemWidth(120);
				ImGui::Combo(XorCStr("Script Template"), (int*)&newScriptTemp, XorCStr("Loop\0Toggle\0None"));

				ImGui::Spacing();
				ImGui::Spacing();

				// CREATE
				if (ImGui::Button(XorCStr("Create")))
				{
					ImGui::CloseCurrentPopup();
					bNewScriptPopup = false;

					settings->bNewScript = true;
					strcpy_s(settings->currentScriptName, XorCStr("New Script.lua"));
					settings->bScriptEditor = true;
					ImGui::OpenPopup(XorCStr("###script editor"));
				}

				ImGui::SameLine();

				// CANCEL
				if (ImGui::Button(XorCStr("Cancel")))
				{
					ImGui::CloseCurrentPopup();
					bNewScriptPopup = false;
				}

				ImGui::EndPopup();
			}
			else
				ImGui::OpenPopup(XorCStr("New Script##popup"));
		}

		// NEW SCRIPT
		if (ImGui::Button(XorCStr(ICON_FA_PLUS" New Script")))
			bNewScriptPopup = true;
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(XorCStr("NOTE: The menu has a built in text-editor but\nI highly recommend using VS Code or another editor"));

		ImGui::SameLine();

		// OPEN FOLDER
		if (ImGui::Button(XorCStr(ICON_FA_FOLDER_OPEN" Open Folder")))
		{
			CreateThread(0, 0, OpenFolderDialog, 0, 0, 0);
		}

		// FOLDER OPTIONS
		if (settings->bOpenedFolder)
		{
			ImGui::SameLine();

			// CLOSE FOLDER
			if (ImGui::Button(XorCStr("Close Folder")))
			{
				settings->bOpenedFolder = false;
				RefreshScripts();
			}

			ImGui::SameLine();

			// REFRESH
			if (ImGui::Button(XorCStr(ICON_FA_REDO" Refresh")))
				RefreshScripts();
		}

		ImGui::SameLine();

		// HELP
		if (ImGui::Button(XorCStr(ICON_FA_QUESTION " Help")))
			ShellExecuteA(0, 0, XorCStr("https://github.com/1hAck-0/CG-Scripts-Community"), 0, 0, SW_SHOWNORMAL);

		ImGui::Separator();
		ImGui::Spacing();

		// SHOW SCRIPTS
		static bool bScriptSettingsPopup = false;
		if (settings->bOpenedFolder)
		{
			for (USHORT i = 0; i < settings->files.size(); i++)
			{
				auto script = &settings->files[i];
				std::string Filename = script->path.filename().string();

				ImGui::PushID(i);

				// SCRIPT NAME
				USHORT avWidth = ImGui::GetContentRegionAvailWidth();
				ImGui::Text(Filename.c_str());
				if (ImGui::BeginPopupContextItem(XorCStr("##script popup")))
				{
					if (ImGui::MenuItem(XorCStr(ICON_FA_TRASH" Delete")))
					{
						fs::remove(script->path);
						RefreshScripts();
					}

					ImGui::EndPopup();
				}

				ImGui::SameLine();

				// EDIT SCRIPT
				if (ImGui::Button(XorCStr(ICON_FA_EDIT"##edit script")))
				{
					try
					{
						strcpy_s(settings->currentScriptName, Filename.c_str());
						settings->currentScipt.release();
						settings->currentScipt = std::unique_ptr<ScriptFile>(script);
						settings->bNewScript = false;
						settings->bScriptEditor = true;
					}
					catch (...) {}
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("Edit Script"));

				ImGui::SameLine();

				// ENABLE SCRIPT
				if (ImGui::Checkbox(XorCStr("##script enabled"), &script->bEnabled))
					OnScriptToggled(script);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("Toggle Script"));

				// SCRIPT SETTINGS
				if (script->bEnabled)
				{
					ImGui::SameLine();

					if (ImGui::Button(XorCStr(ICON_FA_COGS "##script settings")))
					{
						settings->GUIScript = script;
						bScriptSettingsPopup = true;
					}
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip(XorCStr("Script Settings"));
				}

				// SCRIPT HOTKEYKEY
				ImGui::SameLine();
				ImGui_HotkeyButton(script->hotkey, XorCStr("Script Hotkey"), XorCStr("##script hotkey"), script->bGetKey);

				ImGui::PopID();
			}

			// NO SCRIPTS
			if (!settings->files.size())
			{
				ImGui::Text(XorCStr("No LUA scripts found in the current folder"));
			}
		}

		// SCRIPT EDITOR
		static bool bSaveScript = false;
		static bool bInitOpenedScript = false;
		switch (settings->bScriptEditor)
		{
		case true:
		{
			std::string winName = settings->currentScriptName;
			winName += XorCStr("###script editor");

			if (ImGui::BeginPopupModal(winName.c_str(), &settings->bScriptEditor, ImGuiWindowFlags_NoCollapse))
			{
				if (ImGui::IsWindowAppearing())
				{
					ImGui::SetWindowSize(ImVec2(1200, 500));
				}

				// SETUP EDITOR (ONCE PER SESSION)
				if (!settings->bInitedEditor)
				{
					ImGui::SetWindowSize(ImVec2(500, 300));

					editor->SetPalette(TextEditor::GetDarkPalette());
					editor->SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
					settings->bInitedEditor = true;
				}

				auto script = settings->currentScipt.get();

				// SETUP EDITOR TEXT (ONCE PER TEXT EDITOR OPENED)
				if (!bInitOpenedScript)
				{
					// IMPORT EXISTING SCRIPT
					if (!settings->bNewScript)
					{
						if (script)
						{
							if (fs::exists(script->path))
							{
								std::ifstream file;
								file.open(script->path, std::ios::binary | std::ios::ate);

								file.seekg(0, std::ios::end);
								size_t size = file.tellg();
								if (size > 0)
								{
									std::string buffer(size, ' ');
									file.seekg(0);
									file.read(&buffer[0], size);

									editor->SetText(buffer);
								}

								file.close();
								bInitOpenedScript = true;
							}
							else
							{
								settings->bScriptEditor = false;
								RefreshScripts();
							}
						}
						else
						{
							editor->SetText(XorCStr("Error Loading File"));
							bInitOpenedScript = true;
						}
					}

					// NEW SCRIPT
					else
					{
						switch (newScriptTemp)
						{
						case 0: // LOOP SCRIPT TEMP
							editor->SetText(XorCStr(R"(
function Update()
	-- This code will run every tick in Crab Game if the script is on
end

function OnGUI()
	-- Code for the Script settings GUI
end

function OnRender()
	-- This code will run once per frame, use for rendering lines, rectangles...
end
)"));
							break;

						case 1: // TOGGLE SCRIPT TEMP
							editor->SetText(XorCStr(R"(
function OnEnable()
	-- This code will run once when the script is turned ON
end

function OnDisable()
	-- This code will run once when the script is turned OFF
end

function OnGUI()
	-- Code for the Script settings GUI
end

function OnRender()
	-- This code will run once per frame, use for rendering lines, rectangles...
end
)"));
							break;

						case 2: // NONE
							editor->SetText(XorCStr("\n\n"));
							break;
						}
						bInitOpenedScript = true;
					}
				}

				// RENDER WINDOW
				else
				{
					auto SaveScript = [](ScriptFile* script)->bool
					{
						if (settings->bNewScript)
						{
							CreateThread(0, 0, (LPTHREAD_START_ROUTINE)OpenSaveFileDialog, 0, 0, 0);
							return false;
						}
						else if (script)
						{
							std::ofstream file;
							file.open(script->path, std::ios::out);
							if (file.fail() || !file.is_open())
							{
								MessageBoxA(0, XorCStr("Couldn't save the file! Please make sure the parent directory exists!\n\nIf you still get the error, you can save it as a new file."), XorCStr("File Error"), MB_ICONERROR);
								return false;
							}
							else
							{
								std::string text = editor->GetText();
								if (text[text.length() - 1] == '\n') text.pop_back();
								file << text;

								file.close();
							}
						}

						return true;
					};

					if (ImGui::Button(XorCStr(ICON_FA_SAVE"##save script")))
					{
						SaveScript(script);
					}
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip(XorCStr("Save"));

					ImGui::SameLine();

					if (ImGui::Button(XorCStr(ICON_FA_PLUS ICON_FA_SAVE"##save as script")))
					{
						CreateThread(0, 0, (LPTHREAD_START_ROUTINE)OpenSaveFileDialog, 0, 0, 0);
					}
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip(XorCStr("Save As"));

					ImGui::SameLine();

					if (ImGui::Button(XorCStr(ICON_FA_COPY"## copy script")))
					{
						ImGui::SetClipboardText(editor->GetText().c_str());
					}
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip(XorCStr("Copy"));

					ImGui::SameLine();

					if (ImGui::Button(XorCStr(ICON_FA_QUESTION " Help")))
						ShellExecuteA(0, 0, XorCStr("https://github.com/1hAck-0/CG-Scripts-Community"), 0, 0, SW_SHOWNORMAL);

					ImGui::SameLine(ImGui::GetWindowWidth() - 235);

					if (script)
					{
						if (ImGui::Button(script->bEnabled ? XorCStr(ICON_FA_STOP"##script") : XorCStr(ICON_FA_PLAY"##script")))
						{
							if (SaveScript(script))
							{
								script->bEnabled = !script->bEnabled;
								OnScriptToggled(script);
							}
						}
						if (ImGui::IsItemHovered())
							ImGui::SetTooltip(script->bEnabled ? XorCStr("Stop") : XorCStr("Play"));
					}

					ImGui::SameLine();

					ImGui::Checkbox(XorCStr("Ask To Save On Leave"), &cfg.script.bGUISaveOnClose);

					ImGui::Spacing();

					editor->Render(XorCStr("##script text editor"), ImVec2(), true);
				}

				ImGui::EndPopup();
			}
			else
			{
				bInitOpenedScript = false;
				ImGui::OpenPopup(XorCStr("###script editor"));
			}

			// ON CLOSING
			if (!settings->bScriptEditor && cfg.script.bGUISaveOnClose)
			{
				if (settings->bNewScript)
				{
					MBCallback cl = {
						XorCStr("Do you want to save the script?"),
						XorCStr("1hAck Script"),
						MB_YESNOCANCEL | MB_ICONQUESTION,
						[](int id) {
							switch (id)
							{
							case IDYES: settings->bScriptEditor = true; CreateThread(0, 0, (LPTHREAD_START_ROUTINE)OpenSaveFileDialog, 0, 0, 0); break;
							case IDCANCEL: settings->bScriptEditor = true; break;
							}
						}
					};
				}
				else if (settings->currentScipt.get())
				{
					MBCallback cl = {
						XorCStr("Save before leaving?"),
						XorCStr("1hAck Script"),
						MB_YESNOCANCEL | MB_ICONQUESTION,
						[](int id) {
							switch (id)
							{
							case IDYES:
							{
								auto script = settings->currentScipt.get();
								std::string text = editor->GetText();
								if (script && !StringToFile(script->path.string(), text))
								{
									MBCallback cl = {
										"Couldn't save the file!",
										"File Error",
										MB_ICONERROR
									};
								}
								break;
							}

							case IDCANCEL: settings->bScriptEditor = true; break;
							}
						}
					};
				}
			}
		}
		break;

		default:
			bInitOpenedScript = false;
		}

		// SCRIPT SETTINGS
		switch (bScriptSettingsPopup) // using switch so I can break out of the statement
		{
		case true:
			if (!settings->GUIScript || !settings->GUIScript->lua)
			{
				bScriptSettingsPopup = false;
				break;
			}

			std::string winName = settings->GUIScript->path.filename().string().c_str();
			winName += XorCStr(" Settings###script settings popup");

			if (ImGui::BeginPopupModal(winName.c_str(), &bScriptSettingsPopup, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
			{
				if (!settings->GUIScript->DoesFuncnExist(XorCStr("OnGUI")))
				{
					ImGui::Text(XorCStr("This script has no 'void OnGUI(void)' function and therefore no settings!"));
				}
				else
				{
					settings->GUIScript->CallFunc(XorCStr("OnGUI"));
				}

				ImGui::Spacing();
				ImGui::Spacing();

				if (ImGui::Button(XorCStr("OK")))
				{
					bScriptSettingsPopup = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
			else
				ImGui::OpenPopup(XorCStr("###script settings popup"));
		}

		ImGui::End();
	}

	bool Render()
	{
		if (Menu::ToggleKey != 0xADAF && PressKey(Menu::ToggleKey))
			Menu::bShowMenu = !Menu::bShowMenu, bMenuStatusChanged = true;

		// GUI
		if (Menu::bShowMenu)
		{
			std::string windowName = XorStr("Crab Game Mod Menu | by 1hAck | ").c_str();
			{
				char buffer[10];
				sprintf_s(buffer, XorStr("%.0f").c_str(), ImGui::GetIO().Framerate);
				windowName += buffer;
				windowName += XorStr("fps###main window").c_str();
			}

			static bool bMenuSettings = false;

			ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
			switch (tab)
			{
			case 0:
				ImGui::SetNextWindowSize(ImVec2(1200, 310));
				break;

			case 2:
				ImGui::SetNextWindowSize(ImVec2(600, 310));
				break;

			case 3:
				if (bMenuSettings)
				{
					ImGui::SetNextWindowSize(ImVec2(720, 310));
					break;
				}

			default:
				ImGui::SetNextWindowSize(ImVec2(400, 310));
			}

			ImGui::Begin(windowName.c_str(), & bActive,
				ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | (Menu::bLockInPlace ? ImGuiWindowFlags_NoMove : 0));

			// STYLE
			ImGuiStyle& style = ImGui::GetStyle();
			style.FrameRounding = cfg.MenuRounding;
			ImColor color;

			// COLOR
			if (cfg.bMenuRGB)
			{
				static bool bStartFromRed = true;
				RGBEffect(bStartFromRed, cfg.MenuRGBSpeed, cfg.MenuRGB);
				color = ImColor((int)cfg.MenuRGB[0], (int)cfg.MenuRGB[1], (int)cfg.MenuRGB[2], 170);

				style.Colors[ImGuiCol_TitleBg] = { color.Value.x, color.Value.y, color.Value.z, .6f };
				style.Colors[ImGuiCol_TitleBgActive] = { color.Value.x, color.Value.y, color.Value.z, .8f };
			}
			else
			{
				color = *(ImVec4*)&cfg.MenuColor;

				style.Colors[ImGuiCol_TitleBg] = { color.Value.x, color.Value.y, color.Value.z, .8f };
				style.Colors[ImGuiCol_TitleBgActive] = { color.Value.x, color.Value.y, color.Value.z, 1.f };
			}

			style.Colors[ImGuiCol_Button] = { color.Value.x, color.Value.y, color.Value.z, .8f };
			style.Colors[ImGuiCol_ButtonHovered] = color;
			style.Colors[ImGuiCol_Border] = color;
			style.Colors[ImGuiCol_TextDisabled] = { color.Value.x, color.Value.y, color.Value.z, .4f };
			style.Colors[ImGuiCol_CheckMark] = { color.Value.x, color.Value.y, color.Value.z, .8f };
			style.Colors[ImGuiCol_SliderGrab] = color;
			style.Colors[ImGuiCol_Header] = color;
			style.Colors[ImGuiCol_Separator] = color;
			style.Colors[ImGuiCol_ScrollbarGrab] = { color.Value.x, color.Value.y, color.Value.z, .8f };
			//style.Colors[ImGuiCol_ModalWindowDimBg] = { color.Value.x, color.Value.y, color.Value.z, .01f };

			// COLUMNS
			switch (tab)
			{
			case 0:
				ImGui::Columns(6);
				ImGui::SetColumnOffset(1, 120);
				ImGui::SetColumnOffset(2, 310);
				ImGui::SetColumnOffset(3, 500);
				ImGui::SetColumnOffset(4, 700);
				ImGui::SetColumnOffset(5, 900);
				break;
				
			case 2:
				ImGui::Columns(3);
				ImGui::SetColumnOffset(1, 120);
				ImGui::SetColumnOffset(2, 370);
				break;

			case 3:
				if (bMenuSettings)
				{
					ImGui::Columns(3);
					ImGui::SetColumnOffset(1, 120);
					ImGui::SetColumnOffset(2, 400);
					break;
				}

			default:
				ImGui::Columns(2);
				ImGui::SetColumnOffset(1, 120);
			}

			// TABS
			{
				ImGui::PushStyleColor(ImGuiCol_Button, 0);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, 0);

				static byte hoveredTab = -1;
				static constexpr const POINT tabSize = { 100, 64 };

				// MAIN
				ImGui::PushStyleColor(ImGuiCol_Text, hoveredTab == 0 ? ImVec4(color.Value.x, color.Value.y, color.Value.z, 0.6f) : (tab == 0 ? ImVec4(color.Value.x, color.Value.y, color.Value.z, 0.8f) : ImVec4(1.f, 1.f, 1.f, 1.f)));
				if (ImGui::Button(XorStr(ICON_FA_USER" Main###tab0").c_str(), ImVec2(tabSize.x, tabSize.y)))
					tab = 0;
				if (ImGui::IsItemHovered())
					hoveredTab = 0;
				ImGui::PopStyleColor();

				// VISUALS
				ImGui::PushStyleColor(ImGuiCol_Text, hoveredTab == 1 ? ImVec4(color.Value.x, color.Value.y, color.Value.z, 0.6f) : (tab == 1 ? ImVec4(color.Value.x, color.Value.y, color.Value.z, 0.8f) : ImVec4(1.f, 1.f, 1.f, 1.f)));
				if (ImGui::Button(XorStr(ICON_FA_EYE" Visuals###tab1").c_str(), ImVec2(tabSize.x, tabSize.y)))
					tab = 1;
				if (ImGui::IsItemHovered())
					hoveredTab = 1;
				ImGui::PopStyleColor();

				// COMBAT
				ImGui::PushStyleColor(ImGuiCol_Text, hoveredTab == 2 ? ImVec4(color.Value.x, color.Value.y, color.Value.z, 0.6f) : (tab == 2 ? ImVec4(color.Value.x, color.Value.y, color.Value.z, 0.8f) : ImVec4(1.f, 1.f, 1.f, 1.f)));
				if (ImGui::Button(XorStr(ICON_FA_CROSSHAIRS" Combat###tab2").c_str(), ImVec2(tabSize.x, tabSize.y)))
					tab = 2;
				if (ImGui::IsItemHovered())
					hoveredTab = 2;
				ImGui::PopStyleColor();

				// MENU
				ImGui::PushStyleColor(ImGuiCol_Text, hoveredTab == 3 ? ImVec4(color.Value.x, color.Value.y, color.Value.z, 0.6f) : (tab == 3 ? ImVec4(color.Value.x, color.Value.y, color.Value.z, 0.8f) : ImVec4(1.f, 1.f, 1.f, 1.f)));
				if (ImGui::Button(XorStr(ICON_FA_COGS" Menu###tab3").c_str(), ImVec2(tabSize.x, tabSize.y)))
					tab = 3;
				if (ImGui::IsItemHovered())
					hoveredTab = 3;
				ImGui::PopStyleColor();

				if (!ImGui::IsAnyItemHovered())
					hoveredTab = -1;
				ImGui::PopStyleColor(3);
			}

			ImGui::NextColumn();
			ImGui::Spacing();
			
			// OPENED TAB
			struct tmodalwin
			{
				std::string winLabel;
				void(*GUICallback)() = nullptr;
				bool bGetKey = false;

				tmodalwin() {}

				inline void Popup() const { ImGui::OpenPopup(XorCStr("###set modal")); }

				inline void Show()
				{
					std::string buffer = winLabel + XorStr("###set modal");
					if (ImGui::BeginPopupModal(buffer.c_str(), 0, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize))
					{
						if (GUICallback)
							GUICallback();

						ImGui::Spacing();

						if (ImGui::Button(XorCStr(ICON_FA_CHECK" OK")) && !bGetKey)
							ImGui::CloseCurrentPopup();

						ImGui::EndPopup();
					}
				}
			};
			static tmodalwin modalwin_;

			switch (tab)
			{
			// MAIN
			case 0:
			{
				#define IMEDIT(id, offset) ImGui::SameLine(offset); if (ImGui::Button(XorCStr(ICON_FA_EDIT id)))
				static constexpr const int firstEditOffset = 150;

				// MOVEMENT COLUMN

				// FLYHACK
				ImGui::Checkbox(XorStr("Flyhack").c_str(), &cfg.player.bFlyhack);
				IMEDIT("##Flyhack", firstEditOffset)
				{
					modalwin_.winLabel = XorStr("Flyhack");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.flyKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
						ImGui::SliderFloat(XorCStr("Fly Speed"), &cfg.player.flySpeed, 1000.f, 10000.f, XorCStr("%.2f"));
					};
					modalwin_.Popup();
				}

				// NO CLIP
				if (ImGui::Checkbox(XorStr("No Clip").c_str(), &cfg.player.bNoClip))
					cfg.player.bSetNoClip = true;
				IMEDIT("##No Clip", firstEditOffset)
				{
					modalwin_.winLabel = XorStr("No Clip");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.noClipKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
					};
					modalwin_.Popup();
				}

				// SPEEDHACK
				ImGui::Checkbox(XorStr("Speedhack").c_str(), &cfg.player.bSpeedHack);
				IMEDIT("##Speedhack", firstEditOffset)
				{
					modalwin_.winLabel = XorStr("Speedhack");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.speedKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
						ImGui::SliderFloat(XorCStr("Speed"), &cfg.player.Speed, 6.5f, 200.f, XorCStr("%.2f"));
					};
					modalwin_.Popup();
				}

				// SUPER JUMP
				ImGui::Checkbox(XorStr("Super Jump").c_str(), &cfg.player.bSuperJump);
				IMEDIT("##Super Jump", firstEditOffset)
				{
					modalwin_.winLabel = XorStr("Jump Force");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.superJumpKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
						ImGui::SliderFloat(XorCStr("Jump Force"), &cfg.player.jumpForce, 400.f, 1500.f, XorCStr("%.2f"));
					};
					modalwin_.Popup();
				}

				// INFINITE JUMP
				ImGui::Checkbox(XorStr("Infinite Jump").c_str(), &cfg.player.bInfiniteJump);
				IMEDIT("##Infinite Jump", firstEditOffset)
				{
					modalwin_.winLabel = XorStr("Infinite Jump");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.infJumpKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
					};
					modalwin_.Popup();
				}

				// BUNNYHOP
				ImGui::Checkbox(XorStr("Bunnyhop").c_str(), &cfg.player.bBunnyhop);
				IMEDIT("##Bunnyhop", firstEditOffset)
				{
					modalwin_.winLabel = XorStr("Bunnyhop");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.bunKey, XorCStr("Hold Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
					};
					modalwin_.Popup();
				}

				// ANTI PUSH
				ImGui::Checkbox(XorStr("Anti Punch").c_str(), &cfg.player.bAntiPunch);
				IMEDIT("##Anti Punch", firstEditOffset)
				{
					modalwin_.winLabel = XorStr("Anti Punch");
					modalwin_.GUICallback = []()->void
					{
						ImGui::SliderFloat(XorCStr("Anti Force"), &cfg.player.antiForce, 0.f, 1.f, XorCStr("%.2f"));
						ImGui::Checkbox(XorCStr("Anti Knockback"), &cfg.player.bAntiKnockback);
					};
					modalwin_.Popup();
				}

				// ANTI WATER/LAVA
				ImGui::Checkbox(XorCStr("Anti W/L"), &cfg.player.bAntiWater);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("Anti Water/Lava"));
				IMEDIT("##Anti Water", firstEditOffset)
				{
					modalwin_.winLabel = XorStr("Anti Water/Lava");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.antiWaterKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
					};
					modalwin_.Popup();
				}

				// ANTI VOID
				ImGui::Checkbox(XorCStr("Anti Void"), &cfg.player.bAntiVoid);
				IMEDIT("##Anti Void", firstEditOffset)
				{
					modalwin_.winLabel = XorStr("Anti Void");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.antiVoidKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
					};
					modalwin_.Popup();
				}

				// NEXT COLUMN - OTHERS
				ImGui::NextColumn();
				ImGui::Spacing();

				// NO BOUND KILL
				ImGui::Checkbox(XorCStr("No Bound Kill"), &cfg.player.bNoBoundKill);
				IMEDIT("##No Bound Kill")
				{
					modalwin_.winLabel = XorStr("No Bound Kill");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.noBoundKillkey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
					};
					modalwin_.Popup();
				}

				// SUPER TIRE JUMP
				ImGui::Checkbox(XorCStr("High Tire Jump"), &cfg.player.bSuperTireJump);
				IMEDIT("##High Tire Jump")
				{
					modalwin_.winLabel = XorStr("High Tire Jump");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.superTireJumpKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
						ImGui::SliderFloat(XorCStr("Bounce Force"), &cfg.player.tireForce, 25.f, 100.f, XorCStr("%.2f"));
					};
					modalwin_.Popup();
				}

				// SUPER SLIDE
				ImGui::Checkbox(XorCStr("Super Slide"), &cfg.player.bSuperSlide);
				IMEDIT("##Super Slide")
				{
					modalwin_.winLabel = XorStr("Super Slide");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.superSlideKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
						ImGui::SliderFloat(XorCStr("Slide Force"), &cfg.player.slideForce, 8000.f, 100000.f, XorCStr("%.2f"));
					};
					modalwin_.Popup();
				}

				// INFINITE SLIDE
				ImGui::Checkbox(XorCStr("Infinite Slide"), &cfg.player.bInfiniteSlide);
				IMEDIT("##Infinite Slide")
				{
					modalwin_.winLabel = XorStr("Infinite Slide");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.infSlideKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
					};
					modalwin_.Popup();
				}

				// NO ICE SLIDE
				ImGui::Checkbox(XorCStr("No Ice Slide"), &cfg.player.bNoIceSlide);

				// LOW GRAVITY
				ImGui::Checkbox(XorCStr("Mod Gravity"), &cfg.player.bModGravity);
				IMEDIT("##Mod Gravity")
				{
					modalwin_.winLabel = XorStr("Mod Gravity");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.gravityKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
						ImGui::SliderFloat(XorCStr("Gravity"), &cfg.player.gravity, -200.f, 5000.f, XorCStr("%.2f"));
					};
					modalwin_.Popup();
				}

				// GODMODE
				ImGui::Checkbox(XorStr("Godmode").c_str(), &cfg.player.bGodMode);
				IMEDIT("##Godmode")
				{
					modalwin_.winLabel = XorStr("Godmode");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.godModeKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
					};
					modalwin_.Popup();
				}

				// RLGL PLAY BOT
				ImGui::Checkbox(XorCStr("RLGL Play Bot"), &cfg.player.bRLGLBot);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("Red Light Green Ligh Auto Play Bot:\nthis bot will secure you a legit win"));
				IMEDIT("##RLGL Play Bot")
				{
					modalwin_.winLabel = XorStr("RLGL Play Bot");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.rlglBotKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
					};
					modalwin_.Popup();
				}

				// COMMANDS
				ImGui::Checkbox(XorCStr("Commands"), &cfg.player.bCommands);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("Type !help in the chat for information"));
				IMEDIT("##Commands")
				{
					modalwin_.winLabel = XorStr("Commands");
					modalwin_.GUICallback = []()->void
					{
						ImGui::SetNextItemWidth(80);
						ImGui::InputText(XorCStr("Cmd Symbol"), cfg.player.commandSymbol, sizeof(cfg.player.commandSymbol));
					};
					modalwin_.Popup();
				}

				// NEXT COLUMN - OTHERS 2
				ImGui::NextColumn();
				ImGui::Spacing();

				// PLAYER TELEPORT
				ImGui::Checkbox(XorCStr("Player Teleport"), &cfg.player.bPlayerTeleport);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("Teleports the closest player to your crosshair\nright in front of you (similar to far reach, but\nthe target teleports instead of you)"));
				IMEDIT("##Player Teleport")
				{
					modalwin_.winLabel = XorStr("Player Teleport");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.playerTpKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
						ImGui::SliderFloat(XorCStr("Tp Distance"), &cfg.player.playerTpDist, 0.5f, 10.f, XorCStr("%.2f"));
					};
					modalwin_.Popup();
				}

				// RLGL GODMODE
				ImGui::Checkbox(XorCStr("RLGL Godmode"), &cfg.player.bRedLightGodmode);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("Red Light Green Ligh Godmode"));

				// NO GLASS BREAK
				ImGui::Checkbox(XorCStr("No Glass Break"), &cfg.player.bNoGlassBreak);

				// SUICIDE
				if (ImGui::Button(XorCStr("Suicide")))
					cfg.player.bSuicide = true;
				IMEDIT("##Suicide")
				{
					modalwin_.winLabel = XorStr("Suicide");
					modalwin_.GUICallback = []()
					{
						ImGui_HotkeyButton(cfg.player.suicideKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
					};
					modalwin_.Popup();
				}

				ImGui::Spacing();

				// KILL CURRENT BUTTON PRESSER
				if (ImGui::Button(XorCStr("Kill Button Presser")))
					cfg.player.bKillCurrentButtonPresser = true;
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("HOST ONLY!!!\n\nKills the current button presser\non the Bustling Buttons Gamemode"));

				// SAVE CURRENT BUTTON PRESSER
				if (ImGui::Button(XorCStr("Save Button Presser")))
					cfg.player.bSaveCurrentButtonPresser = true;
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("HOST ONLY!!!\n\nSaves the current button presser\non the Bustling Buttons Gamemode"));

				// TROLL BUTTON SCREEN
				ImGui::Checkbox(XorCStr("Troll Button Screen"), &cfg.player.bTrollButtonScreen);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("HOST ONLY!!!\n\nShows red when you press the right button and\nshows green when you press the rigged button\nin the Bustling Buttons Gamemode"));

				// ONLY RIGGED BUTTONS
				ImGui::Checkbox(XorCStr("Only Rigged Buttons"), &cfg.player.bOnlyRiggedButtons);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("HOST ONLY!!!\n\nMakes all buttons rigged for other players\nin the Bustling Buttons Gamemode"));

				// NEXT COLUMN - TROLL OPTIONS
				ImGui::NextColumn();
				ImGui::Spacing();

				// BREAK GLASS
				if (ImGui::Button(XorCStr("Break Glass")))
					cfg.player.bBreakGlass = true;
				IMEDIT("##Break Glass")
				{
					modalwin_.winLabel = XorStr("Break Glass");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.glassBrealKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
					};
					modalwin_.Popup();
				}

				ImGui::SameLine(0, style.ItemInnerSpacing.x);
				ImGui::TextDisabled(XorCStr("(?)"));
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("This feature will break the wrong glass pieces\nin Stepping Stones Gamemode"));

				// INVISIBILITY
				ImGui::Checkbox(XorCStr("Invisibility"), &cfg.player.bInvisibility);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("Don't active near players unless you want to fling them!"));
				IMEDIT("##Invisibility")
				{
					modalwin_.winLabel = XorStr("Invisibility");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.invisibilKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
					};
					modalwin_.Popup();
				}

				// TILE DRIVE TELEPORT
				ImGui::Checkbox(XorCStr("TD Teleport"), &cfg.player.bTileDriveTp);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("Tile Drive Teleport for Tile Drive Gamemode"));
				IMEDIT("##TD Teleport")
				{
					modalwin_.winLabel = XorStr("TD Teleport");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.tileDriveTpKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
					};
					modalwin_.Popup();
				}

				// HAT/BOMB/STICK TELEPORT
				ImGui::Checkbox(XorCStr("HBSK Teleport"), &cfg.player.bHbsTp);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("Hat/Bomb/Stick/Knfie Teleport"));
				IMEDIT("##Hat Teleport")
				{
					modalwin_.winLabel = XorStr("HBSK Teleport");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.hbsTpKey, XorCStr("Hold Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
						ImGui::Checkbox(XorCStr("Auto Tp"), &cfg.player.bAutoHBStp);
					};
					modalwin_.Popup();
				}

				// MOUSE TELEPORT
				ImGui::Checkbox(XorCStr("Mouse Teleport"), &cfg.player.bMouseTeleport);
				IMEDIT("##Mouse Teleport")
				{
					modalwin_.winLabel = XorStr("Mouse Teleport");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.mouseTpKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
					};
					modalwin_.Popup();
				}

				ImGui::Spacing();

				// CLEAR CHAT
				if (ImGui::Button(XorCStr("Clear Chat")))
					cfg.player.bClearChat = true;
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip(XorCStr("Right Click to Stop"));
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
						cfg.player.bClearChat = false;
				}
				IMEDIT("##Clear Chat")
				{
					modalwin_.winLabel = XorStr("Clear Chat");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.clearChatKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
					};
					modalwin_.Popup();
				}
				ImGui::Checkbox(XorCStr("Auto Clear"), &cfg.player.bAutoClsChat);
				
				ImGui::Spacing();

				// SEND MESSAGE
				ImGui::SetNextItemWidth(100);
				ImGui::InputText(XorStr("Message##input").c_str(), cfg.player.message, sizeof(cfg.player.message));

				ImGui::SetNextItemWidth(100);
				ImGui::InputInt(XorStr("Amount##message").c_str(), &cfg.player.messageAmount, 1, 5);
				if (cfg.player.messageAmount > 50)
					cfg.player.messageAmount = 50;
				else if (cfg.player.messageAmount < 1)
					cfg.player.messageAmount = 1;

				ImGui::SameLine();

				if (ImGui::Button(XorStr("Send##message").c_str()))
					cfg.player.messageCounter = cfg.player.messageAmount;
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip(XorCStr("Right Click to Stop"));
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
						cfg.player.messageCounter = 0;
				}

				// NEXT COLUMN - HOST ONLY
				ImGui::NextColumn();
				ImGui::Spacing();

				// START GAME
				if (ImGui::Button(XorStr(ICON_FA_PLAY"  Start Game").c_str()) && Client::bInGame)
					cfg.player.bStartGame = true;
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorStr("HOST ONLY!!!").c_str());

				ImGui::SameLine();

				// NEXT ROUND
				if (ImGui::Button(XorStr(ICON_FA_FORWARD"  Next Round").c_str()) && Client::bInGame)
					cfg.player.bNextGame = true;
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorStr("HOST ONLY!!!").c_str());

				// NEXT MAP
				ImGui::SetNextItemWidth(180);
				ImGui::Combo(XorStr("Next Map").c_str(), &cfg.player.nextMap, Items_Maps, IM_ARRAYSIZE(Items_Maps));
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorStr("HOST ONLY!!!").c_str());
				switch (cfg.player.nextMap - 1)
				{
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
					ImGui::SetNextItemWidth(140);
					ImGui::Combo(XorCStr("Next Gamemode"), &cfg.player.nextGameMode, XorCStr("Random\0Tag\0Hide and Seek\0Bomb Tag\0Hat King\0\0"));
					break;
				}

				ImGui::Spacing();

				// ANTI TAG
				ImGui::Checkbox(XorCStr("Anti Tag"), &cfg.player.bAntiTag);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("HOST ONLY!!!"));
				IMEDIT("##Anti Tag")
				{
					modalwin_.winLabel = XorStr("Anti Tag");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.antiTagKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
					};
					modalwin_.Popup();
				}

				ImGui::SameLine(0, style.ItemInnerSpacing.x);
				ImGui::TextDisabled(XorCStr("(?)"));
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("Anti Tag disables other players from stealing your hat,\ntagging you on Bomb Tag gamemode and\nfrom tagging you in Tag gamemode"));

				ImGui::Spacing();

				// SEND PLAYERS 2 SPACE
				if (ImGui::Button(XorCStr("Send Players 2 Space")))
					cfg.player.bSend2Space = true;
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("HOST ONLY!!!"));
				IMEDIT("##Send Players 2 Space")
				{
					modalwin_.winLabel = XorStr("Send Players 2 Space");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.send2SpaceKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
					};
					modalwin_.Popup();
				}

				// FREEZE TIMER
				ImGui::Checkbox(XorCStr("Freeze Timer"), &cfg.player.bFreezeTimer);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("HOST ONLY!!!"));
				IMEDIT("##Freeze Timer")
				{
					modalwin_.winLabel = XorStr("Freeze Timer");
					modalwin_.GUICallback = []()
					{
						ImGui_HotkeyButton(cfg.player.freezeTimerKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
					};
					modalwin_.Popup();
				}

				// PLAYER NAMES
				std::vector<std::string> playerNames(Cheat::playerNames.size() + 1);
				playerNames[0] = XorStr("everyone");
				for (USHORT i = 0; i < Cheat::playerNames.size(); i++)
					playerNames[i + 1] = Cheat::playerNames[i];

				// FREEZE PLAYERS
				ImGui::Checkbox(XorCStr("Freeze Player(s)"), &cfg.player.bFreezePlayers);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("HOST ONLY!!!"));
				ImGui::SameLine(175);
				ImGui::SetNextItemWidth(120);
				ImGui::Combo(XorCStr("##frozen player"), &cfg.player.frozenPlayer, playerNames);
				if (cfg.player.frozenPlayer >= playerNames.size())
					cfg.player.frozenPlayer = playerNames.size() - 1;

				// KILL PLAYERS
				if (ImGui::Button(XorCStr("Kill")))
					cfg.player.bKillPlayers = true;
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("HOST ONLY!!!"));
				ImGui::SameLine(175);
				ImGui::SetNextItemWidth(120);
				ImGui::Combo(XorCStr("##player to kill"), &cfg.player.playerToKill, playerNames);
				if (cfg.player.playerToKill >= playerNames.size())
					cfg.player.playerToKill = playerNames.size() - 1;

				// RESPAWN
				if (ImGui::Button(XorCStr("Respawn##lel")))
					cfg.player.bRespawn = true;
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("HOST ONLY!!!"));
			}
				break;
			
			// VISUALS
			case 1:
			{
				ImGui::Checkbox(XorStr("Player ESP").c_str(), &cfg.visuals.vp.bESP);

				ImGui::SameLine(150);
				if (ImGui::Button(XorStr(ICON_FA_EDIT "##esp").c_str()))
					ImGui::OpenPopup(XorStr("Player ESP Settings##popup").c_str());

				// PLAYER ESP SETTINGS
				if (ImGui::BeginPopupModal(XorStr("Player ESP Settings##popup").c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize))
				{
					static vis_player_t settings;
					if (ImGui::IsWindowAppearing())
					{
						settings = cfg.visuals.vp;
					}

					static bool bGetKey1 = false;
					ImGui_HotkeyButton(cfg.visuals.vp.key, XorCStr("Toggle Key"), XorCStr("##esp key"), bGetKey1);

					ImGui::Spacing();

					ImGui::Checkbox(XorCStr("Chams"), &cfg.visuals.vp.bChams);
					if (cfg.visuals.vp.bChams)
					{
						ImGui::Checkbox(XorStr("RGB Effect##cham").c_str(), &cfg.visuals.vp.bChamRGB);

						if (!cfg.visuals.vp.bChamRGB)
						{
							ImGui::Text(XorStr("Cham Color: ").c_str());

							static bool bShowPicker1 = false;
							ImGui::SameLine();
							ImGuiColorPicker(cfg.visuals.vp.ChamColor, bShowPicker1, XorStr("##cham color").c_str());
						}
					}

					ImGui::Checkbox(XorCStr("Boxes"), &cfg.visuals.vp.bBoxes);
					if (cfg.visuals.vp.bBoxes)
					{
						ImGui::SameLine();
						ImGui::SetNextItemWidth(110);
						static std::vector<std::string> Items_BoxTypes = { XorStr("Rectangle"), XorStr("Corners"), XorStr("3D Box") };
						ImGui::Combo(XorStr("Box Type").c_str(), &cfg.visuals.vp.BoxType, Items_BoxTypes);

						ImGui::SetNextItemWidth(200);
						ImGui::SliderFloat(XorStr("Box Width").c_str(), &cfg.visuals.vp.BoxLineWidth, 1.f, 5.f, XorStr("%.2f").c_str());

						ImGui::Checkbox(XorStr("RGB Effect##box").c_str(), &cfg.visuals.vp.bBoxRGB);

						if (!cfg.visuals.vp.bBoxRGB)
						{
							ImGui::Text(XorStr("Box Color: ").c_str());

							static bool bShowPicker1 = false;
							ImGui::SameLine();
							ImGuiColorPicker(cfg.visuals.vp.BoxColor, bShowPicker1, XorStr("##box color").c_str());
						}
					}

					ImGui::Spacing();

					ImGui::Checkbox(XorStr("Snaplines").c_str(), &cfg.visuals.vp.bSnaplines);
					if (cfg.visuals.vp.bSnaplines)
					{
						ImGui::SameLine();
						ImGui::SetNextItemWidth(190);
						ImGui::Combo(XorStr("Snapline Type").c_str(), &cfg.visuals.vp.SnapType, XorStr("From Button To Origin\0From Button To Head\0From Center to Origin\0From Center to Head\0\0").c_str());

						ImGui::SetNextItemWidth(200);
						ImGui::SliderFloat(XorStr("Snapline Width").c_str(), &cfg.visuals.vp.SnaplineLineWidth, 1.f, 5.f, XorStr("%.2f").c_str());

						ImGui::Checkbox(XorStr("RGB Effect##snapline").c_str(), &cfg.visuals.vp.bSnaplineRGB);

						if (!cfg.visuals.vp.bSnaplineRGB)
						{
							ImGui::Text(XorStr("Snapline Color: ").c_str());

							static bool bShowPicker1 = false;
							ImGui::SameLine();
							ImGuiColorPicker(cfg.visuals.vp.SnaplineColor, bShowPicker1, XorStr("##snapline color").c_str());
						}
					}

					ImGui::Spacing();

					ImGui::Checkbox(XorCStr("Name ESP"), &cfg.visuals.vp.bNameESP);

					ImGui::Spacing();

					if (cfg.visuals.vp.bBoxRGB || cfg.visuals.vp.bSnaplineRGB)
					{
						ImGui::SetNextItemWidth(200);
						ImGui::SliderFloat(XorStr("RGB Effect Speed").c_str(), &cfg.visuals.vp.RGBSpeed, 0.1f, 5.f, XorStr("%.2f").c_str());
						ImGui::Spacing();
					}

					ImGui::Checkbox(XorCStr("Limit Distance"), &cfg.visuals.vp.bLimitDistance);
					if (cfg.visuals.vp.bLimitDistance)
					{
						ImGui::SameLine();
						ImGui::SetNextItemWidth(180);
						ImGui::SliderFloat(XorCStr("Max Distance"), &cfg.visuals.vp.maxDistance, 1.f, 300.f, XorCStr("%.2f"));
					}

					ImGui::Checkbox(XorCStr("Enemies Only"), &cfg.visuals.vp.bEnemiesOnly);
					ImGui::SameLine(0, style.ItemInnerSpacing.x);
					ImGui::TextDisabled(XorCStr("(?)"));
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip(XorCStr("This option will show only the enemies in\nTag, Bomb Tag, Hat King and Hide and Seek gamemodes"));

					// BUTTONS
					ImGui::Spacing();
					if (ImGui::Button(XorStr(ICON_FA_CHECK" OK").c_str()))
						ImGui::CloseCurrentPopup();

					ImGui::SameLine();
					if (ImGui::Button(XorStr(ICON_FA_TIMES" Cancel").c_str()))
						cfg.visuals.vp = settings, ImGui::CloseCurrentPopup();

					ImGui::SameLine(350);
					if (ImGui::Button(XorStr(ICON_FA_UNDO" Reset").c_str()))
						cfg.visuals.vp = vis_player_t();

					ImGui::EndPopup();
				}

				// WEAPON ESP
				ImGui::Checkbox(XorCStr("Weapon ESP"), &cfg.visuals.bWeaponESP);
				IMEDIT("##Weapon ESP")
				{
					modalwin_.winLabel = XorStr("Weapon ESP");
					modalwin_.GUICallback = []()->void
					{

						ImGui::Text(XorCStr("Weapon Color:"));
						ImGui::SameLine();

						static bool bGetColor1 = false;
						ImGuiColorPicker(cfg.visuals.WeaponColor, bGetColor1, XorCStr("##Weapon color"));
						ImGui::Checkbox(XorCStr("RGB Effect"), &cfg.visuals.bWeaponRGB);

						if (cfg.visuals.bWeaponRGB)
						{
							ImGui::Spacing();
							ImGui::SliderFloat(XorCStr("RGB Effect Speed"), &cfg.visuals.vp.RGBSpeed, 0.1f, 5.f, XorCStr("%.2f"));
						}
					};
					modalwin_.Popup();
				}

				// GLASS ESP
				ImGui::Checkbox(XorStr("Glass ESP").c_str(), &cfg.visuals.GlassESP);

				ImGui::SameLine();
				ImGui::TextDisabled(XorStr("(?)").c_str());
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorStr("This feature shows you the glass pieces\nin red rectangles, which will break").c_str());

				ImGui::SameLine(150);
				if (ImGui::Button(XorCStr(ICON_FA_EDIT "##glass esp")))
					ImGui::OpenPopup(XorCStr("Glass ESP Settings##popup"));

				// GLASS ESP SETTINGS
				if (ImGui::BeginPopupModal(XorStr("Glass ESP Settings##popup").c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize))
				{
					static bool bRGB;
					static float color[4];
					if (ImGui::IsWindowAppearing())
					{
						bRGB = cfg.visuals.bRGBGlass;
						color[0] = cfg.visuals.GlassColor[0];
						color[1] = cfg.visuals.GlassColor[1];
						color[2] = cfg.visuals.GlassColor[2];
						color[3] = cfg.visuals.GlassColor[3];
					}

					if (!cfg.visuals.bRGBGlass)
					{
						ImGui::Text(XorCStr("Glass ESP Color: "));

						static bool bColorPicker1 = false;
						ImGui::SameLine();
						ImGuiColorPicker(cfg.visuals.GlassColor, bColorPicker1, XorCStr("##glass esp color"));

						ImGui::Spacing();
					}

					ImGui::Checkbox(XorCStr("RGB Effect"), &cfg.visuals.bRGBGlass);

					if (cfg.visuals.bRGBGlass)
					{
						ImGui::Spacing();
						ImGui::SetNextItemWidth(200);
						ImGui::SliderFloat(XorCStr("RGB Effect Speed"), &cfg.visuals.vp.RGBSpeed, 0.1f, 5.f, XorCStr("%.2f"));
					}

					ImGui::Spacing();
					ImGui::Spacing();

					if (ImGui::Button(XorStr(ICON_FA_CHECK" OK").c_str()))
						ImGui::CloseCurrentPopup();

					ImGui::SameLine();

					if (ImGui::Button(XorStr(ICON_FA_TIMES" Cancel").c_str()))
					{
						cfg.visuals.bRGBGlass = bRGB;
						*(vec4*)cfg.visuals.GlassColor = *(vec4*)color;
						ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine(350);

					if (ImGui::Button(XorStr(ICON_FA_UNDO" Reset").c_str()))
					{
						*(vec4*)cfg.visuals.GlassColor = { 1.f, 0, 0, 1.f };
						cfg.visuals.bRGBGlass = false;
					}

					ImGui::EndPopup();
				}

				// BUSTLING BUTTONS ESP
				ImGui::Checkbox(XorStr("Bustling Buttons ESP").c_str(), &cfg.visuals.ButtonESP);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorStr("HOST ONLY!!!").c_str());

				ImGui::SameLine();
				ImGui::TextDisabled(XorStr("(?)").c_str());
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorStr("This feature shows you the dangerous button\nin Bustling Buttons Gamemode, which will kill you").c_str());

				ImGui::Spacing();

				// FOV
				ImGui::Checkbox(XorCStr("FOV"), &cfg.visuals.bFOV);
				if (cfg.visuals.bFOV)
				{
					ImGui::SameLine();
					ImGui::SetNextItemWidth(180);
					ImGui::SliderInt(XorCStr("##fov slider"), &cfg.visuals.FOV, 40, 150);
				}

				// DORM FULL BRIGHT
				ImGui::Checkbox(XorCStr("Dorm Full Bright"), &cfg.visuals.bDormFullBright);

				// BOMB TIMER
				ImGui::Checkbox(XorCStr("Bomb Timer"), &cfg.visuals.bBombTimer);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorStr("HOST ONLY!!!").c_str());

				// NO CAMERA SHAKE
				ImGui::Checkbox(XorStr("No Camera Shake").c_str(), &cfg.player.bNoCameraShake);

				// ZOOM
				ImGui::Checkbox(XorCStr("Zoom"), &cfg.visuals.bZoom);
				IMEDIT("##Zoom")
				{
					modalwin_.winLabel = XorStr("Zoom");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.visuals.zoomKey, XorCStr("Hold Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
						ImGui::SliderInt(XorCStr("Zoom FOV"), &cfg.visuals.zoomFOV, 10, 90);
					};
					modalwin_.Popup();
				}
			}
			break;

			// COMBAT
			case 2:
			{
				ImGui::Checkbox(XorStr("Aim Assist##sas").c_str(), &cfg.aim.bAim);

				ImGui::SameLine(130);
				if (ImGui::Button(XorStr(ICON_FA_EDIT"##edit aimbot").c_str()))
					ImGui::OpenPopup(XorStr("Aim Assist Settings##popup").c_str());

				// AIM ASSIST SETTINGS
				if (ImGui::BeginPopupModal(XorStr("Aim Assist Settings##popup").c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize))
				{
					static aim_t aimSettings;
					if (ImGui::IsWindowAppearing())
					{
						aimSettings = cfg.aim;
					}

					ImGui::Checkbox(XorStr("Hold to Aim##b").c_str(), &cfg.aim.bAimKey);

					if (cfg.aim.bAimKey)
					{
						static bool bGetKey1 = false;
						ImGui::SameLine();
						ImGui_HotkeyButton(cfg.aim.AimKey, XorStr("Aim Asisst Key (Hold)").c_str(), XorStr("##aim key").c_str(), bGetKey1);
					}

					ImGui::Spacing();
					ImGui::Spacing();

					ImGui::SetNextItemWidth(220);
					ImGui::SliderFloat(XorStr("Smoothness").c_str(), &cfg.aim.Smoothness, 1.f, 5.f, XorStr("%.2f").c_str());

					ImGui::Spacing();
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("FOV").c_str(), &cfg.aim.bFOV);
					if (cfg.aim.bFOV)
					{
						ImGui::SameLine();
						ImGui::SetNextItemWidth(220);
						ImGui::SliderInt(XorStr("##fov slider").c_str(), &cfg.aim.FOV, 5, Client::WindowCenter.x);

						ImGui::Checkbox(XorStr("FOV Circle##b").c_str(), &cfg.aim.bFOVCircle);
						if (cfg.aim.bFOVCircle)
						{
							static bool bColorPicker1 = false;
							ImGui::SameLine();
							ImGuiColorPicker(cfg.aim.FOVCircleColor, bColorPicker1, XorStr("##color picker1").c_str());
						}
					}

					ImGui::Spacing();
					ImGui::Spacing();

					if (ImGui::Button(XorStr(ICON_FA_CHECK" OK").c_str()))
						ImGui::CloseCurrentPopup();

					ImGui::SameLine();

					if (ImGui::Button(XorStr(ICON_FA_TIMES" Cancel").c_str()))
						cfg.aim = aimSettings, ImGui::CloseCurrentPopup();

					ImGui::SameLine(321);

					if (ImGui::Button(XorStr(ICON_FA_UNDO" Reset").c_str()))
					{
						trg_t buffer = cfg.aim.trg;
						cfg.aim = aim_t();
						cfg.aim.trg = buffer;
					}

					ImGui::EndPopup();
				}

				ImGui::Checkbox(XorStr("Triggerbot##bruh").c_str(), &cfg.aim.trg.bTrg);

				ImGui::SameLine(130);
				if (ImGui::Button(XorStr(ICON_FA_EDIT"##edit triggerbot").c_str()))
					ImGui::OpenPopup(XorStr("Triggerbot Settings##popup").c_str());

				// TRIGGERBOT SETTINGS
				if (ImGui::BeginPopupModal(XorStr("Triggerbot Settings##popup").c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize))
				{
					static trg_t trgSettings;
					if (ImGui::IsWindowAppearing())
					{
						trgSettings = cfg.aim.trg;
					}

					ImGui::Checkbox(XorStr("Hold to Run").c_str(), &trgSettings.bTrgKey);
					static bool bGetKey1 = false;
					if (trgSettings.bTrgKey)
					{
						ImGui::SameLine();
						ImGui_HotkeyButton(trgSettings.TrgKey, XorStr("Triggerbot Key").c_str(), XorStr("##trg key").c_str(), bGetKey1);
					}

					ImGui::Spacing();
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("Limit Target Distance").c_str(), &trgSettings.bLimitDistance);
					if (trgSettings.bLimitDistance)
					{
						ImGui::SameLine();
						ImGui::SetNextItemWidth(200);
						ImGui::SliderFloat(XorStr("Distance").c_str(), &trgSettings.maxDistance, 3, 300, XorStr("%.2f").c_str());
					}

					ImGui::Spacing();
					ImGui::Spacing();

					if (ImGui::Button(XorStr(ICON_FA_CHECK" OK").c_str()))
					{
						bGetKey1 = false;
						cfg.aim.trg = trgSettings;
						ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine();

					if (ImGui::Button(XorStr(ICON_FA_TIMES" Cancel").c_str()))
						ImGui::CloseCurrentPopup(), bGetKey1 = false;

					ImGui::SameLine(420);

					if (ImGui::Button(XorStr(ICON_FA_UNDO" Reset").c_str()))
						trgSettings = trg_t(), bGetKey1 = false;

					ImGui::EndPopup();
				}

				ImGui::Spacing();

				// NO RECOIL
				ImGui::Checkbox(XorCStr("No Recoil"), &cfg.aim.bNoRecoil);
				IMEDIT("##No Recoil")
				{
					modalwin_.winLabel = XorStr("No Recoil");
					modalwin_.GUICallback = []()->void
					{
						ImGui::SliderFloat(XorCStr("Recoil Control"), &cfg.aim.recoilSlider, 0.f, 1.f, XorCStr("%.2f"));
					};
					modalwin_.Popup();
				}

				// HOLD TO SHOOT
				ImGui::Checkbox(XorCStr("Hold to Shoot"), &cfg.aim.bHoldToShoot);

				// INFINITE AMMO
				ImGui::Checkbox(XorStr("Infinite Ammo").c_str(), &cfg.player.bInfiniteAmmo);

				// SUPER PUNCH
				ImGui::Checkbox(XorStr("Super Punch").c_str(), &cfg.player.bSuperPunch);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorStr("HOST ONLY!!!").c_str());
				IMEDIT("##Super Punch")
				{
					modalwin_.winLabel = XorStr("Super Punch");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.superPunchKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
						ImGui::SliderFloat(XorCStr("Punch Multiplier"), &cfg.player.punchMultiplier, 2.f, 200.f, XorCStr("%.2f"));
					};
					modalwin_.Popup();
				}

				// FAR REACH/PUNCH
				ImGui::Checkbox(XorCStr("Far Reach"), &cfg.player.bFarReach);
				ImGui::SameLine(0, style.ItemInnerSpacing.x);
				ImGui::TextDisabled(XorCStr("(?)"));
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("Far Reach/Punch teleports you to the closets\nplayer relative to the crosshair and auto\npunches the player (hold the hotkey to activate)"));
				IMEDIT("##Far Reach")
				{
					modalwin_.winLabel = XorStr("Far Reach");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.player.farReachKey, XorCStr("Hold Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
					};
					modalwin_.Popup();
				}

				ImGui::Spacing();

				// SPAWN ITEM/WEAPON
				ImGui::SetNextItemWidth(120);
				ImGui::Combo(XorStr("##item select").c_str(), &cfg.player.weaponId, Items_Weapons, IM_ARRAYSIZE(Items_Weapons));

				ImGui::SameLine();
				if (ImGui::Button(XorStr("Spawn##weapon").c_str()))
					cfg.player.ForceGiveWeapons = true;
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip(XorCStr("Right Click to spawn as Host"));
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
						cfg.player.bSpawnWeaponAsHost = true, cfg.player.ForceGiveWeapons = true;
				}

				ImGui::NextColumn();
				ImGui::Spacing();

				ImGui::Checkbox(XorCStr("Spinbot"), &cfg.aim.bSpinBot);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("Recommend using in combination with Silent Aim"));
				IMEDIT("##Spinbot")
				{
					modalwin_.winLabel = XorStr("Far Reach");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.aim.spinBotKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
						ImGui::SliderFloat(XorCStr("Spin Speed"), &cfg.aim.spinBotSpeed, 5.f, 20.f, XorCStr("%.2f"));
					};
					modalwin_.Popup();
				}

				ImGui::Checkbox(XorCStr("Silent Aim"), &cfg.aim.bSilentAim);
				IMEDIT("##Silent Aim")
				{
					modalwin_.winLabel = XorStr("Silent Aim");
					modalwin_.GUICallback = []()->void
					{
						ImGui_HotkeyButton(cfg.aim.silentAimKey, XorCStr("Toggle Key"), XorCStr("##hotkey"), modalwin_.bGetKey);
					};
					modalwin_.Popup();
				}

				ImGui::Checkbox(XorCStr("Rapid Fire"), &cfg.aim.bRapidFire);
			}
			break;

			// MENU
			case 3:
			{
				ImGui::Checkbox(XorStr("Lock Menu in Place").c_str(), &bLockInPlace);
				ImGui::SameLine(247);
				if (ImGui::ArrowButton(XorCStr("##menu settings"), bMenuSettings ? ImGuiDir_Left : ImGuiDir_Right))
					bMenuSettings = !bMenuSettings;
				if (!bMenuSettings && ImGui::IsItemHovered())
					ImGui::SetTooltip(XorCStr("Menu Settings"));

				ImGui::Spacing();

				static bool bGetKey1 = false;
				ImGui_HotkeyButton(ToggleKey, XorStr("Menu Toggle Key").c_str(), XorStr("##toggle key").c_str(), bGetKey1);

				static bool bGetKey2 = false;
				ImGui_HotkeyButton(CloseKey, XorStr("Menu Exit Key").c_str(), XorStr("##close key").c_str(), bGetKey2);

				ImGui::Spacing();

				static constexpr const USHORT width = 127;

				if (ImGui::Button(XorStr(ICON_FA_SAVE " Save Config").c_str(), ImVec2(width, 0)))
					cfg.SaveCfg();

				ImGui::SameLine();

				if (ImGui::Button(XorStr(ICON_FA_UPLOAD " Load Config").c_str(), ImVec2(width, 0)))
					cfg.LoadCfg();

				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Text(XorStr("Menu Version: v%s").c_str(), MenuVersion);
				ImGui::Text(XorStr("Working on Crab Game v%s").c_str(), GameVersion);

				ImGui::Spacing();
				ImGui::Spacing();

				if (ImGui::Button(XorStr(ICON_FA_SYNC" Check For Updates").c_str(), ImVec2(260, 0)))
				{
					bool buf = true;
					CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CheckForUpdates, &buf, 0, 0);
				}

				if (ImGui::Button(XorStr(ICON_FA_GLOBE" Join our Discord Server!").c_str(), ImVec2(260, 0)))
					ShellExecuteA(0, 0, XorStr("https://discord.gg/caJ8jtg9Jg").c_str(), 0, 0, 0);

				if (bMenuSettings)
				{
					ImGui::NextColumn();
					ImGui::Spacing();

					if (!cfg.bMenuRGB)
					{
						ImGui::Text(XorCStr("Menu Color: "));
						ImGui::SameLine();
						static bool bShowColorPicker = false;
						ImGuiColorPicker(cfg.MenuColor, bShowColorPicker, XorCStr("##menu color"));
					}

					ImGui::Checkbox(XorCStr("RGB Effect##menu"), &cfg.bMenuRGB);
					if (cfg.bMenuRGB)
					{
						ImGui::SetNextItemWidth(160);
						ImGui::SliderFloat(XorCStr("RGB Effect Speed##menu"), &cfg.MenuRGBSpeed, 0.02f, 3.f, XorCStr("%.2f"));
					}

					ImGui::SetNextItemWidth(160);
					ImGui::SliderFloat(XorCStr("Menu Rouding"), &cfg.MenuRounding, 0.f, 10.f, XorCStr("%.2f"));

					ImGui::Checkbox(XorCStr("Auto Save"), &cfg.bAutoSave);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip(XorCStr("This will automatically save the current config in the\nCrab Game.exe folder when the menu is closed"));

					ImGui::Checkbox(XorCStr("Free Mouse on Popup"), &cfg.bFreeMouseOnPopup);
				}
			}
				break;
			}

			// SCRIPTS BUTTON
			ImGui::SetCursorPos({ ImGui::GetWindowSize().x - 90, ImGui::GetWindowSize().y - 30 });
			if (ImGui::Button(XorCStr(ICON_FA_CODE" Scripts")))
				 cfg.script.bScriptsWindow = !cfg.script.bScriptsWindow;
			
			modalwin_.Show(); // MODAL WIN
			ImGui::End();

			// SCRIPTS WINDOW
			if (cfg.script.bScriptsWindow)
				RenderScriptsWindow();
		}

		// KEYBINDS
		Cheat::activeHNWD = GetForegroundWindow();
		if (!Cheat::bTyping && Cheat::activeHNWD == Client::hwnd)
		{
			if (cfg.player.flyKey != 0xADAF && PressKey(cfg.player.flyKey))
				cfg.player.bFlyhack = !cfg.player.bFlyhack;
			if (cfg.player.noClipKey != 0xADAF && PressKey(cfg.player.noClipKey))
				cfg.player.bNoClip = !cfg.player.bNoClip, cfg.player.bSetNoClip = true;
			if (cfg.player.speedKey != 0xADAF && PressKey(cfg.player.speedKey))
				cfg.player.bSpeedHack = !cfg.player.bSpeedHack;
			if (cfg.player.superJumpKey != 0xADAF && PressKey(cfg.player.superJumpKey))
				cfg.player.bSuperJump = !cfg.player.bSuperJump;
			if (cfg.player.infJumpKey != 0xADAF && PressKey(cfg.player.infJumpKey))
				cfg.player.bInfiniteJump = !cfg.player.bInfiniteJump;
			if (cfg.player.glassBrealKey != 0xADAF && PressKey(cfg.player.glassBrealKey))
				cfg.player.bBreakGlass = true;
			if (cfg.player.clearChatKey != 0xADAF && PressKey(cfg.player.clearChatKey))
				cfg.player.bClearChat = true;
			if (cfg.player.send2SpaceKey != 0xADAF && PressKey(cfg.player.send2SpaceKey))
				cfg.player.bSend2Space = true;
			if (cfg.player.gravityKey != 0xADAF && PressKey(cfg.player.gravityKey))
				cfg.player.bModGravity = !cfg.player.bModGravity;
			if (cfg.player.godModeKey != 0xADAF && PressKey(cfg.player.godModeKey))
				cfg.player.bGodMode = !cfg.player.bGodMode;
			if (cfg.player.superPunchKey != 0xADAF && PressKey(cfg.player.superPunchKey))
				cfg.player.bSuperPunch = !cfg.player.bSuperPunch;
			if (cfg.player.antiVoidKey != 0xADAF && PressKey(cfg.player.antiVoidKey))
				cfg.player.bAntiVoid = !cfg.player.bAntiVoid;
			if (cfg.player.rlglBotKey != 0xADAF && PressKey(cfg.player.rlglBotKey))
				cfg.player.bRLGLBot = !cfg.player.bRLGLBot;
			if (cfg.player.antiTagKey != 0xADAF && PressKey(cfg.player.antiTagKey))
				cfg.player.bAntiTag = !cfg.player.bAntiTag;
			if (cfg.player.antiWaterKey != 0xADAF && PressKey(cfg.player.antiWaterKey))
				cfg.player.bAntiWater = !cfg.player.bAntiWater;
			if (cfg.player.tileDriveTpKey != 0xADAF && PressKey(cfg.player.tileDriveTpKey))
				cfg.player.bTileDriveTp = !cfg.player.bTileDriveTp;
			if (cfg.player.superSlideKey != 0xADAF && PressKey(cfg.player.superSlideKey))
				cfg.player.bSuperSlide = !cfg.player.bSuperSlide;
			if (cfg.player.infSlideKey != 0xADAF && PressKey(cfg.player.infSlideKey))
				cfg.player.bInfiniteSlide = !cfg.player.bInfiniteSlide;
			if (cfg.player.superTireJumpKey != 0xADAF && PressKey(cfg.player.superTireJumpKey))
				cfg.player.bSuperTireJump = !cfg.player.bSuperTireJump;
			if (cfg.visuals.vp.key != 0xADAF && PressKey(cfg.visuals.vp.key))
				cfg.visuals.vp.bESP = !cfg.visuals.vp.bESP;
			if (cfg.aim.spinBotKey != 0xADAF && PressKey(cfg.aim.spinBotKey))
				cfg.aim.bSpinBot = !cfg.aim.bSpinBot;
			if (cfg.aim.silentAimKey != 0xADAF && PressKey(cfg.aim.silentAimKey))
				cfg.aim.bSilentAim = !cfg.aim.bSilentAim;
			if (cfg.player.noBoundKillkey != 0xADAF && PressKey(cfg.player.noBoundKillkey))
				cfg.player.bNoBoundKill = !cfg.player.bNoBoundKill;
			if (cfg.player.invisibilKey != 0xADAF && PressKey(cfg.player.invisibilKey))
				cfg.player.bInvisibility = !cfg.player.bInvisibility;
			if (cfg.player.playerTpKey != 0xADAF && PressKey(cfg.player.playerTpKey))
				cfg.player.bPlayerTeleport = !cfg.player.bPlayerTeleport;
			if (cfg.player.suicideKey != 0xADAF && PressKey(cfg.player.suicideKey))
				cfg.player.bSuicide = !cfg.player.bSuicide;
			if (cfg.player.freezeTimerKey != 0xADAF && PressKey(cfg.player.freezeTimerKey))
				cfg.player.bFreezeTimer = !cfg.player.bFreezeTimer;

			// SCRIPT HOTKEYS
			for (USHORT i = 0; i < cfg.script.files.size(); i++)
			{
				auto script = &cfg.script.files[i];
				if (script->hotkey != 0xADAF && PressKey(script->hotkey))
				{
					script->bEnabled = !script->bEnabled;
					OnScriptToggled(script);
				}
			}
		}

		// RENDER VISUALS
		if (Client::bInGame)
			Cheat::RenderVisuals();

		// RENDER SCRIPTS
		for (USHORT i = 0; i < cfg.script.files.size(); i++)
		{
			auto script = &cfg.script.files[i];

			if (script->bEnabled && (script->flags & ScriptFlags_HasRender))
				script->CallFunc(XorCStr("OnRender"));
		}

		return false;
	}

	bool CheckForUpdates(const bool& bNotifyIfUptodate)
	{
		if (URLDownloadToFileA(0,
			XorCStr("https://raw.githubusercontent.com/1hAck-0/Server/main/Crab%20Game%20Mod%20Menu%20info.json"),
			XorCStr("crab.json"), 0, 0) != S_OK)
		{
			remove(XorCStr("crab.json"));

			MessageBoxA(0,
				XorStr("Couldn't get up-to-date info! Check your internet connection!").c_str(),
				XorStr("Crab Game Mod Menu").c_str(),
				MB_ICONERROR);
			return false;
		}

		std::ifstream file;
		file.open(XorStr("crab.json"), std::ios_base::in);
		if (file.fail() || !file.is_open())
		{
			file.close();
			MessageBoxA(0,
				XorStr("Couldn't download information!").c_str(),
				XorStr("Download Error").c_str(),
				MB_ICONERROR);
			return false;
		}
		using namespace nlohmann;
		ordered_json j = ordered_json::parse(file, 0, false);
		file.close();
		remove(XorStr("crab.json").c_str());

		std::string version, url;
		try
		{
			version = j[XorStr("version")];
			url = j[XorStr("url")];
		}
		catch (...)
		{
			remove(XorStr("crab.json").c_str());
			MessageBoxA(0,
				XorStr("Couldn't get up-to-date info!").c_str(),
				XorStr("Parsing Error").c_str(),
				MB_ICONERROR);
			return false;
		}

		if (version != MenuVersion)
		{
			std::string buffer = XorStr("1hAck v");
			buffer += version;
			buffer += XorStr(" is out now! Would you like to download it?");

			if (MessageBoxA(0,
				buffer.c_str(),
				XorStr("Crab Game Mod Menu").c_str(),
				MB_ICONQUESTION | MB_YESNO) == IDYES)
			{
				if (!ShellExecuteA(0, 0, url.c_str(), 0, 0, 0))
					MessageBoxA(0, XorStr("Couldn't open link!").c_str(), XorStr("Link Error").c_str(), 0);
			}
		}
		else if (bNotifyIfUptodate)
		{
			MessageBoxA(0,
				XorStr("You have the latest version of this Mod Menu!").c_str(),
				XorStr("Crab Game Mod Menu").c_str(),
				MB_ICONINFORMATION);
			return false;
		}
		return true;
	}

	void Setup(ImGuiIO& io)
	{
		// THEME
		{
			ImGuiStyle& style = ImGui::GetStyle();
			style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
			style.WindowBorderSize = 0;

			style.ChildRounding = 0;
			style.FrameRounding = 0;
			style.GrabRounding = 0;
			style.PopupRounding = 0;
			style.ScrollbarRounding = 0;
			style.TabRounding = 0;
			style.WindowRounding = 0;

			style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.55f, 0.13f, 0.13f, 1.00f);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.09f, 0.98f);
			style.Colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.00f);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(0.17f, 0.10f, 0.10f, 0.94f);
			style.Colors[ImGuiCol_Border] = ImVec4(0.79f, 0.18f, 0.18f, 0.50f);
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.35f, 0.34f, 0.40f);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.60f, 0.60f, 0.60f, 0.67f);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(0.39f, 0.03f, 0.03f, 1.00f);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.63f, 0.01f, 0.01f, 1.00f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.00f, 0.00f, 0.49f);
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.29f, 0.05f, 0.05f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.53f);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.39f, 0.00f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.62f, 0.00f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.85f, 0.00f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(0.85f, 0.11f, 0.03f, 1.00f);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.66f, 0.00f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_Button] = ImVec4(1.00f, 0.10f, 0.10f, 0.40f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.95f, 0.21f, 0.02f, 1.00f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.27f, 0.00f, 0.05f, 1.00f);
			style.Colors[ImGuiCol_Header] = ImVec4(0.53f, 0.00f, 0.00f, 0.31f);
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.60f, 0.06f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.92f, 0.16f, 0.16f, 1.00f);
			style.Colors[ImGuiCol_Separator] = ImVec4(0.94f, 0.00f, 0.07f, 0.50f);
			style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 0.00f, 0.12f, 1.00f);
			style.Colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 0.05f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.79f, 0.03f, 0.14f, 0.20f);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.00f, 0.00f, 0.95f);
			style.Colors[ImGuiCol_Tab] = ImVec4(0.41f, 0.06f, 0.00f, 0.86f);
			style.Colors[ImGuiCol_TabHovered] = ImVec4(0.78f, 0.00f, 0.31f, 0.80f);
			style.Colors[ImGuiCol_TabActive] = ImVec4(0.72f, 0.01f, 0.01f, 1.00f);
			style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.23f, 0.00f, 0.00f, 0.97f);
			style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.85f, 0.03f, 0.03f, 1.00f);
			style.Colors[ImGuiCol_PlotLines] = ImVec4(0.41f, 0.00f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.62f, 0.10f, 0.02f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.00f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.86f, 0.00f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.27f, 0.07f, 0.07f, 0.35f);
			style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 0.32f, 0.00f, 0.90f);
			style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.44f, 0.01f, 0.01f, 1.00f);
			style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.78f, 0.00f, 0.00f, 0.70f);
			style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.75f, 0.03f, 0.03f, 0.38f);
			style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.2f);
		}

		// ADD FONTS
		{
			static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
			ImFontConfig icons_config;

			io.IniFilename = nullptr;
			io.LogFilename = nullptr;

			icons_config.MergeMode = true;
			icons_config.PixelSnapH = true;
			icons_config.OversampleH = 3;
			icons_config.OversampleV = 3;

			ImFontConfig rubik;
			rubik.FontDataOwnedByAtlas = false;

			io.Fonts->AddFontFromMemoryTTF((void*)roboto_medium_font_data, sizeof(roboto_medium_font_data), 19.f, &rubik);
			io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 19.f, &icons_config, icons_ranges);

		}
	}
}
