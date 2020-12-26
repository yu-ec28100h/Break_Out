#pragma once

#include "Ball.h"
#include "glm/glm.hpp"

using namespace glm;

struct Paddle
{
	vec2 m_position;
	float m_height;	//paddle�̍���

	bool intersectBall(Ball const& _ball);
	void draw();
};