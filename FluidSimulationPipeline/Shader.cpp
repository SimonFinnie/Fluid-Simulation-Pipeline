#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>


Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    //file name
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // add exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
        catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    //compile shaders
    unsigned int vertex, fragment;

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    CheckCompileErrors(vertex, "VERTEX");


    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    CheckCompileErrors(vertex, "FRAGMENT");


    // shader Program
    ShaderProgramID = glCreateProgram();
    glAttachShader(ShaderProgramID, vertex);
    glAttachShader(ShaderProgramID, fragment);
    glLinkProgram(ShaderProgramID);
    CheckCompileErrors(ShaderProgramID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

Shader::~Shader() {
    glDeleteProgram(ShaderProgramID);
}



void Shader::CheckCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM"){
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else{
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success){
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}



void Shader::Use()
{
    glUseProgram(ShaderProgramID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ShaderProgramID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ShaderProgramID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ShaderProgramID, name.c_str()), value);
}


void Shader::SetVec2(const std::string& name, const glm::vec2& vec) const
{
    glUniform2fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, &vec[0]);
}

void Shader::SetIVec2(const std::string& name, const glm::ivec2& vec) const
{
    glUniform2iv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, &vec[0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}