#include <iostream>
#include <stdexcept>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "App/App.h"
#include "App/graphics/Shader.h"
#include "App/graphics/Texture.h"

GLint App::width = 640;
GLint App::height = 480;
GLfloat App::depth = static_cast<GLfloat>(width);

App::App() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (handle == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    auto vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(handle, (vidMode->width - width) / 2, (vidMode->height - height) / 2);
    glfwMakeContextCurrent(handle);
    glfwSwapInterval(1);
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLEW");
    }
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSetErrorCallback([](int error_code, const char* description) { std::cerr << "[ERROR] (" << error_code << "): " << description; });
    glfwSetKeyCallback(handle, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
            glfwSetWindowShouldClose(window, true);
        }
    });
    glfwSetWindowSizeCallback(handle, [](GLFWwindow*, int w, int h) {
        glViewport(0, 0, w, h);
        width = w;
        height = h;
        depth = static_cast<GLfloat>(w);
    });
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(handle, true);
    ImGui_ImplOpenGL3_Init("#version 460 core");
}

App::~App() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwHideWindow(handle);
    glfwSetErrorCallback(nullptr);
    glfwSetKeyCallback(handle, nullptr);
    glfwSetWindowSizeCallback(handle, nullptr);
    glfwDestroyWindow(handle);
    glfwTerminate();
}

void App::run() {
    GLfloat x = 0, y = 0, z = 0, offset = 50;
    GLint count[6] = { 4, 4, 4, 4, 4, 4 };
    GLint starting[6] = { 0, 28, 56, 84, 112, 140 };
    GLfloat cubeVertices[] = {
        x-offset, y-offset, z+offset,       1, 0, 0, 1,
        x+offset, y-offset, z+offset,       1, 0, 0, 1,
        x-offset, y+offset, z+offset,       1, 0, 0, 1,
        x+offset, y+offset, z+offset,       1, 0, 0, 1,

        x+offset, y-offset, z-offset,       0, 1, 0, 1,
        x-offset, y-offset, z-offset,       0, 1, 0, 1,
        x+offset, y+offset, z-offset,       0, 1, 0, 1,
        x-offset, y+offset, z-offset,       0, 1, 0, 1,

        x-offset, y-offset, z-offset,       0, 0, 1, 1,
        x-offset, y-offset, z+offset,       0, 0, 1, 1,
        x-offset, y+offset, z-offset,       0, 0, 1, 1,
        x-offset, y+offset, z+offset,       0, 0, 1, 1,

        x+offset, y-offset, z+offset,       1, 1, 0, 1,
        x+offset, y-offset, z-offset,       1, 1, 0, 1,
        x+offset, y+offset, z+offset,       1, 1, 0, 1,
        x+offset, y+offset, z-offset,       1, 1, 0, 1,

        x-offset, y+offset, z+offset,       1, 1, 1, 1,
        x+offset, y+offset, z+offset,       1, 1, 1, 1,
        x-offset, y+offset, z-offset,       1, 1, 1, 1,
        x+offset, y+offset, z-offset,       1, 1, 1, 1,

        x-offset, y-offset, z-offset,       1, 1, 1, 1,
        x+offset, y-offset, z-offset,       1, 1, 1, 1,
        x-offset, y-offset, z+offset,       1, 1, 1, 1,
        x+offset, y-offset, z+offset,       1, 1, 1, 1
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof cubeVertices, cubeVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    Shader shader(
        "shaders/vertex.glsl",
        "shaders/fragment.glsl"
    );

    Texture booa("textures/atlas.png", 0);

    GLfloat fov = 45;
    GLfloat rotateAngle[] = { 0, 0 };
    GLfloat translateXYZ[] = { 0, 0, -200 };
    GLfloat scaleXYZ[] = { 1 };

    while (!glfwWindowShouldClose(handle)) {
        glfwPollEvents();        
	    glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(fov), static_cast<float>(width) / static_cast<float>(height), 0.1f, depth);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(translateXYZ[0], translateXYZ[1], translateXYZ[2]));
        model = glm::rotate(model, glm::radians(rotateAngle[0]), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(static_cast<GLfloat>(glfwGetTime() * 100)), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(rotateAngle[1]), glm::vec3(0, 0, 1));
        model = glm::scale(model, glm::vec3(scaleXYZ[0], scaleXYZ[0], scaleXYZ[0]));

        glm::mat4 mvp = projection * model;

        shader.use();
        shader.setUniformMat4("u_mvp", glm::value_ptr(mvp));
//        glDrawArrays(GL_TRIANGLE_STRIP, 0, 24);
        glMultiDrawArrays(GL_TRIANGLE_STRIP, starting, count, 6);

        { ImGuiFrame imGuiFrame;
            ImGui::Begin("Debug"); {
                ImGui::DragFloat("Fov", &fov, 0.05f);
                ImGui::DragFloat2("Rotate", rotateAngle, 0.05f);
                ImGui::DragFloat3("Translate", translateXYZ, 0.05f);
                ImGui::DragFloat("Scale", scaleXYZ, 0.05f);
            } ImGui::End();
        }   
        glfwSwapBuffers(handle);
    }
}

App::ImGuiFrame::ImGuiFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

App::ImGuiFrame::~ImGuiFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
