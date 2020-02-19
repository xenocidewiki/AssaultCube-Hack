#pragma once
#include "utils.hpp"

struct Offsets
{
	const uintptr_t* offset_localplayer		= reinterpret_cast<const uintptr_t*>(0x50F4F4);
	const uintptr_t** offset_entities		= reinterpret_cast<const uintptr_t**>(0x50F4F8);
	const uintptr_t* offset_w2s_matrix		= reinterpret_cast<const uintptr_t*>(0x501AE8);
	const uintptr_t* offset_playercount		= reinterpret_cast<const uintptr_t*>(0x50F500);
};

struct Player
{
	char pad_0000[4];		//0x0000
	Vector3D player_pos;	//0x0004
	char pad_0010[36];		//0x0010
	Vector3D foot_pos;
	Vector3D viewangles;	//0x0040
	char pad_004C[172];		//0x004C
	int32_t health;			//0x00F8
	int32_t armor;			//0x00FC
	char pad_0100[293];		//0x0100
	char playername[16];	//0x0225
	char pad_0235[247];		//0x0235
	int8_t teamnum;			//0x032C

	Player* read_local_player();
	Player* read_entities(int entity_index);
	int read_player_count();
	bool is_visible(Vector3D from, Vector3D to);
};

struct Screen
{
	int32_t screen_x;
	int32_t screen_y;

	Screen* get_screen_settings();
};