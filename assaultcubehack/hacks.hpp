#pragma once

#include "playerinfo.hpp"

struct Hackeroni
{
	void draw_esp();
	void aimbot();
	void telehack();
	void get_closest_player(Player* &out);
	void world_to_screen(Vector3D &enemy_coordinates, Vector2D &pixel_coordinates);
};
