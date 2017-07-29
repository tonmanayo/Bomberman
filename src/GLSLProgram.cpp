//
// Created by Tony MACK on 2017/07/29.
//


#include "../inc/GLSLProgram.hpp"
#include "../inc/ErrorHandle.hpp"

GLSLProgram::GLSLProgram() : _programID(0), _vertexShaderID(0), _fragmentShaderID(0), _numAtterbutes(0){

}

GLSLProgram::~GLSLProgram() {

}

void GLSLProgram::compileShader(const std::string &filepath, GLuint id) {
    try {
        std::ifstream vertexFile(filepath);
        if (vertexFile.fail())
            throw ErrorHandle("Failed to open: " + filepath);

        std::string fileContents = "";
        std::string line;

        while (std::getline(vertexFile, line))
            fileContents += line + "\n";

        vertexFile.close();
        const char *tmp_ptr = fileContents.c_str();
        glShaderSource(id, 1, &tmp_ptr, nullptr);
        glCompileShader(id);

        GLint success = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<char> errorlog(maxLength);
            glGetShaderInfoLog(id, maxLength, &maxLength, &errorlog[0]);
            glDeleteShader(id);
            std::printf("%s\n", &(errorlog[0]));
            throw ErrorHandle("Error loading Shaders!");
        }
    } catch (ErrorHandle errorHandle) {
        std::cout << errorHandle.what() << std::endl;
    }
}

void GLSLProgram::compileShaders(const std::string &vertexShaderFilePath, const std::string &fragmentShaderFilePath) {
    try {
        _programID = glCreateProgram();
        _vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        if (_vertexShaderID == 0)
            throw ErrorHandle("Error: Vertex Shader failer to be created");

        _fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        if (_vertexShaderID == 0)
            throw ErrorHandle("Error: Vertex Shader failer to be created");
        //compile the shaders
       compileShader(vertexShaderFilePath, _vertexShaderID);
       compileShader(fragmentShaderFilePath, _fragmentShaderID);

    } catch (ErrorHandle errorHandle){
       std::cout << errorHandle.what() << std::endl;
        SDL_Quit();
        exit(-1);
    }
}

void GLSLProgram::linkShaders() {
    try {
        glAttachShader(_programID, _fragmentShaderID);
        glAttachShader(_programID, _vertexShaderID);

        glLinkProgram(_programID);

        GLint isLinked = 0;
        glGetProgramiv(_programID, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<char> errorlog(maxLength);
            glGetShaderInfoLog(_programID, maxLength, &maxLength, &errorlog[0]);
            glDeleteShader(_vertexShaderID);
            glDeleteShader(_fragmentShaderID);
            throw ErrorHandle("Error linking Shaders!");
        }

        glDetachShader(_programID, _fragmentShaderID);
        glDetachShader(_programID, _vertexShaderID);
        glDeleteShader(_vertexShaderID);
        glDeleteShader(_fragmentShaderID);
    } catch (ErrorHandle errorHandle) {
        std::cout << errorHandle.what() << std::endl;
        SDL_Quit();
        exit(-1);
    }
}

void GLSLProgram::addAttribute(const std::string &attributeName) {
    glBindAttribLocation(_programID, _numAtterbutes++, attributeName.c_str());
}

void GLSLProgram::use() {
    glUseProgram(_programID);
    for (int i = 0; i < _numAtterbutes; i++) {
        glEnableVertexAttribArray(i);
    }
}

void GLSLProgram::unuse() {
    glUseProgram(0);
    for (int i = 0; i < _numAtterbutes; i++) {
        glDisableVertexAttribArray(i);
    }

}

GLint GLSLProgram::getUiformLocation(const std::string &unifronName) {
    GLint location = glGetUniformLocation(_programID, unifronName.c_str());
    try {
        if (location == GL_INVALID_INDEX) {
            throw ErrorHandle("Uniform name not found in shader");
        }

    } catch (ErrorHandle errorHandle) {
        std::cout << errorHandle.what();
    }
    return (location);
}