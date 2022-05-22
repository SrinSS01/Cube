#include <GL/glew.h>
#include <fstream>
#include <sstream>

#include "App/graphics/Shader.h"

Shader::Shader(const char *vertexSrcPath, const char *fragSrcPath) {
    program = glCreateProgram();
    GLuint vertexShader = compile(GL_VERTEX_SHADER, load(vertexSrcPath));
    GLuint fragShader = compile(GL_FRAGMENT_SHADER, load(fragSrcPath));

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        log_error();
    }
    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (!success) {
        log_error();
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
}

Shader::~Shader() {
    glDeleteProgram(program);
}

GLuint Shader::compile(GLuint type, const std::string& src) {
    GLuint shader = glCreateShader(type);
    const char* srcPtr = src.c_str();
    glShaderSource(shader, 1, &srcPtr, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::string log(len, ' ');
        glGetShaderInfoLog(shader, len, &len, &log[0]);
        glDeleteShader(shader);
        throw std::runtime_error(log);
    }
    return shader;
}

std::string Shader::load(const char *srcPath) {
    std::ifstream file(srcPath);
    std::stringstream buff;
    buff << file.rdbuf();
    return buff.str();
}

void Shader::log_error() const {
    GLint logSize;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
    std::string log(logSize, ' ');
    glGetProgramInfoLog(program, logSize, &logSize, &log[0]);
    glDeleteProgram(program);
    throw std::runtime_error(log);
}

void Shader::setUniformMat4(const char *name, const float *mat) const {
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, mat);
}

void Shader::setUniform1iv(const char *name, int* value, int count) const {
    glUniform1iv(glGetUniformLocation(program, name), count, value);
}

void Shader::setUniform1i(const char *name, int value) const {
    glUniform1i(glGetUniformLocation(program, name), value);
}

void Shader::use() const {
    glUseProgram(program);
}

