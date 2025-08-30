#pragma once

#ifndef PARTICLE_HPP_
#define PARTICLE_HPP_

#include <windows.h>
#include <cstdint>
#include <numbers>

class Particle
{
private:
	float m_x, m_y;
	float m_xSpeed, m_ySpeed;
	float m_speed;
	float m_angle;
	float m_radius;
	float m_angularSpeed;
	float m_radialSpeed;

	float m_minRadius;
	float m_maxRadius;
	float m_minScale;
	float m_maxScale;

public:
	Particle();
	virtual ~Particle();

	void Init(float angle, float radius, float angularSpeed, float radialSpeed, int width, int height);
	void Update(int interval, ULONGLONG elapsedTime, int steps, int width, int height);

	static constexpr const uint32_t NUM_PARTICLES = 5000;
	static constexpr const int RGBA_MIN = 0;
	static constexpr const int RGBA_MAX = 255;

	float GetX() const { return m_x; }
	float GetY() const { return m_y; }
	float GetAngle() const { return m_angle; }
	float GetRadius() const { return m_radius; }
};

#endif