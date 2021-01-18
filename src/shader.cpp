#include "shader.h"

#include <iostream>

Shader::~Shader() {
    glDeleteProgram(_program);
    for (GLuint shader: _shaders) {
        glDeleteShader(shader);
    }
}

bool Shader::loadProgram(const char* vertex_file, const char* frag_file) {
    GLuint vert = loadShader(GL_VERTEX_SHADER, vertex_file);
    GLuint frag = loadShader(GL_FRAGMENT_SHADER, frag_file);

    GLint status;
    glGetShaderiv(vert, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) return false;
    glGetShaderiv(frag, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) return false;

    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);

    glBindFragDataLocation(program, 0, "outColor");
    glLinkProgram(program);

    GLint isLinked;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (isLinked != GL_TRUE) return false;

    if (_shaders.size()) {
        glDeleteProgram(_program);
        for (GLuint shader: _shaders) {
            glDeleteShader(shader);
        }
        _shaders.clear();
    }
    _shaders.push_back(vert);
    _shaders.push_back(frag);
    _program = program;

    return true;
}

GLuint Shader::loadShader(GLenum shaderType, const char* filename) {
    std::ifstream inVertex(filename);
    std::string shaderContents((std::istreambuf_iterator<char>(inVertex)), std::istreambuf_iterator<char>());
    const char* shaderSource = shaderContents.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    return shader;
}

bool Shader::createProgram() {
    _program = glCreateProgram();

    for (GLuint shader: _shaders) {
        glAttachShader(_program, shader);
    }

    glBindFragDataLocation(_program, 0, "outColor");
    glLinkProgram(_program);

    GLint isLinked;
    glGetProgramiv(_program, GL_LINK_STATUS, &isLinked);
    return isLinked == GL_TRUE;
}

void Shader::use() {
    glUseProgram(_program);
}

GLuint Shader::getAttributeLocation(const char* name) {
    return glGetAttribLocation(_program, name);
}

GLuint Shader::getUniformLocation(const char* name) {
    return glGetUniformLocation(_program, name);
}
