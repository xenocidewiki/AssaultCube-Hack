#include "setup.hpp"
#include "hooks.hpp"

void Setup::init_detour()
{
	original_func = detour_function((uintptr_t*)0x40C375, (uintptr_t)drawhud_hook, 13);
}

void Setup::init_hacks()
{
	std::unique_ptr<Hackeroni> hacks_ptr = std::make_unique<Hackeroni>();
	hacks_ptr->draw_esp();
	hacks_ptr->aimbot();
}

void Setup::drawhud_end_hook()
{
	std::unique_ptr<Screen> screen_ptr = std::make_unique<Screen>();
	Screen* screen = screen_ptr->get_screen_settings();
	glPushMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLoadIdentity();
	glOrtho(0, screen->screen_x, screen->screen_y, 0, 0, 1);
	glDepthMask(GL_FALSE);
	glDisable(GL_TEXTURE_2D);

	init_hacks();

	glDepthMask(GL_TRUE);
	glEnable(GL_TEXTURE_2D);

	glPopMatrix();

}
