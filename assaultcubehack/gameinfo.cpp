#include "gameinfo.hpp"

std::unique_ptr<Offsets> offsets = std::make_unique<Offsets>();

Player* Player::read_local_player()
{
	return reinterpret_cast<Player*>(*offsets->offset_localplayer);
}

Player* Player::read_entities(int entity_index)
{
	return reinterpret_cast<Player*>(*(*offsets->offset_entities + entity_index));
}

int Player::read_player_count()
{
	return *offsets->offset_playercount;
}

//Calls the game's internal raytracing function which checks whether you can see an enemy player.
bool Player::is_visible(Vector3D from, Vector3D to)
{
	__asm
	{
		push to.z;
		push to.y;
		push to.x;

		push from.z;
		push from.y;
		push from.x;

		xor cl, cl;
		xor eax, eax;
		mov ebx, 0x48ABD0;
		call ebx;
		add esp, 0x18;
	}
}

Screen* Screen::get_screen_settings()
{
	return reinterpret_cast<Screen*>(0x510C94);
}