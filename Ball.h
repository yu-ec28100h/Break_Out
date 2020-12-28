#pragma once
#include "glm/glm.hpp"

using namespace glm;

//#define BALL_AFTER_IMAGE_MAX 255

struct Ball
{
	float m_radius;
	vec2 m_lastPosition;
	vec2 m_position;
	vec2 m_speed;
	float m_gravity;

	//vec2 m_afterImagePositions[BALL_AFTER_IMAGE_MAX];

	//void updateAfterImage();
	void update();
	//void drawAfterImage();
	void draw();
	void reset();
};