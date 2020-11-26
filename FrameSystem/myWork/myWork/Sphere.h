#pragma once

#include "hitable.h"

class Sphere:public hitable
{
public:
	glm::vec3 center;
	float radius;
	Sphere(glm::vec3 center_, float radius_)
		:center(center_), radius(radius_) {};
	virtual bool hitJudge(	const glm::vec3 pos, 
							const glm::vec3 pVnormal, 
							glm::vec3 normal,
							const float& near, 
							const float& far,
							float& t) const;
};

