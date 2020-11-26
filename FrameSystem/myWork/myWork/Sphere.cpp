#include "Sphere.h"

#include <iostream>

bool Sphere::hitJudge(  const glm::vec3 pos, 
                        const glm::vec3 pVnormal, 
                        glm::vec3 normal,
                        const float& near, 
                        const float& far,
                        float& t) const
{
    //std::cout << "judge" << std::endl;
    //Բ��  (p(t)-center)(p(t)-center) = R*R
    //չ��  (origin + t*direction - center)(origin + t*direction - center) = R*R
    //չ��  t*t*direction*direction + 2*t*direction*(oringin-center) + (oringin-center)*(oringin-center) - R*R = 0
    //a = direction*direction;  b = direction*(oringin-center); c = (oringin-center)*(oringin-center) - R*R
    //Ѱ���Ƿ��� t ��ʹ�����ӳ�����Բ���ڽ��㣻
    glm::vec3 oc = pos - center;
    float a = glm::dot(pVnormal, pVnormal);
    float b = glm::dot(oc, pVnormal);
    float c = glm::dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;

    if (discriminant > 0)
    {
        float temp = (-b - sqrt(discriminant)) / a;
        if (temp < far && temp > near)
        {
            // �������е�tС��˵������ײ�����ø��磻
            if (temp < t)
            {
                t = temp;
                normal = glm::normalize(pos + temp * pVnormal - center);
                return true;
            }
            return false;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < far && temp > near)
        {
            if (temp < t)
            {
                t = temp;
                normal = glm::normalize(pos + temp * pVnormal - center);
                return true;
            }
            return false;
        }
    }
    return false;
}