#include <particle.hpp>

namespace Zion{

	ParticleRenderer::ParticleRenderer(Shader & shader)
	{
		_quad = new SquareSprite(shader, 0, 0, 9, 9);
		_shader = shader;
	}

	ParticleRenderer::~ParticleRenderer()
	{
		delete _quad;
	}

	void ParticleRenderer::render(std::vector<Particle> particles, Camera camera)
	{
		glm::mat4 viewMatrix = camera.getViewMatrix();
		_shader.enable();
		glBindVertexArray(_quad->getVao());
		//glEnableVertexAttribArray(0);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask((GLboolean)false);
		//glDisable(GL_DEPTH_TEST);
		for (Particle& particle : particles)
		{
			//_shader.setUniformMat4((GLchar *)"view_matrix", viewMatrix);
			//_shader.setUniformMat4((GLchar *)"model_matrix", glm::translate(glm::mat4(), particle.getPosition()));
			_shader.setUniformMat4((GLchar *)"model_matrix", glm::translate(glm::mat4(), particle.getPosition()));
			//updateModelViewMatrix(particle.getPosition(), particle.getRotation(), particle.getScale(), glm::mat4());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const GLvoid *)nullptr);
			//_quad->render(glm::translate(glm::mat4(), {0, 0, -1}));
		}
		glDepthMask((GLboolean)true);
		//glDisable(GL_BLEND);
		glBindVertexArray(0);
		_shader.disable();
	}

	void ParticleRenderer::updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 viewMatrix)
	{
		glm::mat4   modelMatrix = glm::mat4();
		glm::translate(modelMatrix, position);

		modelMatrix[0][0] = viewMatrix[0][0];
		modelMatrix[0][1] = viewMatrix[1][0];
		modelMatrix[0][2] = viewMatrix[2][0];
		modelMatrix[1][0] = viewMatrix[0][1];
		modelMatrix[1][1] = viewMatrix[1][1];
		modelMatrix[1][2] = viewMatrix[2][1];
		modelMatrix[2][0] = viewMatrix[0][2];
		modelMatrix[2][1] = viewMatrix[1][2];
		modelMatrix[2][2] = viewMatrix[2][2];
		glm::rotate(modelMatrix, glm::radians(rotation), {0, 0, 1});
		glm::scale(modelMatrix, {scale, scale, scale});
		_shader.setUniformMat4((GLchar *)"model_matrix", modelMatrix);
		//_shader.setUniformMat4((GLchar *)"view_matrix", viewMatrix);
	}
}
