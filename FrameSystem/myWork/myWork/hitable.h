#pragma once

#include "FramePoint.h"
#include <iostream>

class hitable
{
public:
	glm::vec3 center;
	virtual bool hitJudge(	const glm::vec3 pos, 
							const glm::vec3 pVnormal, 
							glm::vec3 normal,
							const float &near, 
							const float &far, 
							float& t)
		const { return false; }
};

