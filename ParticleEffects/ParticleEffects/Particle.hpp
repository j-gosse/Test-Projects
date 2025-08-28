#pragma once

#ifndef PARTICLE_HPP_
#define PARTICLE_HPP_

#include <cstdint>

class Particle
{
private:
	float m_x, m_y;
	float m_xSpeed, m_ySpeed;
	float m_xVelocity, m_yVelocity;
	float m_speed;
	float m_angle;

public:
	Particle();
	virtual ~Particle();

	void Init();
	void Update();

	static constexpr const uint32_t NUM_PARTICLES = 1000;
	static constexpr const float RGBA_MIN = 0.0;
	static constexpr const float RGBA_MAX = 255.0;

	float GetX() const { return m_x; }
	float GetY() const { return m_y; }
};

#endif