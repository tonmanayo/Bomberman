#pragma once

#include <glm/gtx/quaternion.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <window.hpp>
#include <staticSprite.hpp>
#include <camera.hpp>

#define GRAVITY (-50)

class   ParticleMaster;

namespace Zion
{
	class Particle
	{
	private:
		glm::vec3 _position;
		glm::vec3 _velocity;
		float _gravityEffect;
		float _lifeLength;
		float _rotation;
		float _scale;
		float _timeElapsed = 0.0f;
	public:
		Particle(glm::vec3 pos, glm::vec3 velocity, float gravity, float life, float rot, float scale);

		~Particle();

		glm::vec3 getPosition();

		float getRotation();

		float getScale();

		bool update();
	};

	class ParticleRenderer
	{
	private:
		SquareSprite    *_quad;
		Shader         _shader;
	public:
		ParticleRenderer(Shader & shader);
		~ParticleRenderer();

		void        render(std::vector<Particle> particles, Camera camera, glm::mat4 viewMat);
		void        updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 viewMatrix);
	};

	class   ParticleMaster
	{
	private:
		static std::vector<Particle>   _particles;
		static ParticleRenderer        *_renderer;
	public:
		ParticleMaster() = default;
		~ParticleMaster() = default;

		static void        init(Shader & shader);
		static void        update();
		static void        renderParticles(Camera &camera, glm::mat4 viewMat);
		static void        addParticle(Particle particle);
	};

	class   ParticleSystem
	{
	private:
		float       _pps;
		float       _averageSpeed;
		float       _gravity;
		float       _lifeLength;
		float       _scale;
		float       _speedError = 0;
		float       _lifeError = 0;
		float       _scaleError = 0;
		bool        _randomRotation = false;
		glm::vec3   _direction;
		float       _directionDeviation = 0;
	public:
		ParticleSystem(float pps, float speed, float gravity, float lifeLength, float scale);
		~ParticleSystem();

		void        setDirection(glm::vec3 direction, float deviation);
		void        randomizeRotation();
		void        setSpeedError(float error);
		void        setLifeError(float error);
		void        setScaleError(float error);
		void        generateParticles(glm::vec3 centerPosition);
		void        emitParticle(glm::vec3 center);
		float       generateValue(float average, float errorMargin);
		float       generateRotation();
		glm::vec3   generateRandomUnitVector();

	public:
		static  glm::vec3   generateRandomUnitVectorWithinCone(glm::vec3 coneDirection, float angle);
	};
}