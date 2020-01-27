#pragma once

#include "gameinfo.hpp"
#include "opengl.hpp"

struct Hackeroni
{
	void draw_esp();
	void aimbot();
	void telehack();
	void get_closest_player(Player* local_player, Player* &closest_player_out);
	void get_closest_player_to_crosshair(Player* local_player, Player*& closest_player_out);
	int world_to_screen(Vector3D enemy_coordinates, Vector2D &pixel_coordinates);
};
