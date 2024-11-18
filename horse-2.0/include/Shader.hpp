#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>

class Shader {
public:
    GLuint shaderProgram;

    // Constructor
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    // Shader operations
    void useProgram();
    void deleteProgram();

    // Uniform setters
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

    // Uniform getters
    GLint getUniformLocation(const std::string& name);

private:
    void checkCompileErrors(GLuint shader, std::string type);
};

#endif