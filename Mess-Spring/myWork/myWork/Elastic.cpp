#include "Elastic.h"
#include <gl/glut.h>
#include <iostream>

Elastic::Elastic()
{
	SpringBuffer = NULL;
	PointBuffer = NULL;
	PointBufSize = SpringBufSize = 0;
	pos = scale = oG = glm::vec3(0);
}
Elastic::~Elastic()
{
	delete[] PointBuffer;
	delete[] SpringBuffer;
}

void Elastic::set1D_rope(
	uint32_t Pointnumber, 
	glm::vec3 pos_, glm::vec3 scale_, 
	float_t springConst_, float_t frictionConst_)
{
	pos = pos_;
	scale = scale_;
	PointBufSize = Pointnumber;
	SpringBufSize = Pointnumber - 1;
	PointBuffer = new Point[PointBufSize];
	SpringBuffer = new Spring[SpringBufSize];

	float length = 3;
	glm::vec3 o(-1.5, 0, 0);
	glm::vec3 vel(0);
	int p = 0;
	PointBuffer[p].set(0.01, vel);
	PointBuffer[p].pos = o;
	PointBuffer[p].pos.x += length * (float)p / (float)Pointnumber;
	PointBuffer[p++].resetMode(POINT_STATIC);
	for (int s = 0; s < SpringBufSize; p++, s++)
	{
		PointBuffer[p].set(0.01, vel);
		PointBuffer[p].pos = o;
		PointBuffer[p].pos.x += length * (float)p / (float)Pointnumber;

		SpringBuffer[s].set(p - 1, p, springConst_, frictionConst_);
		SpringBuffer[s].setOriginLength(PointBuffer);
		std::cout << p << " " << s << std::endl;
		std::cout << PointBuffer[p].pos.x << std::endl;
	}
}

void Elastic::set2D_ring(
	uint32_t Pointnumber, 
	glm::vec3 pos_, glm::vec3 scale_, 
	float_t springConst_, float_t frictionConst_)
{
	pos = pos_;
	scale = scale_;
	PointBufSize = 2 * Pointnumber;
	SpringBufSize = 4 * PointBufSize;
	//SpringBufSize = PointBufSize;
	PointBuffer = new Point[PointBufSize];
	SpringBuffer = new Spring[SpringBufSize];
	uint32_t springPtr = 0;
	glm::vec3 vel(0);
	float_t pi = (float_t)glm::pi<float_t>();
	// 外圈初始化；
	int p = 0;
	PointBuffer[p].set(0.001, vel);
	PointBuffer[p].pos.x = glm::cos(0);
	PointBuffer[p++].pos.y = glm::sin(0);

	for (; p < Pointnumber; p++)
	{
		PointBuffer[p].set(0.001, vel);
		PointBuffer[p].pos.x = glm::cos(2 * pi * p / Pointnumber);
		PointBuffer[p].pos.y = glm::sin(2 * pi * p / Pointnumber);
		SpringBuffer[springPtr].set(p - 1, p, springConst_, frictionConst_);
		SpringBuffer[springPtr++].setOriginLength(PointBuffer);
	}
	SpringBuffer[springPtr].set(Pointnumber-1, 0, springConst_, frictionConst_);
	SpringBuffer[springPtr++].setOriginLength(PointBuffer);
	// /*
	// 内圈初始化，并链接内外圈；
	float_t del = 0.8;
	float_t delO = pi / Pointnumber;
	PointBuffer[p].set(0.001, vel);
	PointBuffer[p].pos.x = glm::cos(0 + delO) * del;
	PointBuffer[p++].pos.y = glm::sin(0 + delO) * del;

	SpringBuffer[springPtr].set(Pointnumber, 0, springConst_, frictionConst_);
	SpringBuffer[springPtr++].setOriginLength(PointBuffer);
	SpringBuffer[springPtr].set(Pointnumber, 1, springConst_, frictionConst_);
	SpringBuffer[springPtr++].setOriginLength(PointBuffer);

	for (uint32_t count = 1; p < PointBufSize; p++, count++)
	{
		PointBuffer[p].set(0.001, vel);
		PointBuffer[p].pos.x = glm::cos(2 * pi * count / Pointnumber + delO) * del;
		PointBuffer[p].pos.y = glm::sin(2 * pi * count / Pointnumber + delO) * del;
		SpringBuffer[springPtr].set(p - 1, p, springConst_, frictionConst_);
		SpringBuffer[springPtr++].setOriginLength(PointBuffer);
		// 连接；
		SpringBuffer[springPtr].set(p, count, springConst_, frictionConst_);
		SpringBuffer[springPtr++].setOriginLength(PointBuffer);
		SpringBuffer[springPtr].set(p, (count + 1)%Pointnumber, springConst_, frictionConst_);
		SpringBuffer[springPtr++].setOriginLength(PointBuffer);
	}
	SpringBuffer[springPtr].set(PointBufSize - 1, Pointnumber, springConst_, frictionConst_);
	SpringBuffer[springPtr++].setOriginLength(PointBuffer);
	SpringBuffer[springPtr].set(PointBufSize - 1, 0, springConst_, frictionConst_);
	SpringBuffer[springPtr++].setOriginLength(PointBuffer);
	//std::cout << p << std::endl;
	//std::cout << springPtr-1 << std::endl;
	PointBufSize = PointBufSize > p ? p : PointBufSize;
	SpringBufSize = SpringBufSize > (--springPtr) ? springPtr : SpringBufSize;
	// */
}

void Elastic::set3D_TriangularPyramid(
	glm::vec3 pos_, glm::vec3 scale_, 
	float_t springConst_, float_t frictionConst_)
{
   pos = pos_;
   scale = scale_;
   PointBufSize = 4;
   PointBuffer = new Point[PointBufSize];
   SpringBufSize = 6;
   SpringBuffer = new Spring[SpringBufSize];
   glm::vec3 vel(0);
   PointBuffer[0].set(0.01, vel);
   PointBuffer[1].set(0.01, vel);
   PointBuffer[2].set(0.01, vel);
   PointBuffer[3].set(0.01, vel);

   PointBuffer[0].pos.x=1;
   PointBuffer[1].pos.x=-1;
   PointBuffer[2].pos.x=0;
   PointBuffer[3].pos.x = 0;

   PointBuffer[0].pos.y=0;
   PointBuffer[1].pos.y=0;
   PointBuffer[2].pos.y = 1.7320508;
   PointBuffer[3].pos.y = 0.5773502;

   PointBuffer[3].pos.z = 1.7320508;

   SpringBuffer[0].set(0, 1, springConst_, frictionConst_);
   SpringBuffer[0].setOriginLength(PointBuffer);

   SpringBuffer[1].set(1, 2, springConst_, frictionConst_);
   SpringBuffer[1].setOriginLength(PointBuffer);

   SpringBuffer[2].set(2, 0, springConst_, frictionConst_);
   SpringBuffer[2].setOriginLength(PointBuffer);

   SpringBuffer[3].set(3, 1, springConst_, frictionConst_);
   SpringBuffer[3].setOriginLength(PointBuffer);

   SpringBuffer[4].set(3, 2, springConst_, frictionConst_);
   SpringBuffer[4].setOriginLength(PointBuffer);

   SpringBuffer[5].set(3, 0, springConst_, frictionConst_);
   SpringBuffer[5].setOriginLength(PointBuffer);
}

void Elastic::addObject(hitable* obj)
{
	object.addObject(obj);
}

void Elastic::frame()
{
	for (uint32_t i = 0; i < SpringBufSize; i++)
		SpringBuffer[i].simulateForce(PointBuffer, oG);
	for (uint32_t i = 0; i < PointBufSize; i++)
	{
		object.hit(PointBuffer[i], pos, -delteT*3, delteT);
		PointBuffer[i].simulate_AND_clearForce(delteT);
	}
}

void Elastic::drawSpring()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(pos.x, pos.y, pos.z);
	glScalef(scale.x, scale.y, scale.z);
	glColor3f(1, 1, 1);

	
	for (uint32_t i = 0; i < SpringBufSize; i++)
	{
		
		glBegin(GL_LINES);
		Point* get = &PointBuffer[SpringBuffer[i].p1Index];
		//std::cout << (*get).pos.y << std::endl;
		glVertex3f((*get).pos.x, (*get).pos.y, (*get).pos.z);
		get = &PointBuffer[SpringBuffer[i].p2Index];
		glVertex3f((*get).pos.x, (*get).pos.y, (*get).pos.z);
		glEnd();
	}
	
}