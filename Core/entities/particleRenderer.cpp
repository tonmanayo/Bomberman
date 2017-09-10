#include <particle.hpp>

namespace Zion{

	ParticleRenderer::ParticleRenderer(Shader & shader)
	{
		_quad = new SquareSprite(shader, 0, 0, 0.5, 0.5);
		_shader = shader;
	}

	ParticleRenderer::~ParticleRenderer()
	{
		delete _quad;
	}

	void ParticleRenderer::render(std::vector<Particle> particles, Camera camera, glm::mat4 viewMat)
	{
		glm::mat4 viewMatrix = camera.getViewMatrix();
		_shader.enable();
		glBindVertexArray(_quad->getVao());
		glDepthMask((GLboolean)false);
		//glDisable(GL_DEPTH_TEST);
		for (Particle& particle : particles)
		{
			updateModelViewMatrix(particle.getPosition(), particle.getRotation(), particle.getScale(), viewMat);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const GLvoid *)nullptr);
		}
		glDepthMask((GLboolean)true);
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
