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
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

    glfwSetWindowSizeCallback(handle, [](GLFWwindow*, int w, int h) {
        glViewport(0, 0, w, h);
        width = w;
        height = h;
        depth = static_cast<GLfloat>(w);
    });
    glfwMaximizeWindow(handle);
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
    glm::vec3 direction;
    static GLfloat pitch = 0, yaw = -90;
    direction.x = GLfloat(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
    direction.y = GLfloat(sin(glm::radians(pitch)));
    direction.z = GLfloat(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    static glm::vec3 cameraPos     = glm::vec3(0.0f, 0.0f, 500.3f);
    static glm::vec3 cameraFront   = glm::normalize(direction);
    static glm::vec3 cameraUp      = glm::vec3(0.0f, 1.0f,  0.0f);
    static GLdouble deltaTime = 0, lastTime = 0, lastX = GLfloat(width) / 2, lastY = GLfloat(height) / 2;
    static GLfloat fov = 45;

    glfwSetKeyCallback(handle, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        GLfloat cameraSpeed = 100.0f;
        if (mods == GLFW_MOD_CONTROL) cameraSpeed = 500.0f;
        cameraSpeed *= GLfloat(deltaTime);
        if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
            glfwSetWindowShouldClose(window, true);
        }
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            switch (key) {
                case GLFW_KEY_W:
                    cameraPos += cameraSpeed * cameraFront;
                    break;
                case GLFW_KEY_A:
                    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
                    break;
                case GLFW_KEY_S:
                    cameraPos -= cameraSpeed * cameraFront;
                    break;
                case GLFW_KEY_D:
                    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
                    break;
                default:;
            }
        }
    });
    glfwSetCursorPosCallback(handle, [](GLFWwindow*, double xpos, double ypos) {
        static bool firstMouse = true;
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        float xoffset = GLfloat(xpos) - lastX;
        float yoffset = GLfloat(lastY) - ypos; // reversed since y-coordinates range from bottom to top
        lastX = xpos;
        lastY = ypos;

        const float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = GLfloat(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
        direction.y = GLfloat(sin(glm::radians(pitch)));
        direction.z = GLfloat(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
        cameraFront = glm::normalize(direction);
    });
    glfwSetScrollCallback(handle, [](GLFWwindow*, double xoffset, double yoffset) {
        fov -= GLfloat(yoffset);
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f;
    });

    struct Vertex {
        struct { GLfloat x{}, y{}, z{}; } aPos;
        struct { GLfloat u{}, v{}; } aTexCoords;
    };

    GLfloat x = 0, y = 0, z = 0, offset = 25;

    Vertex cubeVertices[] = {
            { x - offset, y - offset, z + offset, 0, 0 },
            { x + offset, y - offset, z + offset, 1, 0 },
            { x - offset, y + offset, z + offset, 0, 1 },
            { x - offset, y + offset, z + offset, 0, 1 },
            { x + offset, y - offset, z + offset, 1, 0 },
            { x + offset, y + offset, z + offset, 1, 1 },

            { x + offset, y - offset, z + offset, 0, 0 },
            { x + offset, y - offset, z - offset, 1, 0 },
            { x + offset, y + offset, z + offset, 0, 1 },
            { x + offset, y + offset, z + offset, 0, 1 },
            { x + offset, y - offset, z - offset, 1, 0 },
            { x + offset, y + offset, z - offset, 1, 1 },

            { x + offset, y - offset, z - offset, 0, 0 },
            { x - offset, y - offset, z - offset, 1, 0 },
            { x + offset, y + offset, z - offset, 0, 1 },
            { x + offset, y + offset, z - offset, 0, 1 },
            { x - offset, y - offset, z - offset, 1, 0 },
            { x - offset, y + offset, z - offset, 1, 1 },

            { x - offset, y - offset, z - offset, 0, 0 },
            { x - offset, y - offset, z + offset, 1, 0 },
            { x - offset, y + offset, z - offset, 0, 1 },
            { x - offset, y + offset, z - offset, 0, 1 },
            { x - offset, y - offset, z + offset, 1, 0 },
            { x - offset, y + offset, z + offset, 1, 1 },

            { x - offset, y + offset, z + offset, 0, 0 },
            { x + offset, y + offset, z + offset, 1, 0 },
            { x - offset, y + offset, z - offset, 0, 1 },
            { x - offset, y + offset, z - offset, 0, 1 },
            { x + offset, y + offset, z + offset, 1, 0 },
            { x + offset, y + offset, z - offset, 1, 1 },

            { x - offset, y - offset, z - offset, 0, 0 },
            { x + offset, y - offset, z - offset, 1, 0 },
            { x - offset, y - offset, z + offset, 0, 1 },
            { x - offset, y - offset, z + offset, 0, 1 },
            { x + offset, y - offset, z - offset, 1, 0 },
            { x + offset, y - offset, z + offset, 1, 1 },
    };
    GLint count = 1331;
    glm::vec3 offsets[count]; GLint index = 0;
    for (int Z = -5; Z <= 5; Z++) {
        for (int Y = -5; Y <= 5; Y++) {
            for (int X = -5; X <= 5; X++) {
                offsets[index++] = glm::vec3 { X * 50, Y * 50, Z * 50 };
            }
        }
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo[2];
    glGenBuffers(2, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof cubeVertices, cubeVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, aPos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, aTexCoords));

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr (sizeof offsets), offsets, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*) offsetof(glm::vec3, x));
    glVertexAttribDivisor(2, 1);


    Shader shader(
        "shaders/vertex.glsl",
        "shaders/fragment.glsl"
    );

    Texture booa("textures/booa.jpg", 0);


    while (!glfwWindowShouldClose(handle)) {
        auto current = glfwGetTime();
        deltaTime = current - lastTime;
        lastTime = current;

        glfwPollEvents();        
	    glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(fov), static_cast<float>(width) / static_cast<float>(height), 0.1f, depth);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 mvp = projection * view/* * model*/;

        shader.use();
        shader.setUniformMat4("u_mvp", glm::value_ptr(mvp));
        shader.setUniform1i("u_texture", 0);
        glDrawArraysInstanced(GL_TRIANGLES, 0, sizeof cubeVertices / sizeof cubeVertices[0], count);

        { ImGuiFrame imGuiFrame;
            ImGui::Begin("Debug"); {
                ImGui::DragFloat("Fov", &fov, 0.05f);
                ImGui::Text("camera (x: %f, y: %f, z: %f)", cameraPos.x, cameraPos.y, cameraPos.z);
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
