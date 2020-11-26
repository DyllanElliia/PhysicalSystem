#include "hitableList.h"

hitableList::hitableList()
{
	objectList = NULL;
}

hitableList::~hitableList()
{
	if (objectList == NULL)
		return;
	Object* ptr, * save;
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

void hitableList::hit(Point& point, glm::vec3 offset, float near, float far)
{
	float temp = far;
	Object* ptr = objectList;
	glm::vec3 pos = point.pos + offset;
	glm::vec3 V = point.vel;
	glm::vec3 normal(0);
	
	while (ptr)
	{
		ptr->obj->hitJudge(pos, V, normal, near, far, temp);
		ptr = ptr->next;
	}

	if (temp == far)
		return;
	//point.vel.y = 0.1;
	//point.force.y = -point.force.y;
	//return;
	//std::cout << "hit" << std::endl;
	glm::vec3 nV = glm::dot(normal, V) * normal;
	//std::cout << nV.x<<" "<< nV.y << " " << nV.z << std::endl;
	float nf = glm::dot(normal, point.force);
	//std::cout << nf << std::endl;
	//std::cout << glm::length(point.force) << std::endl;
	glm::vec3 F = nf * normal;
	//std::cout << point.force.x << " " << point.force.y << " " << point.force.z << std::endl;
	//std::cout << -F.x << " " << -F.y << " " << -F.z << std::endl;
	point.addForce(-F*(float)1.1);
	//std::cout << point.force.x << " " << point.force.y << " " << point.force.z << std::endl;
#define la float(2.1)
	point.addForce(la * point.mess * (-nV) / far);
	
}