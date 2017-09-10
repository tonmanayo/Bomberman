#include <particle.hpp>

namespace       Zion
{
	ParticleSystem::ParticleSystem(float pps, float speed, float gravity, float lifeLength, float scale)
	{
		_pps = pps;
		_averageSpeed = speed;
		_gravity = gravity;
		_lifeLength = lifeLength;
		_scale = scale;
	}

	ParticleSystem::~ParticleSystem()
	{
	}

	void ParticleSystem::setDirection(glm::vec3 direction, float deviation)
	{
		_direction =  glm::vec3(direction);
		_directionDeviation = (float)(deviation * M_PI);
	}

	void ParticleSystem::randomizeRotation()
	{
		_randomRotation = true;
	}

	void ParticleSystem::setSpeedError(float error)
	{
		_speedError = error * _averageSpeed;
	}

	void ParticleSystem::setLifeError(float error)
	{
		_lifeError = error * _averageSpeed;
	}

	void ParticleSystem::setScaleError(float error)
	{
		_scale = error * _averageSpeed;
	}

	void ParticleSystem::generateParticles(glm::vec3 centerPosition)
	{
		float   delta = Renderable::deltaTime;
		float   particlesToCreate = _pps * delta;
		int     count = (int)std::floor(particlesToCreate);
		float   partialParticle = fmodf(particlesToCreate, 1);
		for (int i = 0; i < count; i++)
			emitParticle(centerPosition);
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		if (r < partialParticle)
			emitParticle(centerPosition);
	}

	void ParticleSystem::emitParticle(glm::vec3 center)
	{
		glm::vec3   velocity;
		if (_direction != glm::vec3())
			velocity = generateRandomUnitVectorWithinCone(_direction, _directionDeviation);
		else
			velocity = generateRandomUnitVector();
		glm::normalize(velocity);
		velocity *=  generateValue(_averageSpeed, _speedError);
		float   scale = generateValue(_scale, _scaleError);
		float   lifeLength = generateValue(_lifeLength, _lifeError);
		new Particle(center, velocity, _gravity, lifeLength, generateRotation(), scale);

		/// this is for simple particle system
		/*float dirX = (float) static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2.0f - 1.0f;
		float dirZ = (float) static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2.0f - 1.0f;
		velocity = {dirX, 1, dirZ};
		glm::normalize(velocity);
		velocity *= _averageSpeed;
		new Particle(center, velocity, _gravity, _lifeLength, 0, _scale);*/
	}

	float ParticleSystem::generateValue(float average, float errorMargin)
	{
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float offset = (r - 0.5f) * 2.0f * errorMargin;
		return average + offset;
	}

	float ParticleSystem::generateRotation()
	{
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		if (_randomRotation)
			return r * 360.0f;
		return 0;
	}

	glm::vec3 ParticleSystem::generateRandomUnitVectorWithinCone(glm::vec3 coneDirection, float angle)
	{
		//srand(0);
		float   cosAngle = (float)cos(angle);
		float theta = (float)(static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2.0f * M_PI);
		float z = cosAngle + (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * (1 - cosAngle));
		float rootOneMinusZSquared = (float) sqrt(1 - z * z);
		float x = (float)(rootOneMinusZSquared * cos(theta));
		float y = (float)(rootOneMinusZSquared * sin(theta));

		glm::vec4   direction = glm::vec4(x, y, z, 1);
		if (coneDirection.x != 0 || coneDirection.y != 0 || (coneDirection.z != 1 && coneDirection.z != -1))
		{
			glm::vec3 rotateAxis = glm::cross(coneDirection, {0, 0, 1});
			float rotateAngle = (float)acos(glm::dot(coneDirection, {0, 0, 1}));
			glm::mat4   rotationMatrix = glm::mat4();
			rotationMatrix = glm::rotate(rotationMatrix, -rotateAngle, rotateAxis);
			direction = rotationMatrix * direction;
		}else if (coneDirection.z == -1)
			direction.z *= -1;
		return glm::vec3(direction);
	}

	glm::vec3 ParticleSystem::generateRandomUnitVector()
	{
		//srand(0);
		float theta = (float) (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2.0f * M_PI);
		float z = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2) - 1;
		float rootOneMinusZSquared = (float) sqrt(1 - z * z);
		float x = (float) (rootOneMinusZSquared * cos(theta));
		float y = (float) (rootOneMinusZSquared * sin(theta));
		return {x, y, z};
	}
}