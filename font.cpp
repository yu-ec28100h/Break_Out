#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "Font.h"

#include "glut.h"
#include "glm/glm.hpp"

using namespace glm;
static vec2 position;
static float height = FONT_DEFAULT_HEIGHT;
static float weight = 1;

void fontBegin()
{
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);   //GLbitfield mask

	glMatrixMode(GL_PROJECTION);    //GLenum mode
	glLoadIdentity();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluOrtho2D(0, viewport[2], viewport[3], 0);    //GLdouble left, right, bottom, top

	glMatrixMode(GL_MODELVIEW);    //GLenum mode
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
}
void fontEnd()
{
	glPopMatrix();
	glPopAttrib();
}

void fontSetPosition(float _x, float _y)
{
	position = vec2(_x, _y);
}
void fontSetHeight(float _height)
{
	height = _height;
}
float fontGetWidth()
{
	return FONT_DEFAULT_WIDTH * height / FONT_DEFAULT_HEIGHT;
}
float fontGetHeight()
{
	return height;
}

float fontGetWeightMin()
{
	GLfloat weight[2];
	glGetFloatv(GL_LINE_WIDTH_RANGE, weight);

	return weight[0];
}
float fontGetWeightMax()
{
	GLfloat weight[2];
	glGetFloatv(GL_LINE_WIDTH_RANGE, weight);

	return weight[1];
}
void fontSetWeight(float _weight)
{
	weight = _weight;
}

float fontGetWeight()
{
	return weight;
}

void fontDraw(const char *_format, ...)
{
	va_list argList;
	va_start(argList, _format);
	char str[256];
	vsprintf_s(str, _format, argList);
	va_end(argList);

	glLineWidth(weight);
	char *p = str;
	glPushMatrix();
	{
		glTranslatef(position.x, position.y + height, 0);   //GLfloat x, y, z
		float s = height / FONT_DEFAULT_HEIGHT;
		glScalef(s, -s, s); //GLfloat x, y, z
		for (; (*p != '\0')&&(*p != '\n'); p++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
		}
	}
	glPopMatrix();
	if (*p == '\n')
	{
		glTranslatef(0, height + weight*2, 0);   //GLfloat x, y, z
		fontDraw(++p);
	}
	//glutStrokeCharacter(GLUT_STROKE_ROMAN, '@');
	//printf("%s\n", str);
}