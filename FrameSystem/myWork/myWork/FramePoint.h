#pragma once

#include <stdlib.h>
#include <time.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class FramePoint
{
public:
#define POINT_STATIC	0x00
#define POINT_DYNAMIC	0x01
	int16_t		state;
	glm::vec3	pos;	// λ�ã�
	glm::vec3	vel;	// �ٶȣ�
	//glm::vec3	acv;	// ���ٶȣ�
	float		life;	// ������
	float_t		size;	// ��С��
	float_t		fade;	// ��ʧ�ٶȣ�

	int16_t(FramePoint::* simulate)(const float_t dt);	// ����ָ�룻
	FramePoint();
	~FramePoint() {}

	void changeVel(glm::vec3 vel_);
	void updata();
private:
	float_t		orgLife;// ԭʼ������
	glm::vec3	orgPos;	// ԭʼλ�ã�
	glm::vec3	orgVel;	// ԭʼ�ٶȣ�
	inline void resetPoint();
	int16_t simulate_normal(const float_t dt);
	int16_t simulate_change(const float_t dt);
};

