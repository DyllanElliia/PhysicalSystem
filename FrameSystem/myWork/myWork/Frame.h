#pragma once

#include "FramePoint.h"
#include "hitableList.h"
#include <gl/glut.h>

class Frame
{
public:
	Frame()
	{
		FrameBuffer = NULL;
		life = 2.0;
	}
	~Frame()
	{
		delete[] FrameBuffer;
		delete[] dynamicQueue.LoopQueue;
		delete[] staticQueue.LoopQueue;
	}
	void set(int32_t PointNumber_, glm::vec3 scale_, glm::vec3 origin_, float_t acvY_, float_t dive_);
	void setLife(float_t life_)
	{
		life = life_;
	}
	void create() { create_(); }
	void draw(unsigned int texture_f);
	void simulate();
	void addObject(hitable* obj_);
private:
	struct LoopQueue
	{
		int32_t*	LoopQueue;
		int32_t		QueueSize;
		int32_t		head;
		int32_t		tail;
		inline void create(int32_t size);
		inline int32_t getTail();
		inline void pushHead(int32_t data);
		inline bool isEmpty();
	};

	FramePoint* FrameBuffer;
	int32_t		PointNumber;
	glm::vec3	scale;		// ���ţ�
	glm::vec3	origin;		// ԭ�㣻
	float_t		acvY;		// ��ʼ���ٶȣ�
	float_t		dive;		// ��ɢ�̶ȣ�
	float_t		life;		// ������
	LoopQueue	staticQueue;
	LoopQueue	dynamicQueue;
	hitableList ObjList;

	void create_();
	void wakeUp(int32_t number);
};

