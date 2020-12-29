#pragma once
#include "glm/glm.hpp"

using namespace glm;

struct Ball
{
	float m_radius;
	vec2 m_lastPosition;
	vec2 m_position;
	vec2 m_speed;
	float m_power;

	void update();
	
	void draw();
	void reset();
};