#include <particle.hpp>

namespace Zion {

	Particle::Particle(glm::vec3 pos, glm::vec3 velocity, float gravity, float life, float rot, float scale)
	{
		_position = pos;
		_velocity = velocity;
		_gravityEffect = gravity;
		_lifeLength = life;
		_rotation = rot;
		_scale = scale;
		ParticleMaster::addParticle(*this);
	}

	Particle::~Particle()
	{
	}

	glm::vec3 Particle::getPosition()
	{
		return _position;
	}

	float Particle::getRotation()
	{
		return _rotation;
	}

	float Particle::getScale()
	{
		return _scale;
	}

	bool Particle::update()
	{
		_velocity.y += GRAVITY * _gravityEffect * Window::frameChangeTime;
		glm::vec3 change = glm::vec3(_velocity);
		change *= Window::frameChangeTime;
		_position += change;
		_timeElapsed += Window::frameChangeTime;
		return _timeElapsed < _lifeLength;
	}
}