#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class App {
public:
    App();
    ~App();
    void run();

private:
    GLFWwindow* handle;
    char const* title = "Cube";
    static GLint width, height;
    static GLfloat depth;

private:
    class ImGuiFrame {
    public:
        ImGuiFrame();
        ~ImGuiFrame();
    };
};