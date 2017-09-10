#include <particle.hpp>

namespace   Zion
{
	std::vector<Particle>   ParticleMaster::_particles;
	ParticleRenderer*       ParticleMaster::_renderer;

	void ParticleMaster::init(Shader &shader)
	{
		_renderer = new ParticleRenderer(shader);
	}

	void ParticleMaster::update()
	{
		int     i = 0;
		for (Particle& particle : _particles)
		{
			bool stillAlive = particle.update();
			if (!stillAlive)
				_particles.erase(_particles.begin() + i);
			else
				i++;
		}
	}

	void ParticleMaster::renderParticles(Camera &camera, glm::mat4 viewMat)
	{
		_renderer->render(_particles, camera, viewMat);
	}

	void ParticleMaster::addParticle(Particle particle)
	{
		_particles.push_back(particle);
	}
}