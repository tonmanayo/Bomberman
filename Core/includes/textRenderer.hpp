#pragma once

#include <shader.hpp>
#include <material.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>

namespace   Zion{

	/// Holds all state information relevant to a character as loaded using FreeType
	struct Character {
		GLuint      Advance;     // Horizontal offset to advance to next glyph
		GLuint      TextureID;   // ID handle of the glyph texture
		glm::ivec2  Size;    // Size of glyph
		glm::ivec2  Bearing; // Offset from baseline to left/top of glyph
	};

	class   TextRenderer{
	private:
		GLuint      _vao;
		GLuint      _vbo;
	public:
		Shader                      *textShader;
		std::map<GLchar, Character> Characters;// Holds a list of pre-compiled Characters
	public:
		TextRenderer(Shader *shader, GLuint width, GLuint height);
		~TextRenderer() = default;
		void    loadFont(std::string font, GLuint fontSize);
		void    renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
	};
}