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

	void Renderer::addToRender(const char *type, int id, Renderable *model, glm::mat4 mat)
	{
		_objects[type].push_back({id, model, mat});
	}

	void Renderer::render()
	{
		for (std::pair<const char *, std::vector<RendererObj>> pair : _objects)
		{
			for (RendererObj obj : pair.second)
				obj.model->render(obj.matrix);
		}
	}
}