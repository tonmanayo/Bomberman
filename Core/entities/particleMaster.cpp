#include <particle.hpp>

namespace   Zion
{
	std::map<Material *, std::vector<Particle>> ParticleMaster::_particles;
	ParticleRenderer*       ParticleMaster::_renderer;

	void ParticleMaster::init(Shader &shader)
	{
		_renderer = new ParticleRenderer(shader);
	}

	void ParticleMaster::update(Camera *camera)
	{
		for (std::pair<Material *, std::vector<Particle>> particles : _particles)
		{
			int     i = 0;
			for (Particle& particle : particles.second)
			{
				//bool stillAlive = particle.update();
				bool stillAlive = _particles[particles.first][i].update(camera);
				if (!stillAlive)
					_particles[particles.first].erase(_particles[particles.first].begin() + i);
				else
					i++;
			}
			InsertionSort::sortHighToLow(_particles[particles.first]);
		}
	}

	void ParticleMaster::renderParticles(Camera *camera, glm::mat4 viewMat)
	{
		_renderer->render(_particles, camera, viewMat);
	}

	void ParticleMaster::addParticle(Particle particle)
	{
		_particles[particle.getMaterial()].push_back(particle);
	}
}