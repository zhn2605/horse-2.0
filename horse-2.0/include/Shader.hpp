#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>

class Shader {
public:
    unsigned int shaderProgram;

    // Constructor
    Shader(const char* vertexPath, const char* fragmentPath);

    // Shader operations
    void useProgram();
    void deleteProgram();

    // Uniform setters
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

private:
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif