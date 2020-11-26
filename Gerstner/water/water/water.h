#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float amplitude = 0.01;						// 振幅		A
float waveLength = 0.3;						// 波长		L
float speed = -0.2;							// 传播速度	S
glm::vec2 center[1] = { glm::vec2(0,0) };	// 传播原点	O
#define PI_ 3.14159265358979323846 


float d_ = 0;

inline float Dot(glm::vec2 &v)		// 传播方向长度	D·(x,y)
{
	glm::vec2 d = v - center[0];
	return glm::sqrt(glm::dot(d, d));
}

inline float waveHeight(glm::vec2 v, float time, float& get_x, float& get_z)
{
	float frequency = 2 * PI_ / waveLength;		// w=2PI/L
	//float phase = speed * frequency;			// Sx2PI/L
	//float theta = dot(v);						// D·(x,y)
	get_x = v.x;
	get_z = v.y;
	d_ = Dot(v);
	if (!d_) d_ = FLT_MIN;
	return amplitude * glm::sin((d_ + time * speed) * frequency);
}

inline float dWavedx(glm::vec2 &v, float time)
{
	float frequency = 2 * PI_ / waveLength;
	float theta = d_;
	float A = amplitude * v.x * frequency / theta;
	return A * glm::cos((theta + time * speed) * frequency);
}

inline float dWavedy(glm::vec2 &v, float time)
{
	float frequency = 2 * PI_ / waveLength;
	float theta = d_;
	float A = amplitude * v.y * frequency / theta;
	return A * glm::cos((theta + time * speed) * frequency);
}

inline glm::vec3 waveNormal(glm::vec2 v, float time)
{
	d_ = Dot(v);
	if (!d_) d_ = FLT_MIN;
	float dx = dWavedx(v, time);
	float dy = dWavedy(v, time);

	glm::vec3 n(-dx, 1.0, -dy);

	float l = glm::length(n);
	//if (l != 0)
	//	return n / l;
	//else
	//	return glm::vec3(0, 1, 0);
	return l ? n / l : glm::vec3(0, 1, 0);
}