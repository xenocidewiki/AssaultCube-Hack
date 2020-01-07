#include "hacks.hpp"

void Hackeroni::aimbot()
{
	while (1) {
		unsigned long *moron = (unsigned long*)0x50f4f4;
		unsigned long deref = *moron;
		Player* deref_localplayer_ptr = reinterpret_cast<Player*>(deref);

		//redo this in getclosestplayer, and return a pointer to the entity or w/e 
		//then do the last part of the aimbot calc using that entity

		Player* target;

		get_closest_player(target); // This can be done in a better way jesus christ

		Vector3D yoinked;
		yoinked.x = 0;
		yoinked.y = 0;
		yoinked.z = 0;
		if (target->health > 0) {
			//What the FUCK
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

void Hackeroni::telehack()//doesnt work fam
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

void Hackeroni::world_to_screen(Vector3D enemy_coordinates, Vector2D &pixel_coordinates)
{
	std::vector<float> matrix;
	float* matrix_ptr = (float*)0x501AE8;

	for (int i = 0; i < 16; i++)
		matrix.push_back((float)(*(matrix_ptr + i)));

	float screen_x = 1024 / 2.0F;
	float screen_y = 768 / 2.0F;

	float x = enemy_coordinates.x * matrix[0] + enemy_coordinates.y * matrix[4] + enemy_coordinates.z * matrix[8] + matrix[12];
	float y = enemy_coordinates.x * matrix[1] + enemy_coordinates.y * matrix[5] + enemy_coordinates.z * matrix[9] + matrix[13];
	float z = enemy_coordinates.x * matrix[2] + enemy_coordinates.y * matrix[6] + enemy_coordinates.z * matrix[10] + matrix[14];
	float w = enemy_coordinates.x * matrix[3] + enemy_coordinates.y * matrix[7] + enemy_coordinates.z * matrix[11] + matrix[15];

	pixel_coordinates.x = screen_x + (screen_x * x / w);
	pixel_coordinates.y = screen_y - (screen_y * y / w);
}

void Hackeroni::draw_esp()
{
	unsigned long* moron = (unsigned long*)0x50f4f4;
	unsigned long deref = *moron;
	Player* deref_localplayer_ptr = reinterpret_cast<Player*>(deref);

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

		Vector2D screen_head, screen_foot;

		world_to_screen(yeet->player_pos, screen_head);
		world_to_screen(yeet->foot_pos, screen_foot);

		float height = abs(screen_foot.y - screen_head.y);
		float width = height / 2.0F;

		OpenGL::draw_box(screen_head.x - width / 2, screen_head.y, screen_head.x + width / 2, screen_head.y + height);
	}
}