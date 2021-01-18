#ifndef SHADERS_H
#define SHADERS_H

#include <vector>
#include <GL/glew.h>
#include <fstream>
#include <string>

class Shader {
public:
    ~Shader();
    bool loadProgram(const char* vertex_file, const char* frag_file);
    void use();
    GLuint getAttributeLocation(const char* name);
    GLuint getUniformLocation(const char* name);
private:
    GLuint loadShader(GLenum shaderType, const char* filename);
    bool createProgram();
    std::vector<GLuint> _shaders;
    GLuint _program;
};

#endif
