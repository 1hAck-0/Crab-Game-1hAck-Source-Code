#include "pch.h"
#include "includes.h"
#include <vector>
#include <TlHelp32.h>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include "utils/config.h"
#include "menu/menu.h"

#pragma region menu stuff
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
Present oPresent;
HWND window = 0;
WNDPROC oWndProc;
ID3D11Device* pDevice = 0;
ID3D11DeviceContext* pContext = 0;
ID3D11RenderTargetView* mainRenderTargetView;
inline bool DoesFileExist(const char* const name)
{
	static struct stat buffer;
	return (stat(name, &buffer) == 0);
}
void InitImGui()
{
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	Menu::Setup(io);

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}
#pragma endregion



/*
COMPILE AND BUILD:

(Download Visual Studio 2022 Preview or higher and open the project in it)

1. Make sure you are in x64 Release build mode

2. Press CTRL + SHIFT + B to directly build



(If you get errors while building, you probably have some SDKs missing, make sure you have the latest Windows SDK and DirectX SDK)

DirectX SDK:			https://www.microsoft.com/en-us/download/details.aspx?id=6812
Microsoft Windows SDK:	https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/
(Download and install them in the default folder that the installers give you, unless you want to manually change the project settings to whatever folder you download the SDKs to)

*/

// MAIN CODE
HRESULT WINAPI hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (Menu::bInit)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, 0, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtrA(window, GWLP_WNDPROC, (LONG_PTR)WndProc);

			InitImGui();
			Menu::bInit = false;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	if (Menu::Render())
		Menu::bActive = false;


	ImGui::Render();
	pContext->OMSetRenderTargets(1, &mainRenderTargetView, 0);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

#include "cheat/hooks.h"
#include "kiero/minhook/include/MinHook.h"
#include "il2cpp/il2cpp_loader.h"
#include "utils/cgdata.h"
#include "menu/lua-scripts/lua_handler.h"

DWORD WINAPI Main(void* lpReserved)
{
	if (MH_Initialize() != MH_OK)
	{
		MessageBoxA(0, XorCStr("Couldn't initialize MinHook!"), XorCStr("MinHook Fatal Error"), MB_ICONERROR);
		return -1;
	}

	// INIT
	Client::Init();
	cfg.Init();
	il2cpp::load();
	if (!Hooks::Init())
	{
		std::string msg = XorStr("\
Couldn't initiliaze the cheat!\
This is probably due to the mod menu not working for this version of Crab Game.\n\
\n\
The Menu was made for v");
		msg += GameVersion;
		msg += XorStr(", please compare that number with your Crab Game version!");

		MessageBoxA(0, msg.c_str(), XorCStr("1hAck Fatal Error"), MB_ICONERROR);
		return -1;
	}

	// INIT KIERO HOOK
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)&oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);

	// MAIN LOOP
	while (true)
	{
		// close menu
		if (PressKey(Menu::CloseKey) || !Menu::bActive)
		{
			kiero::shutdown();
			Menu::bActive = false;
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	// SAVE CONFIG BEFORE LEAVE
	if (cfg.bAutoSave)
	{
		WIN32_FIND_DATAW data;
		if (FindFirstFileW(XorCStr(L"*.1hCG"), &data))
			cfg.Save(data.cFileName);
		else
			cfg.Save(XorCStr(L"auto_saved.1hCG"));
	}

	// UNINIT SCRIPTS
	for (USHORT i = 0; i < cfg.script.files.size(); i++)
	{
		auto script = &cfg.script.files[i];
		if (script->lua)
		{
			script->Uninit();
		}
	}
	if (cfg.script.currentScipt)
		cfg.script.currentScipt.release();

	// UNINIT
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	Hooks::Uninit();
	return 0;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, Main, hMod, 0, nullptr);

		return 1;
	}
	return 0;
}
