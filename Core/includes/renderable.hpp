#pragma once

#include <shader.hpp>
#include <window.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <map>

namespace Zion
{
	class   Renderable
	{
	protected:
		Shader  _shader;
	protected:
		static std::string  GetFilePathExtension(const std::string &fileName)
		{
			if (fileName.find_last_of(".") != std::string::npos)
				return fileName.substr(fileName.find_last_of(".") + 1);
			return "";
		}
	public:
		static  float   startTime;
		static  float   runTime;
		static  float   deltaTime;

		virtual void    render(glm::mat4 matrix) = 0;
	};

	struct  RendererObj
	{
		int         id;
		Renderable  *model;
		glm::mat4   matrix;
	};

	struct  Renderer
	{
	private:
		std::map<const char *, std::vector<RendererObj>>    _objects;
	public:
		Renderer() = default;
		Renderer(const Renderer & rhs);
		Renderer&   operator=(const Renderer & rhs);
		~Renderer();

		void    addToRender(const char *type, int id, Renderable *model, glm::mat4 mat = glm::mat4(1.0f));
		void    render();
	};
}