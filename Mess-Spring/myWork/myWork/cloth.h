#pragma once

#include "spring.h"

class cloth
{
	Point*	PointBuffer;	// ���ӻ���أ�
	uint32_t pointBufSize;
	Spring* SpringBuffer;	// ���ɻ���أ�
	uint32_t springBufSize;

	uint16_t xSize;
	uint16_t ySize;
	glm::vec3 posOffset;
	glm::vec3 scale;
};

