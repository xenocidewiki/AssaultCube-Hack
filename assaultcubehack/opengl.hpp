#pragma once
#include <gl/GL.h>
#include <gl/glu.h>

#pragma comment(lib, "opengl32.lib")

namespace OpenGL 
{
	void draw_line(float x_src, float y_src, float x_dst, float y_dst);

	void draw_box(int x, int y, int x2, int y2);
}
