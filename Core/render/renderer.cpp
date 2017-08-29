#include <renderable.hpp>

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
		_objects[type].push_back({id, model, mat});
	}

	void Renderer::render()
	{
		for (std::pair<std::string, std::vector<RendererObj>> pair : _objects)
		{
			for (RendererObj obj : pair.second)
				obj.model->render(obj.matrix);
		}
	}

	void Renderer::removeFromRender(std::string type, int id)
	{
		int     pos = 0;
		try {
			std::vector<RendererObj>& objects = _objects[type];
			for (RendererObj obj : objects)
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
}