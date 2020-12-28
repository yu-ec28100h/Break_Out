#include "Ball.h"
#include "glut.h"

/*void Ball::updateAfterImage()
{
	for (int i = BALL_AFTER_IMAGE_MAX - 2; i >= 0; i--)
	{
		m_afterImagePositions[i + 1] = m_afterImagePositions[i];
	}
	m_afterImagePositions[0] = m_position;
}*/

void Ball::update()
{
	//updateAfterImage();
	
	m_lastPosition = m_position;

	m_speed.y += m_gravity;
	m_position += m_speed;
}

/*void Ball::drawAfterImage()
{
	for (int i = BALL_AFTER_IMAGE_MAX - 1; i > 0; i--)
	{
		{
			GLubyte c = 255 - i;
			glColor3ub(c, c, c);
		}
		glPushMatrix();
		{
			glTranslatef(m_afterImagePositions[i].x, m_afterImagePositions[i].y, 0);
			glScalef(m_radius, m_radius, 0);
			glutSolidSphere(
				1,
				16,
				16
			);
		}
		glPopMatrix();
	}
}*/

void Ball::draw()
{
	//drawAfterImage();

	glPushMatrix();
	{
		glTranslatef(m_position.x, m_position.y, 0);
		glScalef(m_radius, m_radius, 0);
		glutSolidSphere(
			1,
			16,
			16
		);
	}
	glPopMatrix();
}

void Ball::reset()
{
	m_lastPosition = m_position;
	/*for (int i = 0; i < BALL_AFTER_IMAGE_MAX; i++)
	{
		m_afterImagePositions[i] = m_position;
	}*/
	m_speed = {};
}