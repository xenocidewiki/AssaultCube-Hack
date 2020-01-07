#pragma once

#include <Windows.h>
#include <iostream>
#include <memory>
#include <math.h>
#include <vector>

struct Vector3D
{
	float x;
	float y;
	float z;
};

struct Vector2D
{
	float x;
	float y;
};

uintptr_t* detour_function(uintptr_t* original_address, uintptr_t detour_address, size_t length)
{
	unsigned long old_protect;
	VirtualProtect(original_address, length, PAGE_EXECUTE_READWRITE, &old_protect);

	uintptr_t detour_relative_address = detour_address - ((uintptr_t)original_address + 5);

	*(byte*)original_address = 0xE9;
	*(uint32_t*)((uint32_t)original_address + 1) = (uint32_t)detour_relative_address;

	for (int i = 5; i < length; i++)
		*(byte*)((uint32_t)original_address + i) = 0x90;

	VirtualProtect(original_address, length, old_protect, 0);

	return (uintptr_t*)((uintptr_t)original_address + 5);
}