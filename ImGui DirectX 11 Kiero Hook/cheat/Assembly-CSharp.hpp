#pragma once
#include "cheat.h"
#include "UnityEngine.hpp"

namespace Assembly_CSharp
{
	class Packet
	{
	private:
		char pad_0000[0x10];
		void* buffer;

	public:
		generic_array* readableBuffer;
		int readPos;
		bool disposed;

	public:
		Packet() {}
		Packet(const void* src, const size_t& len)
		{

		}

		void* get_data() { return (void*)&readableBuffer->items[0]; }
	};
}
