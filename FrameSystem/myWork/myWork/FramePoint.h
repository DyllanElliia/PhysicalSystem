#pragma once

#include <stdlib.h>
#include <time.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class FramePoint
{
public:
#define POINT_STATIC	0x00
#define POINT_DYNAMIC	0x01
	int16_t		state;
	glm::vec3	pos;	// 位置；
	glm::vec3	vel;	// 速度；
	//glm::vec3	acv;	// 加速度；
	float		life;	// 生命；
	float_t		size;	// 大小；
	float_t		fade;	// 消失速度；

	int16_t(FramePoint::* simulate)(const float_t dt);	// 函数指针；
	FramePoint();
	~FramePoint() {}

	void changeVel(glm::vec3 vel_);
	void updata();
private:
	float_t		orgLife;// 原始生命；
	glm::vec3	orgPos;	// 原始位置；
	glm::vec3	orgVel;	// 原始速度；
	inline void resetPoint();
	int16_t simulate_normal(const float_t dt);
	int16_t simulate_change(const float_t dt);
};

