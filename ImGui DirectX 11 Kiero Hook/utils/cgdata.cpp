#include "pch.h"
#include "cgdata.h"
#include "../il2cpp/il2cpp_loader.h"
#include "config.h"
#include <TlHelp32.h>

inline uintptr_t staticAddress(uintptr_t ptr, const std::vector<int>& offsets)
{
	for (const uintptr_t& offset : offsets)
		ptr = (*(uintptr_t*)ptr) + offset;
	return ptr;
}

template <typename T>
inline T read(const uintptr_t& address)
{
	T buffer;
	ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<LPCVOID>(address), &buffer, sizeof(T), nullptr);
	return buffer;
}
inline uintptr_t IsValidPointer(uintptr_t base, const std::vector<uintptr_t>& offsets)
{
	DWORD oldProtect, newProtect;
	uintptr_t address = base;

	for (int i = 0; i < offsets.size(); ++i)
	{
		if (VirtualProtect((void*)(address), sizeof(address), PAGE_EXECUTE_READWRITE, &oldProtect))
			VirtualProtect((void*)(address),sizeof(address),oldProtect, &newProtect);
		else
			return 0;

		address = read<uintptr_t>(address);
		address += offsets.at(i);
	}

	return address;
}
