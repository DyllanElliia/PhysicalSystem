#include "hitableList.h"
#include <iostream>
hitableList::hitableList()
{
	objectList = NULL;
}

hitableList::~hitableList()
{
	if (objectList == NULL)
		return;
	Object* ptr, *save;
	ptr = objectList;
	save = ptr->next;
	do {
		delete[]ptr->obj;
		delete[]ptr;
		ptr = save;
		save = ptr->next;
	} while (save);
}

bool hitableList::addObject(hitable* obj_)
{
	std::cout << "add" << std::endl;
	objectList = new Object{ obj_,objectList };
	return true;
}

void hitableList::hit(FramePoint& point, glm::vec3 offset, float near, float far)
{
	//std::cout << "runlist" << std::endl;
	float temp = far;
	Object* ptr = objectList;
	glm::vec3 pos = point.pos + offset;
	glm::vec3 pVnormal = glm::normalize(point.vel);
	glm::vec3 normal(0);
	while (ptr)
	{
		//std::cout << "run" << std::endl;
		ptr->obj->hitJudge(pos, pVnormal, normal, near, far, temp);
		ptr = ptr->next;
	}
	if (temp == far)
		return;
	//std::cout << "count" << std::endl;
	point.pos = point.pos + pVnormal * temp;
	point.changeVel(point.vel + 2 * glm::dot(pVnormal, normal)*glm::length(point.vel) * normal);
}