#include "hacks.hpp"
#include <math.h>

void Hackeroni::aimbot()
{
	while (1) {
		unsigned long *moron = (unsigned long*)0x50f4f4;
		unsigned long deref = *moron;
		Player* deref_localplayer_ptr = reinterpret_cast<Player*>(deref);

		//redo this in getclosestplayer, and return a pointer to the entity or w/e 
		//then do the last part of the aimbot calc using that entity

		Player* target;

		get_closest_player(target);

		Vector3D yoinked;
		yoinked.x = 0;
		yoinked.y = 0;
		yoinked.z = 0;
		if (target->health > 0) {
			yoinked.x = target->player_pos.x - deref_localplayer_ptr->player_pos.x;
			yoinked.y = target->player_pos.y - deref_localplayer_ptr->player_pos.y;
			yoinked.z = target->player_pos.z - deref_localplayer_ptr->player_pos.z;

			float mag = sqrt(pow(yoinked.x, 2) + pow(yoinked.y, 2));

			deref_localplayer_ptr->viewangles.y = 180 * asinf(yoinked.z / mag) / 3.14159265358979323846;
			deref_localplayer_ptr->viewangles.x = 90 + 180 * atanf(yoinked.y / yoinked.x) / 3.14159265358979323846;

			if (yoinked.x < 0)
				deref_localplayer_ptr->viewangles.x += 180;
		}
	}
}

void Hackeroni::get_closest_player(Player* &out)
{
	unsigned long* moron = (unsigned long*)0x50f4f4;
	unsigned long deref = *moron;
	Player* deref_localplayer_ptr = reinterpret_cast<Player*>(deref);
	float target_angle = 999999;

	for (int i = 1; i < 32; i++) {
		unsigned long** ent_ptr = (unsigned long**)(0x50f4f8);
		unsigned long* ent_deref = *(ent_ptr)+i;

		Player* yeet = (Player*)(*ent_deref);

		if (yeet == nullptr)
			continue;

		if (*ent_deref < 0x40000)
			continue;

		if (deref_localplayer_ptr->teamnum == yeet->teamnum)
			continue;

		Vector3D yoinked;

		yoinked.x = yeet->player_pos.x - deref_localplayer_ptr->player_pos.x;
		yoinked.y = yeet->player_pos.y - deref_localplayer_ptr->player_pos.y;

		float new_angle = 90 + 180 * atanf(yoinked.y / yoinked.x) / 3.14159265358979323846;

		if (yoinked.x < 0)
			new_angle += 180;

		if (new_angle < target_angle) {
			target_angle = new_angle;
			out = yeet;
		}

	}
}

void Hackeroni::telehack()
{
	unsigned long* moron = (unsigned long*)0x50f4f4;
	unsigned long deref = *moron;
	Player* deref_localplayer_ptr = reinterpret_cast<Player*>(deref);
	float target_angle = 999999;

	for (int i = 1; i < 32; i++) {
		unsigned long** ent_ptr = (unsigned long**)(0x50f4f8);
		unsigned long* ent_deref = *(ent_ptr)+i;

		Player* yeet = (Player*)(*ent_deref);

		if (yeet == nullptr)
			continue;

		if (*ent_deref < 0x40000)
			continue;

		if (deref_localplayer_ptr->teamnum == yeet->teamnum)
			continue;

		yeet->player_pos.x = deref_localplayer_ptr->player_pos.x;
		yeet->player_pos.y = deref_localplayer_ptr->player_pos.y;
	}
}