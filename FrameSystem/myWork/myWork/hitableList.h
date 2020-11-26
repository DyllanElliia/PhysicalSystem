#pragma once

#include "hitable.h"

class hitableList
{
public:
	hitableList();
	~hitableList();

	bool addObject(hitable* obj_);
	void hit(FramePoint& point, glm::vec3 offset, float near, float far);
private:
	struct Object
	{
		hitable* obj;
		struct Object* next;
	};

	Object* objectList;
};

