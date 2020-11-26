#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI_ 3.14159265358979323846 

#define numWaves 1
float amplitude[numWaves] =
{ 0.1 };						// 振幅		A_i
float waveLength[numWaves] =
{ 0.1 * 2 * PI_ };				// 波长		L_i
float speed[numWaves] =
{ -0.2 };						// 传播速度	S_i
glm::vec2 center[numWaves] =
{ 
	glm::vec2(0,0)
};								// 传播原点	O
float Q_parameter[numWaves] =
{ 0.4 };

inline float Dot(int i, glm::vec2 &v)				// 传播方向长度	D·(x,y)
{
	glm::vec2 d = v - center[i];
	return glm::sqrt(glm::dot(d, d));
}

float d_ = 0;

inline float wave(int i, glm::vec2 v, float time)
{
	float frequency = 2 * PI_ / waveLength[i];	// w=2PI/L
	//float phase = speed * frequency;			// Sx2PI/L
	//float theta = dot(v);						// D·(x,y)
	return amplitude[i] * glm::sin((d_ + time * speed[i]) * frequency);
}

inline float waveXZ(int i, glm::vec2 v, float time)
{
	float frequency = 2 * PI_ / waveLength[i];	// w=2PI/L
	//float phase = speed * frequency;			// Sx2PI/L
	//float theta = dot(v);						// D·(x,y)
	return Q_parameter[i] * amplitude[i] * glm::cos((d_ + time * speed[i]) * frequency);
}

float waveHeight(glm::vec2 v, float time, float &get_x, float &get_z)
{
	float height = 0;
	get_x = v.x;
	get_z = v.y;
	for (int i = 0; i < numWaves; i++)
	{
		d_ = Dot(i, v);
		if (!d_) d_ = FLT_MIN;
		height += wave(i, v, time);
		get_x += waveXZ(i, v, time) * v.x / d_;
		get_z += waveXZ(i, v, time) * v.y / d_;
	}
	return height;
}

inline float dWavedx(int i, glm::vec2 &v, float time)
{
	float frequency = 2 * PI_ / waveLength[i];
	float theta = d_;
	float A = amplitude[i] * v.x * frequency / theta;
	return A * glm::cos((theta + time * speed[i]) * frequency);
}

inline float dWavedy(int i, glm::vec2& v, float time)
{
	float frequency = 2 * PI_ / waveLength[i];
	float theta = d_;
	float A = amplitude[i] * frequency;
	return A * glm::sin((theta + time * speed[i]) * frequency) * Q_parameter[i];
}

inline float dWavedz(int i, glm::vec2& v, float time)
{
	float frequency = 2 * PI_ / waveLength[i];
	float theta = d_;
	float A = amplitude[i] * v.y * frequency / theta;
	return A * glm::cos((theta + time * speed[i]) * frequency);
}

glm::vec3 waveNormal(glm::vec2 v, float time)
{
	float dx = 0;
	float dy = 1;
	float dz = 0;
	for (int i = 0; i < numWaves; i++)
	{
		d_ = Dot(i, v);
		if (!d_) d_ = FLT_MIN;
		dx -= dWavedx(i, v, time);
		dy -= dWavedy(i, v, time);
		dz -= dWavedz(i, v, time);
	}
	glm::vec3 n(dx, dy, dz);

	float l = glm::length(n);
	return l ? n / l : glm::vec3(0, 1, 0);
}