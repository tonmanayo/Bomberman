#include <particle.hpp>

namespace Zion{

	int     ParticleRenderer::MAX_INSTANCES = 10000;
	int     ParticleRenderer::INSTANCE_DATA_LENGTH = 21;

	ParticleRenderer::ParticleRenderer(Shader & shader)
	{
		_quad = new SquareSprite(shader, 0, 0, 1.0, 1.0);
		_shader = shader;
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, INSTANCE_DATA_LENGTH * MAX_INSTANCES, "", GL_STREAM_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		addInstanceAttribute(_quad->getVao(), _vbo, 1, 4, INSTANCE_DATA_LENGTH,  0);
		addInstanceAttribute(_quad->getVao(), _vbo, 2, 4, INSTANCE_DATA_LENGTH,  4);
		addInstanceAttribute(_quad->getVao(), _vbo, 3, 4, INSTANCE_DATA_LENGTH,  8);
		addInstanceAttribute(_quad->getVao(), _vbo, 4, 4, INSTANCE_DATA_LENGTH,  12);
		addInstanceAttribute(_quad->getVao(), _vbo, 5, 4, INSTANCE_DATA_LENGTH,  16);
		addInstanceAttribute(_quad->getVao(), _vbo, 6, 1, INSTANCE_DATA_LENGTH,  20);
	}

	ParticleRenderer::~ParticleRenderer()
	{
	}

	void ParticleRenderer::render(std::map<Material *, std::vector<Particle>>& particles, Camera *camera, glm::mat4 viewMat)
	{
		(void)camera;
		prepare();
		for (std::pair<Material *, std::vector<Particle>> listParticles : particles)
		{
			std::vector<Particle> &  ListParticles = listParticles.second;
			if (!ListParticles.empty())
			{
				bindTexture(listParticles.first, listParticles.second[0]);
				_pointer = 0;
				float   vboData[ListParticles.size() * INSTANCE_DATA_LENGTH];
				for (Particle& particle : ListParticles)
				{
					updateModelViewMatrix(particle.getPosition(), particle.getRotation(), particle.getScale(), viewMat, vboData);
					updateTexCoordInfo(particle, vboData);
				}
				glBindBuffer(GL_ARRAY_BUFFER, _vbo);
				glBufferData(GL_ARRAY_BUFFER, (ListParticles.size() * INSTANCE_DATA_LENGTH) * 4, nullptr, GL_STREAM_DRAW);
				glBufferSubData(GL_ARRAY_BUFFER, 0, (ListParticles.size() * INSTANCE_DATA_LENGTH) * 4, &vboData[0]);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				/// instance rendering done here
				glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const GLvoid *)nullptr, ListParticles.size());
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		finishRendering();
	}

	void ParticleRenderer::updateTexCoordInfo(Particle &particle, float *vboData)
	{
		vboData[_pointer++] = particle.getOffset1().x;
		vboData[_pointer++] = particle.getOffset1().y;
		vboData[_pointer++] = particle.getOffset2().x;
		vboData[_pointer++] = particle.getOffset2().y;
		vboData[_pointer++] = particle.getBlend();
	}

	void ParticleRenderer::prepare()
	{
		_shader.enable();
		glBindVertexArray(_quad->getVao());
		glDepthMask((GLboolean)false);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);
	}

	void ParticleRenderer::finishRendering()
	{
		glDepthMask((GLboolean)true);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(5);
		glDisableVertexAttribArray(6);
		glBindVertexArray(0);
		_shader.disable();
	}

	void ParticleRenderer::addInstanceAttribute(int vao, int vbo, int attribute, int dataSize,
	                                            int instancedDataLength, int offset)
	{
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE, instancedDataLength * 4, BUFFER_OFFSET(offset * 4));
		glVertexAttribDivisor(attribute, 1);
		glBindVertexArray(0);
	}

	void ParticleRenderer::bindTexture(Material *material, Particle &particle)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material->texure.getTextureId());
		/// send number of rows in texture to shader
		_shader.setUniform1f((GLchar *)"numberOfRows", material->numRows);
		if (particle.getAddictive())
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}

	void ParticleRenderer::updateModelViewMatrix(glm::vec3 position, float rotation, float scale,
	                                             glm::mat4 viewMatrix, float *vboData)
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
		storeMatrix(modelMatrix, vboData);
	}


	void ParticleRenderer::storeMatrix(glm::mat4 &matrix, float *vboData)
	{
		vboData[_pointer++] = matrix[0][0];
		vboData[_pointer++] = matrix[0][1];
		vboData[_pointer++] = matrix[0][2];
		vboData[_pointer++] = matrix[0][3];
		vboData[_pointer++] = matrix[1][0];
		vboData[_pointer++] = matrix[1][1];
		vboData[_pointer++] = matrix[1][2];
		vboData[_pointer++] = matrix[1][3];
		vboData[_pointer++] = matrix[2][0];
		vboData[_pointer++] = matrix[2][1];
		vboData[_pointer++] = matrix[2][2];
		vboData[_pointer++] = matrix[2][3];
		vboData[_pointer++] = matrix[3][0];
		vboData[_pointer++] = matrix[3][1];
		vboData[_pointer++] = matrix[3][2];
		vboData[_pointer++] = matrix[3][3];
	}
}
