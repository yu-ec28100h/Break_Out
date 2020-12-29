#include <stdio.h>
#include <stdlib.h>

#include "Font.h"
#include "Audio.h"
#include "Rect.h"
#include "Ball.h"
#include "Paddle.h"

#include "glut.h"
#include "glm/glm.hpp"

#define PADDLE_DEFFAULT_WIDTH 48

#define BLOCK_COLUMN_MAX 14
#define BLOCK_ROW_MAX 8

#define BLOCK_HEIGHT 12
#define BALL_X_SPEED_MAX 2.f
#define FONT_HEIGHT 32
#define FONT_WEIGHT 4
#define SE_WAIT_MAX 6

enum {
	LEVEL_DEFAULT,
	LEVEL_HIT_4,
	LEVEL_HIT_12,
	LEVEL_HIT_ORANGE,
	LEVEL_HIT_RED,
	LEVEL_MAX
};

using namespace glm;

ivec2 windowSize = { 800, 600 };

bool keys[256];

Rect field;
Ball ball = { 8 };
Paddle paddle = { PADDLE_DEFFAULT_WIDTH };
Rect blocks[BLOCK_ROW_MAX][BLOCK_COLUMN_MAX];

int turn = 1;
int score;

int seCout;
int seWait;

int level;
float powerTbl[] = {
	3,	//LEVEL_DEFAULT,
	4,	//LEVEL_HIT_4,
	5,	//LEVEL_HIT_12,
	6,	//LEVEL_HIT_ORANGE,
	7	//LEVEL_HIT_RED,
};
int getBlockCount()
{
	int n = 0;

	for (int i = 0; i < BLOCK_ROW_MAX; i++)
	{
		for (int j = 0; j < BLOCK_COLUMN_MAX; j++)
		{
			if (!blocks[i][j].isDead) n++;
		}
	}

	return n;
}

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
	{//blockの描画
		unsigned char colors[][3] = {
			{0xff, 0x00, 0x00},
			{0xff, 0x80, 0x00},
			{0x00, 0xff, 0x00},
			{0xff, 0xff, 0x00},
		};

		for (int i = 0; i < BLOCK_ROW_MAX; i++)
		{
			for (int j = 0; j < BLOCK_COLUMN_MAX; j++)
			{
				if (blocks[i][j].isDead) continue;

				int colorIndx = i / 2;
				unsigned char *color = colors[colorIndx];
				glColor3ub(color[0], color[1], color[2]);
				//blocks[i][j].draw();
				glRectfv((GLfloat *)&(blocks[i][j].m_position + vec2(1, 1)),
					(GLfloat *)&(blocks[i][j].m_position + blocks[i][j].m_size - vec2(1, 1)));
			}
		}
	}

	glColor3ub(0xff, 0xff, 0xff);
	ball.draw();

	glColor3ub(0x00, 0xff, 0xff);
	paddle.draw();

	glColor3ub(0xff, 0xff, 0xff);
	{
		float x = field.m_position.x, y = field.m_position.y;
		fontBegin();
		fontSetHeight(FONT_HEIGHT);
		fontSetWeight(FONT_WEIGHT);
		fontSetPosition(x, y);
		fontDraw("1");
		fontSetPosition(x + field.m_size.x / 2, y);
		fontDraw("%d\n", turn);
		x += fontGetWidth();
		fontSetPosition(x, y);
		{
			static unsigned frame;
			if((++frame/15) % 2 == 0)
				fontDraw("%03d", score);
		}
		fontSetPosition(x + field.m_size.x / 2, y);
		fontDraw("000");
		y += fontGetHeight() + fontGetWeight();

		fontSetHeight(16);
		fontSetWeight(2);
		x = field.m_position.x;
		y += BLOCK_ROW_MAX * BLOCK_HEIGHT;
		fontSetPosition(x, y);
		fontDraw("Level: %d\n", level);
		fontEnd();
	}

	glutSwapBuffers();
}

void idle(void)
{
	if (seCout > 0)
	{
		if (--seWait < 0)
		{
			seCout--;
			seWait = SE_WAIT_MAX;

			audioStop();
			audioFreq(440 / 2);
			audioPlay();
		}
	}
	audioUpdate();

	
	ball.m_power = powerTbl[level];
	ball.update();

	if((ball.m_position.x < field.m_position.x)
		||(ball.m_position.x >= field.m_position.x + field.m_size.x))
	{
		audioStop();
		audioFreq(440);
		audioPlay();

		ball.m_position = ball.m_lastPosition;
		ball.m_speed.x *= -1;
	}
	if((ball.m_position.y < field.m_position.y)
		||(ball.m_position.y >= field.m_position.y + field.m_size.y))
	{
		audioStop();
		audioFreq(440);
		audioPlay();

		ball.m_position = ball.m_lastPosition;
		ball.m_speed.y *= -1;
	}
	
	if (paddle.intersectBall(ball))
	{
		audioStop();
		audioFreq(440 * 2);
		audioPlay();

		ball.m_position = ball.m_lastPosition;
		ball.m_speed.y *= -1;

		float paddleCenterX = paddle.m_position.x + paddle.m_width / 2;
		float sub = ball.m_position.x - paddleCenterX;
		float subMax = paddle.m_width / 2;
		ball.m_speed.x = sub / subMax * BALL_X_SPEED_MAX;
	}

	bool flag = false;
	for (int i = 0; i < BLOCK_ROW_MAX; i++)
	{
		for (int j = 0; j < BLOCK_COLUMN_MAX; j++)
		{
			if (blocks[i][j].isDead) continue;

			if (blocks[i][j].intersect(ball.m_position))
			{
				blocks[i][j].isDead = true;
				ball.m_speed.y *= -1;

				int colorIdx = 3 - (i / 2);
				printf("%d\n", colorIdx);
				int s = 1 + 2 * colorIdx;
				
				audioStop();
				audioFreq(440/2);
				audioPlay();

				seCout += s - 1;
				seWait = SE_WAIT_MAX;

				score += s;

				{
					int n = getBlockCount();
					int blockCountMax = BLOCK_COLUMN_MAX * BLOCK_ROW_MAX;
					if ((n <= blockCountMax - 4) && (level < LEVEL_HIT_4))	//４回ヒットしたら加速
						level = LEVEL_HIT_4;
					
					if ((n <= blockCountMax - 12) && (level < LEVEL_HIT_12))	//12回ヒットしたら加速
						level = LEVEL_HIT_12;
					
					if ((colorIdx == 2) && (level < LEVEL_HIT_ORANGE))	//オレンジブロックにヒットしたら加速
						level = LEVEL_HIT_ORANGE;
					if ((colorIdx == 3) && (level < LEVEL_HIT_RED))	//レッドブロックにヒットしたら加速
						level = LEVEL_HIT_RED;
				}

				flag = true;
				break;
			}
		}
		if (flag) break;
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
	ball.m_speed = vec2(1, 1);
	ball.m_power = 1;

	paddle.m_position = vec2(
		field.m_position.x + field.m_size.x / 2,
		field.m_position.y + field.m_size.y - 48);

	vec2 blockSize = vec2(field.m_size.x / BLOCK_COLUMN_MAX, 12);
	float y = field.m_position.y + (FONT_HEIGHT + FONT_WEIGHT)*2;
	for (int i = 0; i < BLOCK_ROW_MAX; i++)
	{
		for (int j = 0; j < BLOCK_COLUMN_MAX; j++)
		{
			blocks[i][j].m_position = vec2(
				field.m_position.x + field.m_size.x *j / BLOCK_COLUMN_MAX,
				y + i*blockSize.y);
			blocks[i][j].m_size = blockSize;
		}
	}
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
	paddle.m_position.x = x - paddle.m_width / 2;	//paddleの中心
	paddle.m_position.x = max(paddle.m_position.x, field.m_position.x);
	paddle.m_position.x = min(paddle.m_position.x, field.m_position.x + field.m_size.x - paddle.m_width);
}

int main(int argc, char *argv[])
{
	audioInit();
	audioWaveform(AUDIO_WAVEFORM_PULSE_50);
	audioDecay(.9f);

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
	reshape(windowSize.x, windowSize.y);
	glutMainLoop();
}