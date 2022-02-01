#pragma once
#include "../includes.h"
#include "../utils/xorstr.hpp"
#include "../utils/config.h"
#include <vector>

inline void RGBEffect(bool& start_of_colors, const float& speed, float* const rgb_out_put, float* const hsv_out_put)
{
	if (start_of_colors)
		start_of_colors = false, rgb_out_put[0] = 255, rgb_out_put[1] = 47, rgb_out_put[2] = 0;

	else
	{
		if (rgb_out_put[0] >= 255 && rgb_out_put[1] < 255 && rgb_out_put[2] <= 0) rgb_out_put[1] += speed;
		else if (rgb_out_put[0] > 0 && rgb_out_put[1] >= 255 && rgb_out_put[2] <= 0) rgb_out_put[0] -= speed;
		else if (rgb_out_put[0] <= 0 && rgb_out_put[1] >= 255 && rgb_out_put[2] < 255) rgb_out_put[2] += speed;
		else if (rgb_out_put[2] >= 255 && rgb_out_put[0] <= 0 && rgb_out_put[1] > 0)  rgb_out_put[1] -= speed;
		else if (rgb_out_put[0] < 255 && rgb_out_put[1] <= 0 && rgb_out_put[2] >= 255) rgb_out_put[0] += speed;
		else if (rgb_out_put[0] >= 255 && rgb_out_put[1] <= 0 && rgb_out_put[2] > 4) rgb_out_put[2] -= speed;
		else if (rgb_out_put[0] >= 255 && rgb_out_put[1] < 47)
		{
			rgb_out_put[1] += speed;
			if (rgb_out_put[2] > 0) rgb_out_put[2] -= speed;
		}
	}

	if (hsv_out_put)
	{
		hsv_out_put[0] = rgb_out_put[0] / 255.f;
		hsv_out_put[1] = rgb_out_put[1] / 255.f;
		hsv_out_put[2] = rgb_out_put[2] / 255.f;
	}
}
void ImGui_HotkeyButton(USHORT& dst, const char* const label, const char* new_button_id, bool& get_key);

typedef struct Vec2;
typedef struct Vec3;
struct Vec2
{
	float x, y;

	Vec2(){}
	Vec2(const float&x, const float& y) : x(x), y(y) {}

	static float Distance(const Vec2& a, const Vec2& b) { return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)); }

	operator ImVec2() const { return ImVec2(x, y); }
};

struct Vec3
{
	float x, y, z;
	static float Distance(const Vec3& a, const Vec3& b) { return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z)); }

	operator Vec2() const { return Vec2(x, y); }
	Vec3 operator-(Vec3 b) const { return { x - b.x, y - b.y, z - b.z }; }
	Vec3 operator-=(Vec3 b) const { return { x - b.x, y - b.y, z - b.z }; }
	Vec3 operator+(Vec3 b) const { return { x + b.x, y + b.y, z + b.z }; }
	Vec3 operator+=(Vec3 b) const { return { x + b.x, y + b.y, z + b.z }; }
	Vec3 operator*(Vec3 b) const { return { x * b.x, y * b.y, z * b.z }; }
	Vec3 operator*=(Vec3 b) const { return { x * b.x, y * b.y, z * b.z }; }
	Vec3 operator*(float b) const { return { x * b, y * b, z * b }; }
	Vec3 operator*=(float b) const { return { x * b, y * b, z * b }; }
	Vec3 operator/(Vec3 b) const { return { x / b.x, y / b.x, z / b.x }; }
	Vec3 operator/=(Vec3 b) const { return { x / b.x, y / b.x, z / b.x }; }
	Vec3 operator/(float b) const { return { x / b, y / b, z / b }; }
	Vec3 operator/=(float b) const { return { x / b, y / b, z / b }; }
};

struct Vec4
{
	float x, y, z, w;
};
using vec2 = Vec2;
using vec3 = Vec3;
using vec4 = Vec4;

#define PressKey(key) GetAsyncKeyState(key) & 1 && GetAsyncKeyState(key) & 0x8000
#define HoldKey(key) GetAsyncKeyState(key) & 0x8000
#define M_PI 3.14159265359f
#define ImGuiColorPicker(colors, show, id) if (ImGui::ColorButton(id, ImVec4(colors[0], colors[1], colors[2], 1.0f))) show = !show;\
if (show)\
{\
	ImGui::SameLine(0, 5);\
	ImGui::SetNextItemWidth(170);\
	ImGui::ColorPicker4(id, colors, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoSidePreview);\
}

inline const std::string MenuVersion = XorStr("3.4");
inline const std::string GameVersion = XorStr("1.362");

namespace ImGui
{
	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values);
}

namespace Menu
{
	extern bool bInit;
	extern bool bShowMenu;
	extern bool bActive;
	extern bool bLockInPlace;
	extern byte tab;
	inline bool bMenuStatusChanged = false;

	extern USHORT ToggleKey;
	extern USHORT CloseKey;

	struct MBCallback
	{
		char message[256] = { 0 }, title[256] = { 0 };
		DWORD flags = 0;
		void(*Callback)(int MessageBoxResult) = nullptr;

		MBCallback() {}
		MBCallback(const char* msg, const char* title_, DWORD flags_ = 0, void(*pCallback)(int) = nullptr) :
			flags(flags_), Callback(pCallback)
		{
			try
			{
				strcpy_s(message, msg);
				strcpy_s(title, title_);
			}
			catch (...) {}
			this->Call();
		}

		__forceinline void Call()
		{
			MBCallback* buf = new MBCallback();
			*(MBCallback*)buf = *this;
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MBThread, buf, 0, 0);
		}

	private:
		static DWORD MBThread(void* lpParam)
		{
			if (!lpParam) return 0;
			MBCallback* callback = (MBCallback*)lpParam;

			int result = MessageBoxA(Client::hwnd, callback->message, callback->title, callback->flags);

			if (callback->Callback)
				callback->Callback(result);

			delete callback;
			return 0;
		}
	};

	void RefreshScripts();

	bool Render();

	void Setup(ImGuiIO& io);

	bool CheckForUpdates(const bool& bNotifyIfUptodate = true);
}

