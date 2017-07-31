//
// Created by Tony MACK on 2017/07/29.
//

#ifndef BOMBERMAN_GLSLPROGRAM_HPP
#define BOMBERMAN_GLSLPROGRAM_HPP


#include <string>
#include <OpenGL/gl3.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <SDL.h>
namespace WTCEngine {
    class GLSLProgram {
    private:
        GLuint _programID;
        GLuint _vertexShaderID;
        GLuint _fragmentShaderID;
        GLuint _numAtterbutes;

        void compileShader(const std::string &filepath, GLuint id);

    public:
        GLSLProgram();

        virtual ~GLSLProgram();

        void compileShaders(const std::string &vertexShaderFilePath, const std::string &fragmentShaderFilePath);

        void linkShaders();

        void addAttribute(const std::string &attributeName);

        void use();

        void unuse();

        GLint getUiformLocation(const std::string &unifronName);
    };

}
#endif //BOMBERMAN_GLSLPROGRAM_HPP
