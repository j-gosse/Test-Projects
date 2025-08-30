#include "Particle.hpp"

#include <cmath>
#include <cstdlib>

/* CONSTRUCTOR */

Particle::Particle() :
	m_x(0.0f),
	m_y(0.0f),
	m_xSpeed(0.0f),
	m_ySpeed(0.0f),
	m_speed(0.0f),
	m_angle(0.0f),
	m_radius(0.0f),
	m_angularSpeed(0.0f),
	m_radialSpeed(0.0f),
	m_minRadius(0.0f),
	m_maxRadius(0.0f),
	m_minScale(0.0f),
	m_maxScale(0.0f)
{
	//std::wcout << L"CONSTRUCTOR: Particle()" << '\n';
	
	//m_x = ((2.0f * std::rand()) / RAND_MAX) - 1;
	//m_y = ((2.0f * std::rand()) / RAND_MAX) - 1;

	//m_xSpeed = (0.01f * std::rand()) / RAND_MAX;
	//m_ySpeed = (0.01f * std::rand()) / RAND_MAX;
	//m_xSpeed = 0.01f * (((2.0f * std::rand()) / RAND_MAX) - 1.0f);
	//m_ySpeed = 0.01f * (((2.0f * std::rand()) / RAND_MAX) - 1.0f);

	//m_angle = static_cast<float>((2.0f * std::numbers::pi * std::rand()) / RAND_MAX);
	//m_speed = (0.0003f * std::rand()) / RAND_MAX;
	//m_xSpeed = m_speed * std::cos(m_angle);
	//m_ySpeed = m_speed * std::sin(m_angle);
}

/* DESTRUCTOR */

Particle::~Particle()
{
	//std::wcout << L"DESTRUCTOR: ~Particle()" << '\n';
}

/* FUNCTION DEFINITIONS */

void Particle::Init(float angle, float radius, float angularSpeed, float radialSpeed, int width, int height)
{
	float centerX = width / 2.0f;
	float centerY = height / 2.0f;

	m_x = centerX;
	m_y = centerY;
	m_angle = angle;
	m_radius = radius;
	m_angularSpeed = angularSpeed;
	m_radialSpeed = radialSpeed;
	m_minRadius = 0.0f;
	m_maxRadius = min(width, height) * 0.4f; // 40% of window
	m_minScale = 0.2f;
	m_maxScale = 1.0f;
}

void Particle::Update(int interval, ULONGLONG elapsedTime, int steps, int width, int height)
{
	float elapsed = elapsedTime / 1000.0f;
	float centerX = width / 2.0f;
	float centerY = height / 2.0f;
	m_maxRadius = min(width, height) * 0.4f; // 40% of window

	m_x = centerX;
	m_y = centerY;
	m_angle += m_angularSpeed;
	m_radius += m_radialSpeed;
	float pulse = std::sin(elapsed * 1.5f + steps * 0.1f);
	float baseRadius = m_minRadius + (m_maxRadius - m_minRadius) * 0.5f * (pulse + 1);
	float spiralScale = m_minScale + (m_maxScale - m_minScale) * 0.5f * (std::sin(elapsed * 0.8f) + 1);
	m_radius = baseRadius * spiralScale;

	//m_x += m_xSpeed * interval;
	//m_y += m_ySpeed * interval;

	//if (m_x <= -1.0 || m_x >= 1.0) m_xSpeed = -m_xSpeed;
	//if (m_y <= -1.0 || m_y >= 1.0) m_ySpeed = -m_ySpeed;
	//if (m_x <= -1.0 || m_x >= 1.0) m_x = 0.0f;
	//if (m_y <= -1.0 || m_y >= 1.0) m_y = 0.0f;
}