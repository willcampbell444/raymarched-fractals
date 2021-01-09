
#include <iostream>
#include <chrono>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "shader.h"

const int SCR_WIDTH = 1600;
const int SCR_HEIGHT = 900;

glm::vec3 vel;
glm::vec3 dirVel;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (mods & GLFW_MOD_SHIFT) {
        if (action == GLFW_PRESS) {
            if (key == GLFW_KEY_W) {
                dirVel.z += -1.f;
            } else if (key == GLFW_KEY_S) {
                dirVel.z +=  1.f;
            } else if (key == GLFW_KEY_A) {
                dirVel.x += -1.f;
            } else if (key == GLFW_KEY_D) {
                dirVel.x +=  1.f;
            } else if (key == GLFW_KEY_Q) {
                dirVel.y += -1.f;
            } else if (key == GLFW_KEY_E) {
                dirVel.y +=  1.f;
            }
        }
    } else {
        if (action == GLFW_PRESS) {
            if (key == GLFW_KEY_W) {
                vel.z += -1.f;
            } else if (key == GLFW_KEY_S) {
                vel.z +=  1.f;
            } else if (key == GLFW_KEY_A) {
                vel.x += -1.f;
            } else if (key == GLFW_KEY_D) {
                vel.x += +1.f;
            } else if (key == GLFW_KEY_Q) {
                vel.y += -1.f;
            } else if (key == GLFW_KEY_E) {
                vel.y +=  1.f;
            }
        }
    }
}

int main() {

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 8);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);

    Shader shader;
    if (!shader.loadShader(GL_VERTEX_SHADER, "shaders/main.vert")) {
        std::cout << "Main Vertex Shader Failed To Compile" << std::endl;
        return 0;
    }
    if (!shader.loadShader(GL_FRAGMENT_SHADER, "shaders/main.frag")) {
        std::cout << "Main Fragment Shader Failed To Compile" << std::endl;
        return 0;
    }
    if (!shader.createProgram()) {
        std::cout << "Main Shader Failed To Link" << std::endl;
        return 0;
    }
    shader.use();

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "Error" << std::endl;
    }

    float screenVerticies[30] = {
         1,  1, 0, 1, 1,
        -1, -1, 0, 0, 0,
         1, -1, 0, 1, 0,

         1,  1, 0, 1, 1,
        -1,  1, 0, 0, 1,
        -1, -1, 0, 0, 0
    };

    GLuint screenVAO;
    glGenVertexArrays(1, &screenVAO);
    glBindVertexArray(screenVAO);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*30, screenVerticies, GL_STATIC_DRAW);

    GLuint attrib = shader.getAttributeLocation("position");
    glEnableVertexAttribArray(attrib);
    glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0);

    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "Error" << std::endl;
    }

    glm::vec3 pos = glm::vec3(0.f, 0.f, 8.f);
    glm::vec3 rot = glm::vec3(0.f);
    while (!glfwWindowShouldClose(window)) {
        pos += vel*0.033f;
        rot += dirVel*0.0033f;
        glm::vec3 viewDir = glm::vec3(0.0f, 0.0f, -1.0f);
        viewDir = glm::rotateY(viewDir, rot.x);
        //viewDir = glm::rotateY(viewDir, rot.y);
        viewDir = glm::rotateX(viewDir, rot.z);
        
        glUniform3f(shader.getUniformLocation("cameraPos"), pos.x, pos.y, pos.z);
        glUniform3f(shader.getUniformLocation("viewDir"), viewDir.x, viewDir.y, viewDir.z);

        // draw
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();

        glBindVertexArray(screenVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cout << "Error" << std::endl;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}