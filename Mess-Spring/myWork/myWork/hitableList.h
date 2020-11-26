#pragma once
#include "point.h"
#include "hitable.h"

class hitableList
{
public:
	hitableList();
	~hitableList();

	bool addObject(hitable* obj_);
	void hit(Point& point, glm::vec3 offset, float near, float far);

private:
	struct Object
	{
		hitable* obj;
		struct Object* next;
	};

	Object* objectList;
};

