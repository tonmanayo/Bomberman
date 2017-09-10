#include <particle.hpp>

namespace Zion{

	ParticleRenderer::ParticleRenderer(Shader & shader)
	{
		_quad = new SquareSprite(shader, 0, 0, 0.9, 0.9);
		_shader = shader;
	}

	ParticleRenderer::~ParticleRenderer()
	{
		delete _quad;
	}

	void ParticleRenderer::render(std::map<Material *, std::vector<Particle>>& particles, Camera *camera, glm::mat4 viewMat)
	{
		glm::mat4 viewMatrix = camera->getViewMatrix();
		_shader.enable();
		glBindVertexArray(_quad->getVao());
		glDepthMask((GLboolean)false);
		for (std::pair<Material *, std::vector<Particle>> listParticles : particles)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, listParticles.first->texure.getTextureId());
			for (Particle& particle : listParticles.second)
			{
				if (particle.getAddictive())
					glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				updateModelViewMatrix(particle.getPosition(), particle.getRotation(), particle.getScale(), viewMat);
				_shader.setUniform2f((GLchar *)"texOffset1", particle.getOffset1());
				_shader.setUniform2f((GLchar *)"texOffset2", particle.getOffset2());
				_shader.setUniform2f((GLchar *)"texCoordInfo", {listParticles.first->numRows, particle.getBlend()});
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const GLvoid *)nullptr);
			}
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glDepthMask((GLboolean)true);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindVertexArray(0);
		_shader.disable();
	}

	void ParticleRenderer::updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 viewMatrix)
	{
		glm::mat4   modelMatrix = glm::mat4();
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix[0].x =  viewMatrix[0].x;
		modelMatrix[0].y =  viewMatrix[1].x;
		modelMatrix[0].z =  viewMatrix[2].x;

		modelMatrix[1].x =  viewMatrix[0].y;
		modelMatrix[1].y =  viewMatrix[1].y;
		modelMatrix[1].z =  viewMatrix[2].y;

		modelMatrix[2].x =  viewMatrix[0].z;
		modelMatrix[2].y =  viewMatrix[1].z;
		modelMatrix[2].z =  viewMatrix[2].z;
		glm::rotate(modelMatrix, glm::radians(rotation), {0, 0, 1});
		glm::scale(modelMatrix, {scale, scale, scale});
		_shader.setUniformMat4((GLchar *)"model_matrix", modelMatrix);
	}
}
