#include "Particle.hpp"

#include <cmath>
#include <numbers>
#include <cstdlib>

/* CONSTRUCTOR */

Particle::Particle()
	: m_x(0.0f), m_y(0.0f), m_xSpeed(0.0f), m_ySpeed(0.0f), m_xVelocity(0.0f), m_yVelocity(0.0f), m_speed(0.0f), m_angle(0.0f)
{
	//std::wcout << L"CONSTRUCTOR: Particle()" << '\n';
	//m_x = ((2.0f * std::rand()) / RAND_MAX) - 1;
	//m_y = ((2.0f * std::rand()) / RAND_MAX) - 1;

	//m_xSpeed = (0.01f * std::rand()) / RAND_MAX;
	//m_ySpeed = (0.01f * std::rand()) / RAND_MAX;
	//m_xSpeed = 0.01f * (((2.0f * std::rand()) / RAND_MAX) - 1.0f);
	//m_ySpeed = 0.01f * (((2.0f * std::rand()) / RAND_MAX) - 1.0f);

	m_angle = static_cast<float>((2.0f * std::numbers::pi * std::rand()) / RAND_MAX);
	m_speed = (0.01f * std::rand()) / RAND_MAX;
	m_xSpeed = m_speed * std::cos(m_angle);
	m_ySpeed = m_speed * std::sin(m_angle);
}

/* DESTRUCTOR */

Particle::~Particle()
{
	//std::wcout << L"DESTRUCTOR: ~Particle()" << '\n';
}

/* FUNCTION DEFINITIONS */

void Particle::Init()
{

}

void Particle::Update()
{
	m_x += m_xSpeed;
	m_y += m_ySpeed;

	if (m_x <= -1.0 || m_x >= 1.0) m_xSpeed = -m_xSpeed;
	if (m_y <= -1.0 || m_y >= 1.0) m_ySpeed = -m_ySpeed;
	//if (m_x <= -1.0 || m_x >= 1.0) m_x = 0.0f;
	//if (m_y <= -1.0 || m_y >= 1.0) m_y = 0.0f;
}