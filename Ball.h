#pragma once
#include "glm/glm.hpp"

using namespace glm;

struct Ball
{
	vec2 m_lastPosition;
	vec2 m_position;
	vec2 m_speed;

	void update();
	void draw();
};