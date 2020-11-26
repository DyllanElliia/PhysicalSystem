#include "FramePoint.h"

#include <iostream>

FramePoint::FramePoint()
{
	pos = glm::vec3(0);
	vel = glm::vec3(0);
	//acv = glm::vec3(0);
	orgVel = orgPos = glm::vec3(0);
	life = size = orgLife = 0;
	state = POINT_STATIC;
	simulate = &FramePoint::simulate_normal;
}

void FramePoint::changeVel(glm::vec3 vel_)
{
	vel = vel_;
	simulate = &FramePoint::simulate_change;
}

int16_t FramePoint::simulate_normal(const float_t dt)
{
	if (state == POINT_STATIC)
		state = POINT_DYNAMIC;

	pos += vel * dt;
	life -= fade * dt;
	if (life < 0)
		resetPoint();

	return state;
}

int16_t FramePoint::simulate_change(const float_t dt)
{
	int16_t save = simulate_normal(dt);

	vel = orgVel;
	simulate = &FramePoint::simulate_normal;

	return save;
}

inline void FramePoint::resetPoint()
{
	//std::cout << "reset" << std::endl;
	state = POINT_STATIC;
	pos = orgPos;
	life = orgLife*rand()/RAND_MAX;
}

void FramePoint::updata()
{
	orgPos = pos;
	orgLife = life;
}