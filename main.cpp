#include "include/glinclude.h"
#include "include/input.hpp"
#include "include/shader.hpp"
#include "include/buffers.hpp"

#include <iostream>
#include <algorithm>

int main(int argc, char *argv[]) {
    if (argc != 5) {
        return std::cerr << "WRONG NUMBER OF ARGUMENTS" << std::endl, -1;
    }
    extern int width, height;
    width = std::stoi(argv[1]), height = std::stoi(argv[2]);
    extern float scale;
    scale = std::stof(argv[3]);
    float step = std::stof(argv[4]);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(width, height, "Present", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return std::cerr << "FAILED TO INIT GLFW" << std::endl, -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return std::cerr << "FAILED TO INIT GLAD" << std::endl, -1;
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *_, int width, int height) {
        glViewport(0, 0, width, height);
    });

    std::sort(obj.begin(), obj.end(), [](epicycle a, epicycle b) {
        return a.r > b.r;
    });

    shaderInit();
    planeInit(); lineInit();

    planeShader.enable();
    planeShader.setVec2("resolution", width, height);
    planeShader.setFloat("scale", scale);
    planeShader.setInt("n", obj.size());
    for (int i = 0; i < obj.size(); ++i) {
        planeShader.setFloat("r[" + std::to_string(i) + "]", obj[i].r);
    }
    
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        render(step);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}