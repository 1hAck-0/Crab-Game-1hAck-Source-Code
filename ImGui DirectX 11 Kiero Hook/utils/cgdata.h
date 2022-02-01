#pragma once
#include <cstdint>
#include <vector>
#include "../menu/menu.h"

extern uintptr_t staticAddress(uintptr_t ptr, const std::vector<int>& offsets);

extern std::uintptr_t IsValidPointer(std::uintptr_t base, const std::vector<uintptr_t>& offsets);

extern void RGBEffect(bool& start_of_colors, const float& speed, float* const rgb_out_put, float* const hsv_out_put = nullptr);

inline constexpr const char* const Items_GameModes[19] = {
	"Waiting Room", // 0
	"Red Light Green Light", // 1
	"King of The Hill", // 2
	"Stepping Stones", // 3
	"Tag", // 4
	"Hide and Seek", // 5
	"Bomb Tag", // 6
	"Lights out", // 7
	"The Floor is Lava", // 8
	"Tile Drive", // 9
	"Hat King", // 10
	"Race", // 11
	"Bustling Buttons", // 12
	"Practice", // 13
	"Death From Above", // 14
	"Block Drop", // 15
	"Snow Brawl", // 16
	"Dodgeball", // 17
	"Crab Fight", // 18
};

inline constexpr const char* const Items_Maps[63] = {
	"Random", // random
	"Bitter Beach", // 0
	"Blueline", // 1
	"Cocky Containers", // 2
	"Color Climb", // 3
	"Crusty Rocks", // 4
	"Desert", // 5
	"Dorm", // 6
	"Funky Field", // 7
	"Glass Jump", // 8
	"Hasty Hill", // 9
	"Icy Crack", // 10
	"Icy Islands", // 11
	"Icy Rocks", // 12
	"Islands", // 13
	"Karlson", // 14
	"Lanky Lava", // 15
	"Lava Lake", // 16
	"Plains", // 17
	"Playground", // 18
	"Playground2", // 19
	"Return to Monke", // 20
	"Sandstorm", // 21
	"Slippery Slope", // 22
	"Small Color Climb", // 23
	"Small Glass Jump", // 24
	"Small Hill", // 25
	"Small Icy Islands", // 26
	"Small Islands", // 27
	"Small Playground", // 28
	"Snowtop", // 29
	"Splat",// 30
	"Splot",// 31
	"Sunny Saloon",// 32
	"Toxic Train",// 33
	"Twisted Towers",// 34
	"Mini Monke", // 35
	"Small Beach", // 36
	"Small Saloon", // 37
	"Small Containers", // 38
	"Tiny Town 2", // 39
	"Tiny Town", // 40
	"Dodgy Fields", // 41
	"Dodgy Ice", // 42
	"Dodgy Snow", // 43
	"Dodgy Streets", // 44
	"Sandy Islands", // 45
	"Small Sandy Islands", // 46
	"Cheeky Chamber", // 47
	"Lava Drop", // 48
	"Lava Dump", // 49
	"Peaceful Platform", // 50
	"Salty Island", // 51
	"Skybox", // 52
	"Saucy Stage", // 53
	"Lava Climb", // 54
	"Macaroni Mountain", // 55
	"Sussy Sandcastle", // 56
	"Sussy Slope", // 57
	"Sandy Stones", // 58
	"Crabfields", // 59
	"Crabheat", // 60
	"Crabland", // 61
};

inline constexpr const char* const Items_Weapons[11] = {
	"Rifle", // 0
	"Pistol", // 1
	"Revolver", // 2
	"Double Barel", // 3
	"Bat", // 4
	"Bomb", // 5
	"Katana", // 6
	"Knife", // 7
	"Pipe", // 8
	"Snowball", // 9
	"Stick", // 10
};

namespace off
{
	namespace PlayerMovement
	{	
		inline constexpr const uintptr_t rb = 0x38;
		inline constexpr const uintptr_t moveSpeed = 0x44;
		inline constexpr const uintptr_t maxWalkSpeed = 0xA4;
		inline constexpr const uintptr_t maxRunSpeed = 0xBC;
		inline constexpr const uintptr_t maxSpeed = 0xD4;
		inline constexpr const uintptr_t grounded = 0x104;
		inline constexpr const uintptr_t jumpForce = 0x158;
		inline constexpr const uintptr_t swimSpeed = 0x1D8;
		inline constexpr const uintptr_t underWater = 0x208;
	}

	namespace OnlinePlayerMovement
	{
		inline constexpr const uintptr_t lastPositon = 0x38;
		inline constexpr const uintptr_t yRot = 0xC8;
		inline constexpr const uintptr_t playerScale = 0xCC;
		inline constexpr const uintptr_t playerManager = 0x18;
	}
	
	namespace Tile
	{
		inline constexpr const uintptr_t teamId = 0x6C;
	}

	namespace TileManager
	{
		inline constexpr const uintptr_t idToTeam = 0x38;
	}

	namespace Steam
	{
		inline constexpr const uintptr_t steamId = 0x3D828;
	}

	namespace PlayerManager
	{
		inline constexpr const uintptr_t username = 0x40;
	}

	namespace BustlingButtonInteract
	{
		inline constexpr const uintptr_t buttonId = 0x20;
	}

	namespace GameModeButtons
	{
		inline constexpr const uintptr_t dangerousButton = 0x38;
	}

	namespace MoveCamera
	{
		inline constexpr const uintptr_t mainCam = 0xF0;
	}

	namespace RedLightStatue
	{
		inline constexpr const uintptr_t currentTime = 0x5C;
		inline constexpr const uintptr_t head = 0x60;
	}

	namespace ItemData
	{
		inline constexpr const uintptr_t currentAmmo = 0x48;
	}
}
