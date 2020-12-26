#include <stdio.h>
#include <stdlib.h>

#include "Font.h"
#include "Audio.h"

#include "glut.h"
#include "glm/glm.hpp"

using namespace glm;

ivec2 windowSize = { 800, 600 };

bool keys[256];

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);    //GLenum mode
	glLoadIdentity();
	gluOrtho2D(0, windowSize.x, windowSize.y, 0);    //GLdouble left, right, bottom, top

	glMatrixMode(GL_MODELVIEW);    //GLenum mode
	glLoadIdentity();
	
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
	glutPostRedisplay();
}

void reshape(int width, int height)
{
	printf("reshape: width:%d height:%d\n", width, height);
	glViewport(0, 0, width, height);    //GLint x, y / GLsizei width, height
	windowSize = ivec2(width, height);
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
	glutIgnoreKeyRepeat(GL_TRUE);   //キーボード長押し禁止
	glutPassiveMotionFunc(passiveMotion);

	glutMainLoop();
}