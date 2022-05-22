#pragma once
#include <string>

class Shader {
public:
    Shader(char const* vertexSrcPath, char const* fragSrcPath);
    ~Shader();
    void setUniformMat4(const char *name, const float *mat) const;
    void setUniform1iv(const char *name, int* value, int count) const;    
    void setUniform1i(const char *name, int value) const;
    void use() const;

private:
    static unsigned int compile(unsigned int type, const std::string& src);
    static std::string load(char const* srcPath);
    void log_error() const;

private:
    unsigned int program;
};