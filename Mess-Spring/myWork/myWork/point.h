#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

class Point
{
public:
	float_t			mess;	// 质量；
	glm::vec3		pos;	// 位置；
	glm::vec3		vel;	// 速度；
	glm::vec3		force;	// 受力；

#define POINT_STATIC	0x00
#define POINT_DYNAMIC	0x01
	Point()
	{
		mess = 1;
		vel = pos = glm::vec3(0);
		resetForce();
		this->simulate_ = &Point::dynamic_;
	}
	Point(float_t mess_, glm::vec3 pos_, glm::vec3 vel_)
		:mess(mess_), pos(pos_), vel(vel_)
	{
		resetForce();
		this->simulate_ = &Point::dynamic_;
	}
	~Point(){}
	inline void set(float_t mess_, glm::vec3 vel_)
	{
		mess = mess_;
		vel = vel_;
	}
	// 重置受力模式；
	inline void resetMode(uint16_t mode)
	{
		switch (mode)
		{
		case POINT_STATIC:
			this->simulate_ = &Point::static_;
			break;
		case POINT_DYNAMIC:
			this->simulate_ = &Point::dynamic_;
			break;
		default:
			this->simulate_ = &Point::dynamic_;
			break;
		}
	}

	inline void addForce(const glm::vec3 force_)
	{ force += force_;}

	inline void resetForce()
	{ this->force = glm::vec3(0);}

	inline void simulate_AND_clearForce(const float_t dt)
	{
		((*this).*((*this).simulate_))(dt);
		resetForce();
	}
private:
#define FORCE_LIMIT_MAX 1.0
#define FORCE_LIMIT_MIN 2e-4
	void (Point::* simulate_)(const float_t dt);	// 函数指针；

	void dynamic_(const float_t dt)
	{
		float l = glm::length(force);
		if (l < FORCE_LIMIT_MIN)
			resetForce();
		else if (l > FORCE_LIMIT_MAX)
			force *= FORCE_LIMIT_MAX / l;
		vel += (force / mess) * dt;
		pos += vel * dt;
	}
	void static_(const float_t dt) {}
};

