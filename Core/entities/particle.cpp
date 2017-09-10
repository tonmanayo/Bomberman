#include <particle.hpp>

namespace Zion {

	Particle::Particle(Material *material, glm::vec3 pos, glm::vec3 velocity, float gravity, float life, float rot, float scale, bool additive)
	{
		_material = material;
		_position = pos;
		_velocity = velocity;
		_gravityEffect = gravity;
		_lifeLength = life;
		_rotation = rot;
		_scale = scale;
		_additive = additive;
		ParticleMaster::addParticle(*this);
	}

	Particle& Particle::operator=(const Particle &rhs)
	{
		if (this != &rhs)
		{
			_position = rhs._position;
			_velocity = rhs._velocity;
			_gravityEffect = rhs._gravityEffect;
			_lifeLength = rhs._lifeLength;
			_rotation = rhs._rotation;
			_scale = rhs._scale;
			_timeElapsed = rhs._timeElapsed;
			_material = rhs._material;
			_offset1 = rhs._offset1;
			_offset2 = rhs._offset2;
			_blend = rhs._blend;
			_distance = rhs._distance;
		}
		return *this;
	}

	Particle::~Particle()
	{}

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

	Material* Particle::getMaterial()
	{
		return _material;
	}

	glm::vec2 Particle::getOffset1()
	{
		return _offset1;
	}

	glm::vec2 Particle::getOffset2()
	{
		return _offset2;
	}

	float Particle::getBlend()
	{
		return _blend;
	}

	float Particle::getDistance()
	{
		return _distance;
	}

	bool Particle::update(Camera *camera)
	{
		_velocity.y += GRAVITY * _gravityEffect * Zion::Renderable::deltaTime;
		glm::vec3 change = glm::vec3(_velocity);
		change *= Zion::Renderable::deltaTime;
		_position += change;
		_distance = glm::length2(camera->getCameraPosition() - _position);
		updateTextureCoordInfo();
		_timeElapsed += Zion::Renderable::deltaTime;
		return _timeElapsed < _lifeLength;
	}

	void Particle::updateTextureCoordInfo()
	{
		float   lifeFactor = _timeElapsed / _lifeLength;
		int     stageCount = _material->numRows * _material->numRows;
		float   atlasProgression = lifeFactor * stageCount;
		int     index1 = (int)std::floor(atlasProgression);
		int     index2 = index1 < stageCount - 1 ? index1 + 1 : index1;
		_blend = fmodf(atlasProgression, 1);
		setTextureOffset(_offset1, index1);
		setTextureOffset(_offset2, index2);
	}

	void Particle::setTextureOffset(glm::vec2 &offset, int index)
	{
		int column = index % _material->numRows;
		int row = index / _material->numRows;
		offset.x = (float)column / _material->numRows;
		offset.y = (float)row / _material->numRows;
	}
}