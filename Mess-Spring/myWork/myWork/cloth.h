#pragma once

#include "spring.h"

class cloth
{
	Point*	PointBuffer;	// Á£×Ó»º³å³Ø£»
	uint32_t pointBufSize;
	Spring* SpringBuffer;	// µ¯»É»º³å³Ø£»
	uint32_t springBufSize;

	uint16_t xSize;
	uint16_t ySize;
	glm::vec3 posOffset;
	glm::vec3 scale;
};

