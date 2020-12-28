#pragma once

#include "Ball.h"
#include "glm/glm.hpp"

using namespace glm;

struct Paddle
{
	float m_width;	//paddle‚Ì‰¡•
	vec2 m_position;

	bool intersectBall(Ball const& _ball);
	void draw();
};