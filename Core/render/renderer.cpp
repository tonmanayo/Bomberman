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
		_renderStatic(_objects["explosion4"]);
		_renderAnime(_objects["mag"], "mag");
		_renderAnime(_objects["cubex"], "cubex");
		_renderAnime(_objects["dino"], "dino");
		_renderAnime(_objects["illy"], "illy");
		_renderAnime(_objects["enemyBallon"], "enemyBallon");
		_renderAnime(_objects["enemyBallon2"], "enemyBallon2");
		_renderAnime(_objects["player"], "player");
	}

	void Renderer::_renderBreakable(std::vector<RendererObj> &objects)
	{
		std::vector<RendererObj>    deadBlocks;
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
				obj.model->simpleRender(obj.matrix);
			else
				deadBlocks.push_back(obj);
		}
		model->unloadMaterialFromShader();
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
		/// rendering
		for (RendererObj& obj : objects)
		{
			if (!obj.die)
				obj.model->simpleRender(obj.matrix);
		}
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
}