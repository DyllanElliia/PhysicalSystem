#pragma once

#include "spring.h"
#include "hitableList.h"

class Elastic
{
private:
	Point* PointBuffer;
	uint32_t PointBufSize;
	Spring* SpringBuffer;
	uint32_t SpringBufSize;

	glm::vec3 pos;
	glm::vec3 scale;
	glm::vec3 oG;

	float_t delteT = 0.01;

	hitableList object;

public:
	Elastic();
	~Elastic();
	void set1D_rope(uint32_t Pointnumber, glm::vec3 pos_, glm::vec3 scale_, float_t springConst_, float_t frictionConst_);
	void set2D_ring(uint32_t Pointnumber, glm::vec3 pos_, glm::vec3 scale_, float_t springConst_, float_t frictionConst_);
	void set3D_TriangularPyramid(glm::vec3 pos_, glm::vec3 scale_, float_t springConst_, float_t frictionConst_);
	void setOutForce(glm::vec3 o) { oG = o; }
	void frame();
	void drawSpring();
	void addObject(hitable* obj);
};

