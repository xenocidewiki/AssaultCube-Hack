#include "hacks.hpp"

constexpr float math_pi = 3.14159265358979323846;
std::unique_ptr<Player> player_ptr = std::make_unique<Player>();

void Hackeroni::aimbot()
{
	Player* local_player = player_ptr->read_local_player();

	Player* target;

	get_closest_player(local_player, target);

	Vector3D yoinked;
	yoinked.x = 0;
	yoinked.y = 0;
	yoinked.z = 0;
	if (target->health > 0) {
		yoinked.x = target->player_pos.x - local_player->player_pos.x;
		yoinked.y = target->player_pos.y - local_player->player_pos.y;
		yoinked.z = target->player_pos.z - local_player->player_pos.z;

		float mag = sqrt(pow(yoinked.x, 2) + pow(yoinked.y, 2));

		local_player->viewangles.y = 180 * asinf(yoinked.z / mag) / math_pi;
		local_player->viewangles.x = 90 + 180 * atanf(yoinked.y / yoinked.x) / math_pi;

		if (yoinked.x < 0)
			local_player->viewangles.x += 180;
	}
}

void Hackeroni::get_closest_player(Player* local_player, Player* &closest_player_out)
{
	float target_angle	= 999999;
	int player_count	= player_ptr->read_player_count();

	for (int i = 1; i < player_count; i++) {
		Player* current_entity = player_ptr->read_entities(player_count);

		if (current_entity == nullptr)
			continue;

		if (current_entity->teamnum == local_player->teamnum)
			continue;

		if (current_entity->health < 0 || current_entity->health > 100)
			continue;

		Vector3D yoinked;

		yoinked.x = current_entity->player_pos.x - local_player->player_pos.x;
		yoinked.y = current_entity->player_pos.y - local_player->player_pos.y;

		float new_angle = 90 + 180 * atanf(yoinked.y / yoinked.x) / 3.14159265358979323846;

		if (yoinked.x < 0)
			new_angle += 180;

		if (new_angle < target_angle) {
			target_angle = new_angle;
			closest_player_out = current_entity;
		}

	}
}

void Hackeroni::world_to_screen(Vector3D enemy_coordinates, Vector2D &pixel_coordinates)
{
	//I know how to read matrices from memory ex dee
	std::vector<float> matrix;
	float* matrix_ptr = (float*)0x501AE8;

	for (int i = 0; i < 16; i++)
		matrix.push_back(*(matrix_ptr + i));

	float screen_x = 1024 / 2.0F;
	float screen_y = 768 / 2.0F;

	//could be made into a function but i honestly can't be fucked to do it
	float x = enemy_coordinates.x * matrix[0] + enemy_coordinates.y * matrix[4] + enemy_coordinates.z * matrix[8] + matrix[12];
	float y = enemy_coordinates.x * matrix[1] + enemy_coordinates.y * matrix[5] + enemy_coordinates.z * matrix[9] + matrix[13];
	float z = enemy_coordinates.x * matrix[2] + enemy_coordinates.y * matrix[6] + enemy_coordinates.z * matrix[10] + matrix[14];
	float w = enemy_coordinates.x * matrix[3] + enemy_coordinates.y * matrix[7] + enemy_coordinates.z * matrix[11] + matrix[15];

	pixel_coordinates.x = screen_x + (screen_x * x / w);
	pixel_coordinates.y = screen_y - (screen_y * y / w);
}

void Hackeroni::draw_esp()
{
	Player* local_player	= player_ptr->read_local_player();
	int player_count		= player_ptr->read_player_count();

	for (int i = 0; i < player_count; i++) {
		Player* current_entity = player_ptr->read_entities(i);

		if (current_entity == nullptr)
			continue;

		if (current_entity->teamnum == local_player->teamnum)
			continue;

		if (current_entity->health < 0 || current_entity->health > 100)
			continue; 

		Vector2D screen_head, screen_foot;

		world_to_screen(current_entity->player_pos, screen_head);
		world_to_screen(current_entity->foot_pos, screen_foot);

		float height = abs(screen_foot.y - screen_head.y);
		float width = height / 2.0F;

		if (!player_ptr->is_visible(local_player->player_pos, current_entity->player_pos))
			OpenGL::draw_box(screen_head.x - width / 2, screen_head.y, screen_head.x + width / 2, screen_head.y + height, 255, 0, 0);
		else
			OpenGL::draw_box(screen_head.x - width / 2, screen_head.y, screen_head.x + width / 2, screen_head.y + height, 255, 255, 0);

	}
}