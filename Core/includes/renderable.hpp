#pragma once

#include <shader.hpp>
#include <window.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <glm/gtx/quaternion.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define SCALE 1
#define TRANS 2
#define MODEL_INSTANCE_DATA_SIZE 16
#define MODEL_MAX_INSTANCES 100

enum GAMESTATE
{
	MENU,
	PAUSE,
	GAME,
	START,
	END,
	CUT_SCENE_START,
	CUT_SCENE_END,
	CREDITS,
	SCORE
};

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

class MainGame;

namespace Zion
{
	class   Renderable
	{
	protected:
		Shader  _shader;
		bool    _hasAnimation = false;
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
		static  float   secondsPassed;

		virtual void    render(glm::mat4 matrix) = 0;
		virtual void    simpleRender(glm::mat4 matrix) = 0;

		Shader  getShader(){return _shader;}
		bool    hasAnimation(){return _hasAnimation;}
		void    enableShader(){_shader.enable();}
		void    disableShader(){_shader.disable();}
		void    enableAnimeInShader(){_shader.setUniform1i((GLchar *)"hasAnime", (int)true);}
	};

	struct  RendererObj
	{
		int         id;
		float       animeTime;
		int         animeType;
		bool        die;
		float       alpha;
		float       startTime;
		Renderable  *model;
		glm::mat4   matrix;
	};

	struct  Renderer
	{
	private:
		std::map<std::string, std::vector<RendererObj>>    _objects;
		GLuint      _vbo = 0;
		int         _pointer;
	private:
		void    _renderBreakable(std::vector<RendererObj>& objects);
		void    _renderStatic(std::vector<RendererObj>& objects);
		void    _renderAnime(std::vector<RendererObj>& objects, std::string type);
	public:
		Renderer() = default;
		Renderer(const Renderer & rhs);
		Renderer&   operator=(const Renderer & rhs);
		~Renderer();

		void    addToRender(std::string type, int id, Renderable *model, glm::mat4 mat = glm::mat4(1.0f));
		void    removeFromRender(std::string type, int id);
		void    removeObject(std::string type, int id);
		void    removeGroup(std::string type);
		void    removeAll();
		void    applyTransformationToRenderable(std::string type, int id, glm::mat4 mat);
		void    increaseAnimeTime(std::string type, int id, float val);
		void    render();
		void    addInstanceAttribute(GLuint vao, GLuint vbo, GLuint attribute, int dataSize, int dataLength, int offset);
		void    storeMatrix(glm::mat4 &matrix, float *vboData);
	};
}