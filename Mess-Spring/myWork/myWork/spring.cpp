#include "spring.h"
#include <iostream>
void Spring::set(int32_t p1Index_, int32_t p2Index_, const float_t springConst_, const float_t frictionConst_)
{
	p1Index = p1Index_;
	p2Index = p2Index_;
	springConst = springConst_;
	frictionConst = frictionConst_;
}

void Spring::simulateForce(Point* PointBuffer, const glm::vec3 oG)
{
	glm::vec3 p1_p2 = PointBuffer[p1Index].pos - PointBuffer[p2Index].pos;
	//std::cout << p1_p2.x << std::endl;
	float_t p1_p2_len = float_t(glm::length(p1_p2));
	//std::cout << p1_p2_len << std::endl;
	glm::vec3 force_1(0);
	if (p1_p2_len != 0)
	{
		glm::vec3 e = p1_p2 / p1_p2_len;
		force_1 += e * (p1_p2_len - originLength) * (-springConst);
		force_1 += -e * ((PointBuffer[p1Index].vel - PointBuffer[p2Index].vel) * e) * frictionConst;
	}
	//std::cout << force_1.x << " " << force_1.y << " " << force_1.z << std::endl;
	//system("pause");
	PointBuffer[p1Index].addForce(PointBuffer[p1Index].mess * oG + force_1);
	PointBuffer[p2Index].addForce(PointBuffer[p2Index].mess * oG - force_1);
}