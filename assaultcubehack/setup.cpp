#include "setup.hpp"

void Setup::init_detour()
{
	original_func = detour_function((uintptr_t*)0x40C375, (uintptr_t)drawhud_hook, 13);
}

void Setup::init_hacks()
{
	std::unique_ptr<Hackeroni> hacks_ptr = std::make_unique<Hackeroni>();
	hacks_ptr->draw_esp();
}

void Setup::drawhud_end_hook()
{
	glPushMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLoadIdentity();
	glOrtho(0, 1024, 768, 0, 0, 1);
	glDepthMask(GL_FALSE);
	glDisable(GL_TEXTURE_2D);

	init_hacks();

	glDepthMask(GL_TRUE);
	glEnable(GL_TEXTURE_2D);

	glPopMatrix();

}
