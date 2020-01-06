#include "setup.hpp"

void Setup::init_hacks()
{
	std::unique_ptr<Hackeroni> hacks_ptr = std::make_unique<Hackeroni>();
	hacks_ptr->aimbot();
}