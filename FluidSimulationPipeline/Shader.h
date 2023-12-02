#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>


class Shader
{
public:
    // the program ID
    unsigned int ShaderProgramID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    Shader() { ShaderProgramID = 0; }
    ~Shader();
    
protected:
    //checks the shader compiler output for errors
    void CheckCompileErrors(GLuint shader, std::string type);

public:
    // use/activate the shader
    void Use();
    // utility uniform functions
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec2(const std::string& name, const glm::vec2& vec) const;
    void SetIVec2(const std::string& name, const glm::ivec2& vec) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;
};

#endif