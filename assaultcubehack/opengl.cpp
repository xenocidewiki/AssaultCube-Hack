#include <Windows.h>
#include "opengl.hpp"

void OpenGL::draw_line(float x_src, float y_src, float x_dst, float y_dst)
{
	glBegin(GL_LINES);
	glVertex2f(x_src, y_src);
	glVertex2f(x_dst, y_dst);
	glEnd();
}

void OpenGL::draw_box(int x, int y, int x2, int y2, int r, int g, int b)
{
	glDepthMask(GL_FALSE);
	glDisable(GL_TEXTURE_2D);
	glColor4f(r, g, b, 255);

	glBegin(GL_LINE_LOOP);

	glVertex2f(x, y);
	glVertex2f(x2, y);
	glVertex2f(x2, y2);
	glVertex2f(x, y2);

	glEnd();

	glEnable(GL_TEXTURE_2D);
	glDepthMask(GL_TRUE);
}