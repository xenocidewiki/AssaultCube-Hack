#include "gameinfo.hpp"

std::unique_ptr<Offsets> offsets = std::make_unique<Offsets>();

Player* Player::read_local_player()
{
	return reinterpret_cast<Player*>(*offsets->offset_localplayer);
}

std::vector<Player*> Player::read_entities(int player_count)
{

}

int Player::read_player_count()
{

}
