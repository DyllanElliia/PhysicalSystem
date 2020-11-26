#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class hitable
{
public:
	virtual bool hitJudge(	const glm::vec3 pos,
							const glm::vec3 V,
							glm::vec3& normal,
							const float& near,
							const float& far,
							float& t)
		const { return false;}
};

