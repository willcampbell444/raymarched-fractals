#include "shader.h"

#include "options.h"

#include <iostream>

using namespace std::experimental;

Shader::~Shader() {
    glDeleteProgram(_program);
    for (GLuint shader: _shaders) {
        glDeleteShader(shader);
    }
}

bool Shader::loadProgram(const char* vertex_file, const char* frag_file) {
    options.clear();

    GLuint vert = loadShader(GL_VERTEX_SHADER, vertex_file);
    GLuint frag = loadShader(GL_FRAGMENT_SHADER, frag_file);

    GLint status;
    glGetShaderiv(vert, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        std::cout << "Vertex shader failed to compile" << std::endl;
        return false;
    }
    glGetShaderiv(frag, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        std::cout << "Fragment shader failed to compile" << std::endl;
        return false;
    }

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

    _vertFile = vertex_file;
    _fragFile = frag_file;

    return true;
}

GLuint Shader::loadShader(GLenum shaderType, const char* filename) {
    std::ifstream in(filename);

    // preprocess
    std::string line;
    std::stringstream out;
    while (!in.eof()) {
        std::getline(in, line);

        bool replaced = false;
        if (line.size() && line[0] == '#') {
            if (line.find("option") == 1) {
                auto opt = Option::FromString(line.substr(8));
                if (opt) {
                    opt->serialize(out);
                    options.push_back(std::move(opt));
                }
                replaced = true;
            }
        }
        if (!replaced)
            out << line << std::endl;
    }

    std::string shaderContents = out.str();
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

bool Shader::reloadIfNeeded() {
    try {
        for (auto &filename : std::vector<std::string>{_vertFile, _fragFile}) {
            auto file = filesystem::path(filename);
            auto current_file_last_write_time = filesystem::last_write_time(file);
    
            if(!_filetimes.count(file.string())) {
                _filetimes[file.string()] = current_file_last_write_time;
            } else {
                if(_filetimes[file.string()] != current_file_last_write_time) {
                    _filetimes[file.string()] = current_file_last_write_time;
                    loadProgram(_vertFile.c_str(), _fragFile.c_str());
                }
            }
        }
    } catch (...) {
        return false;
    }
    return false;
}
