#include <stdio.h>
#include <stdlib.h>

#include "Font.h"
#include "Audio.h"
#include "Rect.h"
#include "Ball.h"

#include "glut.h"
#include "glm/glm.hpp"

using namespace glm;

ivec2 windowSize = { 800, 600 };

bool keys[256];

Rect field;
Ball ball = {8};

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);    //GLenum mode
	glLoadIdentity();
	gluOrtho2D(0, windowSize.x, windowSize.y, 0);    //GLdouble left, right, bottom, top

	glMatrixMode(GL_MODELVIEW);    //GLenum mode
	glLoadIdentity();
	
	glColor3ub(0xff, 0xff, 0xff);
	glRectf(field.m_position.x - 8, 0,
			field.m_position.x + field.m_size.x + 8, windowSize.y);
	glColor3ub(0x00, 0x00, 0x00);
	field.draw();

	glColor3ub(0xff, 0xff, 0xff);
	ball.draw()

	fontBegin();
	fontSetHeight(FONT_DEFAULT_HEIGHT);
	fontSetPosition(0, 0);
	//fontDraw("");
	fontEnd();

	glutSwapBuffers();
}

void idle(void)
{
	audioUpdate();
	ball.update();

	if((ball.m_position.x < field.m_position.x)
		||(ball.m_position.x >= field.m_position.x + field.m_size.x))
	{
		ball.m_position = ball.m_lastPosition;
		ball.m_speed.y *= -1;
	}
	if((ball.m_position.y >= field.m_position.y + field.m_size.y)
		||(ball.m_position.y < field.m_position.y))
	{
		ball.m_position = ball.m_lastPosition;
		ball.m_speed.y *= -1;
	}

	glutPostRedisplay();
}

void reshape(int width, int height)
{
	printf("reshape: width:%d height:%d\n", width, height);
	glViewport(0, 0, width, height);    //GLint x, y / GLsizei width, height
	windowSize = ivec2(width, height);

	float frameHeight = 16;
	field.m_size.y = windowSize.y - frameHeight;
	field.m_size.x = field.m_size.y;
	field.m_position.x = (windowSize.x - field.m_size.x) / 2;
	field.m_position.y = frameHeight;

	ball.m_lastPosition = ball.m_position = vec2(field.m_position.x, field.m_position.y + field.m_size.y / 2);
	ball.m_speed = vec2(1, 1) * 2.f;
}

void keyboard(unsigned char key, int x, int y)
{
	printf("keyboard: \'%c\' (%#x)\n", key, key);
	if (key == 0x1b)
		exit(0);
	
	keys[key] = true;
}
void keyboardUp(unsigned char key, int x, int y)
{
	printf("keyboardUp: \'%c\' (%#x)\n", key, key);
	keys[key] = false;
}

void passiveMotion(int x, int y)
{
	printf("passiveMotion: x:%d y:%d\n", x, y);
}

int main(int argc, char *argv[])
{
	audioInit();

	glutInit(&argc, argv);          //int *argcp, char **argv
	glutInitDisplayMode(GL_DOUBLE);
	glutInitWindowPosition(640, 0); //int x, y
	glutInitWindowSize(windowSize.x, windowSize.y);   //int width, height
	glutCreateWindow("title");      //const char *title
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutIgnoreKeyRepeat(GL_TRUE);   //�L�[�{�[�h�������֎~
	glutPassiveMotionFunc(passiveMotion);
	reshape(windowSize.x, windowSize.y);
	glutMainLoop();
}