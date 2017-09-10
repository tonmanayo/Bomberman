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

		void        render(std::vector<Particle> particles, Camera camera);
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
		static void        renderParticles(Camera &camera);
		static void        addParticle(Particle particle);
	};
}