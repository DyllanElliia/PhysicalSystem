#pragma once
#include "hitable.h"
#include <iostream>

class ground : public hitable
{
public:
	glm::vec3 center;
	glm::vec3 normal;
	ground(glm::vec3 center_, glm::vec3 normal_)
		:center(center_), normal(glm::normalize(normal_)) {}
	virtual bool hitJudge(	const glm::vec3 pos,
							const glm::vec3 V,
							glm::vec3& normal,
							const float& near,
							const float& far,
							float& t) const;
};

