#pragma once

#include "point.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Spring
{
public:
	int32_t p1Index;
	int32_t p2Index;

	float_t	originLength;	// 弹簧的静态长度；
	float_t	springConst;	// 弹簧静态系数；
	float_t frictionConst;	// 阻尼系数；

	Spring()
	{
		springConst = frictionConst = 1000000;
		p1Index = p2Index = 0;
		originLength = 1;
	}
	Spring(float_t	springConst_, float_t frictionConst_)
		:
		springConst(springConst_),
		frictionConst(frictionConst_)
	{
		p1Index = p2Index = 0;
		originLength = 1;
	}
	Spring(int32_t p1Index_, int32_t p2Index_, float_t	springConst_, float_t frictionConst_)
		:	p1Index(p1Index_), p2Index(p2Index_),
			springConst(springConst_),
			frictionConst(frictionConst_)
	{
		originLength = 1;
	}
	void set(int32_t p1Index_, int32_t p2Index_, const float_t springConst_, const float_t frictionConst_);
	inline void setOriginLength(Point* PointBuffer)
	{
		//std::cout << p1Index<<" "<<p2Index << std::endl;
		originLength = glm::length(PointBuffer[p1Index].pos - PointBuffer[p2Index].pos);
		//originLength /= 0.8;
		//std::cout << originLength << std::endl;
	}
	void simulateForce(Point* PointBuffer, const glm::vec3 oForce);
};

