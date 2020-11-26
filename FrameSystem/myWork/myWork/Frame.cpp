#include "Frame.h"
#include "Sphere.h"

#include <random>
#include <chrono>

#include <iostream>

inline void Frame::LoopQueue::create(int32_t size)
{
	QueueSize = size + 1;
	LoopQueue = new int32_t[QueueSize];
	head = tail = 0;
}

inline int32_t Frame::LoopQueue::getTail()
{
	int32_t data = LoopQueue[tail];
	tail = (tail + 1) % QueueSize;
	return data;
}
inline void Frame::LoopQueue::pushHead(int32_t data)
{
	LoopQueue[head] = data;
	head = (head + 1) % QueueSize;
}

inline bool Frame::LoopQueue::isEmpty()
{
	if (head == tail)
		return true;
	return false;
}

void Frame::addObject(hitable* obj_)
{
	ObjList.addObject(obj_);
}

void Frame::set(int32_t PointNumber_, glm::vec3 scale_, glm::vec3 origin_, float_t acvY_, float_t dive_)
{
	this->PointNumber = PointNumber_;
	this->scale = scale_;
	this->origin = origin_;
	this->acvY = acvY_;
	this->dive = dive_;
	this->life = 1;
	//create_();
}

void Frame::create_()
{
	// 从epoch（1970年1月1日00:00:00 UTC）开始经过的纳秒数，unsigned类型会截断这个值；
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	// 第一个参数为高斯分布的平均值，第二个参数为标准差；
	std::normal_distribution<double> distribution(0.0, 1.0);

	if (FrameBuffer != NULL)
		delete[] FrameBuffer;
	FrameBuffer = new FramePoint[PointNumber];
	staticQueue.create(PointNumber);
	dynamicQueue.create(PointNumber);

	for (int i = 0; i < PointNumber; i++)
	{
		FramePoint* point = &(FrameBuffer[i]);
		// 利用高斯分布得到粒子位置；
#define randP glm::vec3(distribution(generator), 0, distribution(generator))
		point->pos = randP; // 高斯分布得位置；
		// 利用高斯分布获得一带有随机的速度，dive为粒子散度；
#define randV glm::vec3(distribution(generator), distribution(generator), distribution(generator))*dive
		point->vel = glm::vec3(0, acvY, 0) + randV;

		point->life = life;
		// 随机粒子大小，让每个粒子的大小不一样，更好地模拟真实火焰。
		do {
			point->size = distribution(generator)/3;
			if (point->size < 0)
				point->size = -point->size;
		} while (point->size < 1);
		point->fade = 1;
		point->updata();

		staticQueue.pushHead(i);
	}
	//std::cout << "finish111" << std::endl;
}

void Frame::draw(unsigned int texture_f)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(origin.x, origin.y, origin.z);
	glScalef(scale.x, scale.y, scale.z);

	glBindTexture(GL_TEXTURE_2D, texture_f);
	
	for (int i = 0; i < PointNumber; i++)
	{
		if (FrameBuffer[i].state == POINT_STATIC)
			continue;
		//std::cout << "draw" << std::endl;
		FramePoint *point = &(FrameBuffer[i]);
		glColor4f(1, 1, 1, point->life/(float)life);

		glBegin(GL_TRIANGLE_STRIP);

		glTexCoord2d(1, 1); glVertex3f(point->pos.x + point->size, point->pos.y + point->size, point->pos.z);
		glTexCoord2d(0, 1); glVertex3f(point->pos.x - point->size, point->pos.y + point->size, point->pos.z);
		glTexCoord2d(1, 0); glVertex3f(point->pos.x + point->size, point->pos.y - point->size, point->pos.z);
		glTexCoord2d(0, 0); glVertex3f(point->pos.x - point->size, point->pos.y - point->size, point->pos.z);

		glEnd();
	}
}

void Frame::wakeUp(int32_t number)
{
	while (number > 0)
	{
		if (staticQueue.isEmpty())
			return;
		int32_t get = staticQueue.getTail();
		dynamicQueue.pushHead(get);
		number--;
	}
}

void Frame::simulate()
{
	// 唤醒粒子；
	wakeUp(3);
	// 遍历动态粒子队列中的粒子；
	for (int i = dynamicQueue.tail; i != dynamicQueue.head; i = (i+1)%dynamicQueue.QueueSize)
	{
		if (dynamicQueue.isEmpty())
			return;
		// 粒子位移计算，并检测状态是否需要更新；
		FramePoint *fp = &(FrameBuffer[dynamicQueue.LoopQueue[i]]);
		if (((*fp).*((*fp).simulate))(0.06) == POINT_STATIC)
		{
			staticQueue.pushHead(dynamicQueue.LoopQueue[i]);
			dynamicQueue.LoopQueue[i] = dynamicQueue.getTail();
			continue;
		}
		// 碰撞检测；
		ObjList.hit(*fp, origin,0, 0.06 * glm::length(fp->vel));
	}
}