#include <renderable.hpp>
#include <zion.h>
#include <MainGame.hpp>

namespace Zion
{
	Renderer::Renderer(const Renderer &rhs)
	{
		*this = rhs;
	}

	Renderer& Renderer::operator=(const Renderer &rhs)
	{
		return *this;
	}

	Renderer::~Renderer()
	{
		_objects.clear();
	}

	void Renderer::addToRender(std::string type, int id, Renderable *model, glm::mat4 mat)
	{
		if (_vbo == 0){
			glGenBuffers(1, &_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			glBufferData(GL_ARRAY_BUFFER, MODEL_INSTANCE_DATA_SIZE * MODEL_MAX_INSTANCES, "", GL_STREAM_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		_objects[type].push_back({id, 0.0f, 0, false, 1.0f, -1.0f, model, mat});
	}

	void Renderer::render()
	{
		/*for (std::pair<std::string, std::vector<RendererObj>> pair : _objects)
		{
            if (!pair.first.compare("endLevel"))
                _renderStatic(pair.second);
			if (!pair.first.compare("star"))
                _renderStatic(pair.second);
			if (!pair.first.compare("lemon"))
				_renderStatic(pair.second);
			if (!pair.first.compare("present"))
				_renderStatic(pair.second);
			if (!pair.first.compare("wall"))
				_renderStatic(pair.second);
			if (!pair.first.compare("unbreakBlock"))
				_renderStatic(pair.second);
			if (!pair.first.compare("background"))
				_renderStatic(pair.second);
			if (!pair.first.compare("floors"))
				_renderStatic(pair.second);
            if (!pair.first.compare("explosion"))
                _renderStatic(pair.second);
			if (!pair.first.compare("explosion1"))
				_renderStatic(pair.second);
			if (!pair.first.compare("explosion2"))
				_renderStatic(pair.second);
			if (!pair.first.compare("explosion3"))
				_renderStatic(pair.second);
			if (!pair.first.compare("explosion4"))
				_renderStatic(pair.second);
			if (!pair.first.compare("player"))
				_renderStatic(pair.second);
			if (!pair.first.compare("bomb"))
				_renderStatic(pair.second);
			if (!pair.first.compare("breakBlock"))
				_renderStatic(pair.second);
			if (!pair.first.compare("enemy1"))
				_renderAnime(pair.second, pair.first);
		}*/
		_renderStatic(_objects["background"]);
		_renderStatic(_objects["floors"]);
		_renderStatic(_objects["wall"]);
		_renderStatic(_objects["unbreakBlock"]);
		_renderBreakable(_objects["breakBlock"]);
		_renderAnime(_objects["bomb"], "bomb");
		_renderStatic(_objects["heart"]);
		_renderStatic(_objects["star"]);
		_renderStatic(_objects["endLevel"]);
		_renderStatic(_objects["lemon"]);
		_renderStatic(_objects["present"]);
		_renderAnime(_objects["mag"], "mag");
		_renderAnime(_objects["cubex"], "cubex");
		_renderAnime(_objects["dino"], "dino");
		_renderAnime(_objects["illy"], "illy");
		_renderAnime(_objects["wooki"], "wooki");
		_renderAnime(_objects["moos"], "moos");
		_renderAnime(_objects["enemyBallon"], "enemyBallon");
		_renderAnime(_objects["enemyBallon2"], "enemyBallon2");
		_renderAnime(_objects["player"], "player");
	}

	void Renderer::_renderBreakable(std::vector<RendererObj> &objects)
	{
		Shader  *shader = MainGame::game->getShader("fire");

		if (objects.empty())
			return;
		auto *model = (Zion::Gltf *)objects[0].model;
		shader->enable();
		/// render alive blocks
		shader->setUniform1i((GLchar *)"dead", (int)false);
		model->loadMaterialToShader();
		for (RendererObj& obj : objects)
		{
			if (!obj.die)
				model->simpleRender(obj.matrix);
		}
		/// render dead blocks
		Material *fireBlock = MainGame::game->getMaterial("explosion2");
		Material::sendMaterialToShader(*shader, *fireBlock, 0);
		for (RendererObj &obj : objects)
		{
			if (obj.die){
				shader->setUniform1i((GLchar *)"dead", (int)true);
				obj.model->simpleRender(obj.matrix);
				if (obj.startTime == -1.0f)
					obj.startTime = 0.0f;
				obj.startTime += Zion::Renderable::deltaTime;
				if (obj.startTime > 1.2f){
					removeObject("breakBlock", obj.id);
					continue;
				}
				float   lifeFactor = obj.startTime / 1.2f;
				int     stageCount = fireBlock->numRows * fireBlock->numRows;
				float   atlasProgression = lifeFactor * stageCount;
				int     index1 = (int)std::floor(atlasProgression);
				int     index2 = index1 < stageCount - 1 ? index1 + 1 : index1;
				shader->setUniform2f((GLchar *)"rowsBlend", {fireBlock->numRows, fmodf(atlasProgression, 1)});
				int     column = index1 % fireBlock->numRows;
				int     row = index1 / fireBlock->numRows;
				glm::vec2 offSet = {(float)column / fireBlock->numRows, (float)row / fireBlock->numRows};
				shader->setUniform2f((GLchar *)"offSet1", offSet);
				column = index2 % fireBlock->numRows;
				row = index2 / fireBlock->numRows;
				offSet = {(float)column / fireBlock->numRows, (float)row / fireBlock->numRows};
				shader->setUniform2f((GLchar *)"offSet2", offSet);
				obj.model->simpleRender(obj.matrix);
			}
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		shader->disable();
	}

	void Renderer::_renderStatic(std::vector<RendererObj> &objects)
	{
		if (objects.empty())
			return;
		auto *model = (Zion::Gltf *)objects[0].model;
		model->enableShader();
		model->loadMaterialToShader();
		/// instance rendering
		GLuint vao = model->getVao();
		GLuint indicesCount = model->getIndicesCount();
		int     instanceCount = 0;
		/// enabling attrib pointers
		addInstanceAttribute(vao, _vbo, 1, 4, MODEL_INSTANCE_DATA_SIZE,  0);
		addInstanceAttribute(vao, _vbo, 2, 4, MODEL_INSTANCE_DATA_SIZE,  4);
		addInstanceAttribute(vao, _vbo, 3, 4, MODEL_INSTANCE_DATA_SIZE,  8);
		addInstanceAttribute(vao, _vbo, 4, 4, MODEL_INSTANCE_DATA_SIZE,  12);

		glBindVertexArray(vao);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		_pointer = 0;
		for (RendererObj& obj : objects)
		{
			instanceCount++;
		}
		float   vboData[instanceCount * MODEL_INSTANCE_DATA_SIZE];
		for (RendererObj& obj : objects)
		{
			storeMatrix(obj.matrix, vboData);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, (instanceCount * MODEL_INSTANCE_DATA_SIZE) * 4, nullptr, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, (instanceCount * MODEL_INSTANCE_DATA_SIZE) * 4, &vboData[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)indicesCount, GL_UNSIGNED_SHORT, (const GLvoid *)nullptr, instanceCount);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		glBindVertexArray(0);
		/// rendering
		/*for (RendererObj& obj : objects)
		{
			if (!obj.die)
				obj.model->simpleRender(obj.matrix);
		}*/
		model->unloadMaterialFromShader();
		model->disableShader();
	}

	void Renderer::_renderAnime(std::vector<RendererObj> &objects, std::string type)
	{
		if (objects.empty())
			return;
		auto *model = (Zion::Gltf *)objects[0].model;
		model->enableShader();
		model->loadMaterialToShader();
		model->enableAnimeInShader();
		/// rendering
		for (RendererObj& obj : objects)
		{
			model->loadAnimationMatrix(obj.animeType, obj.animeTime);
			obj.model->simpleRender(obj.matrix);
			if (type != std::string("player") && MainGame::game->getGameState() == GAMESTATE::GAME)
			{
				if (type == std::string("bomb"))
					increaseAnimeTime(type, obj.id, 4.0f * Zion::Renderable::deltaTime);
				else
					increaseAnimeTime(type, obj.id, 4.0f * Zion::Renderable::deltaTime);
			}
		}
		model->unloadMaterialFromShader();
		model->disableShader();
	}

	void Renderer::removeObject(std::string type, int id)
	{
		int     pos = 0;
		try {
			std::vector<RendererObj>& objects = _objects[type];
			for (RendererObj& obj : objects)
			{
				if (obj.id == id)
				{
					objects.erase(objects.begin() + pos);
					return;
				}
				pos++;
			}
		} catch (const std::out_of_range& oor) {
			return;
		}
	}

	void Renderer::removeFromRender(std::string type, int id)
	{
		int     pos = 0;
		try {
			std::vector<RendererObj>& objects = _objects[type];
			for (RendererObj& obj : objects)
			{
				if (obj.id == id)
				{
					if (type != std::string("breakBlock"))
						objects.erase(objects.begin() + pos);
					else
						obj.die = true;
					return;
				}
				pos++;
			}
		} catch (const std::out_of_range& oor) {
			return;
		}
	}

	void Renderer::removeGroup(std::string type)
	{
		std::map<std::string, std::vector<RendererObj>>::iterator it;

		it = _objects.find(type);
		if (it != _objects.end())
			_objects.erase(it);
	}

	void Renderer::removeAll()
	{
		for (std::pair<std::string, std::vector<RendererObj>> pair : _objects)
			pair.second.clear();
		_objects.clear();
	}

	void Renderer::applyTransformationToRenderable(std::string type, int id, glm::mat4 mat)
	{
		try {
			std::vector<RendererObj>& objects = _objects[type];
			for (RendererObj& obj : objects)
			{
				if (obj.id == id)
				{
					obj.matrix = mat;
					return;
				}
			}
		} catch (const std::out_of_range& oor) {
			return;
		}
	}

	void Renderer::increaseAnimeTime(std::string type, int id, float val)
	{
		try {
			std::vector<RendererObj>& objects = _objects[type];
			for (RendererObj& obj : objects)
			{
				if (obj.id == id)
				{
					obj.animeTime += val;
					return;
				}
			}
		}catch (const std::out_of_range& oor) {
			return;
		}
	}

	void Renderer::addInstanceAttribute(GLuint vao, GLuint vbo, GLuint attribute, int dataSize, int dataLength, int offset)
	{
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE, dataLength * 4, (void *)(offset * 4));
		glVertexAttribDivisor(attribute, 1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Renderer::storeMatrix(glm::mat4 &matrix, float *vboData)
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