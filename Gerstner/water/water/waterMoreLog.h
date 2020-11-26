#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define numWaves 4

float amplitude[numWaves] =
{ 0.01,0.008,0.00007,0.00004 };	// 振幅		A_i

float waveLength[numWaves] =
{ 0.3,0.2,0.02,0.01 };			// 波长		L_i

float speed[numWaves] =
{ -0.2,-0.3,-0.1,-0.1 };		// 传播速度	S_i

glm::vec2 center[numWaves] =
{ glm::vec2(3,1),
	glm::vec2(3,-1),
	glm::vec2(-1,-2),
	glm::vec2(1,-2),
};							// 传播原点	O
#define PI_ 3.14159265358979323846 

int k = 8;					// 指数倍数
float d_ = 0;

inline float Dot(int i, glm::vec2 &v)				// 传播方向	D·(x,y)
{
	glm::vec2 d = v - center[i];
	return glm::sqrt(glm::dot(d, d));
}

inline float wave(int i, glm::vec2 v, float time)
{
	float frequency = 2 * PI_ / waveLength[i];		// w=2PI/L
	//float phase = speed * frequency;			// Sx2PI/L
	//float theta = dot(v);						// D·(x,y)
	return 2 * amplitude[i] * glm::pow((glm::sin((d_ + time * speed[i]) * frequency) + 1) / 2, k);
}

float waveHeight(glm::vec2 v, float time, float& get_x, float& get_z)
{
	float height = 0;
	get_x = v.x;
	get_z = v.y;
	for (int i = 0; i < numWaves; i++)
	{
		d_ = Dot(i, v);
		if (!d_) d_ = FLT_MIN;
		height += wave(i, v, time);
	}
	return height;
}

inline float dWavedx(int i, glm::vec2 &v, float time)
{
	float frequency = 2 * PI_ / waveLength[i];
	float theta = d_;
	float A = amplitude[i] * v.x * frequency / theta;
	float more = glm::pow((glm::sin((d_ + time * speed[i]) * frequency) + 1) / 2, k - 1) * k;
	return A * glm::cos((theta + time * speed[i]) * frequency) * more;
}

inline float dWavedy(int i, glm::vec2& v, float time)
{
	float frequency = 2 * PI_ / waveLength[i];
	float theta = d_;
	float A = amplitude[i] * v.y * frequency / theta;
	float more = glm::pow((glm::sin((d_ + time * speed[i]) * frequency) + 1) / 2, k - 1) * k;
	return A * glm::cos((theta + time * speed[i]) * frequency)*more;
}

glm::vec3 waveNormal(glm::vec2 v, float time)
{
	float dx = 0;
	float dy = 0;
	for (int i = 0; i < numWaves; i++)
	{
		d_ = Dot(i, v);
		if (!d_) d_ = FLT_MIN;
		dx += dWavedx(i, v, time);
		dy += dWavedy(i, v, time);
	}
	glm::vec3 n(-dx, 1.0, -dy);

	float l = glm::length(n);
	return l ? n / l : glm::vec3(0, 1, 0);
}