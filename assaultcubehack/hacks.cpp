#include "hacks.hpp"

constexpr float math_pi = 3.14159265358979323846;
std::unique_ptr<Player> player_ptr = std::make_unique<Player>();
std::unique_ptr<Screen> screen_ptr = std::make_unique<Screen>();
Screen* screen = screen_ptr->get_screen_settings();

void Hackeroni::aimbot()
{
	Player* local_player = player_ptr->read_local_player();

	Player* target = 0;

	get_closest_player_to_crosshair(local_player, target);

	if (target != 0 && target->health > 0) {
		local_player->viewangles.y = 180 * asinf((target->player_pos.z - local_player->player_pos.z) / sqrt(pow((target->player_pos.x - local_player->player_pos.x), 2) + pow((target->player_pos.y - local_player->player_pos.y), 2))) / math_pi;
		local_player->viewangles.x = 90 + 180 * atanf((target->player_pos.y - local_player->player_pos.y) / (target->player_pos.x - local_player->player_pos.x)) / math_pi;

		if ((target->player_pos.x - local_player->player_pos.x) < 0)
			local_player->viewangles.x += 180;
	}
}

/*
Dumb solution to getting the closest player. I am using the one below that instead.
*/
void Hackeroni::get_closest_player(Player* local_player, Player* &closest_player_out)
{
	float target_angle	= 999999;
	int player_count	= player_ptr->read_player_count();

	for (int i = 0; i < player_count; i++) {
		Player* current_entity = player_ptr->read_entities(i);

		if (current_entity == nullptr)
			continue;

		if (current_entity->teamnum == local_player->teamnum)
			continue;

		if (current_entity->health < 0 || current_entity->health > 100)
			continue;

		if (!player_ptr->is_visible(local_player->player_pos, current_entity->player_pos))
			continue;

		float new_angle = 90 + 180 * atanf((current_entity->player_pos.x - local_player->player_pos.x) / (current_entity->player_pos.y - local_player->player_pos.y)) / math_pi;

		if ((current_entity->player_pos.x - local_player->player_pos.x) < 0)
			new_angle += 180;

		if (new_angle < target_angle) {
			target_angle = new_angle;
			closest_player_out = current_entity;
		}

	}
}

//Calculates straight line distance from the position of your crosshair to the players head, and picks the closest player (least distance)
void Hackeroni::get_closest_player_to_crosshair(Player* local_player, Player*& closest_player_out)
{
	float target_dist		= 999999;
	int player_count		= player_ptr->read_player_count();

	for (int i = 0; i < player_count; i++) {
		Player* current_entity = player_ptr->read_entities(i);

		if (current_entity == nullptr)
			continue;

		if (current_entity->teamnum == local_player->teamnum)
			continue;

		if (current_entity->health < 0 || current_entity->health > 100)
			continue;

		if (!player_ptr->is_visible(local_player->player_pos, current_entity->player_pos))
			continue;

		Vector2D screen_head;

		if (world_to_screen(current_entity->player_pos, screen_head)) {

			float screen_x = screen->screen_x / 2.0f;
			float screen_y = screen->screen_y / 2.0f;

			float new_dist = sqrt(pow((screen_head.x - screen_x), 2) + pow(screen_head.y - screen_y, 2));

			if (new_dist < target_dist) {
				target_dist = new_dist;
				closest_player_out = current_entity;
			}
		}

	}
}

//W2S transform
int Hackeroni::world_to_screen(Vector3D enemy_coordinates, Vector2D &pixel_coordinates)
{
	std::vector<float> matrix;
	float* matrix_ptr = (float*)0x501AE8;

	for (int i = 0; i < 16; i++)
		matrix.push_back(*(matrix_ptr + i));

	float screen_x = screen->screen_x / 2.0F;
	float screen_y = screen->screen_y / 2.0F;

	//Should be turned into a readmatrix function or something along those lines
	float x = enemy_coordinates.x * matrix[0] + enemy_coordinates.y * matrix[4] + enemy_coordinates.z * matrix[8] + matrix[12];
	float y = enemy_coordinates.x * matrix[1] + enemy_coordinates.y * matrix[5] + enemy_coordinates.z * matrix[9] + matrix[13];
	float z = enemy_coordinates.x * matrix[2] + enemy_coordinates.y * matrix[6] + enemy_coordinates.z * matrix[10] + matrix[14];
	float w = enemy_coordinates.x * matrix[3] + enemy_coordinates.y * matrix[7] + enemy_coordinates.z * matrix[11] + matrix[15];

	if (w < 0.01F)
		return 0;

	pixel_coordinates.x = screen_x + (screen_x * x / w);
	pixel_coordinates.y = screen_y - (screen_y * y / w);

	return 1;
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

		if (world_to_screen(current_entity->player_pos, screen_head) && world_to_screen(current_entity->foot_pos, screen_foot)) {

			float height = abs(screen_foot.y - screen_head.y);
			float width = height / 2.0F;

			if (!player_ptr->is_visible(local_player->player_pos, current_entity->player_pos))
				OpenGL::draw_box(screen_head.x - width / 2, screen_head.y, screen_head.x + width / 2, screen_head.y + height, 255, 0, 0);
			else
				OpenGL::draw_box(screen_head.x - width / 2, screen_head.y, screen_head.x + width / 2, screen_head.y + height, 0, 102, 255);
		}

	}
}