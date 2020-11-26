#include "ground.h"


bool ground::hitJudge(	const glm::vec3 pos,
						const glm::vec3 V,
						glm::vec3& normal,
						const float& near,
						const float& far,
						float& t) const
{
	float temp = glm::dot(this->normal, (this->center - pos))
				/ glm::dot(this->normal, V);
	
	if(temp<far&&temp>near)
		if (temp < t)
		{
			//std::cout << temp << std::endl;
			t = temp;
			normal = this->normal;
			return true;
		}
	return false;
}