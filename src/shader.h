#ifndef SHADERS_H
#define SHADERS_H

#include <vector>
#include <GL/glew.h>
#include <fstream>
#include <string>
#include <unordered_map>
#include <experimental/filesystem>

class Option;

class Shader {
public:
    ~Shader();
    bool loadProgram(const char* vertex_file, const char* frag_file);
    void use();
    GLuint getAttributeLocation(const char* name);
    GLuint getUniformLocation(const char* name);
    bool reloadIfNeeded();

    std::vector<std::unique_ptr<Option>> options;
private:
    GLuint loadShader(GLenum shaderType, const char* filename);
    bool createProgram();

    std::vector<GLuint> _shaders;
    GLuint _program;

    std::string _vertFile, _fragFile;
    std::unordered_map<std::string, std::experimental::filesystem::file_time_type> _filetimes;
};

#endif
